<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 17/12/2018
  Sistema: Process_XAMPP

  @rota editcase_webcaptura
 */
include_once(FILES_ROOT . "include/resource01.php");

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_editcase_wrapp_fields.html");
$t_body->addFile("CAMPO_FORMULARIO_CAPTURA", FILES_ROOT . "assets/templates/t_editcase_fields.html");

//$ListaCampos = CamposFormulario($documentoProcId, $documentoStepId);
$ListaCampos = PegaCamposRepositorio($documentoProcId);
foreach ($ListaCampos as $linha) {
    if ($linha["fieldtype"] == "FD")
    {
        continue;
    }
    //$t_body->FIELD_ID = $linha["fieldid"];
    $t_body->FIELD_ID = $linha["FieldCod"];
    $t_body->FIELD_NAME = $linha["fieldname"];
    $t_body->FIELDCODE = $linha["FieldCod"];
    $t_body->PRE_FIELDID = "";
    $t_body->FIELD_VALUE = "";
    $t_body->FIELD_TYPE = "field";
    $t_body->block("BLOCK_CAMPO_HIDDEN");
}

$t_body->PRE_FIELDID = "";

// ProcId
$t_body->FIELDCODE = "procId";
$t_body->FIELD_ID = "procId";
$t_body->FIELD_CODE = "procId";
$t_body->FIELD_VALUE = $documentoProcId;
$t_body->FIELD_TYPE = "docRef";
$t_body->block("BLOCK_CAMPO_HIDDEN");


// StepId
$t_body->FIELDCODE = "stepIdForm";
$t_body->FIELD_ID = "stepIdForm";
$t_body->FIELD_CODE = "stepIdForm";
$t_body->FIELD_TYPE = "docRef";
$t_body->FIELD_VALUE = $documentoStepId;
$t_body->block("BLOCK_CAMPO_HIDDEN");

// CaseNumPai
$t_body->FIELDCODE = "documentCaseNumPai";
$t_body->FIELD_ID = "documentCaseNumPai";
$t_body->FIELD_CODE = "documentCaseNumPai";
$t_body->FIELD_TYPE = "docRef";
$t_body->FIELD_VALUE = $documentCaseNumPai;
$t_body->block("BLOCK_CAMPO_HIDDEN");

$t_body->block("BLOCK_FORMCAPTURA");


$t_body->show();
