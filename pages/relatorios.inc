<?php

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/resource01.php");
include_once(FILES_ROOT . "include/toolbar.inc");

use raelgc\view\Template;

// <editor-fold defaultstate="collapsed" desc="Preparando Variaveis de Ambiente">
if (!empty($Direct)) {
    $Fechar = 1;
}

$userdef = $_SESSION["userdef"];
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
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/t_relatorios_cards.html");
//$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/t_relatorios.html");
$t_body->addFile("RELATORIO_ITEM", FILES_ROOT . "assets/templates/t_relatorios_item.html");
$t_body->SITE_LOCATION = SITE_LOCATION;

$t_body->STYLE_LOGO = "style=\"display:none !important\"";

// Adiciona o css especifico da página

$t_body->CLASSE_WRAP_EDITAR = "col-lg-12";
$t_body->CLASS_COL_MASTER_EDITAR = "col-lg-12";
$t_body->ESCODE_IMAGEM_TIPIFICACAO = "hide";

$t_body->MOSTRA_TOOLBAR_FORMALIZACAO = "hide";
$t_body->CLASSE_DADOS_CASO = "col-lg-12";
$espacoEntreLinhas = "min-height: 100px";

$t_body->addFile("TOOLBAR_TOP", FILES_ROOT . "assets/templates/master/t_toolbar_top.html");

//$t_body->SITE_LOCATION = SITE_LOCATION;
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Popula toolbar">

$Menu = 97;
populaToolBarTemplate($t_body, $Menu);
$t_body->block("BLOCK_TOOLBAR");

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Popula dados Principais do Template">
$t_body->PROCICON = "fa-file-text-o";
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

$t_body->HEADER_TEXT = "Relatórios";
$t_body->HEADER_SMALL_TEXT = "";
$t_body->HEADER_ICO = "fa fa-file-text-o";
$t_body->HEADER_CLASS = "text-primary";
$t_body->HEADER_COLOR = "";
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Popula Navegacao">
// Se estiver apenas visualizando, não mostra a navegação
$t_body->NAV_HEADER_TEXT = "Relatórios";
$t_body->NAV_HEADER_ICO = "fa fa-file-text-o";
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
$t_body->TITULO_PAGE = "Relatórios | Process";
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Processos">


$processosUsuario = $_SESSION["S_Processos"];

// Gera a Lista de Processos do Usuário
$listaProcessos = implode(",", array_column($processosUsuario, "ProcId"));

if (APLICAR_PERMISSAO_RELATORIOS) {
    $SQL = " select "
            . "procdef.procid, "
            . "procname, "
            . "stepdef.pageaction, "
            . "proccod, "
            . "stepname, "
            . "stepcod, "
            . "OrderDesc "
            . "from "
            . "stepaddresses, "
            . "procdef, "
            . "stepdef "
            . "where "
            . "procdef.ProcId in ($listaProcessos) and procdef.procid = stepdef.procid and (stepdef.stepcod like 'relatorio%' or stepdef.steptype = 'R') and stepaddresses.ProcId = procdef.procid and stepaddresses.stepId = stepdef.StepId and "
            . "(( stepaddresses.GroupId in ($userdef->usergroups) and  stepaddresses.GRPFLD = 'G') or ( stepaddresses.GroupId in ($userdef->UserId_Process) and  stepaddresses.GRPFLD = 'U'))"
            . " order by ProcName ";
} else {
    $SQL = "select procdef.procid, procname, stepdef.pageaction, proccod, stepname, stepcod, OrderDesc from procdef, stepdef where procdef.ProcId in ($listaProcessos) and procdef.procid = stepdef.procid and (stepdef.stepcod like 'relatorio%' or stepdef.steptype = 'R') order by ProcName";
}
$Query = mysqli_query($connect, $SQL);
$aCampos = mysqli_fetch_all($Query, MYSQLI_ASSOC);

$relatorioColuna = 1;
foreach ($aCampos as $campo) {
    $t_body->ITEMCLASS = "text-primary";
    $t_body->ITEMDISABLED = "";
    $t_body->ITEMICON = "";
    $t_body->PROCNAME = $campo["procname"];
    $t_body->STEPNAME = $campo["stepname"];
    $t_body->PROCCOD = $campo["proccod"];
    $t_body->STEPCOD = $campo["stepcod"];
    $t_body->FIELDSELECTION = $campo["OrderDesc"];
    if ($campo["pageaction"] != "") {
        $t_body->PAGINA_RELATORIO = $campo["pageaction"];
    } else {
        $t_body->PAGINA_RELATORIO = "relatorio_geral";
    }

    $t_body->DATA_INICIO = date("01/m/Y");
    $t_body->DATA_FIM = date("d/m/Y");

    /*
      $passosDoUsuario = PassosDoUsuario($campo["procid"], $userdef->Origem);
      foreach ($passosDoUsuario as $passo) {
      $t_body->STEPID = $passo["StepId"];
      $t_body->STEPNAME = $passo["StepName"];
      $t_body->block("BLOCK_PASSOS");
      }
     */
    $t_body->block("BLOCO_RELATORIO_CARD_$relatorioColuna");
    $relatorioColuna++;
    if ($relatorioColuna > 3)
    {
        $relatorioColuna = 1;
    }
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="javascript">
$t_body->ARQUIVO_JS = "validacao.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "validadata.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "relatorios.js";
$t_body->block("BLOCO_JAVASCRIPT");

$t_body->ARQUIVO_JS = "jspdf.min.js";
$t_body->block("BLOCO_JAVASCRIPT_VENDOR");
$t_body->ARQUIVO_JS = "jspdf.plugin.autotable.js";
$t_body->block("BLOCO_JAVASCRIPT_VENDOR");
// </editor-fold>
$t_body->block("BLOCK_CONTENT");
$t_body->show();
