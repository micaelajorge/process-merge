<?php

// <editor-fold defaultstate="collapsed" desc="Chamadas externas">
require_once('include/common.php');
require_once('include/ldap.php');
require_once('include/resource01.php');
include("include/validasessao.php");
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Chamada Classe de Template">

include_once(FILES_ROOT . "include/toolbar.inc");

use raelgc\view\Template;

//$t_body = new Template("templates/template_entrada.html");
//$t_body->addFile("TOOLBAR", "templates/template_toolbar.html");
//$t_body->addFile("PROCESSOS", "templates/template_processos.html");

$t_body = new Template(FILES_ROOT . "assets/templates/master/t_master.html");
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/t_entrada.html");
$t_body->addFile("PAGEHEADER", FILES_ROOT . "assets/templates/master/t_header.html");
$t_body->addFile("PROCESSOS", FILES_ROOT . "assets/templates/t_entradamobile_processos.html");
$t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");
$t_body->addFile("PAGEMENULEFT", FILES_ROOT . "assets/templates/master/t_menu.html");
$t_body->addFile("TOOLBAR_LEFT", FILES_ROOT . "assets/templates/master/t_toolbar_left.html");
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

if (strrpos($PaginaRetorno, "/") <> (strlen($PaginaRetorno) - 1))
    $PaginaRetorno .= "/";

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
//$t_body->SIDE_BAR_SIZE = " hide";

$t_body->TITULO_PAGE = "Entrada | Process";

// <editor-fold defaultstate="collapsed" desc="Popula os Processos">

$S_Processos = $_SESSION["S_Processos"];

$CorPadrao = "b9def0";

foreach ($S_Processos as $Processo) {
    switch ($Processo["TipoProc"])
    {
        case "AD":
            continue;
            break;        
        default:
            $paginaDestinoStep = $Processo["PageAction"];
            if ($paginaDestinoStep == "")
            {
                $paginaDestinoStep = "queue/%PROCID%";
            }        
    }
    
    if ($Processo["TipoProc"] == "CT") {
        $Destino = "flow0018.php?ProcId=" . $Processo["ProcId"];
    } else {
        /*
          $Destino = $Processo["PageAction"];
          $Destino = str_replace("%ASP_SESSION%", $userdef->ASP_session, $Destino);
          $Destino = str_replace("%PROCID%", $Processo["ProcId"], $Destino);
         * 
         */
        if (strpos(strtolower($paginaDestinoStep), "bpmqueue.php") == 0) {
            $paginaDestinoStep = replace($paginaDestinoStep, "BPMQueue.php?ProcId=", "queue/");
        }
        $paginaDestinoASP = str_replace("%ASP_SESSION%", $userdef->ASP_session, $paginaDestinoStep);
        $paginaDestino = str_replace("%PROCID%", $Processo["ProcId"], $paginaDestinoASP);
    }

    $Processo["ProcName"] = htmlspecialchars($Processo["ProcName"]);
    $Processo["ProcDesc"] = htmlspecialchars($Processo["ProcDesc"]);
    //$Processo["PageAction"] = "onclick=\"" . $Destino . "\"";
    $Processo["PageAction"] = $paginaDestino;

    // <editor-fold defaultstate="collapsed" desc="Define o Icone do Processo">
    $t_body->PROCICON = "fa fa-tasks";
    if ($Processo["ProcIcon"] != "") {
        $t_body->PROCICON = $Processo["ProcIcon"];
    }
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

    $t_body->block("BLOCK_PROCESSO_LARGE");
}
$t_body->block("BLOCK_GRUPO_PROCESSOS");
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Popula toolbar / Side Bar">
populaToolBarTemplate($t_body, "entradamobile");
//$t_body->block("BLOCK_SIDEBAR");
// </editor-fold>

$t_body->ARQUIVO_JS = "mobile.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");

$t_body->block("BLOCK_CONTENT");

// <editor-fold defaultstate="collapsed" desc="Envio do Template preeencido">
echo $t_body->parse();
// </editor-fold>
