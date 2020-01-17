<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 03/06/2018
  Sistema: Creditus
 */

/**
 * 
 * @param type $dadosOrigem
 * @param type $camposBusca
 * @return type
 */
function api_busca_dados_campo($dadosOrigem, $camposBusca, $retornaFalha = false)
{
    $keys_source = array_keys($dadosOrigem);
    
    // Verifica se é um array
    if (!is_array($camposBusca))
    {
        $campoExistente = $camposBusca;
        $camposBusca = array();
        $camposBusca[] = $campoExistente;
    }
    foreach ($keys_source as $key)
    {
        foreach ($camposBusca as $campo)
        {
            if (strtolower($key) == strtolower($campo))
            {
                return $dadosOrigem[$key];
            }
        }
    }
        
    if ($retornaFalha)
    {
        api_send_falta_parametros();
    }
    return false;
}

/**
 * 
 */
function api_send_falta_parametros($listaParametros = "")
{
    header('Content-Type: application/json');
    header("HTTP/1.0 422 Dados Insuficientes");
    
    $retorno["err"] ="Dados Insuficientes";
    if ($listaParametros != "")
    {
        $retorno["campos minimos"] = $listaParametros;
    }
    $json_retorno = json_encode($retorno);
    echo $json_retorno;
    die();
}

function validaDadosEntrada($dadosRecebidos, $listaCamposObrigatorios, $camposComAlias = array())
{

    $keysEntrada = array_map("strtoupper", array_keys($dadosRecebidos));
    $listaCamposObrigatorios = array_map("strtoupper", $listaCamposObrigatorios);

    $temAlgum = count($camposComAlias) == 0;
    foreach ($camposComAlias as $chave) {
        $temAlgum = $temAlgum || in_array($chave, $keysEntrada);
    }

    $retorno = true && $temAlgum;

    foreach ($listaCamposObrigatorios as $chave) {
        $chaveExiste = in_array($chave, $keysEntrada);
        $retorno = $retorno && $chaveExiste;
    }
    return $retorno;
}

function insereItemArray(&$item, $key)
{
    $item["DT_RowId"] = "row_" . $key;
}

function insereTdDetalhe($aDados)
{
    array_walk($aDados, "insereItemArray");
    return $aDados;
}

/**
 * 
 * @param type $tabela
 * @param type $where clausula where para o filtro interno 
 * @return type
 */
function registrosTabela($tabela, $where = "")
{
    $comDB = AtivaDBProcess();
    $SQL = "select count(*) as total from"
            . " `$tabela` ";
    if ($where !== "") {
        $SQL .= " where $where";
    }
    $Query = mysqli_query($comDB, $SQL);
    $results = mysqli_fetch_array($Query);
    $retorno = $results['total'];
    return $retorno;
}

/**
 * Retorna o numero de Registro com determinado filtro WHERE
 * @global type $comDB Conexão Banco de Dados
 * @param type $tabela Tabela que serao contados os registros
 * @param type $filtro Where para execução da query
 * @return type
 */
function registrosPesquisa($tabela, $filtro)
{
    $comDB = AtivaDBProcess();
    $SQL = "select count(*) as total from"
            . " `$tabela` "
            . " $filtro";

    $Query = $comDB->prepare($SQL);
    $Query->execute();
    $results = $Query->fetchAll(PDO::FETCH_ASSOC);
    return $results[0]['total'];
}

/**
 * 
 * @return type
 */
function get_request_body()
{
    // Pega todos dos dados do BODY da REQUISICAO
    $entityBody = file_get_contents('php://input');
    if (is_object($entityBody)) {
        $dadosBody = (array) $entityBody;
    } else {
        $dadosBody = json_decode($entityBody, true);
//        error_log("body " . $entityBody);
    }
    return $dadosBody;
}

/**
 * Trata os Dados de Entrada para requisições 
 * GET / POST usando REST API (Json)
 * @return type]
 */
function apiTrataDadosEntrada($retornarRota = true)
{

    // Pega os dados do BODY REQUEST quando os dados vem em json
    $dados = get_request_body();

    // Pega dados do Get
    $get = filter_input_array(INPUT_GET);

    // Pega dados do Post
    $post = filter_input_array(INPUT_POST);

    if (!is_array($dados)) {
        $dados = array();
    }

    if (!is_array($post)) {
        $post = array();
    }

    if (!is_array($get)) {
        $get = explode("=", $get);
        //$get = array();
    } else {
        // Remove o Elemento do GET se não for para trazer a rota
        if (!$retornarRota) {
            if (count($get) > 1) {
                $get = array_pop($get);
            } else {
                $get = array();
            }
        }
    }

    $dadosEntrada = array_merge($dados, $post, $get);

    foreach ($dadosEntrada as $chave => $valor) {
        $GLOBALS[$chave] = $valor;
    }

    return $dadosEntrada;
}

function TrataWhereColumns($where)
{
    return $where;
}

function TrataWhereSQL($where)
{
    $and = " where ";
    foreach ($where as $key => $val) {
        $where .= $and;
        $where .= "$key '$val'";
        $and = " and ";
    }
}

function TrataWhereGlobal($where)
{
    foreach ($where as $key => $val) {
        if ($key != "global") {
            $GLOBALS[$key] = $val;
        }
    }
}

/**
 * Trata os campos para FIltro / Limites para DATATABLE
 * @param type $dados
 * @return type
 */
function apiTrataCampos($dados, $completeData = true)
{

    $limite = false;
    $nomeColuna = false;
    $where = "";
    $limites = "";
    $order = "";

    //error_log(var_export($dados, true));
    if (is_array($dados)) {
        if (key_exists('order', $dados)) {
            $idColuna = $dados["order"][0]["column"];
            $nomeColuna = $dados["columns"][$idColuna]["data"];
            $Ordenacao = $dados["order"][0]["dir"];
        }

        $limite = 20;
        if (key_exists('length', $dados)) {
            $limite = $dados["length"];
        }

        $inicio = 0;
        if (key_exists('start', $dados)) {
            $inicio = $dados["start"];
        }

        $where = "";
        if (key_exists("where", $dados)) {
            if (is_array($dados["where"])) {

                // Se o Where tem global, seta os valores globais dos Parâmetros
                // Semelhante se viessem em "PUT / GET"
                if (array_key_exists("global", $dados["where"])) {
                    TrataWhereGlobal($dados["where"]);
                }
                if (array_key_exists("sql", $dados["where"])) {
                    $where = TrataWhereSQL($dados["where"]);
                }
                if (array_key_exists("columns", $dados["where"])) {
                    $where = TrataWhereColumns($dados["where"]["columns"]);
                }
            } else {
                if ($dados["where"] == "clearFilter") {
                    $where = "clearFilter";
                }
            }
        }
    }

    $search = "";
    if (key_exists("search", $dados)) {
        if (key_exists("value", $dados["search"])) {
            $search = $dados["search"]["value"];
        }
    }


    // TODO: Corrigir ordenação Colunas
    if ($nomeColuna !== false) {
        if (!array_key_exists("order", $dados)) {
            $order = "order by  $nomeColuna  ";
        } else {
            $order = array();
            $order["campo"] = $nomeColuna;
            $order["ordem"] = $Ordenacao;
        }
    }

    $limites = "";
    if ($limite !== false) {
        $limites = " LIMIT $inicio, $limite";
    }

    $retorno = array("where" => $where, "order" => $order, "limite" => $limites, "paginacaoinicio" => $inicio, "pagincacaolimite" => $limite, "search" => $search);
    return $retorno;
}

/**
 * Trata os Dados de Entrada para requisições GET / POST usando REST API (Json)
 * 
 * @param type $dadosEntrada
 * @param type $resultadoPesquisa
 * @param type $totalTabela
 * @param type $totalPesquisa
 * @return type
 */
function apiTrataDadosSaida($dadosEntrada, $resultadoPesquisa, $totalTabela, $totalPesquisa)
{
    $draw = "1";
    $dadosRetorno = "";
    if (!is_array($dadosEntrada)) {
        $dadosRetorno = $resultadoPesquisa;
    } else {
        if (key_exists("draw", $dadosEntrada)) {
            $draw = $dadosEntrada["draw"];
            $draw++;
            $dadosRetorno = array(
                "data" => $resultadoPesquisa,
                "draw" => "$draw",
                "recordsFiltered" => $totalPesquisa,
                "recordsTotal" => $totalTabela
            );
        } else {
            $dadosRetorno = $resultadoPesquisa;
        }
    }
    return $dadosRetorno;
}

function apiAllowCors()
{
    header('Access-Control-Allow-Origin: *');
    header("Access-Control-Allow-Credentials: true");
    header('Access-Control-Allow-Methods: GET, PUT, POST, DELETE, OPTIONS');
    header('Access-Control-Max-Age: 1000');
    header('Access-Control-Allow-Headers: Origin, Content-Type, X-Auth-Token , Authorization');
    header('Access-Control-Allow-Headers: x-requested-with, Content-Type, origin, authorization, accept, client-security-token');
}
