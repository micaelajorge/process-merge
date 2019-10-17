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
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/t_adm_entrada.html");
$t_body->addFile("PAGEHEADER", FILES_ROOT . "assets/templates/master/t_header.html");
$t_body->addFile("PROCESSOS", FILES_ROOT . "assets/templates/t_entrada_processos.html");
$t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");
$t_body->addFile("PAGEMENULEFT", FILES_ROOT . "assets/templates/master/t_menu.html");
$t_body->addFile("TOOLBAR_TOP", FILES_ROOT . "assets/templates/master/t_toolbar_top.html");
$t_body->addFile("CAPTURA_DOCUMENTOS", FILES_ROOT . "assets/templates/t_webcaptura.html");


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

$t_body->TITULO_PAGE = "Entrada | Process";

// <editor-fold defaultstate="collapsed" desc="Popula os Processos">

$CorPadrao = "b9def0";


$_ProcessosEspeciais = array();

if ($userdef->Admin) {

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

// <editor-fold defaultstate="collapsed" desc="Dados do Automato">
    $s_statusAutomato = file_get_contents(FILES_ROOT . "atualizacao_automato.json");
    $statusAutomato = json_decode($s_statusAutomato, true);

    $ultimaAtualizacao = $statusAutomato["ultimaAtualizacao"];
    $diferenca = DateDiff(date("Y-m-d H:i:s"), $ultimaAtualizacao, "m");

    if (key_exists("mensagem", $statusAutomato)) {
        $mensagemAutomato = "<br>" . implode("<br>", $statusAutomato["mensagem"]);
    }

    $ProcessoEspecial["ProcColor"] = "#00A65A";
    $ProcessoEspecial["CLASSE_BOX"] = "box-success";
    $iconeAutomato = "fa gears";
    if ($diferenca < -20) {
        $ProcessoEspecial["ProcColor"] = "#f39c12";
        $ProcessoEspecial["CLASSE_BOX"] = "box-danger";
        $mensagemAutomato = "<br><span class=\"text-danger\"><strong>Automato não Atualizado</strong></span>" . $mensagemAutomato;
        $iconeAutomato = "text-danger fa-warning";
    }

    $ProcessoEspecial["ProcIcon"] = $iconeAutomato;
    $ProcessoEspecial["TotalCasos"] = 0;
    $ProcessoEspecial["ProcName"] = "Automato " . ConvDate($ultimaAtualizacao) . $mensagemAutomato;
    $ProcessoEspecial["HideQueue"] = true;
    array_push($_ProcessosEspeciais, $ProcessoEspecial);
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Dados do Storage">



    $ProcessoEspecial["ProcIcon"] = "fa-database";
    $ProcessoEspecial["TotalCasos"] = 0;
    $ProcessoEspecial["ProcName"] = "Armazenamento Disponivel: " . HumanSize($freeSpace) . " (" . $porcentagemDisponivel . "%)";
    $ProcessoEspecial["HideQueue"] = true;
    array_push($_ProcessosEspeciais, $ProcessoEspecial);


    $ProcessoEspecial["ProcIcon"] = "fa-database";
    $ProcessoEspecial["TotalCasos"] = 0;
    $ProcessoEspecial["ProcName"] = "Armazenamento Ocupado: " . HumanSize($totalOcupado) . " (" . $porcentagemOcupado . "%)";
    $ProcessoEspecial["HideQueue"] = true;
    array_push($_ProcessosEspeciais, $ProcessoEspecial);
// </editor-fold>
}

$_ProcessosLista = array_merge($S_Processos, $_ProcessosEspeciais);

$totalProcessos = 0;

$userdef = $_SESSION["userdef"];

// <editor-fold defaultstate="collapsed" desc="Selecao dos Processos">
$GruposAD = $userdef->usergroups;
$SQL = " (select distinct 'ProcAdmin' as TipoAdmin, procdef.ProcId, procdef.ProcName, procdef.ProcDesc, Action, ProcColor, ProcIcon from procdef, procadmins where procadmins.GroupId = $userdef->UserId_Process and procdef.ProcId = procadmins.ProcId and procadmins.GrpFld='U')";
$SQL .= " union ";
$SQL .= " (select distinct 'ProcAdmin' as TipoAdmin, procdef.ProcId, procdef.ProcName, procdef.ProcDesc, Action, ProcColor, ProcIcon from procdef, procadmins where procadmins.GroupId in ($userdef->usergroups) and procdef.ProcId = procadmins.ProcId and procadmins.GrpFld = 'G')";
$SQL .= " union ";
$SQL .= " (select distinct 'GeralAdmin' as TipoAdmin, procdef.ProcId, procdef.ProcName, procdef.ProcDesc, Action, ProcColor, ProcIcon from procdef, geraladmins where geraladmins.GroupId = $userdef->UserId_Process and procdef.ProcId = geraladmins.ProcId and geraladmins.GrpFld='U')";
$SQL .= " union ";
$SQL .= " (select distinct 'GeralAdmin' as TipoAdmin, procdef.ProcId, procdef.ProcName, procdef.ProcDesc, Action, ProcColor, ProcIcon from procdef, geraladmins where geraladmins.GroupId in ($userdef->usergroups) and procdef.ProcId = geraladmins.ProcId and geraladmins.GrpFld = 'G') order by TipoAdmin, ProcName, Action";
$Query = mysqli_query($connect, $SQL);

$NumProcs = mysqli_num_rows($Query);

$_Processos = mysqli_fetch_all($Query, MYSQLI_ASSOC);

$TipoAdmin_old = "";
foreach ($_Processos as $Processo) {    
    if ($TipoAdmin_old != "" & $tipoProc_old != $Processo["TipoAdmin"])
    {        
        $t_body->ADMIN_TEXTO_HEADER = $TipoAdmin_old;
        $t_body->block("BLOCK_GRUPO_PROCESSOS");
    }
    $TipoAdmin_old = $Processo["TipoAdmin"];        
    $paginaDestinoStep = $Processo["PageAction"];
    if (strpos(strtolower($paginaDestinoStep), "queue") === false) {
        $t_body->ICON_QUEUE = "fa-arrow-circle-o-up";
        $t_body->TEXT_QUEUE = "Abrir";
    } else {
        $paginaDestinoStep = replace($paginaDestinoStep, "BPMQueue.php?ProcId=", "queue/");
        $t_body->TEXT_QUEUE = "Fila";
        $t_body->ICON_QUEUE = "fa-tasks";
    }
    $paginaDestinoASP = str_replace("%ASP_SESSION%", $userdef->ASP_session, $paginaDestinoStep);
    $paginaDestino = str_replace("%PROCID%", $Processo["ProcId"], $paginaDestinoASP);


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
    $t_body->CLASSE_BOX = "box-primary box-dialog";
    if ($Processo["TotalCasos"] > 0) {
        $totalProcessos++;
        $t_body->block("BLOCK_PROCESSO_LARGE");
    } else {
        $t_body->HIDEQUEUE = "";
        if (key_exists("HideQueue", $Processo)) {
            $t_body->HIDEQUEUE = "visibility: hidden";
        }
        $t_body->block("BLOCK_PROCESSO_SMALL");
    }
    unset($Processo);
}
$t_body->ADMIN_TEXTO_HEADER = $TipoAdmin_old;
$t_body->block("BLOCK_GRUPO_PROCESSOS");
// </editor-fold>

$Menu = 97;
populaToolBarTemplate($t_body, $Menu);

// Incicializacao web capture
$t_body->ARQUIVO_JS = "webcaptura.js";
$t_body->block("BLOCO_JAVASCRIPT");

// Incicializacao web capture
$t_body->ARQUIVO_JS = "editcase.js";
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
echo $t_body->parse();
// </editor-fold> 