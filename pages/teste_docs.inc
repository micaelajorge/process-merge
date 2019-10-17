<?php

/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 05/06/2019
Sistema: Process_XAMPP
*/

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/master/t_master.html");
$t_body->addFile("PAGEHEADER", FILES_ROOT . "assets/templates/master/t_header.html");
$t_body->addFile("PAGEMENULEFT", FILES_ROOT . "assets/templates/master/t_menu.html");
$t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/t_teste_docs.html");
$t_body->SITE_LOCATION = SITE_LOCATION;

$t_body->block("BLOCK_CONTENT");

$t_body->show();