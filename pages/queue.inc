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

$S_Processos = $_SESSION["S_Processos"];

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
} else {
    $procdef = new procdef;
    $procdef->Create($ProcId, $connect);
}
$_SESSION["Proc$ProcId"]["procdef"] = $procdef;
$_SESSION["procdef"] = $procdef;

$filtro_queue = $procdef->filterQueue;

if ($procdef->ExendProps != "") {
    $obj_ExtenProps = json_decode($procdef->ExendProps, true);
    $sortSelected = $obj_ExtenProps["queueOrder"];
    
    // Verifica a posição do Filtro
    $filterPosition = $obj_ExtenProps["filterPosition"];
}
if ($sortSelected !== "desc")
{
    $sortSelected = "asc";
}
$queueSort = "var sort = [0, \"$sortSelected\"];";

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

// Adiciona o template de DATATABLE
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/master/t_datatable.html");

// <editor-fold defaultstate="collapsed" desc="Cabecalho da Tabela de casos">

switch ($procdef->TipoProc) {
    case "CT":
        $cabecalhoTabela = array(
            array("Titulo" => "#", "class-display" => ""),
            array("Titulo" => "", "class-display" => ""),
            array("Titulo" => "Processo", "class-display" => ""),
            array("Titulo" => "Situação", "class-display" => ""),
            array("Titulo" => "Referências", "class-display" => ""),
            array("Titulo" => "Referências", "class-display" => "visible-md visible-xm visible-xl")
        );
        break;
    case "RP":
        $cabecalhoTabela = array(
            array("Titulo" => "#", "class-display" => ""),
            array("Titulo" => "", "class-display" => ""),
            array("Titulo" => "", "class-display" => ""),
            array("Titulo" => "Documento", "class-display" => ""),
            array("Titulo" => "Referências", "class-display" => ""),
            array("Titulo" => "Referências", "class-display" => "visible-md visible-xm visible-xl")
        );
        break;
    default:
        $cabecalhoTabela = array(
            array("Titulo" => cabNumeroCaso, "class-display" => ""),
            array("Titulo" => "", "class-display" => ""),
            array("Titulo" => "", "class-display" => ""),
            array("Titulo" => "Tarefa", "class-display" => ""),
            array("Titulo" => "Referências", "class-display" => "visible-lg")
                //array ("Titulo" => "Referências", "class-display" => "visible-md visible-xm visible-xl")
        );
        break;
}

foreach ($cabecalhoTabela as $cabecalho) {
    $t_body->CLASSE_DISPLAY_CABECALHO = $cabecalho["class-display"];
    $t_body->TEXTO_CABECALHO = $cabecalho["Titulo"];
    $t_body->block("BLOCK_CABECALHO_TABELA");
}
// </editor-fold>

$t_body->block("BLOCK_CONTENT");


// Adiciona o template complemento da Pagina
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/t_queue_form.html");
$t_body->block("BLOCK_CONTENT");


if (count($S_Processos) > 1) {
    $t_body->SIDE_BAR_SIZE = "sidebar-mini sidebar-collapse";
} else {
    $t_body->SIDE_BAR_SIZE = "sidebar-mini sidebar-collapse";
    //$t_body->SIDEBAR_HIDE = "hide";
}


$t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");

//if ($mobileMode) {
//    $t_body->addFile("TOOLBAR", FILES_ROOT . "assets/templates/master/t_toolbar.html");
//} else {
//    $t_body->addFile("TOOLBAR_LEFT", FILES_ROOT . "assets/templates/master/t_toolbar_left.html");
//    //$t_body->addFile("TOOLBAR_TOP", FILES_ROOT . "assets/templates/master/t_toolbar_left.html");
$t_body->addFile("TOOLBAR_LEFT", FILES_ROOT . "assets/templates/master/t_toolbar_left.html");


if ($procdef->TipoProc == "CT") {
    //$t_body->addFile("TOPO_PAGINA", FILES_ROOT . "assets/templates/t_busca_campos.html");
}

if ($filterPosition === "filterTop")
{
    $t_body->addFile("BOX_FILTROS", FILES_ROOT . "assets/templates/master/t_box_pesquisa.html");
    $t_body->ARQUIVO_JS = "consultabasica.js";
    $t_body->block("BLOCO_JAVASCRIPT");    
}
    


//$t_body->addFile("FILTROS", FILES_ROOT . "assets/templates/t_auditoria.html");
// Adiciona o css especifico da página
$t_body->ARQUIVO_CSS = "queue.css";
$t_body->block("BLOCO_CSS");

$t_body->SITE_LOCATION = SITE_LOCATION;
$t_body->TITULO_PAGE = "$procdef->ProcName | " . INSTANCENAME;
if ($mobileMode) {
    $t_body->TOP_NAME = "$procdef->ProcName";
    $t_body->CONTENT_HEADER_HIDEN = "hide";
} else {
    $t_body->TOP_NAME = APP_NAME;
}

$t_body->HEADER_TEXT = "$procdef->ProcName";
$t_body->HEADER_SMALL_TEXT = "$procdef->ProcDesc";
$t_body->HEADER_ICO = "fa $procdef->ProcIcon";
$t_body->HEADER_COLOR = "$procdef->ProcColor";
$t_body->MAIN_FOOTER_HIDEN = "hide";

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
    foreach ($filtro_queue as $filtro) {
        if (key_exists("filter", $filtro))
        {
            $filtro = $filtro["filter"];
        }
        if (key_exists("filtro", $filtro))
        {
            $filtro = $filtro["filtro"];
        }        
        $valorCampo = $filtro["valor"];
        if ($filtro["tipo"] == "DT") {
            $valorCampo = ConvDate($valorCampo[0]);
        }
        $valorFiltro .= $concatenador . " " . $filtro["nome"] . " = " . $valorCampo;
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

// Insere o ProcId para tratamento no Datatable
$t_body->SCRIPT_DATA = "var tipoProcesso = '$procdef->TipoProc'";
$t_body->block("BLOCO_JAVASCRIPT_INSERTS");


// Insere o StepId para tratamento no Datatable
$t_body->SCRIPT_DATA = "var StepId = '$StepId';";
$t_body->block("BLOCO_JAVASCRIPT_INSERTS");

// Define a order de aparesentação n fila
$t_body->SCRIPT_DATA = "$queueSort";
$t_body->block("BLOCO_JAVASCRIPT_INSERTS");


// Inclusão Javascript da Página
$t_body->ARQUIVO_JS = "dataTables.init.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");

$t_body->ARQUIVO_JS = "executa_funcoes_final.js";
$t_body->block("BLOCO_JAVASCRIPT");

$t_body->ARQUIVO_JS = "queue_datatable.js"; // jsTratamentoDataTable
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "queue.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->addFile("INCLUDE_JAVASCRIPT", FILES_ROOT . "assets/templates/t_queuescript.html");

// <editor-fold defaultstate="collapsed" desc="Pega Passos Novo caso">
$SQL = "( select SD.StepId, StepName, PageAction from stepaddresses as SA, stepdef as SD where SD.ProcId = $ProcId and SA.PRocId = SD.ProcId and SA.StepId = SD.StepId and GroupId = $userdef->UserId and NewDoc = 1 and GrpFld = 'U' and Active = 1) 
		union 
		( select SD.StepId, StepName, PageAction from stepaddresses as SA, stepdef as SD where SD.ProcId = $ProcId and SA.PRocId = SD.ProcId and SA.StepId = SD.StepId and GroupId in ($userdef->usergroups) and NewDoc = 1 and GrpFld = 'G' and Active = 1 ) order by StepName";
$QUERY_PASSOSCRIARNOVOCASO = mysqli_query($connect, $SQL);
$TotalPassosNovoDoc = mysqli_num_rows($QUERY_PASSOSCRIARNOVOCASO);

if ($procdef->TipoProc == "CT" & false) {
    $t_body->ARQUIVO_JS = "webcaptura.js"; // Incicializacao DataTables
    $t_body->block("BLOCO_JAVASCRIPT");

    $Menu = 1;
    if ($TotalPassosNovoDoc > 1) {
        $Menu = 50;
    }
} else {
    switch ($TotalPassosNovoDoc) {
        case 0:
            $Menu = 48;
            if (count($S_Processos) == 1 && !$userdef->Admin && !$ForcarSelecaoProcesso) {
                $Menu = 49;
            }
            break;
        case 1:
            $Menu = 1;
            if (count($S_Processos) == 1 && !$userdef->Admin && !$ForcarSelecaoProcesso) {
                $Menu = 27;
            }
            $Result = mysqli_fetch_array($QUERY_PASSOSCRIARNOVOCASO);
            $StepIdNovoCaso = $Result["StepId"];
            $t_body->NOVOCASOSTEPID = $StepIdNovoCaso;
            $t_body->block("BLOCK_STEPIDNOVOCASO");
            $t_body->block("BLOCO_INCLUDE_JAVASCRIPT");
            break;

        default:
            $Menu = 50;
            if (count($S_Processos) == 1 && !$userdef->Admin && !$ForcarSelecaoProcesso) {
                $Menu = 51;
            }
            $contadorStepIdNovoCaso = 0;
            while ($Result = mysqli_fetch_array($QUERY_PASSOSCRIARNOVOCASO)) {
                $nomePasso = $Result["StepName"];
                if (strlen($nomePasso) > 18) {
                    $nomePasso = substr($nomePasso, 0, 18) . "...";
                }
                $texto_cortado = $nomePasso;
                $StepIdNovoCaso = $Result["StepId"];

                $corIcone = "";
                $PageEdit = (!empty($Result["PageAction"])) ? $Result["PageAction"] : "editcase";
                $acao = "javascript:jsCriaNovoCaso('$PageEdit', $contadorStepIdNovoCaso)";
                $opcaoMenu = array("acao" => $acao, "icone" => "fa fa fa-star", "titulo" => $nomePasso, "texto" => $texto_cortado, "classe" => "text-primary");
                $opcoesMenu[0][] = $opcaoMenu;

                // Inclui na Lista de novos Casos
                $t_body->NOVOCASOSTEPID = $StepIdNovoCaso;
                $t_body->block("BLOCK_STEPIDNOVOCASO");

                $contadorStepIdNovoCaso++;
            }
            $t_body->block("BLOCO_INCLUDE_JAVASCRIPT");
            break;
    }

    $passosDoUsuario = PassosDoUsuario($ProcId, $userdef->Origem);
    foreach ($passosDoUsuario as $passo) {
        $nomePasso = $passo["StepName"];
        if (strlen($nomePasso) > 18) {
            $nomePasso = substr($nomePasso, 0, 18) . "...";
        }
        $texto_cortado = $nomePasso;
        $StepId_Filtro = $passo["StepId"];

        $corIcone = "";
        $acao = "queue/$ProcId/$StepId_Filtro";
        if ($StepId_Filtro == $StepId) {
            $acao = "javascript:";
            $corIcone = "text-primary";
        }
        $opcaoMenu = array("acao" => $acao, "icone" => "fa fa fa-eyedropper $corIcone", "titulo" => $passo["StepName"], "texto" => $texto_cortado, "classe" => "text-primary");
        $opcoesMenu[2][] = $opcaoMenu;
    }
    $opcaoMenu = array("acao" => "javascript:", "icone" => "", "texto" => "", "classe" => "text-grey");
    $opcoesMenu[2][] = $opcaoMenu;

    $opcaoMenu = array("acao" => "javascript:jsShowFilters($ProcId)", "icone" => "fa fa-filter", "texto" => "Avançado", "classe" => "");
    $opcoesMenu[2][] = $opcaoMenu;

    $opcaoMenu = array("acao" => "queue/$ProcId/todos", "icone" => "fa fa-ban", "texto" => "Sem Filtro", "classe" => "");
    $opcoesMenu[2][] = $opcaoMenu;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Popula toolbar / Side Bar">


populaToolBarTemplate($t_body, $Menu);
//$t_body->block("BLOCK_SIDEBAR");
// </editor-fold>
/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>


