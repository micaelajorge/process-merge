<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 28/10/2018
  Sistema: Process_XAMPP
 */
include_once(FILES_ROOT . "includes_ws/wsuser.inc");
include_once(FILES_ROOT . "includes_ws/wserror.inc");
require_once(FILES_ROOT . "includes_ws/wscasesave.inc");
require_once(FILES_ROOT . "includes_ws/wscasestart.inc");
include_once(FILES_ROOT . "include/resource01.php");
include_once(FILES_ROOT . "include/funcs_transitwise.inc");

define("TRANSFERWISE_SENDDATA", 0);
define("TRANSFERWISE_SENDFILE", 1);
define("TRANSFERWISE_CODE_PASSO_DADOS", "RECEBIMENTO_DADOS");
define("TRANSFERWISE_CODE_PASSO_IMAGEM", "RECEBIMENTO_IMAGEM");
define("TRANSFERWISE_CODE_PROCESSO", "TRANSFER_WISE");
define("TRANSFERWISE_CODE_CAMPO_IMAGEM", "DOCUMENTO");

define("ERR_TRANSFERWISE_DATA_NOT_ARRAY", 12);


// <editor-fold defaultstate="collapsed" desc="Funcoes Acessorias">

/**
 * 
 * @param type $dados
 * @return type
 */
function trataDadosTransferWise($dados, $nomeArray = "")
{
    $dadosRetorno = trataCamposEmArray($dados, $nomeArray);
    return $dadosRetorno;
}

function respostaTransferwise()
{
    $inicio = round(microtime(true) * 1000);
    $retorno = func_respostaTransferwise();
    $fim = round(microtime(true) * 1000);
    $retorno["duracao"] = $fim - $inicio;
    $retorno_json = json_encode($retorno);
    header("Content-Type: application/json; charset=UTF-8");
    echo $retorno_json;
}

/**
 * 
 */
function sendfile()
{
    $inicio = round(microtime(true) * 1000);
    $retorno = funct_sendfile();
    $fim = round(microtime(true) * 1000);
    $retorno["duracao"] = $fim - $inicio;
    if ($retorno["error"]["errorNumber"] != "0") {
        header("HTTP/1.0 405 Error");
    }

    $retorno_json = json_encode($retorno);
    header('Content-Type: application/json; charset=UTF-8');   
    echo $retorno_json;
}

/**
 * 
 */
function senddata()
{
    $inicio = round(microtime(true) * 1000);
    $retorno = funct_senddata();
    $fim = round(microtime(true) * 1000);
    $retorno["duracao"] = number_format(($fim - $inicio) / 1000, 3) . " ms";
    if ($retorno["error"]["errorNumber"] != "0") {
        header("HTTP/1.0 405 Error");
    }
    $retorno_json = json_encode($retorno);
    header('Content-Type: application/json; charset=UTF-8');
    echo $retorno_json;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Tratamento de Erros">
/**
 * 
 * @param type $tipoChecagem
 * @param type $dadosEntrada
 * @return string
 */
function checkErrosTransferWise($tipoChecagem, $dadosEntrada)
{
    $dadosRetorno = array();
    $dadosRetorno["error"]["errorDesc"] = "";
    $dadosRetorno["error"]["errorNumber"] = "0";
    $dadosRetorno["return"] = "ok";

    if (!is_array($dadosEntrada)) {
        $dadosRetorno["error"]["errorDesc"] = "data invalid";
        $dadosRetorno["error"]["errorNumber"] = "1";
        $dadosRetorno["return"] = "failed";
        return $dadosRetorno;
    }

    /*
      if (!key_exists("auth", $dadosEntrada)) {
      $dadosRetorno["error"]["errorDesc"] = "user data not sent";
      $dadosRetorno["error"]["errorNumber"] = "2";
      $dadosRetorno["return"] = "failed";
      return $dadosRetorno;
      }

      $auth = $dadosEntrada["auth"];
      if (!is_array($auth)) {
      $dadosRetorno["error"]["errorDesc"] = "user data invalid #3";
      $dadosRetorno["error"]["errorNumber"] = "3";
      $dadosRetorno["return"] = "failed";
      return $dadosRetorno;
      }

     */
    if (!key_exists("batchId", $dadosEntrada)) {
        $dadosRetorno["error"]["errorDesc"] = "batch id invalid";
        $dadosRetorno["error"]["errorNumber"] = "5";
        $dadosRetorno["return"] = "failed";
        return $dadosRetorno;
    }

    /*
      if ($tipoChecagem == TRANSFERWISE_SENDDATA) {
      if (!is_array($dadosEntrada["address"])) {
      $dadosRetorno["error"]["errorDesc"] = "address is invalid";
      $dadosRetorno["error"]["errorNumber"] = ERR_TRANSFERWISE_DATA_NOT_ARRAY;
      $dadosRetorno["return"] = "failed";
      return $dadosRetorno;
      }
      }
     * 
     */

    foreach ($dadosEntrada["images"] as $arquivosRecebidos) {
// Verifica se o Há o campo arquivo
        if (!key_exists("fileData", $arquivosRecebidos)) {
            $dadosRetorno["error"]["errorDesc"] = "data field File invalid";
            $dadosRetorno["error"]["errorNumber"] = "6";
            $dadosRetorno["return"] = "failed";
            return $dadosRetorno;
        }

// Verifica se o Tamanho do aquivo está zero
        if (strlen($arquivosRecebidos["fileData"]) == 0) {
            $dadosRetorno["error"]["errorDesc"] = "data File empty";
            $dadosRetorno["error"]["errorNumber"] = "7";
            $dadosRetorno["return"] = "failed";
            return $dadosRetorno;
        }

// Verifica se há o campo fileName
        if (!key_exists("fileName", $arquivosRecebidos)) {
            $dadosRetorno["error"]["errorDesc"] = "File name invalid";
            $dadosRetorno["error"]["errorNumber"] = "8";
            $dadosRetorno["return"] = "failed";
            return $dadosRetorno;
        }
    }

    return $dadosRetorno;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Teste Resposta Transferwise">
function func_respostaTransferwise()
{
    $dadosEntrada = apiTrataDadosEntrada();
    // Pega a mensagem inteira recebida
    $bodyPart = file_get_contents('php://input');
    $headerRecebidos = getallheaders();

    if (LOG_DATA) {
        error_log("Response Headers:" . var_export($headerRecebidos, true));
        error_log("Response Body Part:" . var_export($bodyPart, true));
        error_log("Response dadosEntrada:" . var_export($dadosEntrada, true));
    }

    $retorno["return"] = "error";
    $testeAssinatura = false;
    if (is_array($headerRecebidos)) {
        if (key_exists("X-Signature", $headerRecebidos)) {
            $assinaturaDoEnvio = $headerRecebidos["X-Signature"];
            error_log("assinatura Recebida: $assinaturaDoEnvio");
            // Testa Assinatura
            $testeAssinatura = geraChaveSeguranca($bodyPart, _SECRET_KEY) == $assinaturaDoEnvio;
        } else {
            $retorno["return"] = "signature not received ";
        }
    } else {
        $retorno["return"] = "signature not received";
    }
    if ($testeAssinatura) {
        $retorno["return"] = "ok";
    }

    return $retorno;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Envio de dados">
/**
 * 
 * @return type
 */
function funct_senddata()
{
    // error_log("Entrou senddata");

    $dadosEntrada = apiTrataDadosEntrada();
// Faz a checagem dos dados Recebidos
    $dadosRetorno = checkErrosTransferWise(TRANSFERWISE_SENDDATA, $dadosEntrada);

// <editor-fold defaultstate="collapsed" desc="Verificacao Assinatura - Antiga Forma">

    /*
      /**
     *  Verificacao da Assinatura
      $fullName = $dadosEntrada["fullName"];
      $salt = $dadosEntrada["auth"]["salt"];
      $assinatura = $dadosEntrada["auth"]["signature"];
      $tipoDocumento = $dadosEntrada["documentType"];
      $batchId = $dadosEntrada["batchId"];

      $dadosValidacao = $fullName . $batchId . $tipoDocumento . $salt;

      // Testa Assinatura
      testeAssinatura = geraChaveSeguranca($dadosValidacao) == $assinatura;
      if (!$testeAssinatura) {
      $dadosRetorno["error"]["errorDesc"] = "signature invalid";
      $dadosRetorno["error"]["errorNumber"] = "-1";
      $dadosRetorno["return"] = "failed";
      return $dadosRetorno;
      }
     * 
     */
// </editor-fold>

    $batchId = $dadosEntrada["batchId"];

    // Pega a mensagem inteira recebida
    $bodyPart = file_get_contents('php://input');

    $headerRecebidos = getallheaders();

    $dadosRecebidos = "";
    $arquivolog = "log/senddata.txt";
    if (file_exists($arquivolog)) {
        $dadosRecebidos = file_get_contents($arquivolog);
    }

    $dadosRecebidos .= "\nHeaders \n";
    $dadosRecebidos .= var_export($headerRecebidos, true);

//    $dadosRecebidos .= $bodyPart;

    $dadosRecebidos .= "\n##############################\n";


    $testeAssinatura = false;
    if (is_array($headerRecebidos)) {
        if (key_exists("X-Signature", $headerRecebidos)) {
            $assinaturaDoEnvio = $headerRecebidos["X-Signature"];

            // Testa Assinatura           
            $testeAssinatura = geraChaveSeguranca($bodyPart, _SECRET_KEY) == $assinaturaDoEnvio;
        }
    }

    if ($dadosRetorno["error"]["errorNumber"] !== "0") {
        return $dadosRetorno;
    }

    if (!$testeAssinatura) {
        $dadosRetorno["error"]["errorDesc"] = "signature invalid";
        $dadosRetorno["error"]["errorNumber"] = "-1";
        $dadosRetorno["return"] = "failed";
        return $dadosRetorno;
    }

    $procId = PegaProcIdByCode(TRANSFERWISE_CODE_PROCESSO);

    $authUser["UserName"] = "transferwise";
    $authUser["UserPassword"] = "transfer";
    $userdef = AuthenticateWs($authUser);

    // Pega o Numero do Caso
    $numeroCaso = pegaNumerCasoUnicoAberto($procId, $batchId);
    if ($numeroCaso == 0) {
        $numeroCaso = GetNewCase($procId, $userdef);
    }

    $dadosBatch["batchid"] = $batchId;
    $dadosBatch["fullName"] = $dadosEntrada["fullName"];

    $documentType = $dadosEntrada["documentType"];
    $dadosBatch["documentType"] = $documentType;

    $dadosImagens = criarArquivosEmCampoFolder($dadosEntrada["images"], $procId, $numeroCaso, TRANSFERWISE_CODE_CAMPO_IMAGEM, false);
    if (!$dadosImagens) {
        $dadosRetorno["error"]["errorDesc"] = "fail create file";
        $dadosRetorno["error"]["errorNumber"] = "10";
        $dadosRetorno["return"] = "failed";
        return $dadosRetorno;
    }

    switch ($documentType) {
// Declaração Rendimentos
        case "taxDeclaration":
            $dadosDocumento["cpfCnpj"] = $dadosEntrada["cpfCnpj"];
            $camposDocumento = trataCamposEmArray($dadosDocumento, "");
            break;
        default:
            $camposDocumento = trataCamposEmArray($dadosEntrada["address"], "address");
            break;
    }

// Trata todos
    $camposBatch = trataCamposEmArray($dadosBatch, "");

// Junta os dados de imagens com os dados do Caso
    $camposCaso = array_merge($camposBatch, $dadosImagens, $camposDocumento);


// Seta o Arr
    $dadoscaso["Fields"] = $camposCaso;


// Cria caso se não existe ainda
//    die();
    $operation = FuncCaseSave($authUser, $procId, TRANSFERWISE_CODE_PASSO_DADOS, $numeroCaso, $dadoscaso, WS_PROCESSAR, 1);

    $dadosRetorno["operation"] = $operation;

    $dadosRecebidos .= "\n" . "##############################################################";
    $dadosRecebidos .= var_export($dadosRetorno, true);
    file_put_contents($arquivolog, $dadosRecebidos);

    if ($operation < 0) {
        $dadosRetorno["error"]["errorDesc"] = "Fail";
        $dadosRetorno["error"]["errorNumber"] = "$operation";
        $dadosRetorno["return"] = "failed";
    }

    return $dadosRetorno;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Envio de Arquivo">

/**
 * 
 * @global type $connect
 * @global type $OrigemLogon
 * @return string
 */
function funct_sendfile()
{
    global $connect, $OrigemLogon;
    $dadosEntrada = apiTrataDadosEntrada();

// Faz a checagem dos dados Recebidos
    $dadosRetorno = checkErrosTransferWise(TRANSFERWISE_SENDFILE, $dadosEntrada);

    if ($dadosRetorno["error"]["errorNumber"] !== "0") {
        header('HTTP/1.0 403 ERRO');
        return $dadosRetorno;
    }

    $authUser = $dadosEntrada["userData"];

    $procId = PegaProcIdByCode(TRANSFERWISE_CODE_PROCESSO);


    $userdef = new userdef;
    $userdef->Create($OrigemLogon, $connect, $authUser);

    $batchId = $dadosEntrada["batchId"];

    $camposCaso["batch_id"] = $batch_id;
    $camposCaso[TRANSFERWISE_CODE_CAMPO_IMAGEM] = $extendData;

    $camposTratados = trataDadosTransferWise($camposCaso);

    $dadosBatch["batchid"] = $batchId;
    $dadosBatch["fullName"] = $dadosEntrada["fullName"];
    $dadosBatch["documentType"] = $dadosEntrada["documentType"];

    $dadoscaso["Fields"] = $camposTratados;

// Pega o Numero do Caso
    $numeroCaso = pegaNumerCasoUnicoAberto($procId, $batchId);
    if ($numeroCaso == 0) {
        $numeroCaso = GetNewCase($procId);
    }

    $dadosImagens = criarArquivosEmCampoFolder($dadosEntrada["images"], $procId, $numeroCaso, TRANSFERWISE_CODE_CAMPO_IMAGEM);

    if (!$camposArquivos) {
        $dadosRetorno["error"]["errorDesc"] = "fail create file";
        $dadosRetorno["error"]["errorNumber"] = "10";
        $dadosRetorno["return"] = "failed";
        return $dadosRetorno;
    }

    $authUser["authUser"] = "transferwise";
    $authUser["authPassword"] = "transfer";


// 
    FuncCaseSave($authUser, $procId, TRANSFERWISE_CODE_PASSO_IMAGEM, $numeroCaso, $dadoscaso, WS_PROCESSAR, 1);
    return $dadosRetorno;
}

// </editor-fold>