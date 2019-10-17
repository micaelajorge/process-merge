<?php
include("eventlistFuncs.php");

$procdef = $_SESSION["procdef"];

if ($InstanceName == '')
	{
	$InstanceName = "Caso";
	}
?> 
<table class="table table-striped small">
  <tr> 
    <th >Data e Hora da A&ccedil;&atilde;o</th>
    <th >A&ccedil;&atilde;o</strong></th>
    <th >Passo</th>
    <th >Usu&aacute;rio</th>
  </tr>
  <?php
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
	$Evento = $EventDesc[$EventId];
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
  ?>
  <tr> 
    <td><?= convdate($Result["EventDateTime"], false) ?></td>  
    <td><?= nl2br($Evento) ?></a></td>
    <td><?= $StepName ?></td>
    <td><?= htmlentities($Result["UserName"]) ?> </td>
  </tr>
  <?php
  }
  ?>
</table>
