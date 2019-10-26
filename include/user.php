<?php

class userdef {

    var $UserId;
    var $UserName;
    var $UserDesc;
    var $usergroups;
    var $UserId_Process;
    var $Admin;
    var $Active;
    var $Validado;
    var $connect;
    var $AdHoc;
    var $Ausergroups;
    var $UserLevel;
    var $Origem;
    var $Direto;
    var $ASP_session;
    var $AGrupos;
    var $CriarSessionIntegration;
    var $lastlogon;
    var $lastnofification;
    var $lastmessages;
    var $Email;

    // Cria o Usuario

    function CriaSessionIntegration()
    {
        include(FILES_ROOT . "include/config.config.php");
        $session_id = session_id();
        $phpSession = md5($session_id);

        if (!isset($ALTERNATIVE_TABLE_userlogins)) {
            $_TABLE_userlogins = "userlogins";
        } else {
            $_TABLE_userlogins = $ALTERNATIVE_TABLE_userlogins;
        }

        if (!isset($ALTERNATIVE_SERVER_userlogins)) {
            $connect = AtivaDBExterno(true);
        } else {
            $SERVER = $ALTERNATIVE_SERVER_userlogins;
            $USER = $ALTERNATIVE_USER_userlogins;
            $PASSWORD = $ALTERNATIVE_PASSWORD_userlogins;
            $DB = $ALTERNATIVE_DB_userlogins;
            $connect = AtivaOutroDB($SERVER, $USER, $PASSWORD, $DB);
        }

        /**
         *  Remove a Sess�o se Existe na Tabela
         */
        $SQL = " delete from $_TABLE_userlogins where UserId = $this->UserId or token = '$this->ASP_session' or SessionId = $phpSession";
        $Result = mysqli_query($connect, $SQL);

        /**
         *  Cria o registro da Sess�o
         */
        $IP = $_SERVER['REMOTE_ADDR'];
        $Hora = date('Y-m-d g:i:s');
        $SQL = " insert into $_TABLE_userlogins
		(SessionId, UserId, IpAddress, LogonTimeStamp, token) 
		values 
		('$phpSession', 
		$this->UserId, '$IP','$Hora', '$this->ASP_session')";
        $Result = mysqli_query($connect, $SQL);
        if (!$Result) {
            error_log("Erro Gravando session\n" . $SQL . " \nErro: " . mysqli_error($connect) . "\n" . var_export($connect, true));
        }
        AtivaDBProcess();
    }

    function Create($OrigemLogon, $connect, $AuthUser = 0, $CriarSessionIntegration = false)
    {
        global $ASP_session;
//        global $ASP_session, $OrigemLogon;
        $this->connect = $connect;
        $this->Validado = false;
        $this->UserLevel = 1;
        $this->UserId_Process = -1;
        $this->CriarSessionIntegration = $CriarSessionIntegration;

        if ($OrigemLogon == NULL) {
            $OrigemLogon = "ProcessLogon";
        }


        switch ($OrigemLogon) {
            case "Server":
                $this->Server($AuthUser);
                break;
            case "ProcessLogon":
                $this->ProcessLogon($AuthUser);
                if ($this->Validado & $this->CriarSessionIntegration) {
                    $this->CriaSessionIntegration();
                }
                break;

            // Logon by token
            case "token":
                $this->tokenLogon($AuthUser);
                if ($this->Validado & $this->CriarSessionIntegration) {
                    $this->CriaSessionIntegration();
                }
                break;
            case "External":
                $this->External($AuthUser);
                if ($this->Validado && $this->CriarSessionIntegration && !empty($ASP_session)) {
                    $this->CriaSessionIntegration();
                }
                break;

            default:
                break;
        }
        if ($this->Validado) {
            $this->Eadmin();
        }
    }

    function PegaOrigemUsuario()
    {
        global $connect, $userdef;
        $SQL = "select Origem from origemgrupos where Grupo in ($userdef->usergroups)";
        $Query = mysqli_query($connect, $SQL);
        $Result = mysqli_fetch_array($Query);
        return $Result["Origem"];
    }

    // Verifica se � Admin
    function Eadmin()
    {
        $this->Admin = false;
        $NumRows = 0;
        $SQL = "";
        $SQL .= " (select distinct ProcId from procadmins where procadmins.groupid  in ($this->usergroups) and GrpFld = 'G') ";
        $SQL .= " union ";
        $SQL .= " (select distinct ProcId from procadmins where procadmins.groupid = $this->UserId_Process  and GrpFld = 'U') ";
        $SQL .= " union ";
        $SQL .= " (select distinct ProcId from stepadmins where stepadmins.groupid in ($this->usergroups)  and GrpFld = 'G') ";
        $SQL .= " union ";
        $SQL .= " (select distinct ProcId from stepadmins where stepadmins.groupid = $this->UserId_Process  and GrpFld = 'U') ";
        $SQL .= " union ";
        $SQL .= " (select distinct ProcId from geraladmins where geraladmins.groupid in ($this->usergroups) and GrpFld = 'G') ";
        $SQL .= " union ";
        $SQL .= " (select distinct ProcId from geraladmins where geraladmins.groupid = $this->UserId_Process and GrpFld = 'U') ";
        $Query = mysqli_query($this->connect, $SQL);
        //echo $SQL;
        $NumRows = mysqli_num_rows($Query);
        $this->Admin = ($NumRows > 0);
    }

    function PegaUserId()
    {
        $SQL = "select UserId from userdef where UserName = '$this->UserName'";
        $Query = mysqli_query($this->connect, $SQL);
        $Result = mysqli_fetch_array($Query);
        if (!empty($Result["UserId"]))
            $this->UserId_Process = $Result["UserId"];
    }

    function Server($AuthUser)
    {
        global $_SERVER, $auth_user, $auth_pass, $_POST;
        require_once("ldap.php");
        if (is_array($AuthUser)) {
            $auth_user = $AuthUser["UserName"];
            $auth_pass = $AuthUser["UserPassword"];
        } else {
            $auth_user = $_POST["UserName"];
            $auth_pass = $_POST["UserPwd"];
        }
        /* 		if (isset($_SERVER["AUTH_USER"]))
          {
          $UserName = basename($_SERVER["AUTH_USER"]);
          }
          if (isset($_SERVER["LOGON_USER"]))
          {
          $UserName = basename($_SERVER["LOGON_USER"]);
          } */
        if ($config["AD"] = 1) {
            $UserAD = PegaDadosADRM($auth_user, $auth_pass);
//			$UserAD = PegaDadosAD($auth_user);
        }
        $this->UserId = -1;
        $this->UserDesc = $UserAD["UserDesc"];
        $this->UserLevel = 1;
        if (empty($UserAD["Grupos"])) {
            $UserAD["Grupos"] = '-1';
        }
        $this->usergroups = $UserAD["Grupos"];
        $this->Ausergroups = $UserAD["AGrupos"];
        $this->UserName = $_POST["UserName"];
        $this->PegaUserId();
        $this->AdHoc = $this->UserCriaAdHoc();
        $this->ASP_session = session_id();
        $this->Origem = '';
    }

    function SalvaDataLogon()
    {
        $SQL = "update userdef set lastlogon = now() where userid = $this->UserId";
        $QUERY_USER = mysqli_query($this->connect, $SQL);
    }

    function tokenLogon($token)
    {
        global $_POST;
        $this->Validado = false;

        $SQL = "SELECT UserId, UserName, UserPwd, UserLevel, UserDesc, Email, Uactive, CustonGate, AdHoc, Origem, lastlogon, lastnotification, lastmessages, token 
		from userdef where UActive=1";
        $QUERY_USER = mysqli_query($this->connect, $SQL);
        if (!$QUERY_USER) {
            $this->Validado = false;
            return;
        }
        while ($result = mysqli_fetch_array($QUERY_USER)) {
            //error_log("Usuario Log: " . $result["UserName"]);
            if ($result["token"] == $token) {
                $this->UserName = $result["UserName"];
                $this->UserPwd = $result["UserPwd"];
                $this->UserId = $result["UserId"];
                $this->UserId_Process = $result["UserId"];
                $this->UserLevel = $result["UserLevel"];
                $this->UserDesc = $result["UserDesc"];
                $this->Active = $result["Uactive"];
                $this->EMail = $result["Email"];
                $this->CustonGate = $result["CustonGate"];
                $this->AdHoc = $result["AdHoc"];
                $this->Origem = $result["Origem"];
                $this->GruposProcess();
                $this->Validado = true;
                $this->Email = $result["Email"];
                $this->lastlogon = (!empty($result["lastlogon"])) ? $result["lastlogon"] : '1901-01-01';
                $this->lastnotification = (!empty($result["lastnotification"])) ? $result["lastnotification"] : '1901-01-01';
                $this->lastmessages = (!empty($result["lastmessages"])) ? $result["lastmessages"] : '1901-01-01';
                $this->SalvaDataLogon();
                return;
            }
        }
        return;
    }

    function ProcessLogon($AuthUser)
    {
        global $_POST;
        $this->Validado = false;
        if (isset($AuthUser["UserName"])) {
            $UserName = $AuthUser["UserName"];
            $UserPwd = $AuthUser["UserPassword"];
        } else {
            return; // Se não tiver nome, invalida o usuário
        }

        $SQL = "SELECT UserId, UserName, UserPwd, UserLevel, UserDesc, Email, Uactive, CustonGate, AdHoc, Origem, lastlogon, lastnotification, lastmessages 
		from userdef where UActive=1";
        //error_log($SQL);
        $QUERY_USER = mysqli_query($this->connect, $SQL);
        if (!$QUERY_USER) {
            $this->Validado = false;
            return;
        }
        while ($result = mysqli_fetch_array($QUERY_USER)) {
            //error_log("Usuario Log: " . $result["UserName"]);
            if (($result["UserName"] !== $UserName)) {
                continue;
            }
            $dbUserPwd = $result["UserPwd"];
            if (!password_verify($UserPwd, $dbUserPwd)) {
                if (($dbUserPwd !== $UserPwd)) {
                    return;
                }
            }
            $this->UserName = $result["UserName"];
//                $this->UserPwd = $result["UserPwd"];
            $this->UserId = $result["UserId"];
            $this->UserId_Process = $result["UserId"];
            $this->UserLevel = $result["UserLevel"];
            $this->UserDesc = $result["UserDesc"];
            $this->Active = $result["Uactive"];
            $this->EMail = $result["Email"];
            $this->CustonGate = $result["CustonGate"];
            $this->AdHoc = $result["AdHoc"];
            $this->Origem = $result["Origem"];
            $this->GruposProcess();
            $this->Validado = true;
            $this->Email = $result["Email"];
            $this->lastlogon = (!empty($result["lastlogon"])) ? $result["lastlogon"] : '1901-01-01';
            $this->lastnotification = (!empty($result["lastnotification"])) ? $result["lastnotification"] : '1901-01-01';
            $this->lastmessages = (!empty($result["lastmessages"])) ? $result["lastmessages"] : '1901-01-01';
            $this->SalvaDataLogon();
            return;
        }
    }

    function GruposProcess()
    {
        $SQL = "select GroupId from usergroup where UserId = $this->UserId";
        $Query = mysqli_query($this->connect, $SQL);
        if (!$Query) {
            $this->Validado = false;
            //error_log("Dump\n" .var_export(debug_backtrace(), true), 0);
            //error_log("Query\n $SQL" , 0);
            AtivaDBProcess();
            return;
        }
        $i = 0;

        while ($Result = mysqli_fetch_array($Query)) {
            $Grupos[$i] = "'" . $Result["GroupId"] . "'";
            $i++;
        }
        if ($i > 0) {
            $this->usergroups = implode(',', $Grupos);
        } else {
            $this->usergroups = '-1';
        }
    }

    function External($AuthUser)
    {
        global $ASP_session, $connectDBExterno;
        $UserName = $AuthUser["UserName"];
        $UserPassword = $AuthUser["UserPassword"];
        $UserPassword = md5($AuthUser["UserPassword"]);
        include("include/config.config.php");
        // Cria a Conexao ao banco externo
        $connectDBExterno = AtivaDbExterno();
        $this->ASP_session = $ASP_session;
        if (empty($ASP_session)) {
            $SQL = "select $EXTERNAL_USERID as UserId, Origem from $EXTERNAL_userdef where $EXTERNAL_USERNAME = '$UserName' and $EXTERNAL_PASSWORD = '$UserPassword' ";
            if (!empty($EXTERNAL_FILTER)) {
                $SQL .= " and $EXTERNAL_FILTER";
            }
            $query = mysqli_query($connectDBExterno, $SQL);
            if (!$query) {
                $this->Validado = false;
                //error_log("Dump\n" . var_export(debug_backtrace(), true), 0);
                //error_log("Query\n $SQL" , 0);
                AtivaDBProcess();
                return;
            }
            $Result = mysqli_fetch_array($query);
            $UserId = $Result["UserId"];
            $this->Origem = $Result['Origem'];
        } else {
            $this->CriarSessionIntegration = false;
            // Pega Id do Usuario logado no Portal
            if (empty($ALTERNATIVE_TABLE_userlogins)) {
                $TABLE_userlogins = "userlogins";
            } else {
                $TABLE_userlogins = $ALTERNATIVE_TABLE_userlogins;
            }

            if ($ALTERNATIVE_SERVER_userlogins) {
                $connect = AtivaOutroDB($ALTERNATIVE_SERVER_userlogins, $ALTERNATIVE_USER_userlogins, $ALTERNATIVE_PASSWORD_userlogins, $ALTERNATIVE_DB_userlogins);
            } else {
                $connect = $connectDBExterno;
            }
            $SQL = "select $EXTERNAL_USERID as UserId from $TABLE_userlogins where SessionID = '$ASP_session'";
            $Query = mysqli_query($connect, $SQL);
            $Result = mysqli_fetch_array($Query);
            $UserId = $Result["UserId"];
            if (!is_int($UserId)) {
                $this->Validado = false;
                AtivaDBProcess();
                return;
            }
            $connectDBExterno = AtivaDBExterno(true);
            $SQL = "select $EXTERNAL_USERID as UserId, Origem from $EXTERNAL_userdef where $EXTERNAL_USERID = $UserId";
            $Query = mysqli_query($connect, $SQL);
            if (!$Query) {
                $this->Validado = false;
                //error_log("Dump\n" .var_export(debug_backtrace(), true), 0);
                //error_log("Query\n $SQL" , 0);
                AtivaDBProcess();
                return;
            }
            $Result = mysqli_fetch_array($Query);
            $UserId = $Result["UserId"];
            $this->Origem = $Result['Origem'];
        }
        $this->Direto = True;
        if (substr($this->Origem, 0, 2) == 'TC') {
            $this->Direto = false;
        }

        if (empty($UserId)) {
            //error_log("Usu�rio n�o autenticado: $UserName", 0);
            $this->Validado = false;
            AtivaDBProcess();
            return;
        }

        // Dados do Usuario
        $SQL = "select * from $EXTERNAL_userdef where $EXTERNAL_USERID = $UserId";
        $Query = mysqli_query($connectDBExterno, $SQL);
        if (!$Query) {
            $this->Validado = false;
            //error_log("Dump\n" .var_export(debug_backtrace(), true), 0);
            //error_log("Query\n $SQL" , 0);
            AtivaDBProcess();
            return;
        }

        $Result = mysqli_fetch_array($Query);
        $this->UserId = $UserId;
        $this->UserId_Process = $UserId;
        $this->UserName = $Result[$EXTERNAL_USERNAME];
        $this->UserDesc = $Result[$EXTERNAL_USERFULLNAME];
        $this->UserDesc = str_replace("'", "''", $this->UserDesc);

        if (isset($Result["IsActive"])) {
            $this->Active = $Result["IsActive"];
        } else {
            $this->Active = true;
        }

        if (isset($Result["HasAdHoc"])) {
            $this->AdHoc = $Result["HasAdHoc"];
        } else {
            $this->AdHoc = false;
        }



        $SQL = "select $EXTERNAL_GROUPID as GroupId from $EXTERNAL_USERSGROUPS where $EXTERNAL_USERID = $UserId";
        $Query = mysqli_query($connectDBExterno, $SQL);
        if (!$Query) {
            $this->Validado = false;
            //error_log("Dump\n" .var_export(debug_backtrace(), true), 0);
            //error_log("Query\n $SQL" , 0);
            AtivaDBProcess();
            return;
        }
        $i = 0;
        while ($Result = mysqli_fetch_array($Query)) {
            $Grupos[$i] = "'" . $Result["GroupId"] . "'";
            $i++;
        }
        $Grupos[$i] = "'-1'";
        if ($i > 0) {
            $this->usergroups = implode(',', $Grupos);
        } else {
            $this->usergroups = "'-1'";
        }
        AtivaDBProcess();
    }

    function UserCriaAdHoc()
    {
        global $connect, $config, $userdef;
        $AdHoc = false;
        if (is_array($userdef->AGrupos)) {
            $AdHoc = in_array($config["AdHocAd"], $userdef->AGrupos);
        }
        return $AdHoc;
    }

}

?>