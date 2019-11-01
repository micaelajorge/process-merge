<?php
function PegaCasosOciosos($CountQueue = 10)
{
	global $connect;
	$Hora = date('H:i:s');
	if ($CountQueue <= 0 || $CountQueue > 50)
	{
		$CountQueue = 1;
	}
	$SQL = "select distinct top $CountQueue casequeue.ProcId, 
	casequeue.StepId, 
	CaseId,
	stepdef.AutoStepOrder
	from 
	casequeue, stepdef, procdef 
	where 
	procdef.Active = 1
	and
	procdef.AutoStep = 1 
	and
	stepdef.Active = 1
	and
	stepdef.AutoStep = 1
	and
	procdef.ProcId = stepdef.ProcId 
	and 
	procdef.ProcId = casequeue.ProcId 
	and 
	stepdef.StepId = casequeue.StepId	
	and
	AutoStepOcioso = 1	
	order by
	stepdef.AutoStepOrder, casequeue.ProcId, CaseId";	
	$Query = mysqli_query($connect, $SQL);	
	return $Query;
}

function PegaCasosBrutForce($CountQueue = 10)
	{
	global $connect;
	if ($CountQueue <= 0 || $CountQueue > 50)
		{
		$CountQueue = 50;
		}
	$SQL = "select top $CountQueue * from autostepbrutforce ";
	$SQL .= " order by ProcId, StepId, CaseNum";
	$Query = mysqli_query($connect, $SQL);	
	return $Query;				
	}

function PegaCasos($CountQueue = 10)
{
	global $connect, $REMOTE_ADDR;
	$Hora = date('H:i:s');
	if ($CountQueue <= 0 || $CountQueue > 50)
	{
		$CountQueue = 50;
	}
	$SQL = "select top $CountQueue casequeue.ProcId, 
	casequeue.StepId, 
	CaseId,
	stepdef.AutoStepOrder
	from 
	casequeue, stepdef, procdef 
	where 
	procdef.Active = 1
	and
	procdef.AutoStep = 1 
	and
	stepdef.Active = 1
	and
	stepdef.AutoStep = 1
	and
	procdef.ProcId = stepdef.ProcId 
	and 
	procdef.ProcId = casequeue.ProcId 
	and 
	stepdef.StepId = casequeue.StepId	
	and
	(
		(
		stepdef.AutoStepOnTime <= convert(datetime, '1900-01-02 $Hora')
		and
		stepdef.AutoStepOffTime >= convert(datetime, '1900-01-02 $Hora')
		)
		or
		(
		stepdef.AutoStepOnTime <= convert(datetime, '1900-01-01 $Hora')
		and
		stepdef.AutoStepOffTime >= convert(datetime, '1900-01-01 $Hora')
		)
	)
	order by stepdef.AutoStepOrder * 10 - datediff(mi, QueueDate, now()),
	stepdef.AutoStepOrder, casequeue.ProcId, CaseId";	
	$Query = mysqli_query($connect, $SQL);	
	return $Query;
}

function autostepqueue($AuthUser, $CountQueue = 10)
	{
		$Queue = Funcautostepqueue($AuthUser, $CountQueue);
		return  $Queue;
	}

	
function PegaCasosAutoStep($CountQueue = 10)
{
	$Query = PegaCasosBrutForce($CountQueue);
	if ($Query)
		{
		if (mysqli_num_rows($Query) > 0)
			{
			return $Query;	
			}
		}
	$Query = PegaCasos($CountQueue);
	if ($Query)
		{
		if (mysqli_num_rows($Query) > 0)
			{
			return $Query;	
			}	
		}
	$Query = PegaCasosOciosos($CountQueue);
	if ($Query)
		{
		if (mysqli_num_rows($Query) > 0)
			{
			return $Query;	
			}	
		}
	return false;	
}

function Funcautostepqueue($AuthUser, $CountQueue = 10)
	{
	// Tratamento de Erro
	$Err[0]['ErrId'] = 0;
	$Err[0]['ErrDesc'] = 'Sem Erro';
	$Error['Count'] = 0;
	$Error['Itens'] = $Err;
/*
	if (!AuthenticateWs($AuthUser))
		{
		error_log('Falha Autentica��o');
		return;
		} 
		*/
	$Query = PegaCasosAutoStep($CountQueue);
	if (!$Query)
		{
		$Queue['NumCount'] = 0;
		$Queue['Items'] = $Items;
		$Queue['Error'] = $Error;
		return $Queue;	
		}
	
	$i = 0;
	while ($Result = mysqli_fetch_array($Query))
	{
		$Items[$i]['ProcId'] = $Result["ProcId"];
		$Items[$i]['StepId'] = $Result["StepId"];
		$Items[$i]['CaseNum'] = $Result["CaseId"];
		$i++;		
	}	
	$Queue['NumCount'] = count($Items);
	$Queue['Items'] = $Items;
	$Queue['Error'] = $Error;
	return $Queue;	
	}
?>