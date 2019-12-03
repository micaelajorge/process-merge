<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 30/11/2019
  Sistema: Process_XAMPP
 */

require_once(FILES_ROOT . "vendor/lib/raelgc/view/Template.php");

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

/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>

