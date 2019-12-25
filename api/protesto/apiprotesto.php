<?php

use OpenApi\Annotations as OA;
use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_swagger.html");
$t_body->block("BLOCK_CONTENT");

$t_body->show();

    function enviaProtesto()
    {
    }
