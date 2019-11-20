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
$t_body->addFile("FILTROS", FILES_ROOT . "assets/templates/t_relatorio_online_filtro.html");                
$t_body->addFile("DATA_TABLE_CONTENT", FILES_ROOT . "assets/templates/master/t_relatorio_online_datatable.html");

$t_body->BODY_STYLE = "overflow: scroll";

// Cria lista de mes/ano para filtro

$SQL = "select concat(month(campo15), '/', year(campo15)) as periodo, month(campo15) as mes, year(campo15) as ano from exportkeys where procid = 75 group by ano, mes order by ano desc, mes desc";
$Query = mysqli_query($connect, $SQL);

$listaMeses = mysqli_fetch_all($Query, MYSQLI_ASSOC);

foreach ($listaMeses as $mesAno)
{
    $t_body->MES_ANO_SELECAO = str_pad($mesAno["mes"], 2, '0', STR_PAD_LEFT) . "/" . $mesAno["ano"];
    $t_body->block("BLOCK_MES_ANO_SELECAO");
}

$ultimoMes = $listaMeses[0];

// Usado para definir o Ultimo mês a ser mostrado na lista da dimensão de meses
$mesAnoInicial = str_pad($ultimoMes["mes"], 2, '0', STR_PAD_LEFT) . "/" . $ultimoMes["ano"];

// <editor-fold defaultstate="collapsed" desc="Cabecalho da Tabela">
$cabecalhoTabela = array(
    "Período",
    "Volume Recebido",
    "Documentos não Digitalizados",
    "Em Fila de Análise",
    "% Tratativa",
    "Analisadas",
    "Regulares",
    "% Eficiência Regulares",
    "Irregulares",
    "% Eficiência Irregulares",
    "Dentro SLA",
    "Fora SLA"
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

// Insere o ProcId para tratamento no Datatable
$t_body->SCRIPT_DATA = "var ProcId = '$ProcId';";
$t_body->block("BLOCO_JAVASCRIPT_INSERTS");

// Define o ultimo mes a ser mostrado na dimensão de Meses
$t_body->SCRIPT_DATA = "var mesAnoInicial = '/$mesAnoInicial';";
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

$t_body->ARQUIVO_JS = "relatorioonline.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");

$t_body->ARQUIVO_JS = "relatorioonline_datatable.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");

$t_body->ARQUIVO_JS = "relatorioonline_tribanco.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");

//$t_body->block("BLOCO_NAO_IMPEDE_PAGINA_DUPLICADA");


// <editor-fold defaultstate="collapsed" desc="Popula toolbar / Side Bar">
$Menu = "voltar";
populaToolBarTemplate($t_body, $Menu);
//$t_body->block("BLOCK_SIDEBAR");
// </editor-fold>
/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>


