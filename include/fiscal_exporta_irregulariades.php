<?php
function ExportaIrregularidades($CaseNum, $Fields, $connect)
{
	$C_NF = 12;
	$C_DATA_NF = 13;
	$C_VALOR_NF = 14;
	$C_TIPO_NF = 9;
	$NF = $Fields->Field[$C_NF]->FieldValueDB;
	$DATA = $Fields->Field[$C_DATA_NF]->FieldValueDB;
	$VALOR_NF = $Fields->Field[$C_VALOR_NF]->FieldValueDB;
	$TIPO_NF =$Fields->Field[$C_TIPO_NF]->FieldValueDB;

	switch ($TIPO_NF)
	{
		case 1:
			$C_IRREGULARIDADES = 4;
			break;
		case 2:
			$C_IRREGULARIDADES = 15;
			break;
		case 3:
			$C_IRREGULARIDADES = 18;
			break;
			
	}
	$IRREGULARIDADES = $Fields->Field[$C_IRREGULARIDADES]->FieldValueDB;
	if (empty($IRREGULARIDADES))
	{
		return;
	}
	$A_IRREGULARIDADES = explode(',', $IRREGULARIDADES);
	for ($i = 0; $i < count($A_IRREGULARIDADES); $i++)
	{
		$IRREGULARIDADE = str_replace("'","", $A_IRREGULARIDADES[$i]);
		if (!empty($IRREGULARIDADE))
		{			
			$SQL = "insert into fiscal_vivo.dbo.notas (numeronota, data, valor, cod_irregularidade, casenum) values ($NF, '$DATA', $VALOR_NF, $IRREGULARIDADE, $CaseNum)";						
			mysqli_query($connect, $SQL);
		}
	}
}
?>