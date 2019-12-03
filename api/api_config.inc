<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 02/12/2019
  Sistema: auto_securities
 */

function salvaConfig()
{
    global $connect;
    $dadosEntrada = apiTrataDadosEntrada(false);

    $parametrosRecebidos = array_keys($dadosEntrada);

    foreach ($parametrosRecebidos as $parametro) {
        $SQL = "delete from config where Funcao = '$parametro'";
        mysqli_query($connect, $SQL);

        $SQL = "insert into config (funcao, valor) values ('$parametro', '{$dadosEntrada[$parametro]}')";
        mysqli_query($connect, $SQL);
    }
}

/**
 * ROTA: api/v1/configs
 * @global type $connect
 */
function getConfig()
{
    global $connect;

    $SQL = "select * from config";
    $Query = mysqli_query($connect, $SQL);

    $retornoConfigs = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $configs = array();
    foreach ($retornoConfigs as $parametro) {
        $configs[$parametro['Funcao']] = $parametro["Valor"];
    }
    
    $dadosRetorno = json_encode($configs);
    
    header('Content-Type: application/json');
    echo $dadosRetorno;    
}
