<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 21/01/2019
  Sistema: Process_XAMPP
 */

$headers = getallheaders();

error_log("SOAPAction: " . $headers["SOAPAction"]);

function GetMinimumClientRequiredVersion()
{
    $dados["GetMinimumClientRequiredVersionResult"] = '1.1.0.0';
    return $dados;
}

function Login()
{
    $dados["LoginResult"] = true;
    return $dados;
}

function GetProcesses()
{
//    $jDados = file_get_contents("getprocess.json");
//     $dados = json_decode($jDados, true);


    $dados = pegaProcessos();

    //error_log("Processo: " . var_export($dados, true));
    return $dados;
}

function GetNextFolder()
{
    $retorno["GetNextFolderResult"] = "";
    $retorno["GetNextFolderResult"]["WorkFolderIDField"] = "1";
    $retorno["GetNextFolderResult"]["CurrentProcessRule"] = "ABOBORA";
    $retorno["GetNextFolderResult"]["ObjectID"] = "999";
    return $retorno;
}


$server = new SoapServer('zoom.xml');

$server->addFunction("GetMinimumClientRequiredVersion");
$server->addFunction("Login");
$server->addFunction("GetProcesses");
$server->addFunction("GetNextFolder");

$server->addFunction(SOAP_FUNCTIONS_ALL);


$server->handle();

function pegaProcessos()
{
    $process["Process"] = array();
    $process["Process"][] = pegaProcesso();
    $retorno["GetProcessesResult"] = $process;
    return $retorno;
}

/**
 * 
 * @return type
 */
function pegaProcesso()
{
    $defProcess["DocTypeGroups"] = null;
    $defProcess["DocTypes"] = null;
    $defProcess["Fields"] = pegaCampos();
    $defProcess["Name"] = "Analise Proposta de Cartão - Agiliza";
    $defProcess["ProcessCode"] = "APROCARTAGILIZA";
    $defProcess["ProcessID"] = "15";
    
    $role["StateCode"] = "ABOBORA";  
    $role["CurrentRule"]["Name"] = "Teste 1";
    $defProcess["Rules"] = array($role);
    
    $defProcess["Settings"]["ProcessConfigurationData"] = array();
    
    $Setting["ConfigurationDataID"] = 29;
    $Setting["Name"] = "ContentHasZipFilesStatusCode";
    $Setting["ProcessCode"] = "APROCARTAGILIZA";
    $Setting["Value"] = "ZOOM";     
    $defProcess["Settings"]["ProcessConfigurationData"][] = $Setting;

    $Setting["ConfigurationDataID"] = 30;
    $Setting["Name"] = "ContentStateCode_WithError";
    $Setting["ProcessCode"] = "APROCARTAGILIZA";
    $Setting["Value"] = "IRREGULAR";        
    $defProcess["Settings"]["ProcessConfigurationData"][] = $Setting;
        
    $Setting["ConfigurationDataID"] = 30;
    $Setting["Name"] = "ContentStateCode_WithError";
    $Setting["ProcessCode"] = "APROCARTAGILIZA";
    $Setting["Value"] = "REGULAR";        
    $defProcess["Settings"]["ProcessConfigurationData"][] = $Setting;

    $defProcess["WorkFolder"] = null;
    return $defProcess;
}

/**
 * 
 * @return string
 */
function pegaCampos()
{

    $campo["CurrentField_Code"] = "CPF";
    $campo["FieldOrder"] = 0;
    $campo["ProcessFieldId"] = 63;
    $campo["Process_Code"] = "AGE";

    $currentProcess["Name"] = "Análise Emprestimo Losango";
    $currentProcess["ProcessCode"] = "AGE";
    $currentProcess["ProcessID"] = "16";

    $campo["CurrentProcess"] = $currentProcess;

    $CurrentField["CurrentField"] = array("DefProcessField" => array("ProcessField" => array($campo)));
    $CurrentField["CurrentField_Code"] = "CPF";
    $CurrentField["FieldOrder"] = 0;
    $CurrentField["ProcessFieldID"] = 69;
    $CurrentField["Process_Code"] = "ACSS";

    $retorno["ProcessField"] = array();
    $retorno["ProcessField"][] = $CurrentField;
    return $retorno;
}
