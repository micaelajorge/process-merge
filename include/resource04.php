<?php
// Manutenчуo de usuсrios

function recriaPermissoesUsuariosGrupo($connect, $GroupId)
	{
	$SQL = " select ";
	$SQL.=" UserId  ";
	$SQL.=" from  ";
	$SQL.=" usergroup  ";
	$SQL.=" where  ";
	$SQL.=" usergroup.GroupId = $GroupId ";
	$SQL.=" order by UserId ";
	$Query = mysqli_query($connect, $SQL);
	while ($Result = mysqli_fetch_array($Query))
		{
		$UserId = $Result["UserId"];
		recriaPermissoesUsuario($connect, $UserId);
		}
	}

function recriaPermissoesUsuario($connect, $UserId)
	{
	$SQL = " update ";
	$SQL.=" userdef ";
	$SQL.=" set ";
	$SQL.=" WF = 0, ";
	$SQL.=" WG = 0, ";
	$SQL.=" CT = 0 ";
	$SQL.=" where ";
	$SQL.=" UserId = $UserId ";
	mysqli_query($connect, $SQL);

	$SQL = " select distinct ";
	$SQL.=" TipoProc  ";
	$SQL.=" from  ";
	$SQL.=" stepaddresses,  ";
	$SQL.=" usergroup,  ";
	$SQL.=" procdef  ";
	$SQL.=" where  ";
	$SQL.=" usergroup.GroupId  = stepaddresses.GroupId  "; 
	$SQL.=" and  ";
	$SQL.=" procdef.ProcId = stepaddresses.ProcId  ";
	$SQL.=" and ";
	$SQL.=" usergroup.UserId = $UserId ";
	$Query2 = mysqli_query($connect, $SQL);
	while ($Result = mysqli_fetch_array($Query2))
		{
		$Tipo = $Result["TipoProc"];
		$SQL = " update ";
		$SQL.=" userdef ";
		$SQL.=" set ";
		$SQL.=" $Tipo = 1 ";
		$SQL.=" where ";
		$SQL.=" UserId = $UserId ";
		mysqli_query($connect, $SQL);			
		}
	}		
?>