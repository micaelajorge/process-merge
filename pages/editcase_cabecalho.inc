<?php
/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 28/02/2019
Sistema: Process_XAMPP
*/

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/resource01.php");
include_once(FILES_ROOT . "include/resource02.php");
include_once(FILES_ROOT . "pages/editcase_form.php");
include_once(FILES_ROOT . "include/bpmeditcasefuncs.inc");
include_once(FILES_ROOT . "pages/editcase_campos.inc");

use raelgc\view\Template;

$Campos = CamposFormulario($ProcId, $StepId);

$t_body = new Template(FILES_ROOT . "assets/templates/t_editcase_cabecalho_processo.html");
$retornoCabecalho = MontaFormularioCabecalho($ProcId, $CaseNum, $Campos);
$t_body->PROCID = $ProcId;
$t_body->STEPID = $StepId;
$t_body->CASENUM = $CaseNum;
$t_body->BOTAO_EDITAR_CLASSE = "btn-primary";

$t_body->CONTEUDO_CABECALHO = $retornoCabecalho;
$t_body->show();
