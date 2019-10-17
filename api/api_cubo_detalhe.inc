<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 30/01/2019
  Sistema: Process_XAMPP
 */

$dadosEntrada = apiTrataDadosEntrada();

$SQL = "select exportkeysdisplay.procid, exportkeysdisplay.casenum, campoDisplay8, campoDisplay9, campoDisplay34, campoDisplay5, campo15 as campoDisplay15, campoDisplay34, campoDisplay35, campoDisplay37, campoDisplay36, campoDisplay17
 from exportkeysdisplay, exportkeys where exportkeysdisplay.procid = 75 and exportkeysdisplay.procid = exportkeys.procid and exportkeysdisplay.casenum = exportkeys.casenum and campoDisplay13 = '$filtro'";
$Query = mysqli_query($connect, $SQL);

$detalhes = mysqli_fetch_all($Query, MYSQLI_ASSOC);
$dadosRetorno = apiTrataDadosSaida($dadosEntrada, $detalhes, $TotalRegistros, $TotalRegistros);

$jDados = json_encode($dadosRetorno);
header('Content-Type: application/json');
echo $jDados;
