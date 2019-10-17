<?php
/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 19/09/2018
Sistema: Process_XAMPP
*/

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/func_api.inc");
include_once(FILES_ROOT . "include/toolbar.inc");

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_editgroup_form.html");

$SQL = "select * from groupdef where groupid = $groupId";
$Query = mysqli_query($connect, $SQL);
$linha = mysqli_fetch_array($Query);

$t_body->GROUP_ID = $linha["GroupId"];
$t_body->GROUP_NAME = $linha["GroupName"];
$t_body->GROUP_DESC = $linha["GroupDesc"];

if ($groupId >= 1)
{
    $t_body->CLASS_COR_ICONE = "text-primary";    
    $t_body->TEXTO_TITULO = $linha["GroupName"];
} else {
    $t_body->CLASS_COR_ICONE = "text-green";
    $t_body->TEXTO_TITULO = "Novo Usuário";
}

/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>



