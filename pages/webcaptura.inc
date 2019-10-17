<?php

/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 17/12/2018
Sistema: Process_XAMPP
 
 ROTA: webcaptura
*/

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_webcaptura_formulario.html");

$_PROCESSOS = $_SESSION["S_Processos"];

foreach ($_PROCESSOS as $processo)
{
    if ($processo["TipoProc"] == "CT")
    {
        $t_body->PROCNAME = $processo["ProcName"];
        $t_body->PROCID = $processo["ProcId"];
        $t_body->block("BLOCK_PROCESSOS_CAPTURA");
    }
}
$t_body->show();
