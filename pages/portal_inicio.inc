<?php

/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 09/11/2019
Sistema: Process_XAMPP
*/

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/func_api.inc");
include_once(FILES_ROOT . "include/toolbar.inc");
require_once(FILES_ROOT . "include/resource02.php");
require_once(FILES_ROOT . "include/resource01.php");

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
    if (!isset($ProcId))
    {
        $ProcId = PegaProcIdByCode("TRIBANCO");
    }
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
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/master/t_relatorio_online.html");
$t_body->addFile("DATA_TABLE_CONTENT", FILES_ROOT . "assets/templates/master/t_relatorio_online_datatable.html");

$t_body->DIMENSAO_RELONLINE = 1;

$t_body->SHOW_LOADING = "";

$t_body->CLASS_BOX_DATATABLE = "box-solid box-primary";
$t_body->CLASS_HEADER_BOX_DATATABLE = "initial";
$t_body->CLASS_DATATABLE_WRAPPER_HIDE = "initial";
$t_body->CLASS_BOX_DATATABLE_HIDE = "initial";
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
//$t_body->ARQUIVO_CSS = "cubo.css";
//$t_body->block("BLOCO_CSS");

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

//$t_body->block("BLOCO_NAO_IMPEDE_PAGINA_DUPLICADA");


// <editor-fold defaultstate="collapsed" desc="Popula toolbar / Side Bar">
$Menu = "voltar";
populaToolBarTemplate($t_body, $Menu);
//$t_body->block("BLOCK_SIDEBAR");
// </editor-fold>
/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>



