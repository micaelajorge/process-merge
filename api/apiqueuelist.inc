<?php

// <editor-fold defaultstate="collapsed" desc="Includes do Script">
require_once(FILES_ROOT . "include/resource01.php");
require_once(FILES_ROOT . "include/resource02.php");
require_once(FILES_ROOT . "include/user.php");
// </editor-fold>
// // <editor-fold defaultstate="collapsed" desc="Carregamento de Variaveis da sessão">
#Ativa o Processo

$dadosEntrada = apiTrataDadosEntrada();
$filtros = apiTrataCampos($dadosEntrada);

if (!isset($Detalhes)) {
    $Detalhes = "";
}

if (!isset($MostraDetalhes)) {
    $MostraDetalhes = "";
}

if ($Detalhes == "") {
    if ($MostraDetalhes == "") {
        $MostraDetalhes = 1;
        $MostrarDetalhes = 0;
        $Detalhes = 1;
    } else {
        if ($MostraDetalhes) {
            $MostrarDetalhes = 0;
        } else {
            $MostrarDetalhes = 1;
            $Detalhes = $MostraDetalhes;
        }
    }
} else {
    $MostraDetalhes = $Detalhes;
    if ($Detalhes == 1) {
        $MostrarDetalhes = 0;
    } else {
        $MostrarDetalhes = 1;
    }
}
if (!isset($_SESSION["lidos"])) {
    session_register("lidos");
    $lidos = array();
}

if (key_exists("PaginaAtual", $_GET)) {
    $Pagina = $_GET["PaginaAtual"];
}

if (empty($PaginaAtual)) {
    $Pagina = 1;
}

if (empty($Linhas)) {
    $Linhas = 10;
}

//$Linhas = 200;	
// </editor-fold>	
// <editor-fold defaultstate="collapsed" desc="Tratamento de Variaveis recebidas GET / POST">

function AcessoCaso($ProcId, $StepId, $CaseNum, $ProcName, $PageEdit)
{
    global $Action, $AdHoc;
    //$ValorAcesso = str_replace("'","/*%/*", $Valor);
    //$ValorAcesso = str_replace(";","/*#/*", $ValorAcesso);
    //$Acesso = "AcessaCaso('', '', $AdHoc, $ProcId, $CaseNum, $StepId, '$ProcName', 'Queue', $PaginaAtual, '$PageEdit', $Action)"; // nÃO ESTAVA TRATANDO A PÁGINA ATUAL
    $Acesso = "jsAcessaCaso('', '', $AdHoc, $ProcId, $CaseNum, $StepId, '$ProcName', 'Queue', 1, '$PageEdit', $Action)";
    return $Acesso;
}

// <editor-fold defaultstate="collapsed" desc="Calculo DeadTime">    
function CalculoDeadTimeExtourado($result, $AdHoc = 0)
{

    global $procdef;
    // Pega o Step Do Caso
    $StepId = $result["StepId"];

    // Pega a data de Dead Time do Caso
    $DeadSoft = $result["deadsoftdatetime"];
    $DeadHard = $result["deadharddatetime"];
    $DeadHardest = $result["deadhardestdatetime"];

    $NDeadSoft = $procdef->Steps[$StepId]["DeadSoft"];
    $NDeadHard = $procdef->Steps[$StepId]["DeadHard"];
    $NDeadHardest = $procdef->Steps[$StepId]["DeadHardest"];

    // Pega se há campos Definidos como Start do DeadTime no Passo
    $NDeadSoftField = $procdef->Steps[$StepId]["DeadSoftField"];
    $NDeadHardField = $procdef->Steps[$StepId]["DeadHardestField"];
    $NDeadHardestField = $procdef->Steps[$StepId]["DeadHardestField"];


    if ($NDeadSoftField > 0) {
        $NDeadSoft = 1;
    }

    if ($NDeadHardField > 0) {
        $NDeadHard = 1;
    }

    if ($NDeadHardestField > 0) {
        $NDeadHardest = 1;
    }

    if ($NDeadSoft == 0) {
        $NDeadSoft = $procdef->DeadSoft;
    }

    if ($NDeadHard == 0) {
        $NDeadHard = $procdef->DeadHard;
    }

    if ($NDeadHardest == 0) {
        $NDeadHardest = $procdef->DeadHardest;
    }

    // Pega uma unica data para todos os DeadTimes
    $dataCalculo = Date("Y-m-d H:i");

    // Verifica a diferença de tempo 
    $DiffSoft = date_diff_i($DeadSoft, $dataCalculo);
    $DiffHard = date_diff_i($DeadHard, $dataCalculo);
    $DiffHardest = date_diff_i($DeadHardest, $dataCalculo);

    $EstouradoSoft = $DiffSoft > 0 & !!($NDeadSoft);
    $EstouradoHard = $DiffHard > 0 & !!($NDeadHard);
    $EstouradoHardest = $DiffHardest > 0 & !!($NDeadHardest);

    // Se não for AdHoc Testa os outros DeadTimes do Caso Na fila
    if ($AdHoc !== 1) {
        $EstouradoHard = $DiffHard > 0 & $NDeadHard;
        $EstouradoHardest = $DiffHardest > 0 & $NDeadHardest;
    }

    $Estourado = $EstouradoSoft | $EstouradoHard | $EstouradoHardest;
    return $Estourado;
}

// </editor-fold>


$userdef = $_SESSION["userdef"];
if (!empty($_GET["PaginaAtual"])) {
    $PaginaAtual = $_GET["PaginaAtual"];
}

if (!empty($_POST["PaginaAtual"])) {
    $Pagina = $_POST["PaginaAtual"];
}
if (isset($_SESSION["procdef"])) {
    $procdef = $_SESSION["procdef"];
    if ($procdef->ProcId != $ProcId & !empty($ProcId)) {
        $procdef = new procdef();
        $procdef->Create($ProcId, $connect);
        $ProcId = $procdef->ProcId;
        $_SESSION["procdef"] = $procdef;
    } else {
        $ProcId = $procdef->ProcId;
    }
} else {
    $procdef = new procdef();
    $procdef->Create($ProcId, $connect);
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="**** Seleciona os Casos na Fila">
if (!isset($StepFiltro)) {
    $StepFiltro = "";
}

if (is_array($filtros)) {
    if (is_array($filtros["order"])) {
        $ordenacaoCasos = $filtros["order"]["ordem"];
        $campoOrdem = $filtros["order"]["campo"];
    } else {
        $campoOrdem = "";
        $ordenacaoCasos = $filtros["ordem"];
    }
} else {
    $ordenacaoCasos = "";
}

$filtros_where = $filtros["where"];
if (is_array($filtros_where)) {
    $procdef->filterQueue = $filtros_where;
} else {
    if ($filtros_where === "clearFilter")
    {
        $procdef->filterQueue = "";
        $filtros_where = "";
    } else {
        $filtros_where = $procdef->filterQueue;
    }
}
$_SESSION["procdef"] = $procdef;

$QUERY = pegaListaCasosNaFila($ProcId, $StepId, 1, $campoOrdem, $ordenacaoCasos, $userdef->Origem, $filtros_where, 1);
$TotalRegistros = mysqli_num_rows($QUERY);

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Posiciona o Registro para paginacao">
$inicioPagina = $filtros["paginacaoinicio"];
if ($inicioPagina > 0) {
    if (!mysqli_data_seek($QUERY, $inicioPagina)) {
        error_log("Cannot seek to row $inicioPagina: " . mysqli_error());
    }
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Popula array de casos">
$StepOld = "";
$StepCodeOld = "";
$Queue = array();
$ACaseNum = array();
$AReferencias = array();
$ListaCasos = array();
$ContadorCasos = 0;
$limiteLinhas = $filtros["pagincacaolimite"];
$stepId_Old = 0;
$procId_old = 0;
while ($result = mysqli_fetch_array($QUERY)) {
    if ($ContadorCasos >= $limiteLinhas) {
        break;
    }

    if ($ProcIdQueue == $result["ProcId"] & $StepId == $result["StepId"] & $Action == 1 & $result["Action"] == 0 & $CaseNum == $result["CaseId"]) {
        continue;
    }

    $ContadorCasos++;
    $XmlCaso = array();
    $CaseNum = $result["CaseId"];
    $AdHoc = $result["AdHoc"];

    // Calcula se estorou os dead times
    $Estourado = CalculoDeadTimeExtourado($result, $AdHoc);


//    if (!isset($AReferencias[$CaseNum])) {
//        array_push($ListaCasos, $CaseNum);
//    }

    array_push($ListaCasos, $CaseNum);

    if ($ProcId == null) {
        $ProcIdQueue = $result["ProcId"];
        $ProcName = PegaNomeProc($ProcIdQueue);
    } else {
        $ProcIdQueue = $ProcId;
        $ProcName = $procdef->ProcName;
    }

    $StepId = $result["StepId"];
    if ($StepId == 0) {
        $StepId = PegaDefaultStep($ProcIdQueue);
    }

    if ($procdef->ProcId == $ProcIdQueue) {
        $StepName = $procdef->Steps[$StepId]["StepName"];
    } else {
        $StepName = PegaNomeStep($ProcIdQueue, $StepId);
    }

    $XmlCaso["procid"] = $ProcIdQueue;
    $XmlCaso["stepid"] = $StepId;
    $XmlCaso["stepname"] = $StepName;
    $XmlCaso["procname"] = $ProcName;
    $XmlCaso["instancename"] = $procdef->InstanceName;

    $XmlCaso["Action"] = PermissoesDoCasoNoPasso($ProcIdQueue, $StepId, $CaseNum);


    $lockedByUserId = $result["lockedbyid"];

    $XmlCaso["lock"] = array();
    $XmlCaso["lock"]["text"] = "";
    $XmlCaso["lock"]["locked"] = "0";
    if ($lockedByUserId != 0 & $lockedByUserId != $userdef->UserId) {
        $XmlCaso["lock"]["locked"] = $lockedByUserId;
        if ($AdHoc == 3 || $AdHoc == 0) {
            $XmlCaso["lock"]["adhock"] = 0;
            $nomeUsuario = PegaNomeUsuario($lockedByUserId);
            $XmlCaso["lock"]["text"] = "Em uso ou bloqueado por " . $nomeUsuario;
            $XmlCaso["lock"]["username"] = $nomeUsuario;
        } else {
            $XmlCaso["lock"]["adhock"] = 1;
            $XmlCaso["lock"]["text"] = "$CaseNum em AdHoc";
            $XmlCaso["lock"]["username"] = "";
        }
    }

//        $StepName = htmlspecialchars($StepName);
//        $XmlCaso["STEPNAME"] = $StepName;
//        $StepCodeOld = $StepCode;
//        $Step["CODE"] = $StepCode;
//        $Step["NAME"] = $StepName;
//        $Step["STEPID"] = $StepId;
//

    $XmlCaso["DEADTIME"] = $Estourado;

    if ($StepId != $stepId_Old) {
        $PageEdit = PegaPageEdit($StepId);
        $stepId_Old = $StepId;
    }

    $Acesso = AcessoCaso($ProcIdQueue, $StepId, $CaseNum, $ProcName, $PageEdit);

    $XmlCaso["ACESSOCASO"] = $Acesso;
    $XmlCaso["COLSPAN"] = $ColSpan;
    $XmlCaso["casenum"] = $CaseNum;
    $XmlCaso["referencias"] = "";
    $XmlCaso["textoreferencias"] = "";
    array_push($Queue, $XmlCaso);
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Pega Referencias">
PegaArrayReferencias($ProcId, $ListaCasos, 1, 1, $StepCode, false);
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Popula Referencias dos Casos">
for ($i = 0; $i < count($Queue); $i++) {
    $Queue[$i]["referencias"] = $AReferencias[$Queue[$i]["casenum"]];
    $separador = "";
    foreach ($AReferencias[$Queue[$i]["casenum"]] as $referencia) {
        $Queue[$i]["textoreferencias"] .= $separador . $referencia["FieldName"] . ": " . $referencia["FieldValue"];
        $separador = ", ";
    }
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Final">
$retorno = $Queue;
$totalTabela = $TotalRegistros;
$registrosFiltrados = $TotalRegistros;
//$dadosRetorno = apiTrataDadosSaida($dadosEntrada, $retorno, $registrosFiltrados, $totalTabela);
$dadosRetorno = apiTrataDadosSaida($dadosEntrada, $retorno, $TotalRegistros, $TotalRegistros);
$jDados = json_encode($dadosRetorno);

//error_log(var_export($jDados, true));
header('Content-Type: application/json');
echo $jDados;
// </editor-fold>