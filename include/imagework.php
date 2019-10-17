<?php
function CreateThumb($DirDestino, $filename)
	{
	// Get new sizes
	$NomeThumb = $DirDestino . "thumb_" . $filename;
	list($width, $height) = getimagesize($DirDestino . $filename);
	$newwidth = $width * $percent;
	
	$newwidth = 150;
	if ($newwidth > $width)
		{
		copy($filename, $NomeThumb);
		return $NomeThumb;	
		}
	
	$percent = $newwidth / $width;
		
	$newheight = $height * $percent;
	
	// Load
	$thumb = imagecreatetruecolor($newwidth, $newheight);
	$source = imagecreatefromjpeg($DirDestino . $filename);
	
	// Resize
	imagecopyresized($thumb, $source, 0, 0, 0, 0, $newwidth, $newheight, $width, $height);
	imagejpeg($thumb, $NomeThumb);
	return $NomeThumb;
	}		
	
function PegaImagensDiretorio($diretorio, $Tipo = 'jpg')
	{	
	$Imagens = array();
	if (file_exists($diretorio))
		{
		$d = dir($diretorio);
		while($entry =  $d->read()) 
			{
			if ($entry == "." || $entry == "..")
				{
				continue;
				}
			$extencion = substr($entry, strpos($entry, ".") + 1, strlen($entry));
			$PosThumb = strpos($entry, "thumb_");
			if ($extencion == $Tipo)
				{
				if ($PosThumb === false)
					{
					array_push($Imagens, $entry);	
					}
				}			
			}
		}	
	return $Imagens;
	}

function PegaThumbs($diretorio)
	{
	$Imagens = array();
	if (file_exists($diretorio))
		{
		$d = dir($diretorio);
		while($entry =  $d->read()) 
			{
			if ($entry == "." || $entry == "..")
				{
				continue;
				}
			$pos = strpos($entry, "thumb_");
			if ($pos === false)
				{
				continue;
				}
			array_push($Imagens, $entry);	
			}
		}	
	return $Imagens;		
	}
?>