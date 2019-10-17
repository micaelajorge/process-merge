<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 15/11/2018
  Sistema: Process_XAMPP
 */

function insereItemArray(&$item, $key)
{
    $item["DT_RowId"] = "row_" . $key;
}

function insereTdDetalhe($aDados)
{
    array_walk($aDados, "insereItemArray");
    return $aDados;
}

function geraDetalhes($dados, $expanded, $endApi, $dimension = 1)
{
    $detalhe_old = "";
    foreach ($dados as $detalhe) {
        if ($detalhe_old != $detalhe["agrupamento"]) {
            if ($detalhe_old !== "") {
                $registros[] = $retorno;
            }
            $detalhe_old = $detalhe["agrupamento"];
            $retorno = array();
            $retorno["periodo"] = $detalhe["agrupamento"];
            $retorno["periodoSel"] = "P_" . $detalhe["agrupamento"];
            $totalCasos = 0;
            $retorno["AGUARDANDO ANALISE"] = "0";
            $retorno["REGULAR"] = "0";
            $retorno["IRREGULAR"] = "0";
            $retorno["SEM IMAGEM"] = "0";
            $retorno["dimensionDisplay"] = $dimension;
        }
        $totalCasos += $detalhe["total"];
        $retorno[$detalhe["campo34"]] = $detalhe["total"];
        $retorno["totalCasos"] = $totalCasos;
        //$retorno["pendente_porcent"] = $totalCasos / $retorno["AGUARDANDO ANALISE"];
        $retorno["analisadas"] = $totalCasos - $retorno["AGUARDANDO ANALISE"];
        if (intval($retorno["analisadas"]) > 0) {
            $retorno["pendente_porcent"] = round($retorno["IRREGULAR"] / $retorno["analisadas"] * 100, 2);
        } else {
            $retorno["pendente_porcent"] = 0;
        }
        $retorno["desvioTotal"] = round($retorno["IRREGULAR"] / $totalCasos, 2);
        $retorno["dimension"] = 1;
        $retorno["expanded"] = $expanded;
        $retorno["endApi"] = $endApi;
        $retorno["expandeble"] = true;
    }
    $registros[] = $retorno;
    return $registros;
}

function MontaCuboOne()
{
    global $connect, $dim2, $dim3, $dim4;
    $dadosEntrada = apiTrataDadosEntrada();

//    if ($dadosEntrada["where"] != "") {
//        $dimensao = $dadosEntrada["where"];
//        $sql = "select 1 as dimension, date(campo27) as dimensionDisplay, campo27 as dismensionData, 'dt' as dimensionType,  count(*) as nrRegistros from exportkeysdisplay, exportkeys where exportkeysdisplay.ProcId = 72 and exportkeys.ProcId = 72 and exportkeys.CaseNum = exportkeysdisplay.casenum and CampoDisplay28 = '$dimensao' group by date(campo27)";
//    } else {
//        $sql = "select 1 as dimension, exportkeysdisplay.CampoDisplay28 as dimensionDisplay, Campo28 as dimensionData, 'txt' as dimensionType, count(*) as nrRegistros from exportkeysdisplay, exportkeys where exportkeysdisplay.ProcId = 72 and exportkeys.ProcId = 72 "
//                //. "and exportKeys.campo27 > '2018-11-14' and exportKeys.campo27 < '2018-11-16' "
//                . "and exportkeys.CaseNum = exportkeysdisplay.CaseNum group by exportkeysdisplay.CampoDisplay28 ";
//    }

    $SQL = " select campo34, count(*) as total from exportkeys where procid = 75 and (Campo15 > '2019-05-01' and Campo15 < '2019-06-01') group by campo34";
    $query = mysqli_query($connect, $SQL);
    $dados = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $registros = array();

    $retorno = array();
    $retorno["periodo"] = "05/2019";
    $retorno["periodoSel"] = "05/2019";
    $totalCasos = 0;

    $retorno["AGUARDANDO ANALISE"] = "0";
    $retorno["REGULAR"] = "0";
    $retorno["IRREGULAR"] = "0";
    $retorno["SEM IMAGEM"] = "0";
    $retorno["dimensionDisplay"] = "1";
    $retorno["endApi"] = "";
    foreach ($dados as $detalhe) {
        $totalCasos += $detalhe["total"];
        $retorno[$detalhe["campo34"]] = $detalhe["total"];
    }
    $retorno["totalCasos"] = $totalCasos;
//    $retorno["pendente_porcent"] = round($totalCasos) $totalCasos / $retorno["AGUARDANDO ANALISE"], 2);
    if ($totalCasos == 0) {
        $retorno["pendente_porcent"] = round($retorno["Irregular"] / $totalCasos * 100, 2);
    } else {
        $retorno["pendente_porcent"] = 0;
    }
    $retorno["analisadas"] = $totalCasos - $retorno["Aguardando Analise"];
    $retorno["desvioTotal"] = round($retorno["Irregular"] / $totalCasos, 2);
    $retorno["dimension"] = 1;
    $retorno["expanded"] = true;
    $retorno["expandeble"] = false;

    $registros[] = $retorno;

    $endApi = "";
    $where = "";
    $endApiDimensao = $endApi;
    if ($dim2 != "") {
        $dim2Filtro = substr($dim2, 2);
        $where = " and campo14 = '$dim2Filtro' and (Campo15 > '2019-05-01' and Campo15 < '2019-06-01')";
        $Dim2Expanded = true;
        $endApi = "$dim2/";
    }

    $SQL = " select campo34, campo14 as agrupamento, count(*) as total from exportkeys where procid = 75 $where group by campo34, campo14 order by campo14";
    $query = mysqli_query($connect, $SQL);
    $dados = mysqli_fetch_all($query, MYSQLI_ASSOC);
    $registros = array_merge($registros, geraDetalhes($dados, $Dim2Expanded, $endApiDimensao));

    if ($Dim2Expanded) {
        $endApiDimensao = $endApi;
        if ($dim3 != "") {
            $dim3Filtro = substr($dim3, 2);
            $where .= " and campo12 = '$dim3Filtro' and (Campo15 > '2019-05-01' and Campo15 < '2019-06-01')";
            $Dim3Expanded = true;
            $endApi .= "$dim3/";
        }
        $SQL = " select campo34, campo12 as agrupamento, count(*) as total from exportkeys where procid = 75 $where group by campo34, campo12 order by campo12";
        $query = mysqli_query($connect, $SQL);
        $dados = mysqli_fetch_all($query, MYSQLI_ASSOC);
        $registros = array_merge($registros, geraDetalhes($dados, $Dim3Expanded, $endApiDimensao));
    }

    if ($Dim3Expanded) {
        $endApiDimensao = $endApi;
        if ($dim4 != "") {
            $dim4Filtro = substr($dim4, 2);
            $where .= " and campo13 = '$dim4Filtro' and (Campo15 > '2019-05-01' and Campo15 < '2019-06-01')";
            $Dim4Expanded = true;
            $endApi .= "$dim4/";
        }
        $SQL = " select campo34, campo13 as agrupamento, count(*) as total from exportkeys where procid = 75 $where group by campo34, campo13 order by campo13";
        $query = mysqli_query($connect, $SQL);
        $dados = mysqli_fetch_all($query, MYSQLI_ASSOC);
        $registros = array_merge($registros, geraDetalhes($dados, $Dim4Expanded, $endApiDimensao, 0));
    }

    $dadosComIdRow = insereTdDetalhe($registros);


    //$dadosRetorno = apiTrataDadosSaida($dadosEntrada, $retorno, $TotalRegistros, $TotalRegistros);
    //$dadosRetorno = apiTrataDadosSaida($dadosEntrada, $dadosComIdRow, $TotalRegistros, $TotalRegistros);
    $dadosRetorno = apiTrataDadosSaida($dadosEntrada, $dadosComIdRow, $TotalRegistros, $TotalRegistros);

    $jDados = json_encode($dadosRetorno);
    header('Content-Type: application/json');
    echo $jDados;
}
