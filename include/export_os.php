<?php
function CriaOS($Fields, $connect, $CaseNum)
{
	$C_NOME = 4;
	$C_DataAbertura = 5;
	$C_DataAprovacao = 6;
	$C_DiretoriaDepartamento = 7;
	$C_TipoDocumento = 8;
	$C_Periodicidade = 9;
	$C_VolumeMedio = 10;
	$C_VolumeTotal = 28;
	$C_GRUPO = 12;
	$C_RiscoOperacional = 14;
	$C_GrandezaFinanceira = 16;
	$C_PS = 11;	
	$C_REGIONALVIVO = 13;
	$C_REQUISITOS_EXTRAS = 15;
	
	$NOME = $Fields->Field[$C_NOME]->FieldValueDB;
	$DT_APROVACAO = $Fields->Field[$C_DataAprovacao]->FieldValueDB;
	$DT_ABERTURA = $Fields->Field[$C_DataAbertura]->FieldValueDB;
	$PERIODICIDADE = $Fields->Field[$C_Periodicidade]->FieldValueDB;
	$ID_REGIONALVIVO = $Fields->Field[$C_REGIONALVIVO]->FieldValueDB;
	$ID_GRUPO = $Fields->Field[$C_GRUPO]->FieldValueDB;
	$PS_ORIGINAL = $Fields->Field[$C_PS]->FieldValueDB;
	$VOLUME_MEDIO = $Fields->Field[$C_VolumeMedio]->FieldValueDB;
	$VOLUME_TOTAL = $Fields->Field[$C_VolumeTotal]->FieldValueDB;
	$RISCO_OPERACIONAL = $Fields->Field[$C_RiscoOperacional]->FieldValueDB;

	ExportFieldLinhas($C_REQUISITOS_EXTRAS, $connect, $CaseNum);
	
	$SQL = "insert into Faturamento_OS.dbo.OS (
	OS_BPM,
	ID_PS,
	Nome,  
	DataInicio,
	DataFim, 
	Frequencia,
	RegionalVivo, 
	ID_GRUPOorigens,
	Status,
    VolumePrevisto,
    VolumeMedio,
    DataAprovacao,
    DataAbertura,
    RiscoOperacional
	) 
	values 
	(
	$CaseNum,
	$PS_ORIGINAL,
	'$NOME', 
	'$DT_INI', 
	'$DT_FIM',
	'$PERIODICIDADE', 
	$ID_REGIONALVIVO, 
	$ID_GRUPO, 
	0, 
	'$VOLUME_PREVISTO',
	'$VOLUME_MEDIO',
	'$DT_APROVACAO', 
	'$DT_ABERTURA',
	'$RISCO_OPERACIONAL'
	)";
	mysqli_query($connect, $SQL);
}

function ExportFieldLinhas($FieldId, $connect, $CaseNum)
{
	$SQL = "select * from casedata where CaseNum = $CaseNum and FieldId = $FieldId";
	$Query = mysqli_query($connect, $SQL);
	while ($Result = mysqli_fetch_array($Query))
	{
		$valores = explode(";", $Result["FieldValue"]);
		$Data = $valores[0];
		$Solicitante = $valores[1];
		$Texto = $valores[2];
		$SQL = "insert into Faturamento_OS.dbo.RequisitosExtras values ($CaseNum, '$Data','$Solicitante', '$Texto')";	
		mysqli_query($connect, $SQL);		
	}	
}

function CriaOSRH($Fields, $connect, $CaseNum)
{
	$C_NOME = 4;
	$C_Frequencia = 17;
	$C_DT_INICIO = 10;
	$C_DT_FIM = 6;
	$C_FREQUENCIA = 7;
	$C_ORIGEM = 6;
	$C_PERIODO = 8;
	$C_REGIONALVIVO = 10;
	$C_GRUPO = 11;
	$C_PSORIGINAL = 15;
	$C_DIASUTEIS = 9;
    $C_VOLUME_PREVISTO = 22;
	$NOME = $Fields->Field[$C_NOME]->FieldValueDB;
	$FREQ = 0;
	$DT_INI = $Fields->Field[$C_DT_INICIO]->FieldValueDB;
	$DT_FIM = $Fields->Field[$C_DT_FIM]->FieldValueDB;
	$PERIODO = 0;
	$ID_REGIONALVIVO = $Fields->Field[$C_REGIONALVIVO]->FieldValueDB;
	$ID_GRUPO = $Fields->Field[$C_GRUPO]->FieldValueDB;
	$PS_ORIGINAL = $Fields->Field[$C_PSORIGINAL]->FieldValueDB;
	$DIASUTEIS = 0;
	$VOLUME_PREVISTO = 0;
	if (empty($PS_ORIGINAL))
	{
		$PS_ORIGINAL = -1;
	}
	IF (empty($PERIODO))
	{
		$PERIODO = 1;
	}
	IF (empty($DIASUTEIS))
	{
		$DIASUTEIS = 0;
	}
	IF ($APLICAFERIADO == "Não")
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
	case "Diário":
		$FREQ = 3;
	case "UnicaVez":
		$FREQ = 4;
	case "De <n> em <n> semanas":
		$FREQ = 5;
	}
	$SQL = "insert into Faturamento_OS.dbo.OS (
	Nome, 
	Frequencia, 
	DataInicio,
	DataFim, 
	NDias, 
	RegionalVivo, 
	ID_PS, 
	ID_GRUPOorigens,
	SomenteUteis,
	Status,
        VolumePrevisto,
	OS_BPM
	) 
	values 
	('$NOME', $FREQ, '$DT_INI', '$DT_INI', $PERIODO, 0, 0, 0, $DIASUTEIS, 0, '$VOLUME_PREVISTO', '$CaseNum')";
	mysqli_query($connect, $SQL);
}
?>