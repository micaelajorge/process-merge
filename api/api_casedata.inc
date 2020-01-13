<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 18/04/2019
  Sistema: Process_XAMPP
 */
include_once(FILES_ROOT . 'includes_ws/funcsws.php');
include_once(FILES_ROOT . "include/resource01.php");
include_once(FILES_ROOT . "includes_ws/wsuser.inc");
include_once(FILES_ROOT . "includes_ws/wscasestart.inc");
include_once(FILES_ROOT . "includes_ws/wserror.inc");
include_once(FILES_ROOT . "includes_ws/wscasesave.inc");

function trataFiltro($ProcId, $filtro)
{
    $retorno = false;
    if (!isset($filtro["fieldCode"])) {
        return false;
    }

    $FieldCode = $filtro["foeldCode"];

    if (!isset($filtro["fieldValue"])) {
        return false;
    }

    $fieldValue = $filtro["fieldValue"];

    $fieldId = PegaFieldIdByCode($ProcId, $FieldCode);

    $retorno = " Campo$fieldId = '$fieldValue' ";

    return $retorno;
}

function trataFiltrosGetCase($ProcId, $filtros)
{
    $concatenador = "";
    foreach ($filtros as $filtro) {
        $expressao = trataFiltro($ProcId, $filtro);
        if ($expressao == false) {
            return false;
        }
        $where_pesquisa .= $concatenador . $expressao;
        $concatenador = " and ";
    }
    return $where_pesquisa;
}

function apigetcase()
{
    global $connect;

    $dadosCaso["Erro"] = array();

    $dadosEntrada = apiTrataDadosEntrada();

    $procCode = $dadosEntrada["procCode"];

    $procId = PegaProcIdByCode($procCode);
    $filtros = $dadosEntrada["filtros"];

    if (!is_array($filtros)) {

        $dadosCaso["Erro"]["errDesc"] = "Sem Filtro";
        $dadosCaso["Erro"]["errCode"] = "-1";
        $jDados = json_encode($dadosCaso);

        header('Content-Type: application/json');
        echo $jDados;
    }

    $expressaoFiltro = trataFiltrosGetCase($filtros, $procId);
    if (!$expressaoFiltro) {
        $dadosCaso["Erro"]["errDesc"] = "Filtro invalido";
        $dadosCaso["Erro"]["errCode"] = "-2";
        $jDados = json_encode($dadosCaso);

        header('Content-Type: application/json');
        echo $jDados;
    }
    
    
}

/**
 * ROTA: "api/apigetcasedata"
 * ROTA: "api/v1/getcasedata"
 */
function apigetcasedata_v2()
{
    global $connect;
    $dadosCaso = array();

    $dadosEntrada = apiTrataDadosEntrada();

    if (!isset($dadosEntrada["caseNum"])) {
        api_send_falta_parametros();
        return;
    }

    $caseNum = $dadosEntrada["caseNum"];
    $procId = PegaProcIdCaso($caseNum);

    if (isset($dadosEntrada["fieldCode"])) {
        $fieldCode = $dadosEntrada["fieldCode"];
        $fieldId = PegaFieldIdByCode($procId, $fieldCode);
    }

    $SQL = "select fieldcod as fieldcod, fieldtype, fieldvalue from casedata, procfieldsdef where casenum = $caseNum "
            . " and "
            . " procfieldsdef.procid = casedata.procid and "
            . " procfieldsdef.fieldId = casedata.fieldid ";
    if ($fieldId > 0) {
        $SQL .= " and casedata.fieldId = $fieldId";
    }

    $query = mysqli_query($connect, $SQL);

    $dadosCaso["caseNum"] = $caseNum;
    $dadosCaso["Fields"] = mysqli_fetch_all($query, MYSQLI_ASSOC);
    $dadosCaso["dadosEntrada"] = var_export($dadosEntrada, true);
    $dadosCaso["fieldId"] = $fieldId;
    $dadosCaso["sql"] = $SQL;

    header('Content-Type: application/json');
    $jDados = json_encode($dadosCaso);
    echo $jDados;
}

/**
 * ROTA: "api/v1/apigetcasedata"
 * ROTA: "api/v1/getcasedata"
 */
function apigetcasedata_v1()
{
    $dadosRetorno = array();

    $dadosEntrada = apiTrataDadosEntrada();
       
    if (!isset($dadosEntrada["caseNum"])) {
        header("HTTP/1.0 402 Falta Parametros");
        return;
    }
    
    $caseNum = $dadosEntrada["caseNum"];
    $procId = PegaProcIdCaso($caseNum);

    $dadosRetorno = pegaDadosCaso($procId, $caseNum);
    
    header('Content-Type: application/json');
    $jDados = json_encode($dadosRetorno);
    echo $jDados;
}

