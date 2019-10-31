<?php

use raelgc\view\Template;

/*
  function MontaCampoBoleano($templateFields, $Valor_Campo, $ValidacaoCampo, $FieldMask, $Valor_Campo, $ReadOnly)
  {
  switch ($FieldMask) {
  case "checkbox":
  if ($Valor_Campo == "0" | $ValidacaoCampo == "") {
  $templateFields->ITEM_CHECKED = "checked";
  }
  $templateFields->block("BLOCK_CAMPO_BO_CHECKBOX");
  break;
  case "combobox":
  $templateFields->ITEM_SELECTED_TRUE = "";
  $templateFields->ITEM_SELECTED_FALSE = "";
  $templateFields->ITEM_SELECTED_TRUE = "";
  if ($Valor_Campo == "") {
  $templateFields->ITEM_SELECTED_BRANCO = "selected";
  } else {
  if ($Valor_Campo == "0") {
  $templateFields->ITEM_SELECTED_FALSE = "selected";
  } else {
  $templateFields->ITEM_SELECTED_TRUE = "selected";
  }
  }
  $templateFields->ARIA_REVERSO = false;
  $templateFields->block("BLOCK_CAMPO_BO_COMBOBOX");
  break;
  case "button":
  if ($Valor_Campo == "0" | $Valor_Campo == "") {
  $templateFields->CAPTION_BO_BUTTON = "Não";
  $templateFields->CLASS_BO_BUTTON = "btn-danger";
  $templateFields->CHECK_VALOR = "0";
  } else {
  $templateFields->CAPTION_BO_BUTTON = "Sim";
  $templateFields->CLASS_BO_BUTTON = "btn-success";
  $templateFields->CHECK_VALOR = "1";
  }
  $templateFields->ARIA_CODE = $ExtendProps;
  $templateFields->block("BLOCK_CAMPO_BO_BUTTON");
  break;
  case "button_reverse":
  if ($Valor_Campo == "0" | $Valor_Campo == "") {
  $templateFields->CAPTION_BO_BUTTON = "Não";
  $templateFields->CLASS_BO_BUTTON = "btn-success";
  $templateFields->CHECK_VALOR = "0";
  } else {
  $templateFields->CAPTION_BO_BUTTON = "Sim";
  $templateFields->CLASS_BO_BUTTON = "btn-danger";
  $templateFields->CHECK_VALOR = "1";
  }
  $templateFields->ARIA_REVERSO = true;
  $templateFields->ARIA_CODE = $ExtendProps;
  $templateFields->block("BLOCK_CAMPO_BO_BUTTON");
  break;

  case "check_reverse":
  if ($Valor_Campo == "0" | $Valor_Campo == "") {
  $templateFields->CLASS_BO_BUTTON = "btn-success";
  $templateFields->CLASS_BO_ICON = "fa-check";
  $templateFields->CHECK_VALOR = "0";
  } else {
  $templateFields->CLASS_BO_BUTTON = "btn-danger";
  $templateFields->CLASS_BO_ICON = "fa-close";
  $templateFields->CHECK_VALOR = "1";
  }
  $templateFields->ARIA_REVERSO = "check";
  $templateFields->ARIA_CODE = $ExtendProps;
  $templateFields->block("BLOCK_CAMPO_BO_CHECK");
  break;

  case "check":
  if ($Valor_Campo == "0" | $Valor_Campo == "") {
  $templateFields->CLASS_BO_BUTTON = "btn-danger";
  $templateFields->CLASS_BO_ICON = "fa-close";
  $templateFields->CHECK_VALOR = "0";
  } else {
  $templateFields->CLASS_BO_BUTTON = "btn-sucess";
  $templateFields->CLASS_BO_ICON = "fa-check";
  $templateFields->CHECK_VALOR = "1";
  }
  $templateFields->ARIA_REVERSO = "true";
  $templateFields->ARIA_CODE = $ExtendProps;
  $templateFields->block("BLOCK_CAMPO_BO_CHECK");
  break;
  }
  if ($ReadOnly) {
  $templateFields->BUTTON_DISABLED = "disabled";
  }
  }
 */

function MontaFormularioCabecalho($ProcId, $CaseNum, $ListaCampos)
{
    $t_body = new Template(FILES_ROOT . "assets/templates/t_editcase_wrapp_fields.html");
    $t_body->addFile(CAMPOSFORMULARIO, FILES_ROOT . "assets/templates/t_editcase_fields.html");
    foreach ($ListaCampos as $linha) {
        if ($linha["fieldtype"] == "RD") {
            continue;
        }
        $t_body->PRE_FIELDID = "t";
        $t_body->FIELD_ID = $linha["fieldid"];
        $t_body->FIELD_NAME = $linha["fieldname"];
        $t_body->FIELDCODE = $linha["FieldCod"];
        $t_body->FIELD_VALUE = PegaValorCampo($ProcId, $CaseNum, $linha["fieldid"], $linha["fieldtype"], true);
        $t_body->FIELD_VALUEDB = PegaValorCampo($ProcId, $CaseNum, $linha["fieldid"], $linha["fieldtype"], 0);
        $t_body->FIELD_DESC = $linha["fielddesc"];
        if ($linha["fieldtype"] == "STS" | $linha["fieldtype"] == "PRC") {
            $t_body->block("BLOCK_CAMPO_HIDDEN");
            $t_body->FIELD_TYPE = "field";
        } else {
            $t_body->block("BLOCO_CAMPO_CABECALHO");
        }
    }
    $t_body->block("BLOCO_CAMPO_SUBFORM");
    $formularioGerado = $t_body->parse();
    return $formularioGerado;
}

/**
 * 
 * Pega os valores enviados previamente por post para os campos com valor Fixo
 * 
 * @param type $ListaCampos
 * @return type
 */
function pegaValoresCamposFixos($ListaCampos)
{
    $retorno = array();
    foreach ($ListaCampos as $campoAtual) {
        if ($campoAtual["FixarValor"] == 1) {
            $fieldId = $campoAtual["fieldid"];
            $retorno[$fieldId] = $_POST["t" . $fieldId];
        }
    }
    return $retorno;
}

/**
 * 
 * @global boolean $CamposEmTabela
 * @global type $campo
 * @global type $userdef
 * @global type $LockedById
 * @global type $ValidacaoCampos
 * @param type $ProcId
 * @param type $StepId
 * @param type $CaseNum
 * @param type $ListaCampos
 * @param type $Action
 * @param type $SubFormulario
 * @param type $AdminForm
 * @param type $FormsPai
 * @param type $classFormularioSubistitura
 * @return string
 */
function MontaFormulario($ProcId, $StepId, $CaseNum, $ListaCampos, $Action, $SubFormulario = false, $AdminForm = false, $FormsPai = "", $classFormularioSubistitura = "", $tipoLabelCampo = "label")
{
    global $CamposEmTabela, $LockedById, $ValidacaoCampos, $FieldCod;
    $CamposEmTabela = true;

    $NomeTemplate = "$StepId";

    if (!defined("TEMPLATE_FIELDS")) {
        define("TEMPLATE_FIELDS", 't_editcase_fields.html');
    }

    if (!defined("TEMPLATE_WRAP_FIELDS")) {
        define("TEMPLATE_WRAP_FIELDS", 't_editcase_wrapp_fields.html');
    }

    $templateFieldsVirgem = new Template(FILES_ROOT . "assets/templates/" . TEMPLATE_FIELDS);
    $t_templateMasterOrigiginal = new Template(FILES_ROOT . "assets/templates/" . TEMPLATE_WRAP_FIELDS);

    $t_templateMaster = unserialize(serialize($t_templateMasterOrigiginal));
    $Todosreadonly = 1;

    // $Contador = 0; // Comentado não parece ter uso
    // <editor-fold defaultstate="collapsed" desc="comment">
    $valoresCampos = PegaValorCampos($CaseNum);

    /**
     *  Se o valor retornado é null, tenta pegar os valores dos campos que tem valor fixos
     */
    if ($valoresCampos == null) {
        $valoresCampos = pegaValoresCamposFixos($ListaCampos);
    }
    // </editor-fold>



    $ValidacaoCampos = '';
    $PrimeiroCampo = true;
    $retorno = array();

    foreach ($ListaCampos as $linha) {

        /// <editor-fold defaultstate="collapsed" desc="Cria o template para cada campo">

        $templateFields = unserialize(serialize($templateFieldsVirgem));

        // </editor-fold>
        /// <editor-fold defaultstate="collapsed" desc="Trata formulario somente leitura (VIEW)">
        if ($Action == "view") {
            $linha["readonly"] = 1;
            $PrimeiroCampo = false;
        }
        // </editor-fold>
        // <editor-fold defaultstate="collapsed" desc="Popula campos nas variaveis">
        $FieldId = $linha["fieldid"];
        $FieldName = htmlentities($linha["fieldname"]);
        $size = $linha["fieldlength"];
        $ReadOnly = $linha["readonly"];
        $FieldDesc_TB = $linha["fielddesc"];
        $FieldType = $linha["fieldtype"];
        $optional = $linha["optional"];
        $FieldCod = $linha["FieldCod"];

        $FieldDesc = str_replace('"', '&#8220;', $FieldDesc_TB);


        // Verifica se � o primeiro campo do Formul�rio
        if ($ReadOnly == 0) {
            $PrimeiroCampo = $PrimeiroCampo && true;
        }

        $ReadOnlyFilhos = 1;
        $CampoReadOnly = "";
        if ($ReadOnly == 0 or $ReadOnly == 2) {
            $ReadOnlyFilhos = 0;
        }
        if (($ReadOnly == 0 or $ReadOnly == 3 or ( $ReadOnly == 2 and $Valor == FALSE)) and $Action == "Edit" and $LockedById == 0) {
            if ($optional == 0) {
                //$ClassStatus = "NameObrigatory";
                $ClassStatus = "has-warning";
                //$StiloObjeto="style='border :solid 1; border-color:gray; '";
            } else {
                //$ClassStatus = "NameOptional";
                $ClassStatus = "has-feedback";
                //$StiloObjeto="style='border: solid 1; border-color:gray'";
            }
            $CampoReadOnly = "";
            $TipoCampoReadOnly = "t";
        } else {
            //$ClassStatus = "NameReadOnly";
            $ClassStatus = "has-feedback";
            //$StiloObjeto="style='border:solid 1; border-color:gray;  '";
            $CampoReadOnly = "readonly";
            $ReadOnly = 1;
            $ScriptInOut = '';
            $TipoCampoReadOnly = "r";
        }

        // Se � o Primeiro Campo, seta o Foco	
        if ($PrimeiroCampo) {
            $ScriptPrimeiroCampo = "<script language=\"javascript\">onLoad = setaFocusPrimeiroCampo($TipoCampoReadOnly$FieldId)</script>";
            $PrimeiroCampo = false;
        }

        $Todosreadonly = 0;
// </editor-fold>

        /**
         *   Define que para campos com BADGE (DATA) é input group para manter o tamanho de todos os campos igual
         */
        $t_templateMaster->CLASSINPUTGROUP = "input-lg";
        $t_templateMaster->FIELD_NAME = $FieldName;
        $t_templateMaster->CAMPOREADONLY = $CampoReadOnly;
        $t_templateMaster->SIZE = $size;
        $t_templateMaster->FIELD_ID = $FieldId;
        $t_templateMaster->CLASSSTATUS = $ClassStatus;
        $t_templateMaster->FIELD_TYPE = $FieldType;
        $t_templateMaster->FIELDCODE = $FieldCod;

        if ($linha["footertext"] != '') {
            $t_templateMaster->FOOTER_TEXT_FIELD = $linha["footertext"];
            $t_templateMaster->block("BLOCO_FOOTER_FIELD");
        }

        switch ($FieldType) {
            case "SFR":
                $classeFormulario = "col-lg-6";
                $blocoCampos = "BLOCO_CAMPO_NORMAL";
                break;

            case "IF":
            case "IT":
                $classeFormulario = "col-lg-12";
                $blocoCampos = "BLOCO_CAMPO_FORMALIZACAO";
                break;
            case "TD":
                $classeFormulario = "col-lg-4";
                $blocoCampos = "BLOCO_CAMPO_FORMALIZACAO";
                break;

            case "PRC":
            case "STS":
            case "RD":
                $blocoCampos = "BLOCO_CAMPO_SUBFORM";
                break;
            
            case "EXT":
                // Pega os dados do Campo                
                $ExtendProps = $linha["ExtendProp"];
                
                $dadosCampo = json_decode($ExtendProps, true);
                
                if (key_exists("classeFormulario", $dadosCampo))
                {
                    $classeFormulario = $dadosCampo["classeFormulario"];
                } else {
                    $classeFormulario = "col-lg-6";
                }
                $blocoCampos = "BLOCO_CAMPO_NORMAL";
                break;

            default:
                if (defined("CLASS_TAMANHO_CAMPO_FORMULARIO")) {
                    $classeFormulario = CLASS_TAMANHO_CAMPO_FORMULARIO;
                } else {
                    $classeFormulario = "col-lg-6";
                }

                $blocoCampos = "BLOCO_CAMPO_NORMAL";
                break;
        }

        if ($classFormularioSubistitura != "") {
            $classeFormulario = $classFormularioSubistitura;
        }

        $estiloFormulario = ($FieldType != "SFR" & $FieldType != "IF") ? "" : "padding-left:0px";
        //$classeFormulario = "col-md-6";
        $t_templateMaster->CLASSEFORMULARIO = $classeFormulario;
        //$templateMaster->ESTILOFORMULARIO = $estiloFormulario;

        /**
         *  Prepara o controle e insere no Template de Campos
         */
        $retorno = MontaCampoParaFormulario($templateFields, $ProcId, $CaseNum, $valoresCampos, $CampoReadOnly, $Action, $linha, $FormsPai, $AdminForm, $TipoCampoReadOnly, $ClassStatus);
        $t_templateMaster->CAMPOSFORMULARIO = $retorno["campoProcessado"];

        // Determina a tag de LABEL do Campo
        if (!isset($tipoLabelCampo)) {
            $t_templateMaster->TIPO_LABEL_CAMPO = "label";
        } else {
            if ($tipoLabelCampo != "span" & $tipoLabelCampo != "label") {
                $t_templateMaster->TIPO_LABEL_CAMPO = "label";
            } else {
                $t_templateMaster->TIPO_LABEL_CAMPO = $tipoLabelCampo;
            }
        }
        $t_templateMaster->block($blocoCampos);

        /**
         *  Trata a Validaçao do Campo em Javascript
         */
        $ValidacaoCampo .= $retorno["Validacao"];
    }

    $ValidacaoCampos = $ValidacaoCampo . $ValidacaoCampos;

    if ($SubFormulario) {
        //error_log(" " . $t_templateMaster->parse());
        //echo 'Hrrr';
        //echo "--- " . $t_templateMaster->parse();
    }

    $Pagina = $t_templateMaster->parse();
    $retorno_pagina = array();
    $retorno_pagina["CONTEUDO"] = $Pagina;
    //$retorno["CONTEUDO"] = ""; 
    $retorno_pagina["VALIDACAO_JAVASCRIPT"] = $ValidacaoCampos;
    return $retorno_pagina;
}

function MontaCampoParaFormulario($templateFields, $ProcId, $CaseNum, $valoresCampos, $CampoReadOnly, $Action, $linha, $FormsPai, $AdminForm, $TipoCampoReadOnly, $ClassStatus = "has-feedback")
{
    global $userdef;
    $StiloObjeto = "";
    $size = 10;
    $ScriptInOut = "";

    $FieldName = $linha["fieldname"];
    $ReadOnly = $linha["readonly"];

    if ($Action == "View") {
        $ReadOnly = 1;
    }
    $FieldType = $linha["fieldtype"];
    $optional = $linha["optional"];
    $FieldSource = $linha["FieldSourceField"];
    $FieldDisplay = $linha["FieldDisplayField"];
    $FieldTableSource_TB = str_replace("''", "'", $linha["FieldSourceTable"]);
    $FieldTableSource = str_replace('%CaseNum%', $CaseNum, $FieldTableSource_TB);
    $ExtendProps = $linha["ExtendProp"];
    $FieldCod = $linha["FieldCod"];
    $FieldChange = trim($linha["FieldChange"]);
    $FieldKeyMaster = trim($linha["FieldKeyMaster"]);
    $FieldMaster = trim($linha["FieldMaster"]);
    $DefaultValue = $linha["DefaultValue"];
    $FieldId = $linha["fieldid"];
    $Special = $linha["special"];
    $FieldMask = $linha["FieldMask"];
    $FieldLength = $linha["fieldlength"];

    $ValidacaoCampo = ScriptValidacaoCampo($linha, $FormsPai);
    $Valor_Campo = PegaValorCampo($ProcId, $CaseNum, $FieldId, $FieldType, 0, 1, $valoresCampos, $DefaultValue);
    /*
      if (!is_array($Valor_Campo)) {
      $Valor_Campo = utf8_decode($Valor_Campo);
      }
     * 
     */
    if ($Special == 1) {
        switch ($FieldType) {
            case "US":
                if (empty($Valor_Campo)) {
                    $Valor_Campo = $userdef->UserId_Process;
                }
                $FieldType = "US";
                $ReadOnly = 1;
                break;
        }
    }

    $templateFields->FIELD_ID = $FieldId;
    $templateFields->TIPOCAMPO = $TipoCampoReadOnly;
    $templateFields->PROCID = $ProcId;
    $templateFields->CASENUM = $CaseNum;


    /**
     *  Popula o Template
     */
    switch ($FieldType) {

        // Campo Processo
        // Campo Status Processo
        case "PRC":
            $templateFields->FIELDCOD = $FieldCod;
            $templateFields->FIELD_VALUE = $Valor_Campo;
            $templateFields->FIELD_ID = $FieldId;
            $templateFields->block("BLOCK_CAMPO_HIDDEN");
            break;

        case "STS":
            $templateFields->FIELDCOD = $FieldCod;
            $templateFields->FIELD_VALUE = $Valor_Campo;
            $templateFields->FIELD_ID = $FieldId;
            $templateFields->block("BLOCK_CAMPO_HIDDEN");
            break;


        // Campo Tipo Documento
        case "TD":
            $templateFields->FIELDCOD = $FieldCod;
            if ($Valor_Campo == "0" | $ValidacaoCampo == "") {
                $templateFields->ITEM_CHECKED = "checked";
            }
            $templateFields->FIELD_NAME = $FieldName;
            $templateFields->block("BLOCK_CAMPO_TD");
            break;

        // Campo Imagem para FOrmalizacao
        case "IT":
            $contadorDocumentos = 1;
            
            foreach ($Valor_Campo as $documento) {
                // Seta o Tipo do Botão
                $templateFields->CLASSE_BOTAO = "btn-success";

                // Pega os dados Extendidos do Campo
                $aExtendData = json_decode($documento["FieldValue"], true);


                // Grava o ValueId do DOCUMENTO
                $ValueId = $documento["ValueId"];
                $aExtendData["ValueId"] = $ValueId;
                $jsonExtendData = json_encode($aExtendData);

                $templateFields->TIPO_DOCUMENTO = $aExtendData["tipoDocumento"];

                $fieldName = $aExtendData["fileNameStorage"];

                $urlImagem = "viewfile/$ProcId/$CaseNum/$FieldId/$fieldName";
                if ($contadorDocumentos == 1) {
                    $URL_PRIMEIRO = $urlImagem;
                    $templateFields->CLASSE_BOTAO = "btn-primary";
                }
                $templateFields->CONTEUDO_DOCUMENTO = htmlspecialchars($jsonExtendData);
                $templateFields->NRDOCUMENTO = $contadorDocumentos;


                $extencaoArquivo = pathinfo($fieldName, PATHINFO_EXTENSION);

                // Se for um PDF gera o SCRIPT para Atualizar a imagem
                if ($extencaoArquivo != "pdf") {
                    $templateFields->URL_IMAGEM_SMALL = $urlImagem;
                } else {
                    $templateFields->URL_IMAGEM_SMALL = "";

                    // Monta a URL completa para o arquivo PDF
                    $endImagem = SITE_ROOT . SITE_FOLDER . $urlImagem;
                    $templateFields->URL_PDF = $endImagem;

                    $templateFields->block("BLOCO_JS_IMAGEM_PDF");
                }
                $templateFields->block("BLOCO_IMAGENS_SMALL");
                $contadorDocumentos++;
            }
            $templateFields->URL_IMAGEM = $URL_PRIMEIRO;
            $templateFields->block("BLOCK_CAMPO_IT");
            break;

        // Campo para VISUALIZACAO DE IMAGENS    
        case "IF":
            $contadorDocumentos = 1;
            $primeiraImagem = true;
            $templateFields->addfile("CONTROLES_IMAGEM", FILES_ROOT . "assets/templates/t_campo_if_controles.html");
            $arquivosLidos = array();
            foreach ($Valor_Campo as $documento) {

                // Seta o Tipo do Botão
                $jsonExtendData = $documento["FieldValue"];
                $aExtendData = json_decode($jsonExtendData, true);
                $fieldName = $aExtendData["fileNameStorage"];
                
                $originalFileName = $aExtendData["fileName"];
                
//                // Verifica se o arquivo já não foi carregado
//                if (in_array($originalFileName, $arquivosLidos))
//                {
//                    continue;
//                }
                
                $arquivosLidos[] = $originalFileName;
                $urlImagemBotao = "viewfile/$ProcId/$CaseNum/$FieldId/$fieldName";
                if ($contadorDocumentos == 1) {
                    $URL_PRIMEIRO = $urlImagemBotao;
                    $templateFields->CLASSE_BOTAO = "btn-default";
                }

                $templateFields->CLASSE_BOTAO = "btn-default";

                $extencaoArquivo = pathinfo($fieldName, PATHINFO_EXTENSION);
                
                if ($extencaoArquivo == 'pdf') {
                    $templateFields->NRDOCUMENTO = "$contadorDocumentos";
                    $templateFields->URL_IMAGEM_BOTAO = "";

                    // Monta a URL completa para o arquivo PDF
                    $endImagem = SITE_ROOT . SITE_FOLDER . $urlImagemBotao;
//                    $endImagem = $urlImagemBotao;
                    $templateFields->URL_PDF = $endImagem;

                    $templateFields->block('BLOCO_JS_IMAGEM_PDF_IF');
                } else {
                    $templateFields->NRDOCUMENTO = "1_IMG$contadorDocumentos";
                    $templateFields->URL_IMAGEM_BOTAO = $urlImagemBotao;
                }

                $templateFields->IMAGE_ONLOAD = "onload=\"jsHabilitaBotaoImagem('IMG1_IMG$contadorDocumentos', 'btn-success')\"";
                if ($primeiraImagem) {
                    $templateFields->IMAGE_ONLOAD = "onload=\"CarregaPrimeiraImagemFormalizacao()\"";
                    //$templateFields->block("BLOCK_JS_PRIMEIRA_IMAGEM");
                    $primeiraImagem = false;
                }
                
                $templateFields->NR_IMAGEM = $contadorDocumentos;
                if ($extencaoArquivo != 'pdf') {
                    $templateFields->block('BLOCO_CONTROLES_IMAGEM');
                }
                $templateFields->block("BLOCO_BOTOES_DOCUMENTOS");
                $contadorDocumentos++;
            }
            $templateFields->URL_IMAGEM = $URL_PRIMEIRO;
            $templateFields->addfile(IMAGEM_FORMALIZACAO, FILES_ROOT . "assets/templates/t_view_imagem_formalizacao.html");
            $templateFields->CLASSE_COLUNA_IMAGEM = "col-lg-12";
            $templateFields->block("BLOCK_CAMPO_IF");

            break;
        case "TL": // Texto Livre
            $templateFields->READONLY = $CampoReadOnly;
            $templateFields->FIELD_VALUE = $Valor_Campo;
            $templateFields->block("BLOCK_CAMPO_TL");
            break;

        case "ATX_":
        case "ANU":
            $AdicionarCampo = " <a href=\"javascript:void(0)\" onclick=\"JScriaNovoCampoArray($FieldId)\"  tabindex=\"-1\">Acrescentar Campo</a><br>";
            $pagina = "<div id=\"DivConteudot$FieldId\">";
            $SCampo = "<input class=\"EditCaseText\" type='text' name='t$FieldId" . "[]' $CampoReadOnly $StiloObjeto size=$size maxlength='$size' value='%Valor%' $ScriptInOut >";
            $ACampos[$FieldId] = str_replace('%Valor%', "", $SCampo) . "<br>";
            if (empty($Valor_Campo)) {
                $pagina .= str_replace('%Valor%', $Valor_Campo, $SCampo);
                if (!$ReadOnly) {
                    $pagina .= $AdicionarCampo;
                }
            } else {
                $valores = explode(',', $Valor);
                if (is_array($valores)) {
                    for ($i = 0; $i < count($valores); $i++) {
                        if (empty($valores[$i])) {
                            continue;
                        }
                        $pagina .= str_replace('%Valor%', $valores[$i], $SCampo);
                        if ($i > 0) {
                            $pagina .= "<br>";
                        } else {
                            if (!$ReadOnly) {
                                $pagina .= $AdicionarCampo;
                            } else {
                                $pagina .= "<br>";
                            }
                        }
                    }
                }
            }
            $pagina .= "</div>";
            $templateFields->CONTEUDO_CAMPO = $pagina;
            $templateFields->block("BLOCK_CAMPO_PR");
            break;


        case "ADT":
            echo $CaptionCampo;
            $Valor = ConvDate($Valor);
            $ACampos[$FieldId] = '';
            ob_start();
            ?>
            <input  type="text" name="<?= $TipoCampoReadOnly ?><?= $FieldId ?>" <?= $CampoReadOnly ?> <?= $StiloObjeto ?> size=13 value="<?= $Valor ?>" onKeypress="FormataData(this, event)" <?= $ScriptInOut ?>> 
            <?php
            if (!$ReadOnly) {
                include("DynCalendarFunc.php");
            }
            $pagina = ob_get_contents();
            ob_clean();
            $templateFields->CONTEUDO_CAMPO = $pagina;
            $templateFields->block("BLOCK_CAMPO_PR");
            break;

        case "HR":
            $templateFields->SIZE = $size;
            $templateFields->FIELD_VALUE = $Valor_Campo;
            if ($ReadOnly) {
                $templateFields->block("BLOCK_CAMPO_READONLY");
            } else {
                $templateFields->block("BLOCK_CAMPO_HR");
            }
            break;

        // _AR
        case "AR":
        case "LF":
            switch ($FieldType) {
                case "LF":
                    $acceptTipoArquivo = ".zip";
                    break;
                case "DS":
                    $acceptTipoArquivo = ".docx,.doc,.pdf";
                    break;
                default:
                    $acceptTipoArquivo = $DefaultValue;
                    break;
            }

            //$Valor_Campo            
            $dirName = FILES_UPLOAD . "/" . formataCaseNum($ProcId, 6) . "/" . formataCaseNum($CaseNum, 10) . "/" . formataCaseNum($FieldId, 6) . "/";
            $jsonExtendData = $Valor_Campo;
            $aExtendData = json_decode($jsonExtendData, true);
            $fileNameStorage = "";
            $fileName = "";
            if (is_array($aExtendData)) {
                if (key_exists("fileNameStorage", $aExtendData)) {
                    $fileNameStorage = $dirName . $aExtendData["fileNameStorage"];
                }
                if (key_exists("fileName", $aExtendData)) {
                    $fileName = $aExtendData["fileName"];
                }
            }
            if (empty($fileName)) {
                $templateFields->EXCLUIRDESABILITADO = "disabled";
                $templateFields->ABRIRDESABILITADO = "disabled";
            }
            $templateFields->FILE_NAME = $aExtendData["fileNameStorage"];
            $templateFields->FILE_DATA = $Valor_Campo;
            $templateFields->FIELD_VALUE = $fileName;
            if (!$ReadOnly) {
                $templateFields->block("BLOCK_CAMPO_AR_EDICAO");
            }
            $templateFields->ACCEPT_TIPOARQUIVOS = $acceptTipoArquivo;
            $templateFields->block("BLOCK_CAMPO_AR");
            break;

        // Arquivo de Repositorio
        case "FRP":
            $idRepositorio = pegaRepositorio($ProcId);
            $codigoDocumento = $FieldSource;

            $dadosArquivo = PegaValorCampo($idRepositorio, $Valor_Campo, 'IMAGENS_INT');
            $fileName = "";
            if ($dadosArquivo !== "") {
                $jsonExtendData = $dadosArquivo;
                $aExtendData = json_decode($jsonExtendData, true);
                $fileNameStorage = "";
                $fileName = "";
                if (is_array($aExtendData)) {
                    if (key_exists("fileNameStorage", $aExtendData)) {
                        $fileNameStorage = $dirName . $aExtendData["fileNameStorage"];
                    }
                    if (key_exists("fileName", $aExtendData)) {
                        $fileName = $aExtendData["fileName"];
                    }
                }
            }
            if (empty($fileName)) {
                $templateFields->EXCLUIRDESABILITADO = "disabled";
                $templateFields->ABRIRDESABILITADO = "disabled";
            }
            $templateFields->PROC_REPOSITORIO = $idRepositorio;
            $templateFields->FIELD_DOC = 'IMAGENS_INT';
            $templateFields->FILE_NAME = $aExtendData["fileNameStorage"];
            $templateFields->FILE_DATA = htmlentities($dadosArquivo);
            $templateFields->NOME_ARQUIVO = $fileName;
            $templateFields->FIELD_VALUE = $Valor_Campo;
            $templateFields->FIELDCODE = $FieldCod;
            $templateFields->FIELDTYPE = $FieldType;
            $templateFields->DOCUMENTO_REPOSITORIO = $codigoDocumento;

            if (!$ReadOnly) {
                $templateFields->block("BLOCK_CAMPO_FRP_EDICAO");
            }
            $templateFields->ACCEPT_TIPOARQUIVOS = $acceptTipoArquivo;
            $templateFields->block("BLOCK_CAMPO_FRP");
            break;

        case "TB":
            $valoresTabela = MontaCampoTabela_TEMPLATE($ReadOnly, $Valor_Campo, $FieldSource, $FieldDisplay, $FieldTableSource, $FieldChange, $FieldKeyMaster, '', $FieldMaster);
            if ($ReadOnly != 1) {
                $templateFields->FIELD_VALUE = $valoresTabela[0]["display"];
                $templateFields->block("BLOCK_CAMPO_READONLY");
            } else {
                for ($i = 0; $i < count($valoresTabela) - 1; $i++) {
                    $templateFields->VALOR = $valoresTabela["id"];
                    $templateFields->DISPLAY = $valoresTabela["display"];
                    $templateFields->SELECTED = $valoresTabela["sel"];
                    $templateFields->block("BLOCK_CAMPO_TB_ITENS");
                }
            }
            break;

        case "MC":
            $CampoTabela = MontaCampoMultiValue($ReadOnly, $StiloObjeto, $Valor_Campo, $FieldSource, $FieldDisplay, $FieldTableSource, "CheckBox");
            $templateFields->CONTEUDO_CAMPO = $CampoTabela;
            $templateFields->block("BLOCK_CAMPO_PR");
            break;

        case "MA":
            $CampoTabela = MontaCampoMultiValue($ReadOnly, $StiloObjeto, $Valor_Campo, $FieldSource, $FieldDisplay, $FieldTableSource, "TextArea");
            $templateFields->CONTEUDO_CAMPO = $CampoTabela;
            $templateFields->block("BLOCK_CAMPO_PR");
            break;

        // _TM
        case "TM":
            $ValorMT = MontaTM($ProcId, $CaseNum, $FieldId, $ReadOnly);
            $templateFields->CONTEUDO_CAMPO = $ValorMT;
            $templateFields->block("BLOCK_CAMPO_PR");
            break;

        // _DC
        case "DC":
            $dadosCampoDC = MontaDadosCampoDC($ProcId, $CaseNum, $FieldId, $ReadOnly, $StiloObjeto, $Valor_Campo, $FieldSource, $FieldDisplay, $FieldTableSource);
//            $dadosCampoDC = MontaDadosCampoDC($ProcId, $CaseNum, $FieldId, $ReadOnly, $StiloObjeto, $Valor_Campo, $FieldSource, $FieldDisplay, $FieldTableSource);
            $templateFields->CONTEUDO_CAMPO = $dadosCampoDC;
            $templateFields->block("BLOCK_CAMPO_PR");
            break;

        // _CR
        case "CR":
            $ValorCR = MontaCampoCR($FieldTableSource, $FieldSource, $CaseNum, false, $FieldName);
            $templateFields->CONTEUDO_CAMPO = $ValorCR;
            $templateFields->block("BLOCK_CAMPO_PR");
            break;

        // _FD
        case "MF":
        case "FD":
            //$FD = MontaFD($ProcId, $StepId, $CaseNum, $FieldId, $ReadOnly);
            //$templateFields->CONTEUDO_CAMPO = $FD;
            if ($FieldType == "FD") {
                $templateFields->CLASSEDIVBOTAO = "btn-group";
                $templateFields->MULTIPLOSARQUIVOS = 0;
            } else {
                $templateFields->HIDEBOTAOARQUIVOS = "hide";
                $templateFields->MULTIPLOSARQUIVOS = 1;
            }
            $TotalArquivos = Totalvalores($ProcId, $FieldId, $CaseNum);
            if ($TotalArquivos == 0) {
                $diretorio = FILES_UPLOAD . "/" . str_pad($ProcId, 6, "0", STR_PAD_LEFT) . "-" . str_pad($CaseNum, 8, "0", STR_PAD_LEFT);
                $TotalArquivos = ContaArquivosDiretorio($diretorio);
            }
            $templateFields->NR_ARQUIVOS = $TotalArquivos;
            if ($ReadOnly) {
                $templateFields->CLASS_BUTTON_FD = "btn-default";
                $templateFields->FD_READONLY = 1;
            } else {
                $templateFields->CLASS_BUTTON_FD = "btn-warning";
                $templateFields->FD_READONLY = 0;
            }
            $templateFields->FD_FILE_LIST = ListaArquivosCampoImagem($ProcId, $CaseNum, $FieldId, $ReadOnly, "botao", "");
            $templateFields->block("BLOCK_CAMPO_FD");
            break;

        case "ATB":
            $CampoTabela = MontaCampoArrayTabela($ReadOnly, $StiloObjeto, $Valor_Campo, $FieldSource, $FieldDisplay, $FieldTableSource, "", $FieldChange, $FieldKeyMaster, "", $FieldMaster);
            if ($ReadOnly != 1) {
                $CampoTabela = "<div id=\"DivConteudot$FieldId\">" . $CampoTabela . "</div>";
            }
            $templateFields->VALOR_ATB = $CampoTabela;
            $templateFields->block("BLOCK_CAMPO_ATB");
            break;

        case "RU":
//                    MontaCampoRum($ProcId, $StepId, $FieldId, $Valor);
            break;

        case "IFD":
            echo $CaptionCampo;
            if (!$ReadOnly) {
                echo "<a href=\"#\" onclick=\"CarregarImagens($ProcId, $FieldId, '$FieldCod', $CaseNum)\">Carregar Imagens</a> <a href=\"#\">Excluir Imagens</a>";
            }
            echo "<div id=\"Div$FieldId\">";
            include("BpmLoadImagesFromFolder.php");
            echo "</div>";
            echo "<div id=\"DivShowFullImage$FieldId\" class=\"DivFullImage\" style=\"display:none\" ><iframe scr=\"blank.html\" class=\"IframeFullImage\"></iframe><div id=\"DivFullImage$FieldId\" class=\"FullImage\"></div></div>";
            break;

        case "SFR":
            montaCampoSFR($templateFields, $Action, $ProcId, $CaseNum, $FieldId, $FormsPai, $linha, $ReadOnly, $StiloObjeto, $Valor_Campo, $ExtendProps, $AdminForm, $nome, $optional, $ReadOnlyFilhos, $ClassStatus, $Online, "input-group", $ClasseSubtitura);
            break;

        // _SFR
        case "_SFR":
            $Script = "";
            if (!empty($FormsPai)) {
                if (is_array($FormsPai)) {
                    $FormsPai_Deste = $FormsPai;
                } else {
                    $FormsPai_Deste = explode(",", $FormsPai);
                }
            } else {
                $FormsPai_Deste = array();
            }
            array_push($FormsPai_Deste, "t$FieldId");
            $Field = $linha;
            $Field["fieldtype"] = "LT";
            $ValidacaoCampo .= ScriptValidacaoCampo($Field, $FormsPai_Deste);

            $pagina .= "<div class=\"$ClassStatus col-md-6\" style=\"padding-left:0px\" >";
            $pagina .= MontaCampoFormulario($ProcId, $FieldId, $ReadOnly, $StiloObjeto, $Valor_Campo, $ExtendProps, $AdminForm, $FormsPai_Deste, $nome, $optional);
            $pagina .= "</div>";


            if (!$AdminForm) {
                $pagina .= "<!-- Inicio Sub-Formulário -->\n";
                $pagina .= "<div id=\"DivFormulario$FieldId\" class=\"col-md-12\" style=\"margin-top:15px\">\n";
                if ($Valor_Campo != "") {
                    //$StepOld = $StepId;
                    $StepId_Form = $Valor_Campo;
                    $Query = CamposFormulario($ProcId, $StepId_Form);
                    if ($ReadOnlyFilhos == 1) {
                        $Action = "view";
                    }
                    $t_bodyFormulario = new Template(FILES_ROOT . "assets/templates/t_subform.html");
                    $t_bodyFormulario->addFile("CONTEUDO_FIELDS", FILES_ROOT . "assets/templates/t_editcase_wrapp_fields.html");
                    $t_bodyFormulario->addFile("CAMPOSFORMULARIO", FILES_ROOT . "assets/templates/t_editcase_fields.html");
                    $retorno_SFR = MontaFormulario($ProcId, $StepId_Form, $CaseNum, $Query, $Action, True, $AdminForm, $FormsPai_Deste);
                    $ValidacaoCampo .= $retorno_SFR["VALIDACAO_JAVASCRIPT"];
                    $pagina .= $retorno_SFR["CONTEUDO"];
                }
                $pagina .= "</div>\n";
                $pagina .= "<!-- Fim Sub-Formulário -->\n\n";
                if (!$Online) {
                    $pagina .= $Script;
                }
            }
            $templateFields->CONTEUDO_CAMPO = $pagina;
            //$templateFields->CONTEUDO_CAMPO = "Saida Errada";
            $templateFields->block("BLOCK_CAMPO_PR");
            break;

        case "LB":
            $templateFields->block("BLOCK_CAMPO_LB");
            break;

        case "HD":
            $templateFields->FIELD_VALUE = $Valor_Campo;
            $templateFields->block("BLOCK_CAMPO_HD");
            break;

        case "NU":
            $templateFields->FIELD_VALUE = $Valor_Campo;
            if ($ReadOnly) {
                $templateFields->FIELD_DISPLAY = $Valor_Campo;
                $templateFields->block("BLOCK_CAMPO_READONLY");
            } else {
                $templateFields->block("BLOCK_CAMPO_NU");
            }

            break;

        case "LT":
            $Lista = MontaCampoLista($ProcId, $FieldId, $FieldCod, $ReadOnly, $StiloObjeto, $Valor_Campo, $ExtendProps);
            $templateFields->CONTEUDO_CAMPO = $Lista;
            $templateFields->block("BLOCK_CAMPO_PR");
            break;

        case "TX":
            $templateFields->CONTEUDO_CAMPO = montaCampoTX($FieldId, $Valor_Campo, $ReadOnly, $TipoCampoReadOnly, false, $FieldCod, $FieldName, $FieldLength);
            $templateFields->block("BLOCK_CAMPO_TX");
            break;

        case "ATX":
            $valoresCampo = array();
            if ($Valor_Campo !== "") {
                $valoresCampo = json_decode($Valor_Campo);
            } else {
                $valoresCampo[] = "";
            }
            $conteudoCampo = "";
            foreach ($valoresCampo as $Valor_Campo) {
                $conteudoCampo .= montaCampoTX($FieldId, $Valor_Campo, $ReadOnly, $TipoCampoReadOnly, true);
            }
            $templateFields->CONTEUDO_CAMPO = $conteudoCampo;
            $templateFields->block("BLOCK_CAMPO_ATX");
            break;

        case "DT":
            $templateFields->CLASSINPUTGROUP = "input-group";
            $dataFormatada = ConvDate($Valor_Campo, false);
            $templateFields->FIELDCODE = $FieldCod;
            if ($ReadOnly) {
                $templateFields->FIELD_DISPLAY = $dataFormatada;
                $templateFields->block("BLOCK_CAMPO_READONLY");
            } else {
                $templateFields->FIELD_VALUE = $dataFormatada;
                $templateFields->block("BLOCK_CAMPO_DT_CALENDARIO");
                $templateFields->block("BLOCK_CAMPO_DT");
            }
            break;

        case "IM":
            $mobileMode = $_SESSION["mobileMode"];
            if ($mobileMode != 0) {
                $templateFields->CAMPO_IM_ICONE_ANEXAR = "fa-camera";
            } else {
                $templateFields->CAMPO_IM_ICONE_ANEXAR = "fa-paperclip";
            }

            $jsonExtendData = $Valor_Campo;
            $aExtendData = json_decode($jsonExtendData, true);
            $fieldName = $aExtendData["fileNameStorage"];
            $templateFields->STORAGEFILENAME = $fieldName;
            $templateFields->FILENAME = $aExtendData["fileName"];
            $templateFields->FIELD_VALUE = $aExtendData["fileName"];
            if (empty($fieldName)) {
                $templateFields->ABRIRDESABILITADO = "disabled";
                $templateFields->EXCLUIRDESABILITADO = "disabled";
            }
            if (!$ReadOnly) {
                $templateFields->block("BLOCK_SELECT_CAMPO_IM");
            }
            $templateFields->block("BLOCK_CAMPO_IM");
            break;

        // Campo Boleano
        case "BO":
            switch ($FieldMask) {
                case "combobox":
                case "checkbox":
                    $templateFields->ITEM_SELECTED_TRUE = "";
                    $templateFields->ITEM_SELECTED_FALSE = "";
                    $templateFields->ITEM_SELECTED_TRUE = "";
                    if ($Valor_Campo == "") {
                        $templateFields->ITEM_SELECTED_BRANCO = "selected";
                    } else {
                        if ($Valor_Campo == "0") {
                            $templateFields->ITEM_SELECTED_FALSE = "selected";
                        } else {
                            $templateFields->ITEM_SELECTED_TRUE = "selected";
                        }
                    }
                    $templateFields->ARIA_REVERSO = false;
                    $templateFields->block("BLOCK_CAMPO_BO_COMBOBOX");
                    break;
                case "button":
                    if ($Valor_Campo == "0" | $Valor_Campo == "") {
                        $templateFields->CAPTION_BO_BUTTON = "Não";
                        $templateFields->CLASS_BO_BUTTON = "btn-danger";
                        $templateFields->CHECK_VALOR = "0";
                    } else {
                        $templateFields->CAPTION_BO_BUTTON = "Sim";
                        $templateFields->CLASS_BO_BUTTON = "btn-success";
                        $templateFields->CHECK_VALOR = "1";
                    }
                    $templateFields->ARIA_CODE = $ExtendProps;
                    $templateFields->block("BLOCK_CAMPO_BO_BUTTON");
                    break;
                case "button_reverse":
                    if ($Valor_Campo == "0" | $Valor_Campo == "") {
                        $templateFields->CAPTION_BO_BUTTON = "Sim";
                        $templateFields->CLASS_BO_BUTTON = "btn-success";
                        $templateFields->CHECK_VALOR = "0";
                    } else {
                        $templateFields->CAPTION_BO_BUTTON = "Não";
                        $templateFields->CLASS_BO_BUTTON = "btn-danger";
                        $templateFields->CHECK_VALOR = "1";
                    }
                    $templateFields->ARIA_REVERSO = true;
                    $templateFields->ARIA_CODE = $ExtendProps;
                    $templateFields->block("BLOCK_CAMPO_BO_BUTTON");
                    break;
                case "checkbox":
                    if ($Valor_Campo == "1" | $ValidacaoCampo == "") {
                        $templateFields->ITEM_CHECKED = "checked=\"true\"";
                    }
                    $templateFields->block("BLOCK_CAMPO_BO_CHECKBOX");
                    break;

                case "check_reverse":
                    if ($Valor_Campo == "0" | $Valor_Campo == "") {
                        $templateFields->CLASS_BO_BUTTON = "btn-success";
                        $templateFields->CLASS_BO_ICON = "fa-check";
                        $templateFields->CHECK_VALOR = "0";
                    } else {
                        $templateFields->CLASS_BO_BUTTON = "btn-danger";
                        $templateFields->CLASS_BO_ICON = "fa-close";
                        $templateFields->CHECK_VALOR = "1";
                    }
                    $templateFields->ARIA_REVERSO = "check";
                    $templateFields->ARIA_CODE = $ExtendProps;
                    $templateFields->block("BLOCK_CAMPO_BO_CHECK");
                    break;

                case "check":
                    if ($Valor_Campo == "0" | $Valor_Campo == "") {
                        $templateFields->CLASS_BO_BUTTON = "btn-danger";
                        $templateFields->CLASS_BO_ICON = "fa-close";
                        $templateFields->CHECK_VALOR = "0";
                    } else {
                        $templateFields->CLASS_BO_BUTTON = "btn-sucess";
                        $templateFields->CLASS_BO_ICON = "fa-check";
                        $templateFields->CHECK_VALOR = "1";
                    }
                    $templateFields->ARIA_REVERSO = "true";
                    $templateFields->ARIA_CODE = $ExtendProps;
                    $templateFields->block("BLOCK_CAMPO_BO_CHECK");
                    break;
            }
            $templateFields->FIELDCODE = $FieldCod;
            if ($ReadOnly) {
                $templateFields->BUTTON_DISABLED = "disabled";
            }
            break;

        // Campo GRUPO
        case "GR":
            $Grupos = PegaListaGruposParaCampo($Valor_Campo, $ReadOnly);

            if ($ReadOnly) {
                for ($i = 0; $i < count($Grupos); $i++) {
                    if ($Grupos[$i]["Sel"] == "selectec") {
                        $templateFields->FIELD_DISPLAY = $Grupos[$i]["Nome"];
                        $templateFields->block("BLOCK_CAMPO_READONLY");
                        break;
                    }
                }
            } else {
                
            }

            if (!$ReadOnly) {
                $templateFields->CONTEUDO_CAMPO = MontaCampoGR($Grupos, $FieldId, $optional);
                $templateFields->block("BLOCK_CAMPO_GR");
            }
            break;

        // Campo Link
        case "LK":
            $Valor = ReplaceFieldsValues($Valor_Campo);
            $templateFields->URL_CAMPO = $Valor;
            $templateFields->block("BLOCK_CAMPO_LK");
            break;

        // Campo Usuário
        case "US": // TODO: Consertar Seleção de Usuário
            $Valor = montaCampoUS($FieldId, $Valor_Campo, $ReadOnly, "", "", $optional);
            $templateFields->CONTEUDO_CAMPO = $Valor;
            $templateFields->block("BLOCK_CAMPO_PR");
            break;

        case "SCC":
            $CampoSignatarios = montaCampoSignatarios($FieldId, $Valor_Campo, $ReadOnly, $optional);
            $templateFields->CONTEUDO_CAMPO = $CampoSignatarios;
            $templateFields->block("BLOCK_CAMPO_PR");
            break;

        case "EXT":
            $CampoExtendido = montaCampoEX($FieldId, $Valor_Campo, $ReadOnly, $optional, $ExtendProps);
            
            if (is_array($CampoExtendido))
            {
                $HTMLCampoExtendido = $CampoExtendido["html"];
            } else {
                $HTMLCampoExtendido = $CampoExtendido;
            }
            $templateFields->CONTEUDO_CAMPO = $HTMLCampoExtendido;
            $templateFields->block("BLOCK_CAMPO_PR");
            break;
    }

    $retorno["Validacao"] = $ValidacaoCampo;
    $retorno["campoProcessado"] = $templateFields->parse();
    return $retorno;
}
