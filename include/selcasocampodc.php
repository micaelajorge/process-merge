<?php

// //Versao 1.0.0 /Versao
require_once("common.php");
include("resource01.php");
include("resource02.php");

$dir =  dirname(__FILE__);


use raelgc\view\Template;



$t_body = new Template("../templates/template_selcase.html");


if (empty($PaginaAtual)) {
    $PaginaAtual = 1;
}

if (!is_numeric($ProcSource)) {
    $ProcSource = PegaProcIdByCode($ProcSource);
}

if (!is_numeric($FieldSource)) {
    $FieldSource = PegaFieldIdByCode($ProcSource, $FieldSource);
}

$limite = 10;
$SQL = "select ";
$SQL .= "count(*) as NrRecords ";
$SQL .= "from ";
$SQL .= "casedata ";
$SQL .= "where ";
$SQL .= "ProcId = $ProcSource ";
$Query = mysqli_query($connect, $SQL);
$Linha = mysqli_fetch_array($Query);
$NrRecords = $Linha['NrRecords'];

/*
  $SQL = "select ";
  $SQL .= "OptValues from stepfieldsdef ";
  $SQL .= "where ";
  $SQL .= "ProcId = $ProcSource ";
  $SQL .= "and ";
  $SQL .= "StepId = $StepId ";
  $Query = mysqli_query($connect, $SQL);
  $Linha = mysqli_fetch_array($Query);
  $OptValues = $Linha["OptValues"];
 */
/*
  $SQL = "select ";
  $SQL .= " FieldValue, ";
  $SQL .= " CaseNum ";
  $SQL .= " from ";
  $SQL .= " casedata ";
  $SQL .= " where ";
  $SQL .= " ProcId = $ProcSource ";
  $SQL .= " and ";
  $SQL .= " FieldId = $FieldSource ";
  $SQL .= " order by FieldValue ";

 */
$SQL = " select ";
$SQL .= " FieldValue, ";
$SQL .= " CaseNum  ";
$SQL .= " from  ";
$SQL .= " casequeue,  ";
$SQL .= " casedata  ";
$SQL .= " where  ";
$SQL .= " casequeue.ProcId = $ProcSource  ";
$SQL .= " and ";
$SQL .= " StepId <> 0 ";
if (!empty($OptValues)) {
    $SQL .= " and ";
    $SQL .= " StepId in $OptValues ";
}
$SQL .= " and ";
$SQL .= " casedata.ProcId = casequeue.ProcId ";
$SQL .= " and  ";
$SQL .= " casedata.CaseNum = casequeue.CaseId  ";
$SQL .= " and ";
$SQL .= " FieldId = $FieldSource ";
//$SQL .= " limit $inicio, $limite ";
$Query = mysqli_query($connect, $SQL);

$t_body->PROCNAME = PegaNomeProc($ProcSource);

while ($linha = mysqli_fetch_array($Query)) {
    $CaseNum = $linha['CaseNum'];
    $Valor = PegaValorCampo($ProcSource, $CaseNum, $FieldSource, 'TX', 1, 1);
    $aReferencias = PegaReferencias($ProcSource, $CaseNum, false, FALSE, 0, CR_NRSHOWREFERENCIAS);
    foreach ($aReferencias as $referencia) {
        $t_body->CAMPOVALOR = $referencia;
        $t_body->block("BLOCK_VALOR_REFERENCIAS");
        }    
    $t_body->CASENUM = $CaseNum;
    $t_body->VALORCAMPO = $Valor;
    $t_body->PROCSOURCE = $ProcSource;
    $t_body->block("BLOCO_CASOS_PROCESSO");
    }

$Paginacao = IndicePaginas($PaginaAtual, $TotalRegistros, 10, "include/selCasoCampoDC.php?ProcCod=$ProcSource&FieldSource=$FieldSource&CaseSource=$CaseSource", "Visualizar=$Visualizar", true);

foreach ($Paginacao as $IndicePagina) {
    $t_body->PAGINAINDICE = $IndicePagina["pagina"];
    $t_body->PAGINAACAO = $IndicePagina["acao"];
    $t_body->PAGINACLASS = $IndicePagina["class"];
    $t_body->block("BLOCK_PAGINAS");
}
 
 echo $t_body->parse();
?>