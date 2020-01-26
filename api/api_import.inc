<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 21/02/2019
  Sistema: Process_XAMPP
 */

/**
 * @rota api/importdefssafedoc
 * @global type $connect
 */
function import_pegaDefinicoesSafeDoc()
{
    global $connect;
    $SQL = "select extendPropsProc, proccod from procdef where extendPropsProc <> ''";
    $Query = mysqli_query($connect, $SQL);
    $linhas = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $safedocImportDefs = array();
    foreach ($linhas as $item) {
        $json_extendProps = $item["extendPropsProc"];
        $extendProps = json_decode($json_extendProps, true);
        if (json_last_error() !== JSON_ERROR_NONE) {
            continue;
        }
        if (key_exists("safedocImport", $extendProps)) {
            if (!$extendProps["safedocImport"]["safedocEnabled"]) {
                continue;
            }
        } else {
            continue;
        }
        $extendProps["safedocImport"]["proccode"] = $item["proccod"];
        $safedocImportDefs[] = $extendProps["safedocImport"];
    }
    header('Content-Type: application/json');
    $json_ocrDefs = json_encode($safedocImportDefs, JSON_UNESCAPED_UNICODE);
    echo $json_ocrDefs;
}
