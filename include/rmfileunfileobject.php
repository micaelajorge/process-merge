<?php
function RMFileUnfileObject($connect, $CaseNum, $ProcId, $FieldCodes, $RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $RMOBJECTPARENTCODE, $RMOBJETCHILDCODE, $FILEUNFILE, $Subst = true)
{	
	require_once(FILES_ROOT . "vendor/lib/nusoap.php");
	include("include/conf_rm.php");

	if ($Subst)
		{
		$PARENTID = $FieldCodes[$RMOBJECTPARENTCODE]["Value"];
		if (strpos($PARENTID, "^"))
			{
			$PARENTID = substr($PARENTID, strrpos($PARENTID, "^") + 1);
			}			
		$CHIELDID = $FieldCodes[$RMOBJETCHILDCODE]["Value"];		
		if (strpos($CHIELDID, "^"))
			{
			$CHIELDID = substr($CHIELDID, strrpos($CHIELDID, "^") + 1);
			}			
		$RMUSERNAME = $FieldCodes[$RMOBJECTUSERNAMECODE]["Value"];
		$RMUSERPWD = $FieldCodes[$RMOBJECTUSERPWDCODE]["Value"];	
		}
	else 
		{
		$PARENTID = $RMOBJECTPARENTCODE;
		$CHIELDID = $RMOBJETCHILDCODE;
		$RMUSERNAME = $RMOBJECTUSERNAMECODE;
		$RMUSERPWD = $RMOBJECTUSERPWDCODE;							
		}
	
    $client = new soapclient($RMWS, 'wsdl');
	$client->setDefaultRpcParams(true);  
	$soap_proxy = $client->getProxy();	
	$Params["userName"] = $RMUSERNAME;
	$Params["userPWD"] = $RMUSERPWD;
	$Params["parentID"] = $PARENTID;
	$Params["childID"] = $CHIELDID;
	$Params["divisionCode"] = "";
	$Params["isFile"] = $FILEUNFILE;
		
	$OBJETOS = "Parent - $PARENTID; Child - $CHIELDID";
	
	$result = @$client->call("ObjectFileUnfile", array($Params));
	
	//error_log("Movendo Objetos $CHIELDID -> $PARENTID", 0);
	if (!is_null($result["faultstring"]))
		{
		$MSG = "-750 Falha ao inserir/remover objeto no Content ObjectID:$CHIELDID em ObjectID:$PARENTID - Erro: " . $result["faultstring"];
		error_log("Erro " . $MSG, 0);
		$retorno = -750;
		}
	else 
		{
		if (!$result)
			{
			$MSG = "-751 Falha ao inserir/remover objeto no Content ObjectID:$CHIELDID em ObjectID:$PARENTID";
			error_log("Erro " . $MSG, 0);
			$retorno = -751;
			}
		else 
			{
			$MSG = "Inserido objeto ObjectID:$CHIELDID em ObjectID:$PARENTID";	
			$retorno = 1;
			}
	}
	$this->EventId = 900;
	$this->Atualizaaudittrail(0, 900, $MSG);		
	return $retorno;	
}	
?>