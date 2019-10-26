<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 26/07/2019
  Sistema: Process_XAMPP
 */

use raelgc\view\Template;

/**
 * 
 * @param type $FieldId
 * @param type $ReadOnly
 * @param type $Valor_Campo
 * @param type $dadosCampo
 * @param type $tipoCampoFormulario
 */
function Inicializacao_ext_tipo_assinatura($FieldId, $ReadOnly, $Valor_Campo, $dadosCampo = "", $tipoCampoFormulario = 't')
{
    global $FieldCod, $ext_tipo_assinatura_javascript_loaded;
    $template = new Template(FILES_ROOT . "assets/templates/t_ext_tipo_assinatura.html");
    $template->FIELDCODE = $FieldCod;
    $template->FIELD_ID = $FieldId;

    // Busca de arquivo a lista de tipos de assinatura permitidos
    $nomeArquivoConfig = FILES_ROOT . "expand/lista_assinaturas/tipos_assinaturas.json";
    $json_tipo_assinaturas = file_get_contents($nomeArquivoConfig);

    $tiposAssinatura = json_decode($json_tipo_assinaturas, true);

    
    $valorCampo = htmlentities($Valor_Campo, ENT_QUOTES);
    if ($Valor_Campo != "")
    {
        $tiposSelecionados = json_decode($Valor_Campo, true);
    } else {
        $valorCampo = "[]";
    }
    $template->FIELD_VALUE = $valorCampo;
    
    // Verifica se o Campo é Somente Leitura
    if ($ReadOnly)
    {
        $template->TIPO_CAMPO = "r";        
        $block_display = "BLOCK_VIEW_TIPOS_ASSINATURA";
    } else {
        $template->TIPO_CAMPO = "t";
        $block_display = "BLOCK_SEL_TIPOS_ASSINATURA";
    }   
    
    $template->FIELD_CODE = $FieldCod;
    foreach ($tiposAssinatura as $tipoAssinatura) {
        $template->TIPO_CHECKED = "";
        $template->TIPO_BADGE = "bg-gray";
        if (in_array($tipoAssinatura["valor"], $tiposSelecionados))
        {
            $template->TIPO_CHECKED = "checked";
            $template->TIPO_BADGE = "bg-light-blue";
        }
        $template->ASSING_TIPO = $tipoAssinatura["valor"];
        $template->ASSING_DESC = $tipoAssinatura["descricao"];
        $template->block($block_display);
    }

    // verifica se já não foi carregado o arquivo javascript
    if (!$ext_tipo_assinatura_javascript_loaded) {
        $template->ARQUIVO_JS = "ext_tipo_assinatura.js";
        $template->block("BLOCO_JAVASCRIPT");
        $ext_tipo_assinatura_javascript_loaded = true;
    }

    // Retorna os dados para a geração da página
    $retorno = $template->parse();
    return $retorno;
}
