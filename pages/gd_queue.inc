<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 21/07/2018
  Sistema: Process
 */

// <editor-fold defaultstate="collapsed" desc="Inicio pagina">
include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/func_api.inc");
include_once(FILES_ROOT . "include/toolbar.inc");
require_once(FILES_ROOT . "include/resource02.php");
require_once(FILES_ROOT . "include/resource01.php");

use raelgc\view\Template;

$dadosEntrada = apiTrataDadosEntrada();
$filtros = apiTrataCampos($dadosEntrada);

$ProcId = PegaProcIdByCode("GATEWAY_DIGITAL");
if (isset($_SESSION["Proc$ProcId"])) {
    if (isset($_SESSION["Proc$ProcId"]["procdef"])) {
        $procdef = $_SESSION["procdef"];
        if ($procdef->ProcId != $ProcId) {
            $procdef = new procdef;
            $procdef->Create($ProcId, $connect);
            $ProcId = $procdef->ProcId;
            $_SESSION[$ProcIdAtivo] = $ProcId;
        }
        if (!is_numeric($StepId)) {
            if ($StepId !== "todos") {
                $StepId = $procdef->StepFiltro;
            } else {
                $StepId = "";
                $procdef->StepFiltro = "";
            }
        } else {
            $procdef->StepFiltro = $StepId;
        }
    }
} else {
    $procdef = new procdef;
    $procdef->Create($ProcId, $connect);
}

$_SESSION["Proc$ProcId"]["procdef"] = $procdef;
$_SESSION["procdef"] = $procdef;

$filtro_queue = $procdef->filterQueue;

$S_Processos = $_SESSION["S_Processos"];

$userdef = $_SESSION["userdef"];
RemoveLocksTemporarios($userdef->UserId);


$PaginaQueueOriginal = "lotesgd";

$_SESSION["PaginaAnterior"] = $PaginaQueueOriginal;
$_SESSION["PaginaQueueOriginal"] = $PaginaQueueOriginal;

$mobileMode = 0;
if (isset($_SESSION["mobileMode"])) {
    $mobileMode = $_SESSION["mobileMode"];
}

$t_body = new Template(FILES_ROOT . "assets/templates/master/t_master.html");
$t_body->addFile("PAGEHEADER", FILES_ROOT . "assets/templates/master/t_header.html");


$t_body->addFile("PAGEMENULEFT", FILES_ROOT . "assets/templates/master/t_menu.html");

$t_body->CONTENT_HEADER_HIDEN = "hidden";
$t_body->SIDEBAR_HIDE = "hidden";

$t_body->HIDE_SEL_ORIGENS = " ";

// Adiciona o template para mostrar a lista de Registros
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/t_gd_queue_div.html");
$t_body->addFile("QUEUE_FORM", FILES_ROOT . "assets/templates/t_queue.html");

/**
 *  Criação dos campos da TELA
 */

/**
 *  Campo lista de clientes
 */
$FieldId = PegaFieldIdByCode($ProcId, "CLIENTES_VINCULADOS");
include(FILES_ROOT . "pages/gateway_clientes.inc");

$clienteSelecionado = "";
$listaClientes = Inicializacao_gateway_clientes($FieldId, 0, $clienteSelecionado, "");
$t_body->CAMPO_LISTA_CLIENTES = $listaClientes;

/**
 * Campo de Status do Lote
 */

$FieldId = PegaFieldIdByCode($ProcId, "STATUS");
$t_body->FIELDID = $FieldId;

$t_body->WRAPPER_STYLE = "style=\"width: 100%; margin-left: 0px !important; \"";

$dadosLista = PegaDadosCampoLista($ProcId, $FieldId, "", "");
foreach ($dadosLista as $item)
{
    $t_body->ITEM_VALOR = $item["Display"];
    $t_body->ITEM_DESC = $item["Valor"];
    $t_body->block("BLOCK_STATUS_ITEM");
}


$t_body->SHOW_LOADING = " ";
$t_body->block("BLOCK_CONTENT");

if (count($S_Processos) > 1) {
    $t_body->SIDE_BAR_SIZE = "sidebar-mini sidebar-collapse";
} else {
    $t_body->SIDE_BAR_SIZE = "sidebar-mini sidebar-collapse";
    //$t_body->SIDEBAR_HIDE = "hide";
}

$t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");
$t_body->addFile("TOOLBAR_LEFT", FILES_ROOT . "assets/templates/master/t_toolbar_left.html");
$t_body->addFile("INCLUDE_JAVASCRIPT", FILES_ROOT . "assets/templates/t_queuescript.html");

$t_body->SITE_LOCATION = SITE_LOCATION;
$t_body->TITULO_PAGE = "Consulta de Protocolos de Registro | " . INSTANCENAME;
if ($mobileMode) {
    $t_body->TOP_NAME = "Consulta de Protocolos de Registro";
    $t_body->CONTENT_HEADER_HIDEN = "hide";
} else {
    $t_body->TOP_NAME = APP_NAME;
}

$t_body->HEADER_TEXT = "$procdef->ProcName";
$t_body->HEADER_SMALL_TEXT = "$procdef->ProcDesc";
$t_body->HEADER_ICO = "fa $procdef->ProcIcon";
$t_body->HEADER_COLOR = "$procdef->ProcColor";
$t_body->MAIN_FOOTER_HIDEN = "hide";
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Final do Template">
/**
 * Seta o Processo de edição
 */
$t_body->PROCID = "GATEWAY_DIGITAL";

if (count($S_Processos) == 1) {
    $t_body->NAVEGACAO_ENTRADA_CLASSE = "hidden";
    $t_body->LINKENTRADA = "javascript:";
}

if (is_numeric($StepId)) {
    $t_body->NAGEVAGAO_LINK = "queue/$ProcId/todos/";
} else {
    $t_body->NAGEVAGAO_LINK = "javascript:";
}

$t_body->NAV_HEADER_TEXT = "$procdef->ProcName";
$t_body->NAV_HEADER_ICO = "fa $procdef->ProcIcon";
$t_body->NAV_HEADER_CLASS = "text-warning";
$t_body->NAV_HEADER_COLOR = "$procdef->ProcColor";
$t_body->block("BLOCO_NAVEGACAO"); // Inclui na navegação do Site

if (is_array($filtro_queue)) {
    $t_body->EXTEND_NAV_HIDDEN = " ";
    $t_body->EXTEND_NAGEVAGAO_LINK = "javascript:";
    $t_body->EXTEND_NAV_HEADER_ICO = "fa fa-filter";

    $concatenador = "";
    $valorFiltro = "";
    foreach ($filtro_queue as $campo) {
        $valorCampo = $campo["valor"];
        if ($campo["tipo"] == "DT") {
            $valorCampo = ConvDate($valorCampo);
        }
        $valorFiltro .= $concatenador . " " . $campo["nome"] . " = " . $valorCampo;
        $concatenador = " e ";
    }
    $t_body->EXTEND_NAV_HEADER_TEXT = $valorFiltro;
}

if (is_numeric($StepId)) {
    $stepName = PegaNomeStep($ProcId, $StepId);
    $t_body->NAV_HEADER_TEXT = $stepName;
    $t_body->NAV_HEADER_ICO = "fa fa-eyedropper";
    $t_body->NAV_HEADER_CLASS = "text-primary";
    $t_body->NAV_HEADER_COLOR = "";
    $t_body->NAGEVAGAO_LINK = "javascript:";
    $t_body->block("BLOCO_NAVEGACAO"); // Inclui na navegação do Site
}

$t_body->block("BLOCO_MOSTRAR_NAVEGACAO");

// Esconde a Navegacao se tiver apenas um Processo

$t_body->PROCNAME = $procdef->ProcName;

// Definições do Usuário
$t_body->USER_DESC = $userdef->UserDesc;
$t_body->USER_NAME = $userdef->UserName;
$t_body->USER_ORIGEM = $userdef->Origem;
$t_body->USER_EMAIL = $userdef->Email;

// Insere o ProcId para tratamento no Datatable
$t_body->SCRIPT_DATA = "var ProcId = '$ProcId';";
$t_body->block("BLOCO_JAVASCRIPT_INSERTS");

// Insere o ProcId para tratamento no Datatable
$t_body->SCRIPT_DATA = "var tipoProcesso = '$procdef->TipoProc'";
$t_body->block("BLOCO_JAVASCRIPT_INSERTS");

$t_body->NOVOCASOSTEPID = "INICIO";
$t_body->block("BLOCK_STEPIDNOVOCASO");
$t_body->block("BLOCO_INCLUDE_JAVASCRIPT");

// Insere o StepId para tratamento no Datatable
$t_body->SCRIPT_DATA = "var StepId = '$StepId';";
$t_body->block("BLOCO_JAVASCRIPT_INSERTS");

$t_body->ARQUIVO_JS = "template.js"; // jsTratamentoDataTable
$t_body->block("BLOCO_JAVASCRIPT");

$t_body->ARQUIVO_JS = "queue.js"; // jsTratamentoDataTable
$t_body->block("BLOCO_JAVASCRIPT");

$t_body->ARQUIVO_JS = "gd_queue.js"; // jsTratamentoDataTable
$t_body->block("BLOCO_JAVASCRIPT");

populaToolBarTemplate($t_body, $Menu);

$t_body->show();


// </editor-fold>    