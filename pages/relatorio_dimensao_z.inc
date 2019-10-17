<?php

/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 15/11/2018
Sistema: Process_XAMPP
*/

use raelgc\view\Template;

$dadosEntrada = apiTrataDadosEntrada();

$t_body = new Template(FILES_ROOT . "assets/templates/master/t_datatable_dimensao.html");

$nomeDimencao = $dadosEntrada["dimensao"];

$t_body->nomeDataTable = "tableData_$nomeDimencao";
$t_body->dimensao = $nomeDimencao;

// <editor-fold defaultstate="collapsed" desc="Cabecalho da Tabela de casos">
$cabecalhoTabela = array(
    "",
    "Data Entrada",
    "Total"
);

foreach ($cabecalhoTabela as $cabecalho) {
    $t_body->TEXTO_CABECALHO = $cabecalho;
    $t_body->block("BLOCK_CABECALHO_TABELA");
}
// </editor-fold>
$t_body->show();