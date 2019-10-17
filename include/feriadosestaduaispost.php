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
	$SQL = "insert into FeriadosEstaduais (nome, data, Estado ) values ('$Nome', '$Data', '$Estado')";
	}
else
	{
	if ($Acao == "Apagar")
		{
		$SQL = "delete from FeriadosEstaduais where Id_Feriado = $Id";
		}
	else
		{		
		$SQL = "update FeriadosEstaduais set nome = '$Nome', Data = '$Data' where Id_Feriado = $Id";
		}
	}
@mysqli_query($connect, $SQL);
include("FeriadosEstaduaisList.php");
?>
