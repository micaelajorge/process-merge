<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 30/11/2019
  Sistema: Process
 */

// <editor-fold defaultstate="collapsed" desc="Chamadas externas">
require_once(FILES_ROOT . '/include/common.php');
require_once(FILES_ROOT . '/include/ldap.php');
require_once(FILES_ROOT . '/include/resource01.php');
require_once(FILES_ROOT . '/include/toolbar.inc');

// </editor-fold>

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/master/t_master.html");
$t_body->addFile("PAGEHEADER", FILES_ROOT . "assets/templates/master/t_header.html");
$t_body->addFile("PAGEMENULEFT", FILES_ROOT . "assets/templates/master/t_menu.html");
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/securities/t_securities_dashboard.html");
$t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");
$t_body->SITE_LOCATION = SITE_LOCATION;

$t_body->SITE_LOCATION = SITE_LOCATION;
$t_body->TITULO_PAGE = "Dashboard | " . INSTANCENAME;
$t_body->TEXT_HEADER = "Process";
$t_body->TEXT_HEADER_SMALL = "";

// Esconde a barra lateral de Menu
$t_body->SIDEBAR_HIDE = "hidden";
$t_body->WRAPPER_STYLE = "style=\"width: 100%; margin-left: 0px !important\"";

$t_body->MAIN_FOOTER_STYLE = "margin-left: 0px";

$t_body->block("BLOCK_CONTENT");

// Definições do Usuário
$t_body->USER_DESC = $userdef->UserDesc;
$t_body->USER_NAME = $userdef->UserName;
$t_body->USER_ORIGEM = $userdef->Origem;
$t_body->USER_EMAIL = $userdef->Email;

$t_body->NOTIFICACOES_HIDE = "display:none";
$t_body->TASKS_HIDE = "display:none";

// Inclusão Javascript da Página
$t_body->ARQUIVO_JS = "dataTables.init.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "securities/securities_dashboard_datatable_listcessions.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "securities/securities_dashboard.js";
$t_body->block("BLOCO_JAVASCRIPT");
// Inclusão CSS da Página
//$t_body->ARQUIVO_CSS = "";
//$t_body->block("BLOCO_CSS");
/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>

