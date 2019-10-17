<?php
// //Versao 1.0.0 /Versao
require_once("include/common.php");
require_once("include/resource01.php");
include("include/validasessao.php");
$DirName = formataCaseNum($ProcId, 3) . formataCaseNum($CaseNum, 8);
if (!empty($file)) {
    if (!is_dir("upload/$DirName")) {
        mkdir("upload/$DirName");
    }
    $filName = str_replace("&", " ", $file_name);
    if (!move_uploaded_file($file, "upload/$DirName/$file_name")) {
        echo "<span>Não foi possível enviar o arquivo.</span>";
    } else {
        echo "Arquivo enviado";
        $extendData = array();        
        $extendData["descricao"] = $descricao;
        $extendData["versao"] = $versao;
        $extendData["username"] = $userdef->UserName;
        $extendData["userid"] = $userdef->UserId;
        $extendData["data"] = Date('d/m/Y');
        $jsonExtendData = json_encode($extendData);
        audittrail(900, "Anexado arquivo: $file_name ");
        $SQL = "insert into casedata (ProcId, CaseNum, FieldValue, ValueId, FieldId, ExtendData) values ($ProcId, $CaseNum, '$Valor', $ValueId, $FieldId, '$jsonExtendData')";
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
$TotalArquivos = mysqli_num_rows($Query);
?>	
<script>
    window.parent.document.getElementById("r<?= $FieldId ?>").value = <?= $TotalArquivos ?>;
    window.parent.document.getElementById("a<?= $FieldId ?>").innerHTML = "Arquivos: " + <?= $TotalArquivos ?>;
    window.parent.CarregaFolder(<?= $ProcId ?>, <?= $StepId ?>, <?= $CaseNum ?>, <?= $FieldId ?>, 0);
</script>