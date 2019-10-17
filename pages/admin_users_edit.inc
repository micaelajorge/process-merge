<?php
/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 19/09/2018
Sistema: Process_XAMPP
 * 
 * ROTA: edituser
*/
//error_log("Entrou admin_users_edit.inc");
include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/func_api.inc");
include_once(FILES_ROOT . "include/toolbar.inc");

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_edituser_form.html");

$SQL = "select * from userdef where userid = $userId";
$Query = mysqli_query($connect, $SQL);
$linha = mysqli_fetch_array($Query);

$t_body->USUARIO_ID = $linha["UserId"];
$t_body->USUARIO_NAME = $linha["UserName"];
$t_body->USUARIO_DESC = $linha["UserDesc"];
$t_body->USUARIO_EMAIL = $linha["Email"];
$t_body->JWT_USER = $linha["token"];
$Uactive = $linha["UActive"];

if ($userId >= 1)
{
    $t_body->USUARIO_NAME_DISABLED = "disabled";    
    $t_body->CLASS_COR_ICONE = "text-primary";    
    $t_body->TEXTO_TITULO = $linha["UserName"];
} else {
    $t_body->CLASS_COR_ICONE = "text-green";
    $t_body->TEXTO_TITULO = "Novo Usuário";
}

/**
 *  Tratamento do Campo de Origem
 */

include FILES_ROOT . "pages/ext_lista_dominios.inc";
$origemUsuario = $linha["Origem"];
$t_body->CAMPO_ORIGEM = Inicializacao_ext_lista_dominios("USUARIO_ORIGEM", 0, $origemUsuario, "", "");

if ($Uactive == 1)
{
    $t_body->USUARIO_ATIVO = "selected";    
} else {
    $t_body->USUARIO_INATIVO = "selected";    
}

/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>



