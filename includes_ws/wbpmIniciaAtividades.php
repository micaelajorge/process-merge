<?php
function IniciaAtividades($AuthUser)
	{
	FuncIniciaAtividades($AuthUser);
	}

function FuncIniciaAtividades($AuthUser)
	{
	global $connect;
	require_once("include/inctime.php");
	$Data = inctime(date('Y-m-d'), 1440, 1);
	$SQL = "select 
			CaseNum as Os,
			Atividades.ID_atividade,			
			Atividades.Volume,
			Atividades.ID_Os, 
			Atividades.ID_AtividadeOs, 
			Atividades.ID_ProcessoOs, 
			Os.Origem,
			Atividades.Periodo,
			TipoAtividade.ID_Volume,
			Atividades.Frequencia
			from 
			faturamento2.dbo.Atividades as Atividades, 
			faturamento2.dbo.TipoAtividade as TipoAtividade, 
			faturamento2.dbo.TipoProcesso as TipoProcesso,
			faturamento2.dbo.OS as Os
			where 
			OS_Processos.ID_ProcessoOs = Atividades.ID_ProcessoOs
			and
			TipoAtividade.ID_Atividade = Atividades.ID_Atividade
			and
			Atividades.DataExec = '$Data'
			and
			OS.ID_Os = Atividades.ID_Os ";
	$Query = mysqli_query($connect, $SQL);
	while ($Result = mysqli_fetch_array($Query))
		{		
		$ProcId = 13;
		$StepId = 1;
		$NovoCaso = 0;

		$i = 0;
		// OS
		$Valor["FieldId"] = 9;
		$Valor["Value"] = $Result["Os"];
		$Fields[$i] = $Valor;
			
		// Atividade
		$Valor["FieldId"] = 4;
		$Valor["Value"] = $Result["ID_atividade"];
		$i++;
		$Fields[$i] = $Valor;
		
		// Volume
		$Valor["FieldId"] = 6;
		$Valor["Value"] = $Result["Volume"];
		$i++;
		$Fields[$i] = $Valor;

		// Data Prevista
		$Valor["FieldId"] = 11;
		$Valor["Value"] = $Data;
		$i++;
		$Fields[$i] = $Valor;
	
		// ID_OS
		$Valor["FieldId"] = 14;
		$Valor["Value"] = $Result["ID_Os"];
		$i++;
		$Fields[$i] = $Valor;

		// ID_AtividadeOs
		$Valor["FieldId"] = 15;
		$Valor["Value"] = $Result["ID_AtividadeOs"];
		$i++;
		$Fields[$i] = $Valor;

		// ID_ProcessoOS
		$Valor["FieldId"] = 16;
		$Valor["Value"] = $Result["ID_ProcessoOs"];
		$i++;
		$Fields[$i] = $Valor;
		
		// Origem
		$Valor["FieldId"] = 8;
		$Valor["Value"] = $Result["Executor"];
		$i++;
		$Fields[$i] = $Valor;

		// ID_TipoVolume
		$Valor["FieldId"] = 5;
		$Valor["Value"] = $Result["ID_Volume"];
		$i++;
		$Fields[$i] = $Valor;

		$casedata["Fields"] = $Fields;
		$casedata["NumFields"] = count($Fields);
		
		$Acao = 0;				
		$Periodo = $Result["Periodo"];
		FuncCaseStart($AuthUser, $ProcId, $StepId, $casedata, $Acao);
		AtualizaDataExec($Result["ID_AtividadeOs"], $Result["Frequencia"], $Periodo);
		}	
	$Err[0]['ErrId'] = 0;
	$Err[0]['ErrDesc'] = 'Sem Erro';
	$Error['Count'] = count($Err);
	$Error['Itens'] = $Err;
	return $Error;
	}

function AtualizaDataExec($ID_AtividadeOs, $Frequencia, $Periodo)
{
	global $connect;
	$Mes = 43200;
	$Semana = 10080;
	$Dia = 1440;
	$Data = inctime(date('Y-m-d'), 1440, 1);
	if ($Frequencia == 1) // Frequencia Mensal
	{
		$Data = inctime($Data, 43200, 1);
	}		
	if ($Frequencia == 2) // Frequencia Semanal
	{
		$Data = inctime($Data, 10080, 1);
	}		
	if ($Frequencia == 3) // Frequencia Diária
	{
		$Data = inctime($Data, 1440, 1);		
	}		
	if ($Frequencia == 4) // 
	{
		return; // Não altera a data;
	}		
	if ($Frequencia == 5) // Frequencia Unica Vez / Exporádico
	{
		if (empty($Periodo))
		{
			$Periodo = 1;
		}
		$Valor = $Semana * Periodo;
		$Data = inctime($Data, $Valor, 1);		
	}		
	
	$Data = incTime($Data, 0, 0);
	$SQL = "update faturamento2.dbo.Atividades set DataExec = '$Data' where ID_AtividadeOS = $ID_AtividadeOs";
	mysqli_query($connect, $SQL);	
}
?>