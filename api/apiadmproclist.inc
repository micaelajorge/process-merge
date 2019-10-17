<?php
//require_once("include/common.php");
include("include/validasessao.php");
require_once("include/resource01.php");
require_once("include/resource02.php");
require_once("include/sqlqueue.php");

$S_Processos = $_SESSION["S_Processos"];
if (count($S_Processos) == 0) {
    $Menu = 28;
    } else {
        $Menu = 16;
}
        
if (isset($_GET["ProcId"])) {
    $ProcId = $_GET["ProcId"];
}

if (isset($_GET["TipoAdmin"])) {
    $TipoAdmin = $_GET["TipoAdmin"];
    $_SESSION["TipoAdmin"] = $TipoAdmin;
} else {
    $TipoAdmin = $_SESSION["TipoAdmin"];
}
        
$_StepId = 0;
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
    $_SESSION["procdef"] = $procdef;                    
}

$ProcIdAtivo = $ProcId;
if (empty($Linhas)) {
    $Linhas = 5;
}

if ($Selecionado == "") {
    $Selecionado = 1;
}

if (empty($Pagina)) {
    $Pagina = 1;
}

$OrigemPesquisa = "admproc.php?Ac=$Ac&Selecionado=$Selecionado&Pagina=$Pagina";
$_SESSION["OrigemPesquisa"] = $OrigemPesquisa;
$PaginaPesquisa = "admproc.php";	



$InstanceName = PegaNomeInstancia($ProcId);
$Action = "View";
if ($Ac == 1) 
	{
	$Action = "Edit";
	}

$StepId = 0;

$ProcName = PegaNomeProc($ProcId);
$InstanceName = PegaNomeInstancia($ProcId);
$Armazenado = 0;

$ResultNumber = ($Pagina * $Linhas) - $Linhas ;
	 
function PegaCorLinha($Linha) {
    if ($Linha == "linha1") {
        $Linha = "Linha2";
    } else {
        $Linha = "linha1";
    }
    return $Linha;
}				

$inicio = ($Pagina * $Linhas) - $Linhas;		

$URLSearch = "Adm0002List.php";

PegaCamposReferenciaProcesso($ProcId);

$TipoAdministracao = "Administra&ccedil;&atilde;o Geral";

function PassosDoCaso($CaseNum)
	{
	global $connect;
	$SQL = "select StepName from casequeue, stepdef where CaseId = $CaseNum and casequeue.StepId > 0 and casequeue.AdHoc <> 2 and stepdef.ProcId = casequeue.ProcId and stepdef.StepId = casequeue.StepId and StepType not in ('M') order by casequeue.StepId";
	$Query = mysqli_query($connect, $SQL);
	if (mysqli_num_rows($Query) > 0)
		{
		while ($Result = mysqli_fetch_array($Query))
			{
			$Passos .= "<PASSOS><PASSO>" .  htmlspecialchars($Result["StepName"]) . "</PASSO></PASSOS>";
			}
		return "$Passos";
		}
	}
	
if (isset($_POST["StepFiltro"])) {	
    $StepFiltro = $_POST["StepFiltro"];
}	
	
if (!empty($_GET["Pagina"])) {
    $Pagina = $_GET["Pagina"];
}	

$CaseNum = 0;
if ($LimparFiltros == 1) {
    $Pesq = "";
    $Filtros = "";
    unset($Filtros);
    $_SESSION["Pesq"] = $Pesq;
    } else {
    if (count($_POST) > 0) {
        $Filtros = montaFiltros($_POST);
    }	
    $Pesq = $PesqT;
    $_SESSION["Pesq"] = $Pesq;
}

if (!isset($StepId)) {
    $StepId = $StepFiltro;
    $StepId = -1;
}

$TextoFiltro = "";
$EspacoFiltro = "";
if ($StepFiltro != '') {
    $TextoFiltro .= "Na fila da Tarefa = " . PegaStepName($StepFiltro, $ProcId);
    $EspacoFiltro = " ";
}
	
if (is_array($Filtros)){
    foreach ($Filtros as $Filtro) {
        $TextoFiltro .= $EspacoFiltro . $Filtro["Nome"] . " = " . $Filtro["ValorDisplay"] . " ";
        $EspacoFiltro = " ";
    }
}

if (is_array($Filtros)) {
    $AFiltros = TrataFiltros($Filtros, $ProcId);
}
	
$TextoFiltro = htmlspecialchars($TextoFiltro);

$SQLOrigeml = "";
$SQLOrigem2 = "";	


if ($TipoAdmin == "ProcAdmin") {
    $TipoAdministracao = "Administra&ccedil;&atilde;o";
    $SQLOrigeml .= " ,exportkeys,  ";
    $SQLOrigeml .= "  origemdominio ";
    $SQLOrigem2 =  " and ";
    $SQLOrigem2 .= " exportkeys.Origem = origemdominio.Origem_DOC ";
    $SQLOrigem2 .= " and ";
    $SQLOrigem2 .= " origemdominio.Origem_uSER = '$userdef->Origem' ";
    $SQLOrigem2 .= " and ";
    $SQLOrigem2 .= " exportkeys.ProcId = casequeue.ProcId ";
    $SQLOrigem2 .= " and ";
    $SQLOrigem2 .= " exportkeys.CaseNum = casequeue.CaseId ";	
}	
// Filtra a Origem dos Casos se ProcAdmin
// Totais

$SQLTotalAtivos = "select count(*) as Total from casequeue where StepId = 0 and CaseId in ";
$SQLTotalAtivos .= " (";
$SQLTotalAtivos .= " select CaseId from ";
$SQLTotalAtivos .= " casequeue ";
$SQLTotalAtivos .= $SQLOrigeml;
$SQLTotalAtivos .= " where casequeue.ProcId = $ProcId and StepId > 0 ";
$SQLTotalAtivos .= $SQLOrigem2;
$SQLTotalAtivos .= " group by CaseId ";
$SQLTotalAtivos .= "  ) ";
$QUERY = mysqli_query($connect, $SQLTotalAtivos);
$Result = mysqli_fetch_array($QUERY);
$TotalAtivos = $Result["Total"];
if (empty($TotalAtivos)) {
    $TotalAtivos = 0;
}

$SQLTotal = "select count(*) as Total from casequeue where CaseId in ";
$SQLTotal .= " (";
$SQLTotal .= " select CaseId from ";
$SQLTotal .= " casequeue ";
$SQLTotal .= $SQLOrigeml;
$SQLTotal .= " where casequeue.ProcId = $ProcId ";
$SQLTotal .= $SQLOrigem2;
$SQLTotal .= " group by CaseId having count(StepId) = 1 ";
$SQLTotal .= " ) ";

$QUERY = mysqli_query($connect, $SQLTotal);
$Result = mysqli_fetch_array($QUERY);
$TotalEncerrados = $Result["Total"];	
if (empty($TotalEncerrados)) {
    $TotalEncerrados = 0;
}

$SQL = "select Count(*) as Total, casequeue.ProcId, CaseId from casequeue ";
if ($TipoAdmin <> 'ProcAdmin') {
    if (is_array($AFiltros) || !empty($CampoOrdem)) {
    $SQL .= ", exportkeys ";
    }
}
$SQL .= $SQLOrigeml; 
$SQL .= " where casequeue.ProcId = $ProcId ";
$SQL .= $SQLOrigem2;

if (is_array($AFiltros)) {
    for ($i = 0; $i < count($AFiltros); $i++) {
        $SQL .= "and ";
        $SQL .= $AFiltros[$i];
    }
}

if (!empty($StepFiltro)) {
    $SQL .= "and ";
    $SQL .= "casequeue.StepId = $StepFiltro ";		
}
	
if ((is_array($AFiltros) && $TipoAdmin <> "ProcAdmin") || !empty($CampoOrdem)) {
    $SQL .= "and ";
    $SQL .= "CaseId = CaseNum ";
}	
$SQL .= " group by casequeue.ProcId, CaseId ";

if (!empty($CampoOrdem)) {
    $SQL .= ", $CampoOrdem ";
}

$SQLAtivos = " having Count(*) > 1 ";
$SQLEncerrados = " having Count(*) = 1 ";
if (is_numeric($StepFiltro)) {	
    $Selecionado = 0;
}

$SELECIONADOS = "<SELTODOS></SELTODOS>";

if ($Selecionado == 1) {
    $SQL .= $SQLAtivos;
    $SELECIONADOS = "<SELATIVOS></SELATIVOS>";
}

if ($Selecionado == 2) {
    $SQL .= $SQLEncerrados;
    $SELECIONADOS = "<SELENCERRADOS></SELENCERRADOS>";
}
	
if (!empty($CampoOrdem)) {
    $SQL .= " order by $CampoOrdem $Ordem";
}

$QUERY = mysqli_query($connect, $SQL);
$TotalRegistros = mysqli_num_rows($QUERY);
if (mysqli_num_rows($QUERY) > 0) {
    mysqli_data_seek($QUERY,$Linhas * ($Pagina - 1));
}

function AcessoCaso($CaseNum) {
    global $Action, $Locked, $LockedBysamaccountname, $AdHoc, $Pagina, $StepId, $ProcId, $procdef, $PageEdit, $TipoAdmin;
    $CaseNumFormatado = formataCaseNum($CaseNum, 5);
    $Acao = $Action;
    $ValorAcesso = str_replace("'", "/*%/*", $Valor);
    $ValorAcesso = str_replace(";", "/*#/*", $ValorAcesso);
    $ProcName = htmlentities($ProcName);
    $Acesso = "AcessaCaso('$Campo', '$ValorAcesso', 0, $ProcId, $CaseNum, 0, '$ProcName', '$TipoAdmin', $Pagina, '$PageEdit')";
    return $Acesso;
}

$userdef = $_SESSION["userdef"];

if (!empty($_GET["Pagina"])) {
    $Pagina = $_GET["Pagina"];
}

if (!empty($_POST["Pagina"])) {
    $Pagina = $_POST["Pagina"];
}

$StepNameOld = "";
$ContadorCasos = 1;
$PageEdit = "admCase.php";
$PosCursor = $Linhas * ($Pagina - 1);
$_TotalLinhas = mysqli_num_rows($QUERY);
if ($_TotalLinhas > 0 && $PosCursor < $_TotalLinhas) {
    mysqli_data_seek($QUERY, $PosCursor);
} else {
    $Pagina = 1;
}


$StepOld = "";
$Steps = array();
$arrayCasos = array();
$casoAtual = array();
$caseNumOld = 0;
while ($result = mysqli_fetch_array($QUERY, MYSQLI_ASSOC)) {
    $CaseNum = $result["CaseId"];
    if ($ContadorCasos > $Linhas) {
        break;
    }
    $ContadorCasos++;
    if ($caseNum != $caseNumOld & $caseNumOld != 0) {
        array_push($arrayCasos, $casoAtual);
        $casoAtual = array();
        $casoAtual["Steps"] = array();
    }
    $casoAtual["casenum"] = $result["CaseId"];
      
    $StepId = $result["StepId"];
    $AdHoc = $result["AdHoc"];

    $result["stepcod"] = $procdef->Steps[$StepId]["StepCod"];
    $result["stepname"] = $procdef->Steps[$StepId]["StepName"];
    $result["referencias"] = PegaReferencias($ProcId, $CaseNum, 1, 1, $StepCode);
    
    $DeadHard = $result["deadharddatetime"];
    $DeadHardest = $result["deadhardestdatetime"];
    $NDeadSoft = $procdef->Steps[$StepId]["DeadSoft"];
    $NDeadHard = $procdef->Steps[$StepId]["DeadHard"];
    $NDeadHardest = $procdef->Steps[$StepId]["DeadHardest"];
    if ($result["DeadSoftField"] > 0) {
        $NDeadSoft = PegaValorCampo($ProcId, $CaseNum, $procdef->Steps[$StepId]["DeadSoftField"]);
    }

    if ($result["DeadHardField"] > 0) {
        $NDeadSoft = PegaValorCampo($ProcId, $CaseNum, $procdef->Steps[$StepId]["DeadHardField"]);
    }

    if ($result["DeadHardestField"] > 0) {
        $NDeadSoft = PegaValorCampo($ProcId, $CaseNum, $procdef->Steps[$StepId]["DeadHardestField"]);
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

    if ($DeadSoft == "") {
        $Estourado = false;
    } else {
        $Estourado = ((date_diff_i($DeadSoft, Date("Y-m-d H:i")) > 1) && ($NDeadSoft > 0));
    }

    if ($AdHoc <> 1) {
        if ($DeadHard == "" || $DeadHardest == "") {
            $Estourado = false;
        } else {
            $Estourado_HARD = ((date_diff_i($DeadHard, Date("Y-m-d H:i:s")) > 0) & ($NDeadHard > 1));
            $Estourado_HARDEST = ((date_diff_i($DeadHardest, Date("Y-m-d H:i:s")) > 0) & ($NDeadHardest > 1));
            $Estourado = $Estourado_HARD || $Estourado_HARDEST;
        }
    }

    $Locked = $result["Locked"];
    $Lock = "";
    if (($Locked <> 0)) {
        if ($AdHoc == 3 || $AdHoc == 0) {
            $Lock = "<LOCK>Caso em uso ou bloqueado por " . PegaNomeUsuario($Locked) . "</LOCK>";
        } else {
            $Lock = "<LOCKADHOC>Caso $CaseNum com AdHoc</LOCKADHOC>";
        }
    }

    $Acesso = AcessoCaso($CaseNum);
    $result["acessocaso"] = $Acesso;
    $result["lock"] = $Lock;
    array_push($casoAtual["Steps"], $result);
    $caseNumOld = $CaseNum;
    }
    
    echo json_encode($arrayCasos);
?>