<?php

require_once(FILES_ROOT . "include/resource01.php");

function DownloadFile($filename)
{
    // Check filename 
    if (empty($filename) || !file_exists($filename)) {
        return false;
    }
    // Create download file name to be displayed to user 
    $saveasname = basename($filename);
    // Send binary filetype HTTP header 
    header('Content-Type: application/octet-stream');
    // Send content-length HTTP header 
    header('Content-Length: ' . filesize($filename));
    // Send content-disposition with save file name HTTP header 
    header('Content-Disposition: attachment; filename="' . $saveasname . '"');
    // Output file 
    readfile($filename);
    // Done 
    return false;
}

$DirName = formataCaseNum($ProcId, 3) . formataCaseNum($CaseNum, 8);

// <editor-fold defaultstate="collapsed" desc="Chamada Classe de Template">
require_once(FILES_ROOT . "vendor/lib/raelgc/view/template.php");

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_folderlist.html");
// </editor-fold>

$t_body->MAXFILESIZE = ini_get('upload_max_filesize');
$t_body->PROCID = $ProcId;
$t_body->CASENUM = $CaseNum;
$t_body->FIELDID = $FieldId;
$t_body->USERID = $userdef->UserId;

$SQL = "select FieldValue, ValueId, ExtendData from casedata where ProcId = $ProcId and FieldId = $FieldId and CaseNum = $CaseNum order by ValueId desc";
$Query = mysqli_query($connect, $SQL);
$Incluir = "BPMFolderFile.php?ProcId=$ProcId&CaseNum=$CaseNum&FieldId=$FieldId";

// contador de arquivos Começa no 1 para se não houver arquivos, já estar em 1
$contadorArquivos = 1;
while ($Linha = mysqli_fetch_array($Query)) {
    $fieldValue = $Linha["FieldValue"];
    $extendData = $Linha["ExtendData"];
    if ($extendData != "") {
        $jsonExtendData = json_decode($extendData, true);
    } else {
        $jsonExtendData = json_decode($fieldValue, true);
    }
    $ValueId = $Linha["ValueId"];
    $t_body->Excluir = "BPMFolderSendFile.php?ProcId=$ProcId&StepId=$StepId&CaseNum=$CaseNum&FieldId=$FieldId&ValueId=$ValueId&Arquivo=" . str_replace(" ", '%20', $nomeArquivo);
    $t_body->ABRIR = $jsonExtendData["fileName"];
    $t_body->ARQUIVO_NOME = $jsonExtendData["fileName"];
    $t_body->ARQUIVO_NOME_JS = addslashes($jsonExtendData["fileName"]);
    $t_body->ARQUIVO_NOME_STORAGE = $jsonExtendData["fileNameStorage"];
    $t_body->ARQUIVO_DESCRICAO = $jsonExtendData["descricao"];
    $t_body->ARQUIVO_USUARIO = "";
    $t_body->ARQUIVO_DATA = ConvDate($jsonExtendData["data"]);
    $t_body->FIELD_ID = $FieldId;
    if ($ReadOnly != 1) {
        $ARQUIVO_4 = $Arquivo[4];
        if ($ReadOnly != 1) {
            $t_body->block("BLOCK_APAGAR_ARQUIVO");
        }
        $contadorArquivos++;
    }
    $t_body->block("BLOCK_ARQUIVO");
}
if ($contadorArquivos == 0) {
    $path = FILES_UPLOAD . "/" . str_pad($ProcId, 6, '0', STR_PAD_LEFT) . "-" . str_pad($CaseNum, 8, '0', STR_PAD_LEFT);
    $contadorArquivos = ContaArquivosDiretorio($path);
    $t_body->block("BLOCK_TOTAL_ARQUIVOS");
} else {
    $t_body->block("BLOCK_LISTA_ARQUIVOS");
}
$t_body->VALUEID = $contadorArquivos;

if ($ReadOnly == '0') {
    $t_body->block("BLOCK_ADD_FD");
}
$t_body->show();
