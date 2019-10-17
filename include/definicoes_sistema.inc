<?php

/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 28/10/2018
Sistema: Process_XAMPP
*/
define("DEFAULT_PAGE", "pages/logon.inc");

define("WS_PROCESSAR", 0);
define("WS_SALVAR", 1);
define("WS_ABRIR", 99);
define("WS_DEADSOFT", 3);
define("WS_DEADHARD", 4);
define("WS_DEADHARDEST", 5);
define("WS_ALTERFIELDVALUE", 6);
define("WS_MACROEXEC", 7);
define("WS_SAVEADMIN", 8);
define("WS_ADHOR", "adhoc");
define("WS_INGNORE", "ignorar");
define("WS_PUTSTEPADMIN", "putstepadmin");

define("SITE_LOCATION", SITE_FOLDER . SITE_FOLDER_COMPLEMENT);