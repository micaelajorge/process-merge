<?php
function RMObjectCheckOutReceived($RMOBJECTID, $DATARETORNO, $RMUSERNAME, $RMUSERPWD)
{	
	require_once("lib/nusoap.php");
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
	$Params["dataRetorno"] = $DATARETORNO;

	$result = @$client->call("ObjectCheckOutReceived", array($Params));
		
	if (!is_null($result["faultstring"]))
		{
		$MSG = "-730 Falha ao Executar ObjectCheckOutReceived ObjectID:$RMOBJECTID Erro:" . $result["faultstring"];
		error_log(" Erro: " .$MSG, 0);
		$retorno = -730;
		}
	else 
		{
		if (!$result)
			{
			$MSG = "-731 Falha ao Executar ObjectCheckOutReceived ObjectID:$RMOBJECTID - N�o houve retorno da chamada";
			error_log("MSG $MSG - Dados $Dados", 0);
			$retorno = -731;
			}
		else 
			{
			$retorno = $result["ObjectCheckOutReceivedResult"];	
			$MSG = "Executado ObjectCheckOutReceived ObjectID:$RMOBJECTID - Retorno:$retorno";			
			}		
		}
	$this->EventId = 900;
	$this->Atualizaaudittrail(0, 900, $MSG);		
	return $retorno;		
}
	
?>