<?php
$gmtDate = gmdate("D, d M Y H:i:s"); 
header("Content-Type: text/html; charset=ISO-8859-1",true);header("Expires: {$gmtDate} GMT"); 
header("Last-Modified: {$gmtDate} GMT"); 
header("Cache-Control: no-cache, must-revalidate"); 
header("Pragma: no-cache"); 
// //Versao 1.0.0 /Versao
// Visualização de Edições do Caso
require_once(FILES_ROOT . "/include/common.php");
require_once(FILES_ROOT . "/include/resource01.php");
include(FILES_ROOT . "/include/eventlistFuncs.php");

use raelgc\view\Template;

$template = new Template(FILES_ROOT . "assets/templates/t_historico_caso.html");

$procdef = $_SESSION["procdef"];

if ($InstanceName == '')
	{
	$InstanceName = "Caso";
	}
	$ShowEvents = array(900);
	
	if (!is_array($_audittrail))
		{
		$_audittrail[0] = "audittrail";	
		}
	$SShowEvents = implode(",", $ShowEvents);	
	
	$Union = "";
	
	$SQL = "";
        $InicioUnion = "";
	foreach ($_audittrail as $audittrail)
		{
		$SQL .= "
                $InicioUnion    
                select EventDateTime, 
		StepName, 
		EventId, 
		UserName, 
		FieldId, 
		Audit, 
		ActionDesc,
		StepId,
		ConditionName,
		StartStep
		from 
		audittrail
		where 
		CaseNum = $CaseNum 
		and
		ProcId = $procdef->ProcId
		";
		if ($Tipo != "Admin")
			{
			$SQL .= " and EventId in ($SShowEvents) ";
			}
			  	
	  	$SQL .= $Union;
	  	$Union = ") union ";
                $InicioUnion = "(";
		}
		
  $SQL .= " order by EventDateTime desc, FieldId desc, EventId desc  ";
  $Query = mysqli_query($connect, $SQL);  
  $AlternaClasse = false;
  while ($Result = mysqli_fetch_array($Query))
  	{
	$EventId = $Result["EventId"];
	$StepName = htmlentities($Result["StepName"]);
	$FieldId = $Result["FieldId"];
	$Audit = $Result["Audit"];
	$EventoDesc = $EventDesc[$EventId];
	$StepId = $Result["StepId"];
	$StartStep = $Result["StartStep"];
	$ConditionName = $Result["ConditionName"];
	$ActionDesc = $Result["ActionDesc"];
  	if ($StepId !== $StepIdOld)
  		{
  		$StepIdOld = $StepId;
  		$AlternaClasse = !$AlternaClasse;
  		$ClassItem = "CorItemAudit2";
  		if ($AlternaClasse)
  			{
  			$ClassItem = "CorItemAudit1";
  			}  		
  		}
  	if ($StepId == 0)
  		{
  		$ClassItem = "CorItemAuditAdministrador";	
  		}
  	$Evento = CriaEventDesc($procdef->ProcId, $CaseNum, $EventId, $StepName, $StepId, $StartStep, $ActionDesc, $ConditionName, $Audit, $Tipo);
        $template->EVENTDATETIME = convdate($Result["EventDateTime"], false);
        $template->EVENTDATA = nl2br($Evento);
        $template->STEPNAME = $StepName;
        $template->USERNAME = htmlentities($Result["UserName"]);
        $template->block("BLOCO_EVENTO");
  }
  $template->show();
  ?>
