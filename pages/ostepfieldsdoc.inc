<?php
// //Versao 1.0.0 /Versao
define('StepFieldsDoc','1.0');
class STEPFIELD extends FIELD
{
var $InStep;
var $ReadOnly;
var $Optional;
var $OrderId;
var $NewLine;
var $CSS;
}

class STEPFIELDS extends FIELDS
{
var $field;	

function Cria($connect, $ProcId, $StepId)
	{
	$this->_createStepFields($connect, $ProcId, $StepId);
	}
		
function _createStepFields($connect, $ProcId, $StepId)
	{
	$this->_create($connect, $ProcId);
	$SQL = " select ";
	$SQL.=" FieldId, ";	
	$SQL.=" ReadOnly, ";
	$SQL.=" Optional, ";
	$SQL.=" CSS, ";
	$SQL.=" NewLine ";
	$SQL.=" from ";
	$SQL.=" stepfieldsdef ";
	$SQL.=" where ";
	$SQL.=" stepfieldsdef.ProcId = $ProcId ";
	$SQL.=" and ";
	$SQL.=" stepfieldsdef.StepId = $StepId ";
	$Query = mysqli_query($connect, $SQL);
	while ($Linha = mysqli_fetch_array($Query))
		{
		$this->setPropsFieldStep($Linha, $connect, $ProcId);
		}
	$this->field = new STEPFIELD;
	mysqli_free_result($Query);	
	}
	
function setPropsFieldStep($Linha)
	{
	$this->Field[$Linha["FieldId"]]->InStep = 1;
	$this->Field[$Linha["FieldId"]]->CSS = $Linha["CSS"];
	$this->Field[$Linha["FieldId"]]->ReadOnly = $Linha["ReadOnly"];
	$this->Field[$Linha["FieldId"]]->NewLine = $Linha["NewLine"];
	$this->Field[$Linha["FieldId"]]->Optional = $Linha["Optional"];		
	}
}