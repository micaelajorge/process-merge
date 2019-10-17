<?php
function RMModifyObject($connect, $CaseNum, $ProcId, $FieldCodes, $RMOBJECTIDCODE, $RMOBJECTSOURCECODE, $RMOBJECTTYPECODE, $RMOBJECTCLASSCODE, $RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $Subst = true)
{	
	require_once("lib/nusoap.php");
	include("include/conf_rm.php");
	
	// Se for na vers�o antiga do webdesigner, substitui os codigos pelos valores
	if ($Subst)
		{
		$RMOBJECTID = $FieldCodes[$RMOBJECTIDCODE]["Value"];
		if (strpos($RMOBJECTID, "^"))
			{
			$RMOBJECTID = substr($RMOBJECTID, strrpos($RMOBJECTID, "^") + 1);
			}	
		$SOURCECODE = $FieldCodes[$RMOBJECTSOURCECODE]["Value"];	
		$TYPECODE = $FieldCodes[$RMOBJECTTYPECODE]["Value"];	
		$CLASSCODE = $FieldCodes[$RMOBJECTCLASSCODE]["Value"];	
		$RMUSERNAME = $FieldCodes[$RMOBJECTUSERNAMECODE]["Value"];
		$RMUSERPWD = $FieldCodes[$RMOBJECTUSERPWDCODE]["Value"];	
		}
	else 
		{
		$RMOBJECTID = $RMOBJECTIDCODE;
		$SOURCECODE = $RMOBJECTSOURCECODE;
		$TYPECODE = $RMOBJECTTYPECODE;
		$CLASSCODE = $RMOBJECTCLASSCODE;
		$RMUSERNAME = $RMOBJECTUSERNAMECODE;
		$RMUSERPWD = $RMOBJECTUSERPWDCODE;			
		}

	$Parametros = "RMOBJECTID: $RMOBJECTID SOURCECODE: $SOURCECODE TYPECODE: $TYPECODE CLASSCODE: $CLASSCODE ";
		
	if (is_null($SOURCECODE ))
	{
		$SOURCECODE = '';
	}
	if (is_null($TYPECODE))
	{
		$TYPECODE = '';
	}
	if (is_null($CLASSCODE))
	{
		$CLASSCODE = '';
	}
	
	$Indices = '';
	$Checks = '';
	$CountIndices = 0;
	$CountChecks = 0;
	foreach ($FieldCodes as $Field)
		{
		switch ($Field["FieldType"])
			{
			case "TX":
			case "NU":
				{				
				if ($CountIndices > 0)
				{
					$Indices .= "|";
				}
				$Indices .= trim($Field["FieldCode"]);
				$Indices .= '=';
				$Indices .= $Field["Value"];								
				$CountIndices++;
				break;
				}
			case "BO":
				{
				if ($CountChecks > 0)
				{
					$Checks .= "|";
				}
				$Checks .= trim($Field["FieldCode"]);
				$Checks .= "=";
				$Checks .= $Field["Value"];
				$CountChecks++;
				break;
				}
			}	
		}
			
    $client = new soapclient($RMWS, 'wsdl');
	$client->setDefaultRpcParams(true);  
	$soap_proxy = $client->getProxy();	
	$Params["userName"] = $RMUSERNAME;
	$Params["userPWD"] = $RMUSERPWD;
	$Params["objectID"] = $RMOBJECTID;
	$Params["classCode"] = $CLASSCODE;
	$Params["sourceCode"] = $SOURCECODE;
	$Params["typeCode"] = $TYPECODE;
	$Params["indices"] = $Indices;
	$Params["checkItems"] = $Checks;	

	$result = @$client->call("ObjectModify", array($Params));
	
	if (!is_null($result["faultstring"]))
		{
		$Dados = "";
		while (list($key, $value) = each($Params))
			{
			$Dados .= "$key - $value ; ";
			}			
		$retorno = $result["faultstring"];
		$MSG = "-710 Falha ao Modificar Objeto no Content ObjectID:$RMOBJECTID Erro:" . $retorno;
		error_log("MSG $MSG - Dados $Dados", 0);
		$retorno = -710;
		}
	else 
		{
		if (!$result)
			{
			$Dados = "";
			while (list($key, $value) = each($Params))
			{
				$Dados .= "$key - $value ; ";
			}			
			$MSG = "-711 Falha ao Modificar Objeto no Content ObjectID:$RMOBJECTID, n�o houve retorno";
			error_log("MSG $MSG - Dados $Dados", 0);
			$retorno = -711;
			}		
		else 
			{
			if ($result != "OK")
				{
				$Dados = "";
				while (list($key, $value) = each($Params))
				{
					$Dados .= "$key - $value ; ";
				}			
				$MSG = "-712 Falha ao Modificar Objeto no Content ObjectID:$RMOBJECTID, Retorno: $result";
				error_log("MSG $MSG - Dados $Dados", 0);
				$retorno = -712;
				}
			else 
				{
				$MSG = "Modificado Objeto no Content ObjectID:$RMOBJECTID";
				$retorno = 1;		
				}
			}
		}
	$this->EventId = 900;
	$this->Atualizaaudittrail(0, 900, $MSG);			
	return $retorno;
}
	
?>