<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 28/02/2019
  Sistema: Process_XAMPP
 * 
 * ROTA: documentosprocesso_getdoc
 */

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/resource01.php");
include_once(FILES_ROOT . "pages/editcase_form.php");
include_once(FILES_ROOT . "include/bpmeditcasefuncs.inc");

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_documentos_processo_view_images.html");

$dadosEntrada = apiTrataDadosEntrada();

$caseNum = $dadosEntrada["CaseNum"];
$acaoDocumentos = $dadosEntrada["AcaoDocumentos"];

// Pega o ProcId do Processo
$procDoc = PegaProcIdCaso($caseNum);

$SQL = " select casenum, fieldvalue, fieldid, valueid from casedata where fieldid = 5 and casenum = $caseNum ";
$query = mysqli_query($connect, $SQL);
$documentosProcesso = mysqli_fetch_all($query, MYSQLI_ASSOC);

foreach ($documentosProcesso as $documento) {
    $aExtendData = json_decode($documento["fieldvalue"], true);
    $CaseNumDoc = $documento["casenum"];
    $fileName = $aExtendData["fileNameStorage"];
    $urlImagem = SITE_ROOT . SITE_FOLDER . "viewfile/$procDoc/$CaseNumDoc/5/$fileName";
    $fieldId = $documento["fieldid"];
    $valueId = $documento["valueid"];
    $caseNum = $documento["casenum"];
    
    if ($acaoDocumentos != "Edit")
    {
        $t_body->HIDE_EDIT = "hide";
    }
    
    $ext = pathinfo($fileName, PATHINFO_EXTENSION);
    
    $t_body->PROCID = $procDoc;
    $t_body->CASENUM - $caseNum;
    $t_body->FIELDID = $fieldId;
    $t_body->DOCID = $docId;
    $t_body->IMG_ID = $caseNum . "_" . $fieldId . "_" . $valueId;
    $t_body->URL_IMAGEM = $urlImagem;
    $t_body->FILE_NAME = $aExtendData["fileName"];
    if ($ext == "pdf")
    {
        $t_body->block("BLOCO_PDF");
    } else {
        $t_body->block("BLOCO_IMAGEM");
    }    
}
$t_body->show();