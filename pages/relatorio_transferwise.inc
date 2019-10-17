<?php

require_once(FILES_ROOT . '/include/resource01.php');
require_once(FILES_ROOT . '/include/inctime.php');

/*
  Cria��o: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Cria��o 10/12/2018
  Sistema: Teste_MARCELO
 */

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_relatorio_campos.html");

$CAMPO_SELECAO = "DATA_ENTRADA_ANALISE";

$dataInicio = FormataDataDB($data_inicio);
$dataFim = incDays(FormataDataDB($data_fim), 1);

$procId = PegaProcIdByCode($PROCCOD);
$stepId = PegaStepIdByCode($procId, $STEPCOD);
$fieldSelecao = PegaFieldIdByCode($procId, $CAMPO_SELECAO);
$SQL_CAMPOS = "select procfieldsdef.fieldId, fieldname from stepfieldsdef, procfieldsdef where procfieldsdef.procId = $procId and procfieldsdef.procid = stepfieldsdef.procid and procfieldsdef.fieldId = stepfieldsdef.fieldid and StepId = $stepId order by orderId";
$Query = mysqli_query($connect, $SQL_CAMPOS);
$campos = mysqli_fetch_all($Query, MYSQLI_ASSOC);

$SQL_DADOS = "select exportkeysdisplay.*, exportkeys.Campo$fieldSelecao from "
        . "exportkeysdisplay, exportkeys, casedata "
        . " where exportkeys.ProcId = $procId and exportkeys.procid = exportkeysdisplay.procid and exportkeys.casenum = exportkeysdisplay.casenum and Campo$fieldSelecao >= '$dataInicio' and Campo$fieldSelecao <= '$dataFim'"
        . "  and casedata.procid = exportkeys.procid and casedata.casenum = exportkeys.casenum and fieldId = 37 and fieldvalue <> '' order by Campo$fieldSelecao";

$Query = mysqli_query($connect, $SQL_DADOS);
$dados = mysqli_fetch_all($Query, MYSQLI_ASSOC);

$t_body->TITULO_RELATORIO = $nomerelatorio;
$t_body->DATA_INICIO = $data_inicio;
$t_body->DATA_FIM = $data_fim;
$t_body->STEPNAME = $STEPNAME;
$t_body->PROCNAME = $PROCNAME;

for ($i = 0; $i < count($campos); $i++) {
    $t_body->CAMPO_NOME = $campos[$i]["fieldname"];
    $t_body->block("BLOCO_CABECALHO");
}

$t_body->COLSPAN = count($campos) - 1;

$contadorDia = 0;
$diaAnterior = "";
foreach ($dados as $linha) {
    $valorDia = strtotime($linha["Campo$fieldSelecao"]);
    $dia = date("d", $valorDia);
    if ($dia != $diaAnterior & $diaAnterior != "") {
        $t_body->DIA = $dataAnterior;
        $t_body->TOTALPARCIAL = $contadorDia;
        $t_body->block("BLOCO_TOTAL_PARCIAL");
        $t_body->block("BLOCO_DIA");
        $contadorDia = 0;
    }
    $diaAnterior = $dia;
    $dataAnterior = date("d/m/Y", strtotime($linha["Campo$fieldSelecao"]));
    for ($i = 0; $i < count($campos); $i++) {
        $campo = $campos[$i]["fieldId"];
        $t_body->CAMPO_VALOR = $linha["CampoDisplay$campo"];
        $t_body->block("BLOCO_CAMPO");
    }
    $t_body->block("BLOCO_LINHA");
    $contadorDia++;
}
$t_body->DIA = $dataAnterior;
$t_body->TOTALPARCIAL = $contadorDia;
$t_body->block("BLOCO_TOTAL_PARCIAL");
$t_body->block("BLOCO_DIA");
$t_body->TOTAL_CASOS = count($dados);
$dadosArquivo = $t_body->parse();
if ($tipoArquivo == 'excel') {
    EnviaArquivoBrowser("$nomerelatorio.xls", "relatorio", $dadosArquivo, 'excel');
} else {
    EnviaArquivoBrowser("$nomerelatorio.pdf", "relatorio", $dadosArquivo, 'excel');
    //echo $dadosArquivo;
}
