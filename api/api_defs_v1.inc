<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 26/05/2019
  Sistema: pdfimage
 */

require_once FILES_ROOT . "include/resource01.php";

/**
 * @rota api/v1/getfieldsdef
 */
function getFieldsDefList()
{
    global $connect;
    $dadosEntrada = apiTrataDadosEntrada();

    if (isset($GLOBALS["procCode"])) {
        $ProcCode = $GLOBALS["procCode"];
    } else {
        $ProcCode = $dadosEntrada["procCode"];
    }

    if (!is_int($ProcCode)) {
        $ProcId = PegaProcIdByCode($ProcCode);
    } else {
        $ProcId = $ProcCode;
    }

    if ($ProcId == 0) {
        header("HTTP/1.0 400 ProcCode não definido");
        return;
    }

    $SQL = "select * from procfieldsdef where procid = $ProcId";
    $Query = mysqli_query($connect, $SQL);
    $itens = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $dadosRetorno["fields"] = $itens;
    $dadosRetorno["numFields"] = count($itens);

    header('Content-Type: application/json');
    echo json_encode($dadosRetorno);
}
