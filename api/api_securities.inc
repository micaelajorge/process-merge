<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 24/07/2019
  Sistema: Process_XAMPP
 */
// <editor-fold defaultstate="collapsed" desc="Inclusões do script">
require_once(FILES_ROOT . 'includes_ws/funcsws.php');
require_once(FILES_ROOT . "include/resource01.php");
require_once(FILES_ROOT . "includes_ws/wsuser.inc");
require_once(FILES_ROOT . "includes_ws/wscasestart.inc");
require_once(FILES_ROOT . "includes_ws/wscasesave.inc");
require_once(FILES_ROOT . "includes_ws/wserror.inc");

// </editor-fold>
/// <editor-fold defaultstate="collapsed" desc="Trata CCBS">
/**
 * Rota: api/v1/changecontract
 */
function api_change_contract()
{
    $dadosEntrada = apiTrataDadosEntrada();

//    error_log("Dados Alteração CCB \n" . var_export($dadosEntrada, true));

    $caseNum = $dadosEntrada["caseNum"];

    $userData["UserName"] = "mmoscz";
    $userData["UserPassword"] = "cerberus";

    foreach ($dadosEntrada as $chave => $valor) {
        $campo["fieldCode"] = strtoupper($chave);
        $campo["fieldValue"] = $valor;
        $camposEnvio[] = $campo;
    }

    $dadosCaso["Fields"] = $camposEnvio;
    $caseNum = FuncCaseSave($userData, "SEC_CCBS", "START", $caseNum, $dadosCaso, 0, 0);

    $jDados = json_encode($caseNum);
    header('Content-Type: application/json');
    echo $jDados;
}

/**
 * Rota: api/v1/createcontract
 */
function api_create_contract()
{
    $dadosEntrada = apiTrataDadosEntrada();

//    error_log('Create Contract: ' .var_export($dadosEntrada, true));

//    $userData["UserName"] = "mmoscz";
//    $userData["UserPassword"] = "cerberus";

    $nrContrato = $dadosEntrada["CONTRATO"];

    // Verfica se já existe a CCB
    $caseNum = pegaNumerCasoUnicoAberto("SEC_CCBS", $nrContrato, "CONTRATO", "CCB_CEDIDA");

    if ($caseNum == 0) {

        $caseNum = 0;
        foreach ($dadosEntrada as $chave => $valor) {
            // error_log("Chave: $chave Valor: $valor");
            switch (strtoupper($chave)) {
                case "CONTRATO":
                    // Cria o campo de Nr Contrato fiel ao que veio do Template                
                    $campo["fieldCode"] = "NR_CONTRATO_TEMPLATE";
                    $campo["fieldValue"] = $valor;
                    $camposEnvio[] = $campo;

                    // Remove a formatação do Campo
                    $valor = preg_replace("/\W+/", "", $valor);
//                    $valor = preg_replace("/[^a-zA-Z0-9]/", $valor);                    
                    break;
            }
            $campo["fieldCode"] = strtoupper($chave);
            $campo["fieldValue"] = $valor;
            $camposEnvio[] = $campo;
        }

        // Operacoes com campos
        $dadosCaso["Fields"] = $camposEnvio;
        $caseNum = FuncCaseSave($userData, "SEC_CCBS", "START", $caseNum, $dadosCaso, 0, 1);
    }

    $jDados = json_encode($caseNum);
    header('Content-Type: application/json');
    echo $jDados;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="api_trataWebHook">

/**
 * 
 * @param type $dadosCCB
 * @return type
 */
function criaCCB($dadosCCB)
{
    // Pega dados das Entidades
    $procIdEntidades = PegaProcIdByCode("SEC_SIGNOR_TRANSFEREE");

    $ACCOUNT_KEY = $dadosCCB["data"]["document"]["account_key"];
    $fieldId = PegaFieldIdByCode($procIdEntidades, "ACCOUNT_KEY");

    $caseNumEntidade = pegaNumerCasoUnicoAberto($procIdEntidades, $ACCOUNT_KEY, $fieldId);
    $dadosEntidade = pegaDadosCaso($procIdEntidades, $caseNumEntidade);

//     Se não tem CESSION_DOCUMENT_KEY é criação de CCB
    if ($dadosEntidade["CESSION_DOCUMENT_KEY"] !== "") {
        header("HTTP/1.0 406 Accontkey not found, $ACCOUNT_KEY");
        return;
    }

    $userData["UserName"] = "mmoscz";
    $userData["UserPassword"] = "cerberus";

    $fieldId = PegaFieldIdByCode("SEC_CCBS", "DOCUMENT_KEY");


    $documentKey = $dadosCCB["data"]["document"]["key"];
    $caseNumCCB = pegaNumerCasoUnicoAberto("SEC_CCBS", $documentKey, $fieldId);

    if ($caseNumCCB == 0) {

        if (key_exists("issuer_cpf", $dadosCCB["data"]["document"]["template"]["data"])) {
            $dadosTemplate = $dadosCCB["data"]["document"]["template"]["data"];
        } else {
            $dadosTemplate = $dadosCCB["data"]["document"]["template"]["data"]["document"]["template"]["data"];
        }

        // Mudado para remover apenas PONTUAÇÃO e LETRAS
//        $campo["fieldCode"] = "CONTRATO";
////        $campo["fieldValue"] = preg_replace("/\D+/", "", $dadosTemplate["number"]);
////        $campo["fieldValue"] = preg_replace("/[^a-zA-Z0-9]/", "", $dadosTemplate["number"]);
//        $campo["fieldValue"] = $dadosTemplate["number"];
////        $campo["fieldValue"] = $dadosTemplate["number"];;
//        $camposEnvio[] = $campo;

        $campo["fieldCode"] = "CONTRATO_2";
        $campo["fieldValue"] = $dadosTemplate["number"];
        $camposEnvio[] = $campo;

        $campo["fieldCode"] = "DOCUMENT_KEY";
        $campo["fieldValue"] = $dadosCCB["data"]["document"]["key"];
        $camposEnvio[] = $campo;

        $campo["fieldCode"] = "ORIGINADOR";
        $campo["fieldValue"] = $dadosCCB["data"]["document"]["account_key"];
        $camposEnvio[] = $campo;

        $campo["fieldCode"] = "ACCOUNT_KEY";
        $campo["fieldValue"] = $dadosCCB["data"]["document"]["account_key"];
        $camposEnvio[] = $campo;

        $campo["fieldCode"] = "CPF";
        $campo["fieldValue"] = $dadosTemplate["issuer_cpf"];
        $camposEnvio[] = $campo;

        $campo["fieldCode"] = "NR_PARCELAS";
        $campo["fieldValue"] = $dadosTemplate["maturity"];
        $camposEnvio[] = $campo;

        $campo["fieldCode"] = "NR_CONTRATO_TEMPLATE";
        $campo["fieldValue"] = $dadosTemplate["number"];

        $campo["fieldCode"] = "CONTRATO";
        $campo["fieldValue"] = $dadosTemplate["number"];
        $camposEnvio[] = $campo;       
        
        $dadosCaso["Fields"] = $camposEnvio;        
        
        return FuncCaseSave($userData, "SEC_CCBS", 'START', 0, $dadosCaso, 0, 1);
    } else {
        return $caseNumCCB;
    }
}

/**
 * 
 * @param type $dadosCCB
 */
function atualizaCCB($dadosCCB)
{
    $userData["UserName"] = "mmoscz";
    $userData["UserPassword"] = "cerberus";

    $fieldId = PegaFieldIdByCode("SEC_CCBS", "DOCUMENT_KEY");

    $documentKey = $dadosCCB["document_key"];
    $caseNumCCB = pegaNumerCasoUnicoAberto("SEC_CCBS", $documentKey, $fieldId);

    $camposEnvio = array();
//    error_log("document_key $documentKey");

    if ($caseNumCCB != 0) {
        $campo["fieldCode"] = "STATUS_CLICKSIGN";
        $campo["fieldValue"] = $dadosCCB["hookEvent"];
        $camposEnvio[] = $campo;

//        error_log("camposEnvio " . var_export($camposEnvio, true));

        $dadosCaso["Fields"] = $camposEnvio;

        FuncCaseSave($userData, "SEC_CCBS", 'START', $caseNumCCB, $dadosCaso, 0, 0);
    }
    return $caseNumCCB;
}

// <editor-fold defaultstate="collapsed" desc="Fila cessoes resolucoes processadas">
/**
 * ROTA: api/v1/processedcessionsqueue
 */
function api_processedQueueCesssions()
{
    processedCessionsResolutions("SEC_CESSOES");
}

/**
 * ROTA: api/v1/processedresolutionsqueue
 */
function api_processedQueueResolutions()
{
    processedCessionsResolutions("SEC_RESOLUCOES");
}

/**
 * 
 * @global type $connect
 * @param type $procCod
 */
function processedCessionsResolutions($procCod)
{
    global $connect;
    $procId = PegaProcIdByCode($procCod);
    $campoStatusQueue = PegaFieldIdByCode($procId, "CESSION_STATUS");

    $campoStatusAssinatura = PegaFieldIdByCode($procId, "STATUS_CLICKSIGN");
    $cessionUuid = PegaFieldIdByCode($procId, "CESSION_UUID");

    $sql = "select Campo$cessionUuid as cession_id, Campo$campoStatusQueue as statusCession from exportkeys where procid = $procId and Campo$campoStatusQueue in ('processed') and Campo$campoStatusAssinatura = 'auto_close'";
//    $sql .= " limit 1";
    $query = mysqli_query($connect, $sql);
    $listaCessoes = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $jDados = json_encode($listaCessoes);
    header('Content-Type: application/json');
    echo $jDados;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Atualiza Status Cessao">
/**
 * 
 * @param type $dadosTermo
 * @param type $tipoDocumento
 * @return type
 */
function atualizaStatusCessao($dadosTermo, $tipoDocumento)
{
    $userData["UserName"] = "mmoscz";
    $userData["UserPassword"] = "cerberus";

    $fieldId = PegaFieldIdByCode($tipoDocumento, "TERMO_CESSAO");

    $documentKey = $dadosTermo["document_key"];
    $caseNumCCB = pegaNumerCasoUnicoAberto($tipoDocumento, $documentKey, $fieldId);

    if ($caseNumCCB != 0) {
        $campo["fieldCode"] = "STATUS_CLICKSIGN";
        $campo["fieldValue"] = $dadosTermo["hookEvent"];
        $camposEnvio[] = $campo;
        $dadosCaso["Fields"] = $camposEnvio;

        FuncCaseSave($userData, $tipoDocumento, 'SESSIONS', $caseNumCCB, $dadosCaso, 0, 0);
    }
    return $caseNumCCB;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Atualiza Status Cessao">
/**
 * ROTA: api/v1/docsigned
 * 
 * @return type
 */
function atualizaAssinaturaCessao()
{
    $dadosEntrada = apiTrataDadosEntrada();

    $userData = "";

    $procId = PegaProcIdByCode("SEC_CESSOES");

    $fieldId = PegaFieldIdByCode($procId, "TERMO_CESSAO");

    $documentKey = $dadosEntrada["document_key"];
    $caseNumCCB = pegaNumerCasoUnicoAberto($procId, $documentKey, $fieldId);

    if ($caseNumCCB != 0) {
        $campo["fieldCode"] = "ASSINANTES_TERMO";
        $campo["fieldValue"] = $dadosEntrada["signer"];
        $camposEnvio[] = $campo;
        $campo["fieldCode"] = "CESSION_POSTS";
        $campo["fieldValue"] = "Assinante: " . $dadosEntrada["name"] . " assinou o termo";
        $camposEnvio[] = $campo;
        $dadosCaso["Fields"] = $camposEnvio;

        FuncCaseSave($userData, $procId, 'SESSIONS', $caseNumCCB, $dadosCaso, 0, 0);
    }
    return $caseNumCCB;
}

// </editor-fold>
function atualizaStatusDocumentos($dadosDocumento)
{
    $caseNumCCB = atualizaCCB($dadosDocumento);
    if ($caseNumCCB == 0) {
        $caseNumCCB = atualizaStatusCessao($dadosDocumento, 'SEC_CESSOES');
        if ($caseNumCCB == 0) {
            $caseNumCCB = atualizaStatusCessao($dadosDocumento, 'SEC_RESOLUCOES');
        }
    }
    return $caseNumCCB;
}

/**
 * 
 * @param type $dadosCCB
 */
function trataCCB($dadosCCB)
{
    $retorno = "sem acao";
    switch ($dadosCCB["hookEvent"]) {
        //switch ($dadosCCB["data"]["event"]["name"]) {
        case "upload":
            $retornoCriacao = criaCCB($dadosCCB);
            $retorno = "upload $retornoCriacao";
            break;
        case "auto_close":
            $dadosRetorno = atualizaStatusDocumentos($dadosCCB);
            $retorno = "$dadosRetorno | auto_close";
            break;
        case "cancel":
            $retorno = "closed";
            break;
    }
    return json_encode($retorno);
}

/**
 * ROTA: process POST
 */
function api_trataWebHook()
{
    try {
        $dadosEntrada = apiTrataDadosEntrada();
        $retorno = trataCCB($dadosEntrada);
        $jDados = json_encode($retorno);
    } catch (Exception $ex) {
        $jDados = json_encode($ex);
    }
    header("Access-Control-Allow-Origin: *");
    header('Content-Type: application/json');
    echo $jDados;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="api_cessionqueue">
/**
 * ROTA: cessionsqueue
 * ROTA: api/v1/cessionsqueue
 */
function api_cessionqueue()
{
    get_cession_resolution_queue("SEC_CESSOES");
}

/**
 * 
 */
function api_resolutionqueue()
{
    get_cession_resolution_queue("SEC_RESOLUCOES");
}

/**
 * 
 * @global type $connect
 * @param type $procCod
 */
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

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="api_process_create">
/**
 * ROTA: api/v1/process POST
 */
function api_process_create()
{
    $procCod = 'SEC_CESSOES';
    processa_cessao_resolucao($procCod);
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="api_resolution_create">
/**
 * ROTA: api/v1/resolution POST
 */
function api_resolution_create()
{
    $procCod = 'SEC_RESOLUCOES';
    processa_cessao_resolucao($procCod);
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Processar cessoes / resolucoes">




function validaDadosEntrada($dadosRecebidos, $listaCamposObrigatorios, $camposComAlias = array())
{

    $keysEntrada = array_map("strtoupper", array_keys($dadosRecebidos));

    $temAlgum = count($camposComAlias) == 0;
    foreach ($camposComAlias as $chave) {
        $temAlgum = $temAlgum || in_array($chave, $keysEntrada);
    }

    $retorno = true && $temAlgum;

    foreach ($listaCamposObrigatorios as $chave) {
        $chaveExiste = in_array($chave, $keysEntrada);
        $retorno = $retorno && $chaveExiste;
    }
    return true;
}

function processa_cessao_resolucao($procCod)
{
    try {
        $dadosEntrada = apiTrataDadosEntrada();

        $chavesRecebidas = array_keys($dadosEntrada);

        $camposObrigatoriosCessao = ["CESSION_NUMBER"];

        $dadosProcessoValido = validaDadosEntrada($dadosEntrada, $camposObrigatoriosCessao);
        if (!$dadosProcessoValido) {
            header("HTTP/1.0 400 Dados Insuficientes");
            return;
        }

        $camposEnvio = array();
        foreach ($chavesRecebidas as $chave) {
            // Salta o valor se for um array
            if (is_array($dadosEntrada[$chave])) {
                continue;
            }
            $fieldValue = $dadosEntrada[$chave];
            if (!is_string($fieldValue)) {
                $fieldValue = (string) $fieldValue;
            }
            $campo["fieldCode"] = strtoupper($chave);
            $campo["fieldValue"] = $fieldValue;
            $camposEnvio[] = $campo;
        }

        // Cria o campo unico da CESSAO = NUMERO CESSAO + BUCKET ORIGEM + BUCKET DESTINO
        $campo["fieldCode"] = "IDENT_CESSAO";
        $campo["fieldValue"] = $dadosEntrada["cession_number"] . $dadosEntrada["assignor"] . $dadosEntrada["transferee"];
        $camposEnvio[] = $campo;

        // Cria o campo unico da CESSAO = NUMERO CESSAO + BUCKET ORIGEM + BUCKET DESTINO
        $sessionUUID = get_guid();

        $campo["fieldCode"] = "CESSION_UUID";
        $campo["fieldValue"] = $sessionUUID;
        $camposEnvio[] = $campo;

        if (key_exists("contracts", $dadosEntrada)) {

            // Valida os dados de todos os Contratos
            $camposObrigatoriosContrato = ["ISSUER_CPF", "NUMBER"];
            $camposExistentes = ["PARCELS", "NR_PARCELS", "NR_PARCELAS"];

            foreach ($dadosEntrada["contracts"] as $contrato) {
                $dadosProcessoValido = validaDadosEntrada($contrato, $camposObrigatoriosContrato, $camposExistentes);
                if (!$dadosProcessoValido) {
                    header("HTTP/1.0 400 Dados Insuficientes");
                    return;
                }
            }

            // Array com os campos a serem atualizados
            $cessionContracts = $dadosEntrada["contracts"];
            $campo["fieldCode"] = "CESSION_CONTRACTS_DATA";
            $campo["fieldValue"] = json_encode($cessionContracts);
            $camposEnvio[] = $campo;

            $cessionContractsNumbers = array_column($cessionContracts, "number");
            $campo["fieldCode"] = "CESSION_CONTRACTS";
            $campo["fieldValue"] = json_encode($cessionContractsNumbers);
            $camposEnvio[] = $campo;

            // Muda o status da CESSAO
            $campo["fieldCode"] = "CESSION_STATUS";
            $campo["fieldValue"] = "queued";
            $camposEnvio[] = $campo;
        } else {
            // Muda o status da CESSAO
            $campo["fieldCode"] = "CESSION_STATUS";
            $campo["fieldValue"] = "created";
            $camposEnvio[] = $campo;
        }

        $dadosCaso["Fields"] = $camposEnvio;

        // Pega o ProcId do Caso
        $procId = PegaProcIdByCode($procCod);

        // Busca o Campo UNIQUE
        $camposUniqueClose = PegaCampoUniqueClose($procId);

        // Pega o campo unique do Caso
        $fieldUnique = $camposUniqueClose["FieldUnique"];

        // Verfifica o valor Enviado do campo unique
        $valorCampoUnique = buscaValorCampoUnique($procId, $camposEnvio, $fieldUnique);


        // Verfifica se é caso unico
        $numeroCasoExiste = pegaNumerCasoUnicoAberto($procId, $valorCampoUnique);

//        $userData["UserName"] = "mmoscz";
//        $userData["UserPassword"] = "cerberus";

        if ($numeroCasoExiste == 0) {
            $numeroCaso = 0;
            $caseNum = FuncCaseSave($userData, $procId, 'START', $numeroCaso, $dadosCaso, 0, 1);
            if ($caseNum === 0) {
                header("HTTP/1.0 201 Created");
                $novoCaseNum["process"] = $sessionUUID;
                $novoCaseNum["error"] = array();
            }
        } else {
            header("HTTP/1.0 402 Falha");
            $novoCaseNum["process"] = "";
            $novoCaseNum["Error"] = "Cessão já Existe para essa ORIGEM / DESTINO";
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

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="api_process_get">
/**
 * ROTA: process
 */
function api_process_get()
{
    global $cessionId, $connect;

    $cessionId = $cessionId;

    error_log("Pegando dados de Cessão: $cessionId");

    
    $procId = PegaProcIdByCode("SEC_CESSOES");
    $fieldCessionUUID = PegaFieldIdByCode($procId, 'CESSION_UUID');

    $sql = "select casenum from exportkeys where procid = $procId and Campo$fieldCessionUUID = '$cessionId'";
    $query = mysqli_query($connect, $sql);

    $dados = mysqli_fetch_all($query, MYSQLI_ASSOC);

    if (count($dados) > 0) {
        $caseNum = $dados[0]["casenum"];
    }

    // Campos para serem retornados
    $camposRetorno = array("cession_number", "cession_value", "cession_parcels", "cession_contracts", "assignor", "transferee", "cession_status");

    $sql = "select fieldcod, fieldvalue from casedata, procfieldsdef where casenum = $caseNum and procfieldsdef.procid = casedata.procid and casedata.procid = $procId and procfieldsdef.fieldid = casedata.fieldid";
    $query = mysqli_query($connect, $sql);

    $dadosCampos = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $dadosRetorno = array();
    foreach ($dadosCampos as $dadoCampo) {
        if (in_array(strtolower($dadoCampo["fieldcod"]), $camposRetorno)) {
            $dadosRetorno[strtolower($dadoCampo["fieldcod"])] = $dadoCampo["fieldvalue"];
        }
        if ($dadoCampo["fieldcod"] == "CESSION_UUID") {
            $dadosRetorno["process"] = $dadoCampo["fieldvalue"];
        }
    }

    $contratosCessao = $dadosRetorno["cession_contracts"];
    if ($contratosCessao != "") {
        $dadosRetorno["cession_contracts"] = json_decode($dadosRetorno["cession_contracts"]);
    }

    
    $jDados = json_encode($dadosRetorno);
    header("Access-Control-Allow-Origin: *");
    header('Content-Type: application/json');
    echo $jDados;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="api_contracts_put">
/**
 * ROTA: contracts POST
 *
 * @global type $connect
 * @return int
 */
function api_contracts_put()
{
    global $connect;
    $dadosEntrada = apiTrataDadosEntrada();
    $cessionUUID = $dadosEntrada["process"];

    $cessionContracts = $dadosEntrada["contracts"];

    $cessionContractsNumbers = array_column($cessionContracts, "number");

    $userData["UserName"] = "mmoscz";
    $userData["UserPassword"] = "cerberus";

    // Pega o ProcId do Processo
    $procId = PegaProcIdByCode('SEC_CESSOES');

    $fieldCessionUUID = PegaFieldIdByCode($procId, 'CESSION_UUID');
    $sql = "select casenum from exportkeys where Campo$fieldCessionUUID = '$cessionUUID'";
    $query = mysqli_query($connect, $sql);

    $dadosCaso = mysqli_fetch_all($query, MYSQLI_ASSOC);

    if (count($dadosCaso) == 0) {
        return 0;
    }

    $numeroCaso = $dadosCaso[0]["casenum"];

    // Array com os campos a serem atualizados
    $camposEnvio = array();
    $campo["fieldCode"] = "CESSION_CONTRACTS_DATA";
    $campo["fieldValue"] = json_encode($cessionContracts);
    $camposEnvio[] = $campo;

    $campo["fieldCode"] = "CESSION_CONTRACTS";
    $campo["fieldValue"] = json_encode($cessionContractsNumbers);
    $camposEnvio[] = $campo;

    // Muda o status da CESSAO
    $campo["fieldCode"] = "CESSION_STATUS";
    $campo["fieldValue"] = "queued";
    $camposEnvio[] = $campo;

    $dadosCaso["Fields"] = $camposEnvio;
    $numeroCaso = FuncCaseSave($userData, $procId, 'SESSIONS', $numeroCaso, $dadosCaso, 0, 0);

    $dadosRetorno = array();
    $dadosRetorno = $numeroCaso;

    $jDados = json_encode($dadosRetorno);
    header("Access-Control-Allow-Origin: *");
    header('Content-Type: application/json');
    header("HTTP/1.0 202 Accepted");
    echo $jDados;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="api_contrats_get">
/**
 * ROTA: contracts GET
 */
function api_contracts_get()
{
    
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="api_get_cession_resolution_data">
/**
 *  ROTA api/v1/cession
 */
function api_get_cession_data()
{
    api_get_cession_resolution_data('SEC_CESSOES');
}

/**
 * ROTA: api/v1/resolution
 */
function api_get_resolution_data()
{
    api_get_cession_resolution_data('SEC_RESOLUCOES');
}

/**
 * 
 * @global type $cessionUUID
 * @param type $procCode
 */
function api_get_cession_resolution_data($procCode)
{
    global $cessionUUID;

    $dadosRetorno = array();

    // Pega Dados da Cessao
    $procId = PegaProcIdByCode($procCode);
    $caseNumCession = pegaNumerCasoUnicoAberto($procId, $cessionUUID, "CESSION_UUID");

    error_log("Pegando dados cessão: $cessionUUID");
    if ($caseNumCession == 0) {
        $jDados = json_encode($dadosRetorno);
        header("HTTP/1.0 400 falha - $procCode - $cessionUUID");
        header('Content-Type: application/json');
        echo $jDados;
        return;
    }
    $dadosRetorno = pegaDadosCaso($procId, $caseNumCession);

    // Pega ProcId das Entidades
    $procIdEntidades = PegaProcIdByCode("SEC_SIGNOR_TRANSFEREE");

    // Pega Dados da Entidade Recebedora
    // Remove a formatação
    $cnpj = preg_replace("/\D+/", "", $dadosRetorno["TRANSFEREE"]);
    $caseNumEntidade = pegaNumerCasoUnicoAberto($procIdEntidades, $cnpj, "CNPJ");
    $dadosRetorno["TRANSFEREE_DATA"] = array();
    $dadosRetorno["TRANSFEREE_DATA"]["ASSIGNERS"] = array();

    if ($caseNumEntidade) {
        $dadosRetorno["TRANSFEREE_DATA"] = pegaDadosCaso($procIdEntidades, $caseNumEntidade);
        $dadosRetorno["TRANSFEREE_DATA"]["ASSIGNERS"] = pegaListaTransferee($dadosRetorno["TRANSFEREE_DATA"]["ACCOUNT_KEY"], $procCode);
    }

    // Pega Dados da Entidade Endossador
    // Remove a formatação
    $cnpj = preg_replace("/\D+/", "", $dadosRetorno["ASSIGNOR"]);
    $caseNumEntidade = pegaNumerCasoUnicoAberto($procIdEntidades, $cnpj, "CNPJ");
    $dadosRetorno["ASSIGNOR_DATA"] = array();
    $dadosRetorno["ASSIGNOR_DATA"]["ASSIGNERS"] = array();

    if ($caseNumEntidade) {
        $dadosRetorno["ASSIGNOR_DATA"] = pegaDadosCaso($procIdEntidades, $caseNumEntidade);

        // Trata os TOKEN das contas
        $tokens = $dadosRetorno["ASSIGNOR_DATA"]["ACCOUNT_TOKEN"];
        $listaTokens = json_decode($tokens);
        if (!is_array($listaTokens)) {
            $listaTokens = array();
            $accountKey = $dadosRetorno["ASSIGNOR_DATA"]["ACCOUNT_KEY"];
            $itemTokens["ACCOUNT_KEY"] = $accountKey;
            $itemTokens["ACCOUNT_TOKEN"] = $tokens;            
            $listaTokens[] = $itemTokens;            
        }
        $dadosRetorno["ASSIGNOR_DATA"]["ACCOUNT_TOKEN"] = $listaTokens;
        $dadosRetorno["ASSIGNOR_DATA"]["ASSIGNERS"] = pegaListaAssigners($dadosRetorno["ASSIGNOR_DATA"]["ACCOUNT_KEY"], $procCode);
    }

    switch ($procCode) {
        case "SEC_RESOLUCOES":
            // Define o proprietario do BUCKET onde está a CCB - FIDC
            $dadosRetorno["CCB_ACCOUNT_KEY"] = $dadosRetorno["TRANSFEREE_DATA"]["ACCOUNT_KEY"];

            // Seta o Tipo de modelo para ser gerardo
            $dadosRetorno["TRANSFEREE_DATA"]["CESSION_DOCUMENT_KEY"] = $dadosRetorno["TRANSFEREE_DATA"]["RESOLUCAO_DOCUMENT_KEY"];
            unset($dadosRetorno["TRANSFEREE_DATA"]["RESOLUCAO_DOCUMENT_KEY"]);
            break;
        case "SEC_CESSOS":
            // Define o proprietario do BUCKET onde está a CCB - ORIGINADOR
            $dadosRetorno["CCB_ACCOUNT_KEY"] = $dadosRetorno["ASSIGNOR_DATA"]["ACCOUNT_KEY"];
            break;
    }
    $dadosCaso = array();

    // Adiciona o Nurmero do Caso
    $campoCodigo = "caseNum";
    $campoValor = $caseNumCession;
    $dadosCaso[$campoCodigo] = $campoValor;

    // Adiciona o id do Processo
    $campoCodigo = "procId";
    $campoValor = $procId;
    $dadosCaso[$campoCodigo] = $campoValor;

    // Adiciona o id da Fila
    $campoCodigo = "stepId";
    $campoValor = PegaStepIdByCode($procId, "SESSIONS");
    $dadosCaso[$campoCodigo] = $campoValor;

    $dadosRetorno["caseInfo"] = $dadosCaso;

    $jDados = json_encode($dadosRetorno);
    header('Content-Type: application/json');
    echo $jDados;
}

function pegaListaTransferee($account_key, $procCode)
{
    $procId = PegaProcIdByCode("SEC_SIGNATARIOS");
    $fieldId = PegaFieldIdByCode($procId, "BUCKET_DESTINO");
    return pegaDadosAssinantes($procId, $fieldId, $account_key, $procCode);
}

/**
 * 
 * @param type $account_key
 * @param type $procCode
 * @return type
 */
function pegaListaAssigners($account_key, $procCode)
{
    $procId = PegaProcIdByCode("SEC_SIGNATARIOS");
    $fieldId = PegaFieldIdByCode($procId, "BUCKET_ORIGEM");
    return pegaDadosAssinantes($procId, $fieldId, $account_key, $procCode);
}

/**
 * 
 * @global type $connect
 * @param type $procId
 * @param type $fieldId
 * @param type $account_key
 * @param type $procCode
 * @return type
 */
function pegaDadosAssinantes($procId, $fieldId, $account_key, $procCode)
{
    global $connect;

    $assinantes = array();
    $sql = "select casenum from exportkeys where procid = $procId and Campo$fieldId = '$account_key'";
    $query = mysqli_query($connect, $sql);
    $listaCasosAssinantes = mysqli_fetch_all($query, MYSQLI_ASSOC);

    foreach ($listaCasosAssinantes as $caso) {
        $dadoAssinante = pegaDadosCaso($procId, $caso["casenum"]);
        switch ($procCode) {
            case "SEC_RESOLUCOES":
                $dadoAssinante["TIPO_ASSINATURA_TERMO"] = $dadoAssinante["TIPO_ASSINATURA_RESOLUCAO"];
                unset($dadoAssinante["TIPO_ASSINATURA_RESOLUCAO"]);
                break;
        }
        if ($dadoAssinante["ASSINANTE_ATIVO"] === "1") {
            $assinantes[] = $dadoAssinante;
        }
    }
    return $assinantes;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="validacao do contrato">
/**
 * ROTA: validacontrato
 */
function api_valida_contrato()
{
    global $connect;

    $dadosEntrada = apiTrataDadosEntrada();

    $dadosRetorno = array();

    $procId = PegaProcIdByCode("SEC_CCBS");

//    $ccbNumero = preg_replace("/\D+/", "", $dadosEntrada["contrato"]["number"]);
    $ccbNumero = preg_replace("/[^a-zA-Z0-9]/", "", $dadosEntrada["contrato"]["number"]);

    // <editor-fold defaultstate="collapsed" desc="Busca dados do caso da CCB">
    $caseNumCCB = pegaNumerCasoUnicoAberto($procId, $ccbNumero);

    $SQL = "select fieldCod, fieldValue from casedata, procfieldsdef where caseNum = $caseNumCCB  and casedata.procid = $procId and procfieldsdef.procid = casedata.procid and procfieldsdef.fieldid = casedata.fieldid";
    $query = mysqli_query($connect, $SQL);

    $valoresCampos = mysqli_fetch_all($query, MYSQLI_ASSOC);

    if (count($valoresCampos) == 0) {
        $dadosRetorno["message"] = "Contrato não encontrado";
        $dadosRetorno["contractNumber"] = $dadosEntrada["contrato"]["number"];
        $dadosRetorno["resultadoAvaliacao"] = false;
        $jDados = json_encode($dadosRetorno);
        header('Content-Type: application/json');
        echo $jDados;
        return;
    }

    $dadosCCB = array();
    foreach ($valoresCampos as $valorCampo) {
        $campoCodigo = $valorCampo["fieldCod"];
        $campoValor = $valorCampo["fieldValue"];
        $dadosCCB[$campoCodigo] = $campoValor;
    }
    $dadosCCB["caseNum"] = $caseNumCCB;

    // </editor-fold>

    $resultadoValidacao = true;
    $separadorMsg = "";

    if ($dadosCCB["STATUS_CLICKSIGN"] !== "auto_close") {
        $msg = "Contrato não assinado";
        $separadorMsg = ",";
        $resultadoValidacao = false;
    }

    $comparacaoCPF = preg_replace("/\D+/", "", $dadosEntrada["contrato"]["issuer_cpf"]) == preg_replace("/\D+/", "", $dadosCCB["CPF"]);
    if (!$comparacaoCPF) {
        $msg = "CPF não confere";
        $separadorMsg = ",";
        $resultadoValidacao = false;
    }

    // Pega a KEY do BUKET de POSSE da CCB
    $ASSIGNOR_KEY = $dadosEntrada["dadosCessao"]["CCB_ACCOUNT_KEY"];
    $ASSIGNOR_KEY_CCB = $dadosCCB["ACCOUNT_KEY"];

    $comparacaoEmissor = $dadosEntrada["dadosCessao"]["ASSIGNOR_DATA"]["ACCOUNT_KEY"] == $dadosCCB["ACCOUNT_KEY"];
    $comparacaoEmissor = true;
    if (!$comparacaoEmissor & true) {
        $msg = $msg . $separadorMsg . "Emissor não confere '$ASSIGNOR_KEY' '$ASSIGNOR_KEY_CCB'";
        $separadorMsg = ",";
        $resultadoValidacao = false;
    }

    $dadosContrato = $dadosEntrada["contrato"];

    if (key_exists("nr_parcels", $dadosContrato)) {
        $numeroParcelasContrato = $dadosContrato["nr_parcels"];
    } else {
        if (key_exists("parcels", $dadosContrato)) {
            $numeroParcelasContrato = $dadosContrato["parcels"];
        }
    }

    $comparacaoParcela = $numeroParcelasContrato <= $dadosCCB["NR_PARCELAS"];
    if (!$comparacaoParcela) {
        $msg = $msg . $separadorMsg . "Nr. Parcelas maior que as parcelas de FACE";
        $separadorMsg = ",";
        $resultadoValidacao = false;
    }

    $dadosRetorno["resultadoAvaliacao"] = $resultadoValidacao;
    if (!$resultadoValidacao) {
        $dadosRetorno["message"] = $msg;
        $dadosRetorno["contractNumber"] = $dadosEntrada["contrato"]["number"];
    }

    $dadosRetorno["dadosCCB"] = $dadosCCB;
    $dadosRetorno["dadosCessao"] = $dadosEntrada;
    $jDados = json_encode($dadosRetorno);
    header('Content-Type: application/json');
    echo $jDados;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="api_change_cession">
/**
 * Rota: api/v1/cessionchange
 */
function api_change_cession()
{
    $dadosEntrada = apiTrataDadosEntrada();

//    error_log("Alteração Cessão" . var_export($dadosEntrada, true));

    $procId = $dadosEntrada["procId"];
    $stepId = $dadosEntrada["stepId"];
    $caseNum = $dadosEntrada["caseNum"];

    $userData["UserName"] = "mmoscz";
    $userData["UserPassword"] = "cerberus";

    foreach ($dadosEntrada as $chave => $valor) {
        $campo["fieldCode"] = $chave;
        $campo["fieldValue"] = $valor;
        $camposEnvio[] = $campo;
    }

    $dadosCaso["Fields"] = $camposEnvio;

    $caseNum = FuncCaseSave($userData, $procId, $stepId, $caseNum, $dadosCaso, 0, 0);

    $jDados = json_encode($caseNum);
    header('Content-Type: application/json');
    echo $jDados;
}

// </editor-fold>
// // <editor-fold defaultstate="collapsed" desc="getAccountToken">
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="account token">
/**
 * ROTA: api/v1/getaccounttoken
 */
function getAccountToken()
{
    $dadosEntrada = apiTrataDadosEntrada();

    // Pega ProcId das Entidades
    $procIdEntidades = PegaProcIdByCode("SEC_SIGNOR_TRANSFEREE");

    $accessKey = $dadosEntrada["accountKey"];

    // Busca o caso da Access Key
    $caseNum = pegaNumerCasoUnicoAberto($procIdEntidades, $accessKey, "ACCOUNT_KEY");

    $dadosCaso = pegaDadosCaso($procIdEntidades, $caseNum, array("ACCOUNT_TOKEN"));

    $jDados = json_encode($dadosCaso);
    header('Content-Type: application/json');
    echo $jDados;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="singers">
/**
 *  ROTA: api/v1/signers
 */
function getSingers()
{
    global $connect;

    $procId = PegaProcIdByCode("SEC_SIGNATARIOS");

    $stepId = PegaStepIdByCode($procId, "DADOS_ASSINANTE");

    $sql = "select caseId from casequeue where procid = $procId and stepid = $stepId";
    $query = mysqli_query($connect, $sql);


    $listaCasos = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $listaAssinantes = array();
    foreach ($listaCasos as $caso) {
        $dadosCaso = pegaDadosCaso("SEC_SIGNATARIOS", $caso["caseId"], array("CHAVE_ASSINATURA", "CPF", "ASSINATURAS_COMO_CEDENTE", "ASSINATURAS_COMO_RECEBEDOR", "TIPO_ASSINATURA_TERMO", "ASSINANTE_ATIVO"));

//        error_log("Dados Caso: " . var_export($dadosCaso, true));

        $dadosAssinante["singer_key"] = $dadosCaso["CHAVE_ASSINATURA"];
        $dadosAssinante["singer_document"] = $dadosCaso["CPF"];

        // Monta lista das assinaturas
        $signatures = array();

        $assinaturasCedente = json_decode($dadosCaso["ASSINATURAS_COMO_CEDENTE"], true);
        if (is_array($assinaturasCedente)) {
            $signatures = array_merge($signatures, $assinaturasCedente);
        }

        $assinaturasTermo = json_decode($dadosCaso["TIPO_ASSINATURA_TERMO"]);
        if (is_array($assinaturasTermo)) {
            $signatures = array_merge($signatures, $assinaturasTermo);
        }

        $assinaturasRecebedor = json_decode($dadosCaso["ASSINATURAS_COMO_RECEBEDOR"]);
        if (!$dadosCaso["ASSINATURAS_COMO_RECEBEDOR"] == "") {
            $signatures = array_merge($signatures, $assinaturasRecebedor);
        }

        $dadosAssinante["signatures"] = $signatures;
        $dadosAssinante["active"] = $dadosCaso["ASSINANTE_ATIVO"];
        $listaAssinantes[] = $dadosAssinante;
    }
    $jDados = json_encode($listaAssinantes);
    header('Content-Type: application/json');
    echo $jDados;
}

// </editor-fold>
// // <editor-fold defaultstate="collapsed" desc="Dummy Templates">
// ROTA: templates

function dummy_template()
{
//    $retorno["data"] = array();
//    $retorno["data"]["document"] = array();
//    $retorno["data"]["document"]["key"] = "abobora";
    $retorno["document"] = array();
    $retorno["document"]["key"] = "abobora";

    $jDados = json_encode($retorno);
    header('Content-Type: application/json');
    echo $jDados;
}

// </editor-fold>
/// <editor-fold defaultstate="collapsed" desc="Verifica contratos">
/**
 * ROTA: api/v1/checkcontracts
 * 
 * @global type $connect
 */
function verificaCCB()
{
    global $connect;
    $dadosEntrada = apiTrataDadosEntrada();

    $campoAccountKey = PegaFieldIdByCode("SEC_CCBS", "ACCOUNT_KEY");
    $campoNumeroContrato = PegaFieldIdByCode("SEC_CCBS", "CONTRATO");
    $campoStatusClickSign = PegaFieldIdByCode("SEC_CCBS", "STATUS_CLICKSIGN");
    $campoDocumentKey = PegaFieldIdByCode("SEC_CCBS", "DOCUMENT_KEY");
    $accountKey = $dadosEntrada["accountKey"];

    $separador = "";
    if (is_array($dadosEntrada["contracts"])) {
        foreach ($dadosEntrada["contracts"] as $contrato) {
            $listaContratos .= "$separador'" . $contrato . "'";
            $separador = ",";
        }
    } else {
        $listaContratos = "'" . $dadosEntrada["contracts"] . "'";
    }

//    $SQL = "select caseNum, campo$campoStatusClickSign as status, campo$campoDocumentKey as documentKey from exportkeys where campo$campoAccountKey = '$accountKey' and campo$campoNumeroContrato in ($listaContratos)";
    $SQL = "select caseNum, campo$campoStatusClickSign as status, campo$campoDocumentKey as documentKey from exportkeys where campo$campoAccountKey = '$accountKey' and campo$campoNumeroContrato in ($listaContratos) and (campo$campoStatusClickSign <> 'auto_close' or campo$campoStatusClickSign is null)";
    $Query = mysqli_query($connect, $SQL);

    if (!$Query) {
        error_log("Falha Buscando contratos: " . mysqli_error($connect) . "\n" . $SQL);
    }

    $resultado = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $dadosRetorno = json_encode($resultado);
    header('Content-Type: application/json');
    echo $dadosRetorno;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Pega dasos Casos Pela cessão">
/**
 * rota: api/v1/getcasesbycession
 * @global type $connect
 */
function getCasosDaCessao()
{
    global $connect;
    $dadosEntrada = apiTrataDadosEntrada();
    $campoAccountKey = PegaFieldIdByCode("SEC_CCBS", "ACCOUNT_KEY");
    $campoCessionNumber = PegaFieldIdByCode("SEC_CCBS", "CESSION_NUMBER");
    $campoDocumentKey = PegaFieldIdByCode("SEC_CCBS", "DOCUMENT_KEY");
    $campoNumeroContrato = PegaFieldIdByCode("SEC_CCBS", "CONTRATO");
    $accountKey = $dadosEntrada["accountKey"];
    $cession_number = $dadosEntrada["cessionNumber"];

    $SQL = "select caseNum, campo$campoDocumentKey as documentKey, campo$campoNumeroContrato as contract from exportkeys where campo$campoAccountKey = '$accountKey' and campo$campoCessionNumber = '$cession_number'";
    $Query = mysqli_query($connect, $SQL);
    $resultado = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $objRetorno = [];
    $objRetorno["query"] = $SQL;
    $objRetorno["contractsData"] = $resultado;
    $objRetorno["contractList"] = array_column($resultado, "contract");
    $objRetorno["documentKeys"] = array_column($resultado, "documentKey");

    $dadosRetorno = json_encode($objRetorno);

    header('Content-Type: application/json');
    echo $dadosRetorno;
}

// </editor-fold>
/// <editor-fold defaultstate="collapsed" desc="Pega casos pelo contrato">
/**
 * ROTA: api/v1/getcasesbycontracts
 * 
 * @global type $connect
 */
function pegaCasosPeloNumeroContrato()
{
    global $connect;
    $dadosEntrada = apiTrataDadosEntrada();

    $campoAccountKey = PegaFieldIdByCode("SEC_CCBS", "ACCOUNT_KEY");
    $campoNumeroContrato = PegaFieldIdByCode("SEC_CCBS", "CONTRATO");
    $campoDocumentKey = PegaFieldIdByCode("SEC_CCBS", "DOCUMENT_KEY");
    $accountKey = $dadosEntrada["accountKey"];
    $listaContratos = implode(",", $dadosEntrada["contracts"]);

//    $SQL = "select caseNum, campo$campoStatusClickSign as status, campo$campoDocumentKey as documentKey from exportkeys where campo$campoAccountKey = '$accountKey' and campo$campoNumeroContrato in ($listaContratos)";
    $SQL = "select caseNum, campo$campoDocumentKey as documentKey, campo$campoNumeroContrato as contract from exportkeys where campo$campoAccountKey = '$accountKey' and campo$campoNumeroContrato in ($listaContratos) and (campo$campoStatusClickSign <> 'auto_close' or campo$campoStatusClickSign is null)";
    $Query = mysqli_query($connect, $SQL);
    $resultado = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $objRetorno = [];
    $objRetorno["contractsData"] = $resultado;
    $objRetorno["contractList"] = array_column($resultado, "contract");
    $dadosRetorno = json_encode($objRetorno);

    header('Content-Type: application/json');
    echo $dadosRetorno;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Cessao Existe">

/**
 * ROTA: api/v1/cessionexists
 */
function cessionexists()
{
    $dadosEntrada = apiTrataDadosEntrada();

    $cessionNumber = $dadosEntrada["cessionNumber"];

    $procId = PegaProcIdByCode("SEC_CESSOES");

    $campoCession = PegaFieldCode($procId, "CESSION_NUMBER");

    $caseNum = pegaNumerCasoUnicoAberto($procId, $cessionNumber, $campoCession);

    $return = array();
    $return["result"] = $caseNum > 0;

    $dadosRetorno = json_encode($return);

    header('Content-Type: application/json');
    echo $dadosRetorno;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Casos Aguardando Assinatura">
/**
 * ROTA: api/v1/queuedcessionsigners
 */
function ListaCasosAguardandoAssinatura()
{
    global $connect;

    $procId = PegaProcIdByCode("SEC_CESSOES");

    $stepId = PegaStepIdByCode($procId, "ENVIAR_EMAIL_CEDENTES");

    $sql = "select caseId from casequeue where procid = $procId and stepid = $stepId";
    $query = mysqli_query($connect, $sql);
    $listaCasos = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $retorno = array();

    foreach ($listaCasos as $caso) {
        $caseNum = $caso["caseId"];
        $dadosCaso = pegaDadosCaso("SEC_CESSOES", $caseNum, array("SIGNATARIOS_ENDORSEE", "SIGNATARIOS_TRANSFEREE", "ASSINANTES_TERMO"));
        $dadosCaso["caseNum"] = $caseNum;
        $retorno[] = $dadosCaso;
    }

    $jDados = json_encode($retorno);
    header('Content-Type: application/json');
    echo $jDados;
}

// </editor-fold>

/**
 * ROTA: api/v1/getcessionclosedcontracts
 */
function getCessionContracts()
{
    global $connect;
    $dadosEntrada = apiTrataDadosEntrada();
    $cessionNumber = $dadosEntrada["cession_number"];
    $account_key = $dadosEntrada["account_key"];

    $procId = PegaProcIdByCode("SEC_CCBS");


    $SQL = "select campo12 as document_key from exportkeys where procId = $procId and Campo20 = $cessionNumber and campo15 = '$account_key'";
    $query = mysqli_query($connect, $SQL);
    $resultado = mysqli_fetch_all($query, MYSQLI_ASSOC);


    $jDados = json_encode($resultado);
    header('Content-Type: application/json');
    echo $jDados;
}
