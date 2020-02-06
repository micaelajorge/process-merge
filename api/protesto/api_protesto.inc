<?php

// <editor-fold defaultstate="collapsed" desc="Inclusões do script">
require_once(FILES_ROOT . 'includes_ws/funcsws.php');
require_once(FILES_ROOT . "include/resource01.php");
require_once(FILES_ROOT . "includes_ws/wsuser.inc");
require_once(FILES_ROOT . "includes_ws/wscasestart.inc");
require_once(FILES_ROOT . "includes_ws/wscasesave.inc");
require_once(FILES_ROOT . "includes_ws/wserror.inc");

// </editor-fold>


//$camposObrigatoriosProtesto = ["nosso_numero"];

/**
 * @rota api/v1/envia_protesto
 * @rota api/v1/envio_protesto
 *  Envio de Lote de Protestos
 */
function envia_lote_protesto_v1()
{
    grava_lote_protesto_v1("1");
}

/**
 * @rota api/v1/cancelamento_protesto
 * 
 *  Envio de lotes de Cancelamento
 */
function envio_lote_cancelamento_v1()
{
    grava_lote_protesto_v1("3");
}

/**
 *  @rota api/v1/desistencia_protesto
 *  Envio de lotes de desistencia
 */
function envio_lote_desistencia_v1()
{
    grava_lote_protesto_v1("2");
}

/** 
 * @global array $camposObrigatoriosProtesto
 */
function grava_lote_protesto_v1($tipoOperacaoEntrada)
{
    global $camposObrigatoriosProtesto;

    $procCod = "PROTESTO_LOTES_CLIENTE";

    try {
        $dadosEntrada = apiTrataDadosEntrada();

        $chavesRecebidas = array_keys($dadosEntrada);

//        $dadosProcessoValido = validaDadosEntrada($dadosEntrada, $camposObrigatoriosProtesto);
//        if (!$dadosProcessoValido) {
//            api_send_falta_parametros();
//        }

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

        $loteUUID = get_guid();

        $dadosLote["LOTE_UUID"] = $loteUUID;

        $dadosBody = get_request_body();
        

        /**
         * Dados do Request
         */
        // PEGA TODOS DOS DADOS DO REQUEST
        $REQUEST_DATA["DADOS_LOTE"] = $dadosLote;
        $REQUEST_DATA["HEADERS"] = getallheaders();
        $REQUEST_DATA["BODY"] = $dadosBody;
        $REQUEST_DATA["GET"] = filter_input_array(INPUT_GET);
        $REQUEST_DATA["POST"] = filter_input_array(INPUT_POST);
        $REQUEST_DATA["SERVER"] = $_SERVER;

        // Transforma em JSON
        $request_json = json_encode($REQUEST_DATA);

        // Transforma em BASE64
        $request_json_base64 = base64_encode($request_json);

        // Inicializacao do Campo
        $campo["fieldCode"] = "REQUEST_DATA";
        $campo["fieldValue"] = array();
        $arquivoDados["fileName"] = "request_data.$loteUUID.json";
        $arquivoDados["fileData"] = $request_json_base64;
        $campo["fieldValue"][] = $arquivoDados;
        $camposEnvio[] = $campo;

        $campo["fieldCode"] = "REQUEST_DATA";
        $campo["fieldValue"] = array();

        /**
         * Dados do LOTE
         */
        // Transforma em JSON
        $dadosLote_json = json_encode($dadosBody);

        // Transforma em BASE64
        $dadosLote_json_base64 = base64_encode($dadosLote_json);

        // Inicializacao do Campo
        $campo["fieldCode"] = "DADOS_LOTE";
        $campo["fieldValue"] = array();

        $arquivoDados["fileName"] = "$loteUUID.json";
        $arquivoDados["fileData"] = $dadosLote_json_base64;
        $campo["fieldValue"][] = $arquivoDados;
        $camposEnvio[] = $campo;

        $campo["fieldCode"] = "LOTE_UUID";
        $campo["fieldValue"] = $loteUUID;
        $camposEnvio[] = $campo;

        $campo["fieldCode"] = "TIPO_OPERACAO";
        $campo["fieldValue"] = $tipoOperacaoEntrada;
        $camposEnvio[] = $campo;

        // Pega o ProcId do Caso
        $procId = PegaProcIdByCode($procCod);

        $userData = "";

        foreach ($camposEnvio as $campo) {
            if (!is_array($campo["fieldValue"])) {
                $dadosCaso["Fields"][] = $campo;
            } else {
                $camposArquivo[] = $campo;
            }
        }


        // Pega os dados do Usuário
        $userdef = $_SESSION["userdef"];

        $numeroCaso = GetNewCase($procId, $userdef);

        $camposArquivo = criarArquivosEmCampo($procId, $numeroCaso, $camposArquivo);

        $dadosCaso["Fields"] = array_merge($dadosCaso["Fields"], $camposArquivo);

        $caseNum = FuncCaseSave($userData, $procId, 'START', $numeroCaso, $dadosCaso, 0, 1);
        if ($caseNum !== 0) {
            header("HTTP/1.0 201 Created");
            $novoCaseNum["process"] = $loteUUID;
            $novoCaseNum["error"] = array();
        }
        $dadosRetorno = $novoCaseNum;
        $jDados = json_encode($dadosRetorno);
        if (json_last_error() > 0) {
            $erroJSON = json_last_error() . " " . json_last_error_msg();
            error_log("Json erro: " . $erroJSON);
        }
    } catch (Exception $ex) {
        $jDados = json_encode($ex);
        header("Access-Control-Allow-Origin: *");
        header('Content-Type: application/json');
        echo $jDados;
    }
    header("Access-Control-Allow-Origin: *");
    header('Content-Type: application/json');
    echo $jDados;
}

function trataCamposCredor($dadosCredor)
{
    $camposEnvio[] = array();

    $chavesRecebidas = array_keys($dadosCredor);
    foreach ($chavesRecebidas as $chave) {
        // Salta o valor se for um array
        if (is_array($dadosCredor[$chave])) {
            continue;
        }
        $fieldValue = $dadosCredor[$chave];
        if (!is_string($fieldValue)) {
            $fieldValue = (string) $fieldValue;
        }
        $campo["fieldCode"] = strtoupper($chave);
        $campo["fieldValue"] = $fieldValue;
        $camposEnvio[] = $campo;
    }
    return $camposEnvio;
}

function trataCamposDevedores($dadosDevedores)
{
    $camposEnvio[] = array();

    $identificacao_devedor = 0;
    
    // Trata se o $devedores não é array
    if (!is_array($dadosDevedores))
    {
        $dadosDevedor = $dadosDevedores;
        $dadosDevedores = array();
        $dadosDevedores[] = $dadosDevedor;
    }
    
    foreach ($dadosDevedores as $dadoDevedor) {
        $chavesRecebidas = array_keys($dadoDevedor);
        foreach ($chavesRecebidas as $chave) {
            // Salta o valor se for um array
            if (is_array($dadoDevedor[$chave])) {
                continue;
            }
            $fieldValue = $dadoDevedor[$chave];
            if (!is_string($fieldValue)) {
                $fieldValue = (string) $fieldValue;
            }
            $campo["fieldCode"] = strtoupper($chave) . "_" . $identificacao_devedor;
            $campo["fieldValue"] = $fieldValue;
            $camposEnvio[] = $campo;
        }
        $identificacao_devedor++;
    }

    return $camposEnvio;
}

function trataCamposCertdox($dadosCertdox)
{
    $camposEnvio[] = array();

    $chavesRecebidas = array_keys($dadosCertdox);
    foreach ($chavesRecebidas as $chave) {
        // Salta o valor se for um array
        if (is_array($dadosCertdox[$chave])) {
            continue;
        }
        $fieldValue = $dadosCertdox[$chave];
        if (!is_string($fieldValue)) {
            $fieldValue = (string) $fieldValue;
        }
        $campo["fieldCode"] = strtoupper($chave);
        $campo["fieldValue"] = $fieldValue;
        $camposEnvio[] = $campo;
    }
    return $camposEnvio;
}

/**
 * 
 * @rota api/v1/envia_protesto_certdox
 * @return type
 */
function protesto_entrada_protesto_v1()
{
    try {

        $dadosEntrada = apiTrataDadosEntrada();

        $procCod = "PROTESTO_TITULOS";

        $chavesRecebidas = array_keys($dadosEntrada);

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

        // Pega os campos Devedor
        $camposDevedores = trataCamposDevedores($dadosEntrada["Devedor"]);
        $camposEnvio = array_merge($camposEnvio, $camposDevedores);

        // Pega os campos Credor
        $camposCredor = trataCamposCredor($dadosEntrada["Credor"]);
        $camposEnvio = array_merge($camposEnvio, $camposCredor);

        // Pega os campos Cedente
        $camposCedente = trataCamposCredor($dadosEntrada["Cedente"]);
        $camposEnvio = array_merge($camposEnvio, $camposCedente);

        // Pega os campos Titulo
        $camposTitulo = trataCamposCredor($dadosEntrada["Titulo"]);
        $camposEnvio = array_merge($camposEnvio, $camposTitulo);

        // Pega os campos Certdox
        $camposCertdox = trataCamposCertdox($dadosEntrada["CertDox"]);
        $camposEnvio = array_merge($camposEnvio, $camposCertdox);

        $procId = PegaProcIdByCode($procCod);

        $userData = "";

        // Pega os dados do Usuário
        $userdef = $_SESSION["userdef"];

        $numeroCaso = GetNewCase($procId, $userdef);

        $dadosCaso["Fields"] = $camposEnvio;

        $caseNum = FuncCaseSave($userData, $procId, 'START', $numeroCaso, $dadosCaso, 0, 1);
        if ($caseNum !== 0) {
            header("HTTP/1.0 201 Created");
            $novoCaseNum["numero_caso"] = $caseNum;
            $novoCaseNum["error"] = array();
        }
        $dadosRetorno = $novoCaseNum;
        $jDados = json_encode($dadosRetorno);
        if (json_last_error() > 0) {
            $erroJSON = json_last_error() . " " . json_last_error_msg();
            error_log("Json erro: " . $erroJSON);
        }
    } catch (Exception $ex) {
        $jDados = json_encode($ex);
        header("Access-Control-Allow-Origin: *");
        header('Content-Type: application/json');
        echo $jDados;
        return;
    }
    header("Access-Control-Allow-Origin: *");
    header('Content-Type: application/json');
    echo $jDados;
}

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
        $campoTipoOperacao = PegaFieldIdByCode($procId, "TIPO_OPERACAO");
        $campos = array();
        
        $SQL = "";
        if($numero_protesto != "")  
        {     
            $filtros_where[0]["campo"]= "NUMERO_PROTESTO";
            $filtros_where[0]["valor"]=$numero_protesto;
            $campos[0] = "Campo$campoNumeroProtesto";
            $campos[1] = "Campo$campoTituloStatus";
            $campos[2] = "Campo$campoResultado";
            $campos[3] = "Campo$campoTipoOperacao";
            $camposTexto = implode($campos, ",");
            
            $camposSelecionados = array();  
            $camposSelecionados =["ELEGIBILIDADE_RESULTADO","TITULO_STATUS", "TIPO_OPERACAO", 
                "NUMERO_PROTESTO", "DATA_ATUALIZACAO", "VALOR_CUSTAS"];
            $Query = pegaListaCasosNaFila($procId, -1, 1, '' , '', '', '', 1, true);
            $retorno = mysqli_fetch_all($Query, MYSQLI_ASSOC);
            //$caseNum = pegaNumerCasoUnicoAberto($procId, $numero_protesto, $campoNumeroProtesto);
            //$resultado = $retorno;
            
           //                if($resultado["TITULO_STATUS"] == "") header("HTTP/1.0 404 Não encontrado.");
//                else header("HTTP/1.0 200 Sucesso");
            //$resultado = pegaDadosCaso($procId, $caseNum, $camposSelecionados);
            $header = "HTTP/1.0 404 Não encontrado.";
            $i = 0;
                foreach ($retorno as $resultado){

                $caseNum = $resultado["CaseId"];
                $caso = pegaDadosCaso($procId, $caseNum, $camposSelecionados);
                if($caso["NUMERO_PROTESTO"] == $numero_protesto){
                    $header = "HTTP/1.0 200 Sucesso";
                  

                    $dadosRetorno[$i]["casenum"] = $caseNum;
                    $dadosRetorno[$i]["caso"] = $caso;   
                    //$dadosRetorno["resultado"] = $resultado;
                    $dadosRetorno[$i]["numero_protesto"] = $numero_protesto;                
                    $dadosRetorno[$i]["status"] = $caso["TITULO_STATUS"];                
                    $dadosRetorno[$i]["data_ultimo_status"] = date($caso["DATA_ATUALIZACAO"]);  
                    $dadosREtorno[$i]["valor_custas"] = $caso["VALOR_CUSTAS"];
                    switch ($caso["TIPO_OPERACAO"])
                    {
                        case 1: $dadosRetorno[$i]["solicitacao"] = "PROTESTO"; break;
                        case 2: $dadosRetorno[$i]["solicitacao"] = "DESISTÊNCIA"; break;
                        case 3: $dadosRetorno[$i]["solicitacao"] = "CANCELAMENTO"; break;
                    }
                    $dadosRetorno[$i]["Erros"] =  json_decode($caso["ELEGIBILIDADE_RESULTADO"]);
                    $i++;
                }
            }
        }
        else 
        {
            header("HTTP/1.0 402 Falha");
            //$dadosRetorno["status"] = $SQL ;
            $dadosRetorno[0]["Error"] = "";
    
        }
        $jDados = json_encode($dadosRetorno);   
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

function detalheTitulo($titulo, $numero_protesto)
{   
    try {
        global $connect;       
 
        $procCod = "PROTESTO_TITULOS";         
        // Pega o ProcId do Caso
        $procId = PegaProcIdByCode($procCod);

        $campoNumeroProtesto = PegaFieldIdByCode($procId, "NUMERO_PROTESTO");
        
        $camposSelecionados = array();  
        $camposSelecionados =[]; // determinar campos
        $Query = pegaListaCasosNaFila($procId, -1, 1, '' , '', '', '', 1, true);
        $retorno = mysqli_fetch_all($Query, MYSQLI_ASSOC);
        $caseNum = pegaNumerCasoUnicoAberto($procId, $numero_protesto, $campoNumeroProtesto);
        $resultado = pegaDadosCaso($procId, $caseNum, $camposSelecionados);
        $caso = pegaDadosCaso($procId, $caseNum, $camposSelecionados);
        return $caso;
    }
    catch (Exception $ex) {
            return ex;
    }
}


