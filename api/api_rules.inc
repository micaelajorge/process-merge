<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 12/06/2019
  Sistema: Process_XAMPP
 */

include_once(FILES_ROOT . "include/resource01.php");

function avaliaRegra($regras, $parametros)
{
    $dadosRetorno = array();
    for ($i = 0; $i < count($regras); $i++) {
        $dadosRetorno[] = trataRegra($regras[$i], $parametros);
    }
    return $dadosRetorno;
}

function trataRegra($listaRegras, $dadosAvaliacao)
{
    $codigoAvaliacao = "";
    foreach ($listaRegras["operacoes"] as $regra) {
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
                $dadosRegra = trataLogicaFuncao($regra, $dadosAvaliacao);
                break;
            case "comparar":
            default:
                $dadosRegra = trataLogicaSimples($regra, $dadosAvaliacao);
                break;
        }

        if (key_exists("logic", $regra)) {
            $logic = $regra["logic"];
        }
        $codigoAvaliacao .= "$bracktOpen $logic $dadosRegra $bracktClose";
    }
    $resultadoAvaliacao = false;
    
    // Se a avaliação está em branco retorna erro
    if ($codigoAvaliacao === "") {
        $resultadoAvaliacao = "false";
    } else {
        $codigoAvaliacao = '$resultadoAvaliacao' . " = ($codigoAvaliacao) ? 'true' : 'false';";
        try {
//        $test = eval($string);
            eval($codigoAvaliacao);
        } catch (Exception $ex) {
            
        }
    }
    if ($resultadoAvaliacao !== "false") {
        $retorno = $listaRegras["resultado"]['true'];
    } else {
        $retorno = $listaRegras["resultado"]['false'];
    }

    return $retorno;
}


function testarRegra()
{
    $dadosEntrada = apiTrataDadosEntrada();
    $parametros = $dadosEntrada["inputData"];
    $rule = $dadosEntrada["rules"];
    $resultadoAvaliacao = avaliaRegra($rule["regras"], $parametros);
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

function trataLogicaFuncao($regra, $dadosAvaliacao)
{
    $funcao = $regra["funcao"];
    $matches = array();
    $regex = "/!%(.*)?!%/m";
//    $regex = "/!{(.*?)}/m";
    $total = preg_match($regex, $funcao, $matches);
    $NOME_CAMPO = $matches[1];
    $comparador = "";
    if (key_exists("comparador", $regra)) {
        $comparador = $regra["comparador"];
    }
    if (key_exists("comparador", $regra)) {
        $valorComparar = "'" . $regra["valor"] . "'";
    }
    $dadosRegra = str_replace($matches[0], $dadosAvaliacao[$NOME_CAMPO], $funcao) . $comparador . $valorComparar;
    return $dadosRegra;
}

function trataLogicaSimples($regra, $dadosAvaliacao)
{
    $valorCampo = $dadosAvaliacao[$regra["campo"]];
    $comparador = $regra["comparador"];
        
    if (isset($regra["valor"]))
    {
        $valorComparar = $regra["valor"];
    }

    if (isset($regra["logicaCampo"]))
    {
        $valorComparar = $dadosAvaliacao[$regra["logicaCampo"]];
    }
    
    $dadosRegra = "'$valorCampo' $comparador '$valorComparar'";
    return $dadosRegra;
}

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
 * @global type $ruleCode
 * 
 * ROTA: api/v1/rules
 */
function avaliaRegra_v1()
{
    global $ruleCode;
    $dadosEntrada = apiTrataDadosEntrada();
    $json_regras = buscaDadosRegra($ruleCode);

    $json_regras = str_replace("\\\\", "\\", $json_regras);
    $rule = json_decode($json_regras, true);
    $resultadoAvaliacao = avaliaRegra($rule["regras"], $dadosEntrada);
    $dadosRetorno["resultado"] = trataRetornos($resultadoAvaliacao);
    $dadosRetorno["dados"] = $resultadoAvaliacao;
    $dadosRetorno["retornoMensagem"] = $rule["retornoMensagem"];
    if (!is_array($dadosRetorno)) {
        $dadosRetorno["resultado"] = false;
    }
    $jDados = json_encode($dadosRetorno);
    header('Content-Type: application/json');
    echo $jDados;
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
    $SQL = "select rule_operations from rules where rule_code = '$ruleCode'";
    $query = mysqli_query($connect, $SQL);
    $retorno = mysqli_fetch_all($query, MYSQLI_ASSOC);
    return $retorno[0]["rule_operations"];
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
            $resultado = false;
            break;
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
