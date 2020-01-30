<?php //

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 12/06/2019
  Sistema: Process_XAMPP
 */

include_once(FILES_ROOT . "include/resource01.php");

function evaluate()
{
    $dadosEntrada = apiTrataDadosEntrada();
    
    $expressao = $dadosEntrada["expressao"];
    $dadosAvaliacao= $dadosEntrada["parametros"];
    
    $retorno = trataExpressao($expressao, $dadosAvaliacao);
    
    $jDados = json_encode($retorno);
    header("HTTP/1.0 202 Regra Ok");
    header('Content-Type: application/json');
    echo $jDados;    
}


function avaliaExpressoes($expressoes, $parametros)
{
    $dadosRetorno = array();    
    foreach ($expressoes as $expressaoAvaliada) {
        
        //Verifica se deve avaliar a expressão
        if ($expressaoAvaliada["evaluate_expression"] === "false")
        {
            continue;
        }        
//        error_log("Avaliando " . var_export($expressaoAvaliada, true));
        $dadosRetorno[] = trataExpressao($expressaoAvaliada, $parametros);
    }
    return $dadosRetorno;
}

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

    if (key_exists("AVALIAR_REGRA", $resultadoOperacao))
    {
        $resultadoRegra = avaliaRegraSelecionada($resultadoOperacao["AVALIAR_REGRA"], $dadosAvaliacao);
        
        $dadosRetorno = $resultadoRegra["dados"];
        
        $retorno = array();
        
        foreach($dadosRetorno as $dadoRetornado)
        {
            $retorno[] = $dadoRetornado;
        }        
//        if (!isset($resultadoOperacao["dados"]))
//        {
//            $resultadoOperacao["dados"] = array();
//        }
//        $dadosRegra = $resultadoRegra["dados"];
//        $resultadoOperacao["dados"] = array_merge($resultadoOperacao["dados"], $dadosRegra);
        
        
    }
    else {
        $retorno = array_merge(["operacao" => $listaOperadores["operacao"]], $resultadoOperacao);
    } 
            
    return $retorno;
}

function testarRegra()
{
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
    $SQL = "select rule_operations from rules where UCASE(rule_code) = UCASE('$ruleCode')";
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
