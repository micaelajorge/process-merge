<?php

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/resource01.php");
include_once(FILES_ROOT . "include/resource02.php");
include_once(FILES_ROOT . "pages/editcase_form.php");
include_once(FILES_ROOT . "include/bpmeditcasefuncs.inc");
include_once(FILES_ROOT . "include/toolbar.inc");
include_once(FILES_ROOT . "pages/ostepdoc.inc");
include_once(FILES_ROOT . "pages/editcase_campos.inc");

use raelgc\view\Template;

function CriaFormularioFluxoWF($t_body, $ProcId, $StepId, $CaseNum, $Campos, $Action)
{
    $t_body->DESCRICAO_TAREFA = "Tarefa";
    $t_body->OPERACAO = $Action;
    $retorno = MontaFormulario($ProcId, $StepId, $CaseNum, $Campos, $Action);
    $t_body->CONTEUDO_FIELDS = $retorno["CONTEUDO"];
    $t_body->STEPCODE_ORIGEM_CAMPOS = PegaStepCode($ProcId, $StepId);
    $t_body->block("BLOCK_NORMAL");
    return $retorno;
}

/**
 * 
 * @param type $t_body
 * @param type $ProcId
 * @param type $StepId
 * @param type $CaseNum
 * @param type $Campos
 * @param type $Action
 * @param type $NovoCaso
 * @param type $TipoFormulario
 * @return string
 */
function CriaFormularioEditarClassificar($t_body, $ProcId, $StepId, $CaseNum, $Campos, $Action, $NovoCaso, $TipoFormulario)
{
    $t_body->DESCRICAO_TAREFA = "Processo";

    $retorno = CriaFormularioFluxoWF($t_body, $ProcId, $StepId, $CaseNum, $Campos, $Action);
    $t_body->INCLUDE_JAVASCRIPT = $retorno["VALIDACAO_JAVASCRIPT"];
    $t_body->block("BLOCO_INCLUDE_JAVASCRIPT");

    $t_body->SHOW_SITUACAO_PROCESSO = "";
    $StatusProcessoAtual = PegaValorCampo($ProcId, $CaseNum, 6);

    switch ($TipoFormulario) {
        case "I":
            $StepIdOrigemCampos = $StepId;
            break;
        default:
            $StepIdOrigemCampos = PegaValorCampo($ProcId, $CaseNum, 5);
            break;
    }
    $t_body->STEPID_ORIGEM_CAMPOS = $StepIdOrigemCampos;
    $t_body->STEPCODE_ORIGEM_CAMPOS = PegaStepCode($ProcId, $StepIdOrigemCampos);

    $t_body->STYLE_LOGO = "style=\"display:none !important\"";

    if ($Action == "Edit") {
        $t_body->BOTAO_EDITAR_CLASSE = "btn-primary";
        $t_body->BOTAO_EDITAR_DISABLED = "";

        foreach ($listaStatus as $status) {
            $t_body->NOVA_SITUACAO_TEXTO = $status["stepname"];
            $t_body->NOVA_SITUACAO_ID = $status["stepid"];
            $t_body->block("BLOCO_OPCOES_SITUACAO");
        }
        if ($TipoFormulario != "I") {
            $t_body->block("BLOCO_ALTERAR_SITUACAO_PROCESSO");
        }
    } else {
        $t_body->BOTAO_CAPTURAR_IMAGEM_DISABLED = "disabled";
        $t_body->BOTAO_CAPTURAR_DISABLED = "disabled";

        $t_body->BOTAO_EDITAR_CLASSE = "btn-default";
        $t_body->BOTAO_EDITAR_DISABLED = "disabled";
        if ($TipoFormulario != "I") {
            $t_body->block("BLOCO_SITUACAO_PROCESSO");
        }
    }

    $t_body->CLASSE_COLUNA_PROCESSO = "col-lg-6";
// Coloca o div para as imagens do documento
    $t_body->block("BLOCO_DOCUMENTOS_PROCESSO");

    $retorno["VALIDACAO_JAVASCRIPT"] = "";
    $t_body->addFile("HTML_WEBCAPTURA", FILES_ROOT . "assets/templates/t_editcase_webcaptura.html");

// Insere o ProcId para tratamento no Datatable
    $t_body->SCRIPT_DATA = "var tipoProcesso = 'CT'";
    $t_body->block("BLOCO_JAVASCRIPT_INSERTS");

    $t_body->SCRIPT_DATA = "$(document).ready(
        function () {
            jsCarregaDocumentosProcesso();
            $('[aria-code]').on('change', jsCarregaDocumentosProcesso);
        }
);";
    $t_body->block("BLOCO_JAVASCRIPT_INSERTS");
    return $retorno;
}

/**
 * 
 * @param type $t_body
 * @param type $ProcId
 * @param type $StepId
 * @param type $CaseNum
 * @param type $Campos
 * @param type $Action
 * @param type $NovoCaso
 * @param type $TipoFormulario
 * @return string
 */
function CriaFormularioFluxoCT($t_body, $ProcId, $StepId, $CaseNum, $Campos, $Action, $NovoCaso, $TipoFormulario)
{
    $t_body->DESCRICAO_TAREFA = "Processo";

// Se é novo caso gera o formulário normalmente
    if ($NovoCaso) {
// Pega o Status de inicio do Processo
//$StatusInicialProcesso = PegaStatusInicialProcesso($ProcId, $StepId);
        $retorno = CriaFormularioFluxoWF($t_body, $ProcId, $StepId, $CaseNum, $Campos, $Action);
        $t_body->POSACTION = "reeditarCT";
        return $retorno;
    } else {
        $t_body->POSACTION = "";
    }
    $t_body->OPERACAO = $Action;
    $t_body->addFile("CABECALHO_PROCESSO", FILES_ROOT . "assets/templates/t_editcase_cabecalho_processo.html");

    $t_body->SHOW_SITUACAO_PROCESSO = "";
    $StatusProcessoAtual = PegaValorCampo($ProcId, $CaseNum, 6);

    switch ($TipoFormulario) {
        case "I":
            $StepIdOrigemCampos = $StepId;
            break;
        default:
            $StepIdOrigemCampos = PegaValorCampo($ProcId, $CaseNum, 5);
            break;
    }
    $t_body->STEPID_ORIGEM_CAMPOS = $StepIdOrigemCampos;
    $t_body->STEPCODE_ORIGEM_CAMPOS = PegaStepCode($ProcId, $StepIdOrigemCampos);

    $t_body->STYLE_LOGO = "style=\"display:none !important\"";

    $NomeStatusAtual = PegaNomeStep($ProcId, $StatusProcessoAtual);
    $listaStatus = PegaListaStatusRotear($ProcId, $StatusProcessoAtual);


    $t_body->SITUACAO_ATUAL_TEXTO = $NomeStatusAtual;
    $t_body->SITUACAO_ATUAL_ID = $StatusProcessoAtual;

    if ($Action == "Edit") {
        $t_body->BOTAO_EDITAR_CLASSE = "btn-primary";
        $t_body->BOTAO_EDITAR_DISABLED = "";

        foreach ($listaStatus as $status) {
            $t_body->NOVA_SITUACAO_TEXTO = $status["stepname"];
            $t_body->NOVA_SITUACAO_ID = $status["stepid"];
            $t_body->block("BLOCO_OPCOES_SITUACAO");
        }
        if ($TipoFormulario != "I") {
            $t_body->block("BLOCO_ALTERAR_SITUACAO_PROCESSO");
        }
    } else {
        $t_body->BOTAO_CAPTURAR_IMAGEM_DISABLED = "disabled";
        $t_body->BOTAO_CAPTURAR_DISABLED = "disabled";

        $t_body->BOTAO_EDITAR_CLASSE = "btn-default";
        $t_body->BOTAO_EDITAR_DISABLED = "disabled";
        if ($TipoFormulario != "I") {
            $t_body->block("BLOCO_SITUACAO_PROCESSO");
        }
    }
    $retornoCabecalho = MontaFormularioCabecalho($ProcId, $CaseNum, $Campos);
//$t_body->CONTEUDO_CABECALHO = $retornoCabecalho;
    $t_body->CONTEUDO_CABECALHO = $retornoCabecalho;
    $t_body->STEPIDPROCESSO = $t_body->block("BLOCK_NORMAL");

//$t_body->block("CABECALHO_PROCESSO"); 

    $t_body->CLASSE_COLUNA_PROCESSO = "col-lg-6";
// Coloca o div para as imagens do documento
    $t_body->block("BLOCO_DOCUMENTOS_PROCESSO");

    $retorno["VALIDACAO_JAVASCRIPT"] = "";
    $t_body->addFile("HTML_WEBCAPTURA", FILES_ROOT . "assets/templates/t_editcase_webcaptura.html");

// Insere o ProcId para tratamento no Datatable
    $t_body->SCRIPT_DATA = "var tipoProcesso = 'CT'";
    $t_body->block("BLOCO_JAVASCRIPT_INSERTS");

    $t_body->ARQUIVO_JS = "webcaptura.js";
    $t_body->block("BLOCO_JAVASCRIPT");

    $t_body->SCRIPT_DATA = "$(document).ready(
        function () {
            jsCarregaDocumentosProcesso();
            $('[aria-code]').on('change', jsCarregaDocumentosProcesso);
        }
);";
    $t_body->block("BLOCO_JAVASCRIPT_INSERTS");
    return $retorno;
}

// <editor-fold defaultstate="collapsed" desc="Preparando Variaveis de Ambiente">
if (!empty($Direct)) {
    $Fechar = 1;
}

/*
  if (session_is_registered("lidos")) {
  array_push($lidos, "$ProcId-$StepId-$CaseNum");
  }
 * 
 */

if (count($_POST) > 0) {

    if (key_exists("ProcId", $_POST)) {
        $EditCase_ProcId = $ProcId;
    } else {
        $ProcId = $EditCase_ProcId;
    }

    if (key_exists("StepId", $_POST)) {
        $EditCase_StepId = $StepId;
    } else {
        $StepId = $EditCase_StepId;
    }

    if (key_exists("CaseNum", $_POST)) {
        $EditCase_CaseNum = $CaseNum;
    } else {
        $CaseNum = $EditCase_CaseNum;
    }
}

if (count($_GET) > 0) {
    if (key_exists("ProcId", $_GET)) {
        $EditCase_ProcId = $_GET["ProcId"];
    } else {
        if (isset($EditCase_ProcId)) {
            $ProcId = $EditCase_ProcId;
        } else {
            $ProcId = PegaProcIdCaso($CaseNum);
        }
    }

    if (key_exists("StepId", $_GET)) {
        $EditCase_StepId = $_GET["StepId"];
    } else {
        $StepId = $EditCase_StepId;
    }

    if (key_exists("CaseNum", $_GET)) {
        $EditCase_CaseNum = $_GET["CaseNum"];
    } else {
        $CaseNum = $EditCase_CaseNum;
    }
}


if (!is_numeric($ProcId)) {
    $ProcId = PegaProcIdByCode($ProcId);
}

if (!is_numeric($StepId)) {
    $StepId = PegaStepIdByCode($ProcId, $StepId);
}

if (isset($_SESSION[$ProcId]["procdef"])) {
    $procdef = $_SESSION[$ProcId]["procdef"];
    if ($procdef->ProcId <> $ProcId) {
        $procdef = new procdef;
        $procdef->Create($ProcId, $connect);
    }
} else {
    $procdef = new procdef;
    $procdef->Create($ProcId, $connect);
    $_SESSION[$ProcId]["procdef"] = $procdef;
}

if ($StepId == "") {
    $StepId = $procdef->DefaultView;
    $VisualizarComoDefaultView = true;
}


$userdef = $_SESSION["userdef"];
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Pega os Botões Habilitados para o Passo no WebDesigner">
// 
// 

$MostrarBotoes = PegaBotoes($ProcId, $StepId);

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Define ação que seré feita no caso">

$valoresCampos = "";

if ($StepId != -1) {
    if (empty($StepId)) {
        $StepId = PegaStepDoc($ProcId, $CaseNum);
    }
} else {
    $StepId = 0;
}

if ($StepId == 0) {
    $StepId = PegaDefaultStep($ProcId);
}

if (empty($Action)) {
    $Action = PegaAcao($ProcId, $StepId, $CaseNum);
}

if ($CaseNum != 0) {
    $CasoCasoEstaNaFila = CasoCasoEstaNaFila($ProcId, $StepId, $CaseNum);
} else {
    $CasoCasoEstaNaFila = true;
}

if (!$CasoCasoEstaNaFila) {
    $Action = "view";
}

// Se for visualizacao de Caso de Campo, esconde os itens
$mostraNavegacao = true;
$mostraBarraSuperior = true;


// </editor-fold>
// Inicio Criaçao da Página
PegaDadoscasequeue();

// Define o Tipo de Formulário que será criado
$tipoFormulario = $procdef->Steps[$StepId]["StepType"];

//$CamposRum = PegaCamposProcRum($ProcId);	
//$CamposRumAtivo = PegaCamposProcRumAtivo($ProcId, $StepId);
// Habilita ou nao botao de hist�rico
$EnableHistorico = "enable=false";

/// <editor-fold defaultstate="collapsed" desc="Prepara Tickler">

$FieldTickler = TemTickler($ProcId);

if ($FieldTickler > 0) {
    $EnableHistorico = "enable=true";
}

if ($FieldTickler > 0 and ! $NovoCaso) {
    $comentario = Tickler($ProcId, $FieldTickler, $CaseNum, $AdHoc);
    $MostrarAdHoc = StepAdHoc($StepId);
    $CriarAdHoc = UserCriaAdHoc($UserId);
    $MontarAdHoc = "BPMAdHoc.php?ProcId=$ProcId&StepId=$StepId&FieldId=$FieldTickler&CaseNum=$CaseNum&Tickler=1&NovaLinha=$TotalComentarios&Edit=0&Tickler=1&Close=1&LockAdHoc=1";
    $TotalComentarios = $TotalComentarios + 3;
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Bloqueia o Caso na Fila">
// 
// Duplicado com Função Posterior
//$dataHoraLock = date("Y-m-d H:i:s");
//$SQL = "update casequeue set lockedbyid = $userdef->UserId, LockedBysamaccountname = '$userdef->UserName', lockeddatetime = '$dataHoraLock' where ProcId=$ProcId and StepId= $StepId and CaseId = $CaseNum and LockedbyId = 0 and 'Edit' = '$Action' and AdHoc=0";
//$retorno_bloqueio = mysqli_query($connect, $SQL);
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Prepara as variaveis o Caso">
$CustonNewInstance = PegaNomeInstancia($ProcId);
$StepName = PegaNomeStep($ProcId, $StepId, 0);
$HelpHtml = PegaHelpStep($ProcId, $StepId);

if ($CaseNum == 0) {
    $CaseNum = NovoCaso($ProcId, $StepId, $tipoFormulario);
    $NovoCaso = 1;
    $AcaoCaso = "Novo $procdef->InstanceName";
} else {
    $NovoCaso = 0;
    if ($LockedById == 0) {
        $AcaoCaso = "Editar $procdef->InstanceName";
    } else {
        $AcaoCaso = "<img src=\"images/LockByUser.gif\" alt=\"Em uso ou Bloqueado por $LockedBy \" width=\"20\" height=\"21\" border=0> Visualizar $procdef->InstanceName";
    }
}

/**
 *  Pega Acao após processar
 */
foreach ($procdef->Steps as $stepDisp) {
    $stepIdAtual = $stepDisp["StepId"];
    if ($stepIdAtual == $StepId) {
        if ($NovoCaso) {
            $AcaoAposProcessar = $stepDisp["DefaultNewCaseAction"];
        } else {
            $AcaoAposProcessar = $stepDisp["DefaultEditAction"];
        }
    }
}

/*
  $Caso = new STEPDOC;
  $Caso->SetConnection($connect);
  $Caso->SetProc($ProcId);
  $Caso->SetStep($StepId);
  $Caso->open();
  $Caso->NovoCaso = $NovoCaso;
  $Caso->UserId = $userdef->UserId;
  $Caso->UserName = $userdef->UserName;
  $Caso->UserDesc = $userdef->UserDesc;
  $Caso->samaccountname = $userdef->UserName;
  $Caso->Origem = $userdef->Origem;
  $Caso->UserDesc = $userdef->UserDesc;
  $Caso->SetCaseNum($CaseNum);
  $Caso->SetAction("Abrir");
 */

if (!$CasoCasoEstaNaFila) {
    $AcaoCaso = "$procdef->InstanceName não está mais no passo";
} else {
    if (!$NovoCaso) {
        $Caso = new STEPDOC;
        $Caso->SetConnection($connect);
        $Caso->SetProc($ProcId);
        $Caso->SetStep($StepId);
        $Caso->open();
        $Caso->NovoCaso = $NovoCaso;
        $Caso->UserId = $userdef->UserId;
        $Caso->UserName = $userdef->UserName;
        $Caso->UserDesc = $userdef->UserDesc;
        $Caso->samaccountname = $userdef->UserName;
        $Caso->Origem = $userdef->Origem;
        $Caso->UserDesc = $userdef->UserDesc;
        $Caso->SetCaseNum($CaseNum);
        $Caso->SetAction("Abrir");
    }
    $Validacao = ScriptValidacao($Formulario["SCRIPT"], $Action);
    BloqueiaRegistroParaSerEditadoPeloUsuario();
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Prepara o Template">

$t_body = new Template(FILES_ROOT . "assets/templates/master/t_master.html");
$t_body->addFile("PAGEHEADER", FILES_ROOT . "assets/templates/master/t_header.html");
$t_body->addFile("PAGEMENULEFT", FILES_ROOT . "assets/templates/master/t_menu.html");
$t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/t_editcase.html");
$t_body->SITE_LOCATION = SITE_LOCATION;

if ($procdef->TipoProc == "CT") {
//    $mostraNavegacao = false;
//    $mostraBarraSuperior = false;
    $t_body->BOX_CONTENT_ROW = "row";
    $t_body->BOX_CONTENT_COL = "col-lg-10 col-lg-offset-1";
}

if (defined("FILE_CABECALHO_CAMPOS")) {
    $t_body->addFile("CABECALHO_CAMPOS", FILES_ROOT . "assets/templates/" . FILE_CABECALHO_CAMPOS);
}

// Adiciona o css especifico da página
$t_body->ARQUIVO_CSS = "editcase.css";
$t_body->block("BLOCO_CSS");

$t_body->CLASSE_WRAP_EDITAR = "col-lg-12";
if (defined("CLASS_COL_MASTER_EDITAR")) {
    $t_body->CLASS_COL_MASTER_EDITAR = CLASS_COL_MASTER_EDITAR;
} else {
    $t_body->CLASS_COL_MASTER_EDITAR = "col-lg-8 col-lg-offset-2";
}

if (defined("SHOW_ICON_TOOLBAR")) {
    $t_body->SHOW_ICON_TOOLBAR = " ";
}

$t_body->ESCODE_IMAGEM_TIPIFICACAO = "hide";

// Esconde a opção de Mudança de situação
$t_body->SHOW_SITUACAO_PROCESSO = "display:none";

// Se está como defaultView esconde os dados COmplementares
if ($VisualizarComoDefaultView) {
    $t_body->ESCONDE_DADOS_ADICIONAIS = "display:none";
}

// Formatacao baseado no Tipo de Formulario
switch ($tipoFormulario) {
// Passo Fluxo
    case "S":
// Não Sei        
    case "C":
// Pasta de Documetnos    
    case "P":
        $t_body->MOSTRA_TOOLBAR_FORMALIZACAO = "hide";
        if (defined("CLASSE_DADOS_CASO")) {
            $t_body->CLASSE_DADOS_CASO = CLASSE_DADOS_CASO;
        } else {
            $t_body->CLASSE_DADOS_CASO = "col-lg-12";
        }

        if (defined("CLASS_TAMANHO_FORMULARIO")) {
            $t_body->CLASS_TAMANHO_FORMULARIO = CLASS_TAMANHO_FORMULARIO;
        }

        $espacoEntreLinhas = "min-height: 100px";
        break;

// Formalização
    case "F":

// Classe de tamanho da coluna de Campos do Formulario de Formalizacao
        $t_body->CLASSE_WRAP_EDITAR = "col-lg-4";

        $t_body->CLASS_COL_MASTER_EDITAR = "col-lg-12";
        $t_body->MOSTRA_TOOLBAR_FORMALIZACAO = "";
        if (!defined("CLASSE_DADOS_CASO")) {
            $t_body->CLASSE_DADOS_CASO = "col-lg-6";
        }
        $espacoEntreLinhas = "min-height: 50px";
// Esconde o Menu do TOPO
        $t_body->HEADER_HIDEN = "hide";
        $t_body->CONTENT_HEADER_HIDEN = "hide";
        $t_body->MAIN_FOOTER_HIDEN = "hide";

        break;

// Tipificação
    case "O":
        $t_body->CLASSE_WRAP_EDITAR = "col-lg-6";
        $t_body->MOSTRA_TOOLBAR_FORMALIZACAO = "";
        $t_body->CLASSE_DADOS_CASO = "col-lg-6";
        $t_body->addfile(IMAGEM_FORMALIZACAO, FILES_ROOT . "assets/templates/t_view_imagem_formalizacao.html");
        $t_body->CLASSE_COLUNA_IMAGEM = "col-lg-12";

        $t_body->ESCODE_IMAGEM_TIPIFICACAO = "";
        $t_body->CLASS_COL_MASTER_EDITAR = "col-lg-12";
        $espacoEntreLinhas = "min-height: 50px";
// Esconde o Menu do TOPO
        $t_body->HEADER_HIDEN = "hide";
        $t_body->CONTENT_HEADER_HIDEN = "hide";
        $t_body->MAIN_FOOTER_HIDEN = "hide";
        break;
}

//$t_body->addFile("CONTEUDO_FIELDS", "templates/template_editcase_fields.html");

$mobileMode = $_SESSION["mobileMode"];

if ($mobileMode) {
    $t_body->TOP_NAME = "$procdef->ProcName";
    $t_body->CONTENT_HEADER_HIDEN = "hide";
} else {
    $t_body->TOP_NAME = APP_NAME;
}

switch ($tipoFormulario) {
    case "O":
    case "F":
        $t_body->addFile("TOOLBAR_FORMALIZACAO", FILES_ROOT . "assets/templates/master/t_toolbar.html");
        $t_body->SIDEBAR_HIDE = "hide";
        break;
    default:
        if ($mobileMode == 0) {
            $t_body->SIDEBAR_HIDE = "hide";
            $t_body->addFile("TOOLBAR_TOP", FILES_ROOT . "assets/templates/master/t_toolbar_top.html");
        } else {
            $t_body->addFile("TOOLBAR_LEFT", FILES_ROOT . "assets/templates/master/t_toolbar_left.html");
            $t_body->SIDEBAR_HIDE = "";
        }
        break;
}

if (!$mostraBarraSuperior) {
    $t_body->HEADER_HIDEN = "hide";
}

$t_body->CLASSEFORMULARIO = "DivFieldForm";
//$t_body->SITE_LOCATION = SITE_LOCATION;
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Verifica o Lock do Caso">
if ($LockedById <> 0) {
    $LockedBy = "Bloqueado por $LockedBy <br>";
    $t_body->LOCKEDBY = $LockedBy;
    $t_body->block("BLOCK_LOCKED");
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Popula toolbar">

switch ($tipoFormulario) {
    case "S":
    case "I":
        $StepIdBuscaCampos = $StepId;
        DefineMenu();
        break;
    case "P":
    case "U":
        if ($NovoCaso) {
            $StepIdBuscaCampos = $StepId;
            $Menu = MENU_ROTEAR_NOVO_PROCESSO;
        } else {
            $StepIdBuscaCampos = PegaValorCampo($ProcId, $CaseNum, 5);
            if ($Action == "View") {
                $Menu = 2;
            } else {
                $Menu = MENU_ROTEAR_PROCESSO;
            }
        }
        $StepIdBuscaCampos = $StepId;
        DefineMenu();
        break;
    default:
        $StepIdBuscaCampos = $StepId;
        $Menu = MENU_PROCESSAR_PROXIMO;
        $t_body->POSACTION = 'ProcessarNext';
        break;
}

// Verifica se é visualizar DOCUMENTO E REMOVE OPCOES DE LOGOUT
if ($Fechar) {
    $t_body->OPCOES_LOGOUT_HIDDEN = "hidden";
    $t_body->LINKENTRADA = "#";
}

populaToolBarTemplate($t_body, $Menu);
$t_body->block("BLOCK_TOOLBAR");

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Popula dados Principais do Template">
$t_body->PROCICON = $procdef->ProcIcon;
$t_body->PROCCOLOR = $procdef->ProcColor;
$t_body->PROCESSODESC = "Processo - " . $procdef->ProcDesc;
$t_body->PROCDESC = $procdef->ProcDesc;
$t_body->PROCID = $ProcId;
$t_body->STEPID = $StepId;
$t_body->PROCCODE = PegaProcCode($ProcId);
$t_body->STEPCODE = PegaStepCode($ProcId, $StepId);
$t_body->PROCNAME = $procdef->ProcName;
$t_body->STEPNAME = $StepName;
$t_body->CASENUM = $CaseNum;
$t_body->ACAOCASO = $AcaoCaso;
$t_body->POSACTION = $AcaoAposProcessar;
$t_body->METODO_FORM = "method='post'";
$t_body->PRIORIDADE = $Prioridade;
$t_body->NOVOCASO = $NovoCaso;
$t_body->FECHADO = 0;

$t_body->HEADER_TEXT = "$procdef->ProcName";
$t_body->HEADER_SMALL_TEXT = "$procdef->ProcDesc";
$t_body->HEADER_ICO = "fa $procdef->ProcIcon";
$t_body->HEADER_CLASS = "text-primary";
$t_body->HEADER_COLOR = "$procdef->ProcColor";
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Popula Navegacao">
// Se estiver apenas visualizando, não mostra a navegação
if ($MostrarNavegacao) {
    $t_body->NAGEVAGAO_LINK = "queue/$procdef->ProcId";
    $t_body->NAV_HEADER_TEXT = "$procdef->ProcName";
    $t_body->NAV_HEADER_ICO = "fa $procdef->ProcIcon";
    $t_body->NAV_HEADER_COLOR = "$procdef->ProcColor";
    $t_body->NAV_HEADER_CLASS = "";
    $t_body->NAGEVAGAO_LINK = "queue/$procdef->ProcId";
    $t_body->block("BLOCO_NAVEGACAO"); // Inclui na navegação do Site

    if ($Action == "view") {
        $t_body->NAV_HEADER_TEXT = "Visualizar";
        $t_body->NAV_HEADER_ICO = "fa fa-eye";
        $t_body->NAV_HEADER_COLOR = "";
        $t_body->NAV_HEADER_CLASS = "text-warning";
    } else {
        $t_body->NAV_HEADER_TEXT = "Edição";
        $t_body->NAV_HEADER_ICO = "fa fa-edit";
        $t_body->NAV_HEADER_COLOR = "";
        $t_body->NAV_HEADER_CLASS = "text-primary";
    }
    $t_body->NAGEVAGAO_LINK = "javascript:";
    $t_body->block("BLOCO_NAVEGACAO"); // Inclui na navegação do Site
    $t_body->block("BLOCO_MOSTRAR_NAVEGACAO");
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="dados do Usuário no Template">
$t_body->USER_DESC = $userdef->UserDesc;
$t_body->USER_NAME = $userdef->UserName;
$t_body->USER_ORIGEM = $userdef->Origem;
$t_body->USER_EMAIL = $userdef->Email;
$t_body->USER_ID = $userdef->UserId;
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Formatação Especifica da Página">
$t_body->WRAPPER_STYLE = "style=\"width: 100%; margin-left: 0px !important\"";
// </editor-fold>
// Espaço entre linhas
$t_body->STYLE_ENTRE_LINHAS = $espacoEntreLinhas;
$t_body->TITULO_PAGE = "$procdef->ProcName | " . INSTANCENAME;

// Trata o Campo IMAGEM E FOLDER se For Formulario de Formalizacao
switch ($tipoFormulario) {
    case "O":
        $Campos = CamposFormulario($ProcId, $StepId);
        $CamposOriginal = $Campos;
        $Campos = array();
        for ($indiceImagem = 0; $indiceImagem < count($CamposOriginal); $indiceImagem++) {
            if (in_array($CamposOriginal[$indiceImagem]["fieldtype"], array("AR", "FD", "IM", "BO"))) {
                $tipoCampoMudado = "IT";
                if ($CamposOriginal[$indiceImagem]["fieldtype"] == "BO") {
                    $tipoCampoMudado = "TD";
                }
                $CamposOriginal[$indiceImagem]["fieldtype"] = $tipoCampoMudado;
                array_push($Campos, $CamposOriginal[$indiceImagem]);
            }
        }
        break;
    case "F":
        $Campos = CamposFormulario($ProcId, $StepIdBuscaCampos);
        $CamposOriginal = $Campos;
        $Campos = array();
        for ($indiceImagem = 0; $indiceImagem < count($CamposOriginal); $indiceImagem++) {
            if (in_array($CamposOriginal[$indiceImagem]["fieldtype"], array("AR", "FD", "IM"))) {
                $CampoImagem[0] = $CamposOriginal[$indiceImagem];
                continue;
            }
            array_push($Campos, $CamposOriginal[$indiceImagem]);
        }
        break;
}

$t_body->DESCRICAO_TAREFA = "Processo";

$t_body->MAIN_FOOTER_HIDEN = "hidden";

// Trata o Campo IMAGEM E FOLDER se For Formulario de FOrmalizacao
switch ($tipoFormulario) {
    case "0":
//        $Campos = CamposFormulario($ProcId, $StepIdBuscaCampos);
        $retorno = CriaFormularioFluxoWF($t_body, $ProcId, $StepId, $CaseNum, $Campos, $Action);

        $t_body->CONTEUDO_FIELDS = $retorno["CONTEUDO"];

//        $CampoImagem[0]["fieldtype"] = "IF";
//        $retorno = MontaFormulario($ProcId, $StepId, $CaseNum, $CampoImagem, $Action);
//        $t_body->CAMPO_IMAGEM_FORMALIZACAO = $retorno["CONTEUDO"];
//        $t_body->block("BLOCK_FORMALIZACAO");

        $t_body->ARQUIVO_JS = "panimage.js";
        $t_body->block("BLOCO_JAVASCRIPT");

        $t_body->ARQUIVO_CSS = "panimage.css";
        $t_body->block("BLOCO_CSS");

        $t_body->ARQUIVO_JS = "dragdrop.js";
        $t_body->block("BLOCO_JAVASCRIPT");

        $t_body->ARQUIVO_JS = "pdf.js";
        $t_body->block("BLOCO_JAVASCRIPT_VENDOR");

        $t_body->ARQUIVO_JS = "pdf.worker.js";
        $t_body->block("BLOCO_JAVASCRIPT_VENDOR");

        $t_body->block("BLOCK_NORMAL");

// Marca para não voltar a Queue e processar o proximo caso
        $t_body->POSACTION = "ProcessarNext";

        break;
    case "F":
    case "C":
    case "L":
        $retorno = MontaFormulario($ProcId, $StepId, $CaseNum, $Campos, $Action);
        $t_body->CONTEUDO_FIELDS = $retorno["CONTEUDO"];

        $CampoImagem[0]["fieldtype"] = "IF";

        $retorno = MontaFormulario($ProcId, $StepId, $CaseNum, $CampoImagem, $Action);

        $t_body->IMAGEM_FORMALIZACAO = $retorno["CONTEUDO"];
//        $t_body->CAMPO_IMAGEM_FORMALIZACAO = $retorno["CONTEUDO"];
        $t_body->block("BLOCK_FORMALIZACAO");

        $t_body->ARQUIVO_JS = "fixar_formalizacao.js";
        $t_body->block("BLOCO_JAVASCRIPT");

        $t_body->ARQUIVO_CSS = "panimage.css";
        $t_body->block("BLOCO_CSS");

        $t_body->ARQUIVO_JS = "panimage.js";
        $t_body->block("BLOCO_JAVASCRIPT");

        $t_body->ARQUIVO_JS = "ler_pdf_process.js";
        $t_body->block("BLOCO_JAVASCRIPT");


// Marca para não voltar a Queue e processar o proximo caso
        $t_body->POSACTION = "ProcessarNext";
        break;
    case "S":
        $Campos = CamposFormulario($ProcId, $StepIdBuscaCampos);
        $retorno = CriaFormularioFluxoWF($t_body, $ProcId, $StepId, $CaseNum, $Campos, $Action);
        break;

// Pasta de Documentos
    case "P":
    case "U":
        $Campos = CamposFormulario($ProcId, $StepIdBuscaCampos);
        $retorno = CriaFormularioFluxoCT($t_body, $ProcId, $StepIdBuscaCampos, $CaseNum, $Campos, $Action, $NovoCaso, $tipoFormulario);
        break;

    case "I":
        $Campos = CamposFormulario($ProcId, $StepIdBuscaCampos);
        $retorno = CriaFormularioEditarClassificar($t_body, $ProcId, $StepIdBuscaCampos, $CaseNum, $Campos, $Action, $NovoCaso, $tipoFormulario);
        break;

    case "D":
        $DefaultView = PassoDefaultView($ProcId, $StepId);

// Se o Passo é DefaultView, coloca todos os campos para apresentação
        if ($DefaultView == 1) {
            $Campos = PegaCamposRepositorio($ProcId, $StepId);
        } else {
            $Campos = CamposFormulario($ProcId, $StepIdBuscaCampos);
        }
        $t_body->SHOW_SITUACAO_PROCESSO = "";
        $t_body->DESCRICAO_TAREFA = "Processo";
        $retorno = MontaFormulario($ProcId, $StepId, $CaseNum, $Campos, $Action);
        $t_body->CONTEUDO_FIELDS = $retorno["CONTEUDO"];
        $t_body->block("BLOCK_NORMAL");
        break;
}

// <editor-fold defaultstate="collapsed" desc="Javascript da página">
// Inclui a validação dos campos no Formulário.
$t_body->ARQUIVO_JS = "webcaptura.js";
$t_body->block("BLOCO_JAVASCRIPT");


$t_body->ARQUIVO_JS = "pdf.js";
$t_body->block("BLOCO_JAVASCRIPT_VENDOR");

$t_body->ARQUIVO_JS = "pdf.worker.js";
$t_body->block("BLOCO_JAVASCRIPT_VENDOR");

$t_body->ARQUIVO_JS = "ler_pdf_process.js?006";
$t_body->block("BLOCO_JAVASCRIPT");

$t_body->INCLUDE_JAVASCRIPT = $retorno["VALIDACAO_JAVASCRIPT"];
$t_body->block("BLOCO_INCLUDE_JAVASCRIPT");

// Define a configuração para o Dynamic Twain
$t_body->INCLUDE_JAVASCRIPT = "var dynamic_twain_config = '" . DINAMIC_TWAIN_CONFIG . "'";
$t_body->block("BLOCO_INCLUDE_JAVASCRIPT");

// Faz a verificação 
$t_body->INCLUDE_JAVASCRIPT = "jsDefineonClose();";
$t_body->block("BLOCO_INCLUDE_JAVASCRIPT");

$t_body->ARQUIVO_JS = "selcase_datatable.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "template.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "editcase.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "validacao.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "validadata.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "showdata.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "CLOSE.JS";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "posicionar.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "seluser.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "browserSniffer.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "dynCalendar.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "ajax.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "window.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "historico.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "posicionar.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "menu.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "executa_funcoes_final.js";
$t_body->block("BLOCO_JAVASCRIPT");
//$t_body->ARQUIVO_JS = "magnifier.js";
//$t_body->block("BLOCO_JAVASCRIPT");
// </editor-fold>


$t_body->block("BLOCK_CONTENT");
$t_body->show();
