<?php
// //Versao 1.0.0 /Versao
include("include\common.php");
$TituloPagina = "Feriados Estaduais";
if (!empty($Id))
	{
	$SQL = "select * from FeriadosEstaduais where Id_Feriado = $Id";
	$Query = mysqli_query($connect, $SQL);
	$Result = mysqli_fetch_array($Query);
	$Nome = $Result["Nome"];
	$Data = explode('/', $Result["Data"]);
	}
?> 
<html>
<head>
<title>Sistema de Faturamento - <?= $TituloPagina ?></title>
<script language="JavaScript">
function SetaFocusInicial()
	{
	document.getElementById("Nome").focus();
	}
function ValidaForm()
	{
	obj = document.getElementById("Nome");
	if (obj.value == "")
		{
		alert('Valor inválido');
		obj.focus();
		return false;
		}
	obj = document.getElementById("Dia");
	if (obj.value == "")
		{
		alert('Valor inválido');
		obj.focus();
		return false;
		}
	obj = document.getElementById("Mes");
	if (obj.value == "")
		{
		alert('Valor inválido');
		obj.focus();
		return false;
		}
	return true;
	}

function Apagar()
	{
	if (confirm("Remover este item???"))
		{
		document.getElementById('Acao').value = 'Apagar';
		document.FrmEdit.submit();
		}
	}
	
function Salvar()
	{
	if (ValidaForm())
		{
		document.FrmEdit.submit();
		}
	}</script>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link href="styles/normal.css" rel="stylesheet" type="text/css">
<link href="styles/lista.css" rel="stylesheet" type="text/css">
</head><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>
<body leftmargin="0" topmargin="0" marginwidth="0" marginheight="0" onLoad="SetaFocusInicial()">
<div id="Acoes"> <a href="FeriadoEstadualList.php?Estado=<?= $Estado ?>" title="Voltar">Voltar</a> <a href="#" title="Salvar" onClick="Salvar()">Salvar</a> 
  <?php
if (!empty($Id))
	{
?>
  <a href="#" title="Apagar" onClick="Apagar()">
  Apagar</a> 
<?php } ?>
</div>
<div id="DivEdit">
<form name="FrmEdit" action="FeriadosEstaduaisPost.php">
    <table width="429" border="1" cellpadding="2" cellspacing="0" bordercolor="#000000" id="TableList">
      <tr>
        <td id="ItemEdit">Estado</td>
        <td><input type="text" name="EstadoNome" value="<?= $EstadoNome ?>"></td>
      </tr>
      <tr> 
        <td width="115" id="ItemEdit">Nome</td>
        <td width="300"><input name="Id" type="hidden" readonly="true" value="<?= $Id ?>"> 
          <input name="Acao" type="hidden" id="Acao"> <input name="Estado" type="hidden" id="Estado" value="<?= $Estado ?>"> 
          <input name="Nome" type="text" tabindex="0" size="50" maxlength="50" id="nome" value="<?= $Nome ?>"></td>
      </tr>
      <tr> 
        <td id="ItemEdit">Data</td>
        <td><input name="Dia" type="text" id="Dia" value="<?= $Data[0] ?>" size="2" maxlength="2">
          / 
          <input name="Mes" type="text" id="Mes" value="<?= $Data[1] ?>" size="2" maxlength="2"></td>
      </tr>
    </table>
</form>
</div>
</body>
</html>
