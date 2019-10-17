<?php

/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 17/12/2018
Sistema: Process_XAMPP
*/
include_once(FILES_ROOT . "/include/resource01.php");
include_once(FILES_ROOT . "pages/editcase_campos.inc");

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_webcaptura_formulario_campos.html");
$t_body->addFile("CAMPOS_FORMULARIO", "assets/templates/t_editcase_fields.html");

$FieldId = 10;
$CaseNum = 0;
$FormsPai = "";
$linha = "";
$ReadOnly = 0;
$StiloObjeto = "";
$Valor_Campo = "";
$ExtendProps = "";
$AdminForm = 0;
$nome = "";
$optional = 0;
$ReadOnlyFilhos = 0;
$ClassStatus = "col-md-12 col-lg-12 form-group-sm input-group";
$ClassStatus = " form-group-sm input-group";
$classeSubstituta = "col-lg-12 col-md-12";
$classeSubstituta = " form-group-sm";
$Action = "Edit";
montaCampoSFR($t_body, "Edit", $ProcId, $CaseNum, $FieldId, $FormsPai, $linha, $ReadOnly, $StiloObjeto, $Valor_Campo, $ExtendProps, $AdminForm, $nome, $optional, $ReadOnlyFilhos, $ClassStatus, $Online, $ClasseCampo, $classeSubstituta, "span");

$t_body->show();


