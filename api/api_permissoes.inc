<?php

//require_once("include/common.php");
require_once("include/resource01.php");
//include("include/validacaoapi.php");

/**
 * 
 * @global type $connect
 */
function permissoes_listaProcessos()
{
    global $connect;
    $dadosEntrada = apiTrataDadosEntrada();
    $filtros = apiTrataCampos($dadosEntrada);

    $SQL = "select procid, proccod, procname, procdesc, tipoproc from procdef ";

    if ($filtros["search"] !== "") {
        $search = $filtros["search"];
        $SQL .= " where proccod like '$search%' or procname like '$search%' or procdesc like '$search'";
    }

    if ($filtros["order"] != "") {
        $SQL .= " order by procname, {$filtros["order"]["campo"]} {$filtros["order"]["ordem"]}";
    } else {
        $SQL .= " order by procname";
    }
    $Query = mysqli_query($connect, $SQL);

    $listaProcessos = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $inicio = $dadosEntrada["start"];
    $fim = $dadosEntrada["length"];
    $listaProcessos = array_slice($listaProcessos, $inicio, $fim);

    $TotalRegistros = registrosTabela('procdef');
    $totalTabela = $TotalRegistros;
    $registrosFiltrados = mysqli_num_rows($Query);
    $dadosRetorno = apiTrataDadosSaida($dadosEntrada, $listaProcessos, $totalTabela, $registrosFiltrados);
    $jDados = json_encode($dadosRetorno);

    header('Content-Type: application/json');
    echo $jDados;
}

function usuariosGruposPorStep($procId, $stepId)
{
    global $connect;
    $SQL = "select 
        stepaddresses.groupid,
        username as entidade, 
        grpfld 
        from 
        stepaddresses, userdef 
        where 
        procid = $procId 
        and 
        stepId = $stepId 
        and  
        userId = stepaddresses.GroupId 
        and 
        GrpFld = 'U'
        union
        select 
        stepaddresses.groupid,
        groupname as entidade, 
        GrpFld 
        from 
        stepaddresses, 
        groupdef 
        where  
        procid = $procId 
        and 
        stepId = $stepId
        and 
        groupdef.groupId = stepaddresses.GroupId 
        and 
        GrpFld = 'G'
        ";
    $Query = mysqli_query($connect, $SQL);
    $retornoGruposUsuarios = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $SQL = "select 
        stepaddresses.groupid,
        fieldname as entidade, 
        grpfld 
        from 
        stepaddresses, procfieldsdef 
        where 
        stepaddresses.procid = $procId 
        and 
        stepaddresses.stepId = $stepId 
        and
        procfieldsdef.procid = $procId 
        and  
        procfieldsdef.fieldid = stepaddresses.GroupId 
        and 
        GrpFld = 'F'
        ";
    $Query = mysqli_query($connect, $SQL);
    $retornoCampos = mysqli_fetch_all($Query, MYSQLI_ASSOC);
   
    $retorno = array_merge($retornoGruposUsuarios, $retornoCampos);
    return $retorno;
}

/**
 * 
 * @global type $connect
 */
function permissoes_listaSteps()
{
    global $connect;
    $dadosEntrada = apiTrataDadosEntrada();
    $filtros = apiTrataCampos($dadosEntrada);

    $procIdPermissoes = $_SESSION["procIdPermissoes"];

    $SQL = "select procid, stepid, stepname, steptype from stepdef where procId = $procIdPermissoes and steptype not in ('M', 'L') " ;

//    if ($filtros["order"] != "") {
//        $SQL .= " order by FIELD(steptype, 'F', 'P', 'T', 'F', 'D', 'R'), {$filtros["order"]["campo"]} {$filtros["order"]["ordem"]}";
//    } else {
//        $SQL .= " order by FIELD(steptype, 'F', 'P', 'T', 'F', 'D', 'R'),  stepname ";
//    }

    $SQL .= " order by FIELD(steptype, 'F', 'P', 'T', 'F', 'D', 'R'),  stepname ";
//    error_log("SQL permissoes " . $SQL);
    
    $Query = mysqli_query($connect, $SQL);

    $listaSteps = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    for ($i = 0; $i < count($listaSteps); $i++)
    {
        $listaSteps[$i]["permissoes"] = usuariosGruposPorStep($procIdPermissoes, $listaSteps[$i]["stepid"]);
    }
        
    $inicio = $dadosEntrada["start"];
    $fim = $dadosEntrada["length"];
    $listaSteps = array_slice($listaSteps, $inicio, $fim);

    $registrosFiltrados = mysqli_num_rows($Query);

    $totalTabela = $registrosFiltrados;
    $dadosRetorno = apiTrataDadosSaida($dadosEntrada, $listaSteps, $totalTabela, $registrosFiltrados);
    $jDados = json_encode($dadosRetorno);

    header('Content-Type: application/json');
    echo $jDados;
}

/**
 * 
 * @global type $connect
 */
function permissoes_salvapermissoes()
{
    global $connect;
    $dadosEntrada = apiTrataDadosEntrada();
    
   (key_exists("VIEW_QUEUE", $dadosEntrada )) ? $viewQueue = 1 : $viewQueue = 0;
   (key_exists("EDIT_ITEM", $dadosEntrada )) ? $editItem = 1 : $editItem = 0;
   (key_exists("CRIAR_NOVO", $dadosEntrada )) ? $criarNovo = 1 : $criarNovo = 0;
   
       
    switch ($dadosEntrada["ADDEDIT"])
    {
        // Adicionar permissoes Entidade
        case "A":
            $SQL = "insert into "
                . "stepaddresses "
                . "( "
                . "procId, "
                . "stepId,"
                . "grpfld,"
                . "groupid,"
                . "action,"
                . "newdoc,"
                . "viewqueue"
                . ") values ("
                . " {$dadosEntrada["PROCID"]},"
                . " {$dadosEntrada["STEPID"]},"
                . " '{$dadosEntrada["GRPFLD"]}',"
                . " '{$dadosEntrada["GROUPID"]}',"
                . " $editItem,"
                . " $criarNovo,"
                . " 1" // Seta para viewqueue automaticamente
                . ")";
            break;
        
        // Editar permissoes Entidade
        case "E":
            $SQL = "update stepaddresses set "
                . "Action = $editItem, "
                . "NewDoc = $criarNovo,"
                . "ViewQueue = $viewQueue "
                . "where "
                . "procId = {$dadosEntrada["PROCID"]} "
                . "and "
                . "stepId = {$dadosEntrada["STEPID"]} "
                . "and  "
                . "grpfld = '{$dadosEntrada["GRPFLD"]}'"
                . "and  "
                . "groupid = {$dadosEntrada["GROUPID"]}";                
            break;
        
        // Remover entidade
        case "D":
            $SQL = "delete from stepaddresses "
                . "where "
                . "procId = {$dadosEntrada["PROCID"]} "
                . "and "
                . "stepId = {$dadosEntrada["STEPID"]} "
                . "and  "
                . "grpfld = '{$dadosEntrada["GRPFLD"]}'"
                . "and  "
                . "groupid = {$dadosEntrada["GROUPID"]}";                
            break;
    }      
    mysqli_query($connect, $SQL);
}