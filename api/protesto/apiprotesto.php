<?php

use OpenApi\Annotations as OA;
use raelgc\view\Template;
require_once(FILES_ROOT . "include/resource01.php");
require_once(FILES_ROOT . "includes_ws/wscasesave.inc");
require_once(FILES_ROOT . "includes_ws/wsuser.inc");

//namespace protesto;

//class ProtestoController
{

function enviaProtesto()
{
    //insereDadosRecebidos();
    header("HTTP/1.0 201 Created");
    header("Access-Control-Allow-Origin: *");
    header('Content-Type: application/json');
    echo $jDados;
}

/**
 * @OA\Post(
 *    path="/protesto",
 *    summary="",
 *    operationId="cancelamento_protesto",
 *    tags={"protesto"},
 *    @OA\Response(response=201, description="Solicitação Recebida."),
 *    @OA\Response(
 *        response="default",
 *        description="unexpected error",
 *        @OA\Schema(ref="#/components/schemas/Error")
 *    )
 * )
 */

function cancelaProtesto()
{
    header("HTTP/1.0 201 Created");
    header("Access-Control-Allow-Origin: *");
    header('Content-Type: application/json');
    echo $jDados;
}

/**
 * @OA\Post(
 *    path="/protesto",
 *    summary="",
 *    operationId="desistencia_protesto",
 *    tags={"protesto"},
 *    @OA\Response(response=201, description="Solicitação Recebida."),
 *    @OA\Response(
 *        response="default",
 *        description="unexpected error",
 *        @OA\Schema(ref="#/components/schemas/Error")
 *    )
 * )
 */
function desistenciaProtesto()
{
    header("HTTP/1.0 201 Created");
    header("Access-Control-Allow-Origin: *");
    header('Content-Type: application/json');
    echo $jDados;
}

function statusById()
{
    
        $jRetorno = json_encode(""); 
    
    header('Content-Type: application/json');
    echo $jRetorno;
}


function insereDadosRecebidos()
{
    try {
        global $connect;        
        $procCod = "PROTESTO";
        $dadosEntrada = file_get_contents('php://input');

        // Cria o campo unico da CESSAO = NUMERO CESSAO + BUCKET ORIGEM + BUCKET DESTINO
        $sessionUUID = get_guid();

        $campo["fieldCode"] = "CESSION_UUID";
        $campo["fieldValue"] = $sessionUUID;
        $camposEnvio[] = $campo;

       // if (key_exists("Protesto", $dadosEntrada)) {

            // Valida os dados de todos os Contratos

            // Array com os campos a serem atualizados
            $protestos = $dadosEntrada;
            $campo["fieldCode"] = "PROTESTO";
            $campo["fieldValue"] = json_encode($protestos);
            $camposEnvio[] = $campo;
       // }
        $dadosCaso["Fields"] = $camposEnvio;
        // Pega o ProcId do Caso
        $procId = PegaProcIdByCode($procCod);

        // Busca o Campo UNIQUE
        //$camposUniqueClose = PegaCampoUniqueClose($procId);

        // Pega o campo unique do Caso
        $fieldUnique = $camposUniqueClose["FieldUnique"];

        // Verfifica o valor Enviado do campo unique
        $valorCampoUnique = buscaValorCampoUnique($procId, $camposEnvio, $fieldUnique);


        // Verfifica se é caso unico
        $numeroCasoExiste = pegaNumerCasoUnicoAberto($procId, $valorCampoUnique);

        if ($numeroCasoExiste == 0) {
            $numeroCaso = 0;
           // function FuncCaseSave($AuthUser, $ProcCode, $StepCode, $CaseNum, $dadosDoCaso, $Acao, $NovoCaso, $GravarEntrada = false)
            $caseNum = FuncCaseSave($userData, $procId, 'INICIO', $numeroCaso, $dadosCaso, 0, 1);
            if ($caseNum === 0) {
                header("HTTP/1.0 201 Created");
                $novoCaseNum["process"] = $sessionUUID;
                $novoCaseNum["error"] = array();
            }
        } else {
            header("HTTP/1.0 402 Falha");
            $novoCaseNum["process"] = "";
            $novoCaseNum["Error"] = "";
        }
        $dadosRetorno = $novoCaseNum;
        $jDados = json_encode($dadosRetorno);
        if (json_last_error() > 0) {
            $erroJSON = json_last_error() . " " . json_last_error_msg();
            error_log("Json erro: " . $erroJSON);
        }
    } catch (Exception $ex) {
        $jDados = json_encode($ex);
    }
    header("Access-Control-Allow-Origin: *");
    header('Content-Type: application/json');
    echo $jDados;
}

function get_cession_resolution_queue($procCod)
{
    global $connect;
    $procId = PegaProcIdByCode($procCod);
    $campoStatusQueue = PegaFieldIdByCode($procId, "CESSION_STATUS");
    $cessionUuid = PegaFieldIdByCode($procId, "CESSION_UUID");
    $sql = "select Campo$cessionUuid as cession_id, Campo$campoStatusQueue as statusCession from exportkeys where procid = $procId and Campo$campoStatusQueue in ('queued', 'cancel', 'test', 'waiting') limit 1";
    $query = mysqli_query($connect, $sql);
    $listaCessoes = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $jDados = json_encode($listaCessoes);
    header('Content-Type: application/json');
    echo $jDados;
}
}