<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 19/09/2018
  Sistema: Process_XAMPP
 */

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/func_api.inc");

use raelgc\view\Template;

$mobileMode = 0;
if (isset($_SESSION["mobileMode"])) {
    $mobileMode = $_SESSION["mobileMode"];
}

if ($reload == "") {
    $t_body = new Template(FILES_ROOT . "assets/templates/t_adm_usuarios_grupos.html");
    $t_body->addFile("CONTEUDO_GRUPOS", FILES_ROOT . "assets/templates/t_adm_usuarios_grupos_lista.html");
} else {
    $t_body = new Template(FILES_ROOT . "assets/templates/t_adm_usuarios_grupos_lista.html");
}

if ($groupId == "") {
    $t_body->SEARCH_HIDDEN = "";
    $t_body->BOTAO_PAGAR_HIDDEN = "hide";
    $acaoEntidade = "A";
    $t_body->VIEWQUEUE_DISABLED = "";
    $t_body->EDIT_DISABLED = "";
    $t_body->CRIARNOVO_DISABLED = "disabled";
    $t_body->TIPOFILTRO = $grpFld;
    $t_body->STYLE_HIDE_PERMISSOES = "display:none";
    $t_body->STYLE_TEXTO_EDITAR = "display:none";
    $t_body->STYLE_NOME_USUARIO = "display:none";
} else {
    $t_body->STYLE_TEXTO_INSERIR = "display:none";
    $t_body->STYLE_SEARCH = "display:none";
    $acaoEntidade = "E";
}
$t_body->STYLE_SEARCH = "display:none";
$t_body->USERNAME = $entidade;

if ($GrpFld == "U") {
    $sql = "select 'G' as grpFld, groupdef.groupid as  entidade_id, groupname as entidade_nome, groupdesc as entidade_desc from usergroup, groupdef where usergroup.userid = $entidade_id and usergroup.groupid = groupdef.groupid";
    $cabecalhoTabelas = array("", "Grupos");
    $t_body->TIPO_ENTIDADE = "Grupo";
} else {
    $sql = "select 'U' as grpFld, userdef.userid as entidade_id, username as entidade_nome, userdesc as entidade_desc from usergroup, userdef where usergroup.groupid = $entidade_id and usergroup.userid = userdef.userid";
    $cabecalhoTabelas = array("", "Usuarios");
    $t_body->TIPO_ENTIDADE = "Usuário";
}

foreach ($cabecalhoTabelas as $cabecalho) {
    $t_body->CAB_TABELA = $cabecalho;
    $t_body->block("BLOCO_CABECALHO_TABELA");
}


$Query = mysqli_query($connect, $sql);

$linhas = mysqli_fetch_all($Query, MYSQLI_ASSOC);

$listaEntidades = array();
foreach ($linhas as $linha) {
    //$listaEntidades[] = $linha["entidade_id"];
    $t_body->ENTIDADE_ID = $linha["entidade_id"];
    $t_body->GRUPO_USUARIO = $linha["entidade_nome"];
    $t_body->DESCRICAO = $linha["entidade_desc"];
    $t_body->GRPFLD_ENTIDADE = $linha["grpFld"];
    $t_body->block("BLOCO_ENTIDADES");
}
$_SESSION["listaEntidades"] = $listaEntidades;
$t_body->ENTIDADE = $entidade;
$t_body->PROCID = $procId;
$t_body->STEPID = $stepId;
$t_body->GRPFLD = $grpFld;
$t_body->GROUPID = $entidade_id;
$t_body->ADDEDIT = "$acaoEntidade";


/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>



