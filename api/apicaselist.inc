<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 27/07/2018
  Sistema: Process_XAMPP

  // <editor-fold defaultstate="collapsed" desc="Includes do Script">
  require_once(FILES_ROOT . "include/resource01.php");
  require_once(FILES_ROOT . "include/resource02.php");

  // </editor-fold>

  /**
 * @rota apicaselist
 * @rota apo/apicaselist
 * @rota api/v1/apicaselist
 * 
 * @return type
 */

/**
 * @rota /api/v1/caselist
 * 
 * @global type $userdef
 * @return type
 */
function apiPegaListaCasos()
{
    global $userdef;

    $dadosEntrada = apiTrataDadosEntrada();

    // Valida usuário do token
    validaUsuarioToken();

    $userdef = $_SESSION["userdef"];

    if (!(key_exists("ProcCode", $dadosEntrada) | key_exists("procCode", $dadosEntrada))) {
        api_send_falta_parametros();
        return;
    }
    $ProcSource = ($dadosEntrada["ProcCode"]) ? $dadosEntrada["ProcCode"] : $dadosEntrada["procCode"];

    $StepCode = ($dadosEntrada["StepCode"]) ? $dadosEntrada["StepCode"] : $dadosEntrada["stepCode"];

    $Filtros = ($dadosEntrada["filters"]) ? $dadosEntrada["filters"] : "";

    $filtrosTratados = [];
    foreach ($Filtros as $filtro) {
        if (!is_numeric($filtro["fieldCode"])) {
            $fieldCode = $filtro["fieldCode"];
            $idField = PegaFieldIdByCode($ProcSource, $fieldCode);
            if ($idField === 0) {
                header("HTTP/1.0 422 Falha Buscando dados");
                header('Content-Type: application/json');
                $retorno = array();
                $retorno[] = "fieldCode:  $fieldCode, não encontrado";
                $json_retorno = json_encode($retorno);
                echo $json_retorno;
                return;
            }
            if (key_exists("fieldType", $filtro))
            {
                $filtroTratado["tipo"] = $filtro["fieldType"];
            }
            $filtroTratado["campo"] = $idField;
            $filtroTratado["valor"] = $filtro["fieldValue"];
        } else {
            $filtroTratado = $filtro;
        }
        $filtrosTratados[] = $filtroTratado;
    }

    $listaCasos = listaCasosApi($ProcSource, $StepCode, $filtrosTratados);

    if ($listaCasos === null) {
        header("HTTP/1.0 422 Falha Buscando dados");
        header('Content-Type: application/json');
        echo "{}";
        return;
    }

    $jDados = json_encode($listaCasos);

    header('Content-Type: application/json');
    echo $jDados;
}

/**
 * 
 * @global type $userdef
 * @param type $ProcSource
 * @param type $StepId
 * @param type $Filtros
 * @return type
 */
function listaCasosApi($ProcSource, $StepId, $Filtros = "")
{
    global $userdef;
    $Query = pegaListaCasosNaFila($ProcSource, $StepId, 1, "CaseNum", "", $userdef->Origem, $Filtros, 1, false);
    $retorno = mysqli_fetch_all($Query, MYSQLI_ASSOC);
    return $retorno;
}
