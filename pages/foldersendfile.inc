<?php
// //Versao 1.0.0 /Versao
/**
 *  ROTA foldersendfile
 */

require_once(FILES_ROOT . "include/resource01.php");

if (key_exists("RemoteFile", $_FILES))
{
    $nomeCampoArquivo = "RemoteFile";
} else {
    $nomeCampoArquivo = "file";
}


$fileName = $_FILES[$nomeCampoArquivo]['name'];
$dirNameProc = formataCaseNum($ProcId, 6);
$dirNameCaseNum = formataCaseNum($CaseNum, 10);
$dirNameFolderId = formataCaseNum($FieldId, 6);

if (!empty($fileName)) {
    $dirName = criaDiretorioCampoArquivo($ProcId, $dirNameCaseNum, $dirNameFolderId);

    $SQL_TOTAL = "select max(ValueId) as totalArquivos from casedata where ProcId = $ProcId and CaseNum = $CaseNum and FieldId = $FieldId";
    $Query = mssql_query($connect, $SQL_TOTAL);
    $result = mysqli_fetch_array($Query);

    $valueId = intval($result["totalArquivos"]) + 1;    
    $valueIdFileName = formataCaseNum($valueId, 2);
    $fieldIdFileName = formataCaseNum($FieldId, 4);
    $extensaoArquivo = pathinfo($fileName, PATHINFO_EXTENSION);
    $fileNameStorage = $dirNameCaseNum . $fieldIdFileName . $valueIdFileName . "." . $extensaoArquivo;    

    if (!move_uploaded_file($_FILES[$nomeCampoArquivo]['tmp_name'], "$dirName/$fileNameStorage")) {
        echo "<span>Não foi possível enviar o arquivo.</span>";
    } else {
        echo "Arquivo enviado";
        $extendData = array();
        $extendData["fileNameStorage"] = $fileNameStorage;
        $extendData["fileName"] = $fileName;
        $extendData["descricao"] = $descricao;
        $extendData["versao"] = $versao;
        $extendData["username"] = $userdef->UserName;
        $extendData["userid"] = $userdef->UserId;
        $extendData["data"] = Date('Y-m-d H:i:s');
        $jsonExtendData = json_encode($extendData);
        audittrail(900, "Anexado arquivo: $fileName ");
        
        atualizaTOCdoCaso($dirName, $ProcId, $CaseNum, $FieldId, $extendData);
        $SQL = "insert into casedata (ProcId, CaseNum, FieldValue, ValueId, FieldId, ExtendData) values ($ProcId, $CaseNum, '$jsonExtendData', $valueId, $FieldId, '$jsonExtendData')";
        mysqli_query($connect, $SQL);
    }
} else {
    $DirName = formataCaseNum($ProcId, 3) . formataCaseNum($CaseNum, 8);
    atualizaTOCdoCaso($dirName, $ProcId, $CaseNum, $FieldId, $extendData);
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
$TotalArquivos = mysqli_num_rows($Query);