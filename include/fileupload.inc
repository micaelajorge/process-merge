<?php

// In PHP versions earlier than 4.1.0, $HTTP_POST_FILES should be used instead
// of $_FILES.

require_once(FILES_ROOT . "include/common.php");
include(FILES_ROOT . "include/validasessao.php");
require_once(FILES_ROOT . "include/resource01.php");

$uploaddir = 'uploads/';
$uploadfile = $uploaddir . basename($_FILES['file']['name']);
$uploadFileName = basename($_FILES['file']['name']);
if (move_uploaded_file($_FILES['file']['tmp_name'], $uploadfile)) {
    header("HTTP/1.0 200 OK");
    echo "Ok";
} else {
    header("HTTP/1.0 500 Erro");
}

$ExtendDataField = array();

$ExtendDataField["descricao"] = $descricao;
$ExtendDataField["versao"] = $versao;
$ExtendDataField["userid"] = $userId;
$jsonExtendDataField = json_encode($ExtendDataField);

$SQL = "insert into casedata (ProcId, CaseNum, FieldId, FieldValue, ValueId, ExtendData) values ($ProcId, $CaseNum, $FieldId, '$uploadFileName', $valueId, '$jsonExtendDataField')";
$resultadoQuery = mysqli_query($connect, $SQL);
if ($resultadoQuery) {
    echo "Arquivo Enviado";
}
?>