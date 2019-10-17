<?php
function EAdminDoProcesso($ProcId)
	{
	global $userdef, $connect;	
	$Retorno = false;
	$SQL =  " (select distinct procdef.ProcId from procdef, procadmins where procadmins.GroupId = $userdef->UserId_Process and procdef.ProcId = procadmins.ProcId and procadmins.GrpFld='U' and procadmins.ProcId = $ProcId  and Action = 1)";
	$SQL .= " union ";
	$SQL .= " (select distinct procdef.ProcId from procdef, procadmins where procadmins.GroupId in ($userdef->usergroups) and procdef.ProcId = procadmins.ProcId and procadmins.GrpFld = 'G'  and procadmins.ProcId = $ProcId  and Action = 1)";	
	$SQL .= " union ";
	$SQL .=  " (select distinct procdef.ProcId from procdef, geraladmins where geraladmins.GroupId = $userdef->UserId_Process and procdef.ProcId = geraladmins.ProcId and geraladmins.GrpFld='U'  and geraladmins.ProcId = $ProcId  and Action = 1)";
	$SQL .= " union ";
	$SQL .= " (select distinct procdef.ProcId from procdef, geraladmins where geraladmins.GroupId in ($userdef->usergroups) and procdef.ProcId = geraladmins.ProcId and geraladmins.GrpFld = 'G' and geraladmins.ProcId = $ProcId and Action = 1)";	
	$Query = mysqli_query($connect, $SQL);
	while ($Result = mysqli_fetch_array($Query)) 
		{
		if ($Result["ProcId"] == $ProcId)
			{
			$Retorno = true;
			}
		}
	return $Retorno;
	}

function PutInStep($AuthUser, $ProcId, $CaseNum, $StepId)
	{
	if (!is_numeric($ProcId))
	{
		$ProcId = PegaProcIdByCode($ProcId);	
	}
	
	if (!is_numeric($StepId))
	{
		$StepId = PegaStepIdByCode($ProcId, $StepId);	
	}
	FuncPutInStep($AuthUser, $ProcId, $StepId, $CaseNum);	
	$Err = EncerraErros();
	$Case["CaseNum"] = $CaseNum;
	$Case["Error"] = $Err;
    return $Case;	
	}

function FuncPutInStep($AuthUser, $ProcId, $StepId, $CaseNum)
	{
	global $connect, $userdef;
	if (!AuthenticateWs($AuthUser))
		{
		GeraErro(4);
		return 0;		
		}	
	if (!EAdminDoProcesso($ProcId))
		{
			GeraErro(9);
			return 0;
		}
	require_once("ostepdoc.inc");
	$Acao = "PutStepAdmin";
	$Caso = new STEPDOC;
	$Caso->SetConnection($connect);
	$Caso->SetProc($ProcId);
	$Caso->SetStep($StepId);
	$Caso->StartStepId = $StepId;
	$Caso->open();
	$Caso->UserId = $userdef->UserId_Process;
	$Caso->UserName = $userdef->UserName;
	$Caso->UserDesc = $userdef->UserDesc;	
	$Caso->samaccountname = $userdef->UserName;
	$Caso->NovoCaso = false;
	$Caso->SetCaseNum($CaseNum);
	$Processo = $Caso->ProcessoDoCaso();
	if ($Processo == 0)
		{
		GeraErro(10);
		return 0;
		}
	if ($Processo <> $ProcId)
		{
		GeraErro(11);
		return 0;			
		}
	if (!$Caso->CasoEstaNaFila())
		{
		$Caso->LogarAlteracoesCondicao = false;
		$Caso->LogarAlteracoesFormulario = true;
		$Caso->SetAction($Acao);
		}
	}
?>