<?php

include_once(FILES_ROOT . "include/func_api.inc");

use raelgc\view\Template;


/**
 * 
 * @global type $FieldCod
 * @param type $FieldId
 * @param type $ReadOnly
 * @param type $Valor_Campo
 * @param type $dadosCampo
 * @return type
 */
function Inicializacao_voto_cotista($FieldId, $ReadOnly, $Valor_Campo, $dadosCampo)
{
    global $FieldCod;
    $t_body_field = new Template(FILES_ROOT . "expand/voto_cotista/t_voto_cotista.html");
    
    if ($Valor_Campo !== "")
    {
        $dadosVoto = json_decode($Valor_Campo, true);
    }
    
    for ($i = 0; $i  < count($dadosVoto); $i++)
    {
        $t_body_field->ITEM = $i;
        $t_body_field->DESCRICAO_VOTO = $dadosVoto[$i]["descricao"];
        $t_body_field->block("BLOCK_VOTO");
    }
    
    $t_body_field->TOTAL_VOTOS = count($dadosVoto);
    return $t_body_field->parse();
}