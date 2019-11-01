<?php
/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 19/09/2018
Sistema: Process_XAMPP
 * 
 * ROTA: trocarsenha
*/

use raelgc\view\Template;

$userdef = $_SESSION["userdef"];

$t_body = new Template(FILES_ROOT . "assets/templates/t_user_change_password.html");
    
$t_body->TEXTO_TITULO = "Alterar senha de \"$userdef->UserName\"";

$t_body->USUARIO_ID = $userdef->UserId;

/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>



