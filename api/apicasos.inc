<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 30/07/2018
  Sistema: Process_XAMPP
 */
include_once(FILES_ROOT . 'includes_ws/funcsws.php');
include_once(FILES_ROOT . "include/resource01.php");
include_once(FILES_ROOT . "includes_ws/wsuser.inc");
include_once(FILES_ROOT . "includes_ws/wscasestart.inc");
include_once(FILES_ROOT . "includes_ws/wserror.inc");
include_once(FILES_ROOT . "includes_ws/wscasesave.inc");

// <editor-fold defaultstate="collapsed" desc="apicaseprocess">
/**
 *  Rota: api/v1/apicaseprocess
 */
function apicaseprocess_v1()
{
    $dadosEntrada = apiTrataDadosEntrada();

    //error_log("dados Recebidos" . var_export($dadosEntrada, true));


    if (!key_exists("caseData", $dadosEntrada)) {
        $dadosEntrada["caseData"] = "";
    }

    if (key_exists("procid", $dadosEntrada)) {
        $dadosEntrada["procCode"] = $dadosEntrada["procid"];
    }

    if (key_exists("proccode", $dadosEntrada)) {
        $dadosEntrada["procCode"] = $dadosEntrada["proccode"];
    }

    if (key_exists("stepid", $dadosEntrada)) {
        $dadosEntrada["StepCode"] = $dadosEntrada["stepid"];
    }

    if (key_exists("stepcode", $dadosEntrada)) {
        $dadosEntrada["stepCode"] = $dadosEntrada["stepcode"];
    }

    if (key_exists("casenum", $dadosEntrada)) {
        $dadosEntrada["caseNum"] = $dadosEntrada["casenum"];
    }

    if (key_exists("userdata", $dadosEntrada)) {
        $dadosEntrada["userData"] = $dadosEntrada["userdata"];
    }

    if (!key_exists("caseNum", $dadosEntrada))
    {
        header("HTTP/1.0 422 Error");
        return;        
    }
    
    /**
     *  Acao Padrão no Caso
     */
    $acaoCaso = 0;
    if (key_exists("salvarCaso", $dadosEntrada)) {
        $acaoCaso = $dadosEntrada["salvarCaso"];
    }

    $Fields = converteFieldsParaCaseData($dadosEntrada["caseData"]);

    $dadosCaso = array();
    $dadosCaso["Fields"] = array();

    foreach ($Fields as $campo) {
        if (!is_array($campo["fieldValue"])) {
            $dadosCaso["Fields"][] = $campo;
        } else {
            $camposArquivo[] = $campo;
        }
    }

    $procId = PegaProcIdByCode($dadosEntrada["procCode"]);

    $dadosImagens = criarArquivosEmCampo($procId, $dadosEntrada["caseNum"], $camposArquivo);

    $Fields = array_merge($dadosCaso["Fields"], $dadosImagens);

    $dadosEntrada["caseData"]["Fields"] = $Fields;

    $novoCaseNum = CaseSave($dadosEntrada["userData"], $dadosEntrada["procCode"], $dadosEntrada["stepCode"], $dadosEntrada["caseNum"], $dadosEntrada["caseData"], $acaoCaso);

    $dadosRetorno = $novoCaseNum;
    $jDados = json_encode($dadosRetorno);
    if (json_last_error() > 0) {
        $erroJSON = json_last_error() . " " . json_last_error_msg();
        error_log("Json erro: " . $erroJSON);
    }
    header('Content-Type: application/json');
    echo $jDados;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="apicasestartunique_v1">
// ROTA "api/v1/casestartunique"
function apicasestartunique_v1()
{
    global $OrigemLogon, $connect;
    try {

        // Dados Enviados no POST / GET
        $dadosEntrada = apiTrataDadosEntrada();

        // Pega o ProcId do Caso
        $procId = PegaProcIdByCode($dadosEntrada["procCode"]);

        if (key_exists("caseData", $dadosEntrada)) {
            if (key_exists("Fields", $dadosEntrada["caseData"])) {
                $fields = $dadosEntrada["caseData"]["Fields"];
            } else {
                $fields = $dadosEntrada["caseData"];
            }
        } else {
            $fields = converteFieldsParaCaseData($dadosEntrada["fields"]);
        }

        // Busca o Campo UNIQUE

        $camposUniqueClose = PegaCampoUniqueClose($procId);

        // Pega o campo unique do Caso
        $fieldUnique = $camposUniqueClose["FieldUnique"];

        // Verfifica o valor Enviado do campo unique
        $valorCampoUnique = buscaValorCampoUnique($procId, $fields, $fieldUnique);

        // Verfifica se é caso unico
        $numeroCasoExiste = pegaNumerCasoUnicoAberto($procId, $valorCampoUnique);

        $dadosCaso["Fields"] = array();
        $camposArquivo = array();


        if ($numeroCasoExiste == 0) {

            validaUsuarioToken();

            if (isset($_SESSION["userdef"])) {
                console . log("usuario da cessao");
                $userdef = $_SESSION["userdef"];
            } else {
                console . log("usuario via parametro");
                $authUser = $dadosEntrada["userData"];
                $userdef = new userdef;
                $userdef->Create($OrigemLogon, $connect, $authUser);
            }

            $numeroCaso = GetNewCase($procId, $userdef);

            $dadosCaso = array();
            $dadosCaso["Fields"] = array();

            $Fields = converteFieldsParaCaseData($dadosEntrada["caseData"]);

            foreach ($Fields as $campo) {
                if (!is_array($campo["fieldValue"])) {
                    $dadosCaso["Fields"][] = $campo;
                } else {
                    $camposArquivo[] = $campo;
                }
            }

            $dadosImagens = criarArquivosEmCampo($procId, $numeroCaso, $camposArquivo);

            $dadosCaso["Fields"] = array_merge($dadosCaso["Fields"], $dadosImagens);

//            error_log("Fields" . var_export($dadosCaso, true));

            $novoCaseNum["caseNum"] = FuncCaseSave($dadosEntrada["userData"], $dadosEntrada["procCode"], $dadosEntrada["stepCode"], $numeroCaso, $dadosCaso, 0, 1);
            $novoCaseNum["error"] = array();

            //$novoCaseNum = CaseStart($dadosEntrada["userData"], $dadosEntrada["procCode"], $dadosEntrada["stepCode"], $dadosEntrada["caseData"], $dadosEntrada["acao"]);
        } else {
            $novoCaseNum["caseNum"] = 0;
            $novoCaseNum["Error"] = "Caso Existe";
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
// <editor-fold defaultstate="collapsed" desc="apicasestartuniqueprocessa">
// ROTA "api/casestartuniqueprocessa"
function apicasestartuniqueprocessa()
{
    try {

        $dadosEntrada = apiTrataDadosEntrada();

        $dadosEntrada = padronizaDadosEntrada($dadosEntrada);

        $procId = PegaProcIdByCode($dadosEntrada["procCode"]);

        $camposUniqueClose = PegaCampoUniqueClose($procId);
        $fieldUnique = $camposUniqueClose["FieldUnique"];

        $fields = $dadosEntrada["caseData"]["Fields"];

        $valorCampoUnique = buscaValorCampoUnique($procId, $fields, $fieldUnique);

        $numeroCasoExiste = pegaNumerCasoUnicoAberto($procId, $valorCampoUnique);

        if ($numeroCasoExiste == 0) {

            $Fields = converteFieldsParaCaseData($dadosEntrada["caseData"]);

            $dadosCaso = array();
            $dadosCaso["Fields"] = array();

            foreach ($Fields as $campo) {
                if (!is_array($campo["fieldValue"])) {
                    $dadosCaso["Fields"][] = $campo;
                } else {
                    $camposArquivo[] = $campo;
                }
            }

            $dadosImagens = criarArquivosEmCampo($procId, $dadosEntrada["caseNum"], $camposArquivo);

            $Fields = array_merge($dadosCaso["Fields"], $dadosImagens);

            $dadosEntrada["caseData"]["Fields"] = $Fields;

            $novoCaseNum = CaseStart($dadosEntrada["userData"], $dadosEntrada["procCode"], $dadosEntrada["stepCode"], $dadosEntrada["caseData"], $dadosEntrada["acao"]);
        } else {
            $novoCaseNum = CaseSave($dadosEntrada["userData"], $dadosEntrada["procCode"], $dadosEntrada["stepCode"], $numeroCasoExiste, $dadosEntrada["caseData"], 0);
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

// ROTA "api/v1/casestartuniqueprocessa"
function apicasestartuniqueprocessa_v1()
{
    try {
//        error_log("Inicia apicasestartuniqueprocessa_v1");
        $dadosEntrada = apiTrataDadosEntrada();

        $dadosEntrada = padronizaDadosEntrada($dadosEntrada);

        $procId = PegaProcIdByCode($dadosEntrada["procCode"]);

        $camposUniqueClose = PegaCampoUniqueClose($procId);
        $fieldUnique = $camposUniqueClose["FieldUnique"];

        $fields = $dadosEntrada["caseData"]["Fields"];

        $valorCampoUnique = buscaValorCampoUnique($procId, $fields, $fieldUnique);

        if ($valorCampoUnique === false) {
            header("HTTP/1.0 422 Campo Unique não informado");
            header('Content-Type: application/json');
            return;
        }

        $numeroCasoExiste = pegaNumerCasoUnicoAberto($procId, $valorCampoUnique);

        if ($numeroCasoExiste == 0) {

            $Fields = converteFieldsParaCaseData($dadosEntrada["caseData"]);

            $dadosCaso = array();
            $dadosCaso["Fields"] = array();

            foreach ($Fields as $campo) {
                if (!is_array($campo["fieldValue"])) {
                    $dadosCaso["Fields"][] = $campo;
                } else {
                    $camposArquivo[] = $campo;
                }
            }

            $dadosImagens = criarArquivosEmCampo($procId, $dadosEntrada["caseNum"], $camposArquivo);

            $Fields = array_merge($dadosCaso["Fields"], $dadosImagens);

            $dadosEntrada["caseData"]["Fields"] = $Fields;


            $novoCaseNum = CaseStart($dadosEntrada["userData"], $dadosEntrada["procCode"], $dadosEntrada["stepCode"], $dadosEntrada["caseData"], $dadosEntrada["acao"]);
        } else {
            $novoCaseNum = CaseSave($dadosEntrada["userData"], $dadosEntrada["procCode"], $dadosEntrada["stepCode"], $numeroCasoExiste, $dadosEntrada["caseData"], 0);
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
    header('Content-Type: application/json');
    echo $jDados;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Apicaseprocess">
/**
 *  Rota: api/apicaseprocess
 */
function apicaseprocess()
{

    $dadosEntrada = apiTrataDadosEntrada();

    //error_log("dados Recebidos", var_export($dadosEntrada, true));


    if (!key_exists("casedata", $dadosEntrada)) {
        $dadosEntrada["casedata"] = "";
    }
    $novoCaseNum = CaseSave($dadosEntrada["userdata"], $dadosEntrada["proccode"], $dadosEntrada["stepcode"], $dadosEntrada["casenum"], $dadosEntrada["casedata"], 0);

    $dadosRetorno = $novoCaseNum;
    $dadosRetorno["dadosEntrada"] = $dadosEntrada;
    $jDados = json_encode($dadosRetorno);
    if (json_last_error() > 0) {
        $erroJSON = json_last_error() . " " . json_last_error_msg();
        error_log("Json erro: " . $erroJSON);
    }
    header('Content-Type: application/json');
    echo $jDados;
}

// </editor-fold>s
// <editor-fold defaultstate="collapsed" desc="Get File">
/**
 * 
 * ROTA: api/getfile
 * ROTA: api/v1/getfile
 * @return type
 */
function apigetfile()
{
    $dadosEntrada = apiTrataDadosEntrada();

    $validadoToken = validaUsuarioToken();

    if (!$validadoToken) {
        $authUser = $dadosEntrada["authUser"];
        $userdef = AuthenticateWs($authUser);
        if (!$userdef) {
            header("HTTP/1.0 403 Error");
            return;
        }
    }

    /**
     *  Verifica o procId
     */
    if (key_exists("procId", $dadosEntrada))
    {
        $procId = $dadosEntrada["procId"];        
    } else {
        if (!key_exists("procCode", $dadosEntrada))
        {
            api_send_falta_parametros();
        }
        $procCode = $dadosEntrada["procCode"];
        $procId = PegaProcIdByCode($procCode);
    }
    
    /**
     *  Verifica o fieldId
     */
    if (key_exists("fieldId", $dadosEntrada))
    {
        $fieldId = $dadosEntrada["fieldId"];        
    } else {
        if (!key_exists("fieldCode", $dadosEntrada))
        {
            api_send_falta_parametros();
        }
        $fieldCode = $dadosEntrada["fieldCode"];
        $fieldId = PegaFieldIdByCode($procId, $fieldCode);
    }

    
    $caseNum = $dadosEntrada["caseNum"];
    

    $diretorioArquivo = diretorioArquivoImagem($procId, $caseNum, $fieldId);

    $arquivoLeitura = FILES_UPLOAD . "/" . $diretorioArquivo . $fileName;

    $arquivoEncontrado = file_exists($arquivoEncontrado);
    if (!$arquivoEncontrado) {
        header("HTTP/1.0 403 File not found");
        return;
    }
    readfile($arquivoLeitura);
}

/*
 *  ROTA api/getfileslist
 */
function apiGetFilesList()
{
    global $connect;

    $dadosEntrada = apiTrataDadosEntrada();

    $proccode = isset($dadosEntrada["proccode"]) ? $dadosEntrada["proccode"] : $dadosEntrada["procCode"];
    $caseNum = isset($dadosEntrada["casenum"]) ? $dadosEntrada["casenum"] : $dadosEntrada["caseNum"];
    $fieldCode = isset($dadosEntrada["fieldcode"]) ? $dadosEntrada["fieldcode"] : $dadosEntrada["fieldCode"];

    $ProcId = PegaProcIdByCode($proccode);

    if ($ProcId == 0) {
        $listaArquivos["Err"] = array();
        $listaArquivos["Err"]["code"] = -1;
        $listaArquivos["Err"]["desc"] = "Nao encontrada Solucao: $proccode";
        echo json_encode($listaArquivos);
        return;
    }

    if ($fieldCode == "") {
        $SQL = "select fieldid from procfieldsdef where procid = $ProcId and fieldType = 'FD'";
        $query_fields = mysqli_query($connect, $SQL);
        if (mysqli_num_rows($query_fields) == 0) {
            header('Content-Type: application/json');
            $listaArquivos["Err"] = array();
            $listaArquivos["Err"]["code"] = -2;
            $listaArquivos["Err"]["desc"] = "Nao encontrados campos de arquivo";
            echo json_encode($listaArquivos);
            return;
        }
        $list_fields = mysqli_fetch_all($query_fields, MYSQLI_ASSOC);
        $fieldsFD = array_column($list_fields, "fieldid");
        $sFieldsFD = implode(", ", $fieldsFD);
    } else {
        $sFieldsFD = PegaFieldIdByCode($ProcId, $fieldCode);
    }

    $listaArquivos = array();
    $listaArquivos["NumFiles"] = 0;

    $SQL = "select procid, fieldId, casenum, valueid, fieldValue from casedata where casenum = $caseNum and fieldId in ($sFieldsFD)";
    $query_arquivos = mysqli_query($connect, $SQL);
    $aArquivos = mysqli_fetch_all($query_arquivos, MYSQLI_ASSOC);

    $arquivos = array();
    foreach ($aArquivos as $item) {
        $dadosArquivo = $item;
        unset($dadosArquivo["fieldValue"]);
        $fileData = json_decode($item["fieldValue"], true);
        $dirName = criaDiretorioCampoArquivo($ProcId, $caseNum, $item["fieldId"]);
        $dadosArquivo["storageSource"] = $dirName . $fileData["fileNameStorage"];
        $dadosArquivo["fileName"] = $fileData["fileName"];
        $arquivos[] = $dadosArquivo;
    }

    $listaArquivos["NumFiles"] = count($arquivos);
    $listaArquivos["arquivos"] = $arquivos;

    header('Content-Type: application/json');
    $retorno = json_encode($listaArquivos);
    echo $retorno;
}

/**
 *  ROTA api/datahora
 */
function apiDataHora()
{
    header('Content-Type: application/json');
    $retorno = array();
    $retorno["data"] = date("Y-m-d");
    $retorno["hora"] = date("h:i");
    $retorno["dataHora"] = date("Y-m-d H:i");
    $retorno["fData"] = date("d/m/Y");
    $retorno["fDataHora"] = date("d/m/Y H:i");
    $retorno = json_encode($retorno);
    echo $retorno;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Envio de Arquivo">

/**
 * 
 * @global type $connect
 * @global type $OrigemLogon
 * @return string
 * 
 * ROTA: api/v1/createcase
 */
function apicasestart_V1()
{
    global $connect, $OrigemLogon;
    $dadosEntrada = apiTrataDadosEntrada();

    $procId = PegaProcIdByCode($dadosEntrada["procCode"]);

    $authUser = $dadosEntrada["userData"];
    $userdef = new userdef;
    $userdef->Create($OrigemLogon, $connect, $authUser);


    $CaseNum = GetNewCase($procId, $userdef);

    $Fields = converteFieldsParaCaseData($dadosEntrada["caseData"]);

    $dadosCaso = array();
    $dadosCaso["Fields"] = array();

    foreach ($Fields as $campo) {
        if (!is_array($campo["fieldValue"])) {
            $dadosCaso["Fields"][] = $campo;
        } else {
            $camposArquivo[] = $campo;
        }
    }

    $dadosImagens = criarArquivosEmCampo($procId, $dadosEntrada["caseNum"], $camposArquivo);

    $Fields = array_merge($dadosCaso["Fields"], $dadosImagens);

    $dadosEntrada["caseData"]["Fields"] = $Fields;

    $novoCaseNum = FuncCaseSave($dadosEntrada["userData"], $procId, $dadosEntrada["stepCode"], $CaseNum, $dadosCaso, 0, 1);

    $dadosRetorno["caseNum"] = $novoCaseNum;
    header("Access-Control-Allow-Origin: *");
    header('Content-Type: application/json');
    echo json_encode($dadosRetorno);
}

// ROTA "api/casestartunique"
function apicasestartunique()
{
    try {
        $dadosEntrada = apiTrataDadosEntrada();
        $procId = PegaProcIdByCode($dadosEntrada["proccode"]);

        $camposUniqueClose = PegaCampoUniqueClose($procId);
        $fieldUnique = $camposUniqueClose["FieldUnique"];

        $fields = $dadosEntrada["casedata"]["Fields"];

        //error_log(var_export($fields, true));

        $valorCampoUnique = buscaValorCampoUnique($procId, $fields, $fieldUnique);

        $numeroCasoExiste = pegaNumerCasoUnicoAberto($procId, $valorCampoUnique);

        if ($numeroCasoExiste == 0) {
            $novoCaseNum = CaseStart($dadosEntrada["userdata"], $dadosEntrada["proccode"], $dadosEntrada["stepcode"], $dadosEntrada["casedata"], $dadosEntrada["acao"]);
        } else {
            $novoCaseNum["CaseNum"] = 0;
            $novoCaseNum["Error"] = EncerraErros();
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
    header('Content-Type: application/json');
    echo $jDados;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="apicasestart">

/**
 * ROTA: "api/apicasestart"
 */
function apicasestart()
{
    $dadosEntrada = apiTrataDadosEntrada();
    $novoCaseNum = CaseStart($dadosEntrada["userdata"], $dadosEntrada["proccode"], $dadosEntrada["stepcode"], $dadosEntrada["casedata"], $dadosEntrada["acao"]);

    $dadosRetorno = $novoCaseNum;
    $jDados = json_encode($dadosRetorno);
    if (json_last_error() > 0) {
        $erroJSON = json_last_error() . " " . json_last_error_msg();
        error_log("Json erro: " . $erroJSON);
    }
    header('Content-Type: application/json');
    echo $jDados;
}

// </editor-fold>

function apicasedeadtime()
{
    $dadosEntrada = apiTrataDadosEntrada();

    if (!key_exists("casedata", $dadosEntrada)) {
        $dadosEntrada["casedata"] = "";
    }
    $novoCaseNum = CaseSave($dadosEntrada["userdata"], $dadosEntrada["proccode"], $dadosEntrada["stepcode"], $dadosEntrada["casenum"], $dadosEntrada["casedata"], $dadosEntrada["acaoprocessar"]);
    $dadosRetorno = $novoCaseNum;
    $dadosRetorno["dadosEntrada"] = $dadosEntrada;
    $jDados = json_encode($dadosRetorno);
    if (json_last_error() > 0) {
        $erroJSON = json_last_error() . " " . json_last_error_msg();
        error_log("Json erro: " . $erroJSON);
    }
    header('Content-Type: application/json');
    echo $jDados;
}

// </editor-fold>

function padronizaDadosEntrada($dadosEntrada)
{
    if (key_exists("proccode", $dadosEntrada)) {
        $dadosEntrada["procCode"] = $dadosEntrada["proccode"];
    }

    if (key_exists("stepcode", $dadosEntrada)) {
        $dadosEntrada["stepCode"] = $dadosEntrada["stepCode"];
    }

    if (key_exists("userdata", $dadosEntrada)) {
        $dadosEntrada["userData"] = $dadosEntrada["userdata"];
    }

    if (key_exists("casedata", $dadosEntrada)) {
        $dadosEntrada["caseDada"] = $dadosEntrada["casedata"];
    }

    if (key_exists("Acao", $dadosEntrada)) {
        $dadosEntrada["acao"] = $dadosEntrada["Acao"];
    }
    return $dadosEntrada;
}

function converteFieldsParaCaseData($fields)
{
    $keys = array_keys($fields);
    if (count($keys) == 1 & $keys[0] == "Fields") {
        $fields = $fields["Fields"];
    }
    $campos = array();
    foreach ($fields as $fieldCode => $fieldValue) {
        if (is_array($fieldValue)) {
            if (key_exists("fieldCode", $fieldValue)) {
                $fieldCode = $fieldValue["fieldCode"];
                $fieldValue = $fieldValue["fieldValue"];
            }
        }
        $campo["fieldCode"] = $fieldCode;
        $campo["fieldValue"] = $fieldValue;
        array_push($campos, $campo);
    }
    return $campos;
}

// ROTA: api/v1/validauser
function apiValidado()
{
    $dados["validado"] = "ok";
    header('Content-Type: application/json');
    echo json_encode($dados);
}

// ROTA "api/v1/casestart"
function casestart_v1()
{
    try {
//        error_log("Inicia apicasestartuniqueprocessa_v1");
        $dadosEntrada = apiTrataDadosEntrada();

        $dadosEntrada = padronizaDadosEntrada($dadosEntrada);

        $procId = PegaProcIdByCode($dadosEntrada["procCode"]);

        $Fields = converteFieldsParaCaseData($dadosEntrada["caseData"]);

        $dadosCaso = array();
        $dadosCaso["Fields"] = array();

        foreach ($Fields as $campo) {
            if (!is_array($campo["fieldValue"])) {
                $dadosCaso["Fields"][] = $campo;
            } else {
                $camposArquivo[] = $campo;
            }
        }

        $dadosImagens = criarArquivosEmCampo($procId, $dadosEntrada["caseNum"], $camposArquivo);

        $Fields = array_merge($dadosCaso["Fields"], $dadosImagens);

        $dadosEntrada["caseData"]["Fields"] = $Fields;


        $novoCaseNum = CaseStart($dadosEntrada["userData"], $dadosEntrada["procCode"], $dadosEntrada["stepCode"], $dadosEntrada["caseData"], $dadosEntrada["acao"]);
        $dadosRetorno = $novoCaseNum;
        $jDados = json_encode($dadosRetorno);
        if (json_last_error() > 0) {
            $erroJSON = json_last_error() . " " . json_last_error_msg();
            error_log("Json erro: " . $erroJSON);
        }
    } catch (Exception $ex) {
        $jDados = json_encode($ex);
    }
    header('Content-Type: application/json');
    echo $jDados;
}
