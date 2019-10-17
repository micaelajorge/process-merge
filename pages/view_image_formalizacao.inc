<?php

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/master/t_master.html");
$t_body->addFile("PAGEHEADER", FILES_ROOT . "assets/templates/master/t_header.html");
$t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/t_blank_page.html");
$t_body->SITE_LOCATION = SITE_LOCATION;

// Adiciona o css especifico da página
$t_body->ARQUIVO_CSS = "editcase.css";
$t_body->block("BLOCO_CSS");

$t_body->CONTENT_HEADER_HIDEN = "hide";

$t_body->TITULO_PAGE = "";
$t_body->CLASSEFORMULARIO = "DivFieldForm";

$t_body->WRAPPER_STYLE = "style=\"width: 100%; margin-left: 0px !important\"";
$t_body->SIDEBAR_HIDE = "hide";
$t_body->MAIN_FOOTER_HIDEN = "hide";
// </editor-fold>

$t_body->ARQUIVO_JS = "selcase_datatable.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "editcase.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "validacao.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "validadata.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "showdata.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "CLOSE.JS";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "posicionar.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "seluser.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "browserSniffer.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "dynCalendar.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "ajax.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "window.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "historico.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "posicionar.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->ARQUIVO_JS = "menu.js";
$t_body->block("BLOCO_JAVASCRIPT");
$t_body->block("BLOCK_CONTENT");

$t_body->ARQUIVO_CSS = "panimage.css";
$t_body->block("BLOCO_CSS");

$t_body->ARQUIVO_JS = "panimage.js";
$t_body->block("BLOCO_JAVASCRIPT");


$t_body->INCLUDE_JAVASCRIPT = "window.opener.jsMoveObjetoParaJanelaFormalizacao(window);";
$t_body->block("BLOCO_INCLUDE_JAVASCRIPT");

$t_body->INCLUDE_JAVASCRIPT = " window.onbeforeunload = function(){
   jsDevolveObjetoParaJanela(window);    
 };
";
$t_body->block("BLOCO_INCLUDE_JAVASCRIPT");


$t_body->show();