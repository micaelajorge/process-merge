<?php
require_once(FILES_ROOT . "include/resource01.php");
use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_recuperar_senha.html");

//$t_body->block("BLOCK_HEADER");
//$t_body->block("BLOCK_FOOTER");
//$t_body->MSGBOASVINDAS = "Bem vindo ao Process";
//$t_body->SITE_LOCATION = SITE_LOCATION;
$t_body->block("BLOCO_RECUPERAR");
if (isset($MsgLogon))
    {
    $t_body->MSGLOGON = $MsgLogon;
    $t_body->block("BLOCO_ALERTA_ERRO");
    }
       
    
$t_body->TITULO_PAGE = PARCEIRONAME . " | " . INSTANCENAME;
$t_body->ICONE_PARCEIRO = SITE_FOLDER . "images/" . ICONE_PARCEIRO;    

if (defined("LOGO_CLIENTE")) {
    $t_body->LOGO_CLIENTE = LOGO_CLIENTE;
    $t_body->block("BLOCO_LOGO_CLIENTE");
}


$Valor = $t_body->show();