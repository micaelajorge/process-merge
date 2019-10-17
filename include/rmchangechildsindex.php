<?php
function RMChangeChildsIndex($RMOBJECTID, $INDEXCODE, $INDEXVALUE, $RMUSERNAME, $RMUSERPWD, $DEEP = 0)
{	
	require_once("lib/nusoap.php");
	include("include/conf_rm.php");
		
	if (strpos($RMOBJECTID, "^"))
		{
		$RMOBJECTID = substr($RMOBJECTID, strrpos($RMOBJECTID, "^") + 1);
		}	
	
    $client = new soapclient($RMWS, 'wsdl');
	$client->setDefaultRpcParams(true);  
	$soap_proxy = $client->getProxy();	
	$Params["userName"] = $RMUSERNAME;
	$Params["userPWD"] = $RMUSERPWD;
	$Params["objectID"] = $RMOBJECTID;
	$Params["indexCode"] = $INDEXCODE;
	$Params["indexValue"] = $INDEXVALUE;
	$Params["deepness"] = $DEEP;
	
	$result = @$client->call("ObjectModifyChildIndices", array($Params));
		
	if (!is_null($result["faultstring"]))
		{
		$MSG = "-740 Falha ao Modificar Indice do Objeto - $RMOBJECTID Erro:" . $result["faultstring"];
		error_log("MSG $MSG - Dados $Dados", 0);
		$retorno = -740;
		}
	else 
		{
		if (!$result)
			{
			$MSG = "-741 Falha alterando indice de Objeto no Content, ObjectID: $RMOBJECTID Indice:$INDEXCODE Valor:$INDEXVALUE, n�o houve retorno na chamada";
			error_log("MSG $MSG - Dados $Dados", 0);
			return -741;
			}
		else 
			{				
			$retorno = $result["ObjectModifyChildIndicesResult"];
			if ($retorno != "OK")
				{
				$MSG = "-742 Falha alterando indice de Objeto no Content, ObjectID: $RMOBJECTID Indice:$INDEXCODE Valor:$INDEXVALUE, Retorno:" . $retorno;
				return -742;
				}
			else 
				{
				$MSG = "Alterando indice de Objeto no Content, ObjectID: $RMOBJECTID Indice:$INDEXCODE Valor:$INDEXVALUE, Retorno:" . $retorno;				
				$retorno = 1;
				}
			}
		}
	$this->EventId = 900;
	$this->Atualizaaudittrail(0, 900, $MSG);			
	return $retorno;
}
	
?>