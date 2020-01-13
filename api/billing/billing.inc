<?php

//require_once(FILES_ROOT . "include/resource01.php");
//require_once(FILES_ROOT . "include/resource02.php");
//
require_once(FILES_ROOT . "billing/api_relatorioonline.inc");


function Mes()
{
    global $connect;
    $SQL = MockCreditasGridConsulta();

    $dadosEntrada = apiTrataDadosEntrada();

    $retorno = array();
    $query = mysqli_query($connect, $SQL);
    $retorno = mysqli_fetch_all($query, MYSQLI_ASSOC);
    $TotalRegistros = mysqli_num_rows($retorno);
    
    $dadosRetorno = apiTrataDadosSaida($dadosEntrada, $retorno, $TotalRegistros, $TotalRegistros);
    $jDados = json_encode($dadosRetorno); 
    
    header('Content-Type: application/json');
    echo $jDados;
    
}

function CarregaRetorno()
{
    $jDados = "";
    echo $jDados;
}

function MockEstados()
{
    global $connect;
    
    $SQL = BuscaEstados();

    $dadosEntrada = apiTrataDadosEntrada();

    $retorno = array();
    $query = mysqli_query($connect, $SQL);
    $retorno = mysqli_fetch_all($query, MYSQLI_ASSOC);
    $TotalRegistros = mysqli_num_rows($retorno);
    
    $dadosRetorno = apiTrataDadosSaida($dadosEntrada, $retorno, $TotalRegistros, $TotalRegistros);
    $jDados = json_encode($dadosRetorno); 
    
    header('Content-Type: application/json');
    echo $jDados;    
}

function MockMunicipio()
{
    global $connect, $estado;
    $SQL = BuscaMunicipios($estado);

    $dadosEntrada = apiTrataDadosEntrada();

    $retorno = array();
    $query = mysqli_query($connect, $SQL);
    $retorno = mysqli_fetch_all($query, MYSQLI_ASSOC);
    $TotalRegistros = mysqli_num_rows($retorno);
    
    $dadosRetorno = apiTrataDadosSaida($dadosEntrada, $retorno, $TotalRegistros, $TotalRegistros);
    $jDados = json_encode($dadosRetorno); 
    
    header('Content-Type: application/json');
    echo $jDados;    
}

function listaCartorios()
{
    require_once FILES_ROOT . "include/resource01.php";
    $dadosEntrada = apiTrataDadosEntrada();


    $inicio = $dadosEntrada["start"];

    $listaCasos = pegaListaCartorios($ProcSource, $FieldSource, $pracaBusca, $inicio);

    $aResultado = $listaCasos["listaRegistros"];
    $totalTabela = $listaCasos["totalRegistros"];
    $registrosFiltrados = count($aResultado);
    $dadosRetorno = apiTrataDadosSaida($dadosEntrada, $aResultado, $registrosFiltrados, $totalTabela);
    $jDados = json_encode($dadosRetorno);

    header('Content-Type: application/json');
    echo $jDados;
}
