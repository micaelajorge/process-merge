<?php

const CR_NRSHOWREFERENCIAS = 4;
const CR_NRSHOWCASOS = 10;


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

use raelgc\view\Template;

/**
 * 
 * @param type $listaGrupos
 * @param type $fieldId
 * @param type $optional
 * @return type
 */
function MontaCampoGR($listaGrupos, $fieldId, $optional = "")
{
    $corIcone = "";
    if ($optional == 0) {
        $corIcone = "text-warning";
    }
    $retorno = MontaCampoTL($listaGrupos, $fieldId, "fa fa-group $corIcone", "Seleção de Grupo");
    return $retorno;
}

/**
 * 
 * @global type $connect
 * @global type $userdef
 * @param type $ProcId
 * @param type $CaseNum
 * @param type $FieldId
 * @param type $ReadOnly
 * @param type $tipoFolderList
 * @return type
 */
function ListaArquivosCampoImagem($ProcId, $CaseNum, $FieldId, $ReadOnly, $tipoFolderList = "", $multipleFiles)
{
    global $connect, $userdef;

    // <editor-fold defaultstate="collapsed" desc="Chamada Classe de Template">


    if ($tipoFolderList == "botao") {
        $t_body = new Template(FILES_ROOT . "assets/templates/t_campo_fd_lista_arquivos_botao.html");
    } else {
        $t_body = new Template(FILES_ROOT . "assets/templates/t_campo_fd_lista_arquivos.html");
    }
    if ($multipleFiles) {
        $t_body->HIDECAMERA = "hide";
    }

// </editor-fold>

    $t_body->MAXFILESIZE = ini_get('upload_max_filesize');
    $t_body->PROCID = $ProcId;
    $t_body->CASENUM = $CaseNum;
    $t_body->FIELDID = $FieldId;
    $t_body->USERID = $userdef->UserId;
    $t_body->MULTIPLEFILES = $multipleFiles;

    $SQL = "select FieldValue, ValueId, ExtendData from casedata where ProcId = $ProcId and FieldId = $FieldId and CaseNum = $CaseNum  order by ValueId desc";
    $Query = mysqli_query($connect, $SQL);

    // contador de arquivos Começa no 1 para se não houver arquivos, já estar em 1
    $contadorArquivos = 1;
    while ($Linha = mysqli_fetch_array($Query)) {
        if ($contadorArquivos > 20) {
            break;
        }
        $fieldValue = $Linha["FieldValue"];
        $extendData = $Linha["ExtendData"];
        if ($extendData != "") {
            $jsonExtendData = json_decode($extendData, true);
        } else {
            $jsonExtendData = json_decode($fieldValue, true);
        }
        $ValueId = $Linha["ValueId"];
        $t_body->Excluir = "BPMFolderSendFile.php?ProcId=$ProcId&StepId=$StepId&CaseNum=$CaseNum&FieldId=$FieldId&ValueId=$ValueId&Arquivo=" . str_replace(" ", '%20', $nomeArquivo);
        $t_body->ABRIR = $jsonExtendData["fileName"];
        $t_body->ARQUIVO_NOME = $jsonExtendData["fileName"];
        $t_body->ARQUIVO_NOME_JS = addslashes($jsonExtendData["fileName"]);
        $t_body->ARQUIVO_NOME_STORAGE = $jsonExtendData["fileNameStorage"];
        $t_body->ARQUIVO_DESCRICAO = $jsonExtendData["descricao"];
        $t_body->ARQUIVO_USUARIO = "";
        $t_body->ARQUIVO_DATA = ConvDate($jsonExtendData["data"]);
        $t_body->FIELD_ID = $FieldId;
        if ($ReadOnly != 1 && $tipoFolderList != "botao") {
            $ARQUIVO_4 = $Arquivo[4];
            if ($ReadOnly != 1) {
                $t_body->block("BLOCK_APAGAR_ARQUIVO");
            }
            $contadorArquivos++;
        }
        $t_body->block("BLOCK_ARQUIVO");
    }
    $totalArquivos = mysqli_num_rows($Query);
    if ($totalArquivos == 0) {
        $path = FILES_UPLOAD . "/" . str_pad($ProcId, 6, '0', STR_PAD_LEFT) . "-" . str_pad($CaseNum, 8, '0', STR_PAD_LEFT);
        $totalArquivos = ContaArquivosDiretorio($path);
        $t_body->block("BLOCK_TOTAL_ARQUIVOS");
    } else {
        $t_body->block("BLOCK_LISTA_ARQUIVOS");
    }
    // Nomer de arquivos total
    $t_body->TOTAL_ARQUIVOS = $totalArquivos;

    $t_body->VALUEID = $contadorArquivos;

    if ($ReadOnly == '0' && $tipoFolderList != "botao") {
        $t_body->block("BLOCK_ADD_FD");
    }

    if ($tipoFolderList == "botao") {
        return $t_body->parse();
    } else {
        $t_body->show();
    }
}

function montaCampoTX($fieldId, $Valor_Campo, $campoReadOnly, $tipoCampo, $isArray = false, $FieldCod = "", $FieldName = "", $FieldSize = 25)
{

    $ReadOnly = ($campoReadOnly == 1) ? "readonly tabindex=\"-1\"" : "";
    $template = new Template(FILES_ROOT . "assets/templates/t_campo_tx.html");
    $template->FIELD_VALUE = htmlentities($Valor_Campo);
    $template->FIELD_ID = $fieldId;
    $template->CAMPOREADONLY = $ReadOnly;
    $template->TIPOCAMPO = $tipoCampo;
    $template->FIELDARRAY = ($isArray) ? "[]" : "";
    $template->FIELDCOD = $FieldCod;
    $template->FIELDNAME = $FieldName;
    $template->FIELD_SIZE = $FieldSize;
    if ($ReadOnly != 0) {
        $template->block("BLOCO_ADDON_TXT");
        $template->CLASSE_CAMPO_TX = "input-group";
    }
    $retorno = $template->parse();
    return $retorno;
}

function MontaCampoTL($listavalores, $fieldId, $iconeCampo = "", $descricaoCampo = "")
{
    $template = new Template(FILES_ROOT . "assets/templates/t_campo_lista.html");
    $template->FIELDID = $fieldId;
    foreach ($listavalores as $aItem) {

        // Trata se for array associativo ou lista direta
        $template->ITEMVALUE = (isset($aItem[0])) ? $aItem[0] : $aItem["Valor"];
        $template->ITEMID = (isset($aItem[1])) ? $aItem[1] : $aItem["Id"];
        $selecionado = (isset($aItem[2])) ? $aItem[2] : $aItem["Sel"];
        $template->ITEMSELECTED = "";
        if ($selecionado == 'sel') {
            $template->ITEMSELECTED = "selected";
        }
        $template->block("BLOCK_CAMPO_LT_ITEM");
    }
    $template->FIELDICON = $iconeCampo;
    $template->FIELDDESC = $descricaoCampo;
    $retorno = $template->parse();
    unset($template);
    return $retorno;
}

function PegaListaGruposParaCampo($Valor, $onlySelected)
{
    global $connect, $OrigemLogon;

    $Grupos = array();

    switch ($OrigemLogon) {
        case "Server":
            $Sql = "select * from groupdef ";
            if ($onlySelected) {
                $Sql .= " where GroupId = $Valor ";
            }
            $Query = mysqli_query($connect, $SQL);
            while ($Result = mysqli_fetch_array($Query)) {
                $Grupo["Valor"] = $Result["GroupName"];
                $Grupo["Id"] = $Result["GroupId"];
                $Grupo["Sel"] = "";
                if ($Valor == $Grupo["Id"]) {
                    $Grupo["Sel"] = "sel";
                    if ($onlySelected) { // Sai pois precisa apenas 1
                        $Grupos = array();
                        array_push($Grupos, $Grupo);
                        break;
                    }
                }
                array_push($Grupos, $Grupo);
            }
            break;
        case "ProcessLogon":
            $SQL = "select * from groupdef ";
            if ($onlySelected) {
                $SQL .= " where GroupId = $Valor ";
            }
            $Query = mysqli_query($connect, $SQL);
            $Grupos = array();
            while ($Result = mysqli_fetch_array($Query)) {
                if (empty($Result["GroupName"])) {
                    continue;
                }
                $Grupo["Valor"] = $Result["GroupName"];
                $Grupo["Id"] = $Result["GroupId"];
                $Grupo["Sel"] = "";
                if ($Valor == $Grupo["Id"]) {
                    $Grupo["Sel"] = "sel";
                    if ($onlySelected) { // Sai pois precisa apenas 1
                        $Grupos = array();
                        array_push($Grupos, $Grupo);
                        break;
                    }
                }
                array_push($Grupos, $Grupo);
            }
            break;
        case "External":
            $connectDBExterno = AtivaDBExterno();
            $Sql = "select GroupName, GroupId from Groups order by GroupName";
            if ($onlySelected) {
                $Sql .= " where GroupId = $Valor ";
            }

            $Query = mysqli_query($connectDBExterno, $SQL);
            $Grupos = array();
            while ($Result = mysqli_fetch_array($Query)) {
                $Grupo["Valor"] = $Result["GroupName"];
                $Grupo["Id"] = $Result["GroupId"];
                $Grupo["Sel"] = "";
                if ($Valor == $Grupo["Id"]) {
                    $Grupo["Sel"] = "sel";
                    if ($onlySelected) {  // Sai pois precisa apenas 1
                        $Grupos = array();
                        array_push($Grupos, $Grupo);
                        break;
                    }
                }
                array_push($Grupos, $Grupo);
            }
            break;
    }
    return $Grupos;
}

function montaFD($ProcId, $StepId, $CaseNum, $Campo, $ReadOnly)
{
    $TotalArquivos = Totalvalores($ProcId, $Campo, $CaseNum);
    if ($TotalArquivos == 0) {
        $path = FILES_UPLOAD . "/" . str_pad($ProcId, 6, '0', STR_PAD_LEFT) . "-" . str_pad($CaseNum, 6, '0', STR_PAD_LEFT);
        $TotalArquivos = ContaArquivosDiretorio($path);
    }
    $Arquivos = "Arquivos: $TotalArquivos";
}

function MontaBoleano($Valor, $ReadOnly, $StiloObjeto, $FieldMask = "combobox", $CaptionCampo = "")
{
    Global $FieldId;
    if ($Valor == "") {
        $Valor = 0;
    }
    if ($ReadOnly == 1) {
        if ($FieldMask == "checkbox") {
            if ($Valor == 1) {
                $checked = "checked";
            }
            return "<input type=\"checkbox\" value=\"1\" name=\"h$FieldId\" $StiloObjeto $checked disabled> $CaptionCampo";
        } else {
            if ($Valor == 1) {
                $Valor = "Sim";
            } else {
                $Valor = "Não";
            }
            return $CaptionCampo . "<br>" . CampoReadOnly($Valor, $StiloObjeto);
        }
    }
    if ($FieldMask == "checkbox") {
        if ($Valor <> '0') {
            $checked = "checked";
        }
        $pagina = "<input type=\"checkbox\" value=\"1\" name=\"h$FieldId\" $StiloObjeto $checked  onclick=\"MudaCheckBox(this, 't$FieldId')\"> $CaptionCampo";
        $pagina .= "<input type=\"hidden\" value=\"$Valor\" name=\"t$FieldId\" id=\"t$FieldId\">";
    } else {
        if ($Valor <> '') {
            $SelecionadoNao = "Selected";
            $SelecionadoSim = "";
            if ($Valor == 1) {
                $SelecionadoNao = "";
                $SelecionadoSim = "Selected";
            }
        }
        $pagina = "$CaptionCampo <br /><select class=\"EditCaseSelect\" name=\"t$FieldId\" $ReadOnly $StiloObjeto size=\"1\">\n";
        $pagina .= "<option value=\"0\"></option>\n";
        $pagina = $pagina . "<option value = \"0\" $SelecionadoNao >N&atilde;o</option>\n";
        $pagina = $pagina . "<option value = \"1\" $SelecionadoSim >Sim</option>\n";
        $pagina = $pagina . "</select>\n";
    }
    return $pagina;
}

/**
 * 
 * @param type $templateFields
 * @param string $Action
 * @param type $ProcId
 * @param type $CaseNum
 * @param type $FieldId
 * @param type $FormsPai
 * @param type $linha
 * @param type $ReadOnly
 * @param type $StiloObjeto
 * @param type $Valor_Campo
 * @param type $ExtendProps
 * @param type $AdminForm
 * @param type $nome
 * @param type $optional
 * @param type $ReadOnlyFilhos
 * @param type $ClassStatus
 * @param type $Online
 * @param type $ClasseCampo
 * @param type $classeSubstitura
 * @param type $tipoLabelCampo
 * @param type $callBack
 */
function montaCampoSFR($templateFields, $Action, $ProcId, $CaseNum, $FieldId, $FormsPai, $linha, $ReadOnly, $StiloObjeto, $Valor_Campo, $ExtendProps, $AdminForm, $nome, $optional, $ReadOnlyFilhos, $ClassStatus, $Online, $ClasseCampo = "col-md-12 col-lg-12 form-group-sm input-group", $classeSubstitura = "", $tipoLabelCampo = "label", $callBack = "")
{
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

    $pagina .= "<div class=\"$ClassStatus $ClasseCampo\" style=\"padding-left:0px\" >";
    $pagina .= MontaCampoFormulario($ProcId, $FieldId, $ReadOnly, $StiloObjeto, $Valor_Campo, $ExtendProps, $AdminForm, $FormsPai_Deste, $nome, $optional, $classeSubstitura, $tipoLabelCampo);
    $pagina .= "</div>";


    if (!$AdminForm) {
        $pagina .= "<!-- Inicio Sub-Formulário -->\n";
        //$pagina .= "<div id=\"DivFormulario$FieldId\" class=\"col-md-12\" style=\"margin-top:15px\">\n";
        $pagina .= "<div id=\"DivFormulario$FieldId\" style=\"margin-top:15px\">\n";
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
}

/**
 * 
 * @global type $ValidacaoCampos
 * @param type $ProcId
 * @param type $FieldId
 * @param type $CaseNum
 * @param type $FormsPai
 * @param type $Valor_Campo
 * @param type $linha
 * @param type $ReadOnly
 * @param type $ExtendProps
 * @param type $AdminForm
 * @param type $nome
 * @param type $optional
 * @param type $ReadOnlyFilhos
 * @return type
 */
function _MontaCampoSFR($ProcId, $FieldId, $CaseNum, $FormsPai, $Valor_Campo, $linha, $ReadOnly, $ExtendProps, $AdminForm, $nome, $optional, $ReadOnlyFilhos)
{
    global $ValidacaoCampos;
    $Script = "";
    $StiloObjeto = "";
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
    $ValidacaoCampos .= ScriptValidacaoCampo($Field, $FormsPai_Deste);
    $Lista = MontaCampoFormulario($ProcId, $FieldId, $ReadOnly, $StiloObjeto, $Valor_Campo, $ExtendProps, $AdminForm, $FormsPai_Deste, $nome, $optional);
    $retorno = $Lista;
    if (!$AdminForm) {
        if ($Valor_Campo != "") {
            //$StepOld = $StepId;
            $StepId_Form = $Valor_Campo;
            $Query = CamposFormulario($ProcId, $StepId_Form);
            if ($ReadOnlyFilhos == 1) {
                $Action = "view";
            }
            $t_bodyFormulario = new Template(FILES_ROOT . "assets/templates/t_subform.html");
            $t_bodyFormulario->addFile("SELECAO_PASSO", FILES_ROOT . "templates/template_campo_lista.html");
            $t_bodyFormulario->FieldId = $FieldId;
            $t_bodyFormulario->addFile("CONTEUDO_FIELDS", FILES_ROOT . "assets/templates/t_editcase_wrapp_fields.html");
            $t_bodyFormulario->addFile("CAMPOSFORMULARIO", FILES_ROOT . "assets/templates/t_editcase_fields.html");
            $t_bodyFormulario = MontaFormulario($ProcId, $StepId_Form, $CaseNum, $Query, $Action, True, $AdminForm, $FormsPai_Deste);
            $retorno = $t_bodyFormulario->parse();
        }
        if (!$Online) {
            echo $Script;
        }
    }
    return $retorno;
}

function MontaCampoArrayTabela($ReadOnly, $StiloObjeto, $Valor, $SourceField, $DisplayField, $TableSource, $FieldChange = "", $MasterKey = "", $MasterValue = "", $FieldMaster = "")
{
    global $FieldId, $ACampos;
    $CampoArrayBase = MontaCampoTabela($ReadOnly, $StiloObjeto, "", $SourceField, $DisplayField, $TableSource, $FieldChange, $MasterKey, $MasterValue, $FieldMaster, true);
    $Name = "name=\"$FieldId" . "[]\"";
    $Sub = "name=\"$FieldId\"";
    $CampoArray = str_replace($Sub, $Name, $CampoArrayBase);
    $ACampos[$FieldId] = $CampoArray . "<br>";
    $AdicionarCampo = " <a href=\"javascript:void(0)\" onclick=\"JScriaNovoCampoArray('$FieldId')\">Acrescentar Campo</a><br>";
    if (empty($Valor)) {
        $pagina .= $CampoArray;
        if (!$ReadOnly) {
            $pagina .= $AdicionarCampo;
        }
    } else {
        $valores = explode(",", $Valor);
        if (is_array($valores)) {
            for ($i = 0; $i < count($valores); $i++) {
                if (empty($valores[$i])) {
                    continue;
                }
                $CampoTabela = MontaCampoTabela($ReadOnly, $StiloObjeto, $valores[$i], $SourceField, $DisplayField, $TableSource, $FieldChange, $MasterKey, $MasterValue, $FieldMaster, true);
                $Name = "name=\"t$FieldId" . "[]\"";
                $Sub = "name=\"t$FieldId\"";
                $CampoTabela = str_replace($Sub, $Name, $CampoArray);
                $pagina .= $CampoTabela;
                if ($i > 0) {
                    $pagina .= " <br>";
                } else {
                    if (!$ReadOnly && (!empty($MasterValue) || empty($FieldMaster) )) {
                        $pagina .= $AdicionarCampo;
                    }
                }
            }
        }
    }
    $pagina = str_replace("name=\"tFieldId\"", "name=\"t" . $FieldId . "[]\"", $pagina);
    return $pagina;
}

function MontaCampoTabela($ReadOnly, $StiloObjeto, $Valor, $SourceField, $DisplayField, $TableSource, $FieldChange = "", $MasterKey = "", $MasterValue = "", $FieldMaster = "", $FieldArray = false)
{
    Global $FieldId, $connect, $CaseNum, $ProcId;
    $SQL = ReplaceFieldsValues(trim($TableSource));
    if (!empty($FieldMaster) && empty($MasterValue)) {
        $MasterValue = trim(PegaValorCampo($ProcId, $CaseNum, $FieldMaster, 'TB'));
    }
    if ($ReadOnly == 1 || (empty($MasterValue) && !empty($MasterKey))) {
        if (strpos($SQL, "where") == 0) {
            $SQL .= "  where ";
        } else {
            $SQL .= " and ";
        }
        $SQL .= "$SourceField = '$Valor' ";
        if (!empty($Valor)) {
            $QUERY = mysqli_query($connect, $SQL);
            $linha = mysqli_fetch_array($QUERY);
            $Valor = $linha[$DisplayField];
        } else {
            $Valor = "";
        }
        $pagina = CampoReadOnly($Valor, $StiloObjeto);
    } else {
        $FieldChange = trim($FieldChange);
        if (!empty($FieldChange)) {
            $OnFieldchanges = explode(";", $FieldChange);
            if (is_array($OnFieldchanges)) {
                $OnFieldChange = "onchange=\";";
                for ($i = 0; $i < count($OnFieldchanges); $i++) {
                    $FieldChange = $OnFieldchanges[$i];
                    $OnFieldChange .= "AtualizaCampo($FieldChange, document.getElementById('$FieldId').value , 'Campo$FieldChange');";
                }
            } else {
                $OnFieldChange = "onchange=\"AtualizaCampo($FieldChange, document.getElementById('$FieldId').value , 'Campo$FieldChange')\"";
            }
            $OnFieldChange .= "\"";
        }
        if ($FieldArray) {
            $OnFieldChange = "onchange=\"CriaNovoCampoArray($FieldId, '$MasterValue' , 'Campo$FieldId'); this.onchange=''\"";
        }
        if (!empty($MasterKey)) {
            if (strpos($SQL, "where") == 0) {
                $SQL .= "  where ";
            } else {
                $SQL .= " and ";
            }
            $SQL .= "$MasterKey in ($MasterValue) ";
        }
        $ProcessarDados = false;
        if (!empty($SQL)) {
            $ProcessarDados = true;
            $SQL .= "  Order by $DisplayField ";
            $QUERY = mysqli_query($connect, $SQL);
        }
        $pagina = "<select name=\"t$FieldId\" id=\"t$FieldId\" $StiloObjeto size=\"1\" $OnFieldChange class=\"EditCaseSelect\">\n";
        $pagina .= "<option value=\"0\"></option>\n";
        if ($ProcessarDados) {
            while ($linha = mysqli_fetch_array($QUERY)) {
                $pagina = $pagina . "<option ";
                $Chave = $linha[$SourceField];
                $Display = htmlentities($linha["$DisplayField"]);
                if ($Valor == $Chave) {
                    $pagina = $pagina . " selected ";
                }
                $pagina = $pagina . " value = \"$Chave\">$Display</option>\n";
            }
            mysqli_free_result($QUERY);
        }
        $pagina = $pagina . "</select>\n";
    }
    return $pagina;
}

function MontaCampoTabela_TEMPLATE($ReadOnly, $StiloObjeto, $Valor, $SourceField, $DisplayField, $TableSource, $MasterKey = "", $MasterValue = "", $FieldMaster = "")
{
    Global $connect, $CaseNum, $ProcId;

    $tabela = array();

    $SQL = ReplaceFieldsValues(trim($TableSource));
    if (!empty($FieldMaster) && empty($MasterValue)) {
        $MasterValue = trim(PegaValorCampo($ProcId, $CaseNum, $FieldMaster, 'TB'));
    }
    if ($ReadOnly == 1 || (empty($MasterValue) && !empty($MasterKey))) {
        if (strpos($SQL, "where") == 0) {
            $SQL .= "  where ";
        } else {
            $SQL .= " and ";
        }
        $SQL .= "$SourceField = '$Valor' ";
        if (!empty($Valor)) {
            $QUERY = mysqli_query($connect, $SQL);
            $linha = mysqli_fetch_array($QUERY);
            $Valor = $linha[$DisplayField];
        } else {
            $Valor = "";
        }
        $item["id"] = 0;
        $item["display"] = $Valor;
        array_push($tabela, $item);
        return $tabela;
    } else {
        if (!empty($MasterKey)) {
            if (strpos($SQL, "where") == 0) {
                $SQL .= "  where ";
            } else {
                $SQL .= " and ";
            }
            $SQL .= "$MasterKey in ($MasterValue) ";
        }
        $SQL .= "  Order by $DisplayField ";
        $QUERY = mysqli_query($connect, $SQL);

        while ($linha = mysqli_fetch_array($QUERY)) {
            $Chave = $linha[$SourceField];
            $Display = htmlentities($linha["$DisplayField"]);
            $item["id"] = $Chave;
            $item["display"] = $Display;
            array_push($tabela, $item);
        }
        $mysqli_free_result = mysqli_free_result($QUERY);
    }
    return $tabela;
}

function MontaTM($ProcId, $CaseNum, $FieldId, $ReadOnly)
{
    $template = new Template(FILES_ROOT . "assets/templates/t_campo_mt.html");
    $Entradas = campoMT_Totalvalores($ProcId, $CaseNum, $FieldId);
    $MultiValue[1] = PegaUltimoValorCampoMultiLinhas($ProcId, $CaseNum, $FieldId);
    $template->PROCID = $ProcId;
    $template->CASENUM = $CaseNum;
    $template->FIELDID = $FieldId;
    $template->ENTRADAS = $Entradas;

    if (!$ReadOnly) {
        $template->block("BLOCO_EDITAR_MT");
    }

    if (count($MultiValue) > 0) {
        $template->block("BLOCO_ACIONA_HISTORICO");
    }

    $retorno = $template->parse();
    return $retorno;
}

/**
 * 
 * @param type $fieldId
 * @param type $valorCampo
 * @param type $readOnly
 * @param type $groupSource
 * @param type $origem
 * @param type $optional
 * @return type
 */
function montaCampoUS($fieldId, $valorCampo, $readOnly, $groupSource = "", $origem = "", $optional = "")
{
    $template = new Template(FILES_ROOT . "assets/templates/t_campo_us.html");
    $nomeUsuario = PegaDescUsuario($valorCampo);
    $corIcone = "";
    if ($optional == 0) {
        $corIcone = "text-warning";
    }
    $DISPLAYCAMPO = CampoReadOnly($nomeUsuario, '', $fieldId, 0, 'form-control', "fa-user $corIcone");
    $template->DISPLAYCAMPO = $DISPLAYCAMPO;
    $template->FIELDID = $fieldId;
    $template->FIELD_VALUE = $valorCampo;
    if (!$readOnly) {
        $template->block("BLOCO_SELECIONAR_USUARIO");
    }
    $retorno = $template->parse();
    return $retorno;
}

/**
 * 
 * @param type $ProcId
 * @param type $CaseNum
 * @param type $FieldId
 * @param type $ReadOnly
 * @param type $StiloObjeto
 * @param int $Valor
 * @param type $FieldSource
 * @param type $DisplayField
 * @param type $ProcessoOrigemDados
 * @return type
 */
function MontaDadosCampoDC($ProcId, $CaseNum, $FieldId, $ReadOnly, $StiloObjeto, $Valor, $FieldSource, $DisplayField, $ProcessoOrigemDados)
{
    $template = new Template(FILES_ROOT . "assets/templates/t_campo_dc.html");

    $Valor2 = PegaValorCampo($ProcId, $CaseNum, $FieldId, "DC", 1);
    $DISPLAYCAMPO = CampoReadOnly($Valor2, "", $FieldId, 0, "form-control input-sm", "fa-tasks");


    $id_button_visualizar = "r" . $FieldId . "BUTTON_VISUALIZAR";

    $template->DISPLAYCAMPO = $DISPLAYCAMPO;
    $template->FIELDID = $FieldId;
    $template->VALORCAMPO = $Valor;
    $template->TABLESOURCE = $ProcessoOrigemDados;
    $template->DISPLAYFIELD = $DisplayField;
    $template->IDBUTTONVISUALIZAR = $id_button_visualizar;

    if (!$ReadOnly) {
        $Valor2 = PegaValorCampo($ProcId, $CaseNum, $FieldId, "TX", 0);
        if (empty($Valor)) {
            $Valor = 0;
        }
        $template->block("BLOCO_SELECIONAR_CASO");
    }


    $DisableVisualizar = "";
    if ($Valor == 0) {
        $DisableVisualizar = "disabled";
    }

    $template->DISABLEDVISUALIZAR = $DisableVisualizar;

    $Retorno = $template->parse();
    return $Retorno;
}

function MontaCampoCR_Pagina($_PARAMETROS)
{
    return MontaCampoCR($ProcId, $FieldId, $CaseNum, true, $FieldName);
}

function MontaCampoCR($ProcId, $FieldId, $CaseNum, $showAll = false, $FieldName = "")
{
    global $connect, $procdef;

    $template = new Template(FILES_ROOT . "assets/templates/t_campo_cr.html");

    $nrReferencias = 0;
    $nrTotalCasos = 0;
    if (!$showAll) {
        $nrReferencias = CR_NRSHOWREFERENCIAS;
        $nrTotalCasos = CR_NRSHOWCASOS;
    }

    if (!is_numeric($ProcId)) {
        $ProcId = PegaProcIdByCode($ProcId);
    }

    if (!is_numeric($FieldId)) {
        $FieldId = PegaFieldIdByCode($ProcId, $FieldId);
    }

    $aReferencias = CabecalhoReferenciasSimples($ProcId, true, $nrReferencias);

    // Coloca as colunas de Referencia
    if ($procdef->TipoProc != "CT") {
        // Cria a coluna de Numero de Caso
        $template->CAMPOREF = "Número";
        $template->block("BLOCK_NOME_REFERENCIAS");

        foreach ($aReferencias as $referencia) {
            $template->CAMPOREF = $referencia["Nome"];
            $template->block("BLOCK_NOME_REFERENCIAS");
        }
    }



    $SQL = "Select distinct CaseNum from casedata where ProcId = $ProcId and FieldId = $FieldId and FieldValue like '$CaseNum' order by CaseNum desc";
    //Limita o nr de casos
    if ($nrTotalCasos > 0) {
        $SQL .= " limit 0, $nrTotalCasos ";
    }
    $query = mysqli_query($connect, $SQL);

    if ($procdef->TipoProc == "CT") {
        $SQL = "select fieldId from procfieldsdef where ProcId = $ProcId and FieldType = 'AR'";
        $Query_FieldAr = mysqli_query($connect, $SQL);
        $linha = mysqli_fetch_array($Query_FieldAr);
        $idCampoAR = $linha["fieldId"];
    }


    $i = 0;
    while ($Result = mysqli_fetch_array($query)) {
        if ($i > $nrTotalCasos & $nrTotalCasos > 0) {
            break;
        }

        $template->CAMPOVALOR = $Result["CaseNum"];
        $template->PROCID = $ProcId;
        $template->CASENUMCR = $Result["CaseNum"];

        if ($procdef->TipoProc == "CT") {
            $CaseNumFilho = $Result["CaseNum"];
            $Valor = PegaValorCampo($ProcId, $CaseNumFilho, $idCampoAR);
            $extendProp = json_decode($Valor, true);
            $template->FIELDID = $idCampoAR;
            $template->CAMPOARQUIVO = $extendProp["fileNameStorage"];
            $template->block("BLOCK_IMAGEM");
        } else {
            $template->block("BLOCK_VALOR_REFERENCIAS");


            $aReferencias = PegaReferencias($ProcId, $Result["CaseNum"], false, false, 0, $nrReferencias);

            // Popula as Referencias
            foreach ($aReferencias as $referencia) {
                $template->CAMPOVALOR = $referencia;
                $template->block("BLOCK_VALOR_REFERENCIAS");
            }

            $template->block("BLOCK_CASO");
        }

        $i++; // Contador de Casos 
        // </editor-fold>
    }
    $template->FIELDID = $FieldId;
    $template->FIELDNAME = $FieldName;
    $template->CASENUM = $CaseNum;

    if ($procdef->TipoProc == "CT") {
        $template->block("BLOCO_CR_CONTENT");
    } else {
        $template->block("BLOCO_CR_NORMAL");
        if (!$showAll) {
            $template->block("BLOCO_MODAL");
        }
    }


    $retorno = $template->parse();
    return $retorno;
}

function MontaCampoListaUsuario($ReadOnly, $StiloObjeto, $Valor, $UserId, $SourceField, $DisplayField, $TableSource)
{
    global $campo;
    $valores = PegavaloresListaUsuario($UserId, $TableSource, $SourceField, $DisplayField);
    if ($ReadOnly == 1) {
        return CampoReadOnly($Valor, $StiloObjeto);
    }
    if (count($valores) < 100) {
        $pagina = "<select name=\"t$campo\" $StiloObjeto size=\"1\">\n";
        $pagina .= "<option ";
        if ($Valor == "") {
            $pagina = $pagina . " selected ";
        }
        $pagina .= " value=\"\"></option>\n";
        for ($contador = 0; $contador < count($valores) - 1; $contador++) {
            $pagina = $pagina . "<option ";
            $Chave = htmlentities($valores[$contador]);
            $Display = htmlentities($valores[$contador]);
            if ($Valor == htmlentities($Chave)) {
                $pagina = $pagina . " selected ";
            }
            $pagina = $pagina . " value = \"$Chave\">$Display</option>\n";
        }
        $pagina = $pagina . "</select>\n";
    } else {
        
    }
    return $pagina;
}

function MontaCampoFormulario($ProcId, $campo, $ReadOnly, $StiloObjeto, $Valor, $ExtendProps = '', $AdminForm = false, $FormsPai = '', $FieldName = '', $Optional = 0, $ClasseSubstituta = "", $tipoLabelCampo = "label")
{
    //global $StepId, $CaseNum, $Action;
    global $CaseNum, $Action;
    if (empty($ExtendProps)) {
        $ExtendProps = PegavaloresLista($ProcId, $campo);
    }

    // $ExtendProps = explode(";", str_replace("\"", "", $ExtendProps));
    if (is_array($ExtendProps)) {
        $ExtendProps = $ExtendProps[0];
    }
    $valores = explode(",", $ExtendProps);

    if ($Valor == -1) {
        $Valor = "";
    }
    if ($ReadOnly == 1) {
        foreach ($valores as $ValorCampo) {
            $ValorCampo = explode("|", $ValorCampo);
            if ($ValorCampo[1] == $Valor) {
                $Valor2 = unhtmlentities($ValorCampo[0]);
                break;
            }
        }
        return CampoReadOnly($Valor2, $StiloObjeto, $campo, 0, "form-control input-sm", "fa-list-alt");
    }
    if ($ReadOnly == 3) {
        $Action = 'view';
    }

    $pagina = "<select aria-type-field=\"SFR\" class=\"form-control\" name=\"t$campo\" id=\"t$campo\" $StiloObjeto size=\"1\"";
    if (!$AdminForm) {
        if (is_array($FormsPai)) {
            $FormsPai = implode(",", $FormsPai);
        }
        $pagina .= "onchange=\"JSmontaFormulario($ProcId, $CaseNum, 't$campo', 'DivFormulario$campo', '$Action', '$FormsPai', '$FieldName', $Optional, '$ClasseSubstituta', '$tipoLabelCampo')\"";
    }
    $pagina .= ">\n";
    $pagina .= "<option ";
    if ($Valor == "") {
        $Valor = 0;
        $pagina = $pagina . " selected ";
    }
    $pagina .= " value=\"\"></option>\n";
    for ($contador = 0; $contador < count($valores); $contador++) {
        $pagina = $pagina . "<option ";
        $Dados = explode("|", $valores[$contador]);
        $Display = htmlentities($Dados[0]);
        // Correcao 1
        // if ($Valor2 == $Display)
        if ($Valor == $Dados[1]) {
            $pagina = $pagina . " selected ";
        }
        $pagina = $pagina . " value = \"$Dados[1]\">$Display</option>\n";
    }
    $pagina = $pagina . "</select><span class=\"input-group-addon\"><i class=\"fa fa-list-alt\"></i></span>\n";
    return $pagina;
}

function MontaCampoLista($ProcId, $campo, $FieldCod, $ReadOnly, $StiloObjeto, $Valor, $ExtendProps = '')
{
    // global $StepId;
    if ($Valor == -1) {
        $Valor = "";
    }
    $Listavalores = PegaDadosCampoLista($ProcId, $campo, $Valor, $ExtendProps);
    if ($ReadOnly == 1) {
        foreach ($Listavalores as $Item) {
            if ($Item["selected"]) {
                $Valor = $Item["Display"];
                break;
            }
        }
        return CampoReadOnly($Valor, $StiloObjeto, $campo, "", "form-control input-sm", "fa-list-ul");
    }

    if (count($Listavalores) < 150) {
        $pagina = "<div class=\"input-group\">";
        $pagina .= "<select aria-code=\"$FieldCod\" class=\"form-control\" name=\"t$campo\" id=\"t$campo\" $StiloObjeto size=\"1\">\n";
        //$pagina="<select  name=\"t$campo\" $StiloObjeto size=\"1\">\n";
        $pagina .= "<option value=\"\"></option>\n";
        foreach ($Listavalores as $Item) {
            $pagina = $pagina . "<option ";
            $Display = $Item["Display"];
            $ValorItem = $Item["Valor"];
            if ($Item["selected"]) {
                $pagina .= " selected ";
            }
            $pagina .= " value = \"$ValorItem\">$Display</option>\n";
        }
        $pagina .= "</select><span class=\"input-group-addon\"><i class=\"fa fa-list-ul\" > </i></span>";
        $pagina .= "</div>";
    } else {
        if ($Valor == -1) {
            $Valor = '';
        }
        $Tamanho = MaiorComprimento($Listavalores);
        $pagina = CampoReadOnly($Valor, $StiloObjeto, $campo, $Tamanho, "control-form", "fa-list-ul");
        if (empty($Valor)) {
            $Valor = -1;
        }
        $pagina .= "<input onMouseOver=\"window.status='Selecionar'\" onMouseOut=\"window.status=''\" name=\"SelecionarDoc\" type=\"button\" class=\"buttonSel\" id=\"Seldoc\" value=\"...\" onClick=\"SelecionaOpcao($ProcId, $campo, t$campo.value)\"> ";
        $pagina .= "<input  aria-code=\"$FieldCod\" type=\"hidden\" name=\"t$campo\" id=\"t$campo\" value=\"$Valor\" $StiloObjeto>";
    }
    return $pagina;
}

function MontaCampoUsuario($campo, $ReadOnly, $StiloObjeto, $Valor, $GroupSource, $Origem = "", $LetraT = 't')
{
    $Display = PegaDescUsuario($Valor);
    $pagina = CampoReadOnly($Display, $StiloObjeto, $campo, 20, "EditCaseTextSelUser");
    if (!$ReadOnly) {
        $pagina .= "<input onMouseOver=\"window.status='Selecionar'\" onMouseOut=\"window.status=''\" name=\"SelecionarUsuario\" type=\"button\" class=\"ButtonSel\" id=\"SelUser$campo\" value=\"...\" onClick=\"JSselecionaUsuario('r$campo','$LetraT$campo', '$GroupSource', $LetraT$campo.value, '$Origem')\"> ";
        $pagina .= "<input type=\"hidden\" name=\"" . $LetraT . $campo . "\" id=\"" . $LetraT . $campo . "\" value=\"$Valor\" $StiloObjeto>";
        $pagina .= "<a href='javascript:void(0)' tabindex=\"-1\" onclick=\"document.getElementById('" . $LetraT . $campo . "').value=''; document.getElementById('r$campo').value=''\">Limpar</a>";
    }
    return $pagina;
}

function MontaCampoMultiValue($ReadOnly, $StiloObjeto, $Valor, $SourceField, $DisplayField, $TableSource, $Tipo)
{
    Global $campo, $connect;
    require_once("ClassMultiValue.php");
    if ($Tipo == 'CheckBox') {
        $Campo = new MultiValueFieldCheckBox();
        $Campo->Campo = $campo;
        $Campo->MaxLinhas = 10;
        $Valor = str_replace("'", "", $Valor);
        $valores = explode(',', $Valor);
    } else {
        $Campo = new MultiValueFieldTextArea();
        $Campo->Campo = $campo;
        $Campo->MaxLinhas = 10;
        $Valor = unhtmlentities($Valor);
        $Dados = explode(';', $Valor);
        $Indices = explode(',', $Dados[0]);
        $valores = explode(',', $Dados[1]);
    }
    $Campo->ReadOnly = $ReadOnly;
    $Campo->StyleFields = $StiloObjeto;
    $SQL = $TableSource;
    if (!strpos($SQL, "order by"))
        $SQL .= " Order by $SourceField ";
    $QUERY = mysqli_query($connect, $SQL);
    while ($linha = mysqli_fetch_array($QUERY)) {
        $Valor_Item = '';
        $Chave = $linha[$SourceField];
        $Display = $linha["$DisplayField"];
        if ($Tipo == 'CheckBox') {
            if (in_array($Chave, $valores)) {
                $Valor_Item = "Checked";
            }
        } else {
            if (is_array($Indices) && is_array($valores)) {
                $Valor_Item = $valores[array_search($Chave, $Indices)];
            }
        }
        $Campo->ADDItem($Display, $Chave, htmlentities($Valor_Item));
    }
    mysqli_free_result($QUERY);
    return $Campo->Gera();
}

function montaCampoSignatarios($fieldId, $fieldValue, $readOnly, $optional)
{
    $template = new Template(FILES_ROOT . "assets/templates/t_signatarios.html");
    $template->FIELD_ID = $fieldId;
    $template->VALOR_CAMPO = $fieldValue;
    $template_listaSignatarios = new Template(FILES_ROOT . "assets/templates/t_signatarios_lista.html");

    if ($fieldValue === "") {
        $fieldValue = "[]";
    }
    $signatarios = json_decode($fieldValue, true);

    $errro = json_last_error_msg();

    $template->ICON_SIGNATARIOS = "fa-envelope-o";
    if (is_array($signatarios)) {
        $template->ICON_SIGNATARIOS = "fa-envelope";
        foreach ($signatarios as $signatario) {
            $template_listaSignatarios->signatario_nome = $signatario["signatario_nome"];
            $template_listaSignatarios->indiceSignatario = $signatario["indiceSignatario"];
            $template_listaSignatarios->signatario_email = $signatario["signatario_email"];
            $separador = "";
            foreach ($signatario["signatario_participacoes"] as $participacao) {
                $template_listaSignatarios->DESCRICAO = $separador . trim($participacao["DESCRICAO"]);
                $template_listaSignatarios->block("signatario_participacoes");
                $separador = ", ";
            }
            $template_listaSignatarios->block("BLOCK_SIGNATARIO");
        }
    }
    $template->LISTA_SIGNATARIOS = $template_listaSignatarios->parse();
    return $template->parse();
}

/**
 * 
 * @param type $FieldId
 * @param type $Valor_Campo
 * @param type $ReadOnly
 * @param type $optional
 * @param type $ExtendProps
 */
function montaCampoEX($FieldId, $Valor_Campo, $ReadOnly, $optional, $ExtendProps)
{              
    // Pega o nome do arquivo de configurações do Campo
    $dadosCampo = json_decode($ExtendProps, true);
    $nomeArquivoExterno = $dadosCampo["arquivoExterno"];
    
    // Verifica o arquivo de configurações do campo
    $arquivoExpand = FILES_ROOT . "expand/$nomeArquivoExterno/$nomeArquivoExterno.inc";
    if (!file_exists($arquivoExpand))
    {
        $arquivoExpand = FILES_ROOT . "pages/$nomeArquivoExterno.inc";
    }
    require_once($arquivoExpand);

    // Executa a função de inicialização do campo
    $inicializacaoCampo = "inicializacao_$nomeArquivoExterno";
    return $inicializacaoCampo($FieldId, $ReadOnly, $Valor_Campo, $dadosCampo);
        
}
