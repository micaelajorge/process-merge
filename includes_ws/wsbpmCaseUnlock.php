<?php
$server->register('CaseUnlock',          // method name
    array('AuthUser' => 'tns:TBPMUser', 'ProcId' => 'xsd:int', 'StepId' => 'xsd:int', 'CaseNum' => 'xsd:int'),        // input parameters
    array('return' => 'tns:TBPMError'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#CaseUnlock',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Unlock one Case'            // documentation
);

function FuncCaseUnlock($AuthUser, $ProcId, $StepId, $CaseNum)
	{
	global $connect, $DadosAD, $UserId;
	if (!AuthenticateWs($AuthUser))
		{
		return;
		}	
	$SQL = "update casequeue set LockedById = 0, LockedBysamaccountname = null, AdHoc = 3 where ProcId = $ProcId and CaseId = $CaseNum and StepId = $StepId and AdHoc = 3 and LockedById = $UserId and LockedBySamaccountname = '" . $DadosAD["samaccountname"] . "'";	
	if (!@mysqli_query($connect, $SQL))
		{
		GeraErro(5);
		return;
		}
	}

// Define the method as a PHP function
function CaseUnlock($AuthUser, $ProcId, $StepId, $CaseNum)
	{
	FuncCaseAcquire($AuthUser, $ProcId, $StepId, $CaseNum);		
	$Err = EncerraErros();
    return $Err;
	}
?>