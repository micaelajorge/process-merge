<?php
// Definicao de Inbox
function GetInBox($AuthUser, $Filtro)
	{
	$Inbox = FuncGetInbox($AuthUser, $Filtro);
	return $Inbox;
	}

function FuncGetInbox($AuthUser, $Filtro)
	{
	global  $connect, $Err, $S_procdef;
	require_once("include/resource01.php");
	// Autentica Usuário
	//error_log(var_export($AuthUser, true));
	if (!AuthenticateWs($AuthUser))
		{
		$Inbox['NumCount'] = 0;
		$Err = EncerraErros();		
		$Inbox['Error'] = $Err;
		return $Inbox;
		}
		
	if ($Filtro["ProcId"] != "0")
		{
		$ProcId = $Filtro["ProcId"];
		if (!is_numeric($ProcId))
			{
			$StepId = PegaProcIdByCode($ProcId);
			}		
		$Pref = "procdef.ProcId = $ProcId ";
		$AND = " AND ";
		}
		
	if ($Filtro["StepId"] != "")
		{
		$StepId = $Filtro["StepId"];
		if (!is_numeric($StepId))
			{
			$StepId = PegaStepIdByCode($ProcId, $Filtro["StepId"]);
			}		
		$Pref = $Pref . $AND . "casequeue.StepId = $StepId ";
		$AND = " AND ";
		}
			
    $Query = CasosNaFila($ProcId, $StepId, 0);
	PegaCamposReferenciaProcesso($ProcId);

	$i = 0;
	foreach ($S_procdef["Referencias"] as $Ref)
		{
		$Refs[$i]["FieldCode"] = $Ref["FieldCod"];
		$Refs[$i]["FieldName"] = $Ref["FieldName"];
		$i++;
		}
	
	$Referencias['Count'] = count($Refs);
	$Referencias['Itens'] = $Refs;
	$Inbox["Refs"] = $Referencias;

	$i = 0;
	while (($Result = mysqli_fetch_array($Query)) && $i < 25)
		{		
		$Inbox['InboxItens'][$i]['Locked'] = 0;
		$Result['LockedBysamaccountname'] = trim($Result['LockedBysamaccountname']);
		if (($Result['lockedbyid'] != 0) || !empty($Result['LockedBysamaccountname']))
			{
			$Inbox['InboxItens'][$i]['Locked'] = 1;
			}
		$Details = PegaReferencias($Result['ProcId'], $Result['CaseId'], 1);
		$Inbox['InboxItens'][$i]['Details']['DetItens'] = array();
		$j = 0;
		foreach ($Details as $Deta)
			{		
				$Det[$j]["FieldCode"] = $Deta["FieldCod"];
				$Det[$j]["FieldValue"] = $Deta["FieldValue"];
				if ($Deta["Empty"])
					$Det[$j]["FieldValue"] = '';				
				$j++;
			}
		$Inbox['InboxItens'][$i]['StepId'] = $Result['StepId'];
		$Inbox['InboxItens'][$i]['Details'] = $Det;
		$Inbox['InboxItens'][$i]['NumDetails'] = count($Det);
		$Inbox['InboxItens'][$i]['CaseNum'] = $Result['CaseId'];
		$Inbox['InboxItens'][$i]['AdHoc'] = 0;
		$Inbox['InboxItens'][$i]['Action'] = 0;
		$i++;
		}

	$Inbox['Details']['Count'] = count($Det);	
	$Inbox['Details']['Itens'] = $Det;
		
	// Tratamento de Erro
	$Err[0]['ErrId'] = 0;
	$Err[0]['ErrDesc'] = 'Sem Erro';
	$Error['Count'] = 0;
	$Error['Itens'] = $Err;
		
	$Inbox['NumCount'] = count($Inbox['InboxItens']);
	$Inbox['Error'] = $Error;
	
	error_log(var_export($Inbox, true));
	return $Inbox;
	}
?>