<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 31/07/2018
  Sistema: Process_XAMPP
 */
include_once(FILES_ROOT . "includes_ws/wsuser.inc");
include_once(FILES_ROOT . "includes_ws/wserror.inc");
include_once(FILES_ROOT . "includes_ws/resource01.inc");

function api_authUser()
{
    $resultado = validaUsuarioToken();
    if (!$resultado) {
        header("HTTP/1.0 403 falha autenticacao");
        $resultadoDados["result"] = "falha autenticacao";
    } else {
        $resultadoDados["result"] = "ok";
    }
    $jDados = json_encode($resultadoDados);
    header('Content-Type: application/json');
    echo $jDados;
}

function autenticausuario()
{
    $dadosEntrada = apiTrataDadosEntrada();
    $dadosRetorno = array();

    if (!key_exists($dadosEntrada)) {
        $aDadosUsuario = array();
        if (key_exists("username", $dadosEntrada)) {
            $aDadosUsuario["UserName"] = $dadosEntrada["username"];
        }
        if (key_exists("password", $dadosEntrada)) {
            $aDadosUsuario["UserPassword"] = $dadosEntrada["password"];
        }
    } else {
        $aDadosUsuario = $dadosEntrada["userdata"];
    }


    $dadosRetorno["retorno"] = AuthenticateWs($aDadosUsuario);
    $jDados = json_encode($dadosRetorno);
    if (json_last_error() > 0) {
        $erroJSON = json_last_error() . " " . json_last_error_msg();
        error_log("Json erro: " . $erroJSON);
    }
    header('Content-Type: application/json');
    header('Access-Control-Allow-Origin: http://localhost');
    header('Access-Control-Allow-Methods: GET, PUT, POST, DELETE, OPTIONS');
    header('Access-Control-Max-Age: 1000');
    header('Access-Control-Allow-Headers: Content-Type, Authorization, X-Requested-With');
    echo $jDados;
}

function autenticateduser()
{
    $userDef = $_SESSION["userdef"];
    header('Content-Type: application/json');
    $retorno = json_encode($userDef);
    echo $retorno;
}
