<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 22/07/2019
  Sistema: Process_XAMPP
 */

require_once(FILES_ROOT . "include/resource01.php");

/**
 * ROTA: api/v1/procoloscaruana
 */
function pegaListaProtocolosCaruanaImportar()
{
    global $connect;
    $procCode = "CARUANA_PROTOCOLOS";
    $procId = PegaProcIdByCode($procCode);
    $stepId = PegaStepIdByCode($procId, "IMPORTAR_PROTOCOLO");
    $SQL = "select caseId from casequeue where procid = $procId and stepId = $stepId limit 1";
    $queryListaCasos = mysqli_query($connect, $SQL);

    $resultado = array();
    if (mysqli_num_rows($queryListaCasos) == 0) {
        header('Content-Type: application/json');
        $json_resultado = json_encode($resultado);
        echo $json_resultado;
        exit;
    }
    
    $listaCasoProtocolo = mysqli_fetch_all($queryListaCasos, MYSQLI_ASSOC);
    
    $caseNumProtocolo = $listaCasoProtocolo[0]["caseId"];
    $resultado = array();
    $sql = "select 
            FieldCod, 
            FieldValue 
            from 
            casedata, 
            procfieldsdef 
            where 
            procfieldsdef.procid = $procId and "
            . "casedata.fieldid = procfieldsdef.fieldid and "
            . "casedata.procid = procfieldsdef.procid and "
            . "fieldcod in ('IMAGENS_INT', 'DATA_DO_PROTOCOLO', 'NR_PROTOLOCO') and "
            . "casenum = $caseNumProtocolo";
    $query = mysqli_query($connect, $sql);

    $listaCamposProtocolo = mysqli_fetch_all($query, MYSQLI_ASSOC);

    // CaseNum do Protocolo
    $dados["caseNum"] = $caseNumProtocolo;

    // Cria os dados do Protocolo
    foreach ($listaCamposProtocolo as $campo)
    {
        $dados[$campo["FieldCod"]] = $campo["FieldValue"];
    }
   
    header('Content-Type: application/json');
    $json_resultado = json_encode($dados);
    echo $json_resultado;
}
