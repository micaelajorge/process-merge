<?php
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
?>