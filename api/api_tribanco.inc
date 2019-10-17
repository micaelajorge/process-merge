<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 28/10/2018
  Sistema: Process_XAMPP
 */
// <editor-fold defaultstate="collapsed" desc="Definicoes e includes">
include_once(FILES_ROOT . "includes_ws/wsuser.inc");
include_once(FILES_ROOT . "includes_ws/wserror.inc");
require_once(FILES_ROOT . "includes_ws/wscasesave.inc");
require_once(FILES_ROOT . "includes_ws/wscasestart.inc");
include_once(FILES_ROOT . "api/apicasos.inc");
include_once(FILES_ROOT . "include/resource01.php");

//$casosImportarImagens = 100;
$casosImportarImagens = 50;

define("TRIBANCO_CODE_CARGA_PASSO_DADOS", "ENTRADA_CARGA");
define("TRIBANCO_CODE_CARGA_PASSO_ARQUIVO", "IMPORTARARQUIVO");
define("TRIBANCO_CODE_CARGA_PROCESSO", "TRIBANCO_CARGA");
define("TRIBANCO_CODE_CARGA_CAMPO_IMAGEM", "ARQUIVOS_DA_CARGA");
define("TRIBANCO_CODE_PROPOSTA_PROCESSO", "tribanco");
define("TRIBANCO_CODE_PROPOSTA_PASSO", "ENTRADA");
define("TRIBANCO_CODE_PROPOSTA_CAMPO_UNICO", "proposta");
define("TRIBANCO_CODE_PROPOSTA_PASSO_ARQUIVOS", "AGUARDANDO_IMAGENS");
define("TRIBANCO_CODE_PROPOSTA_CAMPO_ARQUIVO", "ARQUIVOS");

// </editor-fold>

//<editor-fold defaultstate="collapsed" desc="Cria caso pela proposta">
function tribanco_criaCasoPelaProposta()
{
    $inicio = round(microtime(true) * 1000);
    $retorno = funct_tribanco_criaCasoPelaProposta();
    $fim = round(microtime(true) * 1000);
    $retorno["duracao"] = number_format(($fim - $inicio) / 1000, 3) . " s";
    if ($retorno["error"]["errorNumber"] != "0") {
        header("HTTP/1.0 405 Error");
    }

    header('Content-Type: application/json');
    $retorno_json = json_encode($retorno);
    echo $retorno_json;
}

/**
 * 
 * @return type
 */
function funct_tribanco_criaCasoPelaProposta()
{
    //error_log("Criando Caso Proposta");

    $dadosEntrada = apiTrataDadosEntrada();
    $dadosRetorno = array();
    $dadosRetorno["error"]["errorNumber"] = 0;

    $procId = PegaProcIdByCode("tribanco");
    $authUser["UserName"] = "tribanco_carga";
    $authUser["UserPassword"] = "tribanco2018";
    $userdef = AuthenticateWs($authUser);

    //error_log(var_export($dadosEntrada, true));

    $dadosCaso = $dadosEntrada["casedata"];

    //error_log("Dados do Caso: " . var_export($dadosCaso, true));
    if (!is_array($dadosCaso)) {
        $dadosRetorno["operation"] = "Sem case Data";
        return $dadosRetorno;
    }
    if (count($dadosCaso) == 0) {
        $dadosRetorno["operation"] = "Sem case Data";
        return $dadosRetorno;
    }

    // Pega o Caso pelo numero da Proposta
    $numeroCasoExiste = pegaNumerCasoUnicoAberto($procId, $dadosCaso["proposta"]);

    // Se já existe caso para a Proposta ignora
    if ($numeroCasoExiste > 0) {
        $dadosRetorno["operation"] = 0;
        return $dadosRetorno;
    }

    // Pega o Novo Numero de caso
    $numeroCaso = GetNewCase($procId, $userdef);

    $camposRecebidos = trataCamposEmArray($dadosCaso);

    $dadoscaso["Fields"] = $camposRecebidos;

    // Cria caso se não existe ainda
    $dadosRetorno["operation"] = FuncCaseSave($authUser, $procId, TRIBANCO_CODE_PROPOSTA_PASSO, $numeroCaso, $dadoscaso, WS_PROCESSAR, 1);
    $dadosRetorno["nrCaso"] = $numeroCaso;
    return $dadosRetorno;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Busca de Arquivos de Carga no HOMES">
/**
 * 
 * @return type
 * 
 * ROTA: api/tribanco/verificaarquivoscarga/
 */
function tribanco_buscaDadosCarga()
{
    $inicio = round(microtime(true) * 1000);
    $retorno = funct_tribanco_buscaDadosCarga();
    $fim = round(microtime(true) * 1000);
    $retorno["duracao"] = number_format(($fim - $inicio) / 1000, 3) . " ms";
    if ($retorno["error"]["errorNumber"] != "0") {
        header("HTTP/1.0 405 Error");
    }

    header('Content-Type: application/json');
    $retorno_json = json_encode($retorno);
    echo $retorno_json;
}

function funct_tribanco_buscaDadosCarga()
{
    $dadosRetorno = array();
    $dadosRetorno["error"]["errorNumber"] = 0;

    $procId = PegaProcIdByCode(TRIBANCO_CODE_CARGA_PROCESSO);
    $authUser["UserName"] = "tribanco_carga";
    $authUser["UserPassword"] = "tribanco2018";
    $userdef = AuthenticateWs($authUser);

//    $urlSearch = 'https://tribanco.holmesdoc.com/holmes/api/search?';
//    $urlEnvio = urlencode('query=+_nature:"Base Excel"&start=0&rows=10&sortBy=_uploaddate&order=DESC');

    $urlSearch = 'https://tribanco.holmesdoc.com/holmes/api/search?query=+_nature:%22Base%20Excel%22&start=0&rows=20&sortBy=_uploaddate&order=DESC';
    $urlEnvio = "";

    $headersPesquisa = array();
    $headersPesquisa[] = 'accessToken: 980ed3d0-3061-49e4-96a3-79fbafd66553';
    $headersPesquisa[] = 'clientId: e8fb4acee6ce0c1f1ca67ecc7d9c8d33';

    $retornoWebService = executaWebServiceCURL($urlSearch . $urlEnvio, null, "GET", $headersPesquisa, false, false);

    //error_log("Buscando Arquivos HOMES " . var_export($retornoWebService, true));
    $dadosRetono = json_decode($retornoWebService, true);

    if (!is_array($dadosRetono)) {
        header('HTTP/1.0 403 ERRO');
        return $retornoWebService;
    }

//    error_log("Dados REtorno busca cargas");
//    error_log(var_export($dadosRetono, true));
    foreach ($dadosRetono["response"]["docs"] as $documentoEncontrado) {
        $documentId = $documentoEncontrado["_documentversionid"];

        $nomeArquivo = $documentoEncontrado["_name"];
        $extensao = pathinfo($nomeArquivo, PATHINFO_EXTENSION);


        if ($extensao != "txt") {
            // Se não é um arquivo TXT pula
            error_log("Não é TXT - " . $nomeArquivo);
            continue;
        }

        $numeroCaso = pegaNumerCasoUnicoAberto($procId, $documentId);
        if ($numeroCaso > 0) {
            // error_log("Encontrado Numero Caso: $numeroCaso ");
            // Se já tem um caso para esse DocumentID pula
            continue;
        }

        //error_log("Numero Caso: $numeroCaso");
        // Pega o Novo Numero de caso
        $numeroCaso = GetNewCase($procId, $userdef);

        $documentoEncontrado["ARQUIVO_DE_CARGA_IMPORTADO"] = "ok";

        $camposRecebidos = trataCamposEmArray($documentoEncontrado, "");

        $dadoscaso["Fields"] = $camposRecebidos;

        // Cria caso se não existe ainda
        $dadosRetorno["operation"][] = FuncCaseSave($authUser, $procId, TRIBANCO_CODE_CARGA_PASSO_DADOS, $numeroCaso, $dadoscaso, WS_PROCESSAR, 1);
    }
    return $dadosRetorno;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Busca de Arquivos no HOLMESDOC">

/**
 * 
 * @global type $connect
 * @global type $OrigemLogon
 * @return string
 */
function tribanco_buscaArquivos()
{
    $inicio = round(microtime(true) * 1000);
    $retorno = funct_tribanco_pegaArquivos();
    $fim = round(microtime(true) * 1000);
    $retorno["duracao"] = number_format(($fim - $inicio) / 1000, 3) . " ms";
    if ($retorno["error"]["errorNumber"] != "0") {
        header("HTTP/1.0 405 Error");
    }

    header('Content-Type: application/json');
    $retorno_json = json_encode($retorno);
    echo $retorno_json;
}

function funct_tribanco_pegaArquivos()
{
    $dadosEntrada = apiTrataDadosEntrada();

    $procId = PegaProcIdByCode(TRIBANCO_CODE_CARGA_PROCESSO);
    $authUser["UserName"] = "tribanco_carga";
    $authUser["UserPassword"] = "tribanco2018";

    $documentId = $dadosEntrada["versionid"];
    $numeroCaso = $dadosEntrada["caseNum"];

    // Monta o Headers para a pesquisa
    $headersPesquisa = array();
    $headersPesquisa[] = 'accessToken: 980ed3d0-3061-49e4-96a3-79fbafd66553';
    $headersPesquisa[] = 'clientId: e8fb4acee6ce0c1f1ca67ecc7d9c8d33';

    // Url para pesquisa
    $urlApi = 'https://tribanco.holmesdoc.com/holmes/api/download?versionId=';
    $urlChamada = $urlApi . $documentId;


    // Pega o Valor do Arquivo
    $dadosDoArquivo = executaWebServiceCURL($urlChamada, null, "GET", $headersPesquisa, false, false);

    // Gera dados referente ao arquivo
    $imagenData["fileName"] = $dadosEntrada["_filename"];
    error_log("FileName: " . $dadosEntrada["_filename"]);


    if ($dadosEntrada["codificarUTF8"] == "true") {
        //$imagenData["fileData"] = base64_encode(utf8_encode($dadosDoArquivo));
        $imagenData["fileData"] = base64_encode(mb_convert_encoding($dadosDoArquivo, 'UTF-8', 'UCS-2LE'));
    } else {
        $imagenData["fileData"] = base64_encode($dadosDoArquivo);
    }

    $aImagens[0] = $imagenData;

    $dadosImagens = criarArquivosEmCampoFolder($aImagens, $procId, $numeroCaso, TRIBANCO_CODE_CARGA_CAMPO_IMAGEM);

    error_log("Dados Imagem: " . var_export($dadosImagens, true));
    if (!$dadosImagens) {
        $dadosRetorno["error"]["errorDesc"] = "fail create file";
        $dadosRetorno["error"]["errorNumber"] = "10";
        $dadosRetorno["return"] = "failed";
        return $dadosRetorno;
    }

    // Adiciona o Campo de Arquivo Importado
    $campos = array();
    $campo["fieldCode"] = "ARQUIVO_DE_CARGA_IMPORTADO";
    $campo["fieldValue"] = "ok";
    $campos[] = $campo;

    $dadosCaso["Fields"] = array_merge($dadosImagens, $campos);
// 
    $dadosRetorno["error"]["errorNumber"] = 0;

    error_log("Dados Caso: " . var_export($dadosCaso, true));
    $dadosRetorno["operation"] = FuncCaseSave($authUser, $procId, TRIBANCO_CODE_CARGA_PASSO_ARQUIVO, $numeroCaso, $dadosCaso, WS_PROCESSAR, 0);
    return $dadosRetorno;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Lista de Casos de "Tribanco Carga" aguardando para executar a carga">
/**
 * Gera lista de casos para aguardando para executar a carga
 * @global type $connect 
 * 
 * ROTA: api/tribanco/listacasoscargaimportacao/"
 */
function tribancoListaCasosCargaParaImportacao()
{
    global $connect;
    $procIdTribanco = PegaProcIdByCode('TRIBANCO_CARGA');
    $stepIdGerarPropostas = PegaStepIdByCode($procIdTribanco, 'GERAR_CARGA');
    $SQL = "select "
            . "caseId as caseNum "
            . "from "
            . "casequeue "
            . "where "
            . "casequeue.procid = $procIdTribanco "
            . "and "
            . "StepId = $stepIdGerarPropostas ";
//            . "limit 1";
    $Query = mysqli_query($connect, $SQL);
    if (LOG_DATA) {
        error_log("Casos de Carga para Calculo SLA\n" . $SQL);
    }
       
    $listaCasos = array();
    $casosEncontrados = mysqli_fetch_all($Query, MYSQLI_ASSOC);    
    foreach ($casosEncontrados as $caso)
    {        
        $dadosCaso = pegaDadosCaso($procIdTribanco, $caso["caseNum"], array('ARQUIVOS_DA_CARGA', 'SLA_DOCUMENTOS'));
        $dadosArquivo = json_decode($dadosCaso["ARQUIVOS_DA_CARGA"], true);
        
        console.log("Dados Caso: " . var_export($dadosCaso, true));
        
        if (json_last_error() > 0)
        {
            continue;
        }
        
        $dadosCaso["fileNameStorage"] = $dadosArquivo["fileNameStorage"];
        $dadosCaso["fileName"] = $dadosArquivo["fileNameStorage"];
        $dadosCaso["CaseNum"] = $caso["caseNum"];
        $dadosCaso["StepId"] = $stepIdGerarPropostas;
        $dadosCaso["fieldId"] = PegaFieldIdByCode($procIdTribanco, 'ARQUIVOS_DA_CARGA');
        $dadosCaso["ProcId"] = $procIdTribanco;
        $listaCasos[] = $dadosCaso;
        break;
    }    
    $retorno["data"] = $listaCasos;
    $retorno["totalCasos"] = count($listaCasos);
    header('Content-Type: application/json');
    echo json_encode($retorno);
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Lista de casos para anexar arquivo de Carga">
/**
 * Lista de Casos para executar o anexar arquivo de carga
 * @global type $connect
 * 
 * ROTA: api/tribanco/cargasemarquivo
 */
function tribanco_CargasSemArquivo()
{

    // Define os dados usados para pegar a FILA.
    $procIdTribanco = PegaProcIdByCode('TRIBANCO_CARGA');
    $campoDocumentId = 4;
    $campoNomeArquivo = 6;
    $filaAguardandoArquivo = PegaStepIdByCode($procIdTribanco, 'IMPORTARARQUIVO');



    global $connect;
    $SQL = "select "
            . "casequeue.procid, "
            . "casenum, "
            . "stepid, "
            . "campo$campoDocumentId as documentId, "
            . "campo$campoNomeArquivo as nomeArquivo "
            . "from "
            . "casequeue, "
            . "exportkeys "
            . "where "
            . "exportkeys.procid = $procIdTribanco "
            . "and "
            . "exportkeys.procid = casequeue.procid "
            . "and "
            . "StepId = $filaAguardandoArquivo "
            . "and "
            . "exportkeys.casenum = casequeue.caseid limit 1";
    // error_log("Cargas sem arquivo $SQL");
    $Query = mysqli_query($connect, $SQL);
    $listaCasos = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $retorno["data"] = $listaCasos;
    $retorno["totalCasos"] = count($listaCasos);

    header('Content-Type: application/json');
    echo json_encode($retorno);
}

// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="Lista de Casos para calculo SLA">
/**
 * Gera lista de casos para aguardando para executar a carga
 * @global type $connect 
 * 
 * ROTA: api/tribanco/listacasoscalculosla/"
 */
function tribancoListaCasosCargaSLA()
{
    global $connect;

    $procIdTribanco = PegaProcIdByCode('TRIBANCO_CARGA');
    $stepIdGerarPropostas = PegaStepIdByCode($procIdTribanco, 'CALCULOSLA');
    $SQL = "select "
            . "caseId as caseNum "
            . "from "
            . "casequeue "
            . "where "
            . "casequeue.procid = $procIdTribanco "
            . "and "
            . "StepId = $stepIdGerarPropostas ";
    $Query = mysqli_query($connect, $SQL);
    if (LOG_DATA) {
        error_log("Casos de Carga para Calculo SLA\n" . $SQL);
    }
       
    $listaCasos = array();
    $casosEncontrados = mysqli_fetch_all($Query, MYSQLI_ASSOC);    
    foreach ($casosEncontrados as $caso)
    {
        $dadosCaso = pegaDadosCaso($procIdTribanco, $caso["caseNum"], array('ARQUIVOS_DA_CARGA', 'DATA_RECEBIMENTO'));       
        
        // Calcula se está na hora para fazer a importação
        $dataAtual = date('Y-m-d') . ' 10:10';        
        $diferenca = DateDiff($dataAtual, $dadosCaso["DATA_RECEBIMENTO"]);
        if ($diferenca > 0)
        {
            continue;
        }
        
        $dadosArquivo = json_decode($dadosCaso["ARQUIVOS_DA_CARGA"], true);
        $dadosCaso["fileNameStorage"] = $dadosArquivo["fileNameStorage"];
        $dadosCaso["fileName"] = $dadosArquivo["fileNameStorage"];
        $dadosCaso["CaseNum"] = $caso["caseNum"];
        $dadosCaso["ProcId"] = $procIdTribanco;
        $dadosCaso["fieldId"] = PegaFieldIdByCode($procIdTribanco, 'ARQUIVOS_DA_CARGA');        
        $listaCasos[] = $dadosCaso;
    }
        
    $retorno["data"] = $listaCasos;
    $retorno["totalCasos"] = count($listaCasos);
    header('Content-Type: application/json');
    echo json_encode($retorno);
}

// </editor-fold>

function tribanco_listaPropostasSemArquivo()
{
    global $connect, $casosImportarImagens;
    $listaPropostas = array();

    $procId = PegaProcIdByCode("tribanco");
    $stepId = PegaStepIdByCode($procId, "AGUARDANDO_IMAGENS");

    $SQL = "select caseNum, campo9 as proposta from casequeue, exportkeys where casequeue.procid = $procId and casequeue.stepid = $stepId and exportkeys.procid = casequeue.ProcId and exportkeys.CaseNum = casequeue.CaseId order by caseid limit $casosImportarImagens";
    $query = mysqli_query($connect, $SQL);
    $listaPropostas = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $retorno["dados"] = $listaPropostas;
    header('Content-Type: application/json');
    echo json_encode($retorno);
}

// <editor-fold defaultstate="collapsed" desc="recebimento de Arquivos da proposta">
/**
 * 
 * @return type
 * 
 * ROTA: api/tribanco/enviaarquivospropostas
 */
function tribanco_receberarquivosproposta()
{
    $dadosEntrada = apiTrataDadosEntrada();
    header('Access-Control-Allow-Origin: *');
    header("Access-Control-Allow-Credentials: true");
    header('Access-Control-Allow-Methods: GET, PUT, POST, DELETE, OPTIONS');
    header('Access-Control-Max-Age: 1000');
//header('Access-Control-Allow-Headers: Origin, Content-Type, X-Auth-Token , Authorization');
    header('Access-Control-Allow-Headers: x-requested-with, Content-Type, origin, authorization, accept, client-security-token');


    $procId = PegaProcIdByCode(TRIBANCO_CODE_PROPOSTA_PROCESSO);

    $authUser["UserName"] = "tribanco_carga";
    $authUser["UserPassword"] = "tribanco2018";

    $numeroCaso = $dadosEntrada["caseNum"];

    //error_log(var_export($dadosEntrada, true));

    $dadosImagens = criarArquivosEmCampoFolder($dadosEntrada["images"], $procId, $numeroCaso, TRIBANCO_CODE_PROPOSTA_CAMPO_ARQUIVO, false);
    if (!$dadosImagens) {
        $dadosRetorno["error"]["errorDesc"] = "fail create file";
        $dadosRetorno["error"]["errorNumber"] = "10";
        $dadosRetorno["return"] = "failed";
        header('Content-Type: application/json');
        $retorno = json_encode($dadosRetorno);
        echo $retorno;
        return;
    }

    //error_log("Dados Imagens " . var_export($dadosImagens, true));

    $dadosEnvio = $dadosEntrada["Fields"];

    //error_log("Dados Entrada " . var_export($dadosEntrada, true));
    //error_log("Dados Envio " . var_export($dadosEnvio, true));


    if (is_array($dadosEnvio)) {
        $dadosEnvio = array_merge($dadosEnvio, $dadosImagens);
    } else {
        $dadosEnvio = $dadosImagens;
    }


    $dadoscaso = array();
    $dadoscaso["Fields"] = $dadosEnvio;

    // Cria caso se não existe ainda
    //    die();
    //error_log("Dados caso " . var_export($dadoscaso, true));
    $dadosRetorno["operation"] = FuncCaseSave($authUser, $procId, TRIBANCO_CODE_PROPOSTA_PASSO_ARQUIVOS, $numeroCaso, $dadoscaso, 1, WS_PROCESSAR);

    header('Content-Type: application/json');
    $retorno = json_encode($dadosRetorno);
    echo $retorno;
}

// </editor-fold>