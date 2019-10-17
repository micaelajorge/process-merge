<?php
// //Versao 1.0.0 /Versao
include("include\common.php");
$SQL = "select * from FeriadosFederais order by Nome";
$Query = mysqli_query($connect, $SQL);
$TituloPagina = "Feriados Federais";
?> 
<html>
<head>
<title>Sistema de Faturamento - <?= $TituloPagina ?></title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link href="styles/normal.css" rel="stylesheet" type="text/css">
<link href="styles/lista.css" rel="stylesheet" type="text/css">
</head>
<body leftmargin="0" topmargin="0" marginwidth="0" marginheight="0">
<div id="Acoes">
<a href="index.php" title="Voltar">Voltar</a>
<a href="FeriadosFederaisEdit.php" title="Criar Novo">Criar Novo</a>

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
	$Indice = $Result["ID_Feriado"];
?>
    <tr> 
      <td  id="Item"><a href="FeriadosFederaisEdit.php?Id=<?= $Indice ?>" title="Editar">Editar</a> 
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
