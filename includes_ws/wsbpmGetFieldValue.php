<?php
function ProcIdOfCaseNum($CaseNum)
	{
		global $connect;
		$SQL = "select ProcId from casemap where CaseId = $CaseNum";
		$Query = mysqli_query($connect, $SQL);
		$Result = mysqli_fetch_array($Query);
		if (is_array($Result))
			{
			return $Result["ProcId"];			
			}
		return 0;	
	}
	
function GetFieldValue($AuthUser, $CaseNum, $FieldId)
	{
	// Autentica Usuário
	if (!AuthenticateWs($AuthUser))
		{
		$Error = EncerraErros();
		$FieldData['Error'] = $Error;
		return $FieldData;		
		}
		
	$ProcId = ProcIdOfCaseNum($CaseNum);
	if ($ProcId == 0)
		{
		GeraErro(10);
		$Error = EncerraErros();	
		$FieldData['Error'] = $Error;
		return $FieldData;
		}
		
	if (!is_numeric($CaseNum))
		{
		GeraErro(10);
		$Error = EncerraErros();	
		$FieldData['Error'] = $Error;
		return $FieldData;			
		}

	if (!is_numeric($FieldId))
		{
		$FieldId = PegaFieldIdByCode($ProcId, $FieldId);
		}

	if ($FieldId == 0)
		{
		GeraErro(12);
		$Error = EncerraErros();
		$FieldData['Error'] = $Error;
		return $FieldData;
		}	
		
	$FieldValue = PegaValorCampo($ProcId, $CaseNum, $FieldId, 'TX', 0, 0);	
	$Error = EncerraErros();
	$FieldData["FieldValue"] = $FieldValue;
	$FieldData['Error'] = $Error;
	return $FieldData;
	}
?>