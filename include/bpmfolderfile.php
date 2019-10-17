<?php
// //Versao 1.0.0 /Versao
//require_once("include/common.php");
require_once("include/resource01.php");
include("include/validasessao.php");
$DirName = formataCaseNum($ProcId, 3) . formataCaseNum($CaseNum,8);
?>
<html>
<head>
<link href="STYLES/flow.css" rel="stylesheet" type="text/css">
<script language="JavaScript" src="javascript/bloqueiamenu.js"></script>
<script language="JavaScript" src="javascript/posicionar.js"></script>
<script language="JavaScript">
function PreparaPagina()
	{
	Posiciona();
	}
	
function MudaArquivo(Arquivo)
	{
	window.opener.document.location="BPMFolderList.php?ProcId=<?php echo $ProcId ?>&CaseNum=<?php echo $CaseNum ?>&FieldId=<?php echo $FieldId ?>";
	window.close();
	}
	
</script>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<title>Incluir Arquivo</title></head>
<?php
if(empty($file)) {
?>
<body>
<?php include("roundtop.htm"); ?>
<table width="476" border="0" cellpadding="0" cellspacing="0">
      <form method="POST" enctype="multipart/form-data" >
        <tr> 
          <td colspan="3" class="texto1">Seleção de Arquivo</td>
        </tr>
        <tr> 
          <td width="147"><div align="right" class="texto1"> 
              <?php if ($Tipo == "IM") { ?>
              Imagem: 
              <?php } else { ?>
              Arquivo: 
              <?php } ?>
            </div></td>
          <td width="13">&nbsp;</td>
          <td width="316"><input name="file" type="file" size="40"> <input name="Tipo" type="hidden" value="<?php echo $Tipo ?>"> 
            <input name="ProcId" type="hidden" id="ProcId" value="<?php echo $ProcId ?>"> 
            <input name="CaseNum" type="hidden" id="CaseNum" value="<?php  echo $CaseNum ?>"> 
            <input name="FieldId" type="hidden" value="<?php echo $FieldId ?>"></td>
        </tr>
        <tr> 
          <td><div align="right" class="texto1">Descri&ccedil;&atilde;o:</div></td>
          <td>&nbsp;</td>
          <td><input name="descricao" type="text" size="30" maxlength="30"></td>
        </tr>
        <tr> 
          <td align="right" class="texto1">Vers&atilde;o:</td>
          <td>&nbsp;</td>
          <td><input name="versao" type="text" id="versao" size="10" maxlength="10"></td>
        </tr>
        <tr> 
          <td> <input name="Input" type="submit" class="buttonBlue" style="width:60"value="Enviar"></td>
          <td>&nbsp;</td>
          <td align="right"><input name="Input2" type="button" class="buttonred" style="width:60" value="Cancelar" onClick="window.close()"></td>
        </tr>
      </form>
    </table>
	<?php include("roundbotton.htm"); 
} else
if(!empty($file)) 
	{
	?>
	<body onload="MudaArquivo()">
	<?php
	if(!is_dir("upload/$DirName"))
		{
		mkdir("upload/$DirName");
		}
	if(!move_uploaded_file($file, "upload/$DirName/$file_name"))
		{ 
        echo "<span>N�o foi poss�vel enviar o arquivo.</span>"; 
    	}
		else
		{ 
        echo "Arquivo enviado"; 
		//require_once("include/common.php");
		$Valor = $file_name . ";" . $descricao . ";" . $versao;
		$ValueId = 1;
		$SQL = "select count(*) as val from casedata where ProcId = $ProcId and CaseNum = $CaseNum and FieldId = $FieldId ";
		$Query = mysqli_query($connect, $SQL);
		$Linha = mysqli_fetch_array($Query);
		$ValueId = $Linha["val"] + 1;
				
		$SQL = "insert into casedata (ProcId, CaseNum, FieldValue, ValueId, FieldId) values ($ProcId, $CaseNum, '$Valor', $ValueId, $FieldId)";
		mysqli_query($connect, $SQL);		
    	} 	
	}		
?>
</body>
</html>