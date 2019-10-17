<?php

include_once(FILES_ROOT . "include/func_api.inc");

use raelgc\view\Template;

function pegaListaDominios()
{
    global $connect;

    $sql = "select origem_doc from origemdominio group by origem_doc order by origem_doc";
    $query = mysqli_query($connect, $sql);

    $retorno = mysqli_fetch_all($query, MYSQLI_ASSOC);
    return $retorno;
}

/**
 * 
 * @global type $FieldCod
 * @param type $FieldId
 * @param type $ReadOnly
 * @param type $Valor_Campo
 * @param type $dadosCampo
 * @return type
 */
function Inicializacao_ext_lista_dominios($FieldId, $ReadOnly, $Valor_Campo, $dadosCampo = "", $tipoCampoFormulario = 't')
{
    global $FieldCod;
    $template = new Template(FILES_ROOT . "assets/templates/t_editcase_fields.html");

    $template->FIELDCODE = $FieldCod;
    $template->FIELD_ID = $FieldId;
    $listaClientes = pegaListaDominios($Valor_Campo);
    if (!$ReadOnly) {
        $template->TIPOCAMPO = $tipoCampoFormulario;
        $template->FIELDTYPE = 'TX';
        $template->FIELDNAME = 'Origem do Cliente';
        $template->ITEM_VALUE = "";
        $template->ITEM_ID = "";
        $template->block("BLOCK_CAMPO_LT_ITEMS");
        foreach ($listaClientes as $cliente) {
            $template->ITEM_SELECTED = "";
            if ($Valor_Campo == $cliente["origem_doc"]) {
                $template->ITEM_SELECTED = "selected";
            }
            $template->ITEM_VALUE = $cliente["origem_doc"];
            $template->ITEM_ID = $cliente["origem_doc"];
            $template->block("BLOCK_CAMPO_LT_ITEMS");
        }
        $template->block("BLOCK_CAMPO_LT");

        
        // se é um campo de formulario inclui a validação de campo
        if ($tipoCampoFormulario !== "") {
            $ValidacaodoCampo = '<script> if (typeof Validacao === "undefined") { var Validacao = new Object(); }' . "\n" . 'Validacao["t' . $FieldId . '"] = ' . " \"ValidaLista($(\\\"#t$FieldId\\\")[0], 'Cliente', 't$FieldId', 0)\";\n</script>";
            $template->VALIDACAO_CAMPO = $ValidacaodoCampo;
            $template->block("BLOCK_CAMPO_SFR");
        }
    } else {
        $template->FIELD_DISPLAY = $Valor_Campo;
        $template->block("BLOCK_CAMPO_READONLY");
    }
    return $template->parse();
}
