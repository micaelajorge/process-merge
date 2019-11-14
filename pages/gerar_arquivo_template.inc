<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 28/07/2019
  Sistema: Process_XAMPP
 */

require FILES_ROOT . '/vendor/autoload.php';

use raelgc\view\Template;
use Spipu\Html2Pdf\Html2Pdf;

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


function htmlTemplate()
{
    $dadosEntrada = apiTrataDadosEntrada();

    $template = $dadosEntrada["template"];

    $nomeArquivo = FILES_ROOT . "assets/templates/$template";
    if (!file_exists($nomeArquivo)) {
        $nomeArquivo = FILES_ROOT . "$template";
        if (!file_exists($nomeArquivo)) {
            header("HTTP/1.0 405 template file not found");
            return;
        }
    }

    $t_body = new Template($nomeArquivo);

    $listaVariaveisEnviadas = array_keys($dadosEntrada);

    foreach ($listaVariaveisEnviadas as $variavelEnviada) {
        $dadoTemplate = strtoupper($variavelEnviada);
        $t_body->$dadoTemplate = $dadosEntrada[$variavelEnviada];
    }

    $htmlGerado = $t_body->parse();
    return $htmlGerado;
}
//echo $htmlGerado;
