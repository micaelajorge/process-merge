<?php

include("config.config.php");

if ($OrigemLogon == "Server") {
    if (empty($ldap_domain)) {
        $tmp = explode(".", $ldap_server);
        $i = 1;
        for ($i = 1; $i < count($tmp) - 1; $i++) {
            $ldap_domain .= $tmp[$i] . ".";
        }
        $ldap_domain .= $tmp[$i];
    }
}

function PegaUsuariosProcess($filtro, $Grupo = '', $Origem = '', $Tipo = 0)
{
    global $connect;
    $SQL = "select UserName, UserDesc, userdef.UserId, Origem from userdef ";
    if ($Grupo != '' & $Grupo != 0) {
        $SQL .= " ,usergroup where ";
        $SQL .= " GroupId = $Grupo ";
        $SQL .= " and ";
        $SQL .= " userdef.UserId = usergroup.UserId and";
    } else {
        $SQL .= " where ";
    }
    //$SQL .= " UserName like '$filtro%' and samaccountname is null";
    if ($Tipo == 0) {
        $SQL .= " UserName like '$filtro%' ";
    } else {
        $SQL .= " UserDesc like '$filtro%' ";
    }
    $query = mysqli_query($connect, $SQL);
    $users = mysqli_fetch_all($query, MYSQLI_ASSOC);
    return $users;
}

function PegaUsuariosProcessPermissoes($filtro)
{
    global $connect;
    $SQL = "select UserName, UserDesc, userdef.UserId, 'U' as grpFld from userdef ";
    $SQL .= " where ";
    $SQL .= " UserName like '$filtro%' ";
    $SQL .= " or ";
    $SQL .= " UserDesc like '$filtro%' ";
    $query = mysqli_query($connect, $SQL);
    $users = mysqli_fetch_all($query, MYSQLI_ASSOC);
    return $users;
}

function PegaCamposProcessPermissoes($filtro, $procId)
{
    global $connect;
    $SQL = "select fieldname as UserName, fieldDesc UserDesc, fieldId UserId, 'F' as grpFld from procfieldsdef ";
    $SQL .= " where ";
    $SQL .= " ( ";
    $SQL .= " fieldname like '$filtro%' ";
    $SQL .= " or ";
    $SQL .= " fielddesc like '$filtro%' ";
    $SQL .= " ) ";
    $SQL .= " and ";
    $SQL .= " procId  =  $procId ";
    $query = mysqli_query($connect, $SQL);
    $users = mysqli_fetch_all($query, MYSQLI_ASSOC);
    return $users;
}

/**
 * 
 * @global type $connect
 * @param type $filtro
 * @return type]
 */
function PegaGruposProcessPermissoes($filtro)
{
    global $connect;
    $SQL = "select groupname as UserName, groupdesc as UserDesc, groupid as UserId, 'G' as grpFld from groupdef ";
    $SQL .= " where ";
    $SQL .= " GroupName like '$filtro%' ";
    $SQL .= " or ";
    $SQL .= " GroupDesc like '$filtro%' ";
    $query = mysqli_query($connect, $SQL);
    $users = mysqli_fetch_all($query, MYSQLI_ASSOC);
    return $users;
}

function PegaDadosEmailUsuarioProcess($UserId)
{
    global $connect;
    $connectDBExterno = AtivaDBExterno();
    $SQL = "select UserDesc as UserFullName, Email";
    $SQL .= " from userdef";
    $SQL .= " where ";
    $SQL .= " userdef.UserId  = $UserId";
    $query = mysqli_query($connect, $SQL);
    while ($Result = mysqli_fetch_array($query)) {
        $Usuario["Nome"] = htmlentities($Result["UserFullName"]);
        $Usuario['Email'] = htmlentities($Result["Email"]);
        $Usuario['UserId'] = $UserId;
    }
    return $Usuario;
}

function PegaDadosEmailUsuarioExterno($UserId)
{
    global $connect;
    include("include/config.config.php");
    $connectDBExterno = AtivaDBExterno();
    $SQL = "select $EXTERNAL_USERNAME as UserName, $EXTERNAL_USERFULLNAME UserFullName, $EXTERNAL_userdef.$EXTERNAL_USERID as UserId, Email";
    $SQL .= " from $EXTERNAL_userdef";
    $SQL .= " where ";
    $SQL .= " $EXTERNAL_userdef.$EXTERNAL_USERID = $UserId and Origem = '$OrigemDoc'";
    $query = mysqli_query($connectDBExterno, $SQL);
    while ($Result = mysqli_fetch_array($query)) {
        $Usuario["Nome"] = htmlentities($Result["UserFullName"]);
        $Usuario['Email'] = htmlentities($Result["Email"]);
        $Usuario['UserId'] = $UserId;
    }
    return $Usuario;
}

function PegaDadosEmailUsuario($UserId)
{
    global $OrigemLogon;
    if ($OrigemLogon == "External") {
        return PegaDadosEmailUsuarioExterno($UserId);
    } else {
        return PegaDadosEmailUsuarioProcess($UserId);
    }
}

function PegaUsuariosExterno($filtro, $Grupo = "", $OrigemDoc = "", $Tipo = 0)
{
    global $connect;
    include("include/config.config.php");
    if (!empty($OrigemDoc)) {
        $SQL = "select Origem_doc from origemdominio where Origem_User = '$OrigemDoc'";
        $Query = mysqli_query($connect, $SQL);
        $origensDoc = array();
        while ($Result = mysqli_fetch_array($Query)) {
            array_push($origensDoc, $Result["Origem_doc"]);
        }
    }
    $connectDBExterno = AtivaDBExterno();
    $SQL = "select $EXTERNAL_USERNAME as UserName, $EXTERNAL_USERFULLNAME UserFullName, $EXTERNAL_userdef.$EXTERNAL_USERID as UserId, Origem ";
    $SQL .= " from $EXTERNAL_userdef";
    if (!empty($Grupo)) {
        $SQL .= " , $EXTERNAL_USERSGROUPS where ";
        $SQL .= " $EXTERNAL_GROUPID = $Grupo ";
        $SQL .= " and ";
        $SQL .= " $EXTERNAL_userdef.$EXTERNAL_USERID = $EXTERNAL_USERSGROUPS.$EXTERNAL_USERID and";
    } else {
        $SQL .= " where ";
    }
    if ($Tipo == 0) {
        $SQL .= " $EXTERNAL_USERNAME like '$filtro%'";
    } else {
        $SQL .= " $EXTERNAL_USERFULLNAME like '$filtro%'";
    }
    if (!empty($EXTERNAL_FILTER)) {
        $SQL .= " and $EXTERNAL_FILTER ";
    }
    $query = mysqli_query($connectDBExterno, $SQL);
    $i = count($users);
    while ($Result = mysqli_fetch_array($query)) {
        if (!empty($OrigemDoc)) {
            if (!in_array($Result["Origem"], $origensDoc) && count($origensDoc) > 0) {
                continue;
            }
        }
        $users[$i]["Nome"] = htmlentities($Result["UserFullName"]);
        $users[$i]['samaccountname'] = htmlentities($Result["UserName"]);
        $users[$i]['UserId'] = $Result["UserId"];
        $users[$i]['Origem'] = $Result["Origem"];
        $i++;
    }
    return $users;
}

function _PegaProcessosUsuario($UserId = 0)
{
    global $connect, $userdef;
    $S_Processos = array();
    $SQL = " select 
		ProcName, PageAction, Pf.ProcId, NrTotal as NumCount, ProcCod, TipoProc, ProcDesc 
		from 
		procdef as PF 
		inner join 
		(
		select ProcId, sum(NrTotal) as NrTotal from
		(
		select ProcId, count(CaseId) as NrTotal from  
		(
		select CQ.ProcId, CaseId from casequeue as CQ, exportkeys as EX, origemdominio as OD, stepaddresses as SA where
		CQ.ProcId = EX.ProcId and CQ.CaseId = Ex.CaseNum and OD.Origem_User = '$userdef->Origem' and EX.ORIGEM = Origem_DOC and SA.ProcId = CQ.ProcId and
		SA.StepId = CQ.StepId and (SA.GRPFLD = 'G' and GroupId  
		in ($userdef->usergroups) or GrpFld = 'U' and GroupId = '$userdef->UserId_Process' ) and SA.ViewQueue = 1 
		union
		select SA.ProcId, CaseId
		from 
		stepaddresses as SA, fieldaddresses as FA, procfieldsdef as PF, casequeue as CQ, exportkeys as EX, origemdominio as OD
		where 
		PF.ProcId = FA.ProcId and SA.ProcId = PF.ProcId and SA.GroupId = PF.FieldId and SA.GrpFld = 'F' 
		and
		((PF.FieldType = 'US' and FA.FieldValue = '$userdef->UserId_Process') or (PF.FieldType = 'GR' and FA.FieldValue in ($userdef->usergroups)))
		and
		SA.StepId = CQ.StepId and 
		SA.ProcId = CQ.ProcId and
		FA.CaseNum = CQ.CaseId and
		Ex.ProcId = CQ.ProcId and
		Ex.CaseNum = FA.CaseNum and
		OD.Origem_User = '$userdef->Origem' and
		Ex.Origem = OD.Origem_Doc and
		SA.ViewQueue = 1
		) as Casos group by ProcId
		union
		select ProcId, 0 as NrTotal from stepaddresses as SA where
		(SA.GRPFLD = 'G' and GroupId  
		in ($userdef->usergroups) or GrpFld = 'U' and GroupId = '$userdef->UserId_Process' ) and SA.ViewQueue = 1 
		group by ProcId
		)
		as Totais
		group by ProcId
		) as Totais 
		on (Pf.ProcId = Totais.ProcId) and Active = 1 order by NrTotal desc, ProcName
		";
    $Query = mysqli_query($connect, $SQL);
    while ($Result = mysqli_fetch_array($Query)) {
        $PageAction = trim($Result["PageAction"]);
        if (empty($PageAction)) {
            $PageAction = "queue/%PROCID%";
        }
        $Result["PageAction"] = $PageAction;
        array_push($S_Processos, $Result);
    }
    return $S_Processos;
}

function SQLProcessosDoUsuario($ViewQueue = 1)
{
    global $userdef;


    $SQLUnion = "";
    $SQLUnion .= " union ";
    $SQLUnion .= "( \n";
    $SQL = '';
    $SQL .= "select ProcId from stepaddresses where GrpFld = 'G' and GroupId in ($userdef->usergroups)";
    $SQL .= " and ";
    $SQL .= " stepaddresses.ViewQueue = $ViewQueue ";
    $SQL .= " group by ProcId";
    $SQL .= $SQLUnion;
    $SQL .= "select ProcId from stepaddresses where GrpFld = 'U' and GroupId = $userdef->UserId_Process";
    $SQL .= " and ";
    $SQL .= " stepaddresses.ViewQueue = $ViewQueue ";
    $SQL .= " group by ProcId";
    $SQL .= ")";

    return $SQL;
}

function PegaProcessosUsuario()
{
    global $connect, $userdef;
    $S_Processos = array();
//    $SQL = "select ProcName, ProcDesc, ProcDef.PageAction, procdef.ProcId, ProcIcon, ProcColor, count(*) as TotalCasos, TipoProc, ProcCod, extendPropsProc from "
//            . "procdef, "
//            . "casosdousuario, "
//            . "stepdef "
//            . "where "
//            . "procdef.ProcId = casosdousuario.procId "
//            . "and "
//            . "userid = $userdef->UserId "
//            . "and "
//            . "stepdef.procid = casosdousuario.procid "
//            . "and "
//            . "stepdef.stepid = casosdousuario.stepid "
//            . "and "
//            . "stepdef.EndStep <> 1 "
//            . "group by "
//            . "ProcName, ProcDesc, "
//            . "PageAction, "
//            . "procdef.ProcId, "
//            . "ProcIcon, "
//            . "ProcColor, "
//            . "TipoProc, "
//            . "ProcCod ";

    $SQL = "select ProcName, ProcDesc, ProcDef.PageAction, procdef.ProcId, ProcIcon, ProcColor, count(*) as TotalCasos, TipoProc, ProcCod, extendPropsProc from procdef, casosdousuario, stepdef where procdef.ProcId = casosdousuario.procId and userid = $userdef->UserId and stepdef.procid = casosdousuario.procid and stepdef.stepid = casosdousuario.stepid and stepdef.EndStep <> 1 group by ProcName, ProcDesc, PageAction, procdef.ProcId, ProcIcon, ProcColor, TipoProc, ProcCod 
union distinct
select ProcName, ProcDesc, ProcDef.PageAction, procdef.ProcId, ProcIcon, ProcColor, 0 as TotalCasos, TipoProc, ProcCod, extendPropsProc from procdef, stepdef, stepaddresses where procdef.procid = stepdef.procid and stepaddresses.groupid = $userdef->UserId and GrpFld = 'U' and stepdef.procid = stepaddresses.procid and stepdef.stepid = stepaddresses.stepid and stepdef.EndStep <> 1 group by ProcName, ProcDesc, PageAction, procdef.ProcId, ProcIcon, ProcColor, TipoProc, ProcCod 
union distinct
select ProcName, ProcDesc, ProcDef.PageAction, procdef.ProcId, ProcIcon, ProcColor, 0 as TotalCasos, TipoProc, ProcCod, extendPropsProc from procdef, stepdef, stepaddresses, usergroup where procdef.procid = stepdef.procid and usergroup.GroupId in ($userdef->usergroups) and stepaddresses.groupid = usergroup.GroupId and GrpFld = 'G' and stepdef.procid = stepaddresses.procid and stepdef.stepid = stepaddresses.stepid and stepdef.EndStep <> 1 group by ProcName, ProcDesc, PageAction, procdef.ProcId, ProcIcon, ProcColor, TipoProc, ProcCod 
order by ProcName";

    $Query = mysqli_query($connect, $SQL);
    //echo $SQL;
    $i = 0;
    //echo $SQL;
    $aProcs = array(); // Guarda os Processos que foram incluidos na lista

    while ($result = mysqli_fetch_array($Query)) {

        array_push($aProcs, $result["ProcId"]);

        $S_Processos[$i]["TipoProc"] = $result["TipoProc"];
        $S_Processos[$i]["ProcId"] = $result["ProcId"];
        $S_Processos[$i]["ProcName"] = $result["ProcName"];
        $S_Processos[$i]["ProcDesc"] = $result["ProcDesc"];
        $S_Processos[$i]["TotalCasos"] = $result["TotalCasos"];
        $S_Processos[$i]["ProcCod"] = $result["ProcCod"];
        $S_Processos[$i]["HideEntrada"] = $result["HideEntrada"];

        if (isset($result["ProcIcon"])) {
            $S_Processos[$i]["ProcIcon"] = $result["ProcIcon"];
        } else {
            $S_Processos[$i]["ProcIcon"] = "";
        }

        if (isset($result["ProcColor"])) {
            $S_Processos[$i]["ProcColor"] = $result["ProcColor"];
        } else {
            $S_Processos[$i]["ProcColor"] = "";
        }
        $PageAction = trim($result["PageAction"]);
        if (empty($PageAction)) {
            //$PageAction = "queue/%PROCID%";
            $PageAction = "";
        }
        $S_Processos[$i]["PageAction"] = $PageAction;

        $extendProps = json_decode($result["extendPropsProc"], true);

        $S_Processos[$i]["workSpace"] = "";
        $S_Processos[$i]["workSpace"] = $extendProps["procWorkplace"];
//        if (is_array($extendProps)) {
//            $S_Processos[$i]["workSpace"] = $extendProps["procWorkplace"];
//        }

        $i++;
    }

    $SQLProcs = SQLProcessosDoUsuario();

    $SQL = "select ProcName, ProcDesc, PageAction, ProcId, ProcIcon, TipoProc, ProcColor, ProcCod, HideEntrada from procdef where ProcId in ($SQLProcs) order by ProcName ";
    $Query = mysqli_query($connect, $SQL);
    while ($result = mysqli_fetch_array($Query)) {

        /**
         *  Se o processo j� estiver no array desconsidera
         */
        if (in_array($result["ProcId"], $aProcs)) {
            continue;
        }

        $S_Processos[$i]["TipoProc"] = $result["TipoProc"];
        $S_Processos[$i]["ProcId"] = $result["ProcId"];
        $S_Processos[$i]["ProcName"] = $result["ProcName"];
        $S_Processos[$i]["ProcDesc"] = $result["ProcDesc"];
        $S_Processos[$i]["TotalCasos"] = 0;
        $S_Processos[$i]["ProcCod"] = $result["ProcCod"];
        $S_Processos[$i]["HideEntrada"] = $result["HideEntrada"];


        switch ($result["TipoProc"]) {
            case "CT":
                $ProcIcon = "fa fa-tags";
                $ProcColor = "";
                break;
            case "RP":
                $ProcIcon = "fa fa-files-o";
                $ProcColor = "#008D4C";
                break;
            case "WF":
            default:
                $S_Processos[$i]["ProcIcon"] = "";
                $ProcColor = "";
                break;
        }

        if ($result["ProcIcon"] != "") {
            $S_Processos[$i]["ProcIcon"] = $result["ProcIcon"];
        } else {
            $S_Processos[$i]["ProcIcon"] = $ProcIcon;
        }

        if ($result["ProcColor"] != "") {
            $S_Processos[$i]["ProcColor"] = $result["ProcColor"];
        } else {
            $S_Processos[$i]["ProcColor"] = $ProcColor;
        }

        $PageAction = trim($result["PageAction"]);
        if (empty($PageAction)) {
            //$PageAction = "queue/%PROCID%";
            $PageAction = "";
        }
        $S_Processos[$i]["PageAction"] = $PageAction;
        $i++;
    }

    return $S_Processos;
}

function PegaSamAccountNameGrupo($Grupo)
{
    global $connect;
    if ($Grupo == "") {
        return "";
    }
    $SQL = "select samaccountname from groupdef where GroupId = $Grupo";
    $Query = mysqli_query($connect, $SQL);
    $Result = mysqli_fetch_array($Query);
    return trim($Result["samaccountname"]);
}

function ConnectaAD()
{
    global $auth_user, $auth_pass, $base_dn, $ldap_server, $ldap_domain, $config;
    $Server = "ldap://" . $ldap_server;
    if (!($connect = @ldap_connect($ldap_server))) {
        die("Could not connect to ldap server<br>");
    }
    ldap_set_option($connect, LDAP_OPT_PROTOCOL_VERSION, 3);
    if (!($bind = @ldap_bind($connect, $auth_user . '@' . $ldap_domain, $auth_pass))) {
        session_destroy();
        /* $erro_logon = true;
          $MsgLogon = 'Usu�rio n�o existe, esta inativo ou a Senha est� incorreta';
          include("logon.php");
          exit; */
    }
    return $connect;
}

function SearchAd($connect, $criteria, $justthese, $base_dn_alt = "")
{
    global $base_dn;

    if (empty($base_dn_alt)) {
        $base_dn_alt = $base_dn;
    }
    $ldapSearchResult = @ldap_search($connect, $base_dn_alt, $criteria, $justthese);
    return $ldapSearchResult;
}

function PegaGruposAD($samaccountname)
{
    $Grupos = '';
    $StrPesquisa = "samaccountname=" . $samaccountname;
    $Atributos = array("memberOf");
    $connect = ConnectaAD();
    $ldapSearchResult = SearchAd($connect, $StrPesquisa, $Atributos);
    if (@ldap_count_entries($connect, $ldapSearchResult)) {
        $ldapResults = ldap_get_entries($connect, $ldapSearchResult);
        $entry = ldap_first_entry($connect, $ldapSearchResult);
        $attrs = ldap_get_attributes($connect, $entry);
        $ldapGrupos = $attrs["memberOf"];
        for ($i = 0; $i < count($ldapGrupos) - 1; $i++) {
            $GruposCN = explode(",", $ldapGrupos[$i]);
            $GruposCN = str_replace("CN=", "", $GruposCN);
            $Grupos[$i] = $GruposCN[0];
        }
    }
    if (is_array($Grupos)) {
        $Grupos = implode(",", $Grupos);
        return $Grupos;
    } else {
        return "";
    }
}

function PegaDadosAdRM($UserName, $Password)
{
    require_once(FILES_ROOT . "vendor/lib/nusoap.php");
    include("include/conf_rm.php");
    $client = new soapclient($RMWS, 'wsdl');
    $client->setDefaultRpcParams(true);
    $soap_proxy = $client->getProxy();
    $Params["username"] = $UserName;
    $Params["password"] = $Password;

    $result = @$client->call("AuthenticateUser", array($Params));

    if (!is_null($result["faultstring"])) {
        $UserAD["Valido"] = "0";
    } else {
        $result = $result["AuthenticateUserResult"];
        if ($result["Error"] <> '') {
            $UserAD["Valido"] = "0";
        }
        $GruposAD = $result["Groups"]["string"];
        $i = 0;
        foreach ($GruposAD as $grupo) {
            $Grupos[$i] = PegaIdGrupo($grupo);
            $i++;
        }
        $UserAD["Grupos"] = implode(",", $Grupos);
        $UserAD["AGrupos"] = $Grupos;
        $UserAD["UserDesc"] = $result["DisplayName"];
        $UserAD["Valido"] = "1";
    }
    return $UserAD;
}

function PegaDadosAD($samaccountname)
{
    $UserAD["Valido"] = "0";
    $UserAD["samaccountname"] = $samaccountname;
    $criteria = "samaccountname=$samaccountname";
    //$justthese = array("memberOf","userAccountControl","DisplayName"); 
    $justthese = array("memberOf", "DisplayName,OU");

    //$justthese = array("memberOf"); 
    $connect = ConnectaAD();
    $ldapSearchResult = SearchAD($connect, $criteria, $justthese);
    if (@ldap_count_entries($connect, $ldapSearchResult)) {
        $ldapResults = ldap_get_entries($connect, $ldapSearchResult);
        $entry = ldap_first_entry($connect, $ldapSearchResult);
        $attrs = ldap_get_attributes($connect, $entry);
        $ldapGrupos = $attrs["memberOf"];
        for ($i = 0; $i < count($ldapGrupos) - 1; $i++) {
            $GruposCN = explode(",", $ldapGrupos[$i]);
            $GruposCN = str_replace("CN=", "", $GruposCN);
            $Grupos[$i] = $GruposCN[0];
        }
        $UserAD["UserDesc"] = $attrs["displayName"][0];

        $UserAD["Valido"] = "1";
    }
    if (is_array($Grupos)) {
        $j = 0;
        for ($i = 0; $i < count($Grupos); $i++) {
            $Id = PegaIdGrupo($Grupos[$i]);
            if (!empty($Id)) {
                $IdGrupos[$j] = $Id;
                $j++;
            }
        }
        $UserAD["Grupos"] = implode(",", $IdGrupos);
        $UserAD["AGrupos"] = $IdGrupos;
    } else {
        $UserAD["Grupos"] = "";
        $UserAD["AGrupos"] = array();
    }
    return $UserAD;
}

function PegaSamAccountName($CN)
{
    global $base_dn, $config;
    if ($config["AD"] <> 1) {
        return;
    }
    $StrPesquisa = "CN=*";
    $Atributos = array("sAMAccountName");
    $base_dn_alt = $CN;
    $connect = ConnectaAD();
    $ldapSearchResult = SearchAd($connect, $StrPesquisa, $Atributos, $base_dn_alt);
    if (ldap_count_entries($connect, $ldapSearchResult)) {
        $ldapResults = ldap_get_entries($connect, $ldapSearchResult);
        $entry = ldap_first_entry($connect, $ldapSearchResult);
        $attrs = ldap_get_attributes($connect, $entry);
        $ldapValor = $attrs["sAMAccountName"];
        for ($i = 0; $i < count($ldapValor) - 1; $i++) {
            $valor = $ldapValor[$i];
        }
    }
    return $valor;
}

function PegaIdGrupo($samaccountname)
{
    global $connect;
    $SQL = "select GroupId from groupdef where samaccountname = '$samaccountname'";
    $Query = mysqli_query($connect, $SQL);
    $result = mysqli_fetch_array($Query);
    if ($result["GroupId"] == "") {
        return -1;
    } else {
        return $result["GroupId"];
    }
}

function PegaMembrosGrupo($Grupo, $Filtro = "")
{
    global $base_dn, $config;
    if ($config["AD"] <> 1) {
        return;
    }
    $StrPesquisa = "CN=$Grupo";
    $Atributos = array("member");
    $base_dn_alt = $base_dn;
    $connect = ConnectaAD();
    $ldapSearchResult = SearchAd($connect, $StrPesquisa, $Atributos, $base_dn_alt);
    $LenFiltro = strlen($Filtro);
    if (ldap_count_entries($connect, $ldapSearchResult)) {
        $ldapResults = ldap_get_entries($connect, $ldapSearchResult);
        $entry = ldap_first_entry($connect, $ldapSearchResult);
        $attrs = ldap_get_attributes($connect, $entry);
        $ldapGrupos = $attrs["member"];
        $j = 0;
        for ($i = 0; $i < count($ldapGrupos) - 1; $i++) {
            $Nome = $ldapGrupos[$i];
            $Sam = PegaSamAccountName($Nome);
            if ($LenFiltro > 0) {
                if (strtoupper(substr($Sam, 0, $LenFiltro)) != strtoupper($Filtro)) {
                    continue;
                }
            }
            $Grupos[$j]["samaccountname"] = $Sam;
            $Nome = explode(",", $Nome);
            $Nome = str_replace("CN=", "", $Nome[0]);
            $Grupos[$j]["Nome"] = $Nome;
            $j++;
        }
    }
    return $Grupos;
}

function PegaUsuariosAD($Filtro = "")
{
    global $base_dn, $config;
    if ($config["AD"] <> 1) {
        return;
    }
    $StrPesquisa = "(&(objectClass=user)(objectCategory=person)(cn=$Filtro*))";
    $Atributos = array("samaccountname", "displayname");
    $base_dn_alt = $base_dn;
    $connect = ConnectaAD();
    $ldapSearchResult = SearchAd($connect, $StrPesquisa, $Atributos, $base_dn_alt);
    $info = ldap_get_entries($connect, $ldapSearchResult);
    for ($i = 0; $i < $info["count"]; $i++) {
        while (list($key, $val) = each($info[$i])) {
            if (($key == "samaccountname")) {
                if (is_array($val)) {
                    $Samaccountname = $info[$i][$key][0];
                }
            }
            if (($key == "displayname")) {
                if (is_array($val)) {
                    $DisplayName = $info[$i][$key][0];
                }
            }
        }
        $Grupos[$i]["samaccountname"] = $Samaccountname;
        $Grupos[$i]["Nome"] = $DisplayName;
    }
    return $Grupos;
}

function Authenticate($UserName, $UserPwd)
{
    global $UserAD, $UserId, $userdef;
    include("\include\config.php");
    if (empty($UserName) | empty($UserPwd)) {
        return false;
    }
    $Auth_ldap = Authldap($UserName, $UserPwd);
    $Auth_Process = AuthProcess($UserName, $UserPwd);
    if (!($Auth_ldap | $Auth_Process)) {
        return false;
    }
    if ($Auth_ldap) {
        $UserAD = PegaDadosAd($UserName);
        $UserId = -1;
    } else {
        $UserId = $Auth_Process;
    }
    $S_Processos = PegaProcessosUsuario($UserId);
    if (count($S_Processos) == 0) {
        return false;
    }
    return true;
}

function AuthProcess($UserName, $UserPwd)
{
    global $connect;
    $SQL = "select UserId from userdef where UserName = '$UserName' and UserPwd = '$UserPwd' and UActive = 1";
    if (!($Query = @mysqli_query($connect, $SQL))) {
        return false;
    }
    if (mysqli_num_rows($Query) == 0) {
        return false;
    }
    $Result = @mysqli_fetch_array($Query);
    return $Result["UserId"];
}

function AuthLdap($username, $password)
{
    global $ldap_domain, $ldap_server;
    $auth_user = $username . "@" . $ldap_domain;
    if ($connect = @ldap_connect($ldap_server)) {
        if (($bind = @ldap_bind($connect, $auth_user, $password))) {
            @ldap_close($connect);
            return true;
        }
    }
    @ldap_close($connect);
    return false;
}
