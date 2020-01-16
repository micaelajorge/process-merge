<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 15/01/2020
  Sistema: Process
 */

// <editor-fold defaultstate="collapsed" desc="Inclusões do script">
require_once(FILES_ROOT . 'includes_ws/funcsws.php');
require_once(FILES_ROOT . "include/resource01.php");
require_once(FILES_ROOT . "includes_ws/wsuser.inc");
require_once(FILES_ROOT . "includes_ws/wscasestart.inc");
require_once(FILES_ROOT . "includes_ws/wscasesave.inc");
require_once(FILES_ROOT . "includes_ws/wserror.inc");

// </editor-fold>

/**
 * ROTA: api/v1/envio_documento_registro
 */
function api_gateway_registro_envio_documento_registro_v1()
{
    $dadosEntrada = apiTrataDadosEntrada();

    $procCode = "GATEWAY_DIGITAL";
    $stepCode = "INICIO";

    $procId = PegaProcIdByCode($procCode);

    $campoArquivoDocumentoData = "fileData";
    $campoArquivoDocumentoName = "fileName";

    // Pega os dados do Usuário
    $userdef = $_SESSION["userdef"];

    $caseNum = GetNewCase($procId, $userdef);

    $campoArquivo["fieldValue"] = array();
    $campoArquivo["fieldValue"]["fileName"] = $dadosEntrada[$campoArquivoDocumentoName];
    $campoArquivo["fieldValue"]["fileData"] = $dadosEntrada[$campoArquivoDocumentoData];

    $dadosCampoDocumento = criarArquivosEmCampoFolder($campoArquivo, $procId, $caseNum, "IMAGENS_INT");
//    $dadosCampoDocumento = array();
    
    $camposExcluirEnvio = array("fileName", "fileData");

    $dadosCaso["Fields"] = array();

    $campoCaso["fieldCode"] = "ORIGEM";
    $campoCaso["fieldValue"] = $dadosEntrada["cpf_cnpj"];
    $dadosCaso["Fields"][] = $campoCaso;

    $campoCaso["fieldCode"] = "NOME_DO_LOTE";
    $campoCaso["fieldValue"] = $dadosEntrada["cpf_cnpj"];
    $dadosCaso["Fields"][] = $campoCaso;

    $campoCaso["fieldCode"] = "ESTADO";
    $campoCaso["fieldValue"] = $dadosEntrada["estado"];
    $dadosCaso["Fields"][] = $campoCaso;
            
    $campoCaso["fieldCode"] = "FLG_INTEGRACAO_API";
    $campoCaso["fieldValue"] = "1";
    $dadosCaso["Fields"][] = $campoCaso;

    $campoCaso["fieldCode"] = "NOME_DO_LOTE";
    $campoCaso["fieldValue"] = $dadosEntrada["lote"];
    $dadosCaso["Fields"][] = $campoCaso;

    foreach ($dadosEntrada as $fieldCode => $fieldValue) {
        if (in_array($fieldCode, $camposExcluirEnvio)) {
            continue;
        }
        $campoCaso["fieldCode"] = $fieldCode;
        $campoCaso["fieldValue"] = $fieldValue;
        $dadosCaso["Fields"][] = $campoCaso;
    }

    $dadosCaso["Fields"] = array_merge($dadosCaso["Fields"], $dadosCampoDocumento);

    $dadosRetorno = FuncCaseSave("", $procId, $stepCode, $caseNum, $dadosCaso, WS_PROCESSAR, 1);

    header("Access-Control-Allow-Origin: *");
    header('Content-Type: application/json');
    header("HTTP/1.0 201 Created");
    
    $jDados = json_encode($dadosRetorno);
    echo $jDados;
}
