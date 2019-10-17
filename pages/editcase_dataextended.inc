<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 11/01/2019
  Sistema: Process_XAMPP
 */

use raelgc\view\Template;

include_once(FILES_ROOT . "include/resource01.php");

$dadosRecebidos = apiTrataDadosEntrada();

$t_body = new Template(FILES_ROOT . "assets/templates/t_editcase_dataextended.html");

$SQL = "select * from casequeue where ProcId = $procId and StepId = $stepId and CaseId = $caseNum";
$Query = mysqli_query($connect, $SQL);

$lista = mysqli_fetch_all($Query, MYSQLI_ASSOC);


$t_body->TITULOSECTION = "Dados da Tarefa ";
$t_body->INFORMACAOTITULO = "Data Criação";
$t_body->INFORMACAOVALOR = ConvDate($lista[0]["InsertDate"]);
$t_body->INFORMACAOCLASS = "";
$t_body->block("BLOCK_INFO");
$t_body->INFORMACAOTITULO = "Entrada na Fila";
$t_body->INFORMACAOVALOR = ConvDate($lista[0]["QueueDate"]);
$t_body->INFORMACAOCLASS = "";
$t_body->block("BLOCK_INFO");
$t_body->block("BLOCK_COLUMN");
$t_body->block("BLOCK_SECTION");

/**
 *  Dead Times Tarefa
 */

/**
 *  DeadSoft
 */
if ($lista[0]["DeadSoftWait"] == 1) {
    $t_body->INFORMACAOTITULO = "Prazo 1";
    $t_body->INFORMACAOVALOR = ConvDate($lista[0]["deadsoftdatetime"]);
    if ($lista[0]["DeadSoftExec"] == 1)
    {
        $t_body->INFORMACAOCLASS = "text-primary";
    } else {
        $t_body->INFORMACAOCLASS = "text-danger";
    }
    $t_body->block("BLOCK_INFO");
    $deadTimesQueue = true;
}
/**
 *  DeadHard
 */
if ($lista[0]["DeadHardWait"] == 1) {
    $t_body->INFORMACAOTITULO = "Prazo 2";
    $t_body->INFORMACAOVALOR = ConvDate($lista[0]["deadharddatetime"]);
    if ($lista[0]["DeadHardExec"] == 1)
    {
        $t_body->INFORMACAOCLASS = "text-primary";
    } else {
        $t_body->INFORMACAOCLASS = "text-danger";
    }
    $t_body->block("BLOCK_INFO");
    $deadTimesQueue = true;
}

/**
 *  DeadHardest
 */
if ($lista[0]["DeadHardestWait"] == 1) {
    $t_body->INFORMACAOTITULO = "Prazo 3";
    $t_body->INFORMACAOVALOR = ConvDate($lista[0]["DeadHardestDateTime"]);
    if ($lista[0]["DeadHardestExec"] == 1)
    {
        $t_body->INFORMACAOCLASS = "text-primary";
    } else {
        $t_body->INFORMACAOCLASS = "text-danger";
    }
    $t_body->block("BLOCK_INFO");
    $deadTimesQueue = true;
}
if ($deadTimesQueue) {
    $t_body->TITULOSECTION = "SLA Tarefa ";
    $t_body->block("BLOCK_COLUMN");
    $t_body->block("BLOCK_SECTION");
} else {
    $t_body->TITULOSECTION = "Não há SLA para a Tarefa";
    $t_body->block("BLOCK_COLUMN");
    $t_body->block("BLOCK_SECTION");
}

/**
 *  Dead Times Caso
 */


/*
$SQL = "select * from casequeue where ProcId = $procId and StepId = 0 and CaseId = $caseNum";
$Query = mysqli_query($connect, $SQL);

$lista = mysqli_fetch_all($Query, MYSQLI_ASSOC);

*/
/**
 *  DeadSoft
 */
/*
if ($lista[0]["DeadSoftWait"] == 1) {
    $t_body->INFORMACAOTITULO = "Prazo 1";
    $t_body->INFORMACAOVALOR = ConvDate($lista[0]["deadsoftdatetime"]);
    if ($lista[0]["DeadSoftExec"] == 1)
    {
        $t_body->INFORMACAOCLASS = "text-primary";
    } else {
        $t_body->INFORMACAOCLASS = "text-danger";
    }
    $t_body->block("BLOCK_INFO");
    $deadTimesQueue = true;
}
*/

 /**
 *  Deadhard
 */
/*
if ($lista[0]["DeadHardWait"] == 1) {
    $t_body->INFORMACAOTITULO = "Prazo 2";
    $t_body->INFORMACAOVALOR = ConvDate($lista[0]["deadharddatetime"]);
    if ($lista[0]["DeadHardExec"] == 1)
    {
        $t_body->INFORMACAOCLASS = "text-primary";
    } else {
        $t_body->INFORMACAOCLASS = "text-danger";
    }
    $t_body->block("BLOCK_INFO");
    $deadTimesQueue = true;
}
*/

/**
 *  DeadHardest
 */
/*
if ($lista[0]["DeadHardestWait"] == 1) {
    $t_body->INFORMACAOTITULO = "Prazo 3";
    $t_body->INFORMACAOVALOR = ConvDate($lista[0]["DeadHardestDateTime"]);
    if ($lista[0]["DeadHardestExec"] == 1)
    {
        $t_body->INFORMACAOCLASS = "text-primary";
    } else {
        $t_body->INFORMACAOCLASS = "text-danger";
    }
    $t_body->block("BLOCK_INFO");
    $deadTimesQueue = true;
}
if ($deadTimesQueue) {
    $t_body->TITULOSECTION = "SLA Global";
    $t_body->block("BLOCK_COLUMN");
    $t_body->block("BLOCK_SECTION");
} else {
    $t_body->TITULOSECTION = "Não há SLA Global";
    $t_body->block("BLOCK_COLUMN");
    $t_body->block("BLOCK_SECTION");
}
*/

$t_body->show();

