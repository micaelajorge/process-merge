<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 30/10/2019
  Sistema: Process_XAMPP
 */

use raelgc\view\Template;

function inicializacao_ext_campo_json_array($FieldId, $ReadOnly, $Valor_Campo, $dadosCampo)
{
    global $dadosCampo, $FieldCod;
    $t_body_field = new Template(FILES_ROOT . "expand/ext_campo_json_array/t_ext_campo_json_array.html");
    $t_body_field->FIELDCODE = $FieldCod;
    $t_body_field->FIELDID = $FieldId;
    $t_body_field->FIELD_ID = $FieldId;
    $t_body_field->VALOR_CAMPO = htmlentities($Valor_Campo);

    $parametrosDoCampo = $dadosCampo["parametros"];
    $colunasValor = array();
   
    $contadorColuna = 1;
    foreach ($parametrosDoCampo as $parametro) {
        $t_body_field->MODAL_TITULO_COLUNA = $parametro;
        $t_body_field->block("BLOCK_MODAL_COLUNAS");
        $coluna["title"] = $parametro;
        $coluna["data"] = $parametro;
        $coluna["target"] = $contadorColuna;
        $colunasValor[] = $coluna;
        $contadorColuna++;
    }

    // Seta o valor do Campo Colunas
    $t_body_field->DADOS_COLUNAS = htmlentities(json_encode($colunasValor));

    $listaDados = json_decode($Valor_Campo, TRUE);

    if (!$ReadOnly) {
        $t_body_field->block("BLOCK_NEW_ITEM");
        $t_body_field->block("BLOCO_MODAL_FIELD");
    }

    $t_body_field->ARQUIVO_JS = 'ext_campo_json_array.js';
    $t_body_field->block('BLOCO_JAVASCRIPT');

    $t_body_field->block('BLOCO_INICIA_TABELA');

    $retorno["html"] = $t_body_field->parse();

    return $retorno;
}
