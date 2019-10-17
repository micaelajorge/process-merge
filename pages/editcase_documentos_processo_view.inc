<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 28/02/2019
  Sistema: Process_XAMPP
 * 
 * ROTA: documentosprocesso_list
 */

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/resource01.php");
include_once(FILES_ROOT . "pages/editcase_form.php");
include_once(FILES_ROOT . "include/bpmeditcasefuncs.inc");

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_documentos_processo_view.html");
$t_body->addFile("IMAGENS_THUMBS", FILES_ROOT . "assets/templates/t_documentos_processo_view_images.html");

$dadosEntrada = apiTrataDadosEntrada();

$procId = $dadosEntrada["ProcId"];
$caseNum = $dadosEntrada["CaseNum"];
$procDoc = $dadosEntrada["procDoc"];
$docId = $dadosEntrada["docId"];
$acaoDocumentos = $dadosEntrada["acaoDocumentos"];


$SQL = " select casenum, fieldvalue, casenum, fieldid, valueid from casedata, casequeue where casedata.procid = $procDoc and fieldid = 8 and casequeue.procid = casedata.procid and casequeue.caseid = casedata.casenum and stepid = $docId and casenum in 
 ( select fieldvalue from casedata where procid = $procId and fieldid = 7 and casenum = $caseNum)";
$query = mysqli_query($connect, $SQL);
$documentosProcesso = mysqli_fetch_all($query, MYSQLI_ASSOC);

foreach ($documentosProcesso as $documento) {
    $aExtendData = json_decode($documento["fieldvalue"], true);
    $CaseNumDoc = $documento["casenum"];
    $fileName = $aExtendData["fileNameStorage"];
    $fieldId = $documento["fieldid"];
    $valueId = $documento["valueid"];
    $caseNum = $documento["casenum"];
    $urlImagem = SITE_ROOT . SITE_FOLDER . "viewfile/$procDoc/$CaseNumDoc/$fieldId/$fileName";
    
    if ($acaoDocumentos != "Edit")
    {
        $t_body->HIDE_EDIT = "hide";
    }
    $ext = pathinfo($fileName, PATHINFO_EXTENSION);
    
    $t_body->ARQUIVO_NOME_STORAGE = $fileName;
    $t_body->PROCID = $procDoc;
    $t_body->CASENUM = $caseNum;
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
