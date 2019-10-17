<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 26/06/2019
  Sistema: Process_XAMPP
 */

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
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/t_entrada_custom.html");
$t_body->addFile("PAGEHEADER", FILES_ROOT . "assets/templates/master/t_header.html");
$t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");
$t_body->addFile("PAGEMENULEFT", FILES_ROOT . "assets/templates/master/t_menu.html");
$t_body->addFile("FORM_QUEUE", FILES_ROOT . "assets/templates/t_queue.html");

// Insere o ProcId para tratamento no Datatable
$t_body->SCRIPT_DATA = "var ProcId = 'FUNDOS';";
$t_body->block("BLOCO_JAVASCRIPT_INSERTS");

$t_body->PROCID = 'FUNDOS';

$_SESSION["entrada"] = "entradafundos";

$t_body->TITULO_PAGINA_1 = "Gestão de Fundos";

// Define o código de Passo para Criar novo Caso
$t_body->addFile("INCLUDE_JAVASCRIPT", FILES_ROOT . "assets/templates/t_queuescript.html");
$t_body->NOVOCASOSTEPID = "START";
$t_body->block("BLOCK_STEPIDNOVOCASO");
$t_body->block("BLOCO_INCLUDE_JAVASCRIPT");

$t_body->TITULO_PAGE = "Gestão de Fundos | Clicksign";

// Esconde a barra lateral
$t_body->SIDEBAR_HIDE = " hide";

$t_body->MAIN_FOOTER_STYLE = "margin:0px";

$t_body->WRAPPER_STYLE = "style=\"width: 100%; margin-left: 0px !important; \"";

$procIdFundos = PegaProcIdByCode("FUNDOS");
$CampoNome = PegaFieldIdByCode($procIdFundos, "NOME_DO_FUNDO");
$CampoCNPJ = PegaFieldIdByCode($procIdFundos, "CNPJ");
$SQL = "select casenum, campoDisplay$CampoNome as nomeFundo, campoDisplay$CampoCNPJ as cnpj from exportkeysdisplay where procid = $procIdFundos ";

$Query = mysqli_query($connect, $SQL);

$listaFundos = mysqli_fetch_all($Query, MYSQLI_ASSOC);

foreach ($listaFundos as $fundoAtual) {
    $t_body->FUNDO_NOME = $fundoAtual["nomeFundo"];
    $t_body->FUNDO_CNPJ = $fundoAtual["cnpj"];
    $t_body->block("BLOCO_FUNDOS");
}

// Esconde a opções de Icones
$t_body->NOTIFICACOES_HIDE = "display:none";
$t_body->TASKS_HIDE = "display:none";

$t_body->SHOW_ICON_TOOLBAR = " ";

// Mostra opções de Seleção de Origem
$t_body->SELELECIONARORIGEM = " ";

// Definições do Usuário
$t_body->USER_DESC = $userdef->UserDesc;
$t_body->USER_NAME = $userdef->UserName;
$t_body->USER_ORIGEM = $userdef->Origem;
$t_body->USER_EMAIL = $userdef->Email;

$t_body->block("BLOCK_CONTENT");


$_SESSION["PaginaQueueOriginal"] = "entradafundos";

$t_body->ARQUIVO_JS = "queue.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->show();
