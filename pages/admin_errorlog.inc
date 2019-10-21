<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 21/08/2018
  Sistema: Creditus
 */

function tratadadosErrorLog($dados)
{
    $pattern = '/^.*\b(Deprecated)\b.*$\n/m';
    $resultado = preg_replace($pattern, "", $dados);
    $pattern = '/^.*\b(Fatal)\b.*$/m';
    $pattern = '/^.*\b(thrown|Fatal|Stack)\b.*$\n/m';
    $resultado = preg_replace($pattern, '<span class="log-error">$0</span>', $resultado);
    $pattern = '/^.*\b(PHP Warning)\b.*$/m';
    $resultado = preg_replace($pattern, '<span class="log-warning">$0</span>', $resultado);
    return $resultado;
}

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/resource01.php");
include_once(FILES_ROOT . "include/toolbar.inc");

use raelgc\view\Template;

/**
 *  Apaga (renomeia) arquivo de erros do PHP
 */
function apagarErrorLog()
{
    $i = 1;
    while (file_exists(FILES_ROOT . ARQUIVO_ERROR_LOG_PHP . ".$i.log")) {
        $i++;
    }
    rename(FILES_ROOT . ARQUIVO_ERROR_LOG_PHP, FILES_ROOT . ARQUIVO_ERROR_LOG_PHP . ".$i.log");
    unlink(FILES_ROOT . "error_log");
}

function showlog()
{

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
    $t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/t_admin_errorlog.html");
    $t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");
    $t_body->SITE_LOCATION = SITE_LOCATION;

    $t_body->STYLE_LOGO = "style=\"display:none !important\"";

// Adiciona o css especifico da página

    $nomeArquivoErro = FILES_ROOT . ARQUIVO_ERROR_LOG_PHP;

    $t_body->ARQUIVO_JS = "errorlog.js"; // Incicializacao DataTables
    $t_body->block("BLOCO_JAVASCRIPT");


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
//    $Menu = 97;
//    populaToolBarTemplate($t_body, $Menu);
//    $t_body->block("BLOCK_TOOLBAR");
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

    $t_body->HEADER_TEXT = "Log do Sistema";
    $t_body->HEADER_SMALL_TEXT = "";
    $t_body->HEADER_ICO = "fa fa-file-text-o";
    $t_body->HEADER_CLASS = "text-primary";
    $t_body->HEADER_COLOR = "";
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Popula Navegacao">
// Se estiver apenas visualizando, não mostra a navegação
    $t_body->NAV_HEADER_TEXT = $nomeArquivoErro;
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
    $t_body->TITULO_PAGE = "Log de Erros | Process";
// </editor-fold>


    if (file_exists($nomeArquivoErro)) {

        $conteudoArquivoErro = file_get_contents($nomeArquivoErro);
        $conteudoArquivoErro = tratadadosErrorLog($conteudoArquivoErro);
        if ($conteudoArquivoErro == "") {
            $conteudoArquivoErro = "Encontrado $nomeArquivoErro <br> Mas sem informações uteis.";
        }
        $t_body->CONTET_DATA = nl2br(htmlentities($conteudoArquivoErro));
        $t_body->ALERT_ARQUIVO_STYLE = "display:none";
    } else {
        $t_body->ARQUIVO_LOG = "$nomeArquivoErro não existe";
    }

// <editor-fold defaultstate="collapsed" desc="javascript">

    $t_body->INCLUDE_JAVASCRIPT = "var janelaUnica = true;";
    $t_body->block("BLOCO_INCLUDE_JAVASCRIPT");
// Inclusão Javascript da Página
// </editor-fold>
    $t_body->block("BLOCK_CONTENT");
    $t_body->show();
}

//var_export($_SERVER);