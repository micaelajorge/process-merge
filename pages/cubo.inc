<?php
/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 21/07/2018
  Sistema: Process
 */

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/func_api.inc");
include_once(FILES_ROOT . "include/toolbar.inc");
require_once(FILES_ROOT . "include/resource02.php");

use raelgc\view\Template;

$dadosEntrada = apiTrataDadosEntrada();
$filtros = apiTrataCampos($dadosEntrada);

if (isset($_SESSION[$ProcId]["procdef"])) {
    $procdef = $_SESSION["procdef"];
    if ($procdef->ProcId != $ProcId) {
        $procdef = new procdef;
        $procdef->Create($ProcId, $connect);
        $_SESSION[$ProcId]["procdef"] = $procdef;
        $ProcId = $procdef->ProcId;
        $_SESSION[$ProcIdAtivo] = $ProcId;
    }
} else {
    $procdef = new procdef;
    $procdef->Create($ProcId, $connect);
    $_SESSION[$ProcId]["procdef"] = $procdef;
    $_SESSION["procdef"] = $procdef;
}

$userdef = $_SESSION["userdef"];
RemoveLocksTemporarios($userdef->UserId);


$_SESSION["PaginaQueueOriginal"] = "queue/$procdef->ProcId";
$_SESSION["PaginaAnterior"] = "queue/$procdef->ProcId";

$mobileMode = 0;
if (isset($_SESSION["mobileMode"])) {
    $mobileMode = $_SESSION["mobileMode"];
}

$t_body = new Template(FILES_ROOT . "assets/templates/master/t_master.html");
$t_body->addFile("PAGEHEADER", FILES_ROOT . "assets/templates/master/t_header.html");
$t_body->addFile("PAGEMENULEFT", FILES_ROOT . "assets/templates/master/t_menu.html");
$t_body->addFile("TOOLBAR_TOP", FILES_ROOT . "assets/templates/master/t_toolbar_top.html");                

// Adiciona o template de DATATABLE
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/master/t_cube_datatable2.html");

// <editor-fold defaultstate="collapsed" desc="Cabecalho da Tabela de casos">
$cabecalhoTabela = array(
    "",
    "Período",
    "PeródoSel",
    "Produção",
    "Não Digitalizado",
    "Em Análise",
    "% Desvio Prod Pendente",
    "Analisadas",
    "Regulares",
    "Irregulares",
    "% Desvio Analizados",
    "Total Erros",
    "% Desvio Total"
);

//foreach ($cabecalhoTabela as $cabecalho) {
//    $t_body->TEXTO_CABECALHO = $cabecalho;
//    $t_body->block("BLOCK_CABECALHO_TABELA");
//}

foreach ($cabecalhoTabela as $cabecalho) {
    $t_body->TEXTO_CABECALHO = $cabecalho;
    $t_body->block("BLOCK_CABECALHO_TABELA_CUBO");
}
$t_body->block("BLOCK_CABECALHO_CUBO");

////$t_body->TEXTO_CABECALHO = '';
////$t_body->COLSPAN_CABECALHO = '1';
////$t_body->block("BLOCK_CABECALHO_TABELA_CUBO");
//$t_body->TEXTO_CABECALHO = 'Gerente Area';
//$t_body->COLSPAN_CABECALHO = '1';
//$t_body->block("BLOCK_CABECALHO_TABELA_CUBO");
//$t_body->TEXTO_CABECALHO = '';
//$t_body->COLSPAN_CABECALHO = '10';
//$t_body->block("BLOCK_CABECALHO_TABELA_CUBO");
//$t_body->block("BLOCK_CABECALHO_CUBO");


// </editor-fold>
$t_body->CLASS_BOX_DATATABLE = "box-solid box-primary";
$t_body->CLASS_HEADER_BOX_DATATABLE = "";
$t_body->block("BLOCK_CONTENT");

$t_body->SIDEBAR_HIDE = "hide";
$t_body->WRAPPER_STYLE = "style=\"margin-left:0px !important\"";

$S_Processos = $_SESSION["S_Processos"];

// Esconde o cabecalho
//$t_body->CONTENT_HEADER_HIDEN = "hidden";
$t_body->MAIN_FOOTER_HIDEN = "hidden";

if (count($S_Processos) > 1) {
    $t_body->SIDE_BAR_SIZE = "sidebar-mini sidebar-collapse";
} else {
    $t_body->SIDE_BAR_SIZE = "sidebar-mini sidebar-collapse";    
}

$t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");

//$t_body->addFile("FILTROS", FILES_ROOT . "assets/templates/t_auditoria.html");
// Adiciona o css especifico da página
$t_body->ARQUIVO_CSS = "cubo.css";
$t_body->block("BLOCO_CSS");

$t_body->SITE_LOCATION = SITE_LOCATION;
$t_body->TITULO_PAGE = "Relatórios $procdef->ProcName | Process";
$t_body->HEADER_TEXT = "$procdef->ProcName";
$t_body->HEADER_SMALL_TEXT = "Relatórios";
$t_body->HEADER_ICO = "fa fa-cube";
$t_body->HEADER_COLOR = "$procdef->ProcColor";

$t_body->NAV_HEADER_TEXT = "$procdef->ProcName";
$t_body->NAV_HEADER_ICO = "fa $procdef->ProcIcon";
$t_body->NAV_HEADER_CLASS = "text-primary";
$t_body->NAV_HEADER_COLOR = "$procdef->ProcColor";

$t_body->NAGEVAGAO_LINK = "javascript:";

$t_body->MAIN_FOOTER_HIDEN = "hidden";

// Esconde a Navegacao se tiver apenas um Processo
if (count($S_Processos) > 1) {
    $t_body->block("BLOCO_NAVEGACAO"); // Inclui na navegação do Site
    $t_body->block("BLOCO_MOSTRAR_NAVEGACAO");
}

$t_body->PROCID = $ProcId;
$t_body->PROCNAME = $procdef->ProcName;

// Definições do Usuário
$t_body->USER_DESC = $userdef->UserDesc;
$t_body->USER_NAME = $userdef->UserName;
$t_body->USER_ORIGEM = $userdef->Origem;
$t_body->USER_EMAIL = $userdef->Email;

// Insere o ProcId para tratamento no Datatable
$t_body->SCRIPT_DATA = "var ProcId = '$ProcId';";
$t_body->block("BLOCO_JAVASCRIPT_INSERTS");

// Inclusão Javascript da Página
$t_body->ARQUIVO_JS = "dataTables.init.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");
//$t_body->ARQUIVO_JS = "dataTables.init.js"; // Incicializacao DataTables
//$t_body->block("BLOCO_JAVASCRIPT");

$t_body->ARQUIVO_JS = "Buttons-1.5.4/js/dataTables.buttons.min.js";
$t_body->block("BLOCO_JAVASCRIPT_VENDOR");

$t_body->ARQUIVO_JS = "pdfmake-0.1.36/vfs_fonts.js";
$t_body->block("BLOCO_JAVASCRIPT_VENDOR");

$t_body->ARQUIVO_JS = "pdfmake-0.1.36/pdfmake.min.js"; // Botoes Exportacao
$t_body->block("BLOCO_JAVASCRIPT_VENDOR");

$t_body->ARQUIVO_JS = "Buttons-1.5.4/js/buttons.html5.js"; // Botoes Exportacao
$t_body->block("BLOCO_JAVASCRIPT_VENDOR");

$t_body->ARQUIVO_JS = "Buttons-1.5.4/js/buttons.flash.min.js"; // Botoes Exportacao
$t_body->block("BLOCO_JAVASCRIPT_VENDOR");

$t_body->ARQUIVO_JS = "Buttons-1.5.4/js/buttons.print.min.js";
$t_body->block("BLOCO_JAVASCRIPT_VENDOR");

$t_body->ARQUIVO_JS = "JSZip-2.5.0/jszip.min.js";
$t_body->block("BLOCO_JAVASCRIPT_VENDOR");

$t_body->ARQUIVO_JS = "cubo_datatable.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "cubo.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");


$t_body->ARQUIVO_JS = "cubo_tribanco.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");

$t_body->INCLUDE_JAVASCRIPT = "jsAcoesCuboRelatorio();";
$t_body->block("BLOCO_INCLUDE_JAVASCRIPT");

// <editor-fold defaultstate="collapsed" desc="Popula toolbar / Side Bar">
$Menu = "voltar";
populaToolBarTemplate($t_body, $Menu);
//$t_body->block("BLOCK_SIDEBAR");
// </editor-fold>
/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>


