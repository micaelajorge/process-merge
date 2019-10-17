<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 12/11/2018
  Sistema: Process_XAMPP
 */

/**
 * 
 */
require_once(FILES_ROOT . "include/resource01.php");
require_once(FILES_ROOT . "include/class_mime.php");

define("MAX_LOCK_TIME", 10);
define("MAX_LOGIN_TIME", 15);

function func_removeLoginsExcedidos()
{
    global $connect;
// Calcula a data do dead time com 5 minutos
    $oHora = new DateTime();
    $minutesToAdd = MAX_LOGIN_TIME;
    $oHora->modify("-{$minutesToAdd} minutes");
    $horaExclusao = $oHora->format('Y-m-d H:i');
    $sql = "delete * from userlogins where LogonTimeStamp < '$horaExclusao'";
    mysqli_query($connect, $sql);
}

function func_removeLocksPorTempoExcedido()
{
    global $connect;
// Calcula a data do dead time com 5 minutos
    $oHora = new DateTime();
    $minutesToAdd = MAX_LOCK_TIME;
    $oHora->modify("-{$minutesToAdd} minutes");
    $horaExclusao = $oHora->format('Y-m-d H:i');
    $sql = "select * from casequeue where lockeddatetime < '$horaExclusao' and lockedbyid <> 0";
    $query = mysqli_query($connect, $sql);
    while ($linha = mysqli_fetch_array($query, MYSQLI_ASSOC)) {
        $lockedById = $linha["lockedbyid"];
        $procId = $linha["ProcId"];
        $stepId = $linha["StepId"];
        $caseId = $linha["CaseId"];

        $userNameLock = PegaNomeUsuario($lockedById);
        $sql = "update casequeue set lockedbyid = 0, lockeddatetime = 0 where procId = $procId and stepId = $stepId and caseId = $caseId";
        $query = mysqli_query($connect, $sql);
        insereEntradaAuditTrail($procId, $caseId, $stepId, -1, 'automato', 512, "Tempo de bloqueio excedido, removido Bloqueio do usuário $userNameLock");
    }
}

function pegaCasosAutoStep()
{
    global $connect;
    $SQL = " select "
            . "casequeue.procid, "
            . "caseid as casenum, "
            . "casequeue.stepid "
            . "from "
            . "casequeue, "
            . "stepdef "
            . "where "
            . "casequeue.procid = stepdef.procid "
            . "and "
            . "casequeue.stepid = stepdef.stepid "
            . "and "
            . "stepdef.autostep = 1 "
            . "limit 50";
    $Query = mysqli_query($connect, $SQL);
    $listaDados = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $dadosRetono["data"] = $listaDados;
    $dadosRetono["totalCasos"] = count($listaDados);
    $jDados = json_encode($dadosRetono);
    if (json_last_error() > 0) {
        $erroJSON = json_last_error() . " " . json_last_error_msg();
        error_log("Json erro: " . $erroJSON);
    }
    header('Content-Type: application/json');
    echo $jDados;
}

/**
 *  ROTA: api/automato/alive
 */
function automatoAlive()
{
    $dadosEntrada = apiTrataDadosEntrada();
    $dadosEntrada["ultimaAtualizacao"] = date("Y-m-d H:i:s");
    $dadosGravar = json_encode($dadosEntrada);
    file_put_contents(FILES_ROOT . "/atualizacao_automato.json", $dadosGravar);
    $statusAutomato = file_get_contents(FILES_ROOT . "/status_automato.json");
    if ($statusAutomato !== false) {
        $jDados = $statusAutomato;
    } else {
        $dadosRetono["result"] = "ok";
        $jDados = json_encode($dadosRetono);
    }

    header('Content-Type: application/json');
    echo $jDados;
}

function automatoMudaStatusOperacao()
{
    $userDef = $_SESSION["userdef"];
    $statusAutomato = array();
    // TODO: Implementar autenticação Usuário, para dashboard

    if ($userDef->Admin) {
        $dadosEntrada = apiTrataDadosEntrada();
        $statusAutomato["statusAutomato"] = ($dadosEntrada["statusAutomato"] == "true") ? true : false;
        $jstatusAutomato = json_encode($statusAutomato);
        file_put_contents(FILES_ROOT . "/status_automato.json", $jstatusAutomato);
    } else {
        // Envia o header de falha de Permissão
        header("HTTP/1.0 403 Sem Permissões");
        $statusAutomato["error"] = "Usuário não tem privilégios suficientes";
    }
    $jDados = json_encode($statusAutomato);
    header('Content-Type: application/json');
    echo $jDados;
}

/**
 * 
 */
function automatoRemoveLocks()
{
    func_removeLocksPorTempoExcedido();
    $dadosRetono["result"] = "ok";
    $jDados = json_encode($dadosRetono);
    header('Content-Type: application/json');
    echo $jDados;
}

/**
 * 
 */
function automatoRemoveLogins()
{
    func_removeLoginsExcedidos();
    $dadosRetono["result"] = "ok";
    $jDados = json_encode($dadosRetono);
    header('Content-Type: application/json');
    echo $jDados;
}

function automatoCheckData()
{
    apiAllowCors();
    $statusAutomato = file_get_contents("status_automato.json");
    header('Content-Type: application/json');
    echo $statusAutomato;
}

/**
 *  ROTA: api/v1/checkautomato
 */
function checkAutomato()
{
    $dadosEntrada = $dadosEntrada = apiTrataDadosEntrada();
    $serverPort = $dadosEntrada["serverPort"];
    $retorno = executaWebServiceCURL("localhost:$serverPort", [], "GET", [], false);
    if (is_array($retorno)) {
        $retorno = json_encode($retorno);
    }
    header('Content-Type: application/json');
    echo $retorno;
}

/**
 * ROTA: api/v1/emailsendqueue
 */
function email_send_queue_v1()
{
    global $connect;

    $SQL = "
select 
actionsendemailqueue.procId,
actionsendemailqueue.stepId,
actionsendemailqueue.conditionId,
actionsendemailqueue.trueFalse,
actionsendemailqueue.caseNum,
subject,
body
from actionsendemailqueue, actionsendemail where actionsendemailqueue.procid = actionsendemail.procid and actionsendemailqueue.stepid = actionsendemail.stepid  and  actionsendemailqueue.conditionid = actionsendemail.conditionid and  actionsendemailqueue.TrueFalse = actionsendemail.truefalse and emailsent = 0 order by actionsendemailqueue.procId,
actionsendemailqueue.stepId,
actionsendemailqueue.conditionId,
actionsendemailqueue.trueFalse
 limit 10";
    $query = mysqli_query($connect, $SQL);

    $dadosQueue = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $retorno = json_encode($dadosQueue);
    header('Content-Type: application/json');
    echo $retorno;
}

/**
 * ROTA: api/v1/emailsent
 */
function email_sent()
{
    global $connect;

    $dadosEntrada = apiTrataDadosEntrada();

    $procId = $dadosEntrada["procId"];
    $stepId = $dadosEntrada["stepId"];
    $caseNum = $dadosEntrada["caseNum"];
    $trueFalse = $dadosEntrada["trueFalse"];
    $conditionId = $dadosEntrada["conditionId"];

    $SQL = "update actionsendemailqueue set emailsent = 1 where procid = $procId and stepId = $stepId and conditionId = $conditionId and truefalse = '$trueFalse' and casenum = $caseNum";
    $query = mysqli_query($connect, $SQL);

    header('Content-Type: application/json');
    echo "ok";
}

/**
 * ROTA: api/v1/getemailbody
 */
function pegaArquivoBody()
{
    $dadosEntrada = apiTrataDadosEntrada();

    $arquivoBody = $dadosEntrada["body"];

    
    $mime = new mime($arquivoBody);
    $ContentType = $mime->getMime();
    // Send binary filetype HTTP header 

    $arquivoEnvio = FILES_ROOT . "assets/notificacoes/" . $arquivoBody;
    
    header('Pragma: cache');
    header('Cache-Control: private');
    header('Content-Type: ' . $ContentType);

    header('Content-Length: ' . filesize($arquivoEnvio));
    header('Content-Disposition: attachment; filename="' . $arquivoBody . '"');
    
    readfile($arquivoEnvio);
}

/**
 * ROTA: api/v1/emallist
 */
function pegaEmailList_v1()
{
    global $connect;
    
    $dadosEntrada = apiTrataDadosEntrada();
    
    $procId = $dadosEntrada["procId"];
    $stepId = $dadosEntrada["stepId"];
    $trueFalse = $dadosEntrada["trueFalse"];
    $conditionId = $dadosEntrada["conditionId"];

    $SQL = "(select userdesc, email from actionmailaddress, userdef where grpfld = 'U' and userdef.userid = actionmailaddress.ToId and email <> '' and procid = $procId and stepid = $stepId and truefalse = '$trueFalse' and conditionid = $conditionId)
    union
    (select userdesc, email from actionmailaddress, userdef, usergroup where grpfld = 'G' and usergroup.groupid = actionmailaddress.ToId and usergroup.UserId = userdef.UserId and email <> '' and procid = $procId and stepid = $stepId and truefalse = '$trueFalse' and conditionid = $conditionId)";
    
    $query = mysqli_query($connect, $SQL);
    
    $listaEmails = mysqli_fetch_all($query, MYSQLI_ASSOC);
    
    $retorno = json_encode($listaEmails);
    
    header('Content-Type: application/json');    
    echo $retorno;
}
