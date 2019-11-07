<?php

/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 02/10/2019
Sistema: Process_XAMPP
*/

require_once(FILES_ROOT . "pages/viewfile.inc");

function pegaArquivoOfertaRetorno()
{
    global $ProcId, $CaseNum, $userdef;

    $procCode = PegaProcCode($ProcId);

    switch ($procCode)
    {
        case "ENVIO_CNAB_ACEITE":
            $fieldCode = "IMAGENS_INT";
            break;
        case "SEC_CNABS":
            $fieldCode = "ARQUIVO_CNAB";
            break;
    }
    
    $dadosCampo = pegaDadosCaso($ProcId, $CaseNum, [$fieldCode]);
    
    $json_dadosCampoArquivo = $dadosCampo[$fieldCode];
    
    $dadosCampoArquivo = json_decode($json_dadosCampoArquivo, true);
    
    $arquivoStorage = $dadosCampoArquivo["fileNameStorage"];
    $arquivoName = $dadosCampoArquivo["fileName"];
    
    $diretorio = FILES_UPLOAD . "/" . diretorioArquivoImagem($ProcId, $CaseNum, $fieldCode);
    
    insereEntradaAuditTrail($ProcId, $CaseNum, 0, $userdef->UserId, $userdef->UserDesc, 7, "$arquivoName");
    
    DownloadFile($arquivoStorage, $arquivoName, $diretorio);
}