<?php
$server->register('CaseAcquire',          // method name
    array('AuthUser' => 'tns:TBPMUser', 'ProcId' => 'xsd:int', 'StepId' => 'xsd:int', 'CaseNum' => 'xsd:int'),        // input parameters
    array('return' => 'tns:TBPMError'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#CaseAcquire',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Unlock one Case'            // documentation
);

function FuncCaseAcquire($AuthUser, $ProcId, $StepId, $CaseNum)
	{
	global $connect, $DadosAD, $UserId;
	if (!AuthenticateWs($AuthUser))
		{
		return;
		}	

	$SQL = "update casequeue set LockedById = $UserId, LockedBysamaccountname = '" . $DadosAD["samaccountname"] . " ', AdHoc = 3 where CaseId = $CaseNum and StepId = $StepId and LockedbyId = 0 and AdHoc = 0";	
	if (!@mysqli_query($connect, $SQL))
		{
		GeraErro(5);
		return;
		}
			
	$SQL = "select LockedById, LockedBySamAccountName from casequeue where CaseId = $CaseNum and StepId = $StepId and AdHoc = 3 and LockedById = $UserId and LockedBySamaccountname = '" . $DadosAD["samaccountname"] . "'";
	if (!($Query = @mysqli_query($connect, $SQL)))
		{
		GeraErro(5);
		return;
		}
	if (mysqli_num_rows($Query) == 0) 
		{		
		GeraErro(2);
		return;
		}
	}

// Define the method as a PHP function
function CaseAcquire($AuthUser, $ProcId, $StepId, $CaseNum)
	{
	FuncCaseAcquire($AuthUser, $CaseNum, $StepId);		
	$Err = EncerraErros();
    return $Err;
	}
?>