<?php
require_once(FILES_ROOT . "include/resource01.php");
use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_newlogon.html");

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
    
    
$t_body->LOGO_PARCEIRO = "logo-agiliza.png";
$t_body->LOGO_CLIENTE = "logo-souzalima.png";

$Valor = $t_body->show();
echo str_replace("\n", "", $Valor);