<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 07/01/2020
  Sistema: Process
 */

require_once(FILES_ROOT . '/include/inctime.php');
require_once(FILES_ROOT . '/include/eventlistfuncs.php');


$dataInicio = FormataDataDB($data_inicio);
$dataFim = incDays(FormataDataDB($data_fim), 1);


$SQL .= " select EventDateTime, 
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
                EventDateTime >= '$dataInicio' and EventDateTime < '$dataFim'
		";

$SQL .= " order by EventDateTime desc";
$Query = mysqli_query($connect, $SQL);
$AlternaClasse = false;

$dadosArquivo = "";
while ($Result = mysqli_fetch_array($Query)) {    
    $EventId = $Result["EventId"];
    $StepName = htmlentities($Result["StepName"]);
    $FieldId = $Result["FieldId"];
    $Audit = $Result["Audit"];
    $EventoDesc = $EventDesc[$EventId];
    $StepId = $Result["StepId"];
    $StartStep = $Result["StartStep"];
    $ConditionName = $Result["ConditionName"];
    $ActionDesc = $Result["ActionDesc"];
    $Evento = CriaEventDesc($procdef->ProcId, $CaseNum, $EventId, $StepName, $StepId, $StartStep, $ActionDesc, $ConditionName, $Audit, $Tipo, false);       
    $dadosArquivo .= convdate($Result["EventDateTime"], false);  
    $dadosArquivo .= ";";
    $dadosArquivo .= replace($Evento, "\n", "|");
    $dadosArquivo .= ";";
    $dadosArquivo .= $Result["UserName"];    
    $dadosArquivo .= "\n";
}


EnviaArquivoBrowser("relatorio log.csv", "Logs", $dadosArquivo, 'csv');