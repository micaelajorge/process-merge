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

function pegaDadosSLA($mes, $ano)
{
    global $connect;
    $mes = str_pad($mes, 2, '0', STR_PAD_LEFT);
    if ($mes == 12) {
        $proximoMes = '01';
        $proximoAno = $ano + 1;
    } else {
        $proximoMes = str_pad($mes + 1, 2, '0', STR_PAD_LEFT);
        $proximoAno = $ano;
    }

    $whereSLA = "date(campo15) >= '$ano/$mes/01' and date(campo15) < '$proximoAno/$proximoMes/01'";
    $SQL = "select datediff(campo17, campo36) > 0 as sla, count(*) as total from exportkeys where procid = 75 and $whereSLA group by sla";
    $Query = mysqli_query($connect, $SQL);
    $linhas = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    foreach ($linhas as $itemSLA) {
        switch ($itemSLA["sla"]) {
            case 0:
                $retorno["dentroSLA"] = $itemSLA["total"];
                break;
            case 1:
                $retorno["foraSLA"] = $itemSLA["total"];
                break;
        }
    }

    return $retorno;
}

function geraDetalhes($dados, $expanded, $endApi, $dimension = 1)
{
    $detalhe_old = "";
    $retorno = "";
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
            $retorno["AGUARDANDO ANÁLISE"] = "0";
            $retorno["REGULAR"] = "0";
            $retorno["IRREGULAR"] = "0";
            $retorno["SEM IMAGEM"] = "0";
            $retorno["dimensionDisplay"] = $dimension;
        }
        $totalCasos += $detalhe["total"];
        $retorno[$detalhe["campo34"]] = $detalhe["total"];
        $retorno["totalCasos"] = $totalCasos;
        $retorno["analisadas"] = $totalCasos - $retorno["AGUARDANDO ANÁLISE"];
        if ($totalCasos == 0) {
            $retorno["tratativa"] = 0;
        } else {
            $retorno["tratativa"] = round($retorno["analisadas"] / $totalCasos * 100, 2);
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

function montaEnderecoApi($parametros, $agrupamento)
{
    $endApi = "";
    foreach ($parametros as $parametro) {
        $endApi .= "/" . $parametro;
    }
    $endApi .= "/" . str_replace('/', "&frasl;", trim($agrupamento));
    return $endApi;
}

function MontaCuboDimensao($parametros)
{
    global $connect;

    // Calcula a Dimensao
    $numeroDimensao = count($parametros);

    $mes = str_pad($parametros[1], 2, '0', STR_PAD_LEFT);
    $ano = $parametros[0];
    $inicioData = $ano . "-" . $mes . '-01';
    if ($parametros[0] == 12) {
        $mesFinal = 1;
        $anoFinal = $ano + 1;
    } else {
        $mesFinal = $mes + 1;
        $anoFinal = $ano;
    }

    //$fimData = $anoFinal . "-" . str_pad($mesFinal, 2, '0', STR_PAD_LEFT) . '-01';
    //$where = " and campo15 >= '$inicioData' and campo15 < '$fimData' ";

    switch ($numeroDimensao) {
        case 0:
            //$SQL = "select concat(month(campo15), '/', year(campo15)) as agrupamento, month(campo15) as mes, year(campo15) as ano,  upper(Campo34) as campo34, count(*) as total from exportkeys where procid = 75 and not campo15 is null group by mes, ano, agrupamento order by agrupamento";
            $SQL = "select '122019' as agrupamento, '12' as mes, '2019' as ano from estados order by agrupamento ";
            $dimensaoExpandivel = 1;
            break;
        case 2:
            //$SQL = " select upper(campo34) as campo34, campo12 as agrupamento, count(*) as total from exportkeys where procid = 75 $where group by campo12, campo34 order by campo12";
            $SQL = "select 'Creditas' as agrupamento, 'Creditas' as Cliente from estados order by agrupamento ";
            $dimensaoExpandivel = 1;
            break;
        case 3:
            $where .= " and sigla = 'SP'";
            $SQL = " select upper(campo34) as campo34, campo14 as agrupamento, count(*) as total from exportkeys where procid = 75 $where group by campo14, campo34 order by campo14";
            $filtroCaption = "/ - " . urldecode($parametros[2]);
            $dimensaoExpandivel = 1;
            break;
        case 4:
            $where .= " and campo14 = '" . urldecode($parametros[3]) . "'";
            $SQL = " select upper(campo34) as campo34, campo13 as agrupamento, count(*) as total from exportkeys where procid = 75 $where group by campo13, campo34 order by campo13";
            $filtroCaption = "/ - " . urldecode($parametros[3]);
            $dimensaoExpandivel = 0;
            break;
        case 5:
            $where .= " and campo14 = '" . urldecode($parametros[3]) . "'";
            $SQL = " select upper(campo34) as campo34, campo7 as agrupamento, count(*) as total from exportkeys where procid = 75 $where group by campo13, campo34 order by campo7";
            $filtroCaption = "/ - " . urldecode($parametros[3]);
            $dimensaoExpandivel = 0;
            break;
    }


    $query = mysqli_query($connect, $SQL);
    $dados = mysqli_fetch_all($query, MYSQLI_ASSOC);
    $registros = array();

    $retorno = array();

    $aGrupamento_old = "";
    $totalCasos = 0;

    $dadoOld = "";
    foreach ($dados as $dado) {
        $dado["agrupamento"] = htmlentities(trim($dado["agrupamento"]));
        if ($dado["agrupamento"] == "" | $dado["agrupamento"] == "#N/D") {
            $dado["agrupamento"] = "Não Localizado";
        }
        if ($aGrupamento_old !== $dado["agrupamento"] & $aGrupamento_old !== "") {
            $status = $dadoOld["Campo34"];
            $retorno[$status] = $dadoOld["total"];
            $retorno["periodo"] = $dadoOld["agrupamento"];
            $retorno["endApiComplemento"] = montaEnderecoApi($parametros, trim($dadoOld["agrupamento"]));
            $retorno["caption"] = "Periodo $mes/$ano" . "/ - " . $dadoOld["agrupamento"];



            $retorno["analisadas"] = $retorno["IRREGULAR"] + $retorno["REGULAR"];

            if ($totalCasos == 0) {
                $retorno["tratativa"] = 0;
            } else {
                $retorno["tratativa"] = round($retorno["analisadas"] / $totalCasos * 100, 2);
            }

            if ($retorno["analisadas"] == 0) {
                $retorno["eficiencia_regulares"] = 0;
                $retorno["eficiencia_iregulares"] = 0;
            } else {
                $retorno["eficiencia_iregulares"] = round($retorno["IRREGULAR"] / $retorno["analisadas"] * 100, 2);
                $retorno["eficiencia_regulares"] = round($retorno["REGULAR"] / $retorno["analisadas"] * 100, 2);
            }

            $retorno["totalCasos"] = $totalCasos;

            $retorno["AGUARDANDO ANÁLISE"] = $retorno["totalCasos"] - $retorno["analisadas"];

            $registros[] = $retorno;
            $retorno["analisadas"] = "0";
            $retorno["AGUARDANDO ANÁLISE"] = "0";
            $retorno["REGULAR"] = "0";
            $retorno["IRREGULAR"] = "0";
            $retorno["SEM IMAGEM"] = "0";
            $retorno["endApi"] = "";
            $retorno["eficiencia_regulares"] = 0;
            $retorno["eficiencia_iregulares"] = 0;
            $totalCasos = 0;
        }
        $status = $dado["campo34"];
        $retorno[$status] = $dado["total"];
        $totalCasos = $totalCasos + $dado["total"];
        $aGrupamento_old = $dado["agrupamento"];
        $dadoOld = $dado;
    }
    $retorno["periodo"] = $dadoOld["agrupamento"];
    $retorno["endApiComplemento"] = montaEnderecoApi($parametros, htmlentities(trim($dadoOld["agrupamento"])));
    $retorno["caption"] = "Periodo $mes/$ano" . "/ - " . $dadoOld["agrupamento"];
    $retorno["analisadas"] = $retorno["IRREGULAR"] + $retorno["REGULAR"];
    $retorno["totalCasos"] = $totalCasos;
    $retorno["AGUARDANDO ANÁLISE"] = $retorno["totalCasos"] - $retorno["analisadas"];
    if ($totalCasos == 0) {
        $retorno["tratativa"] = 0;
    } else {
        $retorno["tratativa"] = round($retorno["analisadas"] / $totalCasos * 100, 2);
    }

    if ($retorno["analisadas"] == 0) {
        $retorno["eficiencia_regulares"] = 0;
        $retorno["eficiencia_iregulares"] = 0;
    } else {
        $retorno["eficiencia_iregulares"] = round($retorno["IRREGULAR"] / $retorno["analisadas"] * 100, 2);
        $retorno["eficiencia_regulares"] = round($retorno["REGULAR"] / $retorno["analisadas"] * 100, 2);
    }
    $registros[] = $retorno;
    return $registros;
}

function MontaCuboMeses()
{
    global $connect, $mes_selecionado, $ano_selecionado;

    $SQL = "update exportkeys set campo13 = 'Não Localizado' where procid = 75 and campo13 = '' or campo13 = '#N/D' or campo13 is null";
    $query = mysqli_query($connect, $SQL);

    $SQL = "update exportkeys set campo14 = 'Não Localizado' where procid = 75 and campo14 = '' or campo14 = '#N/D' or campo14 is null";
    $query = mysqli_query($connect, $SQL);

    $SQL = "update exportkeys set campo12 = 'Não Localizado' where procid = 75 and campo12 = '' or campo12 = '#N/D' or campo12 is null";
    $query = mysqli_query($connect, $SQL);

    $SQL = "update exportkeys set campo13 = replace(campo13, '/', ' ') where procid = 75 and campo13 like '%/%' ";
    $query = mysqli_query($connect, $SQL);

    $SQL = "update exportkeys set campo12 = replace(campo12, '/', ' ') where procid = 75 and campo12 like '%/%'";
    $query = mysqli_query($connect, $SQL);

    $SQL = "update exportkeys set campo14 = replace(campo14, '/', ' ') where procid = 75 and campo14 like '%/%' ";
    $query = mysqli_query($connect, $SQL);

    $SQL = "update exportkeys set campo15 = replace(campo15, '/', ' ') where procid = 75 and campo15 like '%/%' ";
    $query = mysqli_query($connect, $SQL);

    $totalMesesAnteriores = 3;
    
    // Trata se é inicio do ANO para pegar meses do ano anterior
    if ($mes_selecionado - $totalMesesAnteriores - 1 <= 0)
    {
        $ano_inicio = $ano_selecionado - 1;
        $mes_inicio = 12 + 1 - $mes_selecionado;        
    } else {
        $mes_inicio = $mes_selecionado - $totalMesesAnteriores + 1;
        $ano_inicio = $ano_selecionado;
    }

    // Pega o primeiro dia do Proximo Mês
    if ($mes_selecionado == 12)
    {
        $ano_fim = $ano_selecionado + 1;
    } else {
        $mes_fim = $mes_selecionado + 1;
        $ano_fim = $ano_selecionado;        
    }
    
    $dataInicio = $ano_inicio . "/" . str_pad($mes_inicio, 2, '0', STR_PAD_LEFT) . '/01';
    $dataFim = $ano_fim . "/" . str_pad($mes_fim, 2, '0', STR_PAD_LEFT) . '/01';
    
    $whereData = "date(Campo15) >= '$dataInicio' and date(Campo15) < '$dataFim'";
            
    $SQL = "select concat(month(campo15), '/', year(campo15)) as periodo, month(campo15) as mes, year(campo15) as ano,  upper(Campo34) as campo34, count(*) as total from exportkeys where procid = 75 and not campo15 is null and $whereData group by periodo, mes, ano, campo34 order by ano desc , mes desc";
//    $SQL = "select concat(month(campo15), '/', year(campo15)) as periodo, month(campo15) as mes, year(campo15) as ano,  upper(Campo34) as campo34, count(*) as total from exportkeys where procid = 75 and not campo15 is null group by periodo, mes, ano, campo34 order by ano, mes";
//    $SQL = "select concat(month(campo15), '/', year(campo15)) as periodo, month(campo15) as mes, year(campo15) as ano,  upper(Campo34) as campo34, count(*) as total from exportkeys where procid = 75 and not campo15 is null and campo15 >= '2019-07-01' group by periodo, mes, ano, campo34 order by periodo";
    $query = mysqli_query($connect, $SQL);

    $dados = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $registros = array();

    $retorno = array();

    $retorno["whereDATA"] = $whereData;
    $periodoOld = "";
    $totalCasos = 0;

    $retorno["endApi"] = "";
    $retorno["totalCasos"] = 0;
    $retorno["dimensionDisplay"] = "1";
    $retorno["tratativa"] = 0;
    $retorno["dimensaoExpandivel"] = 1;

    $dadoOld = "";
//    foreach ($dados as $dado) {
//        if ($periodoOld !== $dado["periodo"] & $periodoOld !== "") {
//            $status = $dadoOld["campo34"];
//            $retorno[$status] = $dadoOld["total"];
//            $retorno["periodo"] = str_pad($dadoOld["mes"], 2, '0', STR_PAD_LEFT) . "/" . $dadoOld["ano"];
//
//            $slaCalculado = pegaDadosSLA($dadoOld["mes"], $dadoOld["ano"]);
//            $retorno["dentroSLA"] = $slaCalculado["dentroSLA"] . ' (' . round($slaCalculado["dentroSLA"] / $totalCasos * 100, 2) . '%)';
//            $retorno["foraSLA"] = $slaCalculado["foraSLA"] . ' (' . round($slaCalculado["foraSLA"] / $totalCasos * 100, 2) . '%)';
//
//            $retorno["periodoSel"] = array();
//            $retorno["periodoSel"]["inicio"] = $dadoOld["ano"] . "-" . str_pad($dadoOld["mes"], 2, '0', STR_PAD_LEFT) . '-01';
//            $retorno["endApiComplemento"] = "/" . $dadoOld["ano"] . "/" . $dadoOld["mes"];
//            $mes = str_pad($dadoOld["mes"], 2, '0', STR_PAD_LEFT);
//            $ano = $dadoOld["ano"];
//            $retorno["caption"] = "Periodo $mes/$ano";
//
//            $retorno["analisadas"] = $retorno["REGULAR"] + $retorno["IRREGULAR"];
//
//            if ($totalCasos == 0) {
//                $retorno["tratativa"] = 0;
//            } else {
//                $retorno["tratativa"] = round($retorno["analisadas"] / $totalCasos * 100, 2);
//            }
////            $retorno["analisadas"] = $totalCasos - $retorno["AGUARDANDO ANÁLISE"];
//            if ($retorno["IRREGULAR"] == 0) {
//                $retorno["desvioTotal"] = 0;
//            } else {
//                $retorno["desvioTotal"] = round($retorno["IRREGULAR"] / $totalCasos * 100, 2);
//            }
//            if ($retorno["analisadas"] == 0) {
//                $retorno["eficiencia_regulares"] = 0;
//                $retorno["eficiencia_iregulares"] = 0;
//            } else {
//                $retorno["eficiencia_iregulares"] = round($retorno["IRREGULAR"] / $retorno["analisadas"] * 100, 2);
//                $retorno["eficiencia_regulares"] = round($retorno["REGULAR"] / $retorno["analisadas"] * 100, 2);
//            }
//            $retorno["totalCasos"] = $totalCasos;
//            $retorno["AGUARDANDO ANÁLISE"] = $totalCasos - $retorno["analisadas"];
//            $registros[] = $retorno;
//            $retorno["REGULAR"] = "0";
//            $retorno["IRREGULAR"] = "0";
//            $retorno["SEM IMAGEM"] = "0";
//            $retorno["endApi"] = "";
//            $retorno["eficiencia_regulares"] = 0;
//            $retorno["eficiencia_iregulares"] = 0;
//            $totalCasos = 0;
//        }
//        $status = $dado["campo34"];
//        $retorno[$status] = $dado["total"];
//        $totalCasos = $totalCasos + $dado["total"];
//        $periodoOld = $dado["periodo"];
//        $dadoOld = $dado;
//    }
    $status = $dadoOld["campo34"];
    $retorno[$status] = $dadoOld["total"];
    $retorno["periodo"] = str_pad($dadoOld["mes"], 2, '0', STR_PAD_LEFT) . "/" . $dadoOld["ano"];
    $retorno["periodoSel"] = array();
    $retorno["periodoSel"]["inicio"] = $dadoOld["ano"] . "-" . str_pad($dadoOld["mes"], 2, '0', STR_PAD_LEFT) . '-01';
    $retorno["endApiComplemento"] = "/" . $dadoOld["ano"] . "/" . $dadoOld["mes"];

    $slaCalculado = pegaDadosSLA($dadoOld["mes"], $dadoOld["ano"]);
    $retorno["dentroSLA"] = $slaCalculado["dentroSLA"] . ' (' . round($slaCalculado["dentroSLA"] / $totalCasos * 100, 2) . '%)';
    $retorno["foraSLA"] = $slaCalculado["foraSLA"] . ' (' . round($slaCalculado["foraSLA"] / $totalCasos * 100, 2) . '%)';


    $mes = str_pad($dadoOld["mes"], 2, '0', STR_PAD_LEFT);
    $ano = $dadoOld["ano"];
    $retorno["caption"] = "Periodo $mes/$ano";
    $retorno["totalCasos"] = $totalCasos;

    $retorno["analisadas"] = $retorno["REGULAR"] + $retorno["IRREGULAR"];

    if ($totalCasos == 0) {
        $retorno["tratativa"] = 0;
    } else {
        $retorno["tratativa"] = round($retorno["analisadas"] / $totalCasos * 100, 2);
    }
    if ($retorno["IRREGULAR"] == 0) {
        $retorno["desvioTotal"] = 0;
    } else {
        $retorno["desvioTotal"] = round($retorno["IRREGULAR"] / $totalCasos * 100, 2);
    }
    if ($retorno["analisadas"] == 0) {
        $retorno["eficiencia_regulares"] = 0;
        $retorno["eficiencia_iregulares"] = 0;
    } else {
        $retorno["eficiencia_iregulares"] = round($retorno["IRREGULAR"] / $retorno["analisadas"] * 100, 2);
        $retorno["eficiencia_regulares"] = round($retorno["REGULAR"] / $retorno["analisadas"] * 100, 2);
    }
    $retorno["AGUARDANDO ANÁLISE"] = $totalCasos - $retorno["analisadas"];
    $registros[] = $retorno;
    return $registros;
}

function MontaCuboOne()
{
    global $PARAMETROS_ROTA;
    $dadosEntrada = apiTrataDadosEntrada();

    if (!is_array($PARAMETROS_ROTA)) {
        $registros = MontaCuboDimensao(array());
//        $registros = MontaCuboMeses();
    } else {
        $registros = MontaCuboDimensao($PARAMETROS_ROTA);
    }

    $dadosComIdRow = insereTdDetalhe($registros);

    $dadosRetorno = apiTrataDadosSaida($dadosEntrada, $dadosComIdRow, $TotalRegistros, $TotalRegistros);

    $jDados = json_encode($dadosRetorno);
    header('Content-Type: application/json');
    if (json_last_error()) {
        echo json_last_error_msg();
    }
    echo $jDados;
}
