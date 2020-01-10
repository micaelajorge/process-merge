<?php
/*
Criação: Micaela Fernanda
Data Criação 26/12/2019
Sistema: Process_XAMPP
 * 
 * ROTA: protesto
*/

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/func_api.inc");
include_once(FILES_ROOT . "include/toolbar.inc");

use raelgc\view\Template;

$mobileMode = 0;
if (isset($_SESSION["mobileMode"]))
{
    $mobileMode = $_SESSION["mobileMode"];
}

$t_body = new Template(FILES_ROOT . "assets/templates/master/t_master.html");
$t_body->addFile("PAGEHEADER", FILES_ROOT . "assets/templates/master/t_header.html");
$t_body->addFile("PAGEMENULEFT", FILES_ROOT . "assets/templates/master/t_menu.html");

// Adiciona o template de DATATABLE
// Adiciona o template de DATATABLE
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/master/t_datatable.html");

// <editor-fold defaultstate="collapsed" desc="Cabecalho da Tabela de casos">
$cabecalhoTabela = array(
    "#",
    "",
    "Status",
    "",
    "Referências"
);

foreach ($cabecalhoTabela as $cabecalho) {
    $t_body->TEXTO_CABECALHO = $cabecalho;
    $t_body->block("BLOCK_CABECALHO_TABELA");
}

$t_body->CLASS_BOX_HIDE = " ";
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
$t_body->TITULO_PAGE = "Consulta de Protestos";
$t_body->HEADER_TEXT = "Protestos";
$t_body->HEADER_SMALL_TEXT = "Consulta de Protestos";
$t_body->HEADER_ICO = "fa fa fa-tasks";
$t_body->HEADER_COLOR = "#3c8dbc";

$t_body->NAV_HEADER_TEXT = "Protestos";
$t_body->NAV_HEADER_ICO = "fa fa fa-tasks";
$t_body->NAV_HEADER_CLASS = "text-primary";
$t_body->NAV_HEADER_COLOR = "";

$t_body->NAGEVAGAO_LINK = "javascript:";
$t_body->block("BLOCO_NAVEGACAO"); // Inclui na navegação do Site
$t_body->block("BLOCO_MOSTRAR_NAVEGACAO");

$t_body->PROCID = $ProcId;
$t_body->PROCNAME = $procdef->ProcName;

$Menu = "1";
populaToolBarTemplate($t_body, $Menu);

$t_body->show();




