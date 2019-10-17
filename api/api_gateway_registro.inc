<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 28/09/2019
  Sistema: Process_XAMPP
 */
include_once(FILES_ROOT . 'includes_ws/funcsws.php');
include_once(FILES_ROOT . "include/resource01.php");
include_once(FILES_ROOT . "includes_ws/wsuser.inc");
include_once(FILES_ROOT . "includes_ws/wscasestart.inc");
include_once(FILES_ROOT . "includes_ws/wserror.inc");
include_once(FILES_ROOT . "includes_ws/wscasesave.inc");
include_once(FILES_ROOT . "includes/resource01.inc");

function converteFieldsParaCaseData($fields)
{
    $keys = array_keys($fields);
    if (count($keys) == 1 & $keys[0] == "Fields") {
        $fields = $fields["Fields"];
    }
    $campos = array();
    foreach ($fields as $fieldCode => $fieldValue) {
        $campo["fieldCode"] = $fieldCode;
        $campo["fieldValue"] = $fieldValue;
        array_push($campos, $campo);
    }
    return $campos;
}

/**
 * ROTA: api/v1/gateway_send_file_oferta
 */
function envioArquivoOferta()
{    
    $campoTipoArquivo = array("fieldCode" => "TIPO_ARQUIVO", "fieldValue" => 0);
    envioArquivoOfertaRetorno("ENVIO_CNAB_ACEITE", "IMAGENS_INT", "ENVIO", $campoTipoArquivo);
}

/**
 * ROTA: api/v1/gateway_send_file_retorno
 */

function envioArquivoRetorno()
{
    envioArquivoOfertaRetorno("SEC_CNABS", "ARQUIVO_CNAB", "START");
}

function envioArquivoOfertaRetorno($procCode, $fieldCode, $stepCode, $campoTipoArquivo = "")
{
    global $connect;
    try {
        // Pega o ProcId do Caso
        $procId = PegaProcIdByCode($procCode);

        $uploadFile_filename = $_FILES["files"]["name"][0];

        $fields = array();
        $fields[] = array("fieldCode" => "$fieldCode", "fieldValue" => $_FILES);
        $fields[] = array("fieldCode" => "NOME_ARQUIVO", "fieldValue" => $uploadFile_filename);

        // Se tem tipo de Arquivo Inclui
        if (is_array($campoTipoArquivo ))
        {
            $fields[] = $campoTipoArquivo;
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

        foreach ($fields as $campo) {
            if (!is_array($campo["fieldValue"])) {
                $dadosCaso["Fields"][] = $campo;
            } else {
                $camposArquivo[] = $campo;
            }
        }

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
            $dadosImagens = criarArquivosEmCampo($procId, $numeroCaso, $camposArquivo);

            $dadosCaso["Fields"] = array_merge($dadosCaso["Fields"], $dadosImagens);

//            error_log("Fields" . var_export($dadosCaso, true));

            $novoCaseNum["caseNum"] = FuncCaseSave($dadosEntrada["userData"], $procId, $stepCode, $numeroCaso, $dadosCaso, 0, 1);
            $novoCaseNum["error"] = array();

            //$novoCaseNum = CaseStart($dadosEntrada["userData"], $dadosEntrada["procCode"], $dadosEntrada["stepCode"], $dadosEntrada["caseData"], $dadosEntrada["acao"]);
        } else {
            header("HTTP/1.0 403 Error");
            $novoCaseNum["caseNum"] = 0;
            $novoCaseNum["Error"] = "Arquivo duplicado ou enviado anteriormente";
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

//
//$uploadFile_nomeTemp = $_FILES["files"]["tmp_name"][0];
//$uploadFile_filename = $_FILES["files"]["name"][0];
//
//$uploadFile_Destino = FILES_UPLOAD . "/" . $uploadFile_filename;
//
//$resultado = move_uploaded_file($uploadFile_nomeTemp, $uploadFile_Destino);
//error_log("Resultado, $uploadFile_Destino Upload: $resultado");
//
//echo "Resultado, $uploadFile_Destino Upload: $resultado";
//header("HTTP/1.0 403 Error");
//return;
}
