<?php
function GetRumDef($AuthUser, $ProcId)
	{
	$Def = FuncrumGetDef($AuthUser, $ProcId);
	return $Def;
	}
	
function FuncrumGetDef($AuthUser, $ProcId)
	{
	$connect;
	$SQL = 'select ProcRum from ProcId = $ProcId ';
	$Query = mysqli_query($connect, $SQL);
	$Result = mysqli_fetch_array($Query);
	$ProcRum = $Result["ProcRum"];
	AtivaDbRum();
	$SQL = "select ClassRM from RelationProcDoc, procdef where RelationProcDoc.ProcId = $ProcRum and procdef.ProcId = RelationProcDoc.DocId ";
	$Query = mysqli_query($connect, $SQL);
	$Rums = array();
	while ($Result = mysqli_fetch_array($Query))
		{
		$Rum = $Result['ClassRM'];
		array_push($Rums, $Rum);
		}
	$Ret["ProcId"] = $ProcId;
	$Ret["Itens"] = $Rums;
	AtivaDBProcess();
	return $Ret;
	}
?>