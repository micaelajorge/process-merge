<?php
function RMCreateObject($connect, $CaseNum, $ProcId, $FieldCodes, $RMOBJECTSOURCECODE, $RMOBJECTTYPECODE, $RMOBJECTCLASSCODE, $RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $Subst = true)
{	
	require_once("lib/nusoap.php");
	include("include/conf_rm.php");
	

	if ($Subst)
		{
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
	$Params["classCode"] = $CLASSCODE;
	$Params["sourceCode"] = $SOURCECODE;
	$Params["typeCode"] = $TYPECODE;
	$Params["indices"] = $Indices;
	$Params["checkItems"] = $Checks;
	$Params["fileContents"] = "";
	$Params["fileExtension"] = "";
		
	$result = @$client->call("ObjectCreateStringArray", array($Params));
	
	if (!is_null($result["faultstring"]))
		{
		$MSG = "-700 Falha ao Criar Objeto - Erro faultstring: " . $result["faultstring"];
		$Dados = "";
		while (list($key, $value) = each($Params))
		{
			$Dados .= "$key $value; ";
		}				
		error_log("MSG $MSG - Dados $Dados", 0);		
		$retorno = -700;
		}
	else 
		{
		if (!$result)
			{
			$Dados = "";
			while (list($key, $value) = each($Params))
			{
				$Dados .= "$key $value; ";
			}			
			$MSG = "-701 Falha ao criar Objeto n�o houve retorno na Chamada";
			error_log("MSG $MSG Dados - $Dados", 0);
			$retorno = -701;
			}
		else
			{			
			$retorno = $result["ObjectCreateStringArrayResult"];
			if ($retorno < 0)
				{
				$MSG = "-702 Falha ao criar Objeto no Content, Retorno: $retorno";
				$Dados = "";
				while (list($key, $value) = each($Params))
				{
					$Dados .= "$key $value; ";
				}			
				error_log("MSG $MSG - Dados $Dados", 0);	
				}
			else 
				{
				$MSG = "Criado Objeto no Content ObjectID: $retorno";
				}				
			}
		}
	$this->EventId = 900;
	$this->Atualizaaudittrail(0, 900, $MSG);			
	return $retorno;	
}	
?>