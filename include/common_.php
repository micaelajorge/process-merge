<?php

/** Testa para saber se já foi incluido anteriormente 
 * 
 */

require_once("error_handler.php");
include("config.config.php");
require_once("processo.php");
require_once("user.php");
$sessao_ok = session_start();

if (!$sessao_ok)
{
    error_log("Falha Iniciando Sessao " . $_SERVER["REQUEST_URI"]);
}

if(session_id() == '') {
     error_log("Falha Carregando SessionId");   
     session_start();
}

if (isset($_SESSION["userdef"])) {
    $userdef = $_SESSION["userdef"];
    error_log("Pegando usuário" . $_SERVER["DOCUMENT_ROOT"]);
} else {
    error_log("Falha Pegando usuário: " . $_SERVER["REQUEST_URI"]);
}

if (isset($common_loaded)) {
    $common_loaded = "true";
    goto SAIR;
}

//extract($_GET, EXTR_SKIP);
//extract($_POST, EXTR_SKIP);

if (!defined('FILES_ROOT')) {
    $ROOT_SERVER = $_SERVER["HTTP_HOST"];
    $currentDir = filter_input(INPUT_SERVER, "DOCUMENT_ROOT");
    if (!$currentDir) {
        $currentDir = getcwd();
        define("FILES_FOLDER", "/");
    } else {
        define("FILES_FOLDER", "/process/");
    }
    
    switch ($ROOT_SERVER) {
        case "mojo-mobil2":
        case "localhost":
            define("FILES_ROOT", $currentDir . FILES_FOLDER);
            define("SITE_ROOT", "http://localhost");
            define("SITE_FOLDER", "/Process");
            define("SITE_FOLDER_COMPLEMENT", "/");
            define("SITE_LOCATION", SITE_FOLDER . SITE_FOLDER_COMPLEMENT);
            define("FILES_UPLOAD", "E:/developer/STORAGE_PROCESS/");
            break;
        default:
            define("FILES_ROOT", $currentDir . FILES_FOLDER);
            define("SITE_ROOT", "https://" . $ROOT_SERVER);
            define("SITE_FOLDER", "/");
            define("SITE_FOLDER_COMPLEMENT", "");
            define("SITE_LOCATION", SITE_FOLDER . SITE_FOLDER_COMPLEMENT);
            define("FILES_UPLOAD", "/home/pres-switch/storage_process/");
    }
    define("SITE_PRINCIPAL_PAGE", "logon.php");
}

if (isset($_SESSION["userdef"])) {
    $userdef = $_SESSION["userdef"];
}

while (list($key, $value) = each($_POST)) {
    $_POST[$key] = preg_replace('/([\x00-\x08,\x0b-\x0c,\x0e-\x19,<,>])/', '', $value);
    $$key = $value;
}
while (list($key, $value) = each($_GET)) {
    $value = preg_replace('/([\x00-\x08,\x0b-\x0c,\x0e-\x19,<,>])/', '', $value);
    $_GET[$key] = $value;
    $$key = $value;
}

require_once("connection.php");

if (empty($EntradaXSL)) {
    $EntradaXSL = "entrada.xsl";
}

if (empty($CustomizedCSS)) {
    $CustomizedCSS = "custon-dotbr.css";
}


$conexaoOk = AtivaDBProcess(True);
if (!$conexaoOk)
{    
    // Falha acessando Banco de dados
    $MsgLogon = "Falha DB000"; 
    include(FILES_ROOT . "pages/logon.inc");
    exit;
}
SAIR:   