<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 28/07/2019
  Sistema: Process_XAMPP
 */


use raelgc\view\Template;
use Spipu\Html2Pdf\Html2Pdf;

require_once FILES_ROOT . '/vendor/autoload.php';

function geraHTML()
{
    echo htmlTemplate();
}

function geraPDF()
{
    $htmlGerado = htmlTemplate();
    $html2pdf = new Html2Pdf();
    $html2pdf->writeHTML($htmlGerado);
    $html2pdf->output();
}

function criaHtmlTemplate($template, $dadosTemplate, $baseTemplate = 'assets/templates/')
{
    
    $nomeArquivo = FILES_ROOT . "$baseTemplate" . "$template";
    
    if (!file_exists($nomeArquivo)) {
        $nomeArquivo = FILES_ROOT . "$template";
        if (!file_exists($nomeArquivo)) {
            header("HTTP/1.0 405 template file not found");
            return;
        }
    }

    $t_body = new Template($nomeArquivo);

    $listaVariaveisEnviadas = array_keys($dadosTemplate);

    foreach ($listaVariaveisEnviadas as $variavelEnviada) {
        $dadoTemplate = strtoupper($variavelEnviada);
        $t_body->$dadoTemplate = $dadosTemplate[$variavelEnviada];
    }

    $htmlGerado = $t_body->parse();
    
    return $htmlGerado;    
}

function htmlTemplate()
{
    $dadosEntrada = apiTrataDadosEntrada();

    $template = $dadosEntrada["template"];
    
    $htmlGerado = criaHtmlTemplate($template, $dadosEntrada);

    return $htmlGerado;
}
//echo $htmlGerado;
