<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 05/08/2018
  Sistema: Process_XAMPP
 * 
 * ROTA: api/tribanco/getfile
 * ROTA: api/getfile
 * ROTA: viewfile
 * 
 */

require_once(FILES_ROOT . "include/class_mime.php");
require_once(FILES_ROOT . "include/resource01.php");

/**
 * 
 * @param type $fileStorageName
 * @param type $fileName
 * @param type $diretorioArquivo
 * @return boolean
 */
function DownloadFile($fileStorageName, $fileName, $diretorioArquivo = "")
{
    // Check filename 
    if (empty($fileStorageName) || !file_exists($diretorioArquivo . $fileStorageName)) {
        header("HTTP/1.0 400 Arquivo Nao Encontrado");
        echo "Dados Post: " . var_export($_POST, true);
        echo "$diretorioArquivo '$fileStorageName'";
        return FALSE;
    }
    // Create download file name to be displayed to user 

    $mime = new mime($fileName);
    $ContentType = $mime->getMime();
    // Send binary filetype HTTP header 


    header('Pragma: cache');
    header('Cache-Control: private');
    header('Content-Type: ' . $ContentType);
    // Send content-length HTTP header
    header('Content-Length: ' . filesize($diretorioArquivo . $fileStorageName));
    // Send content-disposition with save file name HTTP header
    //    header('Content-Disposition: attachment; filename="'.$filename.'"');
    header('Content-Disposition: attachment; filename="' . $fileName . '"');
    // Output file
    readfile($diretorioArquivo . $fileStorageName);
    // Done 
    return TRUE;
}

/**
 * 
 * @param type $fileStorageName
 * @param type $fileName
 * @param type $diretorioArquivo
 * @return boolean
 */
function ViewFile($fileStorageName, $fileName, $diretorioArquivo = "")
{
    $mime = new mime($fileName);

    $ContentType = $mime->getMime();
    // Check filename     

    if (empty($fileStorageName) || !file_exists($diretorioArquivo . $fileStorageName)) {
        header("HTTP/1.0 400 Arquivo Nao Encontrado");
        echo "Dados Post: " . var_export($_POST, true);
        echo "$diretorioArquivo$fileStorageName";
        return FALSE;
    }

    header('Pragma: cache');
    header('Cache-Control: private');
    header('Content-Type: ' . $ContentType);
    header('Content-Length: ' . filesize($diretorioArquivo . $fileStorageName));
    readfile($diretorioArquivo . $fileStorageName);
    return true;
}

function serveArquivo()
{
    global $userdef;
    $dadosEntrada = apiTrataDadosEntrada();
//    $ProcId = $dadosEntrada["ProcId"];
//    $CaseNum = $dadosEntrada["CaseNum"];
//    $fileName = $dadosEntrada["fileName"];
//    $fieldId = $dadosEntrada["fieldId"];
//    $fileNameStorage = $dadosEntrada["fileNameStorage"];
//    $forceDownload = $dadosEntrada["forceDownload"];

    $ProcId = $GLOBALS["ProcId"];
    $CaseNum = $GLOBALS["CaseNum"];
    $fileName = $GLOBALS["fileName"];
    $fieldId = $GLOBALS["fieldId"];

    if (!is_numeric($ProcId)) {
        $ProcId = PegaProcIdByCode($ProcId);
    }

    if (!is_numeric($fieldId)) {
        $fieldId = PegaFieldIdByCode($ProcId, $fieldId);
    }


    $fileNameStorage = $GLOBALS["fileNameStorage"];
    $forceDownload = $GLOBALS["forceDownload"];
    $diretorio = FILES_UPLOAD . "/" . diretorioArquivoImagem($ProcId, $CaseNum, $fieldId);

    insereEntradaAuditTrail($ProcId, $CaseNum, 0, $userdef->UserId, $userdef->UserDesc, 7, "$fileName");
    if ($forceDownload) {
        DownloadFile($fileNameStorage, $fileName, $diretorio);        
    } else {
        ViewFile($fileNameStorage, $fileName, $diretorio);
    }
}
