<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 27/07/2018
  Sistema: Process_XAMPP
 * 
 * ROTA: api/apicaselist
 */

// <editor-fold defaultstate="collapsed" desc="Includes do Script">
require_once(FILES_ROOT . "include/resource01.php");
require_once(FILES_ROOT . "include/resource02.php");

// </editor-fold>

/**
 * ROTA: apicaselist
 * ROTA: api/v1/apicaselist
 * 
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
        header("HTTP/1.0 402 Falta Parametros");
        return;
    }
    $ProcSource = ($dadosEntrada["ProcCode"]) ? $dadosEntrada["ProcCode"] : $dadosEntrada["procCode"];

//    if (!(key_exists("StepCode", $dadosEntrada) | key_exists("stepCode", $dadosEntrada))) {
//        header("HTTP/1.0 402 Falta Parametros");
//        return;
//    }
    
    $StepCode = ($dadosEntrada["StepCode"]) ? $dadosEntrada["StepCode"] : $dadosEntrada["stepCode"];

    $Filtros = ($dadosEntrada["filters"]) ? $dadosEntrada["filters"] : "";
    
    $filtrosTratados = [];
    foreach ($Filtros as $filtro)
    {
        if (!is_numeric($filtro["fieldCode"]))
        {
            $filtroTratado["campo"] = PegaFieldIdByCode($ProcSource, $filtro["fieldCode"]); 
            $filtroTratado["valor"] = $filtro["fieldValue"];
        } else {
            $filtroTratado = $filtro;
        }
        $filtrosTratados[] = $filtroTratado;
    }
    
    $listaCasos = listaCasosApi($ProcSource, $StepCode, $filtrosTratados);

    if ($listaCasos === null)
    {
        header("HTTP/1.0 403 Falha Buscando dados");
        header('Content-Type: application/json');
        echo "{}";
        return;
    }

    $jDados = json_encode($listaCasos);

    header('Content-Type: application/json');
    echo $jDados;
}

function listaCasosApi($ProcSource, $StepId, $Filtros = "")
{
    global $userdef;
    $Query = pegaListaCasosNaFila($ProcSource, $StepId, 1, "CaseNum", "", $userdef->Origem, $Filtros, 1, false);
    $retorno = mysqli_fetch_all($Query, MYSQLI_ASSOC);
    return $retorno;
}

