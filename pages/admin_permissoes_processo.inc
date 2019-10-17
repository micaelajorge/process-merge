<?php
/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 19/09/2018
Sistema: Process_XAMPP
 * 
 * 
*/

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/func_api.inc");
include_once(FILES_ROOT . "include/toolbar.inc");
include_once(FILES_ROOT . "include/resource01.php");

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
    "Formulário",
    "Grupos/Usuários"    
);

foreach ($cabecalhoTabela as $cabecalho) {
    $t_body->TEXTO_CABECALHO = $cabecalho;
    $t_body->block("BLOCK_CABECALHO_TABELA");
}
// </editor-fold>

$t_body->block("BLOCK_CONTENT");

$t_body->SIDE_BAR_SIZE = "sidebar-mini sidebar-collapse";

$t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");

if ($mobileMode)
{
    $t_body->addFile("TOOLBAR", FILES_ROOT . "assets/templates/master/t_toolbar.html");
} else {
    $t_body->addFile("TOOLBAR_LEFT", FILES_ROOT . "assets/templates/master/t_toolbar_left.html");
}

//$t_body->addFile("FILTROS", FILES_ROOT . "assets/templates/t_auditoria.html");

$_SESSION["procIdPermissoes"] = $procId;

$procName = PegaNomeProc($procId);

$t_body->SITE_LOCATION = SITE_LOCATION;
$t_body->TITULO_PAGE = "Permissões | Process";
$t_body->HEADER_TEXT = $procName;
$t_body->HEADER_SMALL_TEXT = "Permissões da Solução";

$tipoProc = PegaTipoProc($procId);

switch ($tipoProc)
{
    case "RP": 
        $t_body->HEADER_ICO = "fa fa-files-o";
        $t_body->NAV_HEADER_CLASS = "text-green";
        break;    
    case "CT": 
        $t_body->HEADER_ICO = "fa fa-tags";
        $t_body->NAV_HEADER_CLASS = "text-warning";
        break;
    default: 
        $t_body->NAV_HEADER_CLASS = "text-primary";
        $t_body->HEADER_ICO = "fa fa-gears";
        break;
}


$t_body->HEADER_COLOR = "#3c8dbc";

$t_body->NAGEVAGAO_LINK = "permissoes";
$t_body->NAV_HEADER_TEXT = "Permissões em Processo";

// Inclui na navegação do Site
$t_body->block("BLOCO_NAVEGACAO"); 

$t_body->NAV_HEADER_TEXT = $procName;
$t_body->NAV_HEADER_ICO = "fa fa-list";

$t_body->NAGEVAGAO_LINK = "javascript:";

// Inclui na navegação do Site
$t_body->block("BLOCO_NAVEGACAO"); 
$t_body->block("BLOCO_MOSTRAR_NAVEGACAO");

$t_body->PROCID = $ProcId;
$t_body->PROCNAME = $procdef->ProcName;

// Definições do Usuário
$t_body->USER_DESC = $userdef->UserDesc;
$t_body->USER_NAME = $userdef->UserName;
$t_body->USER_ORIGEM = $userdef->Origem;
$t_body->USER_EMAIL = $userdef->Email;


$t_body->MAIN_FOOTER_HIDEN = "hide";

// Inclusão Javascript da Página
$t_body->ARQUIVO_JS = "seluser.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");

$t_body->ARQUIVO_JS = "dataTables.init.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");

$t_body->ARQUIVO_JS = "admin_permissoes_processo.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");

$t_body->ARQUIVO_JS = "admin_permissoes_processo_datatable.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");

// <editor-fold defaultstate="collapsed" desc="Popula toolbar / Side Bar">
$Menu = 57;
populaToolBarTemplate($t_body, $Menu);
//$t_body->block("BLOCK_SIDEBAR");
// </editor-fold>
/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>



