<?php
include("tbpmcasedata.php");
$server->register('GetData',          // method name
    array('AuthUser' => 'tns:TBPMUser', 'ProcId' => 'xsd:int', 'StepId' => 'xsd:int', 'CaseNum' => 'xsd:int'),        // input parameters
    array('return' => 'tns:TBPMcasedata'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#GetData',              // soapaction
    'rpc',                            // style
    'encoded',                        // use
    'Acquire one Case'                // documentation
);

function GetDataCase($ProcId, $StepId, $CaseNum)
	{
	global $connect;
	$SQL = " select casedata.FieldId, FieldValue ";
	$SQL .= " from ";
	$SQL .= " stepfieldsdef, casedata ";
	$SQL .= " where ";
	$SQL .= " CaseNum = $CaseNum and ";
	$SQL .= " stepfieldsdef.ProcId = casedata.ProcId and ";
	$SQL .= " casedata.FieldId = stepfieldsdef.FieldId and ";
	$SQL .= " stepfieldsdef.StepId = $StepId ";
	$Query = mysqli_query($connect, $SQL);
	$i = 0;
	while ($Result = mysqli_fetch_array($Query))
		{
		$Valor[$i]["FieldId"] = $Result["FieldId"];
		$Valor[$i]["Value"] = $Result["FieldValue"];
		$i++;
		}
	return $Valor;
	}
	
function VerificaStep($CaseNum, $StepId)
	{
	global $connect;
	$SQL = "select AdHoc, LockedbyId, LockedBysamaccountname from casequeue where CAseId = $CaseNum and AdHoc <> 1 and StepId = $StepId";
	$Query = @mysqli_query($connect, $SQL);
	if (@mysqli_num_rows($Query) == 0 )
		{
		GeraErro(1, " = " . $StepId);
		return false;
		}
	$Result = @mysqli_fetch_array($Query);
	if ($Result["AdHoc"] == 2)
		{
		GeraErro(3);
		return false;
		}
	return true;
	}
	
function GetData($AuthUser, $ProcId, $StepId, $CaseNum)
	{
	$casedata['Fields'][0]["FieldId"] = 0;
	$casedata['Fields'][0]["Value"] = "";
	// Autentica Usuário
	if (!AuthenticateWs($AuthUser))
		{
		$Error = EncerraErros();
		$casedata['Error'] = $Error;
		return $casedata;		
		}
		
	if (!VerificaStep($CaseNum, $StepId))
		{
		$Error = EncerraErros();
		$casedata['Error'] = $Error;
		return $casedata;		
		}
	else
		{
		$casedata['Fields'] = GetDataCase($ProcId, $StepId, $CaseNum);		
		}
	$casedata['NumFields'] = count($casedata['Fields']); // Garante q o nr será o atribuido
    // Tratamento de Erro	
	$Error = EncerraErros();
	$casedata['Error'] = $Error;
	return $casedata;
	}
?>