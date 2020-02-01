<?php

//use OpenApi\Annotations as OA;
use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_rules_swagger.html");

$t_body->SITE_LOCATION = SITE_LOCATION . "rules/swagger";
$t_body->RULES_URL_DATA = SITE_LOCATION . "rules/swagger/data";
$t_body->BASE_SWAGGER = " ";

$siteLocation = SITE_LOCATION;

$nomeArquivoJsonSwagger = "swagger_rules";

// Pega a lista de APIS disponibilizadas para a Instância.
$apisDisp = INSTANCE_APIS;
$t_body->nome_projeto = "Mecanismo Regras";

$t_body->json_swagger = $nomeArquivoJsonSwagger;
$t_body->block("BLOCK_SWAGGER_DEF");


$t_body->show();
