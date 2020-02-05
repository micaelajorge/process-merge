<?php

require_once(FILES_ROOT . "includes_ws/funcsws.php");

function FuncCaseSave($AuthUser, $ProcCode, $StepCode, $CaseNum, $dadosDoCaso, $Acao, $NovoCaso, $GravarEntrada = false)
{
    global $connect, $userdef;
    if (!is_numeric($ProcCode)) {
        $ProcId = PegaProcIdByCode($ProcCode);
    } else {
        $ProcId = $ProcCode;
    }
    if ($ProcId == 0) {
        GeraErro(13, $ProcCode);
        $error["errNum"] = -1003;
        $error["errDesc"] = "ProcCode: \"$ProcCode\"";
        return $error;
    }

    //error_log("StepID $StepCode");

    if (!is_numeric($StepCode)) {
        $StepId = PegaStepIdByCode($ProcId, $StepCode);
    } else {
        $StepId = $StepCode;
    }

    // Testa se o Passo é Zero
    if ($StepId == 0) {        
        // Verifica se Não é Execução de DeadTimes de Processo no Caso
        if (!($Acao == 3 | $Acao == 4 | $Acao == 5)) {
            GeraErro(14, $StepCode);
            return "-1002 StepCode \"$StepCode\"";
        }
    }
    
    validaUsuarioToken();

    // Verifica se o usuário já está logado
    if (!isset($_SESSION["userdef"]))
    {
//        error_log("Autenticou via Parametros");
        if (!AuthenticateWs($AuthUser)) {
            return -1001;
        }
    } else {
//        error_log("Autenticou via TOKEN");
    }
    
    try {
        require_once(FILES_ROOT . "pages/ostepdoc.inc");

        if (defined("GRAVAR_DADOS_ACAO_FuncCaseSave"))
        {
            if (GRAVAR_DADOS_ACAO_FuncCaseSave)
            {
                error_log("Entrada Dados" . var_export($dadosDoCaso, true));
            }
        }
        
        $dadosDoCasoTratados = TrataArrayCamposWebService($ProcId, $dadosDoCaso);

//        $retornoValidacao = ValidaCampos($ProcId, $StepId, $dadosDoCasoTratados);
        $retornoValidacao = true;
                               
        if (!$retornoValidacao & $Acao != 1) {
            return -1004;
        }
        
        $Caso = new STEPDOC;
        $Caso->SetConnection($connect);
        $Caso->SetProc($ProcId);

        if (LOG_DATA) {
            error_log("StepID $StepId");
        }
        $Caso->SetStep($StepId);
        $Caso->open();
        $Caso->UserId = $userdef->UserId_Process;
        $Caso->UserName = $userdef->UserName;
        $Caso->UserDesc = $userdef->UserDesc;
        $Caso->samaccountname = $userdef->UserName;
        $Caso->NovoCaso = $NovoCaso;
        $Caso->Origem = $userdef->Origem;
        $CaseNum = $Caso->SetCaseNum($CaseNum);
        $Caso->LogarAlteracoesCondicao = false;
        $Caso->LogarAlteracoesFormulario = true;
        if ($Caso->CasoEstaNaFila() | $NovoCaso) {
            $Caso->PegaDadosDeArray($dadosDoCasoTratados);
            $Caso->SetPrioridade(0);
            $Caso->GravarEntradaDados = $GravarEntrada;
            $Caso->SetAction($Acao);
            return $CaseNum;
        } else {
            error_log("Caso $CaseNum StepId: \"$StepCode: $StepId\"  não esta na fila para caso $CaseNum Processo: $ProcCode");
            return -1099;
        }
    } catch (Exception $ex) {
        error_log("Falha ao Criar Caso - $ex");
        return $ex->getMessage();
    }
    return 0;
}

/**
 * 
 * @param type $AuthUser
 * @param type $ProcId
 * @param type $CaseNum
 * @param type $StepId
 * @param type $dadosDoCaso
 * @param type $Acao
 * @return type
 */
function CaseSave($AuthUser, $ProcId, $StepId, $CaseNum, $dadosDoCaso, $Acao, $gravaEntradaDados = true)
{
    $CaseNumRetorno = FuncCaseSave($AuthUser, $ProcId, $StepId, $CaseNum, $dadosDoCaso, $Acao, 0, $gravaEntradaDados);
    $Err = EncerraErros();
    $Case["CaseNum"] = $CaseNumRetorno;
    $Case["Error"] = $Err;
    return $Case;
}

function CaseDeadTime($AuthUser, $ProcId, $StepId, $CaseNum, $dadosDoCaso, $Acao)
{
    $CaseNumRetorno = FuncCaseSave($AuthUser, $ProcId, $StepId, $CaseNum, $dadosDoCaso, $Acao, 0);
    $Err = EncerraErros();
    $Case["CaseNum"] = $CaseNumRetorno;
    $Case["Error"] = $Err;
    return $Case;
}