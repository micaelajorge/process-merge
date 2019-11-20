<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 14/10/2019
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

function MontaCuboIrregularidades()
{
    global $connect, $PARAMETROS_ROTA;

    $dadosEntrada = apiTrataDadosEntrada();

    $parametros = $PARAMETROS_ROTA;
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

    $fimData = $anoFinal . "-" . str_pad($mesFinal, 2, '0', STR_PAD_LEFT) . '-01';
    $where = " and campo15 >= '$inicioData' and campo15 < '$fimData' ";


    switch ($numeroDimensao) {
        case 2:
            $SQL = "( select concat(month(campo15), '/', year(campo15)) as periodo, month(campo15) as mes, year(campo15) as ano, count(*) as total, campo37 as Irregularidade from exportkeys where procid = 75 and campo37 <> '' and campo34 = 'Irregular' $where group by campo37
            ) union (
            select concat(month(campo15), '/', year(campo15)) as periodo, month(campo15) as mes, year(campo15) as ano, count(*) as total, campo45 as Irregularidade from exportkeys where procid = 75 and campo45 <> '' and campo34 = 'Irregular' $where group by campo45
            )";
            break;
        case 3:
            $where .= " and campo12 = '" . urldecode($parametros[2]) . "'";
            $SQL = "( select concat(month(campo15), '/', year(campo15)) as periodo, month(campo15) as mes, year(campo15) as ano, count(*) as total, campo37 as Irregularidade from exportkeys where procid = 75 and campo37 <> '' and campo34 = 'Irregular' $where  group by campo37
            ) union (
            select concat(month(campo15), '/', year(campo15)) as periodo, month(campo15) as mes, year(campo15) as ano, count(*) as total, campo45 as Irregularidade from exportkeys where procid = 75 and campo45 <> '' and campo34 = 'Irregular' $where group by campo45
            )";
            break;
        case 4:
            $where .= " and campo14 = '" . urldecode($parametros[3]) . "'";
            $SQL = "( select concat(month(campo15), '/', year(campo15)) as periodo, month(campo15) as mes, year(campo15) as ano, count(*) as total, campo37 as Irregularidade from exportkeys where procid = 75 and campo37 <> '' and campo34 = 'Irregular' $where
            ) union (
            select concat(month(campo15), '/', year(campo15)) as periodo, month(campo15) as mes, year(campo15) as ano, count(*) as total, campo45 as Irregularidade from exportkeys where procid = 75 and campo45 <> '' and campo34 = 'Irregular' $where group by campo45
            )";
            break;
        case 5:
            $where .= " and campo13 = '" . urldecode($parametros[4]) . "'";
            $SQL = "( select concat(month(campo15), '/', year(campo15)) as periodo, month(campo15) as mes, year(campo15) as ano, count(*) as total, campo37 as Irregularidade from exportkeys where procid = 75 and campo37 <> '' and campo34 = 'Irregular' $where
            ) union (
            select concat(month(campo15), '/', year(campo15)) as periodo, month(campo15) as mes, year(campo15) as ano, count(*) as total, campo45 as Irregularidade from exportkeys where procid = 75 and campo45 <> '' and campo34 = 'Irregular' $where group by campo45
            )";
            break;
    }

//    $SQL_TOTAL = "select count(*) as total from exportkeys where campo34 = 'Irregular' and Campo37 <> '' and Campo45 <> '' $where";
//    $query = mysqli_query($connect, $SQL_TOTAL);
//
//    $dadosTotal = mysqli_fetch_array($query, MYSQLI_ASSOC);
//
//    $totalCasos = $dadosTotal["total"];


    $query = mysqli_query($connect, $SQL);
    $dadosQuery = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $dadosRetorno = array();

    $totalCasos = 0;
    foreach ($dadosQuery as $irregularidade) {
        $totalCasos += $irregularidade["total"];
    }


    foreach ($dadosQuery as $irregularidade) {

        $irregularidade["porcent"] = number_format($irregularidade["total"] / $totalCasos * 100, 2);
        $irregularidade["total_acerto"] = $totalCasos;
        $dadosRetorno[] = $irregularidade;
    }

    function ordenacaoArray($a, $b)
    {
        return floatval($a["porcent"]) > floatval($b["porcent"]);
    }

    usort($dadosRetorno, "ordenacaoArray");
    
    $dadosRetorno = array_reverse($dadosRetorno);


    $dadosComIdRow = insereTdDetalhe($dadosRetorno);

    $retornoJSON = apiTrataDadosSaida($dadosEntrada, $dadosComIdRow, $TotalRegistros, $TotalRegistros);

    header('Content-Type: application/json');
    echo json_encode($retornoJSON);
}
