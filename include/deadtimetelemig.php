<?php
require_once("include/connection.php");
require_once("include/resource01.php");
function DeadTimeTelemig($ProcId, $CaseNum, $campo, $Origem)
	{
	global $userdef;
	$Data = PegaValorCampo($ProcId, $CaseNum, $campo);
	if (substr($Origem, 0, 2) == 'LJ')
		{
		return TeleMigDireto($Data);
		}
	else
		{
		return TeleMigInDireto($Data);
		}
	}
	
function TeleMigCalcDeadTime($Data, $meses, $dias)
	{
//	$mes = date('m') + $meses;
//	$ano = date('Y');
	$mes = substr($Data, 5, 2);
	//$mes = date('m', $Data) + $meses;
	//$ano = date('Y', $Data);
	$mes = $mes + $meses;
	$ano = substr($Data, 0, 4);
	if (strlen($dias) == 1)
		{
		$dias = '0' . $dias;
		}		
	if ($mes > 12)
		{
		$ano++;
		}
	if ($mes < 10)
		{
		$mes = '0' . $mes;
		}
	if ($mes == 13)
		{
		$mes = "01";
		}
	if ($mes == 14)
		{
		$mes = "02";
		}
	return "$ano-$mes-$dias";
	//return DateDiff(date('Y-m-d 00:01'), "$ano-$mes-$dias 00:01", 'm');
	}
	
function TeleMigDireto($Data)
	{
	//return DeadTimeTelemig(1,9);
	$minutos = TeleMigCalcDeadTime($Data, 1, 1);
	$DataF = incTime($Data, $minutos, 1);
	$DataF = calcula_DeadDateTime($DataF, 10080, '', '', 2);
	return DateDiff($Data, $DataF , 'm');
	}

function TeleMigIndireto($Data)
	{
	return TeleMigCalcDeadTime($Data, 2,10);
	}


function DiretoIndireto($Origem)
{
	global $connect;
	$SQL = "select Tipo_Canal from dealers where Origem = '$Origem'";
	$Query = mysqli_query($connect, $SQL);
	$Result = mysqli_fetch_array($Query);
	return $Result["Tipo_Canal"] == 'DIRETO';
	/*
	if (substr($Origem,0,2) == 'TC')
	{
		return false;
	}
	else {
		return true;
	} */
}
	
function month($Data)
{
	return substr($Data, 5, 2);
}

function ano($Data)
{
	return substr($Data, 0, 4);
}

function UltimoDiaMes($Data)
{
    $Mes = substr($Data,5,2);
    $Ano = substr($Data,0,4);
    if ($Mes == "04" || $Mes == "06" || $Mes == "09" || $Mes == "11")
	{
	  $UltimoDia = "30";
	}  
	if ($Mes == "02")
	{
		if ((($Ano % 4) == 0) && ((($Ano % 100) <> 0) || (($Ano % 1000) == 0)))
		{
		  $UltimoDia = 29;
		}  
		else 		{ 
		  $UltimoDia = 28;
		}		  
	}	
	else  {
		$UltimoDia = 31;
	}  
	$Data = $Ano . "-" . $Mes . "-" . $UltimoDia;
	return $Data;
}

function DeadTimeTeleMigEnvioHard($ProcId, $CaseNum, $campo, $Origem)
{
	global $userdef;
	$Data = PegaValorCampo($ProcId, $CaseNum, $campo);
	if (!DiretoIndireto($Origem))
	{
		$Data = TeleMigCalcDeadTime($Data, 1, 1);
		return UltimoDiaMes($Data);
	}
	else {
		return 0;
	}
}

function DeadTimeTelemigEnvioSoft($ProcId, $CaseNum, $campo, $Origem)
{
	global $userdef;
	$Data = PegaValorCampo($ProcId, $CaseNum, $campo);
	if (!DiretoIndireto($Origem))
	{
		$Dia = substr($Data, 8, 2);
		if ($Dia <= 15)
		{
			return TeleMigCalcDeadTime($Data, 1, 1);
		}
		else {
			return TeleMigCalcDeadTime($Data,1,16);
		}
	}
	else {
		$Data2 = incTime($Data, 7200, 2);
		$Data2 = substr($Data2, 0,10);
		return $Data2;
		//return DateDiff($Data, $Data2, 'm');
	}	
}
?>
