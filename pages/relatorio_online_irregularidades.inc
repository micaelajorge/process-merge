<?php
/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 21/07/2018
  Sistema: Process
 */

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/func_api.inc");
include_once(FILES_ROOT . "include/toolbar.inc");
require_once(FILES_ROOT . "include/resource02.php");
require_once(FILES_ROOT . "include/resource01.php");

use raelgc\view\Template;

$dadosEntrada = apiTrataDadosEntrada();
$filtros = apiTrataCampos($dadosEntrada);

if (isset($_SESSION[$ProcId]["procdef"])) {
    $procdef = $_SESSION["procdef"];
    if ($procdef->ProcId != $ProcId) {
        $procdef = new procdef;
        $procdef->Create($ProcId, $connect);
        $_SESSION[$ProcId]["procdef"] = $procdef;
        $ProcId = $procdef->ProcId;
        $_SESSION[$ProcIdAtivo] = $ProcId;
    }
} else {
    if (!isset($ProcId))
    {
        $ProcId = PegaProcIdByCode("TRIBANCO");
    }    
    $procdef = new procdef;
    $procdef->Create($ProcId, $connect);
    $_SESSION[$ProcId]["procdef"] = $procdef;
    $_SESSION["procdef"] = $procdef;
}

$t_body = new Template(FILES_ROOT . "assets/templates/master/t_relatorio_online_datatable.html");

// Cria a Tabela de Dimensao mandada pelo parametro da rota
$t_body->nomeDataTable = "dimensao" . $PARAMETROS_ROTA[0];
$t_body->DIMENSAO_RELONLINE = $PARAMETROS_ROTA[0] + 1;


$captionDatatable = urldecode($PARAMETROS_ROTA[1]) . "/" . urldecode($PARAMETROS_ROTA[2]);
if (count($PARAMETROS_ROTA) > 3)
{
    $captionDatatable .= urldecode($PARAMETROS_ROTA[3]);
}
$t_body->CAPTION_DATATABLE = $captionDatatable;
$t_body->CLASS_BOX_DATATABLE = "box-solid box-primary";
$t_body->CLASS_HEADER_BOX_DATATABLE = "initial";
$t_body->CLASS_DATATABLE_WRAPPER_HIDE = "initial";
$t_body->CLASS_BOX_DATATABLE_HIDE = "initial";
// <editor-fold defaultstate="collapsed" desc="Cabecalho da Tabela de casos">

switch ($PARAMETROS_ROTA[0])
{
    case 1:
        $cargo = "Gerente Area";
        break;
    case 2:
        $cargo = "Gerente Regional";
        break;
    case 3:
        $cargo = "Gerente Comercial";
        break;
}
$cabecalhoTabela = array(
    "Irregularidade",
    "%",
    "Total"
);

foreach ($cabecalhoTabela as $cabecalho) {
    $t_body->TEXTO_CABECALHO = $cabecalho;
    $t_body->block("BLOCK_CABECALHO_TABELA_CUBO");
}
$t_body->block("BLOCK_CABECALHO_CUBO");

// </editor-fold>
/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>


