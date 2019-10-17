<?php
/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 03/06/2018
  Sistema: Process
    Mantido para compatibilidade com paginas anteriores
 * 
 */
include("toolbar.inc");
$botoesToolBar = definicaoToolBar($Menu);
for ($i = 0; $i < 6; $i++) {
    $acao[$i] = $botoesToolBar[$i]["acao"];
    $texto[$i] = $botoesToolBar[$i]["texto"];
    $textoDesc[$i] = $botoesToolBar[$i]["textoDesc"];
    $AcionaMenu[$i] = $botoesToolBar[$i]["AcionaMenu"];
    $Link[$i] = $botoesToolBar[$i]["Link"];
    $Link_Float[$i] = $botoesToolBar[$i]["Link_Float"];
    $Imagem[$i] = $botoesToolBar[$i]["Imagem"];
    $class_Icon[$i] = $botoesToolBar[$i]["class_Icon"];
    $Collapse[$i] =  $botoesToolBar[$i]["Collapse"];
    $Target[$i] = $botoesToolBar[$i]["Target"];
}