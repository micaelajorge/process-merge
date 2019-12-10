<?php

function CamposSQLQueue($CampoOrdem = '', $RetornarTodos = true)
{
    $CamposSQL = array();
    if ($RetornarTodos) {
        $CamposSQL[] = "casequeue.ProcId";
        $CamposSQL[] = "lockedbyid";
        $CamposSQL[] = "LockedBysamaccountname";
        $CamposSQL[] = "casequeue.StepId";
        $CamposSQL[] = "CaseId";
        $CamposSQL[] = "priority";
        $CamposSQL[] = "deadsoftdatetime";
        $CamposSQL[] = "deadharddatetime";
        $CamposSQL[] = "deadhardestdatetime";
        $CamposSQL[] = "AdHocDeadSoftNotify";
        $CamposSQL[] = "AdHocDeadHardNotify";
        $CamposSQL[] = "casequeue.AdHoc";
    } else {
        $CamposSQL[] = "casequeue.ProcId";
        $CamposSQL[] = "casequeue.StepId";
        
    }
    if (!empty($CampoOrdem)) {
        if (strtolower($CampoOrdem) == "casenum") {
            $CampoOrdem = "exportkeys.$CampoOrdem";
        }
        $CamposSQL[] = $CampoOrdem;
    }
    return $CamposSQL;
}

function CamposSQLSteps()
{
    $CamposSQL[1] = "casequeue.StepId";
    $CamposSQL[2] = "count(*) as TotalCasos";
    return $CamposSQL;
}

function CamposSQLProcessos()
{
    $CamposSQL[1] = "casequeue.ProcId";
    $CamposSQL[2] = "count(*) as TotalCasos";
    return $CamposSQL;
}

/**
 * 
 * @global type $userdef
 * @param type $ProcId
 * @param type $CamposSQL
 * @param type $StepId
 * @param type $HideQueue
 * @param type $CampoOrdem
 * @param type $OrigemUser
 * @param type $Filtros
 * @param type $ViewQueue
 * @param type $GroupBy
 * @return type
 */
function _SQLCasosGrupos($ProcId, $CamposSQL, $StepId = "-1", $HideQueue = 1, $CampoOrdem = '', $OrigemUser = '', $Filtros = '', $ViewQueue = 1, $GroupBy = '')
{
    global $userdef;
    $SQL = "Select ";
    $SQL .= implode(",", $CamposSQL);
    $SQL .= " from ";
    $SQL .= "casequeue, ";
    $SQL .= "stepaddresses, ";
    if (!empty($OrigemUser)) {
        $SQL .= "origemdominio, ";
    }
    $SQL .= "exportkeys ";
    $SQL .= "where ";
    $SQL .= "stepaddresses.GrpFld = 'G' ";
    if (!empty($ProcId)) {
        $SQL .= "and    ";
        $SQL .= "stepaddresses.ProcId = $ProcId ";
        if ($StepId != -1 && is_numeric($StepId)) {
            $SQL .= "and    ";
            $SQL .= "stepaddresses.StepId = $StepId ";
        }
    }
//    if ($StepId == 0) {
//        $SQL .= "and casequeue.CaseId in (";
//        $SQL .= "Select CaseId from casequeue ";
//        if (!empty($ProcId)) {
//            $SQL .= "where ProcId = $ProcId ";
//        }
//        $SQL .= " group by CaseId having count(*) > 1";
//        $SQL .= ")";
//    }
//    if ($StepId == 0)
//    {
//        $SQL .= "and casequeue.CaseId in (";
//        $SQL .= "Select CaseId from casequeue ";
//        if (!empty($ProcId)) {
//            $SQL .= "where ProcId = $ProcId ";
//        }
//        $SQL .= " group by CaseId having count(*) > 1";
//        $SQL .= ")";        
//    }
    $SQL .= "and    ";
    $SQL .= "stepaddresses.GroupId in ($userdef->usergroups) ";
    $SQL .= "and    ";
    $SQL .= "casequeue.StepId = stepaddresses.StepId    ";
    $SQL .= "and    ";
    $SQL .= "casequeue.ProcId = stepaddresses.ProcID    ";
    $SQL .= "and    ";
    $SQL .= "casequeue.AdHoc <> 1    ";
    $SQL .= "and ";
    $SQL .= "stepaddresses.ViewQueue = $ViewQueue ";
    if ($HideQueue == 1) {
        $SQL .= "and ";
        $SQL .= "casequeue.HideQueue = 0 ";
    }
    $SQL .= "and ";
    $SQL .= "exportkeys.CaseNum = casequeue.CaseId ";
    $SQL .= "and ";
    $SQL .= "exportkeys.ProcId = casequeue.ProcId ";
    if (!empty($OrigemUser)) {
        $SQL .= "and ";
        $SQL .= "exportkeys.Origem = origemdominio.Origem_DOC ";
        $SQL .= "and ";
        $SQL .= "origemdominio.Origem_uSER = '$OrigemUser' ";
    }
    if (is_array($Filtros)) {
        for ($i = 0; $i < count($Filtros); $i++) {
            $SQL .= "and ";
            $SQL .= $Filtros[$i];
        }
    }
    $SQL .= $GroupBy;
    return $SQL;
}

/**
 * 
 * @global type $userdef
 * @param type $ProcId
 * @param type $CamposSQL
 * @param type $StepId
 * @param type $HideQueue
 * @param type $CampoOrdem
 * @param type $OrigemUser
 * @param type $Filtros
 * @param type $ViewQueue
 * @param type $GroupBy
 * @param type $EndStep
 * @return type
 */
function SQLCasosGrupos($ProcId, $CamposSQL, $StepId = "-1", $HideQueue = 1, $CampoOrdem = '', $OrigemUser = '', $Filtros = '', $ViewQueue = 1, $GroupBy = '', $EndStep = false)
{
    global $userdef;
    $SQL = "Select ";
    $SQL .= implode(",", $CamposSQL);
    $SQL .= " from ";
    $SQL .= "casequeue, ";
    $SQL .= "stepaddresses, ";
    if ($EndStep) {
        $SQL .= "stepdef, ";
    }
    if (!empty($OrigemUser)) {
        $SQL .= "origemdominio, ";
    }
    $SQL .= "exportkeys ";
    $SQL .= "where ";
    $SQL .= " (( ";
    $SQL .= "stepaddresses.GrpFld = 'G' ";
    if (!empty($ProcId)) {
        $SQL .= "and    ";
        $SQL .= "stepaddresses.ProcId = $ProcId ";
        if ($StepId != -1 && is_numeric($StepId)) {
            $SQL .= "and    ";
            $SQL .= "stepaddresses.StepId = $StepId ";
        }
    }
    $SQL .= "and    ";
    $SQL .= "stepaddresses.GroupId in ($userdef->usergroups) ";
    $SQL .= ") or (";
    $SQL .= "stepaddresses.GrpFld = 'U' ";
    if (!empty($ProcId)) {
        $SQL .= "and    ";
        $SQL .= "stepaddresses.ProcId = $ProcId ";
        if ($StepId != -1 && is_numeric($StepId)) {
            $SQL .= "and    ";
            $SQL .= "stepaddresses.StepId = $StepId ";
        }
    }
    $SQL .= "and    ";
    $SQL .= "stepaddresses.GroupId in ($userdef->UserId_Process) ";
    $SQL .= "))";
    $SQL .= "and    ";
    $SQL .= "casequeue.StepId = stepaddresses.StepId    ";
    $SQL .= "and    ";
    $SQL .= "casequeue.ProcId = stepaddresses.ProcID    ";
    $SQL .= "and    ";
    $SQL .= "casequeue.AdHoc <> 1    ";
    $SQL .= "and ";
    $SQL .= "stepaddresses.ViewQueue = $ViewQueue ";
    if ($HideQueue == 1) {
        $SQL .= "and ";
        $SQL .= "casequeue.HideQueue = 0 ";
    }
    $SQL .= "and ";
    $SQL .= "exportkeys.CaseNum = casequeue.CaseId ";
    $SQL .= "and ";
    $SQL .= "exportkeys.ProcId = casequeue.ProcId ";
    if (!empty($OrigemUser)) {
        $SQL .= "and ";
        $SQL .= "exportkeys.Origem = origemdominio.Origem_DOC ";
        $SQL .= "and ";
        $SQL .= "origemdominio.Origem_uSER = '$OrigemUser' ";
    }
    if (is_array($Filtros)) {
        for ($i = 0; $i < count($Filtros); $i++) {
            $SQL .= "and ";
            $SQL .= $Filtros[$i];
        }
    }
    if ($EndStep) {
        $SQL .= " and ";
        $SQL .= " stepdef.procid = stepaddresses.procid and  ";
        $SQL .= " stepdef.stepid = stepaddresses.stepid and  ";
        $SQL .= " stepdef.endstep = 0  ";
    }

    $SQL .= $GroupBy;
    return $SQL;
}

function SQLCasosUsuario($ProcId, $CamposSQL, $StepId = "-1", $HideQueue = 1, $CampoOrdem = '', $OrigemUser = '', $Filtros = '', $ViewQueue = 1, $GroupBy = '')
{
    global $userdef;
    $SQL = "Select ";
    $SQL .= implode(",", $CamposSQL);
    $SQL .= " from ";
    $SQL .= "casequeue, ";
    $SQL .= "exportkeys, ";
    if (!empty($OrigemUser)) {
        $SQL .= "origemdominio, ";
    }
    $SQL .= "stepaddresses ";
    $SQL .= "where ";
    $SQL .= "stepaddresses.GrpFld = 'U' ";
    if (!empty($ProcId)) {
        $SQL .= "and    ";
        $SQL .= "stepaddresses.ProcID = $ProcId ";
        if ($StepId != -1 && is_numeric($StepId)) {
            $SQL .= "and    ";
            $SQL .= "stepaddresses.StepId = $StepId ";
        }
    }
//    if ($StepId == 0) {
//        $SQL .= "and casequeue.CaseId in (";
//        $SQL .= "Select CaseId from casequeue ";
//        if (!empty($ProcId)) {
//            $SQL .= " where ProcId = $ProcId ";
//        }
//        $SQL .= " group by CaseId having count(*) > 1 ";
//        $SQL .= ")";
//    }
    $SQL .= "and ";
    $SQL .= "stepaddresses.GroupId = '$userdef->UserId_Process' ";
    $SQL .= "and ";
    $SQL .= "casequeue.StepId = stepaddresses.StepId ";
    $SQL .= "and ";
    $SQL .= "casequeue.ProcId = stepaddresses.ProcID ";
    $SQL .= "and ";
    $SQL .= "casequeue.AdHoc <> 1 ";
    $SQL .= "and ";
    $SQL .= "stepaddresses.ViewQueue = $ViewQueue ";
    if ($HideQueue == 1) {
        $SQL .= "and ";
        $SQL .= "casequeue.HideQueue = 0 ";
    }
    $SQL .= "and ";
    $SQL .= "exportkeys.CaseNum = casequeue.CaseId ";
    $SQL .= "and ";
    $SQL .= "exportkeys.ProcId = casequeue.ProcId ";
    if (!empty($OrigemUser)) {
        $SQL .= "and ";
        $SQL .= "exportkeys.Origem = origemdominio.Origem_DOC ";
        $SQL .= "and ";
        $SQL .= "origemdominio.Origem_uSER = '$OrigemUser' ";
    }
    if (is_array($Filtros)) {
        for ($i = 0; $i < count($Filtros); $i++) {
            $SQL .= "and ";
            $SQL .= $Filtros[$i];
        }
    }
    $SQL .= $GroupBy;
    return $SQL;
}

function SQLCasosCampoUsuario($ProcId, $CamposSQL, $StepId = '-1', $HideQueue = 1, $CampoOrdem = '', $OrigemUser = '', $Filtros = '', $ViewQueue = 1, $GroupBy = '')
{
    global $userdef;
    $SQL = "Select ";
    $SQL .= implode(",", $CamposSQL);
    $SQL .= " from ";
    $SQL .= "stepaddresses, ";
    $SQL .= "procfieldsdef, ";
    $SQL .= "casequeue, ";
    if (!empty($OrigemUser)) {
        $SQL .= "origemdominio, ";
    }
    $SQL .= "exportkeys, ";
    $SQL .= "fieldaddresses casedata ";
    $SQL .= "where ";
    $SQL .= "stepaddresses.GrpFld = 'F' ";
    if (!empty($ProcId)) {
        $SQL .= "and    ";
        $SQL .= "stepaddresses.ProcID = $ProcId ";
        if ($StepId != -1 && is_numeric($StepId)) {
            $SQL .= "and    ";
            $SQL .= "stepaddresses.StepId = $StepId ";
        }
    }
    $SQL .= "and ";
    $SQL .= "procfieldsdef.ProcId = stepaddresses.ProcID ";
    $SQL .= "And ";
    $SQL .= "procfieldsdef.FieldSpecialType <> 'AD' ";
    $SQL .= "and ";
    $SQL .= "procfieldsdef.FieldType = 'US' ";
    $SQL .= "and ";
    $SQL .= "stepaddresses.GroupId = procfieldsdef.FieldId ";
    $SQL .= "and ";
    $SQL .= "casequeue.StepId = stepaddresses.StepId ";
    $SQL .= "and ";
    $SQL .= "casequeue.ProcId = stepaddresses.ProcID ";
    $SQL .= "and ";
    $SQL .= "casequeue.AdHoc <> 1 ";
    $SQL .= "and ";
    $SQL .= "stepaddresses.ViewQueue = $ViewQueue ";
//    if ($StepId == 0) {
//        $SQL .= "and casequeue.CaseId in (";
//        $SQL .= "Select CaseId from casequeue ";
//        if (!empty($ProcId)) {
//            $SQL .= "where ProcId = $ProcId ";
//        }
//        $SQL .= " group by CaseId having count(*) > 1";
//        $SQL .= ")";
//    }
    if ($HideQueue == 1) {
        $SQL .= "and ";
        $SQL .= "casequeue.HideQueue = 0 ";
    }
    $SQL .= "and ";
    $SQL .= "casedata.CaseNum = casequeue.CaseId ";
    $SQL .= "and ";
    $SQL .= "casedata.FieldId = stepaddresses.GroupId ";
    $SQL .= "and ";
    $SQL .= "( ";
    $SQL .= " casedata.FieldValue = '$userdef->UserId_Process' ";
    $SQL .= " or ";
    $SQL .= " casedata.FieldValue = '$userdef->UserName'";
    $SQL .= ") ";
    $SQL .= "and ";
    $SQL .= "exportkeys.CaseNum = casequeue.CaseId ";
    $SQL .= "and ";
    $SQL .= "exportkeys.ProcId = casequeue.ProcId ";
    if (!empty($OrigemUser)) {
        $SQL .= "and ";
        $SQL .= "exportkeys.Origem = origemdominio.Origem_DOC ";
        $SQL .= "and ";
        $SQL .= "origemdominio.Origem_uSER = '$OrigemUser' ";
    }
    if (is_array($Filtros)) {
        for ($i = 0; $i < count($Filtros); $i++) {
            $SQL .= "and ";
            $SQL .= $Filtros[$i];
        }
    }
    $SQL .= $GroupBy;
    return $SQL;
}

function SQLCasosCampoGrupo($ProcId, $CamposSQL, $StepId = '-1', $HideQueue = 1, $CampoOrdem = '', $OrigemUser = '', $Filtros = '', $ViewQueue = 1, $GroupBy = '')
{
    global $userdef;
    $SQL = "Select ";
    $SQL .= implode(",", $CamposSQL);
    $SQL .= " from ";
    $SQL .= "stepaddresses, ";
    $SQL .= "procfieldsdef, ";
    $SQL .= "casequeue, ";
    if (!empty($OrigemUser)) {
        $SQL .= "origemdominio, ";
    }
    $SQL .= "exportkeys, ";
    $SQL .= "fieldaddresses casedata ";
    $SQL .= "where ";
    $SQL .= "stepaddresses.GrpFld = 'F' ";
    if (!empty($ProcId)) {
        $SQL .= "and    ";
        $SQL .= "stepaddresses.ProcID = $ProcId ";
        if ($StepId != -1 && is_numeric($StepId)) {
            $SQL .= "and    ";
            $SQL .= "stepaddresses.StepId = $StepId ";
        }
    }
    $SQL .= "and ";
    $SQL .= "procfieldsdef.ProcId = stepaddresses.ProcID ";
    $SQL .= "And ";
    $SQL .= "procfieldsdef.FieldSpecialType <> 'AD' ";
    $SQL .= "and ";
    $SQL .= "procfieldsdef.FieldType = 'GR' ";
    $SQL .= "and ";
    $SQL .= "stepaddresses.GroupId = procfieldsdef.FieldId ";
    $SQL .= "and ";
    $SQL .= "casequeue.StepId = stepaddresses.StepId ";
    $SQL .= "and ";
    $SQL .= "casequeue.ProcId = stepaddresses.ProcID ";
    $SQL .= "and ";
    $SQL .= "casequeue.AdHoc <> 1 ";
    $SQL .= "and ";
    $SQL .= "stepaddresses.ViewQueue = $ViewQueue ";
//    if ($StepId == 0) {
//        $SQL .= "and casequeue.CaseId in (";
//        $SQL .= "Select CaseId from casequeue ";
//        if (!empty($ProcId)) {
//            $SQL .= " where ProcId = $ProcId ";
//        }
//        $SQL .= " group by CaseId having count(*) > 1";
//        $SQL .= ")";
//    }
    if ($HideQueue == 1) {
        $SQL .= "and ";
        $SQL .= "casequeue.HideQueue = 0 ";
    }
    $SQL .= "and ";
    $SQL .= "casedata.CaseNum = casequeue.CaseId ";
    $SQL .= "and ";
    $SQL .= "casedata.FieldId = stepaddresses.GroupId ";
    $SQL .= "and ";
    $SQL .= " casedata.FieldValue in ($userdef->usergroups) ";
    $SQL .= "and ";
    $SQL .= "exportkeys.CaseNum = casequeue.CaseId ";
    $SQL .= "and ";
    $SQL .= "exportkeys.ProcId = casequeue.ProcId ";
    if (!empty($OrigemUser)) {
        $SQL .= "and ";
        $SQL .= "exportkeys.Origem = origemdominio.Origem_DOC ";
        $SQL .= "and ";
        $SQL .= "origemdominio.Origem_uSER = '$OrigemUser' ";
    }
    if (is_array($Filtros)) {
        for ($i = 0; $i < count($Filtros); $i++) {
            $SQL .= "and ";
            $SQL .= $Filtros[$i];
        }
    }
    $SQL .= $GroupBy;
    return $SQL;
}

function SQLCasosAdHoc($ProcId, $CamposSQL, $StepId = "-1", $HideQueue = 1, $CampoOrdem = '', $OrigemUser = '', $Filtros = '', $ViewQueue = 1, $GroupBy = '')
{
    global $userdef;
    $SQL = "Select ";
    $SQL .= implode(",", $CamposSQL);
    $SQL .= " from ";
    $SQL .= "stepaddresses, ";
    $SQL .= "procfieldsdef, ";
    $SQL .= "casequeue, ";
    $SQL .= "exportkeys, ";
    if (!empty($OrigemUser)) {
        $SQL .= "origemdominio, ";
    }
    $SQL .= "fieldaddresses as casedata ";
    $SQL .= "where ";
    $SQL .= "stepaddresses.GrpFld = 'F' ";
    $SQL .= "and ";
    $SQL .= "stepaddresses.GroupId = procfieldsdef.FieldId ";
    if (!empty($ProcId)) {
        $SQL .= "and    ";
        $SQL .= "stepaddresses.ProcID = $ProcId ";
        if ($StepId != -1 && is_numeric($StepId)) {
            $SQL .= "and    ";
            $SQL .= "stepaddresses.StepId = $StepId ";
        }
    }
    $SQL .= "and ";
    $SQL .= "procfieldsdef.ProcId = stepaddresses.ProcID ";
    $SQL .= "And ";
    $SQL .= "procfieldsdef.FieldSpecialType = 'AD'";
    $SQL .= "and ";
    $SQL .= "casequeue.StepId = stepaddresses.StepId ";
    $SQL .= "and ";
    $SQL .= "casequeue.ProcId = stepaddresses.ProcID ";
    $SQL .= "and ";
    $SQL .= "casequeue.AdHoc = 1 ";
    $SQL .= "and ";
    $SQL .= "stepaddresses.ViewQueue = $ViewQueue ";
    if ($HideQueue == 1) {
        $SQL .= "and ";
        $SQL .= "casequeue.HideQueue = 0 ";
    }
    $SQL .= "and ";
    $SQL .= "casedata.CaseNum = casequeue.CaseId ";
    $SQL .= "and ";
    $SQL .= "casedata.FieldId = stepaddresses.GroupId ";
    $SQL .= "and ";
    $SQL .= "( ";
    $SQL .= "casedata.FieldValue = '$userdef->UserId_Process' ";
    $SQL .= "or ";
    $SQL .= "casedata.FieldValue = '$userdef->UserName'";
    $SQL .= ") ";
    $SQL .= "and ";
    $SQL .= "exportkeys.CaseNum = casequeue.CaseId ";
    $SQL .= "and ";
    $SQL .= "exportkeys.ProcId = casequeue.ProcId ";
    if (!empty($OrigemUser)) {
        $SQL .= "and ";
        $SQL .= "exportkeys.Origem = origemdominio.Origem_DOC ";
        $SQL .= "and ";
        $SQL .= "origemdominio.Origem_uSER = '$OrigemUser' ";
    }
    if (is_array($Filtros)) {
        for ($i = 0; $i < count($Filtros); $i++) {
            $SQL .= "and ";
            $SQL .= $Filtros[$i];
        }
    }
    $SQL .= $GroupBy;
    return $SQL;
}

function MockCreditasGridConsulta()
{
    $SQL .= "Select ";
    $SQL .="1 as '#', ";
    $SQL .="'' as '', ";
    $SQL .="'teste' as Pedido, ";
    $SQL .="'01/01/2019' as DataCadastro, ";
    $SQL .="'Finalizado' as Status, ";
    $SQL .="'Registro' as Natureza, ";
    $SQL .="'Cartorio' as Tipo, ";
    $SQL .="'Pessoa Sobrenome' as Solicitante, ";
    $SQL .="'ExtraJudicial' as Notificacao ";
    $SQL .="from processteste.casosdousuario LIMIT 1";
    return $SQL;
}

function MockCreditasGridDetalhe()
{
    $SQL .= "Select ";
    $SQL .="1 as '#', ";
    $SQL .="'' as '', ";
    $SQL .="'teste' as Pedido, ";
    $SQL .="'01/01/2019' as DataCadastro, ";
    $SQL .="'Finalizado' as Status, ";
    $SQL .="'Registro' as Natureza, ";
    $SQL .="'Cartorio' as Tipo, ";
    $SQL .="'Pessoa Sobrenome' as Solicitante, ";
    $SQL .="'ExtraJudicial' as Notificacao, ";
    $SQL .="'' as '', ";
    $SQL .="from processteste.casosdousuario LIMIT 1";
    return $SQL;
}

function BuscaEstados()
{
    $SQL .= "Select Nome, Sigla from estados";   
    return $SQL;
}

function BuscaMunicipios($estado)
{
    $SQL .= "Select Nome, Codigo  from municipio where Sigla_Estado = '$estado'";
   
    return $SQL;
}

?>