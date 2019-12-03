<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 30/11/2019
  Sistema: Process
 */

// <editor-fold defaultstate="collapsed" desc="Chamadas externas">
require_once(FILES_ROOT . '/include/common.php');
require_once(FILES_ROOT . '/include/ldap.php');
require_once(FILES_ROOT . '/include/resource01.php');
require_once(FILES_ROOT . '/include/toolbar.inc');

// </editor-fold>

use raelgc\view\Template;

/**
 * 
 * @global type $connect
 * @param type $dadosSignatario
 * @return type
 */
function pegaNomeSignatario($dadosSignatario)
{
    global $connect;
    if (key_exists("SIGNATARIO_NOME", $dadosSignatario)) {
        $nomeSignatario = $dadosSignatario["SIGNATARIO_NOME"];
    } else {
        $procId_SEC_SIGNATARIOS = PegaProcIdByCode("SEC_SIGNATARIOS");
        $chaveAssinatura = $dadosSignatario["CHAVE_ASSINATURA"];
        $SQL = "select casenum from casedata where procid = $procId_SEC_SIGNATARIOS and fieldValue = '$chaveAssinatura'";
        $query = mysqli_query($connect, $SQL);
        $retornoQuery = mysqli_fetch_all($query, MYSQLI_ASSOC);
        $caseNumSignatario = $retornoQuery[0]["casenum"];
        $dadosCasoSignatario = pegaDadosCaso($procId_SEC_SIGNATARIOS, $caseNumSignatario, array('SIGNATARIO_NOME'));        
        $nomeSignatario = $dadosCasoSignatario["SIGNATARIO_NOME"];
    }
    return $nomeSignatario;
}

/**
 * 
 * @param type $t_body
 * @param type $jsonListaSignatarios
 * @param type $descEntidade
 */
function montaListaSignatarios($t_body, $jsonListaSignatarios, $descEntidade)
{
    $t_body->DESC_ENTIDADE = $descEntidade;
    $listaSignatariosEndorsee = json_decode($jsonListaSignatarios, true);

    foreach ($listaSignatariosEndorsee as $signatario) {
        $signatarioNome = pegaNomeSignatario($signatario);
        $t_body->NOME_SIGNATARIO = $signatarioNome;
        if ($signatario["ASSINADO"] === true) {
            $t_body->block("BLOCK_ASSINADO");
        } else {
            $t_body->block("BLOCK_N_ASSINADO");
        }
        $t_body->block("BLOCK_SIGNATARIO");
    }
    $t_body->block("BLOCK_ENTIDADE");
}

$t_body = new Template(FILES_ROOT . "assets/templates/securities/t_securities_cessao_status_assinatura.html");

$PROCID_SEC_CESSION = PegaProcIdByCode('SEC_CESSOES');
if ($sessionUUID !== "") {
    $SQL = "select casenum from exportkeys where Campo18 = '$sessionUUID' and procid = $PROCID_SEC_CESSION";
} else {
    $SQL = "select casenum from exportkeys where procid = $PROCID_SEC_CESSION order by casenum desc limit 1";
}
$Query = mysqli_query($connect, $SQL);
$dadosEncontrados = mysqli_fetch_all($Query, MYSQLI_ASSOC);
$dadosCessao = pegaDadosCaso($PROCID_SEC_CESSION, $dadosEncontrados[0]["casenum"], array('SIGNATARIOS_ENDORSEE', 'SIGNATARIOS_TRANSFEREE', 'CESSION_NUMBER', 'DATA_CESSAO'));

$t_body->CESSION_NUMBER = $dadosCessao['CESSION_NUMBER'];
$t_body->CESSION_DATE = ConvDate($dadosCessao['DATA_CESSAO']);

montaListaSignatarios($t_body, $dadosCessao['SIGNATARIOS_ENDORSEE'], 'Signatários Cedente');

montaListaSignatarios($t_body, $dadosCessao['SIGNATARIOS_TRANSFEREE'], 'Signatários Cessionário');

/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>

