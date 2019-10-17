<?php
	$Tipo = "t";
	if ($ReadOnly == 1)
		{
		$Desabled = "disabled";
		$Tipo = "L";
		}
?>
<select name="<?= $Tipo . $campo ?>" <?= $Desabled ?>>
	 <option value=""></option>
<?php
	$Grupos = MontaCampoGrupos($Valor, $ReadOnly);
	for ($i = 0; $i < count($Grupos); $i++)
		{
		?>
  <option value="<?= $Grupos[$i]["Id"] ?>" <?= $Grupos[$i]["Sel"] ?>><?= $Grupos[$i]["Nome"] ?></option>
  <?php
  	}
	?>
</select>