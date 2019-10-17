<?php

include_once(FILES_ROOT . "include/func_api.inc");

use raelgc\view\Template;

function pegaListaClientes($origemSelecionada)
{
    global $userdef, $connect;
    $origemUsuario = $userdef->Origem;

    $procId = PegaProcIdByCode("GATEWAY_DIGITAL_CLIENTES");

    $fieldId = PegaFieldIdByCode($procId, "NOME_CLIENTE");

    $sql = "select exportkeys.casenum, CampoDisplay$fieldId as nome_cliente, exportkeys.origem from exportkeysdisplay, exportkeys, origemdominio where origemdominio.origem_user = '$origemUsuario' and exportkeys.Origem = origemdominio.Origem_Doc   and exportkeys.ProcId = exportkeysdisplay.procid and exportkeys.procid = $procId and exportkeys.casenum = exportkeysdisplay.casenum";
    if ($origemSelecionada !== '' & $origemSelecionada !== null) {
        $sql .= " and exportkeys.origem = '$origemSelecionada'";
    }
    $sql .= " order by CampoDisplay$fieldId";
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
function Inicializacao_gateway_clientes($FieldId, $ReadOnly, $Valor_Campo, $dadosCampo)
{
    global $FieldCod;
    $template = new Template(FILES_ROOT . "assets/templates/t_editcase_fields.html");

    $template->FIELDCODE = $FieldCod;
    $template->FIELD_ID = $FieldId;
    $listaClientes = pegaListaClientes($Valor_Campo);
    if (!$ReadOnly) {              
        $template->TIPOCAMPO = 't';
        $template->FIELDTYPE = 'TX';
        $template->FIELDNAME = 'CLIENTE';
        $template->ITEM_VALUE = "";
        $template->ITEM_ID = "";
        $template->block("BLOCK_CAMPO_LT_ITEMS");
        foreach ($listaClientes as $cliente) {
            $template->ITEM_SELECTED = "";
            if ($Valor_Campo == $cliente["origem"])
            {
                $template->ITEM_SELECTED = "selected";
            }
            $template->ITEM_VALUE = $cliente["nome_cliente"];
            $template->ITEM_ID = $cliente["origem"];
            $template->block("BLOCK_CAMPO_LT_ITEMS");
        }
        $template->block("BLOCK_CAMPO_LT");
        
        $ValidacaodoCampo = '<script> if (typeof Validacao === "undefined") { var Validacao = new Object(); }' . "\n" . 'Validacao["t' . $FieldId . '"] = ' . " \"ValidaLista($(\\\"#t$FieldId\\\")[0], 'Cliente', 't$FieldId', 0)\";\n</script>";        
        
        $template->VALIDACAO_CAMPO = $ValidacaodoCampo;
        $template->block("BLOCK_CAMPO_SFR");
    } else {
        $template->FIELD_DISPLAY = $listaClientes[0]["nome_cliente"];
        $template->block("BLOCK_CAMPO_READONLY");
    }
    return $template->parse();
}
