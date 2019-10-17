<?php

require_once("common.php");
include("validasessao.php");
require_once("resource01.php");
require_once("resource02.php");
require_once(FILES_ROOT . "/pages/editcase_form.php");
require_once(FILES_ROOT . "/pages/editcase_campos.inc");

use raelgc\view\Template;

$Query = CamposFormulario($ProcId, $StepId);
$retorno = "";
if (!empty($FormsPai)) {
    $FormsPai = explode(",", $FormsPai);
    if (count($FormsPai) > 0) {
        $Endereco = "";
        foreach ($FormsPai as $Form) {
            $Endereco .= "[\"$Form\"]";
        }
        $retorno = "Validacao$Endereco = new Object()\n";
    }
}
$NomeFormularioDestino = $Formulario;

$t_body = new Template(FILES_ROOT . "assets/templates/t_editcase_wrapp_fields.html");

if (!isset($classeSubstituta)) {
    $classeSubstituta = "";
}

// Determina a tag de LABEL do Campo
if (!isset($tipoLabelCampo)) {
    $tipoLabelCampo = "label";
} else {
    $tipoLabelCampo = "span";
}

$retornoCampos = MontaFormulario($ProcId, $StepId, $CaseNum, $Query, $Action, True, false, $FormsPai, $classeSubstituta, $tipoLabelCampo);
$t_body->CAMPOSFORMULARIO = $retornoCampos["CONTEUDO"];

$ValidacaoCampos = $retornoCampos["VALIDACAO_JAVASCRIPT"];

$t_body->block("BLOCO_CAMPO_SUBFORM");
$conteudoForm = $t_body->parse();
$conteudoForm = str_replace("\r", "", $conteudoForm);
$conteudoForm = str_replace("\n", "", $conteudoForm);

$retorno .= "$ValidacaoCampos ;\n";
$retorno .= "var conteudo = document.getElementById('$NomeFormularioDestino');\n";
$retorno .= "conteudo.innerHTML=\"" . addslashes($conteudoForm) . "\";\n";
echo $retorno;