<?php

include_once(FILES_ROOT . "include/func_api.inc");

use raelgc\view\Template;

function Inicializacao_Custas_Registro($FieldId, $ReadOnly, $Valor_Campo, $dadosCampo)
{
    $template = new Template(FILES_ROOT . "assets/templates/t_campo_custas_registro.html");
    $template->addFile("TABELA_DESPESAS", FILES_ROOT . "assets/templates/t_campo_custas_registro_table.html");
    $template->FIELDID = $FieldId;

    $template->ARQUIVO_JS = "custas_registro.js";
    $template->block("BLOCO_JAVASCRIPT");

    if (!$ReadOnly) {
        $template->block("BLOCK_EDITAR");
    }

    if ($Valor_Campo == "") {
        $Valor_Campo = "[]";
    }
    //$Valor_Campo = '[{"tipoDespesa":"444","despesa":"44","valorDespesa":"44"}]';

    $listaDespesas = json_decode(rawurldecode($Valor_Campo), true);

    $listaValoresDespesas = array_column($listaDespesas, "valorDespesa");
    $totalCustas = array_sum($listaValoresDespesas);

    MontaTabelaDespesas($template, $listaDespesas);

    $template->LISTADESPESAS = $Valor_Campo;
    $template->block("BLOCO_VIEW_FIELD");

    $template->VALOR_CAMPO = number_format($totalCustas, 2, ',', '.');
    return $template->parse();
}

function MostraModal_Custas_Registro()
{
    $dadosEntrada = apiTrataDadosEntrada();

    $fieldId = $dadosEntrada["fieldId"];
    $template = new Template(FILES_ROOT . "assets/templates/t_campo_custas_registro.html");
    $template->addFile("TABELA_DESPESAS", FILES_ROOT . "assets/templates/t_campo_custas_registro_table.html");

    $template->FIELDID = $fieldId;

    $listaDespesas = array();
    if (key_exists("listaDespesas", $dadosEntrada)) {
        $listaDespesas = $dadosEntrada["listaDespesas"];
    }

    $template->LISTADESPESAS = rawurlencode(json_encode($listaDespesas));
    $template->INDICEDESPESA = -1;
    MontaTabelaDespesas($template, $listaDespesas, 1);

    $template->block("BLOCO_MODAL_FIELD");
    echo $template->parse();
}

function MontaTabelaDespesas($template, $listaDespesas, $editar = 0)
{
    $totalDespesas = 0;
    $i = 0;

    if ($editar == 1) {
        $template->block("BLOCK_HEADER_EDITAR");
    } else {
        $template->block("BLOCK_HEADER");
    }

    foreach ($listaDespesas as $itemDespesa) {
        $template->TIPODESPESA = $itemDespesa["tipoDespesa"];
        $template->DESPESA = $itemDespesa["despesa"];
        $template->VALOR = number_format($itemDespesa["valorDespesa"], 2, ',', '.');
        $totalDespesas += $itemDespesa["valorDespesa"];
        if ($editar == 1) {
            $template->INDICE_DESPESA = $i;
            $template->block("BLOCK_EDITAR_DESPESA");
        }         $template->block("BLOCK_DETALHES");
        $i++;
    }
    if ($editar == 1)
    {
        $template->COLSOAN_TOTAL = "3";
    } else {
        $template->COLSOAN_TOTAL = "2";
    }
    
    $template->VALORTOTAL = number_format($totalDespesas, 2, ',', '.');
}

function CarregaTabelaDespesas()
{
    $dadosEntrada = apiTrataDadosEntrada();
    $template = new Template(FILES_ROOT . "assets/templates/t_campo_custas_registro_table.html");
    $listaDespesas = $dadosEntrada["listaDespesas"];

    MontaTabelaDespesas($template, $listaDespesas, $dadosEntrada["editar"]);
    echo $template->parse();
}

switch ($acaoCampo) {
    case "adicionarDespesa":
        break;
    case "removerDesesa":
        break;
}
