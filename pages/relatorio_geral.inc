<?php

/*
  Cria��o: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Cria��o 10/12/2018
  Sistema: Teste_MARCELO
 *  @rota relatorio_geral
 *  TODO: REFATORAR AS FUNÇÔES, 
 *  foi apenas "duplicada" a função para atender emergencialmente a criação de arquivo CSV
 */
require_once(FILES_ROOT . '/include/resource01.php');
require_once(FILES_ROOT . '/include/inctime.php');

use raelgc\view\Template;

// <editor-fold defaultstate="collapsed" desc="Funcoes Relatorio">
function AcumulaSomatorias($camposAgrupamento, $valorCampos)
{
    for ($contadorGrupos = 0; $contadorGrupos < count($camposAgrupamento); $contadorGrupos++) {
        foreach ($valorCampos as $key => $campo) {
            $camposAgrupamento[$contadorGrupos]["campos_soma"][$key]["valor_soma"] = $camposAgrupamento[$contadorGrupos]["campos_soma"][$key]["valor_soma"] + $campo["valor_soma"];
        }
    }
    return $camposAgrupamento;
}

function TotaisRelatorio($t_body, $camposAgrupamento, $contadorGrupos, $totalizarDados)
{
    // Se não houver totalizacoes sai
    if (!$totalizarDados) {
        return $camposAgrupamento;
    }
    if ($camposAgrupamento[$contadorGrupos]["conteudo_anterior"] != "") {
        $valorCampo = $camposAgrupamento[$contadorGrupos]["conteudo_anterior"];
        $valorCampo = replace($valorCampo, " 00:00", "");
        $t_body->CAMPO_VALOR = "Totais " . $valorCampo;
        $t_body->ALINHAMENTO_CELULA = "left";
        $t_body->COLSPAN = 1;
        $t_body->block("BLOCO_CAMPO");
        foreach ($camposAgrupamento[$contadorGrupos]["campos_soma"] as $key => $valor) {
            $t_body->CAMPO_VALOR = "";
            if ($valor["fieldtype"] == "NU" & $valor["somar"] == 1) {
                $t_body->CAMPO_VALOR = $valor["valor_soma"];
            }
            $t_body->ALINHAMENTO_CELULA = "center";
            $t_body->COLSPAN = 1;
            $t_body->block("BLOCO_CAMPO");
            // Zera o valor do Acumulador
            $camposAgrupamento[$contadorGrupos]["campos_soma"][$key]["valor_soma"] = 0;
        }
        $t_body->block("BLOCO_LINHA");
    }
    return $camposAgrupamento;
}

function CabecalhoRelatorio($t_body, $camposAgrupamento, $colSpan, $linha, $contadorGrupos, $totalizarDados)
{
    if ($totalizarDados) {
        $campo = $camposAgrupamento[$contadorGrupos]["fieldId"];
        $valorCampo = $camposAgrupamento[$contadorGrupos]["fieldname"] . " " . $linha["CampoDisplay$campo"];
        $valorCampo = replace($valorCampo, " 00:00", "");
        $t_body->CAMPO_VALOR = $valorCampo;
        $t_body->ALINHAMENTO_CELULA = "left";
        $t_body->COLSPAN = $colSpan + 1;
        $t_body->block("BLOCO_CAMPO");
        $t_body->block("BLOCO_LINHA");
        $camposAgrupamento[$contadorGrupos]["conteudo_anterior"] = $linha["CampoDisplay$campo"];
    }
    return $camposAgrupamento;
}

function CabecalhosRelatorio($t_body, $camposAgrupamento, $colSpan, $linha, $totalizarDados)
{
    for ($contadorGrupos = 0; $contadorGrupos < count($camposAgrupamento); $contadorGrupos++) {
        $campo = $camposAgrupamento[$contadorGrupos]["fieldId"];
        if ($camposAgrupamento[$contadorGrupos]["conteudo_anterior"] != $linha["CampoDisplay$campo"]) {
            $camposAgrupamento = TotaisRelatorio($t_body, $camposAgrupamento, $contadorGrupos, $totalizarDados);
            $camposAgrupamento = CabecalhoRelatorio($t_body, $camposAgrupamento, $colSpan, $linha, $contadorGrupos, $totalizarDados);
        }
    }
    return $camposAgrupamento;
}

// </editor-fold>

/**
 * Gera o arquivo em CSV
 * 
 * 
 * @global type $connect
 * @global type $FIELDSELECTION
 * @global type $data_inicio
 * @global type $data_fim
 * @global type $PROCCOD
 * @global type $STEPCOD
 * @global type $nomerelatorio
 * @global type $STEPNAME
 * @global type $PROCNAME
 */
function relatorioCSV()
{
    global $connect, $FIELDSELECTION, $data_inicio, $data_fim, $PROCCOD, $STEPCOD, $nomerelatorio, $STEPNAME, $PROCNAME;
    $t_body = new Template(FILES_ROOT . "assets/templates/t_relatorio_campos_CSV.html");

//$CAMPO_SELECAO = "data_preenchimento";
    $CAMPO_SELECAO = $FIELDSELECTION;

    $dataInicio = FormataDataDB($data_inicio);
    $dataFim = incDays(FormataDataDB($data_fim), 1);

    $procId = PegaProcIdByCode($PROCCOD);
    $stepId = PegaStepIdByCode($procId, $STEPCOD);
    $fieldSelecao = PegaFieldIdByCode($procId, $CAMPO_SELECAO);

    $t_body->TITULO_RELATORIO = $nomerelatorio;
    $t_body->DATA_INICIO = $data_inicio;
    $t_body->DATA_FIM = $data_fim;
    $t_body->STEPNAME = $STEPNAME;
    $t_body->PROCNAME = $PROCNAME;
    
    
    $relFileName = $STEPNAME;

//Define se haverá totalização no Relatorio
    $totalizarDados = false;

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Seleção dos Campos do Relatorio">
    $SQL_CAMPOS = "select "
            . "procfieldsdef.fieldId, "
            . "fieldtype, "
            . "fieldname, "
            . "readonly as agrupar, "
            . "optional as somar "
            . "from "
            . "stepfieldsdef, procfieldsdef  "
            . "where "
            . "procfieldsdef.procId = $procId and procfieldsdef.procid = stepfieldsdef.procid and procfieldsdef.fieldId = stepfieldsdef.fieldid and StepId = $stepId order by orderId";
    $Query = mysqli_query($connect, $SQL_CAMPOS);
    $camposSelecionados = mysqli_fetch_all($Query, MYSQLI_ASSOC);
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Definição de Colunas somatoria e agrupamento">
    $camposSoma = array();
    $camposAgrupamento = array();
    $camposRelatorio = array();
    $camposOrdenacaSQLAgrupamento = array();

    foreach ($camposSelecionados as $campoSelecionado) {
        $fieldId = $campoSelecionado["fieldId"];

        // Inclui o campo AGRUPAMENTO e remove da lista de campos apresentados
        if ($campoSelecionado["agrupar"] == 1) {
            $camposOrdenacaSQLAgrupamento[] = "Campo" . $fieldId;
            $campoAgrupamento = $campoSelecionado;
            $campoAgrupamento["conteudo_anterior"] = "";
            $camposAgrupamento[] = $campoAgrupamento;
            continue;
        }

        // Inclui campo de SOMA
        if ($campoSelecionado["somar"] == 1 & $campoSelecionado["fieldtype"] == "NU") {
            $totalizarDados = true;
        }
        $campoSelecionado["valor_soma"] = "";
        $camposSoma[$fieldId] = $campoSelecionado;
        $camposRelatorio[] = $campoSelecionado;
    }

// Quarda uma copia dos campos para manter zerado
    $camposSomaZerado = $camposSoma;
    $camposSomaTotalGeral = $camposSoma;

// Cria os campos de Somatoria para o agrupamento
    for ($contador = 0; $contador < count($camposAgrupamento); $contador++) {
        $camposAgrupamento[$contador]["campos_soma"] = $camposSoma;
    }

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Seleção dados do relatório">
    $SQL_DADOS = "select exportkeysdisplay.*, exportkeys.Campo$fieldSelecao "
            . "from "
            . "exportkeysdisplay, exportkeys "
            . "where "
            . "exportkeys.ProcId = $procId and exportkeys.procid = exportkeysdisplay.procid and exportkeys.casenum = exportkeysdisplay.casenum and Campo$fieldSelecao >= '$dataInicio' and Campo$fieldSelecao <= '$dataFim' ";

    if (count($camposOrdenacaSQLAgrupamento) > 0) {
        $camposOrdem = implode(", ", $camposOrdenacaSQLAgrupamento);
        $SQL_DADOS .= " order by $camposOrdem ";
    }

    $Query = mysqli_query($connect, $SQL_DADOS);
    $dados = mysqli_fetch_all($Query, MYSQLI_ASSOC);
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Monta Cabecalho do relatorio">
//    $t_body->CAMPO_NOME = "";
    $t_body->SEPARADOR = "";
//    $t_body->block("BLOCO_CABECALHO");
    for ($i = 0; $i < count($camposRelatorio); $i++) {
        $t_body->CAMPO_NOME = $camposRelatorio[$i]["fieldname"];
        $t_body->block("BLOCO_CABECALHO");
        $t_body->SEPARADOR = ";";
    }
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Geração Relatorio">
    $colSpan = count($camposRelatorio);
    $contadorDia = 0;
    $diaAnterior = "";
    foreach ($dados as $linha) {
        // <editor-fold defaultstate="collapsed" desc="Coloca cabecalho de agrupamento">
        if ($totalizarDados) {
            $camposAgrupamento = CabecalhosRelatorio($t_body, $camposAgrupamento, $colSpan, $linha, $totalizarDados);
            // </editor-fold>
        }
        $t_body->CAMPO_VALOR = "";
        $t_body->COLSPAN = 1;
        $t_body->block("BLOCO_CAMPO");
        $t_body->SEPARADOR = "\n";
        for ($i = 0; $i < count($camposRelatorio); $i++) {
            $tipoCampo = $camposRelatorio[$i]["fieldtype"];
            $campo = $camposRelatorio[$i]["fieldId"];

            // <editor-fold defaultstate="collapsed" desc="Definição Alinhamento coluna">
            $t_body->ALINHAMENTO_CELULA = "";
            if ($tipoCampo == "NU") {
                $t_body->ALINHAMENTO_CELULA = "center";

                // Acumula valor do campos
                $camposSoma[$campo]["valor_soma"] = $linha["CampoDisplay$campo"];
                $camposSomaTotalGeral[$campo]["valor_soma"] = $camposSomaTotalGeral[$campo]["valor_soma"] + $linha["CampoDisplay$campo"];
            }
            // </editor-fold>

            $valorCampo = trim($linha["CampoDisplay$campo"]);
            $t_body->CAMPO_VALOR = ($valorCampo == "") ? "" : "\"$valorCampo\"";
            $t_body->COLSPAN = 1;
            $t_body->block("BLOCO_CAMPO");
            $t_body->SEPARADOR = ";";
        }
        $t_body->block("BLOCO_LINHA");

        $camposAgrupamento = AcumulaSomatorias($camposAgrupamento, $camposSoma);
        $camposSoma = $camposSomaZerado;

        array_map(zeraValores, $camposSoma);
        $contadorDia++;
    }
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Envio dados BROWSER">
    $dadosArquivo = $t_body->parse();

    EnviaArquivoBrowser("$relFileName.txt", "$STEPNAME", $dadosArquivo, 'txt');
    // </editor-fold>
}

/**
 * Gera o arquivo em XLS
 * 
 * @global type $connect
 * @global type $FIELDSELECTION
 * @global type $data_inicio
 * @global type $data_fim
 * @global type $PROCCOD
 * @global type $STEPCOD
 * @global type $nomerelatorio
 * @global type $STEPNAME
 * @global type $PROCNAME
 * @global type $tipoArquivo
 */
function relatorioXLS()
{
    global $connect, $FIELDSELECTION, $data_inicio, $data_fim, $PROCCOD, $STEPCOD, $nomerelatorio, $STEPNAME, $PROCNAME, $tipoArquivo, $relFileName;
    $t_body = new Template(FILES_ROOT . "assets/templates/t_relatorio_campos.html");

//$CAMPO_SELECAO = "data_preenchimento";
    $CAMPO_SELECAO = $FIELDSELECTION;

    $dataInicio = FormataDataDB($data_inicio);
    $dataFim = incDays(FormataDataDB($data_fim), 1);

    $procId = PegaProcIdByCode($PROCCOD);
    $stepId = PegaStepIdByCode($procId, $STEPCOD);
    $fieldSelecao = PegaFieldIdByCode($procId, $CAMPO_SELECAO);

    $t_body->TITULO_RELATORIO = $nomerelatorio;
    $t_body->DATA_INICIO = $data_inicio;
    $t_body->DATA_FIM = $data_fim;
    $t_body->STEPNAME = $STEPNAME;
    $t_body->PROCNAME = $PROCNAME;

//Define se haverá totalização no Relatorio
    $totalizarDados = false;

// <editor-fold defaultstate="collapsed" desc="Seleção dos Campos do Relatorio">
    $SQL_CAMPOS = "select "
            . "procfieldsdef.fieldId, "
            . "fieldtype, "
            . "fieldname, "
            . "readonly as agrupar, "
            . "optional as somar "
            . "from "
            . "stepfieldsdef, procfieldsdef  "
            . "where "
            . "procfieldsdef.procId = $procId and procfieldsdef.procid = stepfieldsdef.procid and procfieldsdef.fieldId = stepfieldsdef.fieldid and StepId = $stepId order by orderId";
    $Query = mysqli_query($connect, $SQL_CAMPOS);
    $camposSelecionados = mysqli_fetch_all($Query, MYSQLI_ASSOC);
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Definição de Colunas somatoria e agrupamento">
    $camposSoma = array();
    $camposAgrupamento = array();
    $camposRelatorio = array();
    $camposOrdenacaSQLAgrupamento = array();

    $camposParaDisplay = "";
    $separador = "";
    
    foreach ($camposSelecionados as $campoSelecionado) {
        $fieldId = $campoSelecionado["fieldId"];

        // Inclui o campo AGRUPAMENTO e remove da lista de campos apresentados
        if ($campoSelecionado["agrupar"] == 1) {
            $camposOrdenacaSQLAgrupamento[] = "Campo" . $fieldId;
            $campoAgrupamento = $campoSelecionado;
            $campoAgrupamento["conteudo_anterior"] = "";
            $camposAgrupamento[] = $campoAgrupamento;
            continue;
        }

        // Inclui campo de SOMA
        if ($campoSelecionado["somar"] == 1 & $campoSelecionado["fieldtype"] == "NU") {
            $totalizarDados = true;
        }
        $campoSelecionado["valor_soma"] = "";
        $camposSoma[$fieldId] = $campoSelecionado;
        $camposRelatorio[] = $campoSelecionado;
        $camposParaDisplay .= "$separador CampoDisplay" . $fieldId;
        $separador = ",";
    }

// Quarda uma copia dos campos para manter zerado
    $camposSomaZerado = $camposSoma;
    $camposSomaTotalGeral = $camposSoma;

// Cria os campos de Somatoria para o agrupamento
    for ($contador = 0; $contador < count($camposAgrupamento); $contador++) {
        $camposAgrupamento[$contador]["campos_soma"] = $camposSoma;
    }

// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="Seleção dados do relatório">

    // Lista todos os casos se não tiver campo de seleção
    if ($fieldSelecao != "" & $fieldSelecao != "0") {
        $SQL_SELECAO = "and Campo$fieldSelecao >= '$dataInicio' and Campo$fieldSelecao <= '$dataFim' ";
        $CampoDeSelecao = ", exportkeys.Campo$fieldSelecao ";
    }

    $SQL_DADOS = "select $camposParaDisplay "
            . " $CampoDeSelecao"
            . "from "
            . "exportkeysdisplay, exportkeys "
            . "where "
            . "exportkeys.ProcId = $procId and exportkeys.procid = exportkeysdisplay.procid and exportkeys.casenum = exportkeysdisplay.casenum ";

    $SQL_DADOS .= $SQL_SELECAO;
    
    if (count($camposOrdenacaSQLAgrupamento) > 0) {
        $camposOrdem = implode(", ", $camposOrdenacaSQLAgrupamento);
        $SQL_DADOS .= " order by $camposOrdem ";
    }

    $Query = mysqli_query($connect, $SQL_DADOS);
    $dados = mysqli_fetch_all($Query, MYSQLI_ASSOC);
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Monta Cabecalho do relatorio">
    $t_body->CAMPO_NOME = "";
    $t_body->block("BLOCO_CABECALHO");
    for ($i = 0; $i < count($camposRelatorio); $i++) {
        $t_body->CAMPO_NOME = $camposRelatorio[$i]["fieldname"];
        $t_body->block("BLOCO_CABECALHO");
    }
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Geração Relatorio">
    $colSpan = count($camposRelatorio);
    $contadorDia = 0;
    $diaAnterior = "";
    foreach ($dados as $linha) {
        // <editor-fold defaultstate="collapsed" desc="Coloca cabecalho de agrupamento">
        if ($totalizarDados) {
            $camposAgrupamento = CabecalhosRelatorio($t_body, $camposAgrupamento, $colSpan, $linha, $totalizarDados);
            // </editor-fold>
        }
        $t_body->CAMPO_VALOR = "";
        $t_body->COLSPAN = 1;
        $t_body->block("BLOCO_CAMPO");

        for ($i = 0; $i < count($camposRelatorio); $i++) {
            $tipoCampo = $camposRelatorio[$i]["fieldtype"];
            $campo = $camposRelatorio[$i]["fieldId"];

            // <editor-fold defaultstate="collapsed" desc="Definição Alinhamento coluna">
            $t_body->ALINHAMENTO_CELULA = "";
            if ($tipoCampo == "NU") {
                $t_body->ALINHAMENTO_CELULA = "center";

                // Acumula valor do campos
                $camposSoma[$campo]["valor_soma"] = $linha["CampoDisplay$campo"];
                $camposSomaTotalGeral[$campo]["valor_soma"] = $camposSomaTotalGeral[$campo]["valor_soma"] + $linha["CampoDisplay$campo"];
            }
            // </editor-fold>

            $t_body->CAMPO_VALOR = $linha["CampoDisplay$campo"];
            $t_body->COLSPAN = 1;
            $t_body->block("BLOCO_CAMPO");
        }
        $t_body->block("BLOCO_LINHA");

        $camposAgrupamento = AcumulaSomatorias($camposAgrupamento, $camposSoma);
        $camposSoma = $camposSomaZerado;

        array_map(zeraValores, $camposSoma);
        $contadorDia++;
    }
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Totalizacao Final">
    if ($totalizarDados) {

// Totais do ultimo Agrupamento
        for ($contadorGrupos = 0; $contadorGrupos < count($camposAgrupamento); $contadorGrupos++) {
            TotaisRelatorio($t_body, $camposAgrupamento, $contadorGrupos, $totalizarDados);
        }

        foreach ($camposSomaTotalGeral as $soma) {
            $t_body->VALOR_TOTAL = "";
            if ($soma["fieldtype"] == "NU" & $soma["somar"] == 1) {
                $t_body->VALOR_TOTAL = $soma["valor_soma"];
            }
            $t_body->block("BLOCO_COLUNA_TOTAL_GERAL");
        }
        $t_body->block("BLOCK_TOTAL_GERAL");
    }
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Envio dados BROWSER">
    $dadosArquivo = $t_body->parse();

    if ($tipoArquivo !== 'excel') {
        //echo $dadosArquivo;
        EnviaArquivoBrowser("$STEPNAME.pdf", "$STEPNAME", $dadosArquivo, 'pdf');
    } else {
        EnviaArquivoBrowser("$STEPNAME.xls", "$STEPNAME", $dadosArquivo, 'excel');
    }
    // </editor-fold>
}

