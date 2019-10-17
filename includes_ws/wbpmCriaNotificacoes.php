<?php
function CriaNotificaoes($AuthUser)
{
	NotificacoesIniciais($AuthUser);
	NotificacoesFinais($AuthUser);
}

function NotificacoesIniciais($AuthUser)
{
	global $connect;
	$SQL = "SELECT CaseNum, Origem, Campo15 FROM exportkeys where Campo37 = 1 and Campo38 = 0 and ProcId = 1";	
	$Query = mysqli_query($connect, $SQL);
	while ($Result = mysqli_fetch_array($Query))
	{
		$ProcId = 5;
		$StepId = 1;
		$CaseNum = $Result["CaseNum"];
		$Origem = $Result["Origem"];
		$SQL = "select CaseNum from exportkeys where Campo7 = 0 and Origem = '$Origem' and ProcId = $ProcId and Campo6 = 1";
		$QueryNotifi = mysqli_query($connect, $SQL);
		$Res = mysqli_fetch_array($QueryNotifi);
		$Notificacao = $Res["CaseNum"];	
		if (empty($Notificacao))
		{
			$DataAtivacao = $Result["Campo15"];
			$Fields[0]["Value"] = $Origem;
			$Fields[0]["FieldId"] = 4;
			$Fields[1]["Value"] = 0;
			$Fields[1]["FieldId"] = 7;
			$Fields[2]["Value"] = 1;
			$Fields[2]["FieldId"] = 6;
			$dadosDoCaso["Fields"] = $Fields;
			$dadosDoCaso["Error"] = "";
			$dadosDoCaso["NumFields"] = 3;		
			$Notificacao = FuncCaseStart($AuthUser, $ProcId, $StepId, $dadosDoCaso, 0);
		}			
		$Fields = '';
		$Fields[0]["FieldId"] = 38;
		$Fields[0]["Value"] = '1';
		$Fields[1]["FieldId"] = 41;
		$Fields[1]["Value"] = $Notificacao;
		$dadosDoCaso["Fields"] = $Fields;
		$dadosDoCaso["Error"] = "";
		$dadosDoCaso["NumFields"] = '2';		
		FuncCaseRelease($AuthUser, 1, 15, $CaseNum, $dadosDoCaso, 0);
	}	
}

function NotificacoesFinais($AuthUser)
{
	global $connect;
	$SQL = "SELECT CaseNum, Origem, Campo15 FROM exportkeys where Campo39 = 1 and Campo40 = 0 and ProcId = 1";	
	$Query = mysqli_query($connect, $SQL);
	while ($Result = mysqli_fetch_array($Query))
	{
		$ProcId = 5;
		$StepId = 1;
		$SQL = "select CaseNum from exportkeys where Campo7 = 0 and Origem = $Origem and ProcId = $ProcId and Campo6 = 2";
		$QueryNotifi = mysqli_query($connect, $SQL);
		$Res = mysqli_fetch_array($QueryNotifi);
		$Notificacao = $Res["CaseNum"];				
		if (empty($Notificacao))
		{
			$CaseNum = $Result["CaseNum"];
			$Origem = $Result["Origem"];
			$DataAtivacao = $Result["Campo15"];
			$Fields[0]["Value"] = $Origem;
			$Fields[0]["FieldId"] = 4;
			$Fields[1]["Value"] = 0;
			$Fields[1]["FieldId"] = 7;
			$Fields[2]["Value"] = 2;
			$Fields[2]["FieldId"] = 6;
			$dadosDoCaso["Fields"] = $Fields;
			$dadosDoCaso["Error"] = "";
			$dadosDoCaso["NumFields"] = 3;			
			$Notificacao = FuncCaseStart($AuthUser, $ProcId, $StepId, $dadosDoCaso, 0);
		}
		$Fields = '';
		$Fields[0]["FieldId"] = 40;
		$Fields[0]["Value"] = '1';
		$Fields[1]["FieldId"] = 42;
		$Fields[1]["Value"] = $Notificacao;
		$dadosDoCaso["Fields"] = $Fields;
		$dadosDoCaso["Error"] = "";
		$dadosDoCaso["NumFields"] = '2';		
		FuncCaseRelease($AuthUser, 1, 15, $CaseNum, $dadosDoCaso, 0);
	}	
}

?>