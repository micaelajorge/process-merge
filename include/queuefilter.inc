<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 07/01/2019
  Sistema: Process_XAMPP
 * 
 * @rota queuefilters
 */


require_once(FILES_ROOT . "include/resource01.php");

use raelgc\view\Template;

if ($templateFilter == "")
{
    $templateFilter = "t_queue_filter.html";
}

if ($templateFields == "")
{
    $templateFields = "t_queue_filter_form.html";
}


$t_body = new Template(FILES_ROOT . "assets/templates/$templateFilter");
$t_body->addFile("FORM_FIELDS", FILES_ROOT . "assets/templates/$templateFields");

$procId = PegaProcIdByCode($procId);

$SQL = "select *, FieldRazaoOrder + FieldName as order_sort from procfieldsdef where ProcId = $procId and FieldKey = 1 and FieldSpecial = 0 and FieldRazao >= 0 order by order_sort";
$Query = mysqli_query($connect, $SQL);
$camposFiltro = mysqli_fetch_all($Query, MYSQLI_ASSOC);
$procdef = $_SESSION["procdef"];
$filtros = $procdef->filterQueue;

foreach ($camposFiltro as $campo) {
    $t_body->CAMPO_NOME = $campo["FieldName"];
    $t_body->CAMPO_TIPO = $campo["FieldType"];
    $t_body->CAMPO_ID = $campo["FieldId"];
    $t_body->CAMPO_VALOR = "";
    $valorCampo = "";
    foreach ($filtros as $filtro) {
        if (key_exists("filter", $filtro))
        {
            $filtro = $filtro["filter"];
        }
        if (key_exists("filtro", $filtro))
        {
            $filtro = $filtro["filtro"];
        }
        if ($filtro["campo"] === $campo["FieldId"]) {
            // Pega o Valor setado no Filtro
            $valorCampo = $filtro["valor"];
        }
    }
    switch ($campo["FieldType"]) {
        case "LT":            
            // Seta como NU para tratar diretamente o valor que está gravado na tabela EXPORTKEYS
            $t_body->CAMPO_TIPO = "NU";
            $nomeBlocoCampo = criaCampoFiltroTL($t_body, $valorCampo, $campo["ExtendProp"]);            
            break;
        case "DT":
            $valorCampo = $valorCampo[0];
            $valorCampo = ConvDate($valorCampo);
            $nomeBlocoCampo = criaCampoFiltroInput_DT($t_body, $valorCampo);
//            $nomeBlocoCampo = criaCampoFiltroInput($t_body, $valorCampo);
            break;
        default:
            $valorCampo = $valorCampo;
            $nomeBlocoCampo = criaCampoFiltroInput($t_body, $valorCampo);
            break;
    }
    $t_body->block($nomeBlocoCampo);
}
$t_body->show();

function criaCampoFiltroInput($t_body, $valorCampo)
{
    $t_body->CAMPO_VALOR = $valorCampo;
    return "BLOCO_CAMPO_FILTRO";
}

function criaCampoFiltroInput_DT($t_body, $valorCampo)
{
    $t_body->CAMPO_VALOR = $valorCampo;
    return "BLOCO_CAMPO_FILTRO_DT";
}


function criaCampoFiltroTL($t_body, $valorCampo, $valoresDispCampo)
{
    $itensLista = explode(".", $valoresDispCampo);
    foreach ($itensLista as $item) {
        $aValores = explode("|", $item);
        $selecionado = "";
        if ($aValores[1] == $valorCampo) {
            $selecionado = "selected";
        }
        $t_body->OPT_VALOR = $aValores[1];
        $t_body->OPT_DESC = $aValores[0];
        $t_body->OPT_SELECTED = $selecionado;
        $t_body->block("BLOCO_OPCAO_LT");
    }
    return "BLOCO_CAMPO_FILTRO_LT";
}