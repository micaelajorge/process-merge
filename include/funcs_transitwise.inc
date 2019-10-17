<?php

/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 12/11/2018
Sistema: Process_XAMPP
*/

/**
 * 
 * @param type $dados
 * @param type $secretKey
 * @return type
 */
function geraChaveSeguranca($dados, $secretKey)
{
    $hash = hash_hmac('sha256', $dados, $secretKey, false);
    $retorno = $hash;
    return $retorno;
}

