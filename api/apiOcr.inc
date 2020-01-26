<?php
/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 12/04/2019
  Sistema: Process_XAMPP
 */

// <editor-fold defaultstate="collapsed" desc="Definicoes e includes">
include_once(FILES_ROOT . "includes_ws/wsuser.inc");
include_once(FILES_ROOT . "includes_ws/wserror.inc");
require_once(FILES_ROOT . "includes_ws/wscasesave.inc");
require_once(FILES_ROOT . "includes_ws/wscasestart.inc");
include_once(FILES_ROOT . "include/resource01.php");
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="ocrServiceDefs">
/**
 * 
 * @rota api/ocrservicedefs
 * 
 * @global type $connect
 */
function ocrServiceDefs()
{
    global $connect;

    $SQL = "select procid, proccod, extendPropsProc "
            . " from "
            . " procdef"
            . " where "
            . " extendPropsProc like '%OCR%'";

    $query = mysqli_query($connect, $SQL);
    $processosOCR = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $listaProcessosOcr = array();
    
    foreach ($processosOCR as $item)
    {
        $processo = $item;
        unset($processo["extendPropsProc"]);
        $dataOcr = json_decode($item["extendPropsProc"], true);
        $processo["ocrdef"] = $dataOcr["OCR"];
        $listaProcessosOcr[] = $processo;
    }
        
    $retorno = $listaProcessosOcr;
    header('Content-Type: application/json');
    echo json_encode($retorno);
    
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Lista Documentos Tipificacao">
/**
 * 
 * @rota api/listadocumentostipificacao
 * 
 * @global type $connect
 */
function ocrListaDocumentosTipificacao()
{
    global $connect;
    $listaDocumentos = array();

    $SQL = "select casequeue.procid, casequeue.stepid, casequeue.caseid from 
    casequeue, 
    procdef, 
    stepdef 
    where 
    procdef.TipoProc = 'RP'
    and
    casequeue.procid = procdef.procid 
    and 
    stepdef.ProcId = procdef.procid 
    and
    stepdef.DefaultView = 1
    and
    casequeue.stepid = stepdef.stepid 
    limit 50";
    $query = mysqli_query($connect, $SQL);
    $listaDocumentos = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $retorno["dados"] = $listaDocumentos;
    header('Content-Type: application/json');
    echo json_encode($retorno);
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Lista documentos Submeter OCR">
/**
 * 
 * @rota api/listadocumentosocr
 * 
 * @global type $connect
 */
function ocrListaDocumentosOcr()
{
    global $connect;
    $listaDocumentos = array();

    $SQL = "select procid, extendPropsProc "
            . " from "
            . " procdef"
            . " where "
            . " extendPropsProc like '%OCR%'";

    $query = mysqli_query($connect, $SQL);
    $listaProcessosOCR = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $SQL_PROCSTEP = "";
    $SQL_CONCAT = "";
    foreach ($listaProcessosOCR as $itemOCR) {
        $objOCR = json_decode($itemOCR["extendPropsProc"], true);
        
        $ProcId = $itemOCR["procid"];
        
        $stepId = PegaStepIdByCode($ProcId, $objOCR["OCR"]["stepRun"]);
        
        if (is_numeric($ProcId) & is_numeric($stepId))
        {
            $SQL_PROCSTEP .= $SQL_CONCAT . "( procid = " . $ProcId . " and ";
            $SQL_PROCSTEP .= " stepid = " . $stepId . " ) ";
            $SQL_CONCAT = " or ";
        }            
    }

    $SQL = "select casequeue.procid, casequeue.stepid, casequeue.caseid "
            . "from "
            . "casequeue "
            . "where ";
    $SQL .= $SQL_PROCSTEP;
    $SQL .= " limit 50";

    $query = mysqli_query($connect, $SQL);
    $listaDocumentos = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $retorno["numItens"] = count($listaDocumentos);
    $retorno["dados"] = $listaDocumentos;
    header('Content-Type: application/json');
    echo json_encode($retorno);
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Dados Auto Classificacao">
/**
 * 
 * @rota api/dadosautoclassificacao
 * 
 * @global type $connect
 */
function ocrDadosAutoClassificacao()
{
    global $connect;
    $SQL = "select * from autoclassificacao ";
    $query = mysqli_query($connect, $SQL);
    $listaDocumentos = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $retorno["dados"] = $listaDocumentos;
    header('Content-Type: application/json');
    echo json_encode($retorno);
}
// </editor-fold>