<?php
$ProcBPM_Envelope = 11; // Processo Envelope
$StepBPM_Envelope = 5;  // Passo dos envelopes Processados
$StepBPM_IniciaEnvelope = 1;
$Campos_envelope["Fields"] = array(4); // Campos Selecionados
$Campos_envelope["Campos"] = array(4); // Campos Apresentados
//$ProcBPM_Servicos = 32;  // Processo de Conteudo
$ProcBPM_Servicos = 27;  // Processo de Conteudo
$StepBPM_Servicos = 2; // Passo dos Conteudo em Processamento
$ProcId = 5;            // Processo App q Lista os Campos Apresentados do Servico
$StepBPM_APP = 2;      // Passo no App q lista os Campos Apresentados do Servico
$Campo_Envelope = 5;	// Campo No Processo Envelope q contem o nr de envelope informado
$Campo_Envelope_Origem = 8;
$Campo_Servico_Envelope = 17; // Campo No Processo Servico q contem o nr do Envelope Informado
$Campo_Servico_Remover = 25;  // Campo q indicará que o serviço deve ser removido do Envelope
$Campo_Remover_Envelope = 6; // Campo q indicara que o Envelope deve ser apagado
$Embalagem_TipoEmbalagem = 4;
$Embalagem_Detalhado = 6;
$Campo_Processar_Envelope = 11;
//
$NREnvelopeAutomatico = true;
$Campo_Servico_StatusId = 14;
$Campo_Servico_StatusDesc = 15;
$Status_Habilitados = array(1,3,4,5,6,7);

$EmbalagemNomeDistribuidora = 12;
$EmbalagemCodDistribuidora = 13;
$EmbalagemResponsavel = 14;
$EmbalagemTelefone = 15;
$EmbalagemDataPostagem = 18;
$EmbalagemDataRemessa = 19;
$EmbalagemDataRecebimento = 20;
$EmbalagemTipoDistribuidora = 21;
?>
