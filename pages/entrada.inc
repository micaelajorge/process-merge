<?php
// <editor-fold defaultstate="collapsed" desc="Chamadas externas">
require_once(FILES_ROOT . '/include/common.php');
require_once(FILES_ROOT . '/include/ldap.php');
require_once(FILES_ROOT . '/include/resource01.php');
require_once(FILES_ROOT . '/include/toolbar.inc');

//include("include/validasessao.php");
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Chamada Classe de Template">

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/master/t_master.html");
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/t_entrada.html");
$t_body->addFile("PAGEHEADER", FILES_ROOT . "assets/templates/master/t_header.html");
$t_body->addFile("PROCESSOS", FILES_ROOT . "assets/templates/t_entrada_processos.html");
$t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");
$t_body->addFile("PAGEMENULEFT", FILES_ROOT . "assets/templates/master/t_menu.html");
$t_body->addFile("TOOLBAR_TOP", FILES_ROOT . "assets/templates/master/t_toolbar_top.html");
$t_body->addFile("CAPTURA_DOCUMENTOS", FILES_ROOT . "assets/templates/t_webcaptura.html");
$t_body->addFile("TOOLBAR_DROPDOWN", FILES_ROOT . "assets/templates/t_processos_dropdown.html");

$t_body->ARQUIVO_CSS = "entrada.css";
$t_body->block("BLOCO_CSS");

$t_body->SITE_LOCATION = SITE_LOCATION;

$t_body->TITULO = "Seleção de Processos";
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Definicao das Variaveis de ambiente">
if (!array($UserPath)) {
    $UserPath = array();
    $Path["Descricao"] = "Seleção de Processo";
    $Path["file"] = "entrada";
    array_push($UserPath, $Path);
}

if (isset($original_url)) {
    $protocolo = substr($original_url, 0, strpos($original_url, '/')) . '//';
    $PaginaRetorno = $protocolo . $_SERVER['SERVER_NAME'] . ":" . $_SERVER['SERVER_PORT'];
}

if (dirname($_SERVER['SCRIPT_NAME']) <> "\\") {
    $PaginaRetorno .= dirname($_SERVER['SCRIPT_NAME']);
}

if (strrpos($PaginaRetorno, "/") <> (strlen($PaginaRetorno) - 1)) {
    $PaginaRetorno .= "/";
}

$PaginaRetorno .= "entrada";
$PaginaOrigem = "entrada";
$PaginaAnterior = "entrada";
$_SESSION["PaginaAnterior"] = $PaginaAnterior;

// </editor-fold>

$t_body->WRAPPER_STYLE = "style=\"width: 100%; margin-left: 0px !important\"";


// Definições do Usuário
$t_body->USER_DESC = $userdef->UserDesc;
$t_body->USER_NAME = $userdef->UserName;
$t_body->USER_ORIGEM = $userdef->Origem;
$t_body->USER_EMAIL = $userdef->Email;

$t_body->SIDEBAR_HIDE = " hide";
$t_body->MAIN_FOOTER_STYLE = "margin:0px";

if (!defined("INSTANCENAME")) {
    define("INSTANCENAME", "Instancia sem Nome");
}

$t_body->TITULO_PAGE = "Soluções | " . INSTANCENAME;

$t_body->STYLE_LOGO = "style=\"display:none !important\"";

// <editor-fold defaultstate="collapsed" desc="Popula os Processos">

$S_Processos = $_SESSION["S_Processos"];

$CorPadrao = "b9def0";


// <editor-fold defaultstate="collapsed" desc="Processos Especiais">
$_ProcessosEspeciais = array();
if ($userdef->Admin | $userdef->UserId == 1) {
    $ProcessoEspecial["ProcEspecial"] = true;
// <editor-fold defaultstate="collapsed" desc="Armazenamento">
    $freeSpace = disk_free_space(FILES_UPLOAD);
    $totalSpace = disk_total_space(FILES_UPLOAD);
    $totalOcupado = $totalSpace - $freeSpace;

    $porcentagemDisponivel = round(($freeSpace / $totalSpace) * 100, 2);
    $porcentagemOcupado = round(($totalOcupado / $totalSpace) * 100, 2);


    if ($porcentagemDisponivel < 20) {
        if ($porcentagemDisponivel < 10) {
            $ProcessoEspecial["ProcColor"] = "#DD4B39";
            $ProcessoEspecial["CLASSE_BOX"] = "box-danger";
        }
        $ProcessoEspecial["ProcColor"] = "#f39c12";
        $ProcessoEspecial["CLASSE_BOX"] = "box-warning";
    } else {
        $ProcessoEspecial["ProcColor"] = "#00A65A";
        $ProcessoEspecial["CLASSE_BOX"] = "box-success";
    }
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Dados do Automato">
    $s_statusAutomato = file_get_contents(FILES_ROOT . "atualizacao_automato.json");
    $statusAutomato = json_decode($s_statusAutomato, true);

    $o_statusAutomato = file_get_contents(FILES_ROOT . "status_automato.json");
    $operacaoAutomato = json_decode($o_statusAutomato, true);

    $ultimaAtualizacao = $statusAutomato["ultimaAtualizacao"];
    $diferenca = DateDiff(date("Y-m-d H:i:s"), $ultimaAtualizacao, "m");

    if (key_exists("mensagem", $statusAutomato)) {
        $mensagemAutomato = "<br>" . implode("<br>", $statusAutomato["mensagem"]);
    }

    $ProcessoEspecial["ProcColor"] = "#00A65A";
    $ProcessoEspecial["CLASSE_BOX"] = "box-success";
    $ProcessoEspecial["ProcHideOptions"] = "hidden";

    if ($operacaoAutomato["statusAutomato"] != $statusAutomato["automatoPausado"]) {
        $iconeAutomato = "fa-hourglass-half blink";
        $iconeTitulo = "Aguardando mudança status Automato";
        $procAction = "javascript:";
        $mostrarGrande = 1;
    } else {
        if ($operacaoAutomato["statusAutomato"] == false) {
            $procAction = "javascript:jsMudaStatusAutomato(true)";
            $iconeTitulo = "Pausar Operação Automato";
            $iconeAutomato = "fa-pause";
            $mostrarGrande = 0;
        } else {
            $procAction = "javascript:jsMudaStatusAutomato(false)";
            $iconeTitulo = "Iniciar Operação Automato";
            $iconeAutomato = "fa-play  blink";
            $mostrarGrande = 1;
        }
    }

    if ($diferenca < -20) {
        $ProcessoEspecial["ProcColor"] = "#f39c12";
        $ProcessoEspecial["CLASSE_BOX"] = "box-danger";
        $mensagemAutomato = "<br><span class=\"text-danger\"><strong>Automato não Atualizado</strong></span>" . $mensagemAutomato;
        $iconeAutomato = " fa-exclamation blink";

        $iconeTitulo = "Automato não está sendo executado";
        $procAction = "javascript:";
        //$iconeAutomato = "text-danger fa-warning";
    }
    $mostrarGrande = 1;
    $ProcessoEspecial["ProcIconTitle"] = $iconeTitulo;
    $ProcessoEspecial["ProcIcon"] = $iconeAutomato;
    $ProcessoEspecial["TotalCasos"] = $mostrarGrande;
    $ProcessoEspecial["ProcName"] = "Automato " . ConvDate($ultimaAtualizacao) . $mensagemAutomato;
    $ProcessoEspecial["HideQueue"] = true;
    $ProcessoEspecial["PageAction"] = $procAction;
    array_push($_ProcessosEspeciais, $ProcessoEspecial);
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Dados do Storage">


    $ProcessoEspecial["Especial"] = "true";
    $ProcessoEspecial["ProcIcon"] = "fa-database";
    $ProcessoEspecial["TotalCasos"] = 0;
    $ProcessoEspecial["ProcName"] = "Disponivel: " . HumanSize($freeSpace) . " (" . $porcentagemDisponivel . "%)";
    $ProcessoEspecial["HideQueue"] = true;
    array_push($_ProcessosEspeciais, $ProcessoEspecial);

    $ProcessoEspecial["Especial"] = "true";
    $ProcessoEspecial["ProcIcon"] = "fa-database";
    $ProcessoEspecial["TotalCasos"] = 0;
    $ProcessoEspecial["ProcName"] = "<span title=\"Diretorio Arquivos: ". FILES_UPLOAD. "\">Ocupado: " . HumanSize($totalOcupado) . " (" . $porcentagemOcupado . "%)</span>";
    $ProcessoEspecial["HideQueue"] = true;
    array_push($_ProcessosEspeciais, $ProcessoEspecial);
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Dados de Log">
    $ProcessoEspecial["Especial"] = "true";
    $ProcessoEspecial["ProcIcon"] = "fa-file-text-o";
    $ProcessoEspecial["TotalCasos"] = 1;
    $ProcessoEspecial["ProcName"] = "Log Sistema";
    $ProcessoEspecial["PageAction"] = "errorlog";
    $ProcessoEspecial["PageTarget"] = "paginalog";
    $ProcessoEspecial["ProcIconTitle"] = "Log do Sistema";
    array_push($_ProcessosEspeciais, $ProcessoEspecial);
// </editor-fold>
}
// </editor-fold>

$_ProcessosLista = array_merge($S_Processos, $_ProcessosEspeciais);

$processosEscondidos = 0;
$totalProcessos = 0;

// Determina se os processos sem casos ativos vão aparecer ou não na Tela de Entrada
$VisualizarProcessosVazios = "hidden";

if (count($S_Processos) < NR_PROCESSOS_SHOW_ENTRADA)
{
    $VisualizarProcessosVazios = "";
}
if (key_exists("mostrarTodosProcessos", $_COOKIE))
{
    if ($_COOKIE["mostrarTodosProcessos"] == "true")
    {
        $VisualizarProcessosVazios = "";
    }
} 

foreach ($_ProcessosLista as $Processo) {
    if ($Processo["HideEntrada"] == 1)
    {
        continue;
    }
    switch ($Processo["TipoProc"]) {
        case "AD":
            continue;
            break;
        default:
            $paginaDestinoStep = $Processo["PageAction"];
            if ($paginaDestinoStep == "")
            {
                $paginaDestinoStep = "queue/%PROCID%";
            }
            
            if (strpos(strtolower($paginaDestinoStep), "queue") === false) {
                $t_body->ICON_QUEUE = "fa-arrow-circle-o-up";
                $t_body->TEXT_QUEUE = "Abrir";
                $t_body->HIDE_PROCESS = "";
                $t_body->PROCESSO_ESCONDER = "";
                $t_body->TAMANHO_PROCESSO = "col-lg-4";
            } else {
                $paginaDestinoStep = replace($paginaDestinoStep, "BPMQueue.php?ProcId=", "queue/");
                $t_body->HIDE_PROCESS = "$VisualizarProcessosVazios";
                $t_body->PROCESSO_ESCONDER = "PROC_VAZIO";
                $t_body->TEXT_QUEUE = "Fila";
                $t_body->ICON_QUEUE = "fa-tasks";
                $t_body->TAMANHO_PROCESSO = "col-lg-4";
                $processosEscondidos++;
            }
            $paginaDestinoASP = str_replace("%ASP_SESSION%", $userdef->ASP_session, $paginaDestinoStep);
            $paginaDestino = str_replace("%PROCID%", $Processo["ProcId"], $paginaDestinoASP);
            break;
    }

    if ($Processo["TipoProc"] == "CT") {
        $temProcessoCT = true;
    }

    $Processo["PageAction"] = $paginaDestino;

    // <editor-fold defaultstate="collapsed" desc="Define o Icone do Processo">    
    $ProcIcon = "fa fa-tasks";
    if ($Processo["ProcIcon"] != "") {
        $ProcIcon = $Processo["ProcIcon"];
    } else {
        switch ($Processo["TipoProc"]) {
            case "WF":
                $ProcIcon = "fa fa-gears";
                break;
            case "CT":
                $ProcIcon = "fa fa-tags";
                break;
            case "RP":
                $ProcIcon = "fa fa-files-o";
                break;
        }
    }
    $t_body->PROCICON = $ProcIcon;

    // </editor-fold>
    // <editor-fold defaultstate="collapsed" desc="Define a Cor do Processo">
    if ($Processo["ProcColor"] == "") {
        $Processo["ProcColor"] = "#" . dechex(hexdec($CorPadrao) + $Processo["ProcId"]);
    }

    // $Processo->PROCICONBORDERCOLOR = "#" . dechex(hexdec(str_replace("#", "", $Processo["Color"])) - 100);  
    // </editor-fold>    

    $t_body->PROCCOLOR = $Processo["ProcColor"];
    $t_body->PROCNAME = $Processo["ProcName"];
    $t_body->PROCDESC = $Processo["ProcDesc"];
    $t_body->PROCACTION = $Processo["PageAction"];
    $t_body->PROCNUMCOUNT = $Processo["NumCount"];
    $t_body->TOTALCASOS = $Processo["TotalCasos"];
    $t_body->PROCICONTITLE = $Processo["ProcIconTitle"];
    $t_body->PROCHIDEOPTIONS = $Processo["ProcHideOptions"];
    $t_body->PAGE_TARGET = $Processo["PageTarget"];
    $t_body->PROCNAMELOWERCASE = strtolower($Processo["ProcName"]);

    if ($Processo["TotalCasos"] > 0) {
        $totalProcessos++;
        $t_body->block("BLOCK_PROCESSO_LARGE");
    } else {
        $t_body->HIDEQUEUE = "";
        if (key_exists("HideQueue", $Processo)) {
            $t_body->HIDEQUEUE = "visibility: hidden";
        }
        $t_body->CLASSE_BOX = "box-default";
        if (key_exists("CLASSE_BOX", $Processo)) {
            $t_body->CLASSE_BOX = $Processo["CLASSE_BOX"];
        }
        $t_body->block("BLOCK_PROCESSO_SMALL");
    }
    if (!$Processo["ProcEspecial"]) {
        $t_body->block("BLOCO_SHORTCUT_PROCESSOS");
    }
    unset($Processo);
}
$t_body->block("BLOCK_SEL_PROCESSOS");
$t_body->block("BLOCK_GRUPO_PROCESSOS");
// </editor-fold>

$Menu = 96;
if (ALLOW_SEARCH | $temProcessoCT) {
    $Menu = 98;
}

$_SESSION["entrada"] = "entrada";

// Incicializacao web capture
$t_body->ARQUIVO_JS = "entrada.js";
$t_body->block("BLOCO_JAVASCRIPT");


// Incicializacao web capture
$t_body->ARQUIVO_JS = "webcaptura.js";
$t_body->block("BLOCO_JAVASCRIPT");

// Incicializacao web capture
$t_body->ARQUIVO_JS = "editcase.js";
$t_body->block("BLOCO_JAVASCRIPT");

// Operacao Automato
$t_body->ARQUIVO_JS = "automato.js";
$t_body->block("BLOCO_JAVASCRIPT");



//// Incicializacao Webcapture
//$t_body->ARQUIVO_JS = "dynamic/dynamsoft.webtwain.initiate.js"; // Incicializacao DataTables
//$t_body->block("BLOCO_JAVASCRIPT_VENDOR");
//
//// Incicializacao Webcapture
//$t_body->ARQUIVO_JS = "dynamic/dynamsoft.webtwain.config.js"; // Incicializacao DataTables
//$t_body->block("BLOCO_JAVASCRIPT_VENDOR");

$t_body->block("BLOCK_CONTENT");

// <editor-fold defaultstate="collapsed" desc="Envio do Template preeencido">

// !!! Importante !!!! Prepara a toolbar aqui para incluir os atalhos para os Processos do Usuário
populaToolBarTemplate($t_body, $Menu);
echo $t_body->parse();
// </editor-fold> 