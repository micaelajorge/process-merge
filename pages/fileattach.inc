<?php

// <editor-fold defaultstate="collapsed" desc="Chamada Classe de Template">

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_uploadfile.html");
// </editor-fold>


$t_body->MAXFILESIZE = ini_get('upload_max_filesize');
$t_body->PROCID = $ProcId;
$t_body->CASENUM = $CaseNum;
$t_body->FIELDID = $FieldId;
$t_body->show();



