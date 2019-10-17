<?php
function CriaOS($Fields, $connect, $CaseNum)
{
	$C_NOME = 4;
	$C_Frequencia = 7;
	$C_DT_INICIO = 5;
	$C_DT_FIM = 6;
	$C_FREQUENCIA = 17;
	$C_PERIODO = 8;
	$C_REGIONALVIVO = 10;
	$C_GRUPO = 11;
	$C_PS = 13;
	$C_SOMENTEUTEIS;
	$NOME = $Fields->Field[$C_NOME]->FieldValueDB;
	$FREQ = $Fields->Field[$C_Frequencia]->FieldValueDB;
	$DT_INI = $Fields->Field[$C_DT_INICIO]->FieldValueDB;
	$DT_FIM = $Fields->Field[$C_DT_FIM]->FieldValueDB;
	$PERIODO = $Fields->Field[$C_PERIODO]->FieldValueDB;
	$ID_REGIONALVIVO = $Fields->Field[$C_REGIONALVIVO]->FieldValueDB;
	$ID_GRUPO = $Fields->Field[$C_GRUPO]->FieldValueDB;
	$PS = $Fields->Field[$C_PS]->FieldValueDB;
	$SOMENTEUTEIS  = $Fields->Field[$C_SOMENTEUTEIS]->FieldValueDB;
	IF (empty($PERIODO))
	{
		$PERIODO = 1;
	}
	IF ($APLICAFERIADO == "N�o")
	{
		$APLICAFERIADO = 0;
	}
	else {
		$APLICAFERIADO = 1;
	}

	switch ($FREQ)
	{
	case "Mensal":
		$FREQ = 1;
	case "Semanal":
		$FREQ = 2;
	case "Di�rio":
		$FREQ = 3;
	case "UnicaVez":
		$FREQ = 4;
	case "De <n> em <n> semanas":
		$FREQ = 5;
	}
	$SQL = "insert into faturamento_OS.dbo.OS 
	(
	ID_Ps,
	Nome, 
	Frequencia, 
	DataInicio, 
	DataFim, 
	NDias,
	SomenteUteis
	RegionalVivo
	ID_Grupoorigens,
	Status
	values 
	(
	 $PS,
	'$NOME', 
	$FREQ, 
	'$DT_INI', 
	'$DT_FIM',  
	$PERIODO,
	$SOMENTEUTEIS
	 $ID_REGIONALVIVO, 
	 $ID_GRUPO,
	 0 
	 )";
	mysqli_query($connect, $SQL);
}

?>