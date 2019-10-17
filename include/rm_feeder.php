<?php
function RMObjectCheckOutReceived($ObjectId, $Data)
{
	require_once(FILES_ROOT . "vendor/lib/nusoap.php");
	include("include/conf_rm.php");
	$client = new soapclient($RM_FEEDER, 'wsdl');
	$client->setDefaultRpcParams(true);  
	$Param["userName"] = $RM_FEEDER_USER;
	$Param["userPWD"] = $RM_FEEDER_PWD;
	$Param["objectID"] = $ObjectId;
	$Param["dataRetorno"] = $Data;

	$soap_proxy = $client->getProxy();	
	$result = $soap_proxy->ObjectCheckOutReceived($Param);
	if ($err = $soap_proxy->getError())
	{
		return "Falha no CheckOut RM";
	}
	if ($result["ObjectCheckOutReceivedResult"] == 'OK')
	{
		return "CheckOut RM Ok";
	}
	else{
		return $result["ObjectCheckOutReceivedResult"];	
	}
}

function RMObjectCheckIN($ObjectId, $codCaixa)
{
	require_once(FILES_ROOT . "vendor/lib/nusoap.php");
	include("include/conf_rm.php");
	$client = new soapclient($RM_FEEDER, 'wsdl');
	$client->setDefaultRpcParams(true);  
	$Param["userName"] = $RM_FEEDER_USER;
	$Param["userPWD"] = $RM_FEEDER_PWD;
	$Param["objectID"] = $ObjectId;
	$Param["codCaixa"] = $codCaixa;
	$soap_proxy = $client->getProxy();
	$result = $soap_proxy->ObjectCheckIn($Param);
	if ($err = $soap_proxy->getError())
	{
		return "Falha no CheckIdReceived RM";
	}
	if ($result["ObjectCheckInResult"] == 'OK')
	{
		return "CheckIn RM Ok";
	}
	else{
		return $result["ObjectCheckInResult"];	
	}
}
?>