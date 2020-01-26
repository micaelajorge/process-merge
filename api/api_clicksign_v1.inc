<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 18/05/2019
  Sistema: Process_XAMPP
 */
require_once(FILES_ROOT . "/include/resource01.php");

/**
 * @rota api/v1/docsend
 */
function webhook_finalizados()
{
    global $connect;
    $SQL = "select * from webhooks where hookStatus = 0 and hookEvent = 'auto_close' limit 20";
    $Query = mysqli_query($connect, $SQL);

    $dadosRetornados = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $constantesClickSign = pegaConstantesProcesso("LOTE_ASSINATURAS");

    $listHooks = array();
    foreach ($dadosRetornados as $dados) {
        $item["hookId"] = $dados["hookId"];
        $dataWebHook = json_decode($dados["hookdata"]);
//        $item["headers"] = json_decode($dados["hookHeaders"]);
        $item["hookEvent"] = $dados["hookEvent"];
        $item["document_key"] = $dados["document_key"];
        $item["dataHook"] = $dataWebHook;
        $item["clickSignData"] = $constantesClickSign;
        //$item["document"] = json_decode($dados["hookdata"]);
        $listHooks[] = $item;
    }

    $retorno["numRec"] = count($dadosRetornados);
    $retorno["items"] = $listHooks;

    header('Content-Type: application/json');
    echo json_encode($retorno);
}

// <editor-fold defaultstate="collapsed" desc="Pegar Id caso documento @rota api/v1/getcasenumdocument">

/**
 * @rota api/v1/getcasenumdocument* 
 * 
 * @global type $connect
 */
function pegaConstantesProcesso($procId)
{
    global $connect;

    if (!is_numeric($procId)) {
        $procId = PegaProcIdByCode($procId);
    }

    $retorno = array();
    $SQL = "select * from internalconstants where procid = $procId";
    $query = mysqli_query($connect, $SQL);
    $listaConstantes = mysqli_fetch_all($query, MYSQLI_ASSOC);
    if (mysqli_errno($connect)) {
        $dadosErro[] = "2 Erro: " . mysqli_error($connect) . " $SQL";
    }

    foreach ($listaConstantes as $item) {
        $retorno[$item["ConstCode"]] = $item["ConstValue"];
    }
    return $retorno;
}

function getCaseNumDocument()
{
    global $connect;

    $retorno = array();
    $dadosEntrada = apiTrataDadosEntrada();
    $document_key = $dadosEntrada["document_key"];
    $procId = PegaProcIdByCode("DOCUMENTOS_LOTE");
    $SQL = "select procId, caseNum, fieldValue from casedata where fieldvalue = '$document_key' and procid = $procId";
    $Query = mysqli_query($connect, $SQL);
    $dadosCaso = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $caseNum = $dadosCaso[0]["caseNum"];

    $fieldBatchId = PegaFieldIdByCode($procId, 'BATCHID');
    $SQL = "select fieldValue from casedata where caseNum = $caseNum and fieldid = $fieldBatchId and procid = $procId";
    $Query = mysqli_query($connect, $SQL);
    $dadosBatch = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $batchId = $dadosBatch[0]["fieldValue"];

    $procId = PegaProcIdByCode('LOTE_ASSINATURAS');
    $fieldDir = PegaFieldIdByCode($procId, "DIRETORIO_DESTINO");

    $constantesProcesso = pegaConstantesProcesso($procId);

    $SQL = "select fieldValue from casedata where caseNum = $batchId and fieldid = $fieldDir and procid = $procId";
    $Query = mysqli_query($connect, $SQL);
    $dadosBatchDir = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $retorno = array_merge($constantesProcesso, $retorno);
    $retorno["caseNum"] = $caseNum;
    $retorno["batchId"] = $batchId;
    $retorno["dirDestino"] = $dadosBatchDir[0]["fieldValue"];
    $retorno["SQL"] = $SQL;
    if (mysqli_errno($connect)) {
        $retorno["error"] = mysqli_error($connect);
        //$retorno["SQL"] = $SQL;
    }
    header('Content-Type: application/json');
    echo json_encode($retorno);
}

// </editor-fold>

/**
 * @rota api/v1/getlistbatchssendemail
 * 
 * @global type $connect
 */
function getListBatchSendEmail()
{
    global $connect;
    $SQL = "select caseid as caseNum from 
casequeue,
procdef,
stepdef
where 
casequeue.procid = procdef.procid and
procdef.procid = stepdef.procid and 
casequeue.stepid = stepdef.stepid and 
procdef.proccod = 'LOTES_CLICKSIGN' and 
stepdef.stepcod = 'FALHA_ENVIO_EMAIL' ";
    $query = mysqli_query($connect, $SQL);
    $listaCasos = mysqli_fetch_all($query, MYSQLI_ASSOC);

    header('Content-Type: application/json');
    echo json_encode($listaCasos);
}

/**
 * @rota ROTA /api/v1/fakebatch
 * @global type $connect
 */
function fakeBatch()
{
    $dadosEntrada = apiTrataDadosEntrada();
    header('Content-Type: application/json');

    error_log("Dados Recebidos:" . var_export($dadosEntrada, true));
    echo json_encode($dadosEntrada);
}

/**
 * @rota ROTA /api/v1/getbatchlist
 * @global type $connect
 */
// <editor-fold defaultstate="collapsed" desc="Funcoes Acessorias">

function pegaLotes($batchId)
{
    global $connect;
    $SQL = "select caseNum from casedata, procdef, procfieldsdef where 
procdef.proccod = 'BATCHCLICKSIGN' and 
casedata.procid = procdef.procid and 
procfieldsdef.procid = procdef.procid and 
casedata.fieldid = procfieldsdef.fieldid and fieldCod = 'BATCHID' and
fieldValue = $batchId and casenum in 
(
select caseid from casequeue, stepdef, procdef where casequeue.procid = stepdef.procid and casequeue.procid = procdef.procid and procdef.proccod = 'BATCHCLICKSIGN' and stepdef.stepid = casequeue.stepid and stepdef.StepCod = 'CRIAR_LOTE_CLICKSIGN' 
) limit 1
";
    $query = mysqli_query($connect, $SQL);
    $casosLotes = mysqli_fetch_all($query, MYSQLI_ASSOC);
    return $casosLotes;
}

function pegaSignserLote($batchId)
{
    global $connect;
    $SQL = "
select fieldvalue from casedata, procdef, procfieldsdef where 
procdef.proccod = 'SIGNATARIOS_DOCUMENTO' and 
casedata.procid = procdef.procid and 
procfieldsdef.procid = procdef.procid and 
casedata.fieldid = procfieldsdef.fieldid and fieldCod = 'signer.key' and
fieldvalue <> '' and
casenum  in (
select casenum from casedata, procdef, procfieldsdef where 
procdef.proccod = 'SIGNATARIOS_DOCUMENTO' and 
casedata.procid = procdef.procid and 
procfieldsdef.procid = procdef.procid and 
casedata.fieldid = procfieldsdef.fieldid and fieldCod = 'DOCUMENTO' and
fieldValue = $batchId)";

    $query = mysqli_query($connect, $SQL);
    $signer_keys = mysqli_fetch_all($query, MYSQLI_NUM);

    return $signer_keys;
}

function pegaTextoEmail($caseNumBatch)
{
    global $connect;
    $SQL = "select fieldvalue as texto_email from casedata, procfieldsdef where casedata.procid = procfieldsdef.ProcId and casedata.FieldId = procfieldsdef.FieldId and 
 fieldcod = 'TEXTO_EMAIL' and casenum = $caseNumBatch";
    $query = mysqli_query($connect, $SQL);
    $textos = mysqli_fetch_all($query, MYSQLI_ASSOC);
    return $textos[0]["texto_email"];
}

function pegaListKeyDocs($caseNumBatch)
{
    global $connect;
    $SQL = "select fieldvalue as documentKey from casedata, procfieldsdef, procdef where 
casedata.procid = procfieldsdef.ProcId and 
casedata.FieldId = procfieldsdef.FieldId and 
procfieldsdef.ProcId = procdef.procid and
procdef.proccod = 'DOCUMENTOS_LOTE' and 
fieldcod = 'document.key' and fieldValue <> '' and "
            // " casenum in (select casenum from casedata, procdef where procdef.procid = casedata.procid and procdef.ProcCod = 'DOCUMENTOS_LOTE' and ( fieldvalue = 93236 or fieldvalue = 93237 or fieldvalue = 93238))";
            . " casenum in (select casenum from casedata, procdef where procdef.procid = casedata.procid and procdef.ProcCod = 'DOCUMENTOS_LOTE' and fieldvalue = $caseNumBatch )";
    $query = mysqli_query($connect, $SQL);
    $keys_docs = mysqli_fetch_all($query, MYSQLI_NUM);
    return $keys_docs;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Batch List V1">
function getBatchList_v1()
{
    global $connect;

    $SQL = "select casequeue.CaseId as caseNum, casequeue.procId from casequeue, procdef, stepdef where casequeue.procid = procdef.ProcId and procdef.ProcCod = 'LOTE_ASSINATURAS' and stepdef.ProcId = procdef.procid and stepdef.stepid = casequeue.stepid and StepCod = 'LOTES_GERADOS'";
    $query = mysqli_query($connect, $SQL);
    $listaLotes = mysqli_fetch_all($query, MYSQLI_ASSOC);
    if (mysqli_errno($connect)) {
        $dadosErro[] = "1 Erro: " . " $SQL";
    }

    $batchs = array();
    $dadosErro = array();
    $dadosLote = array();
    foreach ($listaLotes as $loteMaster) {

        $procId = $loteMaster["procId"];
        $SQL = "select * from internalconstants where procid = $procId";
        $query = mysqli_query($connect, $SQL);
        $listaConstantes = mysqli_fetch_all($query, MYSQLI_ASSOC);
        if (mysqli_errno($connect)) {
            $dadosErro[] = "2 Erro: " . mysqli_error($connect) . " $SQL";
        }

        foreach ($listaConstantes as $item) {
            $batch[$item["ConstCode"]] = $item["ConstValue"];
        }

        $caseNumLote = $loteMaster["caseNum"];

        $signer_keys = pegaSignserLote($caseNumLote);
        if (count($signer_keys) == 0) {
            $retorno["erro"] = "Não há Signatarios";
            header('Content-Type: application/json');
            echo json_encode($retorno);
            continue;
        }

        $casosLotes = pegaLotes($caseNumLote);

        foreach ($casosLotes as $lote) {
            $caseNumBatch = $lote["caseNum"];

            $textoEmail = pegaTextoEmail($caseNumBatch);
            $lote["email_text"] = $textoEmail;

            $keys_docs = pegaListKeyDocs($caseNumBatch);
            if (count($keys_docs) == 0) {
                $retorno["erro"] = $dadosErro;
                continue;
            }

            $lote["caseNumBatch"] = $caseNumLote;
            $lote["signer_keys"] = $signer_keys;
            $lote["document_keys"] = $keys_docs;
            $dadosLote[] = $lote;
        }
        $batch["caseNumBatch"] = $caseNumLote;
        $batch["lotes"] = $dadosLote;
        $batch["numLotes"] = count($dadosLote);
        $batchs[] = $batch;
    }

    $retorno["batchs"] = $batchs;
    $retorno["batchCount"] = count($batchs);
    $retorno["erro"] = $dadosErro;
    if (mysqli_errno($connect)) {
        $retorno["err"] = "Erro: " . mysqli_error($connect);
    }

    header('Content-Type: application/json');
    echo json_encode($retorno);
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Batch List V2">
function getBatchList_v2()
{
    global $connect;

    $SQL = "select casequeue.CaseId as caseNum, casequeue.procId from casequeue, procdef, stepdef where casequeue.procid = procdef.ProcId and procdef.ProcCod = 'LOTE_ASSINATURAS' and stepdef.ProcId = procdef.procid and stepdef.stepid = casequeue.stepid and StepCod = 'LOTES_GERADOS'";
    $query = mysqli_query($connect, $SQL);
    $listaLotes = mysqli_fetch_all($query, MYSQLI_ASSOC);
    if (mysqli_errno($connect)) {
        $dadosErro[] = "1 Erro: " . " $SQL";
    }

    $batchs = array();
    $dadosErro = array();
    $dadosLote = array();
    $sum_keys_docs = array();
    foreach ($listaLotes as $loteMaster) {

        $procId = $loteMaster["procId"];
        $SQL = "select * from internalconstants where procid = $procId";
        $query = mysqli_query($connect, $SQL);
        $listaConstantes = mysqli_fetch_all($query, MYSQLI_ASSOC);
        if (mysqli_errno($connect)) {
            $dadosErro[] = "2 Erro: " . mysqli_error($connect) . " $SQL";
        }

        foreach ($listaConstantes as $item) {
            $batch[$item["ConstCode"]] = $item["ConstValue"];
        }

        $caseNumLote = $loteMaster["caseNum"];

        $signer_keys = pegaSignserLote($caseNumLote);
        if (count($signer_keys) == 0) {
            continue;
        }

        $casosLotes = pegaLotes($caseNumLote);

        foreach ($casosLotes as $lote) {
            $caseNumBatch = $lote["caseNum"];
            $caseNumBatch = 93236;

            $textoEmail = pegaTextoEmail($caseNumBatch);
            $lote["email_text"] = $textoEmail;

            $keys_docs = pegaListKeyDocs($caseNumBatch);
            if (count($keys_docs) == 0) {
                continue;
            }

//            $lote["caseNumBatch"] = $caseNumLote;
//            $lote["signer_keys"] = $signer_keys;
//            $lote["document_keys"] = $keys_docs;
//            $dadosLote[] = $lote;
        }
        $lote["caseNumBatch"] = $caseNumLote;
        $lote["signer_keys"] = $signer_keys;
        $lote["document_keys"] = $sum_keys_docs;
        $dadosLote[] = $lote;


        $batch["caseNumBatch"] = $caseNumLote;
        $batch["lotes"] = $dadosLote;
        $batch["numLotes"] = count($dadosLote);

        $batchs[] = $batch;
    }

    $retorno["batchs"] = $batchs;
    $retorno["batchCount"] = count($batchs);
    $retorno["erro_sql"] = $dadosErro;
    if (mysqli_errno($connect)) {
        $retorno["err"] = "Erro: " . mysqli_error($connect);
    }

    header('Content-Type: application/json');
    echo json_encode($retorno);
}

// </editor-fold>

/**
 * @rota ROTA /api/v1/getbatchlistkeys
 * @global type $connect
 */
function getListKeysBatch_v1()
{
    global $connect;

    $dadosEntrada = apiTrataDadosEntrada();
    $caseNumBatch = $dadosEntrada["caseNumBatch"];

    $SQL = "select fieldvalue as documentKey from casedata, procfieldsdef, procdef where 
casedata.procid = procfieldsdef.ProcId and 
casedata.FieldId = procfieldsdef.FieldId and 
procfieldsdef.ProcId = procdef.procid and
procdef.proccod = 'DOCUMENTOS_LOTE' and 
fieldcod = 'document.key' and 
casenum in (select casenum from casedata, procdef where procdef.procid = casedata.procid and procdef.ProcCod = 'DOCUMENTOS_LOTE' and fieldvalue = $caseNumBatch)";

    $Query = mysqli_query($connect, $SQL);

    $dados = mysqli_fetch_all($Query, MYSQLI_NUM);

    $retorno["items"] = array_values($dados);
    $retorno["itemsCount"] = count($dados);
    if (mysqli_errno($connect)) {
        $retorno["err"] = "Erro: " . mysqli_error($connect);
    }

    header('Content-Type: application/json');
    echo json_encode($retorno);
}

function getListDocsBatch_clicksing()
{
    global $connect;

    $dadosEntrada = apiTrataDadosEntrada();

    $caseNumBatch = $dadosEntrada["caseNum"];
    $SQL = "select caseNum from casedata, procfieldsdef 
where 
casedata.fieldId = procfieldsdef.FieldId and 
procfieldsdef.procid = casedata.procid and procfieldsdef.FieldCod = 'BATCHID' and fieldvalue = $caseNumBatch";
    $query = mysqli_query($connect, $SQL);
    $linhas = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $dadosSaida["itens"] = $linhas;
    $dadosSaida["itensCount"] = count($linhas);

    header('Content-Type: application/json');
    echo json_encode($dadosSaida);
}

/**
 * ROTA /api/v1/getdocsbatch
 */
function getmaxdocsbatch_clicksign()
{
    global $connect;
    $SQL = "SELECT casedata.fieldValue, caseNum, casedata.procId, casedata.fieldId, fieldCod FROM procdef, casequeue, stepdef, casedata, procfieldsdef where casequeue.procid = stepdef.procid and casequeue.stepid = stepdef.stepid and stepdef.StepCod = 'BATCHSCREATE' and
casedata.procid = casequeue.procid and casedata.casenum = casequeue.caseid and procfieldsdef.procid = casedata.procid and casedata.FieldId = procfieldsdef.FieldId and procfieldsdef.FieldCod = 'MAXDOCBATCH' and procdef.procid = stepdef.procid and proccod = 'LOTE_ASSINATURAS' ";
    $query = mysqli_query($connect, $SQL);
    $linhas = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $itens = array();
    foreach ($linhas as $item) {
        $valorCampo["maxDocsBatch"] = intval($item["fieldValue"]);
        $valorCampo["caseNum"] = $item["caseNum"];
        $valorCampo["procId"] = $item["procId"];
        $itens[] = $valorCampo;
    }
    $dadosSaida["itens"] = $itens;
    $dadosSaida["itensCount"] = count($itens);

    header('Content-Type: application/json');
    echo json_encode($dadosSaida);
}

/**
 * ROTA /api/v1/getbatchsignqueue
 */
function getbatchqueue_clicksign()
{
    global $connect;
    // $SQL = "SELECT casedata.fieldValue, caseNum, casedata.procId, casedata.fieldId FROM casequeue, stepdef, casedata, procfieldsdef where casequeue.procid = stepdef.procid and casequeue.stepid = stepdef.stepid and stepdef.StepCod = 'CLICKSIGN_BATCH_CREATE' and casedata.procid = casequeue.procid and casedata.casenum = casequeue.caseid and procfieldsdef.procid = casedata.procid and casedata.FieldId = procfieldsdef.FieldId and procfieldsdef.FieldType = 'FL' limit 2";
    //$SQL = "SELECT casedata.fieldValue, caseNum, casedata.procId, casedata.fieldId FROM casequeue, stepdef, casedata, procfieldsdef where casequeue.procid = stepdef.procid and casequeue.stepid = stepdef.stepid and stepdef.StepCod = 'CLICKSIGN_BATCH_CREATE' and casedata.procid = casequeue.procid and casedata.casenum = casequeue.caseid and procfieldsdef.procid = casedata.procid and casedata.FieldId = procfieldsdef.FieldId and procfieldsdef.FieldCod = 'DIR_FILES' limit 2";
    $procId = PegaProcIdByCode('LOTE_ASSINATURAS');
    $stepId = PegaStepIdByCode($procId, "CLICKSIGN_BATCH_CREATE");
    $SQL = "select caseId as caseNum, procId from casequeue where casequeue.procid = $procId and stepid = $stepId limit 1";
    $query = mysqli_query($connect, $SQL);
    $linhas = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $itens = array();
    foreach ($linhas as $item) {

        $dirNameProc = str_pad($procId, 6, '0', STR_PAD_LEFT);
        $dirNameCaseNum = str_pad($item["caseNum"], 8, '0', STR_PAD_LEFT);

        $diretorioStorage = FILES_UPLOAD . "/" . $dirNameProc . "-" . $dirNameCaseNum;

        $valorCampo["fileNameStorage"] = $diretorioStorage;
        $valorCampo["caseNum"] = $item["caseNum"];
        $valorCampo["procId"] = $item["procId"];
        $itens[] = $valorCampo;
    }
    $dadosSaida["itens"] = $itens;
    $dadosSaida["itensCount"] = count($itens);

    header('Content-Type: application/json');
    echo json_encode($dadosSaida);
}

/**
 * @rota api/v1/callbackclicksign
 * @rota api/v1/webhook
 * @global type $connect
 */
function callback_clicksign()
{
    global $connect;

    $headerRecebidos = getallheaders();

    if (!isset($headerRecebidos["Content-Hmac"])) {
        header("HTTP/1.0 402");
        die();
    }

    // Eventos que não devem ser guardados no sistema
//    $excluirEventos = array();
//    $excluirEventos = array("cancel", "upload", "add_signer", "sign");
//    $excluirEventos = array();
    $excluirEventos = array("add_signer", "deadline");
    if (in_array($headerRecebidos["Event"], $excluirEventos)) {
        header("HTTP/1.0 200");
        die();
    }

    $dadosRecebidos = apiTrataDadosEntrada();

    $sourceHost = $headerRecebidos["Host"];
    $hookEvent = $headerRecebidos["Event"];
    $json_headers = json_encode($headerRecebidos);
    $json_post = json_encode($dadosRecebidos);

    $document_key = $dadosRecebidos["document"]["key"];

    $SQL = "insert into webhooks (hookdata, hookHeaders, hookSourceHost, hookStatus, hookEvent, document_key) values ('$json_post', '$json_headers', '$sourceHost', 0, '$hookEvent', '$document_key')";
    $retorno["operation"] = mysqli_query($connect, $SQL);

    $retorno["error"] = mysqli_error($connect);
//    $retorno["receivedHeaders"] = $headerRecebidos;
//    $retorno["receivedData"] = $dadosRecebidos;

    if (mysqli_error($connect)) {
        error_log("Erro webhook \n" . mysqli_error($connect));
        header("HTTP/1.0 500");
        return;
    }

    header('Content-Type: application/json');
    echo json_encode($retorno);
}

/**
 * @rota api/v1/hooksqueue
 */
function hook_queue()
{
    global $connect;

    $SQL = "select count(*) as total from webhooks where hookStatus = 0";
    $Query = mysqli_query($connect, $SQL);

    $resultado = mysqli_fetch_all($Query, MYSQLI_ASSOC);    
    $totalQueue = $resultado[0]["total"];
    
    
    $SQL = "select * from webhooks where hookStatus > -1 and hookStatus < 2 order by hookStatus limit 50 ";
    $Query = mysqli_query($connect, $SQL);

    $dadosRetornados = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $listHooks = array();
    foreach ($dadosRetornados as $dados) {
        $item["hookId"] = $dados["hookId"];
        $item["hookEvent"] = $dados["hookEvent"];
        $item["hookStatus"] = $dados["hookStatus"];
        $item["document_key"] = $dados["document_key"];
        $item["hookDateTime"] = $dados["hookDateTime"];
        $item["data"] = json_decode($dados["hookdata"]);
        $item["headers"] = json_decode($dados["hookHeaders"]);
        $listHooks[] = $item;
    }
    $retorno["numRec"] = count($dadosRetornados);
    $retorno["numQueued"] = $totalQueue;
    $retorno["itens"] = $listHooks;

    header('Content-Type: application/json');
    echo json_encode($retorno);
}

/**
 * @rota api/v1/hooksetstatus
 * 
 * @global type $connect
 */
function hook_set_status()
{
    global $connect;
    $dadosRecebidos = apiTrataDadosEntrada();

    $hookId = $dadosRecebidos["hookId"];
    $hookStatus = $dadosRecebidos["hookStatus"];

    $SQL = "update webhooks set hookStatus = $hookStatus where hookId = $hookId";
    mysqli_query($connect, $SQL);

    if (mysqli_errno($connect)) {
        $retorno["erro"] = mysqli_error($connect);
        $retorno["SQL"] = $SQL;
    } else {
        $retorno["result"] = 'ok';
    }

    header('Content-Type: application/json');
    echo json_encode($retorno);
}

/**
 *  @rota api/hookgetdata
 */
function hook_get_data()
{
    global $connect;
    $dadosRecebidos = apiTrataDadosEntrada();

    $SQL = "select * from webhooks where ";
    if (key_exists("document_key", $dadosRecebidos)) {
        $SQL .= " document_key = '{$dadosRecebidos["document_key"]}'";
        $concatenador = " and ";
    }
    if (key_exists("hookstatus", $dadosRecebidos)) {
        $SQL .= " $concatenador hookstatus = '{$dadosRecebidos["hookstatus"]}'";
    }
    if (key_exists("hookStatus", $dadosRecebidos)) {
        $SQL .= " $concatenador hookstatus = '{$dadosRecebidos["hookStatus"]}'";
    }
    if (key_exists("hookdata", $dadosRecebidos)) {
        $SQL .= " hookdata like '%{$dadosRecebidos["document_key"]}%'";
        $concatenador = " and ";
    }


    $limite = 10;
    if (key_exists("limit", $dadosRecebidos)) {
        $limite = $dadosRecebidos["limit"];
    }
    $SQL .= " limit $limite";
    $query = mysqli_query($connect, $SQL);
    $dadosResultado = mysqli_fetch_all($query, MYSQLI_ASSOC);
    if (mysqli_errno($connect)) {
        header("HTTP/1.0 202 Accepted");
        $retorno["erro"] = mysqli_error($connect);
        $retorno["SQL"] = $SQL;
    } else {
        $retorno["SQL"] = $SQL;
        $retorno["dados"] = $dadosResultado;
    }

    header('Content-Type: application/json');
    echo json_encode($retorno);
}
