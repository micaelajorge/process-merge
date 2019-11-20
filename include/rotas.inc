<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 01/06/2018
  Sistema: Creditus
 */

include_once(FILES_ROOT . "include/resource01.php");

/**
 * 
 * @param array $listaArquivosRotas
 */
function incluiRotasArquivo($listaArquivosRotas)
{
    $retornoRotas = [];
    foreach ($listaArquivosRotas as $arquivoRota) {
        $rotasArquivo = carregaArrayRotas($arquivoRota);
        if (is_array($rotasArquivo)) {
            $retornoRotas = array_merge($retornoRotas, $rotasArquivo);
        }
    }
    return $retornoRotas;
}

/**
 * Carrega as rotas existentes no sistema
 * 
 * @return type array com as rotas existentes
 */
function carregaArrayRotas($arquivoRotas = ARQUIVO_ROTAS)
{
    $arquivoRotasNome = FILES_ROOT . "assets/config/" . $arquivoRotas;
    $rotas_retorno = array();

    // Verifica se a rota existe
    if (!file_exists($arquivoRotasNome)) {
        return $rotas_retorno;
    }

    $conteudoArquivoRotas = file_get_contents($arquivoRotasNome);
    $rotasLidas = json_decode($conteudoArquivoRotas, true);

    // Verifica se o arquivo conseguiu ser parceado
    if (json_last_error()) {
        return $rotas_retorno;
    }
    
    // Verifica se é um array
    if (!is_array($rotasLidas))
    {
        return $rotasLidas;
    }
    
    // 
    if (key_exists("rotas", $rotasLidas)) {
        $rotas_retorno = $rotasLidas["rotas"];
    }
    if (key_exists("arquivos", $rotasLidas)) {
        $rotas_retorno = incluiRotasArquivo($rotasLidas["arquivos"]);
    }

    return $rotas_retorno;
}

function defineRotaPadrao($dadosURI)
{
    if (strpos($dadosURI, "api/") !== false) {
        $rotaEncontrada["page"] = "pages/naoencontradaapi.inc";
    } else {
        if (key_exists("UserDef", $_SESSION)) {
            $rotaEncontrada["page"] = SITE_PRINCIPAL_PAGE;
        } else {
            $rotaEncontrada["page"] = "pages/logon.inc";
        }
    }
    return $rotaEncontrada;
}

function _definePaginaAcesso($dadosURI, $rotas, $methodHTTP)
{
    $paginaAcesso = $dadosURI;

    $posicaoInterrogacao = strpos($dadosURI, "?");

    if ($posicaoInterrogacao) {
        $dadosURI = substr($dadosURI, 0, $posicaoInterrogacao);
    }

    // Verifica se é uma rota direta
    // Pega a URL Total
    $posicaoUltimaBarra = strlen($dadosURI);
    while ($posicaoUltimaBarra) {
        $paginaAcesso = substr($dadosURI, 0, $posicaoUltimaBarra);
        if (key_exists($paginaAcesso, $rotas[$methodHTTP])) {
            $rotaDefinida = true;
            break;
        }
        $posicaoUltimaBarra = strripos($paginaAcesso, "/");
    }
    return $paginaAcesso;
}

function existePagina($paginaAcesso, $rotas, $methodHTTP)
{
    if (key_exists("POST", $rotas)) {
        if (key_exists($paginaAcesso, $rotas[$methodHTTP])) {
            return true;
        }
    } else {
        if (key_exists($paginaAcesso, $rotas)) {
            if (key_exists($methodHTTP, $rotas[$paginaAcesso])) {
                return true;
            }
        }
    }
    return false;
}

function definePaginaAcesso($dadosURI, $rotas, $methodHTTP)
{
    $paginaAcesso = $dadosURI;

    $posicaoInterrogacao = strpos($dadosURI, "?");



    if ($posicaoInterrogacao) {
        $dadosURI = substr($dadosURI, 0, $posicaoInterrogacao);
    }

    if (existePagina($paginaAcesso, $rotas, $methodHTTP)) {
        return $paginaAcesso;
    }

    // Verifica se é uma rota direta
    // Pega a URL Total
    $posicaoUltimaBarra = strlen($dadosURI);
    while ($posicaoUltimaBarra) {
        $paginaAcesso = substr($dadosURI, 0, $posicaoUltimaBarra);
        if (existePagina($paginaAcesso, $rotas, $methodHTTP)) {
            break;
        }
        $posicaoUltimaBarra = strripos($paginaAcesso, "/");
    }
    return $paginaAcesso;
}

function geraParametrosRecebidosURL($paginaAcesso, $dadosURI, $rotaEncontrada)
{
    global $rotaPadrao;
    // se tem barra, trata os parâmetros
    $regexPaginaAcesso = "/" . str_replace("/", '\/', $paginaAcesso) . "/";
    $parametros = preg_replace($regexPaginaAcesso, "", $dadosURI, 1);
    $aParametrosURL = explode("/", $parametros);
    $posicaoInterrogacao = strpos($dadosURI, "?");
    if ($posicaoInterrogacao) {
        $itensURL = explode("?", $dadosURI);
        $paginaAcesso = $itensURL[0];
        if (count($itensURL) == 2) {
            $aParametrosURLTemporario = explode("&", $itensURL[1]);
        }
        // Remove os nomes dos valores vindos da URL
        for ($i = 0; $i < count($aParametrosURLTemporario); $i++) {
            $posIgual = strpos($aParametrosURLTemporario[$i], "=");
            $aParametrosURLTemporario[$i] = substr($aParametrosURLTemporario[$i], $posIgual + 1);
        }
        $aParametrosURL = array_merge(array($paginaAcesso), $aParametrosURLTemporario);
    }

    if (key_exists("parametros", $rotaEncontrada)) {
        $parametrosRota = $rotaEncontrada["parametros"];
        $parametrosObrigatorios = true;
        if (key_exists("obrigatorio-parametros", $rotaEncontrada)) {
            $parametrosObrigatorios = $rotaEncontrada["obrigatorio-parametros"];
        }

        // Se não for parametros com valor default
        // Verifica se os parametros necessários na Rota foram passados
        if (isset($parametrosRota[0]) & count($aParametrosURL) == 0 & $parametrosObrigatorios) {
            // Parametros da Rota não foram entrados, vai para rota default
            $rotaEncontrada = $rotaPadrao;
        } else {
            TrataParametrosRoda($aParametrosURL, $parametrosRota);
        }
    }

    return $aParametrosURL;
}

function defineRota($p_dadosURI, $methodHTTP)
{
    global $connect;
    // Rota usada quando não for encontrada uma rota definida
    $rotaPadrao = defineRotaPadrao($p_dadosURI);

    $rotas = carregaArrayRotas();
    if ($rotas == null) {
        echo "Erro - R001";
        header("HTTP/1.0 404 End Point Não Encontrado");
        die();
    }

    $dadosURI = $p_dadosURI;

    $paginaAcesso = definePaginaAcesso(strtolower($dadosURI), $rotas, $methodHTTP);

    if (!key_exists($paginaAcesso, $rotas)) {
        $rotaEncontrada = $rotaPadrao;
        return $rotaEncontrada;
    } else {
        if (key_exists("POST", $rotas)) {
            $rotaEncontrada = $rotas[$methodHTTP][$paginaAcesso];
        } else {
            $rotaEncontrada = $rotas[$paginaAcesso][$methodHTTP];
        }
    }

//    // Verifica se é uma URL amigavel    
//    $parametrosURL = array();
    // Gera os parametros através dos dados recebidos da URL
    geraParametrosRecebidosURL($paginaAcesso, $dadosURI, $rotaEncontrada);

    $validadoToken = validaUsuarioToken();

    $userdef = $_SESSION["userdef"];
    if ($userdef === null) {
        $userValidado = false;
    } else {
        $userValidado = $userdef->Validado;
    }

    $rotaRequerLogon = !isset($rotaEncontrada["logon"]);

    if (!$userValidado & ($rotaRequerLogon & !$validadoToken)) {
        error_log("Rota requer autenticacao: $dadosURI " . var_export($rotaEncontrada, true));
        error_log("Dados Sessao: " . var_export($_SESSION, true));
        header("HTTP/1.0 402 Usuario sem permissão ou não autenticado");
        return $rotaPadrao;
    }

    return $rotaEncontrada;
}

/**
 * Popula as variaveis globais usadas na rota
 * 
 * @param type $aParametrosURL - Array de Parametros vindos da URL
 * @param type $aParametrosRota - Array de Parametros definidos na Rota
 */
function TrataParametrosRoda($aParametrosURL, $aParametrosRota)
{
    if (count($aParametrosRota) == 0) {
        if (count($aParametrosURL) == 1) {
            return;
        }
        unset($aParametrosURL[0]);
        $aParametrosURL = array_values($aParametrosURL);
        $GLOBALS["PARAMETROS_ROTA"] = $aParametrosURL;
    } else {
        while (list($nomeParametroRota, $valorParametroRota) = each($aParametrosRota)) {
            if (is_integer($nomeParametroRota)) {
                $valorParametro = $aParametrosURL[$nomeParametroRota + 1];
                $nomeParametro = $valorParametroRota;
            } else {
                $nomeParametro = $nomeParametroRota;
                $valorParametro = $valorParametroRota;
            }
            $valorDecodificado = urldecode($valorParametro);
            $GLOBALS[$nomeParametro] = $valorDecodificado;
        }
    }
}

/**
 * 
 * @return type Dados tratados da rota
 */
function trataUrl()
{
    if (isset($_SERVER["REQUEST_URI"])) {
        $requestURI = $_SERVER["REQUEST_URI"];
    } else {
        if (isset($_SERVER["SCRIPT_URI"])) {
            $requestURI = $_SERVER["SCRIPT_URI"];
        } else {
            echo "Falha";
            die();
        }
    }


//$url = strtolower($requestURI);
    $url = $requestURI;

//    $posicaoComercial = strpos($url, "&");
//    if ($posicaoComercial > 0)
//    {
//        $url = substr($url, 0, $posicaoComercial);
//    }

    $siteAlias = SITE_ALIAS;
    if ($siteAlias != "/") {
        // Monta o Regex para substituir apenas a primira parte da URL que contem o nome da URL
        $siteFolder = "/" . str_replace("/", '\/', SITE_FOLDER) . "/i";
        $dadosUrl = preg_replace($siteFolder, "", $url, 1);
    } else {
        $dadosUrl = $url;
    }
    if ($dadosUrl[0] == "/") {
        $dadosUrl = substr($dadosUrl, 1);
    }
    return $dadosUrl;
}

// <editor-fold defaultstate="collapsed" desc="Desatualizado">
/**
 * 
 * @param type $dadosURI - URL completa 
 * @param type $action - GET / POST
 * @return type definição da Página definida na rota
 */
function _defineRota($p_dadosURI, $methodHTTP)
{

    // Rota usada quando não for encontrada uma rota definida
    $rotaPadrao = defineRotaPadrao($p_dadosURI);

    $rotas = carregaArrayRotas();

    if ($rotas == null) {
        echo "Erro - R001";
        header("HTTP/1.0 404 End Point Nao Encontrado");
        die();
    }

    $dadosURI = strtolower($p_dadosURI);
    $rotaDefinida = true;

    $posicaoPrimeiraBarra = strpos($dadosURI, "/");
    $posicaoInterrogacao = strpos($dadosURI, "?");

    $paginaAcesso = $dadosURI;

    // Verifica se é uma rota direta
    // Pega a URL Total
    $posicaoUltimaBarra = strlen($dadosURI);
    while ($posicaoUltimaBarra) {
        $paginaAcesso = substr($dadosURI, 0, $posicaoUltimaBarra);
        if (key_exists($paginaAcesso, $rotas[$methodHTTP])) {
            $rotaDefinida = true;
            break;
        }
        $posicaoUltimaBarra = strripos($paginaAcesso, "/");
    }

    // Verifica se é uma URL amigavel    
    $parametrosURL = array();
    if (!$posicaoPrimeiraBarra && !$posicaoInterrogacao && !$rotaDefinida) {
        $paginaAcesso = $dadosURI;
    } else {
        // se tem barra, trata os parâmetros
        if ($posicaoPrimeiraBarra) {

            $regexPaginaAcesso = "/" . str_replace("/", '\/', $paginaAcesso) . "/";
            $parametros = preg_replace($regexPaginaAcesso, "", $dadosURI, 1);
            $aParametrosURL = explode("/", $parametros);
            if (!$rotaDefinida) {
                $paginaAcesso = $parametrosURL[0];
            }
        }
        if ($posicaoInterrogacao) {
            $itensURL = explode("?", $dadosURI);
            $paginaAcesso = $itensURL[0];
            if (count($itensURL) == 2) {
                $aParametrosURLTemporario = explode("&", $itensURL[1]);
            }
            // Remove os nomes dos valores vindos da URL
            for ($i = 0; $i < count($aParametrosURLTemporario); $i++) {
                $posIgual = strpos($aParametrosURLTemporario[$i], "=");
                $aParametrosURLTemporario[$i] = substr($aParametrosURLTemporario[$i], $posIgual + 1);
            }

            $aParametrosURL = array_merge(array($paginaAcesso), $aParametrosURLTemporario);
        }
    }

    if (!key_exists($paginaAcesso, $rotas[$methodHTTP])) {
//        error_log("Não Encontrada: $paginaAcesso");
//        error_log("Rota Padrão " . var_export($rotaPadrao, true));
        $rotaEncontrada = $rotaPadrao;
        return $rotaEncontrada;
    } else {
        $rotaEncontrada = $rotas[$methodHTTP][$paginaAcesso];
    }


    if (key_exists("parametros", $rotaEncontrada)) {
        $parametrosRota = $rotaEncontrada["parametros"];

        $parametrosObrigatorios = true;
        if (key_exists("obrigatorio-parametros", $rotaEncontrada)) {
            $parametrosObrigatorios = $rotaEncontrada["obrigatorio-parametros"];
        }

        // Se não for parametros com valor default
        // Verifica se os parametros necessários na Rota foram passados
        if (isset($parametrosRota[0]) & count($aParametrosURL) == 0 & $parametrosObrigatorios) {
            // Parametros da Rota não foram entrados, vai para rota default
            $rotaEncontrada = $rotaPadrao;
        } else {
            TrataParametrosRoda($aParametrosURL, $parametrosRota);
        }
    }

// Verifica se a rota definida exige logon
    if (!isset($_SESSION["userdef"]) & !isset($rotaEncontrada["logon"]) & $methodHTTP != "POST") {
        $rotaEncontrada = array("page" => DEFAULT_PAGE, "funcao" => "");
        return $rotaEncontrada;
    }

// Verifica se o tipo do usuário logado tem acesso a Rota encontrada 
    if (isset($_SESSION["entidade"])) {
        $tipoUsuario = $_SESSION["entidade"]["entidade_tipo"];

        if (isset($rotaEncontrada["permissao"])) {
            $permissaoAcessoRota = $rotaEncontrada["permissao"];
            if ($tipoUsuario != $permissaoAcessoRota) {
                $rotaEncontrada = $rotaPadrao;
                return $rotaEncontrada;
            }
        }
    }
    return $rotaEncontrada;
}

// </editor-fold>