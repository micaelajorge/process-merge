<?php
function StepDoCaso($ProcId, $StepId, $CaseNum)
	{
	global $connect;
	$SQL = "select ";
	$SQL .= " StepId ";
	$SQL .= " from ";
	$SQL .= " casequeue ";
	$SQL .= " where ";
	$SQL .= " ProcId = $ProcId ";
	$SQL .= " and CaseId = $CaseNum "; 
	$SQL .= " and StepId = $StepId "; 
	$Query = mysqli_query($connect, $SQL);
	if (mysqli_num_rows($Query) > 0)
		{
		return $StepId;
		}
	return 0;
	}

function Eadminstep($ProcId, $StepId, $UserId) 
	{
	global $connect;
	$NumRows = 0;
	$SQL = "(Select usergroup.UserId from procadmins, usergroup where usergroup.UserId = $UserId and procadmins.GroupId = usergroup.GroupId and ProcId = $ProcId)";
	$SQL .= " union ( Select usergroup.UserId from stepadmins, usergroup where usergroup.UserId = $UserId and stepadmins.GroupId = usergroup.GroupId and ProcId = $ProcId and StepId = $StepId)";
	$SQL .= " union ( Select usergroup.UserId from geraladmins, usergroup where usergroup.UserId = $UserId and geraladmins.GroupId = usergroup.GroupId and ProcId = $ProcId)";
	$Query = mysqli_query($connect, $SQL);
	$NumRows = mysqli_num_rows($Query);
	if ($NumRows > 0) 
		{
		return 1;
		}
	else
		{
		return 0;
		}
	}

function VerificaPermissaoUsuario($ProcId, $StepId, $CaseNum, $userdef, $AdHoc)
	{
	global $connect, $userdef;
	if (Eadminstep($ProcId, $StepId, $userdef->UserId_Process) == 1)
		{
		return 1;
		}
	if ($AdHoc == 0)
		{		
		$SQL = "(";
		$SQL .= " select ";
		$SQL .= " action ";
		$SQL .= " from ";
		$SQL .= " stepaddresses ";
		$SQL .= " where ";
		$SQL .= " ProcId = $ProcId ";
		$SQL .= " and ";
		$SQL .= " StepId = $StepId ";
		$SQL .= " and ";
		$SQL .= " stepaddresses.GroupId in ( $userdef->usergroups )";
		$SQL .= " and ";
		$SQL .= " GrpFld = 'G' ";
		$SQL .= " ) ";
		$SQL .= " union ";
		$SQL .= " (";
		$SQL .= " select ";
		$SQL .= " action ";
		$SQL .= " from ";
		$SQL .= " stepaddresses ";
		$SQL .= " where ";
		$SQL .= " ProcId = $ProcId ";
		$SQL .= " and ";
		$SQL .= " StepId = $StepId ";
		$SQL .= " and ";
		$SQL .= " stepaddresses.GroupId = $userdef->UserId_Process ";
		$SQL .= " and ";
		$SQL .= " GrpFld = 'U' ";
		$SQL .= " ) ";
		$SQL .= " union ";
		$SQL .= " ("; 
		$SQL .= " select "; 
		$SQL .= " Action "; 
		$SQL .= " from  "; 
		$SQL .= " fieldaddresses, stepaddresses, procfieldsdef  "; 
		$SQL .= " WHERE  "; 
		$SQL .= " procfieldsdef.ProcId = $ProcId and stepaddresses.ProcId = $ProcId and procfieldsdef.FieldId = GroupId and fieldaddresses.FieldId = GroupId and StepId = $StepId and fieldaddresses.CaseNum = $CaseNum and FieldValue = $userdef->UserId_Process and FieldType = 'US' "; 
		$SQL .= " ) ";
		$Query = mysqli_query($connect, $SQL);
		if (mysqli_num_rows($Query) == 0)
			{
			return -1;
			}
		else
			{
			$Result = mysqli_fetch_array($Query);
			return $Result["action"];
			}
		}
	else
		{
		$SQL = "select ";
		$SQL .= " GroupId ";
		$SQL .= " from ";
		$SQL .= " stepaddresses ";
		$SQL .= " where ";
		$SQL .= " ProcId = $ProcId ";
		$SQL .= " and ";
		$SQL .= " StepId = $StepId ";
		$SQL .= " and ";
		$SQL .= " GrpFld = 'F' ";
		$Query = mysqli_query($connect, $SQL);
		if (mysqli_num_rows($Query) == 0)
			{
			return -1;
			}
		else
			{
			$Result = mysqli_fetch_array($Query);
			$Campo = $Result["GroupId"];
			$SQL = "select ";
			$SQL.= " UserId";
			$SQL.= " from ";
			$SQL.= " casedata, ";
			$SQL.= " userdef";
			$SQL.= " where ";
			$SQL.= " ProcId = $ProcId ";
			$SQL.= " and ";
			$SQL.= " CaseNum = $CaseNum ";
			$SQL.= " and ";
			$SQL.= " FieldId = $Campo ";
			$SQL.= " and ";
			$SQL.= " FieldValue = UserId ";
			//echo $SQL;
			$Query = mysqli_query($connect, $SQL);
			if (mysqli_num_rows($Query))
				{
				return 0;
				}			
			}		
		}
	}			

function DonoDoCaso($ProcId,$CaseNum)
	{
	global $connect;
	$SQL = "select ";
	$SQL .= " FieldValue ";	
	$SQL .= " from ";	
	$SQL .= " casedata ";
	$SQL .= " where ";
	$SQL .= " FieldId = 1 ";
	$SQL .= " and ";
	$SQL .= " ProcId = $ProcId ";
	$SQL .= " and  ";
	$SQL .= " CaseNum = $CaseNum ";
	$Query = mysqli_query($connect, $SQL);
	$Result = mysqli_fetch_array($Query);
	return $Result["FieldValue"];
	}

function VerificaCaso($ProcId, $StepId, $CaseNum, $userdef, $AdHoc = 0)
	{
	
	if (($userdef->UserLevel == 0) && (DonoDoCaso($ProcId,$CaseNum, $userdef->UserLevel) <> $$userdef->UserId_Process))
		{
		return "NaoDono"; // Usuário não é dono do Caso
		}
	$ActionUser = VerificaPermissaoUsuario($ProcId, $StepId, $CaseNum, $userdef->UserId, $AdHoc);
	if ($ActionUser == "-1")
		{
		$ActionUser = VerificaPermissaoUsuario($ProcId, 0, $CaseNum, $userdef->UserId, $AdHoc);		
		}
	
	if ($ActionUser=="-1")
		{
		return "PerdeuAcesso"; // Usuário não tem mais direitos sobre o caso
		}				
	else
		{
		if ($ActionUser == 1)
			{
			return "Edit";
			}
		if ($ActionUser == 0)
			{
			return "View";
			}
		}
	}
?>