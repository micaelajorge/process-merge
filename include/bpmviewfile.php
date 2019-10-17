<?php
// //Versao 1.0.0 /Versao
//require_once("include/common.php");
include("include/validasessao.php");
require_once("include/resource01.php");
require_once("include/class_mime.php");
$DirName = FormataCaseNum($ProcId,3) . FormataCaseNum($CaseNum,8);
$dir = "upload/$DirName/";
DownloadFile($Target, $dir);


function DownloadFile($filename, $dir) 
{ 
	$mime = new mime($filename);
    // Check filename 
    if (empty($filename) || !file_exists($dir . $filename)) 
    { 
        return FALSE; 
    } 
    // Create download file name to be displayed to user 
    $saveasname = basename($filename); 
    $ContentType = $mime->getMime();
    // Send binary filetype HTTP header 

    header('Pragma: cache');
    header('Cache-Control: private');
    header('Content-Type: ' . $mime->getMime()); 
    // Send content-length HTTP header 
    header('Content-Length: '.filesize($dir . $filename)); 
    // Send content-disposition with save file name HTTP header 
//    header('Content-Disposition: attachment; filename="'.$filename.'"'); 
    header('Content-Disposition: attachment; filename="'.$saveasname.'"'); 
    // Output file 
    readfile($dir . $filename); 
    // Done 
    return TRUE; 
} 
?>