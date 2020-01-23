<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 08/01/2020
  Sistema: Process
 */

use raelgc\view\Template;

function iniciaScript()
{
    $t_body = new Template(FILES_ROOT . "assets/templates/t_endsession.html");
    $t_body->SITE_LOCATION = SITE_LOCATION;
    return $t_body;
}

function paginaDuplicada()
{
    $t_body = iniciaScript();
    $t_body->PAGE_TITLE = "Sessão Duplicada";
    $t_body->block("BLOCK_SESSAO_DUPLICADA_TITULO");
    $t_body->block("BLOCK_SESSAO_DUPLICADA_TEXTO_AUXILIAR");
/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
    $t_body->show();
// </editor-fold>            
}

function cessaoAlterada()
{
    $t_body = iniciaScript();
    $t_body->PAGE_TITLE = "Sessão Modificada";
    $t_body->block("BLOCK_SESSAO_MODIFICADA_TITULO");
    $t_body->block("BLOCK_SESSAO_MODIFICADA_TEXTO_AUXILIAR");
/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
    $t_body->show();
// </editor-fold>            
}

function paginaEncerrada()
{
    $t_body = iniciaScript();
    $t_body->PAGE_TITLE = "Sessão Encerrada";
    $t_body->block("BLOCK_SESSAO_ENCERRADA_TITULO");
    $t_body->block("BLOCK_SESSAO_ENCERRADA_TEXTO_AUXILIAR");
/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
    $t_body->show();
// </editor-fold>            
}
