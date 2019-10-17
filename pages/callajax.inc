<?php
include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/resource01.php");

session_start();
$gmtDate = gmdate("D, d M Y H:i:s");
header("Content-Type: text/html; charset=utf8",true);
header("Expires: {$gmtDate} GMT"); 
header("Last-Modified: {$gmtDate} GMT"); 
header("Cache-Control: no-cache, must-revalidate"); 
header("Pragma: no-cache"); 

//use raelgc\view\Template;
if (!empty($script))
	{        
	include($script);		
	return;
	}
	
if (!empty($file))
	{
	include($file);
	return; 		
	}
		
if (!empty($funcao))
	{
	eval("$funcao;");
	$ShowFile = false;	
	}
try
    {
    
    while (list($_NCampo, $_NValor) = each($_GET))
        {
        $$_NCampo = $_NValor;
        }      
    
if (!empty($_GET["include"]))
    {
    //include("includes/db_connection.inc");
    include("./includes/" . $_GET["include"]);
    }

        
$NOME_USUARIO = $session["SESS_NOME_USUARIO"];

if (!empty($_GET["script"]))
	{
	include($_GET["script"]);		
	}
	
if (!empty($_GET["funcao"]))
	{
        $funcao = $_GET["funcao"];
	eval("$funcao;");
	$ShowFile = false;	
	}
	
if (!empty($_GET["ExeFunc"]))	
	{	
        $ExeFunc = $_GET["ExeFunc"];
	$ExeFunc($_GET);
	}
	
if (!empty($_GET["EchoExeFunc"]))
	{
        $EchoExeFunc = $_GET["EchoExeFunc"];	
	$Retorno = $EchoExeFunc($_GET); 
	echo $Retorno;	
	}

    }
    
  catch (Exception $e) 
        {
	echo "\n<div> $e </div>";
        }
?>        