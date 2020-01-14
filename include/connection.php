<?php

function AtivaDBExterno($SelDb = True)
{
    global $connectDBExterno, $EXTERNALSERVER, $EXTERNALUSER, $EXTERNALPWD, $EXTERNALDB;
    $connectDBExterno = mysqli_connect($EXTERNALSERVER, $EXTERNALUSER, $EXTERNALPWD, $EXTERNALDB);
    if (!$connectDBExterno) {
        error_log("Erro acesso ao DB " . mysqli_error($connectDBExterno));
        error_log("Erro de Conexao: " . mysqli_connect_errno() . ' - ' . mysqli_connect_error());
//        header('Content-Type: application/json');
        header("HTTP/1.0 501 db Externo");                
    }
    mysqli_query($connectDBExterno, "SET NAMES 'utf8'");
    mysqli_query($connectDBExterno, "SET character_set_connection=utf8");
    mysqli_query($connectDBExterno, "SET character_set_client=utf8");
    mysqli_query($connectDBExterno, "SET character_set_results=utf8");
    return $connectDBExterno;
}

function AtivaDBProcess($SelDb = True)
{
    global $connect, $BPMSERVER, $BPMUSER, $BPMPWD, $BPMDB;

    //$connect = mysqli_connect('p:' . $BPMSERVER, $BPMUSER, $BPMPWD, $BPMDB);
    $connect = mysqli_connect($BPMSERVER, $BPMUSER, $BPMPWD, $BPMDB);
    //echo var_export($connect, true) . "<br>";
    if (!$connect) {
        error_log("Erro acesso ao DB " . mysqli_error($connect));
        error_log("Erro de Conexao: " . mysqli_connect_errno() . ' - ' . mysqli_connect_error());
//        header('Content-Type: application/json');
        header("HTTP/1.0 501 db Externo");        
        die();
    }

    mysqli_query($connect, "SET NAMES 'utf8'");
    mysqli_query($connect, "SET character_set_connection=utf8");
    mysqli_query($connect, "SET character_set_client=utf8");
    mysqli_query($connect, "SET character_set_results=utf8");
//	if ($SelDb)
//		{
//		mssql_select_db($BPMDB, $connect);
//		mssql_query("SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED", $connect);
//		}
    return $connect;
}

function AtivaOutroDB($SERVER, $USER, $PASSWORD, $DB)
{
    global $connectOutroDB;
    include(FILES_ROOT . "include/config.config.php");
    $connectOutroDB = mssql_connect($SERVER, $USER, $PASSWORD);
    mssql_select_db($DB, $connectOutroDB);
    if (!$connectOutroDB) {
        error_log("Erro acesso ao DB " . mysqli_error($connectOutroDB));
        error_log("Erro de Conexao: " . mysqli_connect_errno() . ' - ' . mysqli_connect_error());
        header('Content-Type: application/json');
        header("HTTP/1.0 501 db Externo");                
    }
    mysqli_query($connectOutroDB, "SET NAMES 'utf8'");
    mysqli_query($connectOutroDB, "SET character_set_connection=utf8");
    mysqli_query($connectOutroDB, "SET character_set_client=utf8");
    mysqli_query($connectOutroDB, "SET character_set_results=utf8");
    return $connectOutroDB;
}
