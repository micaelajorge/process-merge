<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 12/11/2018
  Sistema: Process_XAMPP
 */

include_once(FILES_ROOT . "/include/funcs_transitwise.inc");

/**
 * 
 * @param type $urlWebservice
 * @param type $dados em formato JSON (texto)
 * @param type $metodo POST / GET
 * @return type
 */
function executaWebService_file_get($urlWebservice, $dados, $metodo)
{
    $resultado = file_get_contents($urlWebservice, null, stream_context_create(array(
        'http' => array(
            'method' => $metodo,
            'header' => 'Content-Type: application/json' . "\r\n"
            . 'Content-Length: ' . strlen($dados) . "\r\n",
            'content' => $dados,
        ),
    )));
    return $resultado;
}

// <editor-fold defaultstate="collapsed" desc="transit - TxDeclaration">
/**
 * 
 * @param type $objCaso
 * @param type $urlWebservice
 * @return boolean
 */
function transitWiseTaxDeclaration($objCaso, $urlWebservice)
{
    $aRetornoFuncao["result"] = false;
    $aRetornoFuncao["erroRetorno"] = "indefinido";

    $secretKey = $objCaso->ParseValuesFields("!{_SECRET_KEY}!");
    //error_log("Secret Key $secretKey");

    $adadosParaEnvio = array();
    $adadosParaEnvio["batchId"] = "!%batchId!%";
    $adadosParaEnvio["resultCode"] = "!%resultCode!%";
    $adadosParaEnvio["allowance"] = "!%allowance!%";
    $adadosParaEnvio["year"] = "!%year!%";

    $json_envio = json_encode($adadosParaEnvio);
    
    $dadosParaEnvio = $objCaso->ParseValuesFields($json_envio);
    
    $batchId = $objCaso->ParseValuesFields("!%batchId!%");
    //error_log("Dados Envio: \n$json_envio");
    // Gera a asinatura
    
    file_put_contents("log/send_$batchId.txt", $dadosParaEnvio);
    
    
    $assinatura = geraChaveSeguranca($dadosParaEnvio, $secretKey);
    //error_log("Assinatura ENVIO $assinatura");
    $headerDeAssinatura = array("X-Signature: $assinatura", "X-BODY: $dadosParaEnvio");
    
    if (LOG_DATA)
    {
        error_log("Body Part: " . var_export($dadosParaEnvio, true));
        error_log("Signature: " . var_export($headerDeAssinatura, true));
        error_log("Dados Envio: " . var_export($dadosParaEnvio, true));
        error_log("Array Envio: " . var_export($adadosParaEnvio, true));
    }

    $retornoExecucao = executaWebServiceCURL($urlWebservice, $dadosParaEnvio, "POST", $headerDeAssinatura);
    $dadosRetorno = json_decode($retornoExecucao, true);
    if (is_array($dadosRetorno)) {
        if (key_exists("return", $dadosRetorno)) {
            $retornoFuncao = $dadosRetorno["return"];
            $aRetornoFuncao["result"] = true;
            $aRetornoFuncao["valorRetorno"] = $retornoFuncao;
            unset($aRetornoFuncao["erroRetorno"]);
        } else {
            $aRetornoFuncao["erroRetorno"] = $dadosRetorno["erroChamada"];
        }
    } else {
        // $aRetornoFuncao["erroRetorno"] = error_get_last();
        $aRetornoFuncao["erroRetorno"] = "erro";
    }
    return $aRetornoFuncao;
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="transit - address">
/**
 * 
 * @param type $objCaso
 * @param type $urlWebservice
 * @return boolean
 */
function transitWiseAddress($objCaso, $urlWebservice)
{
    $aRetornoFuncao["result"] = false;
    $aRetornoFuncao["erroRetorno"] = "indefinido";

    $secretKey = $objCaso->ParseValuesFields("!{_SECRET_KEY}!");
    //error_log("Secret Key $secretKey");

    $adadosParaEnvio = array();
    $adadosParaEnvio["batchId"] = "!%batchId!%";
    $adadosParaEnvio["resultCode"] = "!%resultCode!%";

    $json_envio = json_encode($adadosParaEnvio);
    
    $dadosParaEnvio = $objCaso->ParseValuesFields($json_envio);
    
    $batchId = $objCaso->ParseValuesFields("!%batchId!%");
    //error_log("Dados Envio: \n$json_envio");
    // Gera a asinatura
    
    file_put_contents("log/send_$batchId.txt", $dadosParaEnvio);
    
    
    $assinatura = geraChaveSeguranca($dadosParaEnvio, $secretKey);
    //error_log("Assinatura ENVIO $assinatura");
    $headerDeAssinatura = array("X-Signature: $assinatura", "X-BODY: $dadosParaEnvio");
    
    error_log("Body Part: " . var_export($dadosParaEnvio, true));
    error_log("Signature: " . var_export($headerDeAssinatura, true));

    $retornoExecucao = executaWebServiceCURL($urlWebservice, $dadosParaEnvio, "POST", $headerDeAssinatura);
    $dadosRetorno = json_decode($retornoExecucao, true);
    if (is_array($dadosRetorno)) {
        if (key_exists("return", $dadosRetorno)) {
            $retornoFuncao = $dadosRetorno["return"];
            $aRetornoFuncao["result"] = true;
            $aRetornoFuncao["valorRetorno"] = $retornoFuncao;
            unset($aRetornoFuncao["erroRetorno"]);
        } else {
            $aRetornoFuncao["erroRetorno"] = $dadosRetorno["erroChamada"];
        }
    } else {
        // $aRetornoFuncao["erroRetorno"] = error_get_last();
        $aRetornoFuncao["erroRetorno"] = "erro";
    }
    return $aRetornoFuncao;
}
// </editor-fold>

function TribancoPegaArquivo()
{
    
}