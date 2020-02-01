<?php

//use OpenApi\Annotations as OA;
use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_rules_swagger_data.html");


$sql = 'select * from rules where rule_operations like \'%"public_rule"%\'';
$query = mysqli_query($connect, $sql);

$regrasPublicas = mysqli_fetch_all($query, MYSQLI_ASSOC);

$rulesUrl = SRCACCESS . "/" .ALIAS_SERVIDOR;
$t_body->RULES_URL = $rulesUrl;


$t_body->RULE_COMMA = "";    
$t_body->DEFINITION_COMMA = "";
foreach ($regrasPublicas as $regra)
{
    $t_body->RULE_CODE = $regra["rule_code"];
    $t_body->RULE_NAME = $regra["rule_name"];
    
    // Pega dos dados de Documentação
    $json_operation = $regra["rule_operations"];    
    $operations = json_decode($json_operation, true);    
    $documentacaoRegra = $operations["doc"];    
    $camposDocumentacao = array_keys($documentacaoRegra);
    
    // Pega os dados de Exemplo
    $json_input = $regra["rule_input"];    
    $examples = json_decode($json_input, true);    
    
    // Limpa o campo de VIRGULA para inicio Definições
    $t_body->FIELD_COMA = "";    
    foreach($camposDocumentacao as $campo)
    {
        $fieldType = $documentacaoRegra[$campo]["fieldType"];
        
        switch ($fieldType)
        {
            case "date":
                $t_body->FIELD_FORMAT = "YYYY-MM-DD";
                $t_body->block("BLOCK_FORMAT");
                break;
            case "int":
                $t_body->FIELD_FORMAT = "int64";
                $t_body->block("BLOCK_FORMAT");
                break;
            case "string":
                $t_body->FIELD_SIZE = $documentacaoRegra[$campo]["fieldSize"];
                $t_body->block("BLOCK_SIZE");
                break;
        }
        
        $t_body->FIELD_NAME = $campo;
        $t_body->FIELD_TYPE = $fieldType;
        $t_body->FIELD_EXAMPLE = $examples[$campo];

        
        $t_body->block("BLOCK_FIELD");
        $t_body->FIELD_COMA = ",";
    }
    $t_body->block("BLOCK_DEFINITION");
    $t_body->block("BLOCK_REGRA");        
    
    // Seta a virgula para divisão de Operações
    $t_body->RULE_COMMA = ","; 
}

$t_body->show();
