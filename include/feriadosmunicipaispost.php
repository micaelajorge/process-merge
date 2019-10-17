<?php
// //Versao 1.0.0 /Versao
include("include\common.php");
if (strlen($Dia) == 1)
	{
	$Dia = '0' . $Dia;
	}
if (strlen($Mes) == 1)
	{
	$Mes = '0' . $Mes;
	}	
$Data = $Dia . '/' . $Mes;
if (empty($Id))
	{
	$SQL = "insert into FeriadosFederais (nome, data) values ('$Nome', '$Data')";
	}
else
	{
	if ($Acao == "Apagar")
		{
		$SQL = "delete from FeriadosFederais where Id_Feriado = $Id";
		}
	else
		{		
		$SQL = "update FeriadosFederais set nome = '$Nome', Data = '$Data' where Id_Feriado = $Id";
		}
	}
@mysqli_query($connect, $SQL);
include("FeriadosFederaisList.php");
?>
