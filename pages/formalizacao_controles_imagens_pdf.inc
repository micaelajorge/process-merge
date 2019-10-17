<?php

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_campo_if_controles.html");

$dadosEntrada = apiTrataDadosEntrada();

$totalPaginas = $dadosEntrada["totalPaginas"];
$id_imagem = $dadosEntrada["id_imagem"];
$inicioContador = $dadosEntrada["inicioContador"];
// Se for o primeiro PDF carrega
$primeiraImagem = $id_imagem == '_IMG1';
for ($contadorDocumentos = 1; $contadorDocumentos <= $totalPaginas; $contadorDocumentos++) {
    $NR_DOCUMENTO = $contadorDocumentos . $id_imagem;    
    $t_body->IMAGE_ONLOAD = "onload=\"jsHabilitaBotaoImagem('IMG$NR_DOCUMENTO', 'btn-success')\"";
    if ($primeiraImagem) {
        $t_body->IMAGE_ONLOAD = "onload=\"CarregaPrimeiraImagemFormalizacao()\"";
        $primeiraImagem = false;
    }
    $t_body->NR_IMAGEM = $inicioContador + $contadorDocumentos;
    $t_body->NRDOCUMENTO = $NR_DOCUMENTO;
    $t_body->block("BLOCO_CONTROLES_IMAGEM");    
}
$t_body->show();
