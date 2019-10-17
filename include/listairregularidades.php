<?php
include("include\common.php");
AtivaDB_Fiscal();
$SQL = "select * from irregularidades";
$Query = mysqli_query($connect, $SQL);
$TituloPagina = "Irregularidades";
?> 
  <table width="100%" border="1" cellpadding="2" cellspacing="0" bordercolor="#000000" id="TableList">
    <tr> 
      <td width="7%" id="Titulo">&nbsp;</td>
      <td width="37%" id="Titulo">Descri&ccedil;&atilde;o</td>
      <td width="16%" id="Titulo">NF Sa&iacute;da</td>
      <td width="18%" id="Titulo">NF Sa&iacute;da Cancelada</td>
      <td width="22%" id="Titulo">NF Entrada</td>
    </tr>
    <?php
while ($Result = mysqli_fetch_array($Query))
	{
	$Indice = $Result["cod_irregularidade"];
	$descricao = $Result["descricao"];
	$nfsaida = "N�o";
	$nfcancelada = "N�o";
	$nfentrada = "N�o";
	if ($Result["nfsaida"] == 1)
		$nfsaida = "Sim";
	if ($Result["nfcancelada"] == 1)
		$nfcancelada = "Sim";
	if ($Result["nfentrada"] == 1)
		$nfentrada = "Sim";
		
?>
    <tr> 
      <td  id="Item"><a href="irregularidadesedit.php?Id=<?= $Indice ?>" title="Editar">Editar</a> 
      </td>
      <td id="Item"> 
        <?= $descricao ?>
        &nbsp;</td>
      <td id="Item"><?= $nfsaida ?></td>
      <td id="Item"><?= $nfcancelada ?></td>
      <td id="Item"> 
        <?= $nfentrada ?>
        &nbsp;</td>
    </tr>
    <?php
	}
	?>
  </table>
