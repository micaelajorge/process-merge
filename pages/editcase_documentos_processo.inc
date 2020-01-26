<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 28/02/2019
  Sistema: Process_XAMPP
 * 
 * @rota documentosprocesso
 */

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/resource01.php");
include_once(FILES_ROOT . "include/resource02.php");
include_once(FILES_ROOT . "pages/editcase_form.php");
include_once(FILES_ROOT . "include/bpmeditcasefuncs.inc");
include_once(FILES_ROOT . "pages/editcase_campos.inc");

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_documentos_processo.html");

$dadosEntrada = apiTrataDadosEntrada();

$procId = $dadosEntrada["procId"];
$stepId = $dadosEntrada["stepId"];
$caseNum = $dadosEntrada["caseNum"];
$acaoDocumentos =  $dadosEntrada["acaoDocumentos"];

if ($stepId == "")
{
    $stepId = 0;
}

// Carrega a lista de documentos do Processo
$documentosDisponiveis = pegaDocumentosProcesso($procId, $stepId, true);

$documentoPadrao = pegaPassoDocumentoDefault($procId);

// Se nao tiver documento padrão, desabilita a opção de Tipificação Automática
if (count($documentoPadrao) == 0)
{
    $t_body->BOTAO_CAPTURAR_DISABLED = "disabled";
}

// Habilitar para Tipificação automática
//$listaCompletaDocumentosDisponiveis = array_merge($documentoPadrao, $documentosDisponiveis);
$listaCompletaDocumentosDisponiveis = $documentosDisponiveis;

$listaTotaldocumentos = pegaTotalDocumentos($procId, $caseNum);

foreach ($listaCompletaDocumentosDisponiveis as $documento) {    
    //$t_body->BOX_DOCUMENTO_OPCIONAL = ($documento["optional"] == 0) ? "box-default" : "box-danger";    
    $t_body->DOCUMENTOOBRIGATORIO = ($documento["optional"] == 0) ? "true" : "false";
    $t_body->CLASS_BOTAO_IMAGENS = "btn-default";
    $t_body->NOMEDOCUMENTO = $documento["StepName"];
    $t_body->PROCDOCID = $documento["ProcId"];
    $t_body->CASENUM_PAI = $caseNum;
    $t_body->DOCID = $documento["StepId"];
    $t_body->PROCID = $procId;        
    $t_body->DOCUMENT_CODE = $documento["StepCod"];
    $colunas = array_column($listaTotaldocumentos, "stepid");
    $indiceDocumento = array_search($documento["StepId"], $colunas);
    
    if ($acaoDocumentos !== "Edit")
    {
        $t_body->HIDE_EDIT = "hide";
    }
    
    $t_body->BOTAO_VIEW_IMAGEM_DISABLED = "";
    if ($indiceDocumento === false)
    {
        $t_body->BOTAO_VIEW_IMAGEM_TITLE = "Não há imagens para visualizar";
        $totalDocumentos = 0;
        $t_body->BOTAO_VIEW_IMAGEM_DISABLED = "disabled";
    } else 
    {
        $t_body->BOTAO_VIEW_IMAGEM_TITLE = "Visualizar Documentos";
        $t_body->BOTAO_VIEW_IMAGENS_TEXT = "Visualizar Documentos";
        $totalDocumentos = $listaTotaldocumentos[$indiceDocumento]["total_documentos"];
    }
    $t_body->TOTAL_IMAGENS_DOCUMENTO = $totalDocumentos;

    $classePanel = "";
    if ($documento["optional"] == 0)
    {
        if ($totalDocumentos == 0)
        {
            $classePanel = "box-warning box-solid";
        } else {
            $classePanel = "box-success box-solid";
        }        
    } else {
        if ($totalDocumentos > 0)
        {
        $classePanel = "box-success";
        }                
    }
    $t_body->TIPOBOXDOCUMENTOOPCIONAL = $classePanel;    
    // Seta o documento padrão para não tipificados
    if ($documento["DefaultView"] == 1) {
        $t_body->DOCID_DEFAULT = $documento["StepId"];
    }
    $t_body->block("BLOCO_DOCUMENTO");
}

$t_body->show();
