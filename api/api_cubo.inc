<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 13/12/2018
  Sistema: Process
 */

function pegaDimensaoSelecionada($defCubo, $dimensionIdSel = 1)
{
    foreach ($defCubo["dimensions"] as $item) {
        if ($item["dimensionId"] == $dimensionIdSel) {
            $dimensionSel = $item;
            break;
        }
    }
    return $dimensionSel;
}

function montaDisplayMonthYear($fieldId)
{    
    $sql = " concat(month(campo$fieldId),'/',year(campo$fieldId)) as DIMENSIONKEY, concat(month(campo$fieldId),'/',year(campo$fieldId)) as DIMENSIONDISPLAY ";    
    return $sql;
}

function montaDisplayText($fieldId)
{
    $sql = " campo$fieldId as dimensionKey, campoDisplay$fieldId as DIMENSIONDISPLAY";
    return $sql;
}

function montaDisplayCamposDimensao($procId, $dimensionData)
{
    $displayType = $dimensionData["displayType"];
    $fieldCode = $dimensionData["fieldSource"];
    $fieldId = PegaFieldIdByCode($procId, $fieldCode);    
    switch ($displayType) {
        case "monthYear":
            $SQL_dimension = montaDisplayMonthYear($fieldId);
            break;
        case "text":
            $SQL_dimension = montaDisplayText($fieldId);
            break;        
    }    
    return $SQL_dimension;
}

function dataInicioFim($mes, $ano)
{
    $mes = str_pad($mes, 2, '0', STR_PAD_LEFT);
    $dataInicio = $ano . "-" . $mes . '-01';
    if ($ano == 12) {
        $mesFinal = 1;
        $anoFinal = $ano + 1;
    } else {
        $mesFinal = $mes + 1;
        $anoFinal = $ano;
    }
    $dataFim = $anoFinal . "-" . str_pad($mesFinal, 2, '0', STR_PAD_LEFT) . '-01';
    $retorno = [];
    $retorno["dataInicio"] = $dataInicio;
    $retorno["dataFim"] = $dataFim;
    return $retorno;
}

function montaWhereMonthYear($fieldId, $dataSel)
{
    $dataInicioFim = dataInicioFim($dataSel[1], $dataSel[2]);
    
    $sql = " and campo$fieldId >= '{$dataInicioFim["dataInicio"]}' and campo$fieldId < '{$dataInicioFim["dataFim"]}'";
    return $sql;
}

function montaWhereParentDimensions($procId, $dimensionIdSel, $defCubo, $dataSel)
{
    foreach ($defCubo["dimensions"] as $dimension)
    {
        if ($dimension["dimensionId"] == $dimensionIdSel)
        {
            break;
        }
        $displayType = $dimension["displayType"];
        $fieldSource = $dimension["fieldSource"];
        $fieldId = PegaFieldIdByCode($procId, $fieldSource);
        switch ($displayType)
        {
            case "monthYear":
                $SQL = $SQL . montaWhereMonthYear($fieldId, $dataSel);
                break;
            case "text":
                break;
        }
    }    
    return $SQL;
}

function montaWhereDados($procId, $dimensionIdSel, $defCubo, $dataSel)
{        
    $SQL_primario = "where exportkeys.procid = $procId and exportkeysdisplay.procid = exportkeys.procid and exportkeysdisplay.casenum = exportkeys.casenum ";
    if ($dimensionIdSel == 1)
    {
        $SQL_DIMENSION_PARENTS = "";
        $SQL = $SQL_primario;
    } else {
        $SQL_DIMENSION_PARENTS = montaWhereParentDimensions($procId, $dimensionIdSel, $defCubo, $dataSel);        
    }
    $SQL = $SQL_primario . $SQL_DIMENSION_PARENTS;
    return $SQL;
}

function montaQueryDados($procId, $dimensionData)
{    
    $dimensionColuns = $dimensionData["columns"];
    $SQL = "";
    $contat = ",\n";
    foreach ($dimensionColuns as $column)
    {
        $operation = $column["operation"];
        $fieldSource = $column["fieldSource"];
        $fieldId = PegaFieldIdByCode($procId, $fieldSource);        
        switch ($operation)
        {
            case "sum":
                $SQL .= "$contat sum(campo$fieldId) as $fieldSource"; 
                break;
            case "count":
                $SQL .= "$contat count(campo$fieldId) as $fieldSource "; 
                break;
        }
    }
    return $SQL;
}

function busca_dados_dimensao()
{
    global $connect, $PARAMETROS_ROTA;
    // Pega dados enviados da API
    $dadosEntrada = apiTrataDadosEntrada(true);
    
    if (count($PARAMETROS_ROTA) == 0)
    {
        $dimensionIdSel = 1;
    } else {
        $dimensionIdSel = $PARAMETROS_ROTA[0];
    }

    $json_defCubo = file_get_contents("defcubo.json");

    $defCubo = json_decode($json_defCubo, true);
    
    $procId = PegaProcIdByCode($defCubo["procCode"]);

    $dimensionData = pegaDimensaoSelecionada($defCubo, $dimensionIdSel);        
           
    $campoDimensao = montaDisplayCamposDimensao($procId, $dimensionData);
    $camposDisplay = montaQueryDados($procId, $dimensionData);
    $whereSelect = montaWhereDados($procId, $dimensionIdSel, $defCubo, $PARAMETROS_ROTA);
    $SQL = "select $campoDimensao $camposDisplay from exportKeys, exportKeysDisplay $whereSelect group by dimensionKey";
    
    
    $query = mysqli_query($connect, $SQL);
    
    $dadosRetorno = mysqli_fetch_all($query);
    
    $TotalRegistros = count($dadosRetorno);
                
    $dadosComIdRow = insereTdDetalhe($dadosRetorno);

    $retornoJSON = apiTrataDadosSaida($dadosEntrada, $dadosComIdRow, $TotalRegistros, $TotalRegistros);

    header('Content-Type: application/json');
    echo json_encode($retornoJSON);
}
