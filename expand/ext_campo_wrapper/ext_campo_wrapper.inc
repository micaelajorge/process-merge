<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 30/10/2019
  Sistema: Process_XAMPP
 */

use raelgc\view\Template;

function inicializacao_ext_campo_wrapper($FieldId, $ReadOnly, $Valor_Campo, $dadosCampo)
{
    global $dadosCampo, $FieldCod;
    $t_body_field = new Template(FILES_ROOT . "expand/ext_campo_wrapper/t_ext_campo_wrapper.html");
    $t_body_field->FIELDCODE = $FieldCod;
    $t_body_field->FIELDID = $FieldId;
    $t_body_field->FIELD_ID = $FieldId;
    $t_body_field->VALOR_CAMPO = htmlentities($Valor_Campo);
    
    $listaCampos = "[";
    $separador = '';
    foreach ($dadosCampo["listaCampos"] as $campo)
    {
        $listaCampos .= "$separador '$campo'";
        $separador = ', ';
    }
    $listaCampos .= "]";
    
    $t_body_field->LISTA_CAMPOS_WRAPP = $listaCampos;
    $retorno["html"] = $t_body_field->parse();
    return $retorno;
}
