<?php

// //Versao 1.0.0 /Versao
/**
 * ROTA: fileattachupload
 */

require_once(FILES_ROOT . "include/common.php");
require_once(FILES_ROOT . "include/resource01.php");
include(FILES_ROOT . "include/validasessao.php");
$fileName = $_FILES["file$FieldId"]['name'];

//error_log(var_export($_FILES, true));

$dirName = criaDiretorioCampoArquivo($ProcId, $CaseNum, $FieldId);

if (!empty($fileName)) {  
    $valueId = 1;
    
    // Pega Extenção Arquivo
    $extensaoArquivo = pathinfo($fileName, PATHINFO_EXTENSION);    
    
    // Cria nome Arquivo 
    $fileNameStorage = criaNomeArquivoUpload($CaseNum, $valueId, $FieldId, $extensaoArquivo);

    if (!move_uploaded_file($_FILES["file$FieldId"]['tmp_name'], "$dirName/$fileNameStorage")) {
        echo "alert('Não foi possível enviar o arquivo.');";
        return;
    } else {
        
        // Cria os dados Extendidos do Campo Arquivo
        $extendData = criaExtendDataCampoArquivo($fileNameStorage, $fileName, $userdef->UserName, $userdef->UserId, true);
        audittrail(900, "Anexado arquivo: $fileName ");

        $SQL = "delete from casedata where ProcId = $ProcId and CaseNum = $CaseNum and FieldId = $FieldId";
        mysqli_query($connect, $SQL);
        $SQL = "insert into casedata (ProcId, CaseNum, FieldValue, ValueId, FieldId, ExtendData) values ($ProcId, $CaseNum, '$fileName', $valueId, $FieldId, '$jsonExtendData')";
        mysqli_query($connect, $SQL);
    }
} else {
    $DirName = formataCaseNum($ProcId, 3) . formataCaseNum($CaseNum, 8);
    if (!empty($Arquivo)) {
        if (file_exists("upload/" . $DirName . "/" . $Arquivo)) {
            unlink("upload/" . $DirName . "/" . $Arquivo);
        }
        audittrail(900, "Removido arquivo: $Arquivo ");
        $SQL = "delete from casedata where ProcId = $ProcId and CaseNum = $CaseNum and ValueId = $ValueId and FieldId = $FieldId ";
        mysqli_query($connect, $SQL);
    }
}
$SQL = "select FieldValue, ValueId from casedata where ProcId = $ProcId and FieldId = $FieldId and CaseNum = $CaseNum order by ValueId desc";
$Query = mysqli_query($connect, $SQL);
       
$extendData["valueId"] = $valueId;

$jExtendData = addslashes(json_encode($extendData));
atualizaTOCdoCaso($dirName, $ProcId, $CaseNum, $FieldId, $extendData);
$TotalArquivos = mysqli_num_rows($Query);
//header('Content-Type: application/json');
//header('Access-Control-Allow-Origin: http://localhost');
//header('Access-Control-Allow-Methods: GET, PUT, POST, DELETE, OPTIONS');
//header('Access-Control-Max-Age: 1000');
//header('Access-Control-Allow-Headers: Content-Type, Authorization, X-Requested-With');
$Retorno = "jsAtualizaCampoArquivo('$FieldId', '$fileName', '$jExtendData');";
echo $Retorno;
