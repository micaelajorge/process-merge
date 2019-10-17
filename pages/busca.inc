<?php

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/resource01.php");
include_once(FILES_ROOT . "include/toolbar.inc");

use raelgc\view\Template;

// <editor-fold defaultstate="collapsed" desc="Preparando Variaveis de Ambiente">
if (!empty($Direct)) {
    $Fechar = 1;
}

/*
  if (session_is_registered("lidos")) {
  array_push($lidos, "$ProcId-$StepId-$CaseNum");
  }
 * 
 */

if (count($_POST) > 0) {

    if (key_exists("ProcId", $_POST)) {
        $EditCase_ProcId = $ProcId;
    } else {
        $ProcId = $EditCase_ProcId;
    }

    if (key_exists("StepId", $_POST)) {
        $EditCase_StepId = $StepId;
    } else {
        $StepId = $EditCase_StepId;
    }

    if (key_exists("CaseNum", $_POST)) {
        $EditCase_CaseNum = $CaseNum;
    } else {
        $CaseNum = $EditCase_CaseNum;
    }
}

if (count($_GET) > 0) {
    if (key_exists("ProcId", $_GET)) {
        $EditCase_ProcId = $_GET["ProcId"];
    } else {
        if (isset($EditCase_ProcId)) {
            $ProcId = $EditCase_ProcId;
        } else {
            $ProcId = PegaProcIdCaso($CaseNum);
        }
    }

    if (key_exists("StepId", $_GET)) {
        $EditCase_StepId = $_GET["StepId"];
    } else {
        $StepId = $EditCase_StepId;
    }

    if (key_exists("CaseNum", $_GET)) {
        $EditCase_CaseNum = $_GET["CaseNum"];
    } else {
        $CaseNum = $EditCase_CaseNum;
    }
}


if (!is_numeric($ProcId)) {
    $ProcId = PegaProcIdByCode($ProcId);
}

if (isset($_SESSION[$ProcId]["procdef"])) {
    $procdef = $_SESSION[$ProcId]["procdef"];
    if ($procdef->ProcId <> $ProcId) {
        $procdef = new procdef;
        $procdef->Create($ProcId, $connect);
    }
} else {
    $procdef = new procdef;
    $procdef->Create($ProcId, $connect);
    $_SESSION[$ProcId]["procdef"] = $procdef;
}

if ($StepId == "") {
    $StepId = $procdef->DefaultView;
}



$userdef = $_SESSION["userdef"];
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Pega os Botões Habilitados para o Passo no WebDesigner">
// 
// 
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Define ação que seré feita no caso">
// Se for visualizacao de Caso de Campo, esconde os itens
$mostraNavegacao = true;
$mostraBarraSuperior = true;

// </editor-fold>
//$CamposRum = PegaCamposProcRum($ProcId);	
//$CamposRumAtivo = PegaCamposProcRumAtivo($ProcId, $StepId);
// Habilita ou nao botao de hist�rico
$EnableHistorico = "enable=false";

// <editor-fold defaultstate="collapsed" desc="Prepara o Template">

$t_body = new Template(FILES_ROOT . "assets/templates/master/t_master.html");
$t_body->addFile("PAGEHEADER", FILES_ROOT . "assets/templates/master/t_header.html");
$t_body->addFile("PAGEMENULEFT", FILES_ROOT . "assets/templates/master/t_menu.html");
$t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/master/t_datatable.html");
$t_body->addFile("TOPO_PAGINA", FILES_ROOT . "assets/templates/t_busca_campos.html");
$t_body->SITE_LOCATION = SITE_LOCATION;

$t_body->addFile("TOOLBAR_TOP", FILES_ROOT . "assets/templates/master/t_toolbar_top.html");

//$t_body->SITE_LOCATION = SITE_LOCATION;
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Popula toolbar">

$Menu = 97;
populaToolBarTemplate($t_body, $Menu);
$t_body->block("BLOCK_TOOLBAR");

// </editor-fold>

$t_body->CLASS_BOX_DATATABLE_HIDE = "show";
$t_body->CLASS_DATATABLE_WRAPPER_HIDE = " ";

// <editor-fold defaultstate="collapsed" desc="Popula dados Principais do Template">
$t_body->PROCICON = $procdef->ProcIcon;
$t_body->PROCCOLOR = $procdef->ProcColor;
$t_body->PROCESSODESC = "Processo - " . $procdef->ProcDesc;
$t_body->PROCDESC = $procdef->ProcDesc;
$t_body->PROCID = $ProcId;
$t_body->STEPID = $StepId;
$t_body->PROCNAME = $procdef->ProcName;
$t_body->STEPNAME = $StepName;
$t_body->CASENUM = $CaseNum;
$t_body->ACAOCASO = $AcaoCaso;
$t_body->METODO_FORM = "method='post'";
$t_body->POSACTION = $AcaoAposProcessar;
$t_body->PRIORIDADE = $Prioridade;
$t_body->NOVOCASO = $NovoCaso;
$t_body->FECHADO = 0;

$t_body->HEADER_TEXT = "Busca";
$t_body->HEADER_SMALL_TEXT = "Busca de Objetos";
$t_body->HEADER_ICO = "fa fa-search";
$t_body->HEADER_CLASS = "text-primary";
$t_body->HEADER_COLOR = "$procdef->ProcColor";
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Popula Navegacao">
// Se estiver apenas visualizando, não mostra a navegação
$t_body->NAV_HEADER_TEXT = "Busca";
$t_body->NAV_HEADER_ICO = "fa fa-search";
$t_body->NAV_HEADER_COLOR = "";
$t_body->NAV_HEADER_CLASS = "text-primary";
$t_body->NAGEVAGAO_LINK = "javascript:";
$t_body->block("BLOCO_NAVEGACAO"); // Inclui na navegação do Site
$t_body->block("BLOCO_MOSTRAR_NAVEGACAO");
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="dados do Usuário no Template">
$t_body->USER_DESC = $userdef->UserDesc;
$t_body->USER_NAME = $userdef->UserName;
$t_body->USER_ORIGEM = $userdef->Origem;
$t_body->USER_EMAIL = $userdef->Email;
$t_body->USER_ID = $userdef->UserId;
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Formatação Especifica da Página">
$t_body->WRAPPER_STYLE = "style=\"width: 100%; margin-left: 0px !important\"";
$t_body->SIDEBAR_HIDE = "hide";

// Espaço entre linhas
$t_body->STYLE_ENTRE_LINHAS = $espacoEntreLinhas;
$t_body->TITULO_PAGE = "Busca | Process";


// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Processos e campos">


$processosUsuario = $_SESSION["S_Processos"];

foreach ($processosUsuario as $processo) {
    $t_body->PROCID = $processo["ProcId"];
    $t_body->PROCNAME = $processo["ProcName"];
    $t_body->block("BLOCO_SEL_PROCESSOS");
}

// Gera a Lista de Processos do Usuário
$listaProcessos = implode(",", array_column($processosUsuario, "ProcId"));

$listaTiposCampos = implode(",", array("'US'", "'GR'", "'TX'", "'DT'", "'TB'", "'NU'", "'BO'"));

$SQL = "select ProcName, ProcDef.ProcId, FieldId, FieldName, FieldType from procDef, procfieldsdef where ProcDef.ProcId in ($listaProcessos) and ProcDef.procid = ProcFieldsDef.procid and FieldType in ($listaTiposCampos) order by ProcName, FieldName";
$Query = mysqli_query($connect, $SQL);
$aCampos = mysqli_fetch_all($Query, MYSQLI_ASSOC);

$procName_old = "";
foreach ($aCampos as $campo) {
    if ($procName_old != $campo["ProcName"]) {
        $t_body->FIELDNAME = "Processo -" . $campo["ProcName"];
        $t_body->ITEMDISABLED = "disabled";
        $t_body->ITEMCLASS = "text-default";
        $t_body->ITEMICON = "fa fa-list";
        $t_body->block("BLOCO_CAMPOS_PESQUISA");        
        $procName_old = $campo["ProcName"];
    }
    $t_body->ITEMCLASS = "text-primary";
    $t_body->ITEMDISABLED = "";
    $t_body->ITEMICON = "";
    $t_body->FIELDNAME = $campo["FieldName"];
    $t_body->FIELDTYPE = $campo["FieldType"];
    $t_body->FIELDID = $campo["FieldId"];
    $t_body->PROCID = $campo["ProcId"];
    $t_body->PROCNAME = $campo["ProcName"];
    $t_body->block("BLOCO_CAMPOS_PESQUISA");
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Definições pesquisa avançada">
$t_body->SEGUNDOVALOR_DISABLED = "disabled";
$t_body->COMPARADOR_DISABLED = "disabled";
$t_body->PRIMEIROVALOR_DISABLED = "disabled";
$t_body->COMPARADOR_DISABLED = "disabled";
$t_body->ADICIONAR_DISABLED = "disabled";
$t_body->LOGICA_DISABLED = "disabled";
$t_body->BOLEANOVALOR_HIDDEN = "display:none";
// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="Cabecalho da Tabela ">
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
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="javascript">
$t_body->ARQUIVO_JS = "validacao.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "busca_datatable.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "validadata.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "showdata.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "busca.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "posicionar.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "seluser.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "browserSniffer.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "dynCalendar.js";
// </editor-fold>
$t_body->block("BLOCK_CONTENT");
$t_body->show();
