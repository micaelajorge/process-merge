<?php
function GetDeadTimeExpireList($AuthUser, $CountQueue = 10)
	{
	$Retorno = FuncDeadTimesExpire($AuthUser, $CountQueue);
	return  $Retorno;
	}
		
function DeadTimeNotify($AuthUser, $ProcId, $StepId, $CaseNum, $DeadType, $Origem)
	{
	$Retorno = FuncDeadTimeNotify($AuthUser, $ProcId, $StepId, $CaseNum, $DeadType, $Origem);
	return  $Retorno;		
	}

function DeadTimeRelease($AuthUser, $ProcId, $StepId, $CaseNum)
	{
	$Retorno = FuncDeadTimeRelease($AuthUser, $ProcId, $StepId, $CaseNum);
	return  $Retorno;		
	}
	
function FuncDeadTimesExpire($AuthUser, $NumItems = 10)
	{
	// Tratamento de Erro
	$Err[0]['ErrId'] = 0;
	$Err[0]['ErrDesc'] = 'Sem Erro';
	$Error['Count'] = 0;
	$Error['Itens'] = $Err;
	
	$Items = array();
	
	$Query = PegaCasosDeadTime($CountQueue);

	if (!$Query)
		{
		$Retorno['NumCount'] = 0;
		$Retorno['Items'] = $Items;
		$Retorno['Error'] = $Error;
		return $Retorno;	
		}
	
	$i = 0;
	while ($Result = mysqli_fetch_array($Query))
	{
		if ($i >= $NumItems)
			{
			break;
			}
		$Items[$i]['ProcId'] = $Result["ProcId"];
		$Items[$i]['StepId'] = $Result["StepId"];
		$Items[$i]['CaseNum'] = $Result["CaseId"];
		$Items[$i]['DeadType'] = $Result["DeadType"];
		$Items[$i]['Origem'] = $Result["Origem"];
		if (empty($Result["SendEmail"]))
			{
			$Items[$i]['SendEmail'] = 0;
			}
		else 	
			{
			$Items[$i]['SendNotify'] = $Result["SendEmail"];
			}
		
		if (empty($Result["ReleaseStep"]))
			{
			$Items[$i]['ReleaseStep'] = 0;
			}
		else 
			{
			$Items[$i]['ReleaseStep'] = $Result["ReleaseStep"];				
			}
		
		if (empty($Result["StartStepId"]))
			{
			$Items[$i]['StartStepId'] = 0;
			}
		else 
			{
			$Items[$i]['StartStepId'] = $Result["StartStepId"];
			}		
		$i++;		
	}	
	$Retorno['NumCount'] = count($Items);
	$Retorno['Items'] = $Items;
	$Retorno['Error'] = $Error;
	return $Retorno;	
	}
	
	
function PegaCasosDeadTime($CountQueue)
{
	global $connect;
	$SQL = "(
select Origem, casequeue.ProcId, casequeue.StepId, casequeue.CaseId, 'S' as DeadType, ReleaseSoft as Release,DeadSoftDateTime as DeadTime, DeadSoftRemove as RemoveStep, SendEmail, StartStepId from casequeue left join actionstartstepdef on casequeue.ProcId = actionstartstepdef.ProcId and casequeue.StepId= actionstartstepdef.StepId and TrueFalse = 'S', stepdef left join notifications on notifications.ProcId = stepdef.ProcId and notifications.StepId = stepdef.StepId and NotificationType = 'DS', exportkeys where DeadSoftWait = 1 and DeadSoftDateTime < getDate() and stepdef.ProcID = casequeue.ProcId and casequeue.StepID = stepdef.StepId and exportkeys.ProcId = stepdef.ProcId and exportkeys.CaseNum = casequeue.CaseId
)
union
(
select Origem, casequeue.ProcId, casequeue.StepId, casequeue.CaseId, 'H' as DeadType, ReleaseHard as Release,DeadSoftDateTime as DeadTime, DeadSoftRemove as RemoveStep, SendEmail, StartStepId from casequeue left join actionstartstepdef on casequeue.ProcId = actionstartstepdef.ProcId and casequeue.StepId= actionstartstepdef.StepId and TrueFalse = 'H', stepdef left join notifications on notifications.ProcId = stepdef.ProcId and notifications.StepId = stepdef.StepId and NotificationType = 'DH', exportkeys where DeadHardWait = 1 and DeadHardDateTime < getDate() and stepdef.ProcID = casequeue.ProcId and casequeue.StepID = stepdef.StepId and exportkeys.ProcId = stepdef.ProcId and exportkeys.CaseNum = casequeue.CaseId
)
union
(
select Origem, casequeue.ProcId, casequeue.StepId, casequeue.CaseId, 'D' as DeadType, ReleaseHardest as Release,DeadSoftDateTime as DeadTime, DeadSoftRemove as RemoveStep, SendEmail, StartStepId from casequeue left join actionstartstepdef on casequeue.ProcId = actionstartstepdef.ProcId and casequeue.StepId= actionstartstepdef.StepId and TrueFalse = 'D', stepdef left join notifications on notifications.ProcId = stepdef.ProcId and notifications.StepId = stepdef.StepId and NotificationType = 'DD', exportkeys where DeadHardestWait = 1 and DeadHardestDateTime < getDate() and stepdef.ProcID = casequeue.ProcId and casequeue.StepID = stepdef.StepId and exportkeys.ProcId = stepdef.ProcId and exportkeys.CaseNum = casequeue.CaseId
)
order by DeadTime";
	$Query = mysqli_query($connect, $SQL);
	return $Query;
}


function GetDataNotify($ProcId, $StepId, $CaseNum, $DeadType)
	{
	global $connect;
	switch ($DeadTypeExtend)
		{
			case 'S':
				$DeadTypeExtend = 'DD';
				break;
			case 'H':
				$DeadTypeExtend = 'DH';
				break;
			case 'D':
				$DeadTypeExtend = 'DD';
			
		}
	$SQL = "select htmlfile, Subject from notifications where ProcId = 2 and StepId = $StepId and NotificationType = '$DeadTypeExtend'";
	$Query = mysqli_query($connect, $SQL);
	if (mysqli_num_rows($Query) > 0)
		{
		$Tupla = mysqli_fetch_array($Query);	
		$htmlfile = $Tupla["htmlfile"];
		$subject = $Tupla["Subject"];
		}

	$retorno['file'] = '';
	$retorno['subject'] = '';		
	if (!file_exists("./notifications/$page"))
		{
		return $retorno;
		}	

	include_once("include/resource01.php");
	$fh = fopen("./notifications/$htmlfile", 'r');
	$contents = fread($fh, filesize("./notifications/$htmlfile"));
	fclose($fh);
	$ParsedFile = ParsevaloresCampos($ProcId, $CaseNum, $contents);	
	$ParsedSubject = ParsevaloresCampos($ProcId, $CaseNum, $subject);

	$retorno['File'] = $ParsedFile;
	$retorno['Subject'] = $ParsedSubject;		
	}

function PegaEmailUsuariosGrupo($GroupId, $Origem)
	{
	global $OrigemLogon;
	}
	
function AdicionaGrupo($addressesStep, $GroupId)	
	{
	return $addressesStep;
	}
	
function PegaDadosEmailUser($UserId)
	{
	return $DadosUsuario;	
	}
	
function AdicionaUsuario($addressesStep, $DadosUsuario)	
	{
	if (!in_array($Usuario, $addressesStep["Users"]))
		{
		array_push($addressesStep["DadosUser"], $DadosUsuario);	
		array_push($addressesStep["Users"], $DadosUsuario["UserId"]);
		}
	return $addressesStep;
	}
	
function AdicionaAddress($addressesStep, $GrpFld, $Id)
	{
	switch ($GrpFld)
		{
			case 'U':
				$DadosUsuario = PegaDadosEmailUser($UserId);
				$addressesStep = AdicionaUsuario($addressesStep, $DadosUsuario);
				break;
			case 'G':
				$addressesStep = AdicionaGrupo($addressesStep, $Id);
				break;
			case 'F':
				$FieldType = PegaTipoCampo($ProcId, $linha["GroupId"]); 				
				$Valor = PegaValorCampo($ProcId, $CaseNum, $Id);
				if ($FieldType = 'GR') 
					{	
					$GrpFld = 'G';					
					$addressesStep = AdicionaAddress($addressesStep, $GrpFld, $Valor);
					}
				else 
					{
					$DadosUsuario = PegaDadosEmailUser($UserId);
					$addressesStep = AdicionaUsuario($addressesStep, $DadosUsuario);
					}				
				break;
		}		
		return $addressesStep;
	}
	
function GetaddressesStep($ProcId, $StepId, $CaseNum, $DeadType, $Origem)
	{
	global $connect;
	$addressesStep["Users"] = array();
	$addressesStep["DadosUser"] = array();
	
	$SQL = "select GroupId, GrpFld from stepaddresses where ProcId = $ProcId and StepId = $StepId";
	$Query = mysqli_query($connect, $SQL);
	
	while ($linha = mysqli_fetch_array($Query))
		{
		$addressesStep = AdicionaAddress($addressesStep, $linha["GrpFld"], $linha["GroupId"], $Origem);
		}		
	}
	
function FuncDeadTimeNotify($AuthUser, $ProcId, $StepId, $CaseNum, $DeadType, $Origem)
	{
	global $connect;
	$retorno = GetDataNotify($ProcId, $StepId, $CaseNum, $DeadType);
	$retorno["addresses"] = GetaddressesDeadTime($ProcId, $StepId, $CaseNum, $DeadType, $Origem);
	return $retorno;			
	}
?>