<?php

use OpenApi\Annotations as OA;
use raelgc\view\Template;
require_once(FILES_ROOT . "include/resource01.php");
require_once(FILES_ROOT . "includes_ws/wscasesave.inc");
require_once(FILES_ROOT . "includes_ws/wsuser.inc");

function statusById()
{   
    try {
        global $connect, $numero_protesto;       
 
        $procCod = "PROTESTO_TITULOS"; 
        
        // Pega o ProcId do Caso
        $procId = PegaProcIdByCode($procCod);

        $campoNumeroProtesto = PegaFieldIdByCode($procId, "NUMERO_PROTESTO");
        $campoTituloStatus  = PegaFieldIdByCode($procId, "TITULO_STATUS");
        $campoResultado = PegaFieldIdByCode($procId, "ELEGIBILIDADE_RESULTADO");
        
        $SQL = "";
        if($numero_protesto != "")  
        {       
            $caseNum = pegaNumerCasoUnicoAberto($procId, $numero_protesto, $campoNumeroProtesto);
            $camposSelecionados = array();  
            $camposSelecionados =["ELEGIBILIDADE_RESULTADO","TITULO_STATUS"];
            $resultado = pegaDadosCaso($procId, $caseNum, $camposSelecionados);

            if($resultado["TITULO_STATUS"] == "") header("HTTP/1.0 404 Não encontrado.");
            else header("HTTP/1.0 200 Sucesso");
            $dadosRetorno["numero_protesto"] = $numero_protesto;
            $dadosRetorno["caso"] = $caseNum ;
            //$dadosRetorno["resultado"] = $resultado;
            $dadosRetorno["status"] = $resultado["TITULO_STATUS"];
            $dadosRetorno["Error"] =  $resultado["ELEGIBILIDADE_RESULTADO"];
            $jDados = json_encode($dadosRetorno);       
        }
        else 
        {
            header("HTTP/1.0 402 Falha");
            $dadosRetorno["status"] = $SQL ;
            $dadosRetorno["Error"] = "";
            $jDados = json_encode($dadosRetorno);       
        }
    }
    catch (Exception $ex) {
            $jDados = json_encode($ex);
    }
    finally {
        header("Access-Control-Allow-Origin: *");
        header('Content-Type: application/json');
        echo $jDados;
    }
}





