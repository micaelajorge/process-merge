<?php
// Definicao de Inbox
function GetInBoxFilter($AuthUser, $Filtro, $Conds)
	{
	$Inbox = FuncGetInboxFilter($AuthUser, $Filtro, $Conds);
	return $Inbox;
	}

function FuncGetInboxFilter($AuthUser, $Filtro, $Conds)
	{
	global $connect, $Err;

	// Autentica Usuário
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
		}
	if ($Filtro["StepId"] != "0")
		{
		$Pref = $Pref . $AND . "casequeue.StepId = " . $Filtro["StepId"];
		$AND = " AND ";
		}

//	if ($Filtro["Locked"] <> "")
//		$Pref = $Pref . $AND . "procdef.ProcId = " . $Filtro["Locked"];
//		}
		
	require_once("include/resource01.php");

    $Query = CasosNaFila($ProcId);
	$i = 0;
	while ($Result = mysqli_fetch_array($Query))
		{		
		if (is_array($Conds))
			{
			if (!VerificaCondicoes($Result['CaseId'], $Conds))
				{
				continue;
				}
			}
		$Valor[$i]['ProcId'] = $Result['ProcId'];
		$Valor[$i]['StepId'] = $Result['StepId'];
		$Valor[$i]['CaseNum'] = $Result['CaseId'];
		$Detail = PegaReferencias($Result['ProcId'], $Result['CaseId'], 1);
		
		$DeadItem['DateTime'] = convdate($Result['deadsoftdatetime']);
		$DeadItem['Over'] = date_diff_i($DeadTime, Date("Y-m-d H:i")) > 1;
		$DeadTimesItens[0] = $DeadItem;

		$DeadItem['DateTime'] = convdate($Result['deadharddatetime']);
		$DeadItem['Over'] = date_diff_i($DeadTime, Date("Y-m-d H:i")) > 1;
		$DeadTimesItens[1] = $DeadItem;

		$DeadItem['DateTime'] = convdate($Result['deadhardestdatetime']);
		$DeadItem['Over'] = date_diff_i($DeadTime, Date("Y-m-d H:i")) > 1;
		$DeadTimesItens[2] = $DeadItem;

		$DeadTime['Itens'] = $DeadTimesItens;
		$DeadTime['Count'] = 3;		

		$Valor[$i]['DeadTimes'] = $DeadTime;
		$Valor[$i]['Details']['Count'] = count($Detail);
		$Valor[$i]['Details']['Itens'] = $Detail;

		$Valor[$i]['Details']['Locked'] = (!empty($Result['LockedById'])) || (!empty($Result['LockedBysamaccountname']));
		$Valor[$i]['Details']['LockedById']= $Result['LockedById'];
		$Valor[$i]['Details']['LockedByUser'] = $Result['LockedBysamaccountname'];
		$i++;
		}
	
	// Tratamento de Erro
	$Err[0]['ErrId'] = 0;
	$Err[0]['ErrDesc'] = 'Sem Erro';
	$Error['Count'] = 0;
	$Error['Itens'] = $Err;
		
	$Inbox['Itens'] = $Valor;
	$Inbox['NumCount'] = $i;
	$Inbox['Error'] = $Error;
	
	return $Inbox;
	}
	
function VerificaCondicoes($CaseNum, $Conds)
	{
	global $connect;
	for ($i = 0; $i < count($Conds); $i++)
		{
		$FieldId = $Conds[$i]["FieldId"];
		$Operator = $Conds[$i]["Operator"];
		$Value = $Conds[$i]["Value"];
		$SQL = "select CaseNum from casedata where CaseNum = $CaseNum and FieldId = $FieldId and FieldValue $Operator '$Value' ";
//		echo $SQL .  "<BR>";
		$query = mysqli_query($connect, $SQL);
//		echo mysqli_num_rows($query) . "<br>"; 
		if (mysqli_num_rows($query) == 0)
			{
			return false;
			}
		}
	return true;
	}
?>