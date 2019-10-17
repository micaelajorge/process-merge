<?php

/**
 * 
 * @global type $userdef
 * @global type $connect
 * @param type $ProcId
 * @return type
 */
function StartStep($ProcId) {
    global $userdef, $connect;
    $SQL = "(select NewDoc from stepaddresses where ProcId = $ProcId and GrpFld = 'G' and GroupId in ($userdef->usergroups) and NewDoc = 1)";
    $SQL .= " union ";
    $SQL .= "(select NewDoc from stepaddresses where ProcId = $ProcId and GrpFld = 'U' and GroupId = $userdef->UserId_Process and NewDoc = 1)";

    $Query = mysqli_query($connect, $SQL);
    return mysqli_num_rows($Query) > 0;
}

/**
 * 
 * @global type $connect
 * @global type $userdef
 * @param type $ProcId
 * @return type
 */
function GetNewCase($ProcId, $P_userdef = "") {
    global $connect, $userdef;
    
    if (!isset($userdef))
    {
        $userdef = $P_userdef;
    }
    
    if (!is_numeric($ProcId))
    {
        $ProcId = PegaProcIdByCode($ProcId);
    }
    
    $Hora = microtime();
    $SQL = "insert into casemap (ProcId, UserId, samaccountname, Hora) values ($ProcId, $userdef->UserId, '" . $userdef->UserName . "', '$Hora')";
    mysqli_query($connect, $SQL);
//    $SQL = "select CaseId as NRCaso from casemap where ProcId = $ProcId and UserId = $userdef->UserId_Process and samaccountname = '" . $userdef->UserName . "' and Hora = '$Hora'";
//    $Query = mysqli_query($connect, $SQL);
//    $Result = mysqli_fetch_array($Query);
//    $NrCaso = $Result["NRCaso"];
    $NrCaso = mysqli_insert_id($connect);
    return $NrCaso;
}

/**
 * 
 * @param type $AuthUser
 * @param type $ProcCode
 * @param type $StepCode
 * @param type $dadosDoCaso
 * @param type $Acao
 * @return type
 */
function FuncCaseStart($AuthUser, $ProcCode, $StepCode, $dadosDoCaso, $Acao) {
    // Autentica Usu�rio
    if (!AuthenticateWs($AuthUser)) {
        GeraErro(4);
        return -1001;
    }
    if (!is_numeric($ProcCode)) {
        $ProcId = PegaProcIdByCode($ProcCode);
    } else {
        $ProcId = $ProcCode;
    }

    if ($ProcId == 0) {
        GeraErro(13, $ProcCode);
        return -1003;
    }

    if (!is_numeric($StepCode)) {
        $StepId = PegaStepIdByCode($ProcId, $StepCode);
    } else {
        $StepId = $StepCode;
    }

    if ($StepId == 0) {
        GeraErro(14, $StepCode);
        $Err = EncerraErros();
        return -1002;
    }

    if (!StartStep($ProcId, $StepId)) {
        GeraErro(7, "Processo $ProcId, Passo $StepId  ");
        return -1005;
    }
    $CaseNum = GetNewCase($ProcId, $StepId);
    $CaseNum = FuncCaseSave($AuthUser, $ProcId, $StepId, $CaseNum, $dadosDoCaso, $Acao, 1);
    return $CaseNum;
}

// Define the method as a PHP function
function CaseStart($AuthUser, $ProcId, $StepId, $dadosDoCaso, $Acao) {
    $CaseNum = FuncCaseStart($AuthUser, $ProcId, $StepId, $dadosDoCaso, $Acao);
    $Case["CaseNum"] = $CaseNum;
    $Case["Error"] = EncerraErros();   
    return $Case;
}

function CaseNumUnique($ProcId, $UNIQUECODE, $UNIQUEVALUE) {
    global $connect;
    $retorno = 0;
    if (!is_numeric($ProcId))
    {
        $ProcId = PegaProcIdByCode($ProcId);
    }
    $CodesFields = PegaFieldIdsByCode($ProcId);
    $FieldId = $CodesFields[$UNIQUECODE];
    $SQL = "select CaseNum from exportkeys where Campo$FieldId = '$UNIQUEVALUE' and ProcId = $ProcId";
    $Query = mysqli_query($connect, $SQL);
    if (mysqli_num_rows($Query) > 0) {
        $Result = mysqli_fetch_array($Query);
        $retorno = $Result["CaseNum"];
    }
    return $retorno;
}

function CaseStartImportRM($AuthUser, $ProcId, $StepId, $dadosDoCaso, $Acao, $UNIQUECODE, $UNIQUEVALUE) {
    $CaseNum = CaseNumUnique($ProcId, $UNIQUECODE, $UNIQUEVALUE);
    if ($CaseNum > 0) {
        $StepId = "ADDDOC";
        FuncCaseSave($AuthUser, $ProcId, $StepId, $CaseNum, $dadosDoCaso, 0, 0);
    } else {
        $CaseNum = FuncCaseStart($AuthUser, $ProcId, $StepId, $dadosDoCaso, $Acao);
    }
    $Case["CaseNum"] = $CaseNum;
    $Case["Error"] = EncerraErros();
    
    //error_log("CaseNum = $CaseNum", 0);
    return $Case;
}

function FuncMultiCaseStart($AuthUser, $CasesData) {
    $Cases = array();
    for ($i = 0; $i < count($CasesData->Count); $i++) {
        $casoCriado = CaseStart($AuthUser, $CasesData->ProcId, $CasesData->StepId, $CasesData[i]->casedata, $CasesData->Action);
        array_push($Cases, $casoCriado);
    }
    return $Cases[i];
}