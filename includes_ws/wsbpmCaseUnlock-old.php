<?php
$server->register('CaseUnlock',          // method name
    array('AuthUser' => 'tns:TBPMUser', 'CaseNum' => 'xsd:int', 'StepId' => 'xsd:int'),        // input parameters
    array('return' => 'tns:TBPMError'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#CaseUnlock',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Unlock one Case'            // documentation
);

function FuncCaseUnlock($AuthUser, $CaseNum, $StepId)
	{
	global $connect, $DadosAD, $UserId;
	// Autentica Usuário
	if (!AuthenticateWs($AuthUser))
		{
		return;
		}

	$SQL = "update casequeue set LockedById = 0, LockedBysamaccountname = null, AdHoc = 0 where LockedById = $UserId and LockedBysamaccountname = '" . $DadosAD["samaccountname"] . " ' and AdHoc = 3 ";
	if (!(@mysqli_query($connect, $SQL)))
		{
		GeraErro(5);
		return;		
		}
	}
	
function CaseUnlock($AuthUser, $CaseNum, $StepId)
	{
	FuncCaseUnlock($CaseNum, $StepId);
	$Err = EncerraErros();
	return $Err;
	}
?>