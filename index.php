<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 01/06/2018
  Sistema: Creditus
 */

define("RELEASE_SCRIPT", "0051");

define("SYS_VERSION", "3.2.1");
define("LOG_DATA", false);

function iniciarPhpSession()
{
    $headers = getallheaders();
    if (key_exists("token", $headers) || key_exists("Authorization", $headers)) {
        ini_set('session.use_cookies', 0);
    }session_start();
}


//ini_set('display_errors', 1);
//ini_set('display_startup_errors', 1);
//error_reporting(E_ALL);
ini_set("error_reporting", E_ERROR);


$failRequireModules = false;
if (!extension_loaded('gd')) {
    echo "Modulo GD não carregado <br/>";
    $failRequireModules = true;
}

if (!extension_loaded('mysqli')) {
    echo "Modulo mysqli não carregado <br/>";
    $failRequireModules = true;
}

if (!extension_loaded('curl')) {
    echo "Modulo curl não carregado<br/>";
    $failRequireModules = true;
}

if ($failRequireModules) {
    die;
}

// <editor-fold defaultstate="collapsed" desc="Definições do Sistema">
$currentDir = __DIR__;
$enderecoServidor = filter_input(INPUT_SERVER, "HTTP_HOST");
if (empty($enderecoServidor)) {
    $enderecoServidor = $_SERVER["HTTP_HOST"];
}

$urlChamada = $_SERVER["REQUEST_URI"];

$srvAccess = $_SERVER["HTTP_HOST"];
$servidor = $srvAccess;

define("SRCACCESS", $srvAccess);

// Numero de Processos para mostrar na janela de Entrada
define("NR_PROCESSOS_SHOW_ENTRADA", 15);

define("APP_NAME", "Process");

(LOG_DATA) ? error_log("Servidor: $srvAccess") : null;

define("ARQUIVO_ROTAS", "rotas.json");

$redirectQueryString = $_SERVER["REDIRECT_QUERY_STRING"];

if (key_exists("REQUEST_URI", $_SERVER)) {
    $redirectUrl = $_SERVER["REQUEST_URI"];
} else {
    $redirectUrl = $_SERVER["REDIRECT_SCRIPT_URI"];
}

$INSTANCIA_APLICACAO = str_replace($redirectQueryString, "", $redirectUrl);

$regexPattern = '/\/(.*?)\//m';
$retorno = preg_match($regexPattern, $redirectUrl, $encontrados);
if ($retorno == 0) {
    $aliasServidor = "";
} else {
    $aliasServidor = $encontrados[1];
}

define("ALIAS_SERVIDOR", $aliasServidor);

$PaginaLogon = "logon.inc";


// </editor-fold>

$OrigemLogon = "ProcessLogon";

switch ($srvAccess) {

    // Acesso para instancias SECURITIES.
    case "clicksign-securities.com.br":
    case "ec2-3-88-59-18.compute-1.amazonaws.com":
        include("config_intancias_securities.inc");
        break;

    // <editor-fold defaultstate="collapsed" desc="plena">
    case "process.plenainformatica.com":
        define("ALINHAMENTO_LOGO", "float:left");
        /**
         *  Definições Banco de dados
         */
        $BPMUSER = "process";
        $BPMPWD = "cerberus";
        $EXTERNALUSER = "process";
        $EXTERNALPWD = "cerberus";
        $EXTERNAL_USERNAME = "process";
        $BPMDB = "process_plena";
        $EXTERNALDB = 'process_plena';
        define("ALINHAMENTO_LOGO", "float:left");
        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
        define("configDB", "localhost");
        define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
        define("_SECRET_KEY", "f9QsJ4Oufz");
        define("ALLOW_SEARCH", FALSE);
        define("FILES_FOLDER", "/");
        define("FILES_UPLOAD", "/system/transferwise/storage/files");
        define("INSTANCENAME", 'Process Demo');
        define("SITE_FOLDER", "/");
        define("SITE_FOLDER_COMPLEMENT", "");
        define("LOGO_PARCEIRO", "logo-plena.png");
        define("PARCEIRONAME", 'Plena');
        define("ICONE_PARCEIRO", "logo-plena-mini.png");
        define("URL_OWNER", "https://www.agilizaonline.com.br");
        define("NAME_OWNER", "Plena Informática");
        define("ALINHAMENTO_LOGO", "float:left");
        break;
// </editor-fold>
    // <editor-fold defaultstate="collapsed" desc="sandbox certdox">
    case "sandbox.certdox.com.br":
        define("ALINHAMENTO_LOGO", "float:left");
        define("TEMASISTEMA", "skin-red");
        /**
         *  Definições Banco de dados
         */
        $BPMUSER = "process";
        $BPMPWD = "cerberus";
        $EXTERNALUSER = "process";
        $EXTERNALPWD = "cerberus";
        $EXTERNAL_USERNAME = "process";
        $BPMDB = "process_certdox_poc";
        $EXTERNALDB = 'process_certdox_poc';
        define("ALINHAMENTO_LOGO", "float:left");
        define("SITE_ROOT", "http://$srvAccess");
        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
        define("SERVER_ADDRESS", $srvAccess);
        define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
        define("_SECRET_KEY", "f9QsJ4Oufz");
        define("ALLOW_SEARCH", FALSE);
        define("FILES_FOLDER", "/");
        define("FILES_UPLOAD", "/storage/certdox_poc");
        define("INSTANCENAME", 'Sandbox Certdox');
        define("SITE_FOLDER", "/");
        define("SITE_FOLDER_COMPLEMENT", "");
        define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
        define("PARCEIRONAME", 'Sandbox Certdox');
        define("ICONE_PARCEIRO", "icone-certdox-final.jpg");
        define("URL_OWNER", "https://www.certdox.com.br");
        define("ALINHAMENTO_LOGO", "float:left");
        define("NAME_OWNER", "Certdox");
        break;
// </editor-fold>
    // <editor-fold defaultstate="collapsed" desc="Local host">
    case "localhost":
    case "192.168.42.193":
    case "127.0.0.1":
    case "192.168.1.16":
        define("ARQUIVOS_NOVA_JANELA", "true");
        define("ALINHAMENTO_LOGO", "float:left");
//define("TEMPLATE_LOGON", "t_logon_agiliza.html");
        /**
         *  Definições Banco de dados
         */
        $BPMUSER = "root";
        $BPMPWD = "";
        $EXTERNALUSER = "root";
        $EXTERNALPWD = "";
        $EXTERNAL_USERNAME = "root";
        switch ($aliasServidor) {
            case "process-teste":
                define("TEMASISTEMA", "skin-red");
                define("INSTANCENAME", 'Process Teste');
                define("SITE_FOLDER", "/process-teste/");
                define("INSTANCENAME", 'Process Teste');
                define("FILES_UPLOAD", "E:/developer/STORAGE_PROCESS_TESTE");
                define("configDB", "db-teste");
                $BPMDB = "tribanco_teste";
                $EXTERNALDB = 'tribanco_teste';
                break;

            case "dmcard":
                define("TEMASISTEMA", "skin-red");
                define("TEMPLATE_LOGON", "t_logon_dmcard.html");
                define("INSTANCENAME", 'DM Card');
                define("SITE_FOLDER", "/dmcard/");
                define("INSTANCENAME", 'DM Card');
                define("FILES_UPLOAD", "E:/developer/STORAGE_PROCESS_TESTE");
                define("configDB", "db-teste");
                $BPMDB = "process-new";
                $EXTERNALDB = 'process-new';
                break;

            case "securities_start":
                define("TEMASISTEMA", "skin-red");
                define("INSTANCENAME", 'Securities Start');
                define("SITE_FOLDER", "/securities_start/");
                define("FILES_UPLOAD", "E:/developer/STORAGE_PROCESS_TESTE");
                define("PARCEIRONAME", 'Clicksign');
                define("ARQUIVO_ERROR_LOG_PHP", "log_securities_start/PHP_errors.log");
                $BPMDB = "securities_start";
                $EXTERNALDB = 'securities_start';
                break;

            default:
                setcookie("XDEBUG_SESSION", "netbeans-xdebug");
                define("INSTANCENAME", 'Process Desenv');
                define("SITE_FOLDER", "/process/");
                define("INSTANCENAME", 'Process Local');
                define("FILES_UPLOAD", "E:/developer/STORAGE_PROCESS");
                define("configDB", "localhost");
                define("PARCEIRONAME", 'Default');
//                $BPMDB = "securities_start";
//                $EXTERNALDB = "securities_start";
                $BPMDB = "processteste";
                $EXTERNALDB = "processteste";
                break;
        }
        define("FILES_FOLDER", "\\");
        define("SITE_ROOT", "http://$srvAccess");
        define("SITE_FOLDER_COMPLEMENT", "");
        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
        define("SERVER_ADDRESS", $srvAccess);
        if (!defined("ARQUIVO_ERROR_LOG_PHP")) {
            define("ARQUIVO_ERROR_LOG_PHP", "log_local/PHP_errors.log");
        }
        define("_SECRET_KEY", "f9QsJ4Oufz");
        define("ALLOW_SEARCH", false);
        define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
        define("LOGO_PARCEIRO_TOP", "icone-certdox-final.jpg");
        define("ICONE_PARCEIRO", "icone-certdox-final.jpg");
        define("URL_OWNER", "https://www.certdox.com.br");
        define("NAME_OWNER", "Cerdox");
        break;
    // </editor-fold>

    case "alicred.securities.com.br":
        define("ALINHAMENTO_LOGO", "float:left");
        define("TEMASISTEMA", "skin-blue-light");
        /**
         *  Definições Banco de dados
         */
        $BPMUSER = "process";
        $BPMPWD = "cerberus";
        $EXTERNALUSER = "process";
        $EXTERNALPWD = "cerberus";
        $EXTERNAL_USERNAME = "process";
        $BPMDB = "prod_alicred";
        $EXTERNALDB = 'prod_alicred';
        define("ALINHAMENTO_LOGO", "float:left");
        define("SITE_ROOT", "http://$srvAccess");
        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
        define("SERVER_ADDRESS", $srvAccess);
        define("ARQUIVO_ERROR_LOG_PHP", "log_alicred/PHP_errors.log");
        define("_SECRET_KEY", "f9QsJ4Oufz");
        define("ALLOW_SEARCH", FALSE);
        define("FILES_FOLDER", "/");
        define("FILES_UPLOAD", "/storage/alicred");
        define("INSTANCENAME", 'Alicred');
        define("SITE_FOLDER", "/");
        define("SITE_FOLDER_COMPLEMENT", "");
        define("LOGO_PARCEIRO", "logo-clicksign.png");
        define("PARCEIRONAME", 'ClickSign');
        define("ICONE_PARCEIRO", "icone_clicksign-mini.png");
        define("URL_OWNER", "https://app.clicksign.com");
        define("ALINHAMENTO_LOGO", "float:left");
        define("NAME_OWNER", "Powered by Certdox");
        break;

    case "creditas-scd.certdox.com.br":
        define("ALINHAMENTO_LOGO", "float:left");
        define("TEMASISTEMA", "skin-blue-light");
        /**
         *  Definições Banco de dados
         */
        $BPMUSER = "process";
        $BPMPWD = "cerberus";
        $EXTERNALUSER = "process";
        $EXTERNALPWD = "cerberus";
        $EXTERNAL_USERNAME = "process";
        $BPMDB = "gateway_creditas_scd";
        $EXTERNALDB = 'gateway_creditas_scd';
        define("ALINHAMENTO_LOGO", "float:left");
        define("SITE_ROOT", "http://$srvAccess");
        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
        define("SERVER_ADDRESS", $srvAccess);
        define("ARQUIVO_ERROR_LOG_PHP", "log_gateway_creditas_scd/PHP_errors.log");
        define("_SECRET_KEY", "f9QsJ4Oufz");
        define("ALLOW_SEARCH", FALSE);
        define("FILES_FOLDER", "/");
        define("FILES_UPLOAD", "/storage/gateway_creditas_scd");
        define("INSTANCENAME", 'Gateway Creditas');
        define("SITE_FOLDER", "/gateway_creditas_scd/"); // Nome do Alias no APACHE
        define("SITE_FOLDER_COMPLEMENT", "");
        define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
        define("PARCEIRONAME", 'Certdox');
        define("ICONE_PARCEIRO", "icone-certdox-final.jpg");
        define("URL_OWNER", "https://www.clicksign-securities.com");
        define("ALINHAMENTO_LOGO", "float:left");
        define("NAME_OWNER", "Powered by Certdox");
        break;


    case "creditas-scd.securities.com.br":
    case "creditasscd.securities.com.br":
        define("ALINHAMENTO_LOGO", "float:left");
        define("TEMASISTEMA", "skin-blue-light");
        /**
         *  Definições Banco de dados
         */
        $BPMUSER = "process";
        $BPMPWD = "cerberus";
        $EXTERNALUSER = "process";
        $EXTERNALPWD = "cerberus";
        $EXTERNAL_USERNAME = "process";
        $BPMDB = "prod_creditas_scd";
        $EXTERNALDB = 'prod_creditas_scd';
        define("ALINHAMENTO_LOGO", "float:left");
        define("SITE_ROOT", "http://$srvAccess");
        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
        define("SERVER_ADDRESS", $srvAccess);
        define("ARQUIVO_ERROR_LOG_PHP", "log_creditas_scd/PHP_errors.log");
        define("_SECRET_KEY", "f9QsJ4Oufz");
        define("ALLOW_SEARCH", FALSE);
        define("FILES_FOLDER", "/");
        define("FILES_UPLOAD", "/storage/creditas_scd");
        define("INSTANCENAME", 'Creditas SCD');
        define("SITE_FOLDER", "/"); // Nome do Alias no APACHE
        define("SITE_FOLDER_COMPLEMENT", "");
        define("LOGO_PARCEIRO", "logo-clicksign.png");
        define("PARCEIRONAME", 'Clicksign Securities');
        define("ICONE_PARCEIRO", "icone_clicksign-mini.png");
        define("URL_OWNER", "https://www.clicksign-securities.com");
        define("ALINHAMENTO_LOGO", "float:left");
        define("NAME_OWNER", "Powered by Certdox");
        break;

    case "bpc.securities.com.br":
        define("ALINHAMENTO_LOGO", "float:left");
        define("TEMASISTEMA", "skin-blue-light");
        /**
         *  Definições Banco de dados
         */
        $BPMUSER = "process";
        $BPMPWD = "cerberus";
        $EXTERNALUSER = "process";
        $EXTERNALPWD = "cerberus";
        $EXTERNAL_USERNAME = "process";
        $BPMDB = "prod_bpc";
        $EXTERNALDB = 'prod_bpc';
        define("ALINHAMENTO_LOGO", "float:left");
        define("SITE_ROOT", "http://$srvAccess");
        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
        define("SERVER_ADDRESS", $srvAccess);
        define("ARQUIVO_ERROR_LOG_PHP", "log_bpc/PHP_errors.log");
        define("_SECRET_KEY", "f9QsJ4Oufz");
        define("ALLOW_SEARCH", FALSE);
        define("FILES_FOLDER", "/");
        define("FILES_UPLOAD", "/storage/bpc");
        define("INSTANCENAME", 'BPC');
        define("SITE_FOLDER", "/"); // Nome do Alias no APACHE
        define("SITE_FOLDER_COMPLEMENT", "");
        define("LOGO_PARCEIRO", "logo-clicksign.png");
        define("PARCEIRONAME", 'Clicksign Securities');
        define("ICONE_PARCEIRO", "icone_clicksign-mini.png");
        define("URL_OWNER", "https://www.clicksign-securities.com");
        define("ALINHAMENTO_LOGO", "float:left");
        define("NAME_OWNER", "Powered by Certdox");
        break;

    case "clicksign.certdox.com.br":
        define("ALINHAMENTO_LOGO", "float:left");
        define("TEMASISTEMA", "skin-red");
        /**
         *  Definições Banco de dados
         */
        $BPMUSER = "process";
        $BPMPWD = "cerberus";
        $EXTERNALUSER = "process";
        $EXTERNALPWD = "cerberus";
        $EXTERNAL_USERNAME = "process";
        $BPMDB = "process_certdox_poc";
        $EXTERNALDB = 'process_certdox_poc';
        define("ALINHAMENTO_LOGO", "float:left");
        define("SITE_ROOT", "http://$srvAccess");
        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
        define("SERVER_ADDRESS", $srvAccess);
        define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
        define("_SECRET_KEY", "f9QsJ4Oufz");
        define("ALLOW_SEARCH", FALSE);
        define("FILES_FOLDER", "/");
        define("FILES_UPLOAD", "/storage/certdox_poc");
        define("INSTANCENAME", 'Demo Itau]');
        define("SITE_FOLDER", "/");
        define("SITE_FOLDER_COMPLEMENT", "");
        define("LOGO_PARCEIRO", "logo-clicksign.png");
        define("PARCEIRONAME", 'ClickSign');
        define("ICONE_PARCEIRO", "icone_clicksign-mini.png");
        define("URL_OWNER", "https://app.clicksign.com");
        define("ALINHAMENTO_LOGO", "float:left");
        define("NAME_OWNER", "ClickSign");
        break;

    case "process.certdox.com.br":
    case "40.114.46.89":
    case "securities.com.br":
        define("ARQUIVOS_NOVA_JANELA", "true");
        switch ($aliasServidor) {
            case "homolog_creditas_consignado":
                define("ALINHAMENTO_LOGO", "float:left");
                define("TEMASISTEMA", "skin-red");
                /**
                 *  Definições Banco de dados
                 */
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "homolog_creditas_consignado";
                $EXTERNALDB = 'homolog_creditas_consignado';
                define("ALINHAMENTO_LOGO", "float:left");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log_homolog_creditas_consignado/PHP_errors.log");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", FALSE);
                define("FILES_FOLDER", "/");
                define("FILES_UPLOAD", "/storage/gateway_creditas_scd");
                define("INSTANCENAME", 'Homolog Creditas Consignado');
                define("SITE_FOLDER", "/homolog_creditas_consignado/"); // Nome do Alias no APACHE
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
                define("PARCEIRONAME", 'Securities');
                define("ICONE_PARCEIRO", "icone-certdox-final.jpg");
                define("URL_OWNER", "https://www.certdox.com.br");
                define("ALINHAMENTO_LOGO", "float:left");
                define("NAME_OWNER", "Powered by Certdox");
                break;

            case "creditas_consignado":
                define("ALINHAMENTO_LOGO", "float:left");
                define("TEMASISTEMA", "skin-red");
                /**
                 *  Definições Banco de dados
                 */
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "prod_creditas_consignado";
                $EXTERNALDB = 'prod_creditas_consignado';
                define("ALINHAMENTO_LOGO", "float:left");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log_prod_creditas_consignado/PHP_errors.log");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", FALSE);
                define("FILES_FOLDER", "/");
                define("FILES_UPLOAD", "/storage/prod_creditas_consignado");
                define("INSTANCENAME", 'Creditas Consignado');
                define("SITE_FOLDER", "/creditas_consignado/"); // Nome do Alias no APACHE
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
                define("PARCEIRONAME", 'Securities');
                define("ICONE_PARCEIRO", "icone-certdox-final.jpg");
                define("URL_OWNER", "https://www.certdox.com.br");
                define("ALINHAMENTO_LOGO", "float:left");
                define("NAME_OWNER", "Powered by Certdox");
                break;

            case "homolog_creditas_sorocred":
                define("ALINHAMENTO_LOGO", "float:left");
                define("TEMASISTEMA", "skin-red");
                /**
                 *  Definições Banco de dados
                 */
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "homolog_creditas_sorocred";
                $EXTERNALDB = 'homolog_creditas_sorocred';
                define("ALINHAMENTO_LOGO", "float:left");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log_gateway_creditas_scd/PHP_errors.log");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", FALSE);
                define("FILES_FOLDER", "/");
                define("FILES_UPLOAD", "/storage/gateway_creditas_scd");
                define("INSTANCENAME", 'Homolog Creditas Sorocred');
                define("SITE_FOLDER", "/gateway_creditas_scd/"); // Nome do Alias no APACHE
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
                define("PARCEIRONAME", 'Securities');
                define("ICONE_PARCEIRO", "icone-certdox-final.jpg");
                define("URL_OWNER", "https://www.certdox.com.br");
                define("ALINHAMENTO_LOGO", "float:left");
                define("NAME_OWNER", "Powered by Certdox");
                break;

            case "gateway_creditas_scd":
                define("ALINHAMENTO_LOGO", "float:left");
                define("TEMASISTEMA", "skin-blue-light");
                /**
                 *  Definições Banco de dados
                 */
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "prod_gateway_creditas_scd";
                $EXTERNALDB = 'prod_gateway_creditas_scd';
                define("ALINHAMENTO_LOGO", "float:left");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log_gateway_creditas_scd/PHP_errors.log");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", FALSE);
                define("FILES_FOLDER", "/");
                define("FILES_UPLOAD", "/storage/gateway_creditas_scd");
                define("INSTANCENAME", 'Gateway Creditas');
                define("SITE_FOLDER", "/gateway_creditas_scd/"); // Nome do Alias no APACHE
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
                define("PARCEIRONAME", 'Certdox');
                define("ICONE_PARCEIRO", "icone-certdox-final.jpg");
                define("URL_OWNER", "https://www.certdox.com.br");
                define("ALINHAMENTO_LOGO", "float:left");
                define("NAME_OWNER", "Powered by Certdox");
                break;
            
            
            case "gatewaydigital":
                define("ALINHAMENTO_LOGO", "float:left");
                define("TEMASISTEMA", "skin-red");
                define("TEMPLATE_LOGON", "t_logon_dmcard.html");

                /**
                 *  Definições Banco de dados
                 */
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "process_certdox_poc";
                $EXTERNALDB = 'process_certdox_poc';
                define("ALINHAMENTO_LOGO", "float:left");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", FALSE);
                define("FILES_FOLDER", "/");
                define("FILES_UPLOAD", "/storage/certdox_poc");
                define("INSTANCENAME", 'DM Card Certdox');
                define("SITE_FOLDER", "/");
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
                define("PARCEIRONAME", 'DM Card Certdox');
                define("ICONE_PARCEIRO", "icone-certdox-final.jpg");
                define("URL_OWNER", "https://www.certdox.com.br");
                define("ALINHAMENTO_LOGO", "float:left");
                define("NAME_OWNER", "Certdox");
                break;

            case "teste_securities":
                define("ALINHAMENTO_LOGO", "float:left");
                define("TEMASISTEMA", "skin-blue");
                /**
                 *  Definições Banco de dados
                 */
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "teste_securities";
                $EXTERNALDB = 'teste_securities';
                define("ALINHAMENTO_LOGO", "float:left");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log_teste_securities/PHP_errors.log");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", FALSE);
                define("FILES_FOLDER", "/");
                define("FILES_UPLOAD", "/storage/teste_securities");
                define("INSTANCENAME", 'teste_securities');
                define("SITE_FOLDER", "/teste_securities/");
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
                define("PARCEIRONAME", 'Teste Securities');
                define("ICONE_PARCEIRO", "icone-certdox-final.jpg");
                define("URL_OWNER", "https://www.clicksign-securities.com");
                define("ALINHAMENTO_LOGO", "float:left");
                define("NAME_OWNER", "Clicksign Securities");
                break;

            case "homolog_bpc":
                define("ALINHAMENTO_LOGO", "float:left");
                define("TEMASISTEMA", "skin-blue");
                /**
                 *  Definições Banco de dados
                 */
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "homolog_bpc";
                $EXTERNALDB = 'homolog_bpc';
                define("ALINHAMENTO_LOGO", "float:left");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log_bpc/PHP_errors.log");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", FALSE);
                define("FILES_FOLDER", "/");
                define("FILES_UPLOAD", "/storage/homolog_bpc");
                define("INSTANCENAME", 'homolog_bpc');
                define("SITE_FOLDER", "/homolog_bpc/");
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
                define("PARCEIRONAME", 'Clicksign Securities');
                define("ICONE_PARCEIRO", "icone-certdox-final.jpg");
                define("URL_OWNER", "https://www.clicksign-securities.com");
                define("ALINHAMENTO_LOGO", "float:left");
                define("NAME_OWNER", "Clicksign Securities");
                break;

            case "idtrust":
                define("ALINHAMENTO_LOGO", "float:left");
                /**
                 *  Definições Banco de dados
                 */
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "process_idtrust";
                $EXTERNALDB = 'process_idtrust';
                define("ALINHAMENTO_LOGO", "float:left");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", FALSE);
                define("FILES_FOLDER", "/");
                define("FILES_UPLOAD", "/storage/idtrust");
                define("INSTANCENAME", 'Id Trust');
                define("SITE_FOLDER", "/");
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
                define("PARCEIRONAME", 'Id Trust Certdox');
                define("ICONE_PARCEIRO", "icone-certdox-final.jpg");
                define("URL_OWNER", "https://www.certdox.com.br");
                define("ALINHAMENTO_LOGO", "float:left");
                define("NAME_OWNER", "Certdox");
                break;

            case "sandbox":
                define("ALINHAMENTO_LOGO", "float:left");
                define("TEMASISTEMA", "skin-red");
                /**
                 *  Definições Banco de dados
                 */
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "process_certdox_poc";
                $EXTERNALDB = 'process_certdox_poc';
                define("ALINHAMENTO_LOGO", "float:left");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", FALSE);
                define("FILES_FOLDER", "/");
                define("FILES_UPLOAD", "/storage/certdox_poc");
                define("INSTANCENAME", 'Sandbox Certdox');
                define("SITE_FOLDER", "/");
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
                define("PARCEIRONAME", 'Sandbox Certdox');
                define("ICONE_PARCEIRO", "icone-certdox-final.jpg");
                define("URL_OWNER", "https://www.certdox.com.br");
                define("ALINHAMENTO_LOGO", "float:left");
                define("NAME_OWNER", "Certdox");
                break;

            case "alicred":
                define("ALINHAMENTO_LOGO", "float:left");
                define("TEMASISTEMA", "skin-red");
                /**
                 *  Definições Banco de dados
                 */
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "process_certdox_poc";
                $EXTERNALDB = 'process_certdox_poc';
                define("ALINHAMENTO_LOGO", "float:left");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", FALSE);
                define("FILES_FOLDER", "/");
                define("FILES_UPLOAD", "/storage/certdox_poc");
                define("INSTANCENAME", 'Sandbox Certdox');
                define("SITE_FOLDER", "/alicred/");
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
                define("PARCEIRONAME", 'Sandbox Alicred - Homolog');
                define("ICONE_PARCEIRO", "icone-certdox-final.jpg");
                define("URL_OWNER", "https://www.certdox.com.br");
                define("ALINHAMENTO_LOGO", "float:left");
                define("NAME_OWNER", "AliCred");
                break;

            case "geru":
                define("ALINHAMENTO_LOGO", "float:left");
                define("TEMASISTEMA", "skin-blue");
                /**
                 *  Definições Banco de dados
                 */
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "prod_geru";
                $EXTERNALDB = 'prod_geru';
                define("ALINHAMENTO_LOGO", "float:left");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log_geru/PHP_errors.log");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", FALSE);
                define("FILES_FOLDER", "/");
                define("FILES_UPLOAD", "/storage/geru");
                define("INSTANCENAME", 'Geru Produção');
                define("SITE_FOLDER", "/geru/"); // Nome do Alias no APACHE
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-clicksign.png");
                define("PARCEIRONAME", 'Clicksign Securities');
                define("ICONE_PARCEIRO", "icone_clicksign-mini.png");
                define("URL_OWNER", "https://www.clicksign-securities.com");
                define("ALINHAMENTO_LOGO", "float:left");
                define("NAME_OWNER", "Powerd by Certdox");
                break;

            case "homolog_gateway_creditas_scd":
                define("ALINHAMENTO_LOGO", "float:left");
                define("TEMASISTEMA", "skin-green");
                /**
                 *  Definições Banco de dados
                 */
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "homolog_gateway_creditas_scd";
                $EXTERNALDB = 'gateway_creditas_scd';
                define("ALINHAMENTO_LOGO", "float:left");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log_gateway_creditas_scd/PHP_errors.log");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", FALSE);
                define("FILES_FOLDER", "/");
                define("FILES_UPLOAD", "/storage/homolog_gateway_creditas_scd");
                define("INSTANCENAME", 'Gateway Creditas');
                define("SITE_FOLDER", "/homolog_gateway_creditas_scd/"); // Nome do Alias no APACHE
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
                define("PARCEIRONAME", 'Certdox');
                define("ICONE_PARCEIRO", "icone-certdox-final.jpg");
                define("URL_OWNER", "https://www.clicksign-securities.com");
                define("ALINHAMENTO_LOGO", "float:left");
                define("NAME_OWNER", "Powered by Certdox");
                break;

            case "homolog_creditas_scd":
                define("ALINHAMENTO_LOGO", "float:left");
                define("TEMASISTEMA", "skin-red");
                /**
                 *  Definições Banco de dados
                 */
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "homolog_creditas_scd";
                $EXTERNALDB = 'creditas_scd';
                define("ALINHAMENTO_LOGO", "float:left");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log_creditas_scd/PHP_errors.log");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", FALSE);
                define("FILES_FOLDER", "/");
                define("FILES_UPLOAD", "/storage/homolog_creditas_scd");
                define("INSTANCENAME", 'Creditas SCD');
                define("SITE_FOLDER", "/homolog_creditas_scd/"); // Nome do Alias no APACHE
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-clicksign.png");
                define("PARCEIRONAME", 'Clicksign Securities');
                define("ICONE_PARCEIRO", "icone_clicksign-mini.png");
                define("URL_OWNER", "https://www.clicksign-securities.com");
                define("ALINHAMENTO_LOGO", "float:left");
                define("NAME_OWNER", "Powered by Certdox");
                break;

            case "creditas_scd":
                define("ALINHAMENTO_LOGO", "float:left");
                define("TEMASISTEMA", "skin-blue-light");
                /**
                 *  Definições Banco de dados
                 */
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "creditas_scd";
                $EXTERNALDB = 'creditas_scd';
                define("ALINHAMENTO_LOGO", "float:left");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log_creditas_scd/PHP_errors.log");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", FALSE);
                define("FILES_FOLDER", "/");
                define("FILES_UPLOAD", "/storage/creditas_scd");
                define("INSTANCENAME", 'Creditas SCD');
                define("SITE_FOLDER", "/creditas_scd/"); // Nome do Alias no APACHE
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-clicksign.png");
                define("PARCEIRONAME", 'Clicksign Securities');
                define("ICONE_PARCEIRO", "icone_clicksign-mini.png");
                define("URL_OWNER", "https://www.clicksign-securities.com");
                define("ALINHAMENTO_LOGO", "float:left");
                define("NAME_OWNER", "Powered by Certdox");
                break;


            case "clicksign":
                define("ALINHAMENTO_LOGO", "float:left");
                define("TEMASISTEMA", "skin-blue");
                /**
                 *  Definições Banco de dados
                 */
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "process_certdox_poc";
                $EXTERNALDB = 'process_certdox_poc';
                define("TEMPLATE_LOGON", "t_logon_logo_top.html");
                define("ALINHAMENTO_LOGO", "float:left");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", FALSE);
                define("FILES_FOLDER", "/");
                define("FILES_UPLOAD", "/storage/certdox_poc");
                define("INSTANCENAME", 'Clicksign');
                define("SITE_FOLDER", "/clicksign/");
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-clicksign.png");
                define("PARCEIRONAME", 'Clicksign Securities');
                define("ICONE_PARCEIRO", "icone_clicksign-mini.png");
                define("URL_OWNER", "https://www.certdox.com.br");
                define("ALINHAMENTO_LOGO", "float:left");
                define("NAME_OWNER", "Powered by Certdox");
                define("LOGO_PARCEIRO_TOP", "logo-clicksign-top.png");
                break;

            case "homolog_geru":
                define("ALINHAMENTO_LOGO", "float:left");
                define("TEMASISTEMA", "skin-red");
                /**
                 *  Definições Banco de dados
                 */
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "process_certdox_geru";
                $EXTERNALDB = 'process_certdox_geru';
                define("ALINHAMENTO_LOGO", "float:left");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", FALSE);
                define("FILES_FOLDER", "/");
                define("FILES_UPLOAD", "/storage/homolog_geru");
                define("INSTANCENAME", 'Sandbox Geru');
                define("SITE_FOLDER", "/homolog_geru/");
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
                define("PARCEIRONAME", 'Sandbox Geru');
                define("ICONE_PARCEIRO", "logo-clicksign.png");
                define("URL_OWNER", "https://www.clicksign-securities.com");
                define("ALINHAMENTO_LOGO", "float:left");
                define("NAME_OWNER", "Powered by Certdox");
                break;

            case "creditas":
                define("ALINHAMENTO_LOGO", "float:left");
                define("TEMASISTEMA", "skin-red");
                /**
                 *  Definições Banco de dados
                 */
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "process_certdox_creditas";
                $EXTERNALDB = 'process_certdox_creditas';
                define("ALINHAMENTO_LOGO", "float:left");
                define("SITE_ROOT", "http://$srvAccess/creditas/");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", FALSE);
                define("FILES_FOLDER", "/");
                define("FILES_UPLOAD", "/storage/creditas");
                define("INSTANCENAME", 'Sandbox Creditas');
                define("SITE_FOLDER", "/creditas/");
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
                define("PARCEIRONAME", 'Sandbox Creditas');
                define("ICONE_PARCEIRO", "icone-certdox-final.jpg");
                define("URL_OWNER", "https://www.clicksign-securities.com");
                define("ALINHAMENTO_LOGO", "float:left");
                define("NAME_OWNER", "Clicksign Securities");
                break;


            default:
                define("ALINHAMENTO_LOGO", "float:left");
                /**
                 *  Definições Banco de dados
                 */
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "process_certdox_poc";
                $EXTERNALDB = 'process_certdox_poc';
                define("ALINHAMENTO_LOGO", "float:left");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", FALSE);
                define("FILES_FOLDER", "/");
                define("FILES_UPLOAD", "/storage/certdox");
                define("INSTANCENAME", 'Certdox');
                define("SITE_FOLDER", "/");
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
                define("PARCEIRONAME", 'Certdox');
                define("ICONE_PARCEIRO", "icone-certdox.png");
                define("URL_OWNER", "https://www.certdox.com.br");
                define("ALINHAMENTO_LOGO", "float:left");
                define("NAME_OWNER", "Certdox");
        }
        break;

    case "registros.certdox.com.br":
        define("ALINHAMENTO_LOGO", "float:left");
        define("TEMASISTEMA", "skin-blue-light");
        define("TEMPLATE_LOGON", "t_logon_dmcard.html");

        /**
         *  Definições Banco de dados
         */
        $BPMUSER = "process";
        $BPMPWD = "cerberus";
        $EXTERNALUSER = "process";
        $EXTERNALPWD = "cerberus";
        $EXTERNAL_USERNAME = "process";
        $BPMDB = "process_certdox_poc";
        $EXTERNALDB = 'process_certdox_poc';
        define("ALINHAMENTO_LOGO", "float:left");
        define("SITE_ROOT", "http://$srvAccess");
        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
        define("SERVER_ADDRESS", $srvAccess);
        define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
        define("_SECRET_KEY", "f9QsJ4Oufz");
        define("ALLOW_SEARCH", FALSE);
        define("FILES_FOLDER", "/");
        define("FILES_UPLOAD", "/storage/certdox_poc");
        define("INSTANCENAME", 'Sandbox Certdox');
        define("SITE_FOLDER", "/");
        define("SITE_FOLDER_COMPLEMENT", "");
        define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
        define("PARCEIRONAME", 'Gateway Digital');
        define("ICONE_PARCEIRO", "icone-certdox-final.jpg");
        define("URL_OWNER", "https://www.certdox.com.br");
        define("ALINHAMENTO_LOGO", "float:left");
        define("NAME_OWNER", "Certdox");
        break;


    case "idtrust.certdox.com.br":
        define("ALINHAMENTO_LOGO", "float:left");
        /**
         *  Definições Banco de dados
         */
        $BPMUSER = "process";
        $BPMPWD = "cerberus";
        $EXTERNALUSER = "process";
        $EXTERNALPWD = "cerberus";
        $EXTERNAL_USERNAME = "process";
        $BPMDB = "process_idtrust";
        $EXTERNALDB = 'process_idtrust';
        define("ALINHAMENTO_LOGO", "float:left");
        define("SITE_ROOT", "http://$srvAccess");
        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
        define("SERVER_ADDRESS", $srvAccess);
        define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
        define("_SECRET_KEY", "f9QsJ4Oufz");
        define("ALLOW_SEARCH", FALSE);
        define("FILES_FOLDER", "/");
        define("FILES_UPLOAD", "/storage/idtrust");
        define("INSTANCENAME", 'Certdox');
        define("SITE_FOLDER", "/");
        define("SITE_FOLDER_COMPLEMENT", "");
        define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
        define("PARCEIRONAME", 'Id Trust Certdox');
        define("ICONE_PARCEIRO", "icone-certdox-final.png");
        define("URL_OWNER", "https://www.certdox.com.br");
        define("ALINHAMENTO_LOGO", "float:left");
        define("NAME_OWNER", "Certdox");
        break;

    case "showroom.agilizaonline.com.br":

        /**
         *  Definições Banco de dados
         */
        $BPMUSER = "process";
        $BPMPWD = "cerberus";
        $EXTERNALUSER = "process";
        $EXTERNALPWD = "cerberus";
        $EXTERNAL_USERNAME = "process";
        define("TEMPLATE_LOGON", "t_logon_agiliza.html");

        switch ($aliasServidor) {
            case "poc_caruana":
                define("TEMASISTEMA", "skin-purple-light");
                define("FILES_FOLDER", "/");
                define("SITE_FOLDER", "/poc_caruana/");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_FOLDER_COMPLEMENT", "");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("configDB", "localhost");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
                define("FILES_UPLOAD", "/system/caruana/storage/files");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", false);
                define("INSTANCENAME", 'POC Caruana');
                define("LOGO_PARCEIRO", "logo-agiliza.png");
                define("PARCEIRONAME", 'Agiliza');
                define("ICONE_PARCEIRO", "logo-agiliza-mini.png");
                define("URL_OWNER", "https://www.agilizaonline.com.br");
                define("NAME_OWNER", "Agiliza");
                $BPMDB = "process_poc_caruana";
                $EXTERNALDB = 'process_poc_caruana';
                break;
            default:
                define("cabNumeroCaso", "Número do Objeto");
                define("TEMASISTEMA", "skin-blue");
                define("FILES_FOLDER", "/");
                define("SITE_FOLDER", "/");
                define("SITE_ROOT", "http://$srvAccess");
                define("SITE_FOLDER_COMPLEMENT", "");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("configDB", "localhost");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
                define("FILES_UPLOAD", "/system/transferwise/storage/files");
                define("_SECRET_KEY", "f9QsJ4Oufz");
                define("ALLOW_SEARCH", false);
                define("INSTANCENAME", 'Show Room Agiliza');
                define("LOGO_PARCEIRO", "logo-agiliza.png");
                define("PARCEIRONAME", 'Agiliza');
                define("ICONE_PARCEIRO", "logo-agiliza-mini.png");
                define("LOGO_PARCEIRO", "logo-agiliza.png");
                define("LOGO_PARCEIRO_TOP", "icone-agiliza.png");
                define("URL_OWNER", "https://www.agilizaonline.com.br");
                define("NAME_OWNER", "Agiliza");
                $BPMDB = "processteste";
                $EXTERNALDB = 'processteste';
        }
        break;

    case "www.process.pres-switch.com.br":
    case "process.pres-switch.com.br":

        define("FILES_FOLDER", "/");
        define("SITE_FOLDER", "/");
        define("SITE_ROOT", "http://$srvAccess");
        define("SITE_FOLDER_COMPLEMENT", "");
        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
        define("configDB", "localhost");
        define("SERVER_ADDRESS", $srvAccess);
        define("ARQUIVO_ERROR_LOG_PHP", "error_log");
        define("FILES_UPLOAD", "E:/developer/STORAGE_PROCESS");
        define("_SECRET_KEY", "f9QsJ4Oufz");
        define("ALLOW_SEARCH", false);
        define("INSTANCENAME", 'Show Room Agiliza');
        define("ICONE_PARCEIRO", "logo-agiliza-mini.png");
        break;

// Servidor Testes Transferwise
    case "23.96.24.191":
    case "23.96.24.191:8080":
    case "137.117.100.121:8080":
    case "23.96.24.191:8080":
        define("TEMASISTEMA", "skin-red");
        define("TEMPLATE_LOGON", "t_logon_agiliza.html");
        /**
         *  Definições Banco de dados
         */
        $BPMUSER = "process";
        $BPMPWD = "cerberus";
        $EXTERNALUSER = "process";
        $EXTERNALPWD = "cerberus";
        $EXTERNAL_USERNAME = "process";
        $OrigemLogon = "ProcessLogon";
        $BPMDB = "processteste";
        $EXTERNALDB = 'processteste';
        define("SITE_ROOT", "http://$srvAccess");
        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
        define("configDB", "localhost");
        define("SERVER_ADDRESS", $srvAccess);
        define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
        define("_SECRET_KEY", "f9QsJ4Oufz");
        define("ALLOW_SEARCH", FALSE);
        define("TEMASISTEMA", "skin-blue");
        switch ($aliasServidor) {
            case "showroom":
                define("TEMASISTEMA", "skin-blue");
                define("FILES_UPLOAD", "/system/transferwise/storage/files");
                define("INSTANCENAME", 'ShowRoom - Agiliza Flow');
                define("SITE_FOLDER", "/showroom/");
                define("SITE_FOLDER_COMPLEMENT", "");
                define("LOGO_PARCEIRO", "logo-agiliza.png");
                define("PARCEIRONAME", 'Agiliza');
                define("ICONE_PARCEIRO", "logo-agiliza-mini.png");
                define("URL_OWNER", "https://www.agilizaonline.com.br");
                define("NAME_OWNER", "Agiliza");
                break;
            case "certdox":
                define("INSTANCENAME", 'Certdox');
                define("SITE_FOLDER", "/certdox/");
                define("SITE_FOLDER_COMPLEMENT", "");
                define("FILES_UPLOAD", "/system/transferwise/www/Teste/storage/files");
                define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
                define("PARCEIRONAME", 'Certdox');
                define("ICONE_PARCEIRO", "icone-certdox.png");
                define("URL_OWNER", "https://www.certdox.com.br");
                define("NAME_OWNER", "Cerdox");
            default :

                define("FILES_UPLOAD", "/system/transferwise/storage/files");
                define("INSTANCENAME", 'Teste - Agiliza Flow');
                define("SITE_FOLDER", "/");
                define("SITE_FOLDER_COMPLEMENT", "/");
                define("LOGO_PARCEIRO", "logo-agiliza.png");
                define("PARCEIRONAME", 'Agiliza');
                define("ICONE_PARCEIRO", "logo-agiliza-mini.png");
                define("URL_OWNER", "https://www.agilizaonline.com.br");
                define("NAME_OWNER", "Agiliza");
        }
        if (!defined("FILES_FOLDER")) {
            define("FILES_FOLDER", "/");
        }
        break;

    case "40.121.135.140":
    case "orion.app.agilizaonline.com.br":
        // define("LOG_DATA", true);
        define("TEMPLATE_LOGON", "t_logon_agiliza.html");
        switch ($aliasServidor) {
            case "caruana":
                define("INSTANCENAME", 'Agiliza Flow - Caruana');
                define("LOGO_PARCEIRO", "logo-agiliza.png");
                define("PARCEIRONAME", 'Agiliza');
                define("SITE_FOLDER", "/caruana/");
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "caruana";
                $EXTERNALDB = 'caruana';
                define("LOGO_CLIENTE", "caruna-logo.jpeg");
                define("ALINHAMENTO_LOGO_CLIENTE", "align:right");
                break;
            default:
//                define("INSTANCENAME", 'Agiliza Flow');
//                define("LOGO_PARCEIRO", "logo-agiliza.png");
//                define("PARCEIRONAME", 'Agiliza');
//                define("SITE_FOLDER", "");
//                $BPMUSER = "process";
//                $BPMPWD = "cerberus";
//                $EXTERNALUSER = "process";
//                $EXTERNALPWD = "cerberus";
//                $EXTERNAL_USERNAME = "process";
//                $BPMDB = "processteste";
//                $EXTERNALDB = 'processteste';
                $BPMUSER = "process";
                $BPMPWD = "cerberus";
                $EXTERNALUSER = "process";
                $EXTERNALPWD = "cerberus";
                $EXTERNAL_USERNAME = "process";
                $BPMDB = "processteste";
                $EXTERNALDB = 'processteste';
                define("TEMASISTEMA", "skin-blue");
                define("FILES_FOLDER", "/");
                define("SITE_FOLDER", "");
                define("SITE_ROOT", "https://$srvAccess/");
                define("SITE_FOLDER_COMPLEMENT", "/");
                define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
                define("configDB", "localhost");
                define("SERVER_ADDRESS", $srvAccess);
                define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
                define("FILES_UPLOAD", "/system/transferwise/storage/files");
                define("_SECRET_KEY", "cLWD;KCBgP?a1'(0m03W");
                define("ALLOW_SEARCH", false);
                define("INSTANCENAME", 'Agiliza');
                define("LOGO_PARCEIRO", "logo-agiliza.png");
                define("PARCEIRONAME", 'Agiliza');
                define("ICONE_PARCEIRO", "logo-agiliza-mini.png");
                define("ALINHAMENTO_LOGO_CLIENTE", "align:right");
        }
//        define("TEMASISTEMA", "skin-blue");
//        define("FILES_FOLDER", "/");
//        define("SITE_ROOT", "https://$srvAccess");
//        define("SITE_FOLDER_COMPLEMENT", "/");
//        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
//        define("configDB", "localhost");
//        define("SERVER_ADDRESS", $srvAccess);
//        define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
//        define("FILES_UPLOAD", "/system/transferwise/storage/files");
//        define("_SECRET_KEY", "cLWD;KCBgP?a1'(0m03W");
//        define("ALLOW_SEARCH", false);
//        define("ICONE_PARCEIRO", "logo-agiliza-mini.png");
        break;

    case "caruana.agilizaonline.com.br":
        /**
         *  Definições Banco de dados
         */
        $BPMUSER = "process";
        $BPMPWD = "cerberus";
        $EXTERNALUSER = "process";
        $EXTERNALPWD = "cerberus";
        $EXTERNAL_USERNAME = "process";
        $BPMDB = "caruana";
        $EXTERNALDB = 'caruana';
//        define("TEMPLATE_LOGON", "t_logon_agiliza.html");
        define("TEMASISTEMA", "skin-blue");
        define("FILES_FOLDER", "/");
        define("SITE_FOLDER", "");
        define("SITE_ROOT", "http://$srvAccess/");
        define("SITE_FOLDER_COMPLEMENT", "/");
        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
        define("configDB", "localhost");
        define("SERVER_ADDRESS", $srvAccess);
        define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
        define("FILES_UPLOAD", "/system/caruana/storage/files");
        define("_SECRET_KEY", "cLWD;KCBgP?a1'(0m03W");
        define("ALLOW_SEARCH", false);
        define("INSTANCENAME", 'Caruana');
        define("LOGO_PARCEIRO", "logo-agiliza.png");
        define("PARCEIRONAME", 'Agiliza');
        define("ICONE_PARCEIRO", "logo-agiliza-mini.png");
        define("LOGO_CLIENTE", "caruna-logo.jpeg");
        define("ALINHAMENTO_LOGO_CLIENTE", "align:right");
        break;

    case "tribanco.agilizaonline.com.br":
        /**
         *  Definições Banco de dados
         */
        $BPMUSER = "process";
        $BPMPWD = "cerberus";
        $EXTERNALUSER = "process";
        $EXTERNALPWD = "cerberus";
        $EXTERNAL_USERNAME = "process";
        $BPMDB = "tribanco";
        $EXTERNALDB = 'tribanco';
        define("TEMASISTEMA", "skin-blue");
        define("FILES_FOLDER", "/");
        define("SITE_FOLDER", "");
        define("SITE_ROOT", "http://$srvAccess/");
        define("SITE_FOLDER_COMPLEMENT", "/");
        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
        define("configDB", "localhost");
        define("SERVER_ADDRESS", $srvAccess);
        define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
        define("FILES_UPLOAD", "/system/tribanco/files");
        define("_SECRET_KEY", "cLWD;KCBgP?a1'(0m03W");
        define("ALLOW_SEARCH", false);
        define("INSTANCENAME", 'Tribanco');
        define("LOGO_PARCEIRO", "logo-agiliza.png");
        define("PARCEIRONAME", 'Agiliza');
        define("ICONE_PARCEIRO", "logo-agiliza-mini.png");
        define("ALINHAMENTO_LOGO_CLIENTE", "align:right");
        break;

    case "master.agilizaonline.com.br":
        /**
         *  Definições Banco de dados
         */
        $BPMUSER = "process";
        $BPMPWD = "cerberus";
        $EXTERNALUSER = "process";
        $EXTERNALPWD = "cerberus";
        $EXTERNAL_USERNAME = "process";
        $BPMDB = "master";
        $EXTERNALDB = 'master';
        define("TEMASISTEMA", "skin-blue");
        define("FILES_FOLDER", "/");
        define("SITE_FOLDER", "");
        define("SITE_ROOT", "http://$srvAccess/");
        define("SITE_FOLDER_COMPLEMENT", "/");
        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
        define("configDB", "localhost");
        define("SERVER_ADDRESS", $srvAccess);
        define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
        define("FILES_UPLOAD", "/system/master/storage/files");
        define("_SECRET_KEY", "cLWD;KCBgP?a1'(0m03W");
        define("ALLOW_SEARCH", false);
        define("INSTANCENAME", 'Tribanco');
        define("LOGO_PARCEIRO", "logo-agiliza.png");
        define("PARCEIRONAME", 'Agiliza');
        define("ICONE_PARCEIRO", "logo-agiliza-mini.png");
        define("ALINHAMENTO_LOGO_CLIENTE", "align:right");
        break;


    case "poc.caruana.agilizaonline.com.br":
        define("TEMPLATE_LOGON", "t_logon_agiliza.html");
        /**
         *  Definições Banco de dados
         */
        $BPMUSER = "process";
        $BPMPWD = "cerberus";
        $EXTERNALUSER = "process";
        $EXTERNALPWD = "cerberus";
        $EXTERNAL_USERNAME = "process";
        $BPMDB = "process_poc_caruana";
        $EXTERNALDB = 'process_poc_caruana';
        define("TEMASISTEMA", "skin-purple-light");
        define("FILES_FOLDER", "/");
        define("SITE_FOLDER", "");
        define("SITE_ROOT", "https://$srvAccess");
        define("SITE_FOLDER_COMPLEMENT", "/");
        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
        define("configDB", "localhost");
        define("SERVER_ADDRESS", $srvAccess);
        define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
        define("FILES_UPLOAD", "/system/caruana/storage/files");
        define("_SECRET_KEY", "cLWD;KCBgP?a1'(0m03W");
        define("ALLOW_SEARCH", false);
        define("INSTANCENAME", 'POC Caruana');
        define("LOGO_PARCEIRO", "logo-agiliza.png");
        define("PARCEIRONAME", 'Agiliza');
        define("ICONE_PARCEIRO", "logo-agiliza-mini.png");
        break;

    case "caruana.agilizaonline.com.br":
        define("TEMPLATE_LOGON", "t_logon_agiliza.html");
        /**
         *  Definições Banco de dados
         */
        $BPMUSER = "process";
        $BPMPWD = "cerberus";
        $EXTERNALUSER = "process";
        $EXTERNALPWD = "cerberus";
        $EXTERNAL_USERNAME = "process";
        $BPMDB = "process_caruana";
        $EXTERNALDB = 'process_caruana';
        define("TEMASISTEMA", "skin-red-light");
        define("FILES_FOLDER", "/");
        define("SITE_FOLDER", "");
        define("SITE_ROOT", "https://$srvAccess");
        define("SITE_FOLDER_COMPLEMENT", "/");
        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
        define("configDB", "localhost");
        define("SERVER_ADDRESS", $srvAccess);
        define("ARQUIVO_ERROR_LOG_PHP", "log/PHP_errors.log");
        define("FILES_UPLOAD", "/system/caruana/storage/files");
        define("_SECRET_KEY", "cLWD;KCBgP?a1'(0m03W");
        define("ALLOW_SEARCH", false);
        define("INSTANCENAME", 'Caruana');
        define("LOGO_PARCEIRO", "logo-agiliza.png");
        define("PARCEIRONAME", 'Agiliza');
        define("ICONE_PARCEIRO", "logo-agiliza-mini.png");
        break;

    default:
        echo "Não Encontrado: '$srvAccess', Redirect '$redirectUrl'";
        break;
}

if (!defined("SITE_ROOT"))
{
    define("SITE_ROOT", "http://$srvAccess");
}

if (!defined("SERVER_ADDRESS"))
{
    define("SERVER_ADDRESS", $srvAccess);
}

if (!defined("LIMITE_DIAS_ULTIMO_LOGON")) {
    define("LIMITE_DIAS_ULTIMO_LOGON", 90);
}

// Define o texto para cabeçalho na fila para o coluna Nr Caso
if (!defined("cabNumeroCaso")) {
    define("cabNumeroCaso", "#");
}

if (!defined("LOGO_PARCEIRO_TOP")) {
    define("SHOW_ICON_TOOLBAR", "hidden");
}

if (!defined("SITE_ALIAS")) {
    define("SITE_ALIAS", SITE_FOLDER);
}

define("FILES_ROOT", $currentDir . FILES_FOLDER);
if (!defined("TEMASISTEMA")) {
    define("TEMASISTEMA", "skin-blue-light");
}

if (defined("ARQUIVO_ERROR_LOG_PHP")) {
    $nomeDirDestino = FILES_ROOT . dirname(ARQUIVO_ERROR_LOG_PHP);
    if (!file_exists($nomeDirDestino)) {
        mkdir(FILES_FOLDER + $nomeDirDestino);
    }
    ini_set('error_log', ARQUIVO_ERROR_LOG_PHP);
//    echo ARQUIVO_ERROR_LOG_PHP;
}

if (!defined("ARQUIVOS_NOVA_JANELA")) {
    define("ARQUIVOS_NOVA_JANELA", "null");
}

(LOG_DATA) ? error_log("FILES_ROOT: " . FILES_ROOT) : null;

//error_log("Passou e setou o error Log");

$dir = FILES_ROOT;

// Inclusao definicoes do SISTEMA
include_once(FILES_ROOT . 'include/definicoes_sistema.inc');

// <editor-fold defaultstate="collapsed" desc="Includes do Sistema">
require_once FILES_ROOT . "include/config.config.php";
require_once FILES_ROOT . "include/connection.php";
require_once FILES_ROOT . "include/rotas.inc";
require_once FILES_ROOT . "include/func_api.inc";
require_once FILES_ROOT . "include/processo.php";
require_once FILES_ROOT . "include/user.php";
require_once FILES_ROOT . "vendor/lib/raelgc/view/template.php";

// Deve ficar após a carga das Definições de Classes
iniciarPhpSession();

if (isset($_SESSION["userdef"])) {
    $userdef = $_SESSION["userdef"];
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Trata dados de GET e POST">
while (list($key, $value) = each($_POST)) {
    $_POST[$key] = preg_replace('/([\x00-\x08,\x0b-\x0c,\x0e-\x19,<,>])/', '', $value);
    $$key = $value;
}
while (list($key, $value) = each($_GET)) {
    $value = preg_replace('/([\x00-\x08,\x0b-\x0c,\x0e-\x19,<,>])/', '', $value);
    $_GET[$key] = $value;
    $$key = $value;
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Faz a conexão com o Banco de dados">

$comDB = AtivaDBProcess();
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Definição de Rota">

try {
    header('Access-Control-Allow-Headers: x-requested-with, Content-Type, origin, authorization, accept, client-security-token');
    $dadosURL = TrataUrl();
    $rota = $dadosURL;
    $methodHTTP = $_SERVER["REQUEST_METHOD"];
    $rotaDefinida = defineRota($rota, $methodHTTP);

    $LOGAR_ROTA = LOG_DATA | LOGAR_ROTA;

    ($LOGAR_ROTA) ? error_log("Dados URI: '$rota'") : null;
    ($LOGAR_ROTA) ? error_log("Endereço Client: " . var_export($_SERVER["REMOTE_ADDR"], true)) : null;
    ($LOGAR_ROTA) ? error_log("Dados Url 1: " . var_export($dadosURL, true)) : null;
    ($LOGAR_ROTA) ? error_log("REQUEST_METHOD: " . $methodHTTP) : null;
    ($LOGAR_ROTA) ? error_log("SITE_ALIAS: '" . SITE_ALIAS . "'") : null;
    //(LOG_DATA) ? error_log("HEADERS:" . var_export(getallheaders(), true) ) : null;
    ($LOGAR_ROTA) ? error_log(var_export($rotaDefinida, true)) : null;

    $arquivoRota = FILES_ROOT . $rotaDefinida["page"];
    //$arquivoRota = FILES_ROOT . "/" . $rotaDefinida["page"];

    (LOG_DATA) ? error_log("Arquivo ROTA" . $arquivoRota) : null;

    if (file_exists($arquivoRota)) {
        include(FILES_ROOT . $rotaDefinida["page"]);
//        include(FILES_ROOT . "/" . $rotaDefinida["page"]);
    } else {
        header("HTTP/1.0 404 Not Found");
        exit;
    }

    if (array_key_exists("funcao", $rotaDefinida)) {
        if ($rotaDefinida["funcao"] != '') {
            $funcao = $rotaDefinida["funcao"];
            $funcao();
        }
    }
} catch (ErrorException $e) {
    error_log("Rota Definida:" . var_export($rotaDefinida, true));
    error_log("Erro " . $e->getMessage());
}
// </editor-fold>