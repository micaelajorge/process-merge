<?php
/*
Criação: Natan Moraes
Data Criação 08/12/2019
Sistema: Process_XAMPP
 * 
 * @rota pedidos_detalhe
*/

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/func_api.inc");
include_once(FILES_ROOT . "include/toolbar.inc");

use raelgc\view\Template;

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
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/master/t_datatable.html");

// <editor-fold defaultstate="collapsed" desc="Cabecalho da Tabela de casos">
$cabecalhoTabela = array(
    "#",
    "Pedido",
    "Data Cadastro",
    "Status",
    "Natureza",
    "Tipo",
    "Solicitante",
    "Notificação",
    ""
);

//$t_body->addFile("FILTROS", FILES_ROOT . "assets/templates/t_consulta.html");

foreach ($cabecalhoTabela as $cabecalho) {
    $t_body->TEXTO_CABECALHO = $cabecalho;
    $t_body->block("BLOCK_CABECALHO_TABELA");
}
//</editor-fold>

$listaGrid = array(
    "1",
    "teste",
    "01/01/2019",
    "Finalizado",
    "Registro",
    "Cartorio",
    "Pessoa Sobrenome",
    "ExtraJudicial",
    '<a href="pedidos_detalhe" class="fa fa-download" role="button">Download</a href>',
);
foreach ($listaGrid as $linha) {
    $t_body->TEXTO_LINHA = $linha;
    $t_body->block("BLOCK_LINHA_TABELA");
}


$t_body->block("BLOCK_CONTENT");

//// Adiciona o template complemento da Pagina
//$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/t_edituser.html");
//$t_body->block("BLOCK_CONTENT");

$t_body->SIDE_BAR_SIZE = "sidebar-mini sidebar-collapse";

$t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");

if ($mobileMode)
{
    $t_body->addFile("TOOLBAR", FILES_ROOT . "assets/templates/master/t_toolbar.html");
} else {
    $t_body->addFile("TOOLBAR_LEFT", FILES_ROOT . "assets/templates/master/t_toolbar_left.html");
}

//$t_body->addFile("FILTROS", FILES_ROOT . "assets/templates/t_auditoria.html");

// Adiciona o css especifico da página
$t_body->ARQUIVO_CSS = "queue.css";
$t_body->block("BLOCO_CSS");

$t_body->SITE_LOCATION = SITE_LOCATION;
$t_body->TITULO_PAGE = "Detalhe Pedido | Process";
$t_body->HEADER_TEXT = "Detalhe";
$t_body->HEADER_SMALL_TEXT = "Detalhe Pedido";
$t_body->HEADER_ICO = "fa fa fa-tasks";
$t_body->HEADER_COLOR = "#3c8dbc";

$t_body->NAV_HEADER_TEXT = "Detalhe";
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
$t_body->ARQUIVO_JS = "dataTables.init.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");

//$t_body->ARQUIVO_JS = "admin_users.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");

//$t_body->ARQUIVO_JS = "admin_users_datatable.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");

$t_body->ARQUIVO_JS = "pedidos.js"; // Inicializacao Pedidos
$t_body->block("BLOCO_JAVASCRIPT");



// <editor-fold defaultstate="collapsed" desc="Popula toolbar / Side Bar">
    $Menu = "voltar";
    populaToolBarTemplate($t_body, $Menu);
// </editor-fold>
/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>



