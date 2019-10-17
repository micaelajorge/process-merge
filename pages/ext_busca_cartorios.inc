<?php

include_once(FILES_ROOT . 'include/func_api.inc');

use raelgc\view\Template;

function pegaListaDominios()
{
    global $connect;

    $sql = 'select origem_doc from origemdominio group by origem_doc order by origem_doc';
    $query = mysqli_query($connect, $sql);

    $retorno = mysqli_fetch_all($query, MYSQLI_ASSOC);
    return $retorno;
}

/**
 * 
 * @global type $FieldCod
 * @param type $FieldId
 * @param type $ReadOnly
 * @param type $Valor_Campo
 * @param type $dadosCampo
 * @return type
 */
function Inicializacao_ext_busca_cartorios($FieldId, $ReadOnly, $Valor_Campo, $dadosCampo = "", $tipoCampoFormulario = 't')
{
    global $FieldCod;
    $template = new Template(FILES_ROOT . 'assets/templates/t_ext_busca_cartorios.html');

    if ($Valor_Campo == "") {
        $template->BTN_VISUALIZAR_DISABLED = 'disabled';
    } else {
        $template->VALOR_CAMPO = $Valor_Campo;        
        $template->VALOR_DISPLAY = PegaValorCampo('CARTORIOS_GATEWAY_REGISTROS', $Valor_Campo, 'cartorio_nome');
    }

    if ($ReadOnly == 1) {
        $template->BTN_SELECIONAR_DISABLED = 'disabled';
    }
    $template->FIELDCODE = $FieldCod;
    $template->FIELDID = $FieldId;
    $template->FIELD_ID = $FieldId;
//    $listaClientes = pegaListaDominios($Valor_Campo);
    $template->block('BLOCO_VIEW_FIELD');

    $template->ARQUIVO_JS = 'ext_busca_cartorios.js';
    $template->block('BLOCO_JAVASCRIPT');

    return $template->parse();
}

/**
 * Carrega o formulario de seleção de Cartórios
 * 
 * ROTA: buscacartorios
 */
function CarregaModalBuscaCartorios()
{
    $dadosEntrada = apiTrataDadosEntrada();

    $pracaSelecionada = $dadosEntrada["pracaSelecionada"];
    $cartorioSelecionado = $dadosEntrada["cartorioSelecionado"];

    $template = new Template(FILES_ROOT . 'assets/templates/t_ext_busca_cartorios.html');

    $template->PRACA_SELECIONADA = $pracaSelecionada;

    if ($cartorioSelecionado == '') {
        $template->PRACA_BUSCA = $pracaSelecionada;
    }

    $template->block('BLOCO_MODAL_FIELD');
    echo $template->parse();
}

/**
 * ROTA: listacartorios
 * 
 * @return type
 */
function listaCartorios()
{
    require_once FILES_ROOT . "include/resource01.php";
    $dadosEntrada = apiTrataDadosEntrada();

    if (!isset($ProcSource)) {
        if (!key_exists("where", $dadosEntrada)) {
            return;
        }
        if (!key_exists("ProcSource", $dadosEntrada["where"])) {
            return;
        }
        $ProcSource = $dadosEntrada["where"]["ProcSource"];
    }

    if (!isset($FieldSource)) {
        if (!key_exists("where", $dadosEntrada)) {
            return;
        }
        if (!key_exists("FieldSource", $dadosEntrada["where"])) {
            return;
        }
        $FieldSource = $dadosEntrada["where"]["FieldSource"];
    }

    if (!key_exists("pracaSelecionada", $dadosEntrada["where"])) {
        return;
    }
    $pracaBusca = $dadosEntrada["where"]["pracaSelecionada"];

    $inicio = $dadosEntrada["start"];

    $listaCasos = pegaListaCartorios($ProcSource, $FieldSource, $pracaBusca, $inicio);

    $aResultado = $listaCasos["listaRegistros"];
    $totalTabela = $listaCasos["totalRegistros"];
    $registrosFiltrados = count($aResultado);
    $dadosRetorno = apiTrataDadosSaida($dadosEntrada, $aResultado, $registrosFiltrados, $totalTabela);
    $jDados = json_encode($dadosRetorno);

    header('Content-Type: application/json');
    echo $jDados;
}

function pegaListaCartorios($ProcSource, $FieldSource, $pracaBusca, $inicio)
{
    global $connect;
    if (!is_numeric($ProcSource)) {
        $ProcSource = PegaProcIdByCode($ProcSource);
    }

    if (!is_numeric($FieldSource)) {
        $FieldSource = PegaFieldIdByCode($ProcSource, $FieldSource);
    }


    $SQL = "select casenum, Campo10 as cartorio_nome, Campo32 as cidade from exportkeys where procid = $ProcSource and Campo32 like '%$pracaBusca%' order by Campo10";

    //$SQL .= " limit $inicio, $limite ";
    $Query = mysqli_query($connect, $SQL);
// <editor-fold defaultstate="collapsed" desc="Posiciona o Registro para paginacao">
    $inicioPagina = $filtros["paginacaoinicio"];
    if ($inicioPagina != 0) {
        if (!mysqli_data_seek($Query, $inicioPagina)) {
            //error_log("Cannot seek to row $inicioPagina: " . mysqli_error());
        }
    }
// </editor-fold>

    $limiteLinhas = $filtros["pagincacaolimite"];
    if (!isset($limiteLinhas) | $limiteLinhas == null) {
        $limiteLinhas = 10;
    }
    
    $contadorLinhas = 0;
    $contadorDados = 0;
    $aResultado = array();
    while ($linha = mysqli_fetch_array($Query, MYSQLI_ASSOC)) {
        if ($contadorLinhas < $inicio)
        {
            $contadorLinhas++;
            continue;
        }
        if ($contadorDados >= $limiteLinhas) {
            break;
        }
        $contadorLinhas++;
        $contadorDados++;
        array_push($aResultado, $linha);
    }
    $TotalRegistros = mysqli_num_rows($Query);
    $retorno["totalRegistros"] = $TotalRegistros;
    $retorno["listaRegistros"] = $aResultado;
    return $retorno;
}
