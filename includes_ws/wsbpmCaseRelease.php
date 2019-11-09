<?php

function CaseRelease($AuthUser, $ProcCode, $StepCode, $CaseNum, $casedata)
{
    if (!is_numeric($ProcCode)) {
        $ProcId = PegaProcIdByCode($ProcCode);
    } else {
        $ProcId = $ProcCode;
    }

    if ($ProcId == 0) {
        GeraErro(13, $ProcCode);
        $Err = EncerraErros();
        $Case["CaseNum"] = $CaseNum;
        $Case["Error"] = $Err;
        return $Case;
    }

    if (!is_numeric($StepCode)) {
        $StepId = PegaStepIdByCode($ProcId, $StepCode);
    } else {
        $StepId = $StepCode;
    }
    if ($StepId == 0) {
        GeraErro(14, $StepCode);
        $Err = EncerraErros();
        $Case["CaseNum"] = $CaseNum;
        $Case["Error"] = $Err;
        return $Case;
    }

    FuncCaseRelease($AuthUser, $ProcId, $StepId, $CaseNum, $casedata);
    $Err = EncerraErros();
    $Case["CaseNum"] = $CaseNum;
    $Case["Error"] = $Err;
    return $Case;
}

function FuncCaseRelease($AuthUser, $ProcId, $StepId, $CaseNum, $casedata)
{
    global $connect, $userdef, $REMOTE_ADDR;
    if (!AuthenticateWs($AuthUser)) {
        GeraErro(4);
        return 0;
    }
    require_once("ostepdoc.inc");
    $Acao = "Processar";
    $Caso = new STEPDOC;
    $Caso->SetConnection($connect);
    $Caso->SetProc($ProcId);
    $Caso->SetStep($StepId);
    $Caso->open();
    $Caso->UserId = $userdef->UserId_Process;
    $Caso->UserName = $userdef->UserName;
    $Caso->UserDesc = $userdef->UserDesc;
    $Caso->samaccountname = $userdef->UserName;
    $Caso->NovoCaso = false;
    $Caso->SetCaseNum($CaseNum);
    $Caso->LogarAlteracoesCondicao = false;
    $Caso->LogarAlteracoesFormulario = true;
    $HoraInicio = date("Y-m-d H:i:s");
    if (count($casedata["Fields"]) > 0) {
        $FCaseNum = $CaseNum;
        $Date = date("Ymd");
        reset($casedata["Fields"]);
        if (LOG_DATA) {
            error_log("CaseNum - $CaseNum Dados recebidos");
            error_log("CaseNum - $CaseNum Origem chamada $REMOTE_ADDR");
            error_log("CaseNum - $CaseNum Processo: $ProcId, Passo: $StepId");
            error_log("CaseNum - $CaseNum Total de Campos enviados " . $casedata["NumFields"]);
        }
        while (list($key, $var) = each($casedata["Fields"])) {
            $FieldId = $var["FieldId"];
            $FieldValue = $var["Value"];
            //error_log("CaseNum $CaseNum Campo: $FieldId => Valor $FieldValue");
        }

        if (is_array($casedata["Fields"])) {
            reset($casedata["Fields"]);
        }
    }
    if ($Caso->NaFila()) {
        $Caso->PegaDadosDeArray($casedata);
        $Caso->SetAction($Acao);
    } else {
        GeraErro(1, $StepId);
    }
    if (count($casedata["Fields"]) > 0) {

        error_log("CaseNum - $CaseNum Processamento Inicio " . $HoraInicio . " T�rmino " . date("Y-m-d H:i:s"));
    }
    if (count($casedata["Fields"]) >= 0) {
        $HoraFim = date("Y-m-d H:i:s");
        $Tempo = date_diff_i($HoraInicio, $HoraFim, "s");
        error_log("CaseNum - $CaseNum $Tempo $REMOTE_ADDR $ProcId $StepId");
        error_log("-------------------------------------------------");
    }
}

?>