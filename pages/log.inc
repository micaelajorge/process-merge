<?php
//error_log("Entrando LOG");

if (!isset($_POST["UserName2"]) & !isset($_POST["ASP_session"]) & isset($_POST["SessionID"])) {
    (LOG_DATA) ? error_log("Falha rebendo dados Usuário") : null;
    header("location index.php");
    exit;
}
if (isset($_POST["SessionID"])) {
    $ASP_session = $_POST["SessionID"];
}

$_COOKIE = array();

require_once("include/connection.php");
require_once("include/ldap.php");
require_once("include/validaentradadireta.php");
require_once("include/resource01.php");
require_once("include/user.php");


$TextoCarga = "Validando Usuário";

(LOG_DATA) ? error_log("$TextoCarga") : null;

$Carga = True;
$userdef = new userdef;

$mobileMode = 0;
if (isset($_POST["mobileMode"])) {
    $mobileMode = $_POST["mobileMode"];
}
$_SESSION["mobileMode"] = $mobileMode;

if ($mobileMode) {
    $paginaInicialPreferencial = "entradamobile";
} else {
    $paginaInicialPreferencial = "entrada";
}

$AuthUser['UserName'] = "";
$AuthUser["UserPassword"] = "";

if (isset($_POST['UserName2'])) {
    $AuthUser['UserName'] = $_POST['UserName2'];
}
if (isset($_POST['UserPwd2'])) {
    $AuthUser["UserPassword"] = $_POST['UserPwd2'];
}
AtivaDBProcess();

$userdef->Create($OrigemLogon, $connect, $AuthUser, $CriarSessionIntegration);

if (!$userdef->Validado) {
//	error_log("não validou - " . $AuthUser['UserName'],0);
    $_SESSION = array();
    $erro_logon = true;
    $MsgLogon = 'Usuário ou Senha incorretos';
    include(FILES_ROOT . "pages/logon.inc");
    exit;
}

$_SESSION["userdef"] = $userdef;
$config = Carregaconfig();
$paginaEntrada = $paginaInicialPreferencial;
$_SESSION["entrada"] = $paginaEntrada;

if (isset($PageSelect)) {
    if ($PageSelect !== "" && $PageSelect !== "logon" && $PageSelect !== "log.php" && $PageSelect !== "log_php") {
        $paginaEntrada = $PageSelect;
    }
}

$CaseNum = "";
if (isset($_POST["CaseNum"])) {
    $CaseNum = $_POST["CaseNum"];
}

$ProcId = "";
if (isset($_POST["ProcId"])) {
    $ProcId = $_POST["ProcId"];
}

$S_Processos = PegaProcessosUsuario($userdef->UserId, $userdef->Origem);


if ($userdef->Admin) {
    $Item["ProcName"] = "Administração";
    $Item["ProcDesc"] = "Administração de Processos";
    $Item["TipoProc"] = "WF";
    $Item["Color"] = "#9fcdff";
    $Item["ProcId"] = 0;
    $Item["NumCount"] = 0;
    $Item["PageAction"] = "adm_entrada";
    $Item["ProcIcon"] = "fa fa-cogs";
    array_push($S_Processos, $Item);
}

if (count($S_Processos) > 1) {
    if (!empty($ProcId)) {
        $ProcIdFound = PegaProcIdByCode($ProcId);
        $listaProcCod = array_column($S_Processos, "ProcId");
        $indiceProcesso = array_search($ProcIdFound, $listaProcCod);

        $procId_EntradaUnico = null;
        $totalProcessosView = 0;
        for ($i = 0; $i < count($S_Processos); $i++) {
            if ($S_Processos[$i]["HideEntrada"] == 0) {
                $indiceProcesso = $i;
                $totalProcessosView++;
            }
        }

        if ($totalProcessosView > 1) {
            $indiceProcesso = false;
        }

        // Verifica se o Codigo existe na lista de Processos do usuário
        if ($indiceProcesso > -1) {
            $paginaAcao = $S_Processos[$indiceProcesso]["PageAction"];
            if ($paginaAcao !== "") {
                $paginaEntrada = $paginaAcao;
            } else {
                $paginaEntrada = "queue/" . $S_Processos[$indiceProcesso]["ProcId"];
            }
        } else {
            $listPageAcitons = array_column($S_Processos, "PageAction");
            $indiceProcesso = array_search($ProcId, $listPageAcitons);
            if ($indiceProcesso > -1) {
                $paginaEntrada = $ProcId;
            }
        }
    }
    if ($CaseNum != "") {
        if ($Action == "") {
            $Action = "View";
        }
        if (!is_numeric($StepId)) {
            $StepId = PegaStepIdByCode($ProcId, $StepId);
        }
        $StepId = StepDoCaso($ProcId, $StepId, $CaseNum);
        if ($CaseNum <> 0) {
            $StepId = StepDoCaso($ProcId, $StepId, $CaseNum);
        }
        $StatusCaso = VerificaCaso($ProcId, $StepId, $CaseNum, $userdef, $AdHoc);
        if (isset($_POST["ProcId"])) {
            if (!empty($ProcId)) {
                $ProcIdAtivo = $ProcId;
            } else {
                if (isset($_POST["ProcIdAtivo"])) {
                    $ProcId = $ProcIdAtivo;
                }
            }
        }
    } else {
        $_SESSION["S_Processos"] = $S_Processos;
    }
    echo "<html><body><script>document.location = \"$paginaEntrada\"</script></body></html>";
    exit();
}

$_SESSION["entrada"] = "logoff.php";
if ($S_Processos[0]["ProcId"] == '') {
    if ($userdef->Admin) {
        include("cargafooter.html");
        $_SESSION["S_Processos"] = $S_Processos;
        echo "<html><body><script>document.location = \"admentrada.php\"</script></body></html>";
        exit;
    }
    include("cargafooter.html");
    $erro_logon = true;
    $MsgLogon = 'Usuário não tem acesso a nenhum processo';
    $TIPO_ALERTA_LOGON = "alert-info";
    include($PaginaLogon);
    exit;
}

if ($CaseNum != "") {
    if ($Action != "") {
        $Action = "View";
    }
    if (!is_numeric($ProcId)) {
        $ProcId = PegaProcIdByCode($ProcId);
    }
    if (!is_numeric($StepId)) {
        $StepId = PegaStepIdByCode($ProcId, $StepId);
    }
    $StepId = StepDoCaso($ProcId, $StepId, $CaseNum);
    $StatusCaso = VerificaCaso($ProcId, $StepId, $CaseNum, $userdef);
    mysqli_close($connect);
    $_SESSION["S_Processos"] = $S_Processos;
    include("direciona.php");
    exit();
} else {
    $ProcId = $S_Processos[0]["ProcId"];
    switch ($S_Processos[0]["TipoProc"]) {
        case "CT":
            $paginaEntrada = "flow0018.php";
            break;
        case "WF":
            if ($userdef->Admin) {
                $entrada = $paginaInicialPreferencial;
            } else {
                $paginaEntrada = $S_Processos[0]["PageAction"];
                if ($paginaEntrada == "") {
                    $paginaEntrada = "queue/%PROCID%";
                }
                $paginaEntrada = str_replace("%ASP_SESSION%", $userdef->ASP_session, $paginaEntrada);
                $paginaEntrada = str_replace("%PROCID%", $S_Processos[0]["ProcId"], $paginaEntrada);
            }
            $ProcIdAtivo = $S_Processos[0]["ProcId"];
            break;
        case "AD":
            $Menu = 28;
            $paginaEntrada = "adm0001.php";
            break;
    }
}

if ($ForcarSelecaoProcesso) {
    $paginaEntrada = $paginaInicialPreferencial;
}
include("cargafooter.html");
$_SESSION["S_Processos"] = $S_Processos;
?>
<html>
    <body>
        <script>
            document.location = "<?= $paginaEntrada ?>";
        </script>
    </body>
</html>
