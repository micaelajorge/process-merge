<?php
/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 15/01/2019
Sistema: Process_XAMPP
*/
header('Content-Type: application/json');
echo file_get_contents(FILES_ROOT . "/atualizacao_automato.json");