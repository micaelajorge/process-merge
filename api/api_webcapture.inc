<?php

include_once(FILES_ROOT . "includes_ws/wsuser.inc");
include_once(FILES_ROOT . "includes_ws/wserror.inc");
require_once(FILES_ROOT . "includes_ws/wscasesave.inc");
require_once(FILES_ROOT . "includes_ws/wscasestart.inc");
include_once(FILES_ROOT . "include/resource01.php");
/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 23/12/2018
  Sistema: Process_XAMPP
 */

/**
 * 
 * @param type $campos
 * @return type
 */
function criaCamposDoCaso($campos)
{
    $listaCampos = array();
    foreach ($campos as $idCampo => $valor) {
        $campo["FieldId"] = $idCampo;
        $campo["FieldValue"] = $valor;
        $listaCampos[] = $campo;
    }
    return $listaCampos;
}

/**
 * ROTA: api/webcapture_anexar
 */
function webcature_anexaDocumento()
{
    $dadosEntrada = apiTrataDadosEntrada();

    if (isset($_SESSION["userdef"])) {
        $userdef = $_SESSION["userdef"];
    }

    $procId = $dadosEntrada["procId"];
    $stepId = $dadosEntrada["stepId"];
    $caseNum = $dadosEntrada["caseNum"];
    $numeroCasoPai = $dadosEntrada["numeroCasoPai"];

    $camposDoProcesso = PegaProcFields($procId);

    foreach ($camposDoProcesso["Itens"] as $item) {
        if ($item["FieldType"] == "RD") {
            $idCampoReferenciaPai = $item["FieldId"];
            break;
        }
    }

    $dadosCaso = array();
    $dadosCaso["Fields"] = array();

    // Dados do Caso para criar a referencia a Pasta Pai
    $campoIdPai["fieldCode"] = $idCampoReferenciaPai;
    $campoIdPai["fieldValue"] = $numeroCasoPai;
    $dadosCaso["Fields"][] = $campoIdPai;

    $retornoCasoFilho = FuncCaseSave("", $procId, $stepId, $caseNum, $dadosCaso, 1, 0);
    echo $retornoCasoFilho;
}

/**
 * ROTA: api/webcapture_send
 */
function webcapture_recebeDados()
{
    $dadosEntrada = apiTrataDadosEntrada();

    $ProcCode = $dadosEntrada["procId"];
    $tipoCampo = $dadosEntrada["tipoCampo"];

    if (isset($_SESSION["userdef"])) {
        $userdef = $_SESSION["userdef"];
    }

    if (!is_numeric($ProcCode)) {
        $procId = PegaProcIdByCode($ProcCode);
    } else {
        $procId = $ProcCode;
    }

    //$camposProcesso = PegaFieldIdsByCode($procId);
    $camposProcesso = PegaProcFields($procId);
    $camposAgrupamento = PegaCampoUniqueClose($procId);
    $idFieldUnificador = $camposAgrupamento["FieldGrouping"];

    $valorCampoUnico = $dadosEntrada["t$idFieldUnificador"];

    $campoUnico = $camposAgrupamento["FieldUnique"];

    $dadosCaso = array();
    $dadosCaso["Fields"][] = array();

    foreach ($dadosEntrada as $campoRecebido => $campoValor) {
        $idCampo = preg_replace('/\D/', '', $campoRecebido);
        if (is_numeric($idCampo)) {
            $campoDados["fieldCode"] = $idCampo;
            $campoDados["fieldValue"] = $campoValor;
        } else {
            $campoDados["fieldCode"] = $campoRecebido;
            $campoDados["fieldValue"] = $campoValor;
        }
        $dadosCaso["Fields"][] = $campoDados;
    }

    $campoDados["fieldCode"] = $idFieldUnificador;
    $campoDados["fieldValue"] = $valorCampoUnico;
    $dadosCaso["Fields"][] = $campoDados;

    foreach ($camposProcesso["Itens"] as $campo) {
        if ($campo["FieldType"] == "FD") {
            $idCampoArquivos = $campo["FieldId"];
            break;
        }
    }

    foreach ($camposProcesso["Itens"] as $campo) {
        if ($campo["FieldType"] == "DC") {
            $idCampoReferenciaPai = $campo["FieldId"];
            break;
        }
    }

    $stepIdForm = $dadosEntrada["stepIdForm"];

    $numeroCaso = GetNewCase($procId);

    // Une os campos de imagem e dados
    $arquivosRecebidos = trataArquivosRecebidoPost();

    $dadosImagens = criarArquivosEmCampoFolder($arquivosRecebidos, $procId, $numeroCaso, $idCampoArquivos, false);

    $dadosCaso["Fields"] = array_merge($dadosImagens, $dadosCaso["Fields"]);

//    // Dados do Caso para criar a referencia a Pasta Pai
//    $campoIdPai["fieldCode"] = $idCampoReferenciaPai;
//    $campoIdPai["fieldValue"] = $numeroCasoPai;
//    $dadosCaso["Fields"][] = $campoIdPai;

    $campoDados["fieldCode"] = $idFieldUnificador;
    $campoDados["fieldValue"] = $valorCampoUnico;
    $dadosCaso["Fields"][] = $campoDados;



    $retornoCasoFilho = FuncCaseSave("", $procId, $stepIdForm, $numeroCaso, $dadosCaso, 0, 1);

    if ($tipoCampo == 'FRP') {
        header('Content-Type: application/json');
        $retorno["caseNum"] = $retornoCasoFilho;
        $retorno["fileData"] = $dadosImagens;
        echo json_encode($retorno);
    } else {
    echo $retornoCasoFilho;
    }
}
