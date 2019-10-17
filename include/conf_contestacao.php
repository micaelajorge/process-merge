<?php
$ProcId_Servicos = 1;
$StepId_Servicos = 16;
$Status_Servicos = '1,2';
$Origem = $userdef->Origem;
//$Origem = 'TC-000-0001';

$CampoStatus_Servicos = "24"; // Campo Id do Status
$CampoDescStatus = 29;
$CampoOrigem_Servicos = "21"; // Origem do Serviço
$CampoJustificativa_Servicos = "24"; // Campo Justificativa
$CampoContestacao_ok = "28";
$CampoDataExecucao_Servicos = 15;
$CampoProcRum = 5;
$CampoProcRumDesc = 32;

$ProcId_Pendencia = 2;
$StepId_Pendencia = 2;

$CampoOrigem_Pendencia = "5"; // Origem da Pendencia
$CampoNrServico_Pendencia = "6";
$CampoArquivo_Pendencia = 10;
$CampoPendencia_Pendencia = 7;
$CampoJustificativa_Pendencia = 5;
$CampoContestacao_ok_Pendencia = "9";
$CampoNome_Pendencia = 13;

$AuthUser["UserName"] = "pdv1";
$AuthUser["UserPassword"] = "123";

$ProcId_Contestacao = 3;
$CampoContestacao_NrServico = 4;
$CampoCotestacao_NrPendencia = 5;
$CampoContestacao_Contestacao = 6;
$CampoContestacao_Origem = 8;
$CampoContestacao_Arquivo = 10;
$CampoContestacao_resposta = 7;
$CampoContestacao_respondido = 15;
$CampoContestacao_Unread = 11;
$CampoContestacao_DataResposta = 14;

?>