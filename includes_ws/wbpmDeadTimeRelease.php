<?php
function DeadTimeRelease($AuthUser)
	{
	FuncDeadTimeRelease($AuthUser, 'S');
	FuncDeadTimeRelease($AuthUser, 'H');
	FuncDeadTimeRelease($AuthUser, 'D');
	}

function FuncDeadTimeRelease($AuthUser, $Tipo)
	{
	global $connect;
	$SQL = "select  ";
	$SQL .= "ProcId,  ";
	$SQL .= "StepId,  ";
	$SQL .= "CaseId ";
	$SQL .= "from  ";
	$SQL .= "casequeue ";
	$SQL .= "where  ";
	switch ($Tipo)
		{
		case 'S':
		$SQL .= "casequeue.DeadSoftDateTime < getdate() and DeadSoftExec = 1 ";
		$Acao = 3;
		break;
		case 'H':
		$SQL .= "casequeue.DeadHardDateTime < getdate() and DeadHardExec = 1 ";
		$Acao = 4;
		break;
		case 'D':
		$SQL .= "casequeue.DeadHardestDateTime < getdate() and DeadHardestExec = 1 ";
		$Acao = 5;
		break;
	}
	$Query = mysqli_query($connect, $SQL);
	while ($Result = mysqli_fetch_array($Query))
		{		
		$ProcId = $Result["ProcId"];
		$StepId = $Result["StepId"];
		$CaseNum = $Result["CaseId"];
		$NovoCaso = 0;
		$dadosDoCaso = array();
		FuncCaseSave($AuthUser, $ProcId, $StepId, $CaseNum, $dadosDoCaso, $Acao, $NovoCaso);
		}	
	$Err[0]['ErrId'] = 0;
	$Err[0]['ErrDesc'] = 'Sem Erro';
	$Error['Count'] = count($Err);
	$Error['Itens'] = $Err;
	return $Error;
	}
?>