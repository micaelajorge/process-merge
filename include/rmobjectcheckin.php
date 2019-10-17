<?php
function RMObjectCheckIn($RMOBJECTID, $CODCAIXA, $RMUSERNAME, $RMUSERPWD)
{	
	require_once(FILES_ROOT . "vendor/lib/nusoap.php");
	include("include/conf_rm.php");
		
	if (strpos($RMOBJECTID, "^"))
		{
		$RMOBJECTID = substr($RMOBJECTID, strrpos($RMOBJECTID, "^") + 1);
		}	
	
    $client = new soapclient($RMFeeder, 'wsdl');
	$client->setDefaultRpcParams(true);  
	$soap_proxy = $client->getProxy();	
	$Params["userName"] = $RMUSERNAME;
	$Params["userPWD"] = $RMUSERPWD;
	$Params["objectID"] = $RMOBJECTID;
	$Params["codCaixa"] = $CODCAIXA;

	$result = @$client->call("ObjectCheckIn", array($Params));
		
	if (!is_null($result["faultstring"]))
	{
		$MSG = "-720 Falha ao Executar ObjectCheckIn ObjectID:$RMOBJECTID Erro:" . $result["faultstring"];
                error_log("MSG $MSG - $Dados", 0);	
		$retorno = -720;
	}
	else {
		if (!$result)
			{
			$MSG = "-721 Falha ao Executar ObjectCheckIn ObjectID:$RMOBJECTID";
			error_log("MSG $MSG - $Dados", 0);	
			$retorno = -721;
			}
		else
			{
			$retorno = $result["ObjectCheckInResult"];	
			$MSG = "Executado ObjectCheckIn ObjectID:$RMOBJECTID Retorno: $retorno";
			}		
	}
	$this->EventId = 900;
	$this->Atualizaaudittrail(0, 900, $MSG);
	return $retorno;
}
	
?>