<?php
/*
Criação: Micaela Fernanda
Data Criação 28/11/2019
Sistema: Process_XAMPP
 * 
 * ROTA: billing
*/

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/func_api.inc");
include_once(FILES_ROOT . "include/toolbar.inc");
require_once(FILES_ROOT . "api/billing/api_relatorioonline.inc");

use raelgc\view\Template;

$nomeDataTable = "tabelaResultado"; 
$dadosEntrada = apiTrataDadosEntrada();
$filtros = apiTrataCampos($dadosEntrada);

$mobileMode = 0;
if (isset($_SESSION["mobileMode"]))
{
    $mobileMode = $_SESSION["mobileMode"];
}

$t_body = new Template(FILES_ROOT . "assets/templates/master/t_master.html");
$t_body->addFile("PAGEHEADER", FILES_ROOT . "assets/templates/master/t_header.html");
$t_body->addFile("PAGEMENULEFT", FILES_ROOT . "assets/templates/master/t_menu.html");

// Adiciona o template de DATATABLE
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/billing/t_cube.html");
$t_body->CLASS_BOX_HIDE = " ";
$t_body->block("BLOCK_CONTENT");

$t_body->addFile("FILTROS", FILES_ROOT . "assets/templates/billing/t_relatorio_online_filtro.html");

// Cria lista de mes/ano para filtro
//$t_body->MES_ANO_SELECAO = "11/2019";
//$t_body->block("BLOCK_MES_ANO_SELECAO");
//$t_body->MES_ANO_SELECAO = "12/2019";
//$t_body->block("BLOCK_MES_ANO_SELECAO");

$ultimoMes = "12/2019";

// Usado para definir o Ultimo mês a ser mostrado na lista da dimensão de meses
$mesAnoInicial = "12/2019";

//$t_body->DIMENSAO_RELONLINE = 1;
//
//$t_body->SHOW_LOADING = "";
//
//$t_body->CLASS_BOX_DATATABLE = "box-solid box-primary";
//$t_body->CLASS_HEADER_BOX_DATATABLE = "initial";
//$t_body->CLASS_DATATABLE_WRAPPER_HIDE = "initial";
//$t_body->CLASS_BOX_DATATABLE_HIDE = "initial";
//$t_body->block("BLOCK_CONTENT");

//$t_body->SIDEBAR_HIDE = "hide";
//$t_body->WRAPPER_STYLE = "style=\"margin-left:0px !important\"";


//$t_body->CUBE_DATATABLE = MontaCuboOne();
//$t_boby->block("BLOCO_DATATABLE");

//// Adiciona o template complemento da Pagina
$t_body->block("BLOCK_CONTENT");

$t_body->SIDE_BAR_SIZE = "sidebar-mini sidebar-collapse";
$t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");

if ($mobileMode)
{
    $t_body->addFile("TOOLBAR", FILES_ROOT . "assets/templates/master/t_toolbar.html");
} else {
    $t_body->addFile("TOOLBAR_LEFT", FILES_ROOT . "assets/templates/master/t_toolbar_left.html");
}

// Adiciona o css especifico da página
$t_body->ARQUIVO_CSS = "queue.css";
$t_body->block("BLOCO_CSS");

$t_body->SITE_LOCATION = SITE_LOCATION;
$t_body->TITULO_PAGE = "Portal Billing";
$t_body->HEADER_TEXT = "Billing";
$t_body->HEADER_SMALL_TEXT = "Portal Billing";
$t_body->HEADER_ICO = "fa fa fa-tasks";
$t_body->HEADER_COLOR = "#3c8dbc";

$t_body->NAV_HEADER_TEXT = "Billing";
$t_body->NAV_HEADER_ICO = "fa fa fa-tasks";
$t_body->NAV_HEADER_CLASS = "text-primary";
$t_body->NAV_HEADER_COLOR = "";

$t_body->NAGEVAGAO_LINK = "javascript:";
$t_body->block("BLOCO_NAVEGACAO"); // Inclui na navegação do Site
$t_body->block("BLOCO_MOSTRAR_NAVEGACAO");

$t_body->PROCID = $ProcId;
$t_body->PROCNAME = $procdef->ProcName;

// Definições do Pedido
//$t_body->USER_DESC = $userdef->UserDesc;

// Inclusão Javascript da Página
//$t_body->ARQUIVO_JS = "dataTables.init.js"; // Incicializacao DataTables
//$t_body->block("BLOCO_JAVASCRIPT");
//
//$t_body->ARQUIVO_JS = "Buttons-1.5.4/js/dataTables.buttons.min.js";
//$t_body->block("BLOCO_JAVASCRIPT_VENDOR");

$Menu = "voltar";
populaToolBarTemplate($t_body, $Menu);

$t_body->show();




