<?php

//use OpenApi\Annotations as OA;
use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_swagger.html");

$t_body->SITE_LOCATION = SITE_LOCATION;

$nomeArquivoJsonSwagger = $nome_projeto . "_" . $versao_api . ".json";

if (!file_exists(FILES_ROOT . "assets/swagger/" . $nomeArquivoJsonSwagger))
{
    $nomeArquivoJsonSwagger = "404.json";
}

$t_body->json_swagger = $nomeArquivoJsonSwagger;

$t_body->show();
