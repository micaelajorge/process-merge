<?php

require_once(FILES_ROOT . "include/resource01.php");
require_once(FILES_ROOT . "pages/editcase_campos.inc");
include_once(FILES_ROOT . 'includes_ws/funcsws.php');

use raelgc\view\Template;

function fieldFileSelect()
{
    geraPagina("t_field_select_file.html");
}

function fieldFileScan()
{
    geraPagina("t_field_scan.html");    
}

function geraPagina($template)
{
    $dadosEntrada = apiTrataDadosEntrada();
    $t_body = new Template(FILES_ROOT . "assets/templates/$template");
    $t_body->PROCID = $dadosEntrada["ProcId"];
    $t_body->CASENUM = $dadosEntrada["CaseNum"];
    $t_body->FIELDID = $dadosEntrada["FieldId"];
    $t_body->VALUEID = $dadosEntrada["ValueId"];
    $t_body->USERID = $dadosEntrada["UserId"];
    $t_body->show();
}
