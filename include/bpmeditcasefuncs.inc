<?php

/*
 * Funcoes usadas em BPMEDITCASE.php
 */

function DefineMenu($tipoFormulario = "S")
{
    global $StepId, $Fechar, $Voltar, $Action, $Menu, $Queue, $CustonMenu, $CustonGate, $CustonNewInstance, $CustonRoot, $Custon, $CaseNum, $Retornar, $userdef, $LockedById;
    if ($Fechar == 1) {
        $Menu = 35;
        return;
    }

    $Menu = 2;
    $ImagemTopo = "caseedit0001.png";
    if (($Action == "Edit") && empty($LockedById)) {
        if (empty($userdef->UserId)) {
            $Menu = 19;
            $ImagemTopo = "caseedit0001.png";
        } else {
            $Menu = 3;
            $ImagemTopo = "caseedit0001_pb.png";
        }
    }
    if ($Action == "Pesq") {
        if (empty($UserId)) {
            $Menu = 43;
        } else {
            $Menu = 43;
        }
    }
    if ($Custon == 1) {
        if ($Action == "Edit") {
            if (empty($CaseNum)) {
                $Menu = $CustonMenu[3];
                $Queue = $CustonRoot . $CustonGate;
                $Retornar = $CustonRoot . $CustonNewInstance;
            } else {
                $Menu = $CustonMenu[1];
                $Queue = "flow0001-2.php";
                $Retornar = $Queue;
            }
        } else {
            $Menu = $CustonMenu[2];
        }
    }
}

function TemTickler($ProcId)
{
    global $connect;
    if ($procdef->Ticker == "T") {
        $SQL = "select FieldId from procfieldsdef where ProcId = $ProcId and FieldType = 'TK'";
        $Query = mysqli_query($connect, $SQL);
        $result = mysqli_fetch_array($Query);
        return $result["FieldId"];
    }
    return 0;
}

function PegaDadosQueue()
{
    global $connect, $DeadSoftAdHoc, $DeadHardAdHoc, $ProcId, $StepId, $CaseNum;
    $SQL = "select DeadSoftDateTime, DeadHardDateTime from casequeue where CaseId = $CaseNum and ProcId = $ProcId and StepId = $StepId";
    $Query = mysqli_query($connect, $SQL);
    $Result = mysqli_fetch_array($Query);
    $DeadSoftAdHoc = $Result["DeadSoftDateTime"];
    $DeadHardAdHoc = $Result["DeadHardDateTime"];
}

function MostraComentario($Comentario, $contador)
{
    return $Coment;
}

function NovoCaso($ProcId, $StepId, $tipoProcesso)
{
    global $connect, $userdef;
    $Hora = microtime();
    $SQL = "insert into casemap (ProcId, UserId, samaccountname, Hora) values ($ProcId, $userdef->UserId, '$userdef->UserName', '$Hora')";
    mysqli_query($connect, $SQL);
    $SQL = "select CaseId as NRCaso from casemap where ProcId = $ProcId and UserId = $userdef->UserId and samaccountname = '$userdef->UserName' and Hora = '$Hora'";
    $Query = mysqli_query($connect, $SQL);
    $Result = mysqli_fetch_array($Query);
    $NrCaso = $Result["NRCaso"];

    switch ($tipoProcesso) {
        case "P":
            $SQL = "insert into casedata (procid, casenum, fieldid, valueid, fieldvalue) values ($ProcId, $NrCaso, 5, 0, '$StepId')";
            $resultado = mysqli_query($connect, $SQL);

            $StatusInicialProcesso = PegaStatusProcessoDefault($ProcId, $StepId);
            $SQL = "insert into casedata (procid, casenum, fieldid, valueid, fieldvalue) values ($ProcId, $NrCaso, 6, 0, '$StatusInicialProcesso')";
            $resultado = mysqli_query($connect, $SQL);
            break;
        case "I":
            $SQL = "insert into casedata (procid, casenum, fieldid, valueid, fieldvalue) values ($ProcId, $NrCaso, 5, 0, '0')";
            $resultado = mysqli_query($connect, $SQL);
            break;
    }

    return $NrCaso;
}

function PegaDocInicio($CaseNum)
{
    return PegaDoc($CaseNum, -2);
}

function PegaDocFim($CaseNum)
{
    return PegaDoc($CaseNum, 2);
}

function PegaDocProximo($CaseNum)
{
    return PegaDoc($CaseNum, 1);
}

function PegaDocAnterior($CaseNum)
{
    return PegaDoc($CaseNum, -1);
}

function PegaHelpStep($ProcId, $StepId)
{
    global $connect, $procdef;
    foreach ($procdef->Steps as $Result) {
        if ($Result["StepId"] == $StepId) {
            break;
        }
    }
    return $Result["HelpHtml"];
}

function PegaDadoscasequeue()
{
    global



    $connect, $CaseNum, $ProcId, $LockedBy, $LockAdHoc, $Prioridade, $InsertDate, $StepId, $userdef, $LockedById;
    if ($CaseNum == 0) {
        $LockedBy = "";
        $LockedById = 0;
        $Prioridade = 1;
    } else {
        $SQL = "select lockedbyid, priority, InsertDate, AdHoc from casequeue where CaseId = $CaseNum and ProcId = $ProcId and StepId = $StepId";
        $QUERY1 = mysqli_query($connect, $SQL);
        $linha = mysqli_fetch_array($QUERY1);
        mysqli_free_result($QUERY1);

        $LockedById = $linha["lockedbyid"];
        if ($LockedById == $userdef->UserId) {
            $LockedById = 0;
        }
        $LockedBy = PegaNomeUsuario($linha["lockedbyid"]);
        $InsertDate = $linha["InsertDate"];
        $Prioridade = $linha["priority"];
    }
}

function BloqueiaRegistroParaSerEditadoPeloUsuario()
{
    global $connect, $Action, $CaseNum, $ProcId, $LockedById, $StepId, $userdef, $AdHoc;
    $dataHoraLock = date("Y-m-d H:i:s");
    if (!empty($CaseNum) && $Action == "Edit" && $AdHoc <> 1) {
        if ($LockedById == 0) {
            $SQL = "update casequeue set lockedbyid = $userdef->UserId, Lockedbysamaccountname = '$userdef->UserName', lockeddatetime = '$dataHoraLock' where StepId = $StepId and CaseId = $CaseNum and ProcId = $ProcId and lockedbyid = 0";
            $resultado = mysqli_query($connect, $SQL);
        }
    }
}

function MontaEstilos($QUERY)
{
    global $Rodape, $ProcId, $StepId;
    while ($Linha = mysqli_fetch_array($QUERY)) {
        $campo = $Linha["fieldid"];
        $option = $Linha["optional"];
        $ReadOnly = $linha["ReadOnly"];
        $CSSParametros = $Linha["CSS"];

        $Estilo = $Estilo . MontaEstilo($CSSParametros, $campo, $ReadOnly, $optional, 'NORMAL');
        $CSSTop = PegaParametroCSS($CSSParametros, "Top");
        $CSSHeight = PegaParametroCSS($CSSParametros, "Height");
        $Top = substr($CSSTop, 0, strlen($CSSTop) - 2);
        $Height = substr($CSSHeight, 0, strlen($CSSHeight) - 2);
        $RodapeCalc = substr($Rodape, 0, strlen($Rodape) - 2);
        if (($Top + $Height) > $RodapeCalc) {
            $Rodape = ($Top + $Height) . "px";
        }
    }

    return $Estilo;
}

function MostrarCampoRum(
$Campo)
{
    global $CamposRumAtivo, $CamposRum;
    if (is_array($CamposRum)) {
        if (in_array($Campo, $CamposRum)) {
            if (is_array($CamposRumAtivo)) {
                if (!in_array($Campo, $CamposRumAtivo)) {
                    return false;
                }
            }
        }
    }
    return true;
}

function UserCriaAdHoc($UserId)
{
    global



    $connect, $GrupoADAdHoc, $config, $userdef, $OrigemLogon;
    if ($OrigemLogon == "External") {
        $Grupos = explode(',', $userdef->usergroups);
        $AdHoc = in_array($config["AdHocAd"], $Grupos);
    } else {
        $SQL = "select AdHoc from userdef where UserId = $userdef->UserId";
        $Query = mysqli_query($connect, $SQL);
        $Linha = mysqli_fetch_array($Query);
        $AdHoc = $Linha["AdHoc"];
    }
    return $AdHoc;
}

function StepAdHoc(
$StepId)
{
    global $connect, $ProcId;
    $SQL = "select AdHoc from stepdef where ProcId = $ProcId and StepId = $StepId";
    $Query = mysqli_query($connect, $SQL);
    $Linha = mysqli_fetch_array($Query);
    $AdHoc = $Linha["AdHoc"];
    return $AdHoc;
}

?>
