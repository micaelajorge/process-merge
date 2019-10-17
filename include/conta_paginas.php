<?php

/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 04/09/2019
Sistema: Process_XAMPP
*/

require_once(FILES_ROOT . "/include/resource01.php");

$pageCount = pdfCountPages("e:/developer/amostras/exemplo.pdf");

echo "$pageCount";