<?php
header('HTTP/1.0 200 ok');
$fileName = $_FILES['file']['name'];
$dirNameProc = str_pad($ProcId, 6, '0', STR_PAD_LEFT);
$dirNameCaseNum = str_pad($CaseNum, 8, '0', STR_PAD_LEFT);
$dirName = FILES_UPLOAD . "/" . $dirNameProc . "-" . $dirNameCaseNum;
if (!is_dir($dirName)) {
    mkdir($dirName);
}

if (!move_uploaded_file($_FILES['file']['tmp_name'], "$dirName/$fileName")) {
    echo "<span>Não foi possível enviar o arquivo.</span>";
} else {
    echo "Arquivo enviado";
}