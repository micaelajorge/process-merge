<?php

// <editor-fold defaultstate="collapsed" desc="Requires do script">



function PegaUsuariosLogados()
{
    $conect = AtivaDBExterno();
    $SQL = "select USERID from userlogins";
    $Query = mysqli_query($conect, $SQL);
    $Retorno = mysqli_fetch_all($Query, MYSQLI_ASSOC);
    return $Retorno;
}

function DesconectaUsuario($userId)
{
    $conect = AtivaDBExterno();
    $SQL = "delete from userlogins where UserId = $userId";
    $Query = mysqli_query($conect, $SQL);
}

function ProcuraUsuario($UserId, $Lista)
{
    $retorno = "0";
    foreach ($Lista as $Usuario) {
        if ($Usuario["USERID"] == $UserId) {
            $retorno = "1";
            break;
        }
    }
    return $retorno;
}

//require_once("include/common.php");
require_once("include/resource01.php");
//include("include/validacaoapi.php");

$dadosEntrada = apiTrataDadosEntrada();
$filtros = apiTrataCampos($dadosEntrada);

if (isset($_GET["DISCONECT"])) {
    DesconectaUsuario($_GET["USERID"]);
    unset($_GET["USERID"]);
}

//$SQL = "select USERID, USERNAME, USERDESC, EMAIL, UACTIVE from userdef where UACTIVE = 1 order by UserId";
$SQL = "select ORIGEM_USER, ORIGEM_DOC from origemdominio ";

$SQLOrder = "order by ORIGEM_USER";

$where = " ";
$sqlAnd = " ";
if (isset($dadosEntrada["search"]["value"])) {
    if ($dadosEntrada["search"]["value"] != "") {
        $sqlAnd = " ";
        $sqlCondicoes = " where ";
        $sqlCondicoes .= " ( "
                . "ORIGEM_USER like '{$dadosEntrada["search"]["value"]}%' "
                . "or "
                . "ORIGEM_DOC like '{$dadosEntrada["search"]["value"]}%' "
                . ")";
    }
}

$SQL .= $sqlCondicoes . $sqlAnd . $sqlUserId . $SQLOrder;

$Query = mysqli_query($connect, $SQL, MYSQLI_ASSOC);

//Move o Ponteiro para a posição indicada
//if (isset($dadosEntrada["start"]))
//{
//    $posicao = $dadosEntrada["start"] + 1;
//    //$retorno_movimento = mysqli_data_seek($Query, $posicao);
//}    

$aLinhas = mysqli_fetch_all($Query, MYSQLI_ASSOC);

$listaUsuarios = array();

$contadorItens = 0;
$totalMaxRegistros = 10;
if (isset($dadosEntrada["length"])) {
    $totalMaxRegistros = $dadosEntrada["length"];
}

$posicao = $dadosEntrada["start"];

$contadorItens = 0;
foreach ($aLinhas as $Linha) {
// Sai do loop se alcançou a quantidade
    if ($contadorItens <= $posicao)
    {
        $contadorItens++;
        continue;
    }
    if ($contadorItens == $totalMaxRegistros + $posicao) {
        break;
    }
    $listaUsuarios[] = $Linha;
    $contadorItens++;
}

$registrosFiltrados = count($aLinhas);
$totalTabela = registrosTabela('origemdominio');

if ($registrosFiltrados == 0) {
    $registrosFiltrados = $totalTabela;
}

$dadosRetorno = apiTrataDadosSaida($dadosEntrada, $listaUsuarios, $totalTabela, $registrosFiltrados);
$jDados = json_encode($dadosRetorno);

header('Content-Type: application/json');
echo $jDados;
?>