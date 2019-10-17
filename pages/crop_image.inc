<?php

/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 03/11/2018
Sistema: Process_XAMPP
*/

function cropImagem($arquivo)
{
    $extencaoArquivo = strtolower(pathinfo($arquivo, PATHINFO_EXTENSION));
    switch ($extencaoArquivo)
    {
        case "jpg":
            $imagemOriginal = imagecreatefromjpeg($arquivo);
            break;
        case "png":
            $imagemOriginal = imagecreatefrompng($arquivo);
            break;
        default :
            return file_get_contents($arquivo);
            break;
    }
    
    if (!$imagemOriginal)
    {
        return false;
    }
    $cropped=imagecropauto($imagemOriginal, IMG_CROP_DEFAULT);    
    switch ($extencaoArquivo)
    {
        case "jpg":
            $retorno = imagejpeg($cropped);
            break;
        case "png":
            $retorno = imagepng($cropped);
            break;
    }
    return $retorno;
}
