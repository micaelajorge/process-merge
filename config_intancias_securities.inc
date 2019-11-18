<?php

switch ($aliasServidor) {
    case "homolog_teste":
        define("ALINHAMENTO_LOGO", "float:left");
        define("TEMASISTEMA", "skin-blue-light");
        /**
         *  Definições Banco de dados
         */
        $BPMSERVER = "MUDAR";
        $BPMUSER = "MUDAR";
        $BPMPWD = "MUDAR";
        $EXTERNALUSER = "MUDAR";
        $EXTERNALPWD = "MUDAR";
        $EXTERNAL_USERNAME = "MUDAR";
        $BPMDB = "MUDAR";
        $EXTERNALDB = 'MUDAR';
        
        // Diretorio onde será gravado o arquivo de LOG do PHP
        define("ARQUIVO_ERROR_LOG_PHP", "log_gateway_creditas_scd/PHP_errors.log");
        
        define("_SECRET_KEY", "f9QsJ4Oufz");
        define("ALLOW_SEARCH", FALSE);
        define("FILES_FOLDER", "/");
        
        // Diretorio para armazenamento dos arquivos enviados para o servidor
        define("FILES_UPLOAD", "/storage/gateway_creditas_scd");
        
        // Nome da Instancia
        define("INSTANCENAME", 'Homolog Creditas Sorocred');
        
        // Diretorio da instancia "ALIAS apache"
        define("SITE_FOLDER", "/gateway_creditas_scd/"); // Nome do Alias no APACHE

        define("SITE_FOLDER_COMPLEMENT", "");
        define("LOGO_PARCEIRO", "logo-certdox-final.jpg");
        define("PARCEIRONAME", 'Securities');
        define("ICONE_PARCEIRO", "icone-certdox-final.jpg");
        define("URL_OWNER", "https://www.certdox.com.br");
        define("ALINHAMENTO_LOGO", "float:left");
        define("NAME_OWNER", "Powered by Certdox");
        define("ALINHAMENTO_LOGO", "float:left");
        define("SITE_ROOT", "http://$servidor");
        define("SITE_PRINCIPAL_PAGE", "pages/entrada.inc");
        define("SERVER_ADDRESS", $servidor);
        break;
}