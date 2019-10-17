<?php
// //Versao 1.0.0 /Versao
include("include\common.php");
$SQL = "select * from estados order by Nome";
$Query = mysqli_query($connect, $SQL);
$estados = array();
while ($Result = mysqli_fetch_array($Query))
	{
	if ($Result["Sigla"] == $Estado)
		{
		$Result["Selected"] = "selected";
		$EstadoNome = $Result["Nome"];
		}
	array_push($estados, $Result);
	}
$SQL = "select * from FeriadosEstaduais where Estado = '$Estado'";
$Query = mysqli_query($connect, $SQL);
$TituloPagina = "Feriados Estaduais";

?> 
<html>
<head>
<title>Sistema de Faturamento - <?= $TituloPagina ?></title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link href="styles/normal.css" rel="stylesheet" type="text/css">
<link href="styles/lista.css" rel="stylesheet" type="text/css">
<link href="styles/form.css" rel="stylesheet" type="text/css">
</head>
<body leftmargin="0" topmargin="0" marginwidth="0" marginheight="0">
<div id="Acoes">
<a href="index.php" title="Voltar">Voltar</a>
<a href="FeriadosEstaduaisEdit.php?Estado=<?= $Estado ?>&EstadoNome=<?= $EstadoNome ?>" title="Criar Novo">Criar Novo</a>
<br>
<br>
<form name="FrmEstado" action="FeriadosEstaduaisList.php">
Estado: <select name="Estado" onChange="document.FrmEstado.submit()">
<?php
	foreach ($estados as $Estado)
		{
		?>
      <option value="<?= $Estado["Sigla"] ?>" <?= $Estado["Selected"] ?>><?= $Estado["Nome"] ?></option>
	  <?php
	  }
	  ?>
    </select>
</form>
</div>
<div id="DivLista">
  <table width="100%" border="1" cellpadding="2" cellspacing="0" bordercolor="#000000" id="TableList">
    <tr> 
      <td width="10%" id="Titulo">&nbsp;</td>
      <td width="19%" id="Titulo">Nome</td>
      <td width="35%" id="Titulo">Data</td>
    </tr>
    <?php
while ($Result = mysqli_fetch_array($Query))
	{
	$Indice = $Result["ID_Estado"];
?>
    <tr> 
      <td  id="Item"><a href="FeriadosEstaduaisEdit.php?Id=<?= $Indice ?>" title="Editar">Editar</a> 
      </td>
      <td id="Item"> 
        <?= $Result["Nome"] ?>
        &nbsp;</td>
      <td id="Item"><?= $Result["Data"] ?>&nbsp;</td>
    </tr>
    <?php
	}
	?>
  </table>
</div>
</body>
</html>
