<?php

//use OpenApi\Annotations as OA;
use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_swagger_2.html");

$t_body->SITE_LOCATION = SITE_LOCATION;

$nomeArquivoJsonSwagger = $nome_projeto . "_" . $versao_api . ".json";

// Pega a lista de APIS disponibilizadas para a Instância.
$apisDisp = INSTANCE_APIS;

if (!file_exists(FILES_ROOT . "assets/swagger/" . $nomeArquivoJsonSwagger))
{
    $t_body->nome_projeto = "End Points";
    foreach ($apisDisp as $fileDef)
    {
        $json_defApi = file_get_contents("assets/swagger/${fileDef}");
        $defApi = json_decode($json_defApi, true);
        $t_body->END_POINT_GROUP_DESC = $defApi["description"];
        
        foreach ($defApi["endPoints"] as $item)
        {
            $t_body->ENDPOINT_DESC = $item["description"];
            $t_body->ENDPOINT_URL = $item["url"];
            $t_body->block("BLOCK_END_POINT");
        }
    }
        
    $t_body->block("BLOCK_LIST_API");
} else {
    $t_body->nome_projeto = $nome_projeto;
    $t_body->json_swagger = $nomeArquivoJsonSwagger;    
    $t_body->block("BLOCK_SWAGGER_DEF");
}


$t_body->show();
