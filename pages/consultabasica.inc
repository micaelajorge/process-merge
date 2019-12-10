<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 04/12/2019
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
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/t_consultabasica.html");
$t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");

$t_body->addFile("DATA_TABLE", FILES_ROOT . "assets/templates/master/t_datatable_basic.html");

$t_body->SITE_LOCATION = SITE_LOCATION;

$procId = PegaProcIdByCode($procCode);

$SQL = "select *, FieldRazaoOrder + FieldName as order_sort from procfieldsdef where ProcId = $procId and FieldKey = 1 and FieldSpecial = 0 order by order_sort";
$Query = mysqli_query($connect, $SQL);
$camposFiltro = mysqli_fetch_all($Query, MYSQLI_ASSOC);
$procdef = $_SESSION["procdef"];
$filtros = $procdef->filterQueue;

foreach ($camposFiltro as $campo) {
    $t_body->TEXTO_CABECALHO = $campo["FieldName"];
    $t_body->block("BLOCK_CABECALHO_TABELA");
}

$t_body->block("BLOCK_CONTENT");

// Esconder Header
//$t_body->CONTENT_HEADER_HIDEN = "hidden";

$t_body->SITE_LOCATION = SITE_LOCATION;
$t_body->TITULO_PAGE = "Process | " . INSTANCENAME;
$t_body->HEADER_ICO = "fa fa-server";
$t_body->HEADER_TEXT = "Consulta";
$t_body->HEADER_SMALL_TEXT = "Consulta Basica";

// Esconde a barra lateral de Menu
$t_body->SIDEBAR_HIDE = "hidden";
$t_body->WRAPPER_STYLE = "style=\"width: 100%; margin-left: 0px !important\"";

$t_body->MAIN_FOOTER_STYLE = "margin-left: 0px";

// Definições do Usuário
$t_body->USER_DESC = $userdef->UserDesc;
$t_body->USER_NAME = $userdef->UserName;
$t_body->USER_ORIGEM = $userdef->Origem;
$t_body->USER_EMAIL = $userdef->Email;

// Inclusão Javascript da Página
//$t_body->ARQUIVO_JS = "dataTables.init.js"; // Incicializacao DataTables
//$t_body->block("BLOCO_JAVASCRIPT");

$t_body->ARQUIVO_JS = "consultabasica.js";
$t_body->block("BLOCO_JAVASCRIPT");

// Inclusão CSS da Página
//$t_body->ARQUIVO_CSS = "";
//$t_body->block("BLOCO_CSS");
/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>

