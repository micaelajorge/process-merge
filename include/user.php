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
    var $startLogon;

    // Cria o Usuario

    function CriaSessionIntegration()
    {
//        include(FILES_ROOT . "include/config.config.php");
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
        $SQL = " delete from $_TABLE_userlogins where UserId = $this->UserId or token = '$this->ASP_session' or SessionId = '$phpSession'";
        $ResultDelete = mysqli_query($connect, $SQL);

        if (!$ResultDelete) {
            error_log("Erro Gravando session\n" . $SQL . " \nErro: " . mysqli_error($connect) . "\n" . var_export($connect, true));
        }
        
        
        /**
         *  Cria o registro da Sess�o
         */
        $IP = $_SERVER['REMOTE_ADDR'];
        $Hora = date('Y-m-d H:i:s');
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

    /**
     * Verifica se o usuário deve fazer a troca de senha
     * 
     * @param type $dataUltimaTroca
     * @return boolean
     */
    function testaStartLogon($dataUltimaTroca)
    {
        if ($dataUltimaTroca == null) {
            return true & APLICAR_POLITICAS_SEGURANCA;
        }

        $dataUltimaTroca = date_create($dataUltimaTroca);
        $dataAtual = new DateTime();
        $interval = date_diff($dataAtual, $dataUltimaTroca);
        $diasDecorridos = $interval->format("%a");
        $configPoliticasSeguranca = pegaDadosConfig();
        if (key_exists("diasTrocaSenha", $configPoliticasSeguranca)) {
            $diasTrocaSenha = $configPoliticasSeguranca["diasTrocaSenha"];
        } else {
            $diasTrocaSenha = LIMITE_DIAS_TROCA_SENHA;
        }

        $exigeTrocaSenha = $diasDecorridos > $diasTrocaSenha;
        $exigeTrocaSenha = $exigeTrocaSenha & APLICAR_POLITICAS_SEGURANCA;

        return $exigeTrocaSenha;
    }

    function testaUltimoLogon($dataUltimoLogon)
    {
        $ultimologon = date_create($dataUltimoLogon);
        $dataAtual = new DateTime();
        $interval = date_diff($dataAtual, $ultimologon);
        $diasDecorridos = $interval->format("%a");

        $configPoliticasSeguranca = pegaDadosConfig();
        if (key_exists("mesesInatividade", $configPoliticasSeguranca)) {
            $mesesInatividade = $configPoliticasSeguranca["mesesInatividade"];
        } else {
            $mesesInatividade = LIMITE_DIAS_ULTIMO_LOGON;
        }

        $usuarioDesabilitadoPorInatividade = $diasDecorridos < $mesesInatividade;
        $usuarioDesabilitadoPorInatividade = $usuarioDesabilitadoPorInatividade & APLICAR_POLITICAS_SEGURANCA;
        
        return $usuarioDesabilitadoPorInatividade;
    }

    function SalvaDataLogon()
    {
        $SQL = "update userdef set lastlogon = now(), loginsFailed = 0 where userid = $this->UserId";
        $QUERY_USER = mysqli_query($this->connect, $SQL);
    }

    function tokenLogon($token)
    {
        global $_POST;
        $this->Validado = false;

        $SQL = "SELECT UserId, UserName, UserPwd, UserLevel, UserDesc, Email, Uactive, CustonGate, AdHoc, Origem, lastlogon, lastnotification, lastmessages, token 
		from userdef where UActive = 1";
        $QUERY_USER = mysqli_query($this->connect, $SQL);
        if (!$QUERY_USER) {
            $this->Validado = false;
            return;
        }
        while ($result = mysqli_fetch_array($QUERY_USER)) {
            //error_log("Usuario Log: " . $result["UserName"]);
            if ($result["token"] == $token) {
                $validado = $result["Uactive"] == '1';

                // Desativado teste de ultimo logon para não travar o automato
//                $validado = $validado && $this->testaUltimoLogon($result["lastlogon"]);
                $this->Validado = $validado;
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
                $this->Active = $validado;
                $this->Email = $result["Email"];
                $this->lastlogon = (!empty($result["lastlogon"])) ? $result["lastlogon"] : '1901-01-01';
                $this->lastnotification = (!empty($result["lastnotification"])) ? $result["lastnotification"] : '1901-01-01';
                $this->lastmessages = (!empty($result["lastmessages"])) ? $result["lastmessages"] : '1901-01-01';
                if ($validado) {
                    $this->SalvaDataLogon();
                }
                return;
            }
        }
        return;
    }

    function verificaTotalTentativasLogon($userId, $userName)
    {
        $SQL = "select loginsFailed from userdef where userId = $userId";
        $Query = mysqli_query($this->connect, $SQL);
        $linhas = mysqli_fetch_all($Query, MYSQLI_ASSOC);


        $configs = pegaDadosConfig();

        $max_falhas_logon = ($configs["maxFalhaLogon"]) ? $configs["maxFalhaLogon"] : DEFAULT_MAX_FALHAS_LOGON;


        if ($linhas[0]["loginsFailed"] > $max_falhas_logon) {
            $SQL = "update userdef set active = 0 where userId = $userId and loginsFailed = 0";
            $Query = mysqli_query($this->connect, $SQL);
            insereEntradaAuditTrail(0, 0, 0, $userId, $userName, 904, 'Usuário desativado por excesso de tentativas de logon');
            return false;
        } else {
            insereEntradaAuditTrail(0, 0, 0, $userId, $userName, 901, 'Tentativa de logon sem sucesso');
            return true;
        }
    }

    function AcrescentaTentativasLogon($userId)
    {
        $SQL = "SHOW COLUMNS FROM `userdef` LIKE 'loginsFailed' int default 0";
        $Query = mysqli_query($this->connect, $SQL);

        $Lista = mysqli_fetch_all($Query);

        if (count($Lista === 0)) {
            $SQL = "alter table `userdef` add column loginsFailed int ";
            $Query = mysqli_query($this->connect, $SQL);
        }

        $SQL = "update userdef set loginsFailed = loginsFailed + 1 where userId = $userId";
        $Query = mysqli_query($this->connect, $SQL);
    }

    function ProcessLogon_BuscaUsuario($userName, $password, $connect = false)
    {

        $SQL = "SELECT * from userdef where UActive=1";
//        $SQL = "SELECT UserId, UserName, UserPwd, UserLevel, UserDesc, Email, Uactive, CustonGate, AdHoc, Origem, lastlogon, lastnotification, lastmessages, lastpasswords, tokenEmail 
//		from userdef where UActive=1";
        //error_log($SQL);

        if (!$connect) {
            $connect = $this->connect;
        }
        $QUERY_USER = mysqli_query($connect, $SQL);
        if (!$QUERY_USER) {
            console . log("Erro na busca de usuários:" . mysqli_error($connect));
            $this->Validado = false;
            return;
        }

        while ($result = mysqli_fetch_array($QUERY_USER)) {
            //error_log("Usuario Log: " . $result["UserName"]);
            if (($result["UserName"] !== $userName)) {
                continue;
            }
            $dbUserPwd = $result["UserPwd"];

            // Verifica a senha criptografada
            if (!password_verify($password, $dbUserPwd)) {

                // Verifica a senha em plain text
                if (($password !== $dbUserPwd)) {

                    // Acrescenta um erro ao Login                    
                    $this->AcrescentaTentativasLogon($result["UserId"]);

                    $this->verificaTotalTentativasLogon($result["UserId"], $userName);
                    error_log("Falha Logon $userName");
                    return false;
                }
            }
            return $result;
        }
        insereEntradaAuditTrail(0, 0, 0, 0, $userName, 902, "Tentativa de logon do usuário $userName");
        return false;
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

        $result = $this->ProcessLogon_BuscaUsuario($UserName, $UserPwd);

        if ($result) {
            $usuarioAtivo = $result["UActive"] == '1';


            $this->UserName = $result["UserName"];
            $this->UserPwd = $result["UserPwd"];
            $this->UserId = $result["UserId"];
            $this->UserId_Process = $result["UserId"];
            $this->UserLevel = $result["UserLevel"];
            $this->UserDesc = $result["UserDesc"];
            $this->Active = $result["UActive"];
            $this->EMail = $result["Email"];
            $this->CustonGate = $result["CustonGate"];
            $this->AdHoc = $result["AdHoc"];
            $this->Origem = $result["Origem"];
            $this->GruposProcess();
            $this->Active = $validado;
            $this->Email = $result["Email"];
            $this->lastlogon = (!empty($result["lastlogon"])) ? $result["lastlogon"] : '1901-01-01';
            $this->lastnotification = (!empty($result["lastnotification"])) ? $result["lastnotification"] : '1901-01-01';
            $this->lastmessages = (!empty($result["lastmessages"])) ? $result["lastmessages"] : '1901-01-01';
            $this->CriarSessionIntegration = true;
                        
            // Verifica se o usuário é um usuário de sistema
            if (key_exists("systemuser", $result)) {
                $this->systemUser = $result["systemuser"];
            } else {
                $this->systemUser = true;
            }

            // Se não for usuário de sistema, faz os testes de logon
            if (!$this->systemUser) {
                $ultimoLogonOK = $this->testaUltimoLogon($result["lastlogon"]);
                $validado = $usuarioAtivo && $ultimoLogonOK;
            } else {
                $validado = $usuarioAtivo;
            }
            $this->Validado = $validado;

            // Verifica se o campo de Email Token Existe
            $campoEmailTokenExiste = key_exists("tokenEmail", $result);

            if (!$this->systemUser) {
                // Verifica se é a primeira senha, ou se o tempo para expiração de troca de senha ocorreu

                $trocarSenhaPrimeiroLogon = $this->testaStartLogon($result["lastAlterPassword"]);
                $this->startLogon = $trocarSenhaPrimeiroLogon & $campoEmailTokenExiste;
                if ($this->startLogon) {
                    $tokenRecuperacao = uniqid();
                    $SQL = "update userdef set tokenEmail = '$tokenRecuperacao' where userId = {$result["UserId"]}";
                    $Query = mysqli_query($this->connect, $SQL);

                    $this->tokenEmail = $tokenRecuperacao;
                }
            }
            insereEntradaAuditTrail(0, 0, 0, $result["UserId"], $UserName, 900, "Logon efetuado para $UserName");
            if ($validado) {
                $this->SalvaDataLogon();
            }
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
            //error_log("Usuário não autenticado: $UserName", 0);
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
