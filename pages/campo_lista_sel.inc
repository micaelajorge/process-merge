<?php
include_once("include/resource01.php");
$valores = PegaDadosCampoLista($ProcId, $FieldId, $Valor);
if (e_ordenado($ProcId, $FieldId))
	{
	sort($valores);
	}
?>
<table align="center" cellpadding="0" cellspacing="0" width="100%">
    <?php
	for ($contador = 0;  $contador < count($valores); $contador++)
		{ 
		$classeSel = "class=\"UnSelItem\"";
		if ($valores[$contador]["Valor"] == $Valor)
			{
			$classeSel = "class=\"SelItem\"";
			}
			?>  		
        <tr> 
          <td <?= $classeSel ?>><a href="javascript:void(0)" onClick="MudaItemLista('<?= $valores[$contador]["Display"] ?>','<?= $valores[$contador]["Valor"] ?>')"><?= $valores[$contador]["Display"] ?></a></td>
        </tr>
        <?php
		}
		?>
</table>
