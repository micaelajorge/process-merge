<?php

// <editor-fold defaultstate="collapsed" desc="Inclusões do script">
require_once(FILES_ROOT . 'includes_ws/funcsws.php');
require_once(FILES_ROOT . "include/resource01.php");
require_once(FILES_ROOT . "includes_ws/wsuser.inc");
require_once(FILES_ROOT . "includes_ws/wscasestart.inc");
require_once(FILES_ROOT . "includes_ws/wscasesave.inc");
require_once(FILES_ROOT . "includes_ws/wserror.inc");

// </editor-fold>


$camposObrigatoriosProtesto = ["nosso_numero"];

/**
 * ROTA: api/v1/envia_protesto
 * 
 * @global array $camposObrigatoriosProtesto
 */
function envia_lote_protesto_v1()
{
    global $camposObrigatoriosProtesto;

    $procCod = "PROTESTO_LOTES_CLIENTE";

    try {
        $dadosEntrada = apiTrataDadosEntrada();

        $chavesRecebidas = array_keys($dadosEntrada);

        $dadosProcessoValido = validaDadosEntrada($dadosEntrada, $camposObrigatoriosProtesto);
        if (!$dadosProcessoValido) {
            api_send_falta_parametros();
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

        $loteUUID = get_guid();

        $dadosLote["LOTE_UUID"] = $loteUUID;


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
 * ROTA: api/v1/envia_protesto_certdox
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
