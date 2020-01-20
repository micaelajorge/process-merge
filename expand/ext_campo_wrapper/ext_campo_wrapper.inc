<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 30/10/2019
  Sistema: Process_XAMPP
 */

use raelgc\view\Template;

function pegaListaCamposWrapp($StepCode)
{
    global $ProcId, $connect;
    
    $stepId = PegaStepIdByCode($ProcId, $StepCode);
    
    $sql = "select fieldCod from procfieldsdef, stepfieldsdef where procfieldsdef.procid = $ProcId and stepfieldsdef.ProcId = procfieldsdef.procid and stepid = $stepId and procfieldsdef.fieldid = stepfieldsdef.FieldId";
    $query = mysqli_query($connect, $sql);
    $resultado = mysqli_fetch_all($query);
    
    return $resultado;
}

function inicializacao_ext_campo_wrapper($FieldId, $ReadOnly, $Valor_Campo, $dadosCampo)
{
    global $dadosCampo, $FieldCod;
    $t_body_field = new Template(FILES_ROOT . "expand/ext_campo_wrapper/t_ext_campo_wrapper.html");
    $t_body_field->FIELDCODE = $FieldCod;
    $t_body_field->FIELDID = $FieldId;
    $t_body_field->FIELD_ID = $FieldId;
    $t_body_field->VALOR_CAMPO = htmlentities($Valor_Campo);
    
    if (key_exists("listaCampos", $dadosCampo))
    {
        $listaCampos = $dadosCampo["listaCampos"];
    } else {
        $listaCampos = pegaListaCamposWrapp($dadosCampo["StepCode"]);
    }
    
    $t_body_field->BOX_TOOL_SIGNAL = "fa-minus";
    if (key_exists("box_type", $dadosCampo))
    {
        if ($dadosCampo["box_type"] === "box-collapsed")
        {
         $t_body_field->BOX_COLAPSED = "collapsed-box";        
         $t_body_field->BOX_TOOL_SIGNAL = "fa-plus";
        }
    }
    
    $separador = '';
    foreach ($listaCampos as $campo)
    {
        if (is_array($campo))
        {
          $campo = $campo[0];   
        }    
        $listaCamposWrap .= "$separador '$campo'";
        $separador = ', ';
    }
    
    $t_body_field->LISTA_CAMPOS_WRAPP = $listaCamposWrap;
    $retorno["html"] = $t_body_field->parse();
    return $retorno;
}
