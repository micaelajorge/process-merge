<?php
// Definicao de Inbox
$server->register('GetProcs',          // method name
    array('AuthUser' => 'tns:TBPMUser'),        // input parameters
    array('return' => 'tns:TBPMProcs'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#GetProcs',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Pega Processos do Usu�rio'            // documentation
);

function GetProcs($AuthUser)
	{
	return FuncGetProcs($AuthUser);
	}

function FuncGetProcs($AuthUser)
	{
	global  $connect, $Err;
	require_once("include/resource01.php");
	require_once("include/ldap.php");
	if (!AuthenticateWs($AuthUser))
		{
		$GetProcs['NumCount'] = 0;
		$GetProcs['Itens'] = array();
		$GetProcs['Error'] = $Err;
		return $GetProcs;
		}
	$GetProcs['Itens'] = PegaProcUsuario();
	$GetProcs['NumCount'] = count($GetProcs["Itens"]);
	$Err = EncerraErros();
	$GetProcs['Error'] = $Err;
	return $GetProcs;
	}
	
function GetDef($AuthUser, $ProcId, $StepId)
	{
	$Def = FuncGetDef($AuthUser, $ProcId, $StepId);
	return $Def;
	}

function GetDefbyCod($AuthUser, $ProcCod)
	{
	$Def = FuncGetDefbyCod($AuthUser, $ProcCod);
	return $Def;
	}

function FuncGetDef($AuthUser, $ProcId, $StepId)
	{
	global $connect, $Err;
	// Autentica Usu�rio
	if (!AuthenticateWs($AuthUser))
		{
		$Procs['NumCount'] = 0;
		$Err = EncerraErros();		
		$Inbox['Error'] = $Err;
		return $Inbox;
		}
	$Procs["Itens"] = PegaProcessos($ProcId, $StepId);
	$Procs["NumCount"] = count($Procs["Itens"]);
	$Error = EncerraErros();
	$Procs['Error'] = $Error;	
	return $Procs;
	}

function FuncGetDefByCod($AuthUser, $ProcCod)
	{
	global $connect, $Err;
	// Autentica Usu�rio
	if (!AuthenticateWs($AuthUser))
		{
		$Procs['NumCount'] = 0;
		$Err = EncerraErros();		
		$Inbox['Error'] = $Err;
		return $Inbox;
		}
	$Procs["Itens"] = PegaProcessoByCod($ProcCod);
	$Procs["NumCount"] = count($Procs["Itens"]);
	$Error = EncerraErros();
	$Procs['Error'] = $Error;	
	return $Procs;
	}
	
function PegaProcessos($ProcId = 0, $StepId = 0)
	{
	global $connect;	
	$SQL = "select * from procdef where ProcId > 0";
	if ($ProcId > 0)
		{
		$SQL .= " and ProcId = $ProcId";
		}
	$Query = mysqli_query($connect, $SQL);
	$Procs = array();
	while ($Result = mysqli_fetch_array($Query))
		{
		$Proc = "";
		$Proc["ProcId"] = $Result["ProcId"];
		$Proc["ProcName"] = $Result["ProcName"];
		$Proc["ProcDesc"] = $Result["ProcDesc"];	
		$Proc["ProcCod"] = trim($Result["ProcCod"]);	
		$Proc["ProcFields"] = PegaProcFields($Proc["ProcId"]);		
		$Proc["Steps"] = PegaSteps($Proc["ProcId"], $StepId);
		array_push($Procs, $Proc);
		}
	return $Procs;
	}


function PegaProcessoByCod($ProcCod)
	{
	global $connect;	
	$SQL = "select * from procdef where ProcCod = '$ProcCod'";
	$Query = mysqli_query($connect, $SQL);
	$Procs = array();
	while ($Result = mysqli_fetch_array($Query))
		{
		$Proc = "";
		$Proc["ProcId"] = $Result["ProcId"];
		$Proc["ProcName"] = $Result["ProcName"];
		$Proc["ProcDesc"] = $Result["ProcDesc"];	
		$Proc["ProcCod"] = trim($Result["ProcCod"]);	
		$Proc["ProcFields"] = PegaProcFields($Proc["ProcId"]);		
		$Proc["Steps"] = PegaSteps($Proc["ProcId"], 0);
		array_push($Procs, $Proc);
		}
	return $Procs;
	}


function PegaSteps($ProcId, $StepId)
	{
	global $connect;
	$SQL = "select * from stepdef where ProcId = $ProcId ";
	if ($StepId > 0)
		{
		$SQL .= " and StepId = $StepId ";
		}
	$Query = mysqli_query($connect, $SQL);
	$Steps = array();
	while ($result = mysqli_fetch_array($Query))
		{
		$Step = '';
		$Step["StepId"] = $result["StepId"];
		$Acoes = PegaPermissao($ProcId, $Step["StepId"]);
		$Step["StepName"] = $result["StepName"];
		$Step["StepDesc"] = $result["StepDesc"];
		$Step["StepCod"] = trim($result["StepCod"]);
		$Step["Edit"] = $Acoes["Action"];
		$Step["NewDoc"] = $Acoes["NewDoc"];
		$Step["StepFields"] = PegaStepFields($ProcId, $Step["StepId"]);
		array_push($Steps, $Step);
		}
	$Itens["Itens"] = $Steps;
	$Itens["NumCount"] = count($Steps);
	return $Itens;
	}

function PegaStepFields($ProcId, $StepId)
	{
	global $connect;
	$SQL = "select procfieldsdef.FieldId, FieldName, FieldCod, FieldDesc, FieldType, ReadOnly, Optional from procfieldsdef, stepfieldsdef where procfieldsdef.ProcId = $ProcId and stepfieldsdef.ProcId = $ProcId and StepId = $StepId and procfieldsdef.FieldId = stepfieldsdef.FieldId";
	$Query = mysqli_query($connect, $SQL);
	$Fields = array();
	while ($result = mysqli_fetch_array($Query))
		{
		$Field = '';
		$Field["FieldId"] = $result["FieldId"];	
		$Field["FieldName"] = $result["FieldName"];
		$Field["FieldDesc"] = $result["FieldDesc"];
		$Field["FieldCod"] = trim($result["FieldCod"]);
		$Field["FieldType"] = $result["FieldType"];
		$Field["ReadOnly"] = $result["ReadOnly"];
		$Field["Optional"] = $result["Optional"];
		array_push($Fields, $Field);
		}
	$Itens["Itens"] = $Fields;
	$Itens["NumCount"] = count($Fields);
	return $Itens;
	}

function PegaPermissao($ProcId, $StepId)
	{
	global $connect, $userdef;
	$SQL  = "( ";
	$SQL .= " select Action, NewDoc from stepaddresses where ProcId = $ProcId and StepId = $StepId and GroupId = $userdef->UserId_Process";
	$SQL .= " ) ";
	$SQL .= " union ";
	$SQL .= " ( ";
	$SQL .= " select Action, NewDoc from stepaddresses where ProcId = $ProcId and StepId = $StepId and GroupId in ($userdef->usergroups) ";
	$SQL .= " ) ";
	$Query = mysqli_query($connect, $SQL);
	$Acoes["Action"] = 0;
	$Acoes["NewDoc"] = 0;
	while ($Result = mysqli_fetch_array($Query))
		{
		if ($Result["Action"] == 1)
			{
			$Acoes["Action"] = 1;
			}
		if ($Result["NewDoc"] == 1)
			{
			$Acoes["NewDoc"] = 1;
			}
		}
	return $Acoes;
	}
?>