<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 19/09/2018
  Sistema: Process_XAMPP
 * 
 * @rota permissao/processo
 */

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/func_api.inc");
include_once(FILES_ROOT . "include/resource01.php");

use raelgc\view\Template;

$mobileMode = 0;
if (isset($_SESSION["mobileMode"])) {
    $mobileMode = $_SESSION["mobileMode"];
}

$t_body = new Template(FILES_ROOT . "assets/templates/t_adm_permissoes_processo_entidade.html");
$t_body->addFile("FORM_ENTIDADE", FILES_ROOT . "assets/templates/t_adm_permissoes_processo_entidade_form.html");

if ($groupId == "") {
    $t_body->SEARCH_HIDDEN = "";
    $t_body->BOTAO_PAGAR_HIDDEN = "hide";
    $acaoEntidade = "A";
    $t_body->VIEWQUEUE_DISABLED = "";
    $t_body->EDIT_DISABLED = "";
    $t_body->CRIARNOVO_DISABLED = "disabled";
    $t_body->TIPOFILTRO = $grpFld;
    $t_body->STYLE_HIDE_PERMISSOES = "display:none";
    $t_body->DISABLED_SALVAR = "disabled";
    $t_body->STYLE_TEXTO_EDITAR = "display:none";
    $t_body->STYLE_NOME_USUARIO = "display:none";
} else {
    $t_body->STYLE_TEXTO_INSERIR = "display:none";
    $t_body->STYLE_SEARCH = "display:none";
    $acaoEntidade = "E";
}
$t_body->USERNAME = $entidade;
$t_body->TEXTO_TITULO_INSERIR = "Selecionar Usuário / Grupo para \"$stepName\"";

$sql = "select * from stepaddresses where procid = $procId and stepId = $stepId and grpfld = '$grpFld' and groupId = $groupId";
$Query = mysqli_query($connect, $sql);

$linhas = mysqli_fetch_all($Query, MYSQLI_ASSOC);

$tipoPasso = PegaTipoPasso($procId, $stepId);


($linhas[0]["ViewQueue"] == '1') ? $t_body->VIEWQUEUE_CHECKED = "checked" : null;
($linhas[0]["Action"] == '1') ? $t_body->EDIT_CHECKED = "checked" : $t_body->CRIARNOVO_DISABLED = "disabled";
($linhas[0]["NewDoc"] == '1') ? $t_body->CRIARNOVO_CHECKED = "checked" : null;

switch ($tipoPasso) {
    case "P":
        $t_body->SHOW_EDITAR = "display:none";
        $t_body->SHOW_EDITAR = "";
        $t_body->SHOW_VIEW = "display:none";
        $t_body->EDIT_CHECKED = "checked";
        $t_body->CRIARNOVO_DISABLED = "";
        $linhas[0]["Action"] = 1;
        $linhas[0]["NewDoc"] = 1;
        $descStep = "Processo ";
        break;
    case "T":
        $t_body->SHOW_NOVO = "display:none";
        $descStep = "Status ";
        break;
    default:
        $descStep = "Tarefa";
        break;
}

$t_body->TEXTO_TITULO_EDITAR = "Permissões $descStep \"$stepName\"";




$t_body->ENTIDADE = $entidade;
$t_body->PROCID = $procId;
$t_body->STEPID = $stepId;
$t_body->GRPFLD = $grpFld;
$t_body->GROUPID = $groupId;
$t_body->ADDEDIT = "$acaoEntidade";


/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>



