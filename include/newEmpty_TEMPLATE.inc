<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 03/11/2018
  Sistema: Process_XAMPP
 */

require_once(FILES_ROOT . "vendor/lib/raelgc/view/template.php");

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/master/t_master.html");
$t_body->addFile("PAGEHEADER", FILES_ROOT . "assets/templates/master/t_header.html");
$t_body->addFile("PAGEMENULEFT", FILES_ROOT . "assets/templates/master/t_menu.html");
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/master/t_content.html");
$t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");
$t_body->SITE_LOCATION = SITE_LOCATION;
$t_body->TITULO_PAGE = "Creditus | Pagina em Branco";
$t_body->TEXT_HEADER = "Creditus";
$t_body->TEXT_HEADER_SMALL = "Parceria de Resultados";

// Inclusão Javascript da Página
//$t_body->ARQUIVO_JS = "dataTables.init.js"; // Incicializacao DataTables
//$t_body->ARQUIVO_JS = "";
//$t_body->block("BLOCO_JAVASCRIPT");
// Inclusão CSS da Página
//$t_body->ARQUIVO_CSS = "";
//$t_body->block("BLOCO_CSS");
/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>

