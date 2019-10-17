<?PHP
// //Versao 1.0.0 /Versao
require_once("include/common.php");
include("include/validasessao.php");
function DownloadFile($filename) 
	{ 
    // Check filename 
    if (empty($filename) || !file_exists($filename)) 
    	{ 
        return FALSE; 
    } 
    // Create download file name to be displayed to user 
    $saveasname = basename($filename); 
    // Send binary filetype HTTP header 
    header('Content-Type: application/octet-stream'); 
    // Send content-length HTTP header 
    header('Content-Length: '.filesize($filename)); 
    // Send content-disposition with save file name HTTP header 
    header('Content-Disposition: attachment; filename="'.$saveasname.'"'); 
    // Output file 
    readfile($filename); 
    // Done 
    return TRUE; 
} 
require_once("include/resource01.php");
$SQL = "select ValueId, FieldValue from casedata where ProcId = $ProcId and FieldId = $FieldId and CaseNum = $CaseNum order by ValueId desc";
$Query = mysqli_query($connect, $SQL);
$Incluir = "BPMFolderFile.php?ProcId=$ProcId&CaseNum=$CaseNum&FieldId=$FieldId";
$DirName = formataCaseNum($ProcId, 3) . formataCaseNum($CaseNum,8);
?>
<html>
<head>
<script language="JavaScript" src="javascript/posicionar.js"></script>
<script language="JavaScript" src="javascript/bloqueiamenu.js"></script>
<script language="JavaScript">
function AbreArquivo(Source) 
	{ 
	if (Source.length == 0)
		{
		alert("Não há arquivo para Abrir/Salvar");
		return;
		}
//	WinSelect = window.open("BPMViewFile.php?ProcId=<?php echo $ProcId ?>&CaseNum=<?php echo $CaseNum ?>&Target=" + Source + "&Source=" + Source ,"Select","toolbar=0,Location=0,directories=0,status=1,menubar=0,scrollbars=1"); 
	WinSelect = window.open("BPMViewFile.php?ProcId=<?= $ProcId ?>&CaseNum=<?= $CaseNum ?>&Target=" + Source,"Select"); 
	WinSelect.focus();	
	};
</script>
<title>Arquivos</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<style type="text/css">
<!--
-->
</style>
<link href="STYLES/flow.css" rel="stylesheet" type="text/css">
<style type="text/css">
<!--
a:hover {
	color: #FF0000;
	text-decoration: underline;
}
a {
	color: #0000FF;
	text-decoration: underline;
	font-size: 12px;
}
-->
</style>
<style type="text/css">
<!--
-->
</style>
</head>
<body>
<div id="Cabecalho">
<a href="#" onclick="window.close()">Fechar</a><br><br><br>
<?php
if ($ReadOnly == 0)
	{
	?>
<a href="#" onClick="window.open('<?php echo $Incluir ?>','selarquivo','toolbar=0,location=0,width=540,height=165,left=' + PegaPosX(540) + ',top=' + PegaPosY(165))">Incluir Arquivo</a>
<?php
	}
	?>
</div>
<br>
      <table width="100%" border="1" cellpadding="2" cellspacing="0" bordercolor="#666666" class="TableBorda">
        <tr style="background-color:#FFFFFF"> 
          <td align="center" bgcolor="#CCCCCC">Arquivo</td>
          <td align="center" bgcolor="#CCCCCC">Descri&ccedil;&atilde;o</td>
          <td align="left" bgcolor="#CCCCCC"><div align="center">Vers&atilde;o</div></td>
          <td align="center" bgcolor="#CCCCCC"">&nbsp;</td>
        </tr>
        <?php
  $contador = 0;
  while ($Linha = mysqli_fetch_array($Query))
  	{
	$Arquivo = $Linha["FieldValue"];
	$Arquivo = explode(';', $Arquivo);
	$ValueId = $Linha["ValueId"];
	$Abrir =  $Arquivo[0];
	$Excluir = "flow0037.php?ProcId=$ProcId&CaseNum=$CaseNum&FieldId=$FieldId&ValueId=$ValueId&Arquivo=" . str_replace(" ", '%20', $Arquivo[0]);
	?>
        <tr> 
          <td width="317" align="center"><a href="javascript:" onClick="AbreArquivo('<?= $Abrir ?>')"><?= $Arquivo[0] ?></a></td>
          <td width="233" align="center"><?= $Arquivo[1] ?></td>
          <td width="170" align="center"><?= $Arquivo[2] ?></td>
		  <td width="289" align="center"><?php if ($ReadOnly <> 1) { ?><a href="#" onClick="window.open('<?= $Excluir ?>','','width=1,height=1,toolbar=0,location=0')">Excluir Arquivo</a> <?php } ?>
          </td>
        </tr>
        <?php
  	}
	?>
      </table>
      <?php
	$contador = 6 - $contador;
	for ($contador2=0; $contador2 < $contador; $contador2++)
		{
		echo "<br>";
		}
?>	
</body>
</html>
