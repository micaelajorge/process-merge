<?php
// WebService do PROCESSBR
require_once("include/resource01.php");
require_once("include/common.php");
require_once('./lib/nusoap.php');

$server = new soap_server();
// Initialize WSDL support
$server->configureWSDL('wsbpm', 'urn:wsbpm');
$config = Carregaconfig();
// Error
// Auth User


require_once('/includes_ws/tbpmerror.php');
require_once('/includes_ws/wsbpmerror.php');

include("./includes_ws/tbpmuser.php");
include("./includes_ws/tbpmdefs.php");


include("./includes_ws/wbpmCriaNotificacoes.php");
include("./includes_ws/tbpmCriaNotificacoes.php");


// Inbox Get
include("./includes_ws/tbpminbox.php");
include('./includes_ws/wsbpmInbox.php');
include('./includes_ws/wsbpmInboxFilter.php');
include('./includes_ws/tbpmCaseStart.php');
include('./includes_ws/tbpmCaseRelease.php');
include('./includes_ws/wsbpmUser.php');

// Case Unlock
include('./includes_ws/wsbpmCaseUnlock.php');

//GetData
include('./includes_ws/wsbpmGetData.php');

// Case Start
include('./includes_ws/wsbpmCaseStart.php');

// Case Release
include('./includes_ws/wsbpmCaseRelease.php');

// CaseAcquire (Lock)
include('./includes_ws/wsbpmCaseAcquire.php');

// CaseAcquire (Lock)
include('./includes_ws/wsbpmCaseSave.php');

// Get Definicao do caso
include('./includes_ws/wsbpmGetDef.php');


// Use the request to (try to) invoke the service
$HTTP_RAW_POST_DATA = isset($HTTP_RAW_POST_DATA) ? $HTTP_RAW_POST_DATA : '';
$server->service($HTTP_RAW_POST_DATA);
?>