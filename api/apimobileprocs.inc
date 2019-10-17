<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 31/07/2018
  Sistema: Process_XAMPP
 */

include_once(FILES_ROOT . "includes_ws/wsUser.inc");
include_once(FILES_ROOT . "includes_ws/wsError.inc");
include_once(FILES_ROOT . "includes_ws/wsmobileprocs.inc");


function pegaProcessosUsuario() {
    $dadosEntrada = apiTrataDadosEntrada();

    $dadosRetorno = wsPegaProcessosUsuario($dadosEntrada["userdata"]);
    $jDados = json_encode($dadosRetorno);
    if (json_last_error() > 0) {
        $erroJSON = json_last_error() . " " . json_last_error_msg();
        error_log("Json erro: " . $erroJSON);
    }
    header('Content-Type: application/json');
    echo $jDados;
}
