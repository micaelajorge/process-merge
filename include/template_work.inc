<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 24/11/2019
  Sistema: Process_XAMPP
 */

include(FILES_ROOT . "vendor/autoload.php");

use PhpOffice\PhpWord\Element\Table;


function _criaTabelaTemplate($dadosTabela)
{
    $tabela = "<table>";
    if (key_exists("headers", $dadosTabela)) {
        $tabela .= "<thead>";
        $tabela .= "<tr>";
        foreach ($dadosTabela["headers"] as $header) {
            $tabela .= "<th>$header</th>";
        }
        $tabela .= "</tr>";
        $tabela .= "</thead>";
    }
    if (key_exists("tableData", $dadosTabela)) {
        $tabela .= "<tbody>";
        foreach ($dadosTabela["tableData"] as $linha) {
            $tabela .= "<tr>";
            foreach ($linha as $coluna) {
                $tabela .= "<td>$coluna</td>";
            }
            $tabela .= "</tr>";
        }
        $tabela .= "</tbody>";
    }
    $tabela .= "</table>";
    return $tabela;
}

function criaTabelaTemplate($dadosTabela)
{
    $table = new Table(array('borderSize' => 12, 'borderColor' => 'green', 'width' => 6000, 'unit' => 'auto'));
    $table->addRow();
    $table->addCell(150)->addText('Cell A1');
    $table->addCell(150)->addText('Cell A2');
    $table->addCell(150)->addText('Cell A3');
    $table->addRow();
    $table->addCell(150)->addText('Cell B1');
    $table->addCell(150)->addText('Cell B2');
    $table->addCell(150)->addText('Cell B3');
    return $table;
}

function substituiVariavelTemplate($template, $chave, $valor)
{
    if (is_array($valor)) {
//        TODO: Tratar Tabelas em modelo
//        $tabelaGerada = criaTabelaTemplate($valor);
//        $template->setComplexBlock($chave, $tabelaGerada);
    } else {
        $template->setValue($chave, $valor);
    }
}

function populaTemplate($template, $templateData)
{
    // Popular os valores de campos do Template    
    if (!is_array($templateData)) {
        return;
    }

    $keys = array_keys($templateData);
    foreach ($keys as $key) {
        substituiVariavelTemplate($template, $key, $templateData[$key]);
    }
}

function mergeTemplate($templateData, $templateFile)
{
    $dadosRetorno = array();

    // Carrega o arquivo de Template
    $phpWord = new \PhpOffice\PhpWord\PhpWord();
    $templateDocument = $phpWord->loadTemplate($templateFile);

    // Popula os dados do Template
    populaTemplate($templateDocument, $templateData);

    // Salva o resultado do Template
    $arquivoResultadoTemporario = $templateDocument->save();

    $phpWord = \PhpOffice\PhpWord\IOFactory::load($arquivoResultadoTemporario);
    $htmlWriter = new \PhpOffice\PhpWord\Writer\HTML($phpWord);

    $tmpTemplateFileName = tempnam(sys_get_temp_dir(), 'template_docx');
    $htmlWriter->save($tmpTemplateFileName);

    $conteudoArquivoHtml = file_get_contents($tmpTemplateFileName);
//    unlink($tmpTemplateFileName);

    $matches = array();
    $totalDadosEncontrados = preg_match('/(?<=(<body>))(.|\n)+?(?=(<\/body>))/m', $conteudoArquivoHtml, $matches);

    $conteudoBody = "";
    if ($totalDadosEncontrados !== false) {
        $conteudoBody = $matches[0];
    }

    return $conteudoBody;
}

function getTemplateData($templateData, $templateFile)
{
    $dadosRetorno = array();

    // Carrega o arquivo de Template
    $phpWord = new \PhpOffice\PhpWord\PhpWord();
    $templateDocument = $phpWord->loadTemplate($templateFile);

    // Extrair as variaveis do template
    $dadosRetorno["variables"] = $templateDocument->getVariables();

    // Popula os dados do Template
    populaTemplate($templateDocument, $templateData);

    // Salva o resultado do Template
    $arquivoResultadoTemporario = $templateDocument->save();

    $phpWord = \PhpOffice\PhpWord\IOFactory::load($arquivoResultadoTemporario);
    $htmlWriter = new \PhpOffice\PhpWord\Writer\HTML($phpWord);

    $tmpTemplateFileName = tempnam(sys_get_temp_dir(), 'template_docx');
    $htmlWriter->save($tmpTemplateFileName);

    $conteudoArquivoHtml = file_get_contents($tmpTemplateFileName);
    unlink($tmpTemplateFileName);

    $matches = array();
    $totalDadosEncontrados = preg_match('/(?<=(<body>))(.|\n)+?(?=(<\/body>))/m', $conteudoArquivoHtml, $matches);

    if ($totalDadosEncontrados !== false) {
        $conteudoBody = $matches[0];
    }

    // Pega os dados do arquivo HTML
    $dadosRetorno["htmlData"] = base64_encode($conteudoBody);

    return $dadosRetorno;
}
