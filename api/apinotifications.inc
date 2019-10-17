<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 23/07/2018
  Sistema: Process_XAMPP
 */

// <editor-fold defaultstate="collapsed" desc="Includes do Script">
require_once(FILES_ROOT . "include/resource01.php");
require_once(FILES_ROOT . "include/resource02.php");
include_once(FILES_ROOT . "includes_ws/wsuser.inc");

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Carregamento de Variaveis da sessão">
#Ativa o Processo
//$Linhas = 200;	
// </editor-fold>	

/**
 * Rota: countbyfield
 * 
 * @global type $fieldId
 * @global type $ProcId
 */
function ContaCasosPorCampo()
{
    global $fieldId, $ProcId;
    $StepId = "";
    
    if (!is_numeric($ProcId))
    {
        $ProcId = PegaProcIdByCode($ProcId);
    }
    $TotalCasosPorCampo = TotalCasosPorCampo($ProcId, $StepId, $fieldId);
    $resultado = array();
    $contador = 0;
    
    $listaStatus = PegaValoresCampoLista_array($ProcId, 10);
    $indicesStatus = array_column($listaStatus, "valor");
    for ($i = 0; $i < count($listaStatus); $i++)
    {
        $listaStatus[$i]["total"] = 0;
        $listaStatus[$i]["cor_texto"] = 'text-gray';
        $listaStatus[$i]["cor_fundo"] = 'bg-gray';
    }
    foreach ($TotalCasosPorCampo as $item)
    {        
        $posStatus = array_search($item["status"], $indicesStatus);
        $listaStatus[$posStatus]["cor_texto"] = 'text-green';
        $listaStatus[$posStatus]["cor_fundo"] = 'bg-green';
        $listaStatus[$posStatus]["total"] = $item["total"];
        $contador++;
    }
    header('Content-Type: application/json');
    echo json_encode($listaStatus);    
}

function NotificacoesUsuario()
{
    global $ProcCode, $UserName, $UserPassword, $userdef;

    if ($UserName != "") {
        $DadosUsuario["UserName"] = $UserName;
        $DadosUsuario["UserPassword"] = $UserPassword;
        $UsuarioAtual = TestaValidacaoUsuario($DadosUsuario);
    } else {
        $UsuarioAtual = $userdef;
    }
    // Se não autenticou, retorna a falha
    if (!$UsuarioAtual->Validado) {
        $retorno["total"] = 0;
        $retorno["retorno"] = "falha autenticacao";
        header('HTTP/1.1 401 Unauthorized');
    } else {
        // Verifica se o ProcCode é um código alpha
        if (!is_numeric($ProcCode) && $ProcCode != "") {
            $ProcId = PegaProcIdByCode($ProcCode);
        } else {
            $ProcId = "";
        }

        // Verifica se é EXATAMENTE ZERO para fazer a pesquisa geral por notificacoes
        if ($ProcId === 0) {
            $retorno["total"] = 0;
            $retorno["retorno"] = "Processo Não Encontrado";
        } else {
            $Query = NovosCasosUsuario($ProcId, $UsuarioAtual->Origem);
            $Linha = mssql_fetch_array($Query, MSSQL_ASSOC);
            $retorno["total"] = $Linha["total"];
            $retorno["retorno"] = "Ok";
        }
    }

    header('Content-Type: application/json');
    echo json_encode($retorno);
}

function notificacoesLidas()
{
    global $connect, $userdef;

    $lastnotification = date("Y-m-d H:i:s");
    $userdef->lastnotification = date("Y-m-d H:i:s");

    $SQL = "update userdef set lastnotification = '$lastnotification' where UserId = " . $userdef->UserId;
    mysqli_query($connect, $SQL);

    $retorno["retorno"] = "ok";
    header('Content-Type: application/json');
    echo json_encode($retorno);
}

/*
 * ROTA: api/casobloqueado/
 */
function casoBloqueado()
{
    global $connect, $userdef;
    $dadosEntrada = apiTrataDadosEntrada();
    //$SQL = "select lockedbyid from casequeue where ProcId = {$dadosEntrada["procId"]} and stepId = {$dadosEntrada["stepId"]} and caseid = {$dadosEntrada["caseNum"]} and UserId = " . $userdef->UserId;
    $SQL = "select lockedbyid from casequeue where ProcId = {$dadosEntrada["procId"]} and stepId = {$dadosEntrada["stepId"]} and caseid = {$dadosEntrada["caseNum"]}";
    $Query = mysqli_query($connect, $SQL);
    $linhas = mysqli_fetch_array($Query);
    if (count($linhas) > 0) {
        $retorno["bloqueio"] = $linhas["lockedbyid"];
    } else {
        $retorno["bloqueio"] = $userdef->UserId;
    }
    header('Content-Type: application/json');
    echo json_encode($retorno);
}
