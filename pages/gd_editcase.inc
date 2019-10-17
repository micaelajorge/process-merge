<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 26/06/2019
  Sistema: Process_XAMPP
 */

define("TEMPLATE_FIELDS", 't_editcase_fields.html');

// Esconde a opão de notificacoes
define("NOTIFICACOES_HIDE", "display:none");
define("TASKS_HIDE", "display:none");

define("FILE_CABECALHO_CAMPOS", "gd_editcase_cabecalho.html");
define("SHOW_ICON_TOOLBAR", "");

define("CLASSE_COLUNA_PROCESSO", "col-lg-10 col-lg-offset-1");
define("CLASS_COL_MASTER_EDITAR", "col-lg-6 col-lg-offset-3");
define("CLASS_TAMANHO_FORMULARIO", "col-lg-6 col-lg-offset-3");
define("CLASS_TAMANHO_CAMPO_FORMULARIO", "col-lg-12");

define("HIDE_CABECALHO_CASO", "hide");
define("CONTENT_HEADER_HIDEN", "hide");

include FILES_ROOT . "pages/editcase.inc";
