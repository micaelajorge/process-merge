<?php
/**
 * @rota fileattachsel
 */
// <editor-fold defaultstate="collapsed" desc="Chamada Classe de Template">

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_uploadfile.html");
// </editor-fold>

$maxFileSize = ini_get('upload_max_filesize');
$maxFileSizeMegaBytes = replace($maxFileSize, "M", ".");
$maxFileSizeBytes = floatVal($maxFileSizeMegaBytes) * 1000000;

if ($tipoCampo !== "FRP")
{
    $t_body->ACAO_UPLOAD = "jsUploadFile('formFD$FieldId', $FieldId)";    
    $t_body->block("BLOCK_SEL_ARQUIVO");
} else {
    $t_body->ACAO_UPLOAD = "jsUploadDocumentToRepository(jsAtualizaCampoArquivoRepositorio,'formFD$FieldId', 'json')";    
    $t_body->block("BLOCK_SEL_ARQUIVO_REPOSITORIO");
}

$t_body->ACCEPT_FILES = $acceptFiles;
$t_body->MAXFILESIZE = $maxFileSize;
$t_body->MAXFILESIZEBYTES = $maxFileSizeBytes;
$t_body->PROCID = $ProcId;
$t_body->PROCID_REPOSITORIO = $ProcId;
$t_body->STEPID_REPOSITORIO = $StepId;
$t_body->CASENUM = $CaseNum;
$t_body->FIELDID = $FieldId;
$t_body->TIPOCAMPO = $tipoCampo;
$t_body->show();



