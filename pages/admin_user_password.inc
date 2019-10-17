<?php
/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 19/09/2018
Sistema: Process_XAMPP
*/
// error_log("Entrou admin_users_edit.inc");
include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/func_api.inc");
include_once(FILES_ROOT . "include/toolbar.inc");

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_edituser_password.html");

$t_body->USUARIO_ID = $userId;
    
$t_body->TEXTO_TITULO = "Alterar senha de \"$userName\"";


/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>



