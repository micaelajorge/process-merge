<?php

include(FILES_ROOT . "include/template_work.inc");

function api_mergeTemplate()
{
    $dadosEntrada = apiTrataDadosEntrada();    
    $templateData = $dadosEntrada["data"];
    $templateFile = $dadosEntrada["file"];
    echo mergeTemplate($templateData, $templateFile);
}

function api_getTemplateData()
{
    $dadosEntrada = apiTrataDadosEntrada();    
    $templateFile = $dadosEntrada["file"];
    $templateData = $dadosEntrada["data"];
    $dataTemplate = getTemplateData($templateData,$templateFile);
    
    $jDados = json_encode($dataTemplate);    
     header('Content-Type: application/json');
    echo $jDados;
}