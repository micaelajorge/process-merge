<?php

/**
 *  ROTA: logon
 */
require_once(FILES_ROOT . "include/resource01.php");
require_once FILES_ROOT . "vendor/lib/raelgc/view/template.php";

use raelgc\view\Template;

if (defined("TEMPLATE_LOGON")) {
    $templateLogon = TEMPLATE_LOGON;
} else {
    $templateLogon = 't_newlogon.html';
}
$t_body = new Template(FILES_ROOT . "assets/templates/$templateLogon");

//$t_body->block("BLOCK_HEADER");
//$t_body->block("BLOCK_FOOTER");
//$t_body->MSGBOASVINDAS = "Bem vindo ao Process";
$t_body->SITE_LOCATION = SITE_LOCATION;

if (!defined("INSTANCENAME")) {
    define("INSTANCENAME", "Instancia sem Nome");
}

$t_body->TITULO_PAGE = PARCEIRONAME . " | " . INSTANCENAME;
$t_body->ICONE_PARCEIRO = SITE_FOLDER . "images/" . ICONE_PARCEIRO;

if (defined("LOGO_CLIENTE")) {
    $t_body->LOGO_CLIENTE = LOGO_CLIENTE;
    $t_body->block("BLOCO_LOGO_CLIENTE");
}

$t_body->block("BLOCO_LOGON");

if ($redirectQueryString != "") {
    $t_body->ProcId = $redirectQueryString;
}


$keyGET = array_keys($_GET);
$pageSelect = array_pop($keyGET);
$t_body->PageSelect = "$pageSelect";

if (!isset($TIPO_ALERTA_LOGON)) {
    $TIPO_ALERTA_LOGON = "alert-warning";
}
if (isset($MsgLogon)) {
    if ($MsgLogon !== "") {
        $t_body->TIPO_ALERTA_LOGON = $TIPO_ALERTA_LOGON;
        $t_body->MSGLOGON = $MsgLogon;
        $t_body->block("BLOCO_ALERTA_ERRO");
    }
}

$t_body->LOGO_PARCEIRO = LOGO_PARCEIRO;
//$t_body->LOGO_CLIENTE = "logo-souzalima.png";

$Valor = $t_body->show();
echo str_replace("\n", "", $Valor);

echo "<!-- " . __DIR__ . "  -->";
