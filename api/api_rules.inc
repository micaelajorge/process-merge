<?php

//

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 12/06/2019
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

function ruleData_v1()
{
    global $ruleCode;

    $dadosRegra = buscaDadosEntradaRegra($ruleCode);

    header("HTTP/1.0 200");
    header('Content-Type: application/json');
    $dadosRegra_saida = json_decode($dadosRegra);
    echo json_encode($dadosRegra_saida, JSON_PRETTY_PRINT);
}

/**
 * 
 * @global type $ruleCode
 * 
 * @rota api/v1/rules
 */
function avaliaRegra_v1()
{
    global $ruleCode;
    $dadosEntrada = apiTrataDadosEntrada();

    $dadosRetorno = avaliaRegraSelecionada($ruleCode, $dadosEntrada);
    if (!is_array($dadosRetorno)) {
        $dadosRetorno["resultado"] = false;
    }
    $jDados = json_encode($dadosRetorno);

    header("HTTP/1.0 201");
    header('Content-Type: application/json');
    echo $jDados;
}

function callRule($dadosEntrada, $userToken, $ruleCode, $ExecuteUUID)
{
    $curl = curl_init();

    $urlCall = SERVER_URL . "/api/v1/rules_real/$ruleCode";

    $dadosChamadaCurl = array(
        CURLOPT_URL => "$urlCall",
        CURLOPT_RETURNTRANSFER => true,
        CURLOPT_ENCODING => "",
        CURLOPT_MAXREDIRS => 10,
        CURLOPT_TIMEOUT => 0,
        CURLOPT_FOLLOWLOCATION => true,
        CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
        CURLOPT_CUSTOMREQUEST => "POST",
        CURLOPT_POSTFIELDS => json_encode($dadosEntrada),
        CURLOPT_HTTPHEADER => array(
            "token: $userToken",
            "Content-Type: application/json"
        )
    );

    curl_setopt_array($curl, $dadosChamadaCurl);
    $json_response = curl_exec($curl);
    $returnHttpCode = curl_getinfo($curl, CURLINFO_HTTP_CODE);

    if ($returnHttpCode == 500)
    {
        // Seta o UUID da CHAMADA para gravar no log
        $dadosChamada["ExecuteUUID"] = $ExecuteUUID;
        $dadosChamada["DadosCURL"] = $dadosChamadaCurl;
        error_log("Falha na chamada callRule " . var_export($dadosChamada , true));
    }
    
    $dadosRetorno = array();
    $dadosRetorno["httpCode"] = $returnHttpCode;
    $dadosRetorno["jsonReturn"] = $json_response;

    curl_close($curl);
    return $dadosRetorno;
}

function executeRule_v1()
{
    global $ruleCode;

    $dadosEntrada = apiTrataDadosEntrada();

    $executeUUID = get_guid();

    $headers = getallheaders();

    $userToken = $headers["token"];

    $jsonDadosEntrada = json_encode($dadosEntrada);

    $dadosRetorno = callRule($jsonDadosEntrada, $userToken, $ruleCode, $executeUUID);

    $httpCode = $dadosRetorno["httpCode"];

    // Grava o log da Executação da REGRA
    grava_log_rule_v1($dadosRetorno, $executeUUID, $ruleCode, $httpCode);

    header("HTTP/1.0 $httpCode");
    header("X-Request-UUID: $executeUUID");
    header('Content-Type: application/json');
    echo $dadosRetorno["jsonReturn"];
}

/**
 * 
 * @rota: api/v1/evaluate
 */
function evaluate()
{
    ini_set('html_errors', false);
    ini_set('display_errors', 1);
    ini_set('display_startup_errors', 1);
    error_reporting(E_ERROR | E_PARSE);
    $dadosEntrada = apiTrataDadosEntrada();

    $expressao = $dadosEntrada["expressao"];
    $dadosAvaliacao = $dadosEntrada["parametros"];
    try {
        $retorno = trataExpressao($expressao, $dadosAvaliacao);
    } catch (ParseError $p) {

        echo $p->getMessage();
    }

    $jDados = json_encode($retorno);
    header("HTTP/1.0 202 Regra Ok");
    header('Content-Type: application/json');
    echo $jDados;
}

/**
 * 
 * @param type $expressoes
 * @param type $parametros
 * @return type
 */
function avaliaExpressoes($expressoes, $parametros)
{
    $dadosRetorno = array();
    foreach ($expressoes as $expressaoAvaliada) {

        //Verifica se deve avaliar a expressão
        if ($expressaoAvaliada["evaluate_expression"] === "false") {
            continue;
        }
        error_log("Avaliando " . var_export($expressaoAvaliada, true));
        $dadosRetorno[] = trataExpressao($expressaoAvaliada, $parametros);
        error_log("Avaliada" . var_export($expressaoAvaliada, true));
    }
    return $dadosRetorno;
}

/**
 * 
 * @param type $listaOperadores
 * @param type $dadosAvaliacao
 * @return type
 */
function trataExpressao($listaOperadores, $dadosAvaliacao)
{
    $codigoAvaliacao_Regra = "";
    foreach ($listaOperadores["operacoes"] as $regra) {
        $logic = "";
        $bracktOpen = "";
        $bracktClose = "";
        if (key_exists("bracketOpen", $regra)) {
            $bracktOpen = $regra["bracketOpen"];
        }

        $bracktClose = "";
        if (key_exists("bracketClose", $regra)) {
            $bracktClose = $regra["bracketClose"];
        }

        $typeOperation = "comparar";
        if (key_exists("typeOperation", $regra)) {
            $typeOperation = $regra["typeOperation"];
        }
        switch ($typeOperation) {
            case "funcao":
                $dadosOperador = trataLogicaFuncao($regra, $dadosAvaliacao);
                break;
            case "comparar":
            default:
                $dadosOperador = trataLogicaSimples($regra, $dadosAvaliacao);
                break;
        }

        if (key_exists("logic", $regra)) {
            $logic = $regra["logic"];
        }

        $codigoAvaliacao_Regra .= "$bracktOpen $logic $dadosOperador $bracktClose";
    }
    $resultadoAvaliacao = false;

    // Se a avaliação está em branco retorna erro
    if ($codigoAvaliacao_Regra === "") {
        $resultadoAvaliacao = "false";
    } else {
        $codigoAvaliacao = '$resultadoAvaliacao' . " = ($codigoAvaliacao_Regra) ? 'true' : 'false';";
        try {
//        $test = eval($string);
            eval($codigoAvaliacao);
        } catch (Exception $ex) {
            
        }
    }

    if ($resultadoAvaliacao !== "false") {
        $resultadoOperacao = $listaOperadores["resultado"]['true'];
    } else {
        $resultadoOperacao = $listaOperadores["resultado"]['false'];
    }

    if (key_exists("AVALIAR_REGRA", $resultadoOperacao)) {
        $resultadoRegra = avaliaRegraSelecionada($resultadoOperacao["AVALIAR_REGRA"], $dadosAvaliacao);

        $dadosRetorno = $resultadoRegra["dados"];

        $retorno = array();

        foreach ($dadosRetorno as $dadoRetornado) {
            $retorno[] = $dadoRetornado;
        }
//        if (!isset($resultadoOperacao["dados"]))
//        {
//            $resultadoOperacao["dados"] = array();
//        }
//        $dadosRegra = $resultadoRegra["dados"];
//        $resultadoOperacao["dados"] = array_merge($resultadoOperacao["dados"], $dadosRegra);
    } else {
        $retorno = array_merge(["operacao" => $listaOperadores["operacao"]], $resultadoOperacao);
    }

    return $retorno;
}

/**
 * 
 */
function testarRegra()
{
    ini_set('html_errors', false);
    ini_set('display_errors', 1);
    ini_set('display_startup_errors', 1);
    error_reporting(E_ERROR | E_PARSE);
    $dadosEntrada = apiTrataDadosEntrada();
    $parametros = $dadosEntrada["inputData"];
    $rule = $dadosEntrada["rules"];
    $resultadoAvaliacao = avaliaExpressoes($rule["regras"], $parametros);
    $dadosRetorno["resultado"] = trataRetornos($resultadoAvaliacao);
    $dadosRetorno["resultadosTratamento"] = $resultadoAvaliacao;
    $dadosRetorno["dadosRecebidos"] = $parametros;
    $dadosRetorno["retornoMensagem"] = $rule["retornoMensagem"];
    if (!is_array($dadosRetorno)) {
        $dadosRetorno["resultado"] = false;
    }
    $jDados = json_encode($dadosRetorno);
    header('Content-Type: application/json');
    echo $jDados;
}

/**
 * 
 * @param type $funcao
 * @return boolean
 */
function verificaFuncaoExiste($funcao)
{
    $regex = "/^(.*?)\(/m";
    $matches = array();
    $totalEncontrados = preg_match($regex, $funcao, $matches);
    if ($totalEncontrados == 0) {
        return false;
    }
    $listaFuncoes = get_defined_functions();
    if (in_array($matches[1], $listaFuncoes["user"])) {
        return true;
    }
    if (in_array($matches[1], $listaFuncoes["user"])) {
        return true;
    }

    return false;
}

/**
 * 
 * @param type $regra
 * @param type $dadosAvaliacao
 * @return string
 */
function trataLogicaFuncao($regra, $dadosAvaliacao)
{
    $funcao = $regra["funcao"];
    $matches_campos = array();
    $matches_funcao = array();

    /*
     * PEGA o campo para macro substituição
     */
    $regex_CAMPO = "/!%(.*)?!%/m";
    preg_match($regex_CAMPO, $funcao, $matches_campos);

    /*
     * PEGA o NOME da FUNÇÃO
     */
    $regex_FUNCAO = "/(^.*)\(/m";
    preg_match($regex_FUNCAO, $funcao, $matches_funcao);

    if (!function_exists($matches_funcao[1])) {
        $retorno["falha"] = "Função " . $matches_funcao[1] . " não encontrada";
        $retorno["regra"] = $regra;
        $json_retorno = json_encode($retorno);
        header("HTTP/1.0 422 Falha tratamento Regra");
        header('Content-Type: application/json');
        echo $json_retorno;
        die();
    }

//    // Busca a função strlen e modifica para função interna w_strlen
//    if (strtolower($matches_funcao[1]) == "strlen")
//    {
//        $funcao = str_ireplace("strlen", "w_strlen", $funcao);
//    }

    $NOME_CAMPO = $matches_campos[1];
    $comparador = "";
    if (key_exists("comparador", $regra)) {
        $comparador = $regra["comparador"];
        if ($comparador != "") {
            $comparador = $regra["comparador"];
            if (key_exists("comparador", $regra)) {
                $valorComparar = "'" . $regra["valor"] . "'";
            }
        }
    }
    $dadosRegra = str_replace($matches_campos[0], $dadosAvaliacao[$NOME_CAMPO], $funcao) . $comparador . $valorComparar;
    return $dadosRegra;
}

/**
 * 
 * @param type $regra
 * @param type $dadosAvaliacao
 * @return type
 */
function trataLogicaSimples($regra, $dadosAvaliacao)
{
    $valorCampo = $dadosAvaliacao[$regra["campo"]];
    $comparador = $regra["comparador"];

    if (isset($regra["valor"])) {
        $valorComparar = $regra["valor"];
    }

    if (isset($regra["logicaCampo"])) {
        $valorComparar = $dadosAvaliacao[$regra["logicaCampo"]];
    }

    if (isset($regra["logicaFuncao"])) {
        switch ($regra["logicaFuncao"]) {
            case "DATA_ATUAL":
                $valorComparar = date("Y-m-d");
                break;
        }
    }

    $dadosRegra = "'$valorCampo' $comparador '$valorComparar'";
    return $dadosRegra;
}

/**
 * 
 * @param type $code
 * @return type
 */
function betterEval($code)
{
    ob_start();
    $tmp = tmpfile();
    $tmpf = stream_get_meta_data($tmp);
    $tmpf = $tmpf ['uri'];
    fwrite($tmp, $code);
    $ret = include ($tmpf);
    fclose($tmp);
    ob_clean();
    return $resultadoAvaliacao;
}

/**
 * 
 * @param type $ruleCode
 * @param type $dadosEntrada
 * @return type
 */
function avaliaRegraSelecionada($ruleCode, $dadosEntrada)
{
    $json_regras = buscaDadosRegra($ruleCode);
    $json_regras = str_replace("\\\\", "\\", $json_regras);
    $rule = json_decode($json_regras, true);
    try {
        $resultadoAvaliacao = avaliaExpressoes($rule["regras"], $dadosEntrada);
    } catch (Exception $ex) {
        
    }

    $dadosRetorno["resultado"] = trataRetornos($resultadoAvaliacao);
    $dadosRetorno["dados"] = $resultadoAvaliacao;
    $dadosRetorno["retornoMensagem"] = $rule["retornoMensagem"];
    return $dadosRetorno;
}

/**
 * @global array $camposObrigatoriosProtesto
 */
function grava_log_rule_v1($retornoRule, $ExecuteUUID, $ruleCode, $httpCode)
{
    $procCod = "RULE_LOG";

    try {

        $dadosBody = get_request_body();

        $request = array();
        $request["headers"] = getallheaders();
        $request["server_data"] = $_SERVER;
        $request["receive_body"] = $dadosBody;

        // Transforma Dados Entrada em JSON
        $request_json = json_encode($request, JSON_PRETTY_PRINT);

        // Transforma em BASE64
        $request_json_base64 = base64_encode($request_json);

        // Transforma Resultado em JSON
        $resultado_json = json_encode($retornoRule, JSON_PRETTY_PRINT);

        // Transforma em BASE64
        $resultado_json_base64 = base64_encode($resultado_json);


        // Inicializacao do Campo
        $campo["fieldCode"] = "IMAGENS_INT";
        $campo["fieldValue"] = array();

        // Guarda Dados Recebidos
        $arquivoDados["fileName"] = "request_data.$ExecuteUUID.json";
        $arquivoDados["fileData"] = $request_json_base64;
        $campo["fieldValue"][] = $arquivoDados;

        // Guarda Dados Enviados
        $arquivoDados["fileName"] = "response_data.$ExecuteUUID.json";
        $arquivoDados["fileData"] = $resultado_json_base64;
        $campo["fieldValue"][] = $arquivoDados;
        $camposEnvio[] = $campo;

        $campo["fieldCode"] = "REQUEST_UUID";
        $campo["fieldValue"] = $ExecuteUUID;
        $camposEnvio[] = $campo;

        $campo["fieldCode"] = "RULE_CODE";
        $campo["fieldValue"] = $ruleCode;
        $camposEnvio[] = $campo;

        $campo["fieldCode"] = "HTTP_CODE";
        $campo["fieldValue"] = $httpCode;
        $camposEnvio[] = $campo;

        $campo["fieldCode"] = "URL_SERVER";
        $campo["fieldValue"] = SERVER_URL;
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

        $caseNum = FuncCaseSave($userData, $procId, 'CHAMADA_EFETUADA', $numeroCaso, $dadosCaso, 0, 1);
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
        
    }
}

function _avaliaRegra_v1()
{
    global $rouleCode, $rouleVersion;
    $dadosEntrada = apiTrataDadosEntrada();
    $dadosRetorno["rouleCode"] = $rouleCode;
    $dadosRetorno["rouleVersion"] = $rouleVersion;
    $dadosRetorno["parameters"] = $dadosEntrada;
    switch ($rouleCode) {
        case "POC_RENNER_INICIO1":
        case "POC_RH_INICIO":
            $dadosRetorno["retorno"] = array();
            $dadosRetorno["retorno"] = array();
            $dadosRetorno["retorno"]["documentos"] = array();
            if ($SEXO == "M") {
                $dadosRetorno["retorno"]["documentos"][] = "RESERVISTA";
            }
            if ($CASADO == "1") {
                $dadosRetorno["retorno"]["documentos"][] = "CERTIDAO_DE_CASAMENTO";
            }
            if ($TEM_FILHOS == 1) {
                $dadosRetorno["retorno"]["documentos"][] = "CADERNETA_VACINACAO_SUS";
            }
//        if (!VALIDACPF($CPF))
//        {
//            $dadosRetorno["retorno"]["falha_validacao"][] = "CPF inválido";
//        }
            $dadosRetorno["retorno"]["documentos"][] = "RG";
            $dadosRetorno["retorno"]["documentos"][] = "CPF";
            $dadosRetorno["retorno"]["documentos"][] = "PROPOSTA_DE_EMPREGO";
            $dadosRetorno["resultado"] = true;
            break;
        case "POC_RH_DEMISSAO":
            $dadosRetorno["retorno"] = array();
            $dadosRetorno["retorno"]["documentos"] = array();
            $dadosRetorno["retorno"]["documentos"][] = "ADF";
            $dadosRetorno["resultado"] = true;
            break;
        case "TESTE_CAMPO_CPF_TESTE":
            if (!validaCPF($CPF)) {
                $dadosRetorno["resultado"] = false;
                $dadosRetorno["retornoMensagem"] = "CPF inválido";
            } else {
                $dadosRetorno["resultado"] = true;
                $dadosRetorno["retornoMensagem"] = "Validação OK";
            }
            validaCPF($CPF);
            break;
        default:
            $dadosRetorno["resultado"] = true;
            $dadosRetorno["retornoMensagem"] = "Regra não encontrada";
    }
    if (!is_array($dadosRetorno)) {
        $dadosRetorno["resultado"] = false;
    }
    $jDados = json_encode($dadosRetorno);
    header('Content-Type: application/json');
    echo $jDados;
}

function buscaDadosRegra($ruleCode)
{
    global $connect;
    $SQL = "select rule_operations from rules where UCASE(rule_code) = UCASE('$ruleCode')";
    $query = mysqli_query($connect, $SQL);
    $retorno = mysqli_fetch_all($query, MYSQLI_ASSOC);
    return $retorno[0]["rule_operations"];
}

function buscaDadosEntradaRegra($ruleCode)
{
    global $connect;
    $SQL = "select rule_input from rules where UCASE(rule_code) = UCASE('$ruleCode')";
    $query = mysqli_query($connect, $SQL);
    $retorno = mysqli_fetch_all($query, MYSQLI_ASSOC);
    return $retorno[0]["rule_input"];
}

function trataRetornos($dadosRetorno)
{
    $resultado = true;
    foreach ($dadosRetorno as $retorno) {
        if (!is_array($retorno)) {
            $resultado = false;
            break;
        }

        if (!key_exists("resultado", $retorno)) {
            $retorno["resultado"] = trataRetornos($retorno);
        }
        // Trata se foi definido como BOOLEANO ou como TEXTO "true" / "false"
        $valorRetorno = (is_bool($retorno["resultado"])) ? $retorno["resultado"] : $retorno["resultado"] == "true";
        $resultado = $resultado && $valorRetorno;
        if (!$resultado) {
            break;
        }
    }
    return $resultado;
}

//function trataRetornos($dadosRetorno)
//{
//    $resultado = true;
//    foreach ($dadosRetorno as $retorno) {
//        if (!is_array($retorno)) {
//            $resultado = false;
//            break;
//        }
//                
//        if (!key_exists("resultado", $retorno)) {                                   
//            $resultado = false;
//            break;
//        }
//        // Trata se foi definido como BOOLEANO ou como TEXTO "true" / "false"
//        $valorRetorno = (is_bool($retorno["resultado"])) ? $retorno["resultado"] : $retorno["resultado"] == "true";
//        $resultado = $resultado && $valorRetorno;
//        if (!$resultado) {
//            break;
//        }
//    }
//    return $resultado;
//}
