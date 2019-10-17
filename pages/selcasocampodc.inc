<?php
/**
 * 
 * ROTA: selcasocampodc
 */


// //Versao 1.0.0 /Versao
include(FILES_ROOT . "include/resource01.php");
include(FILES_ROOT . "include/resource02.php");

$dir =  dirname(__FILE__);

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_selcase.html");

if (!is_numeric($ProcSource))
{
    $ProcIdSource = PegaProcIdByCode($ProcSource);
}
 else {
    $ProcIdSource = $ProcSource;
}

$nomeProcesso = PegaNomeProc($ProcIdSource);
$t_body->PROCNAME = $nomeProcesso;
$t_body->FIELDID = $fieldid;
// <editor-fold defaultstate="collapsed" desc="Cabecalho da Tabela">
$cabecalhoTabela = array(
    "#",
    "Referências"
);

foreach ($cabecalhoTabela as $cabecalho) {
    $t_body->TEXTO_CABECALHO = $cabecalho;
    $t_body->block("BLOCK_CABECALHO_TABELA");
}
// </editor-fold>

echo $t_body->parse();