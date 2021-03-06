<?php

/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 24/10/2019
Sistema: Process_XAMPP
*/
/**
 * @rota 
 * 
 * @global type $portaAutomato
 */
function buscaStatusAutomato()
{
    global $portaAutomato;
    
    $curl_dados = curl_init("http://localhost:" . $portaAutomato);
 
    $headerEnvio = array('Content-Type: application/json');
    curl_setopt($curl_dados, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($curl_dados, CURLOPT_HTTPHEADER, $headerEnvio);
    header('Content-Type: application/json');
    $resultado = curl_exec($curl_dados);    
    echo $resultado;
}