<?php


use raelgc\view\Template;

ob_start();

// <editor-fold defaultstate="collapsed" desc="Calculo DeadTime">    
function CalculoDeadTimeExtourado($result, $AdHoc = 0) {

    global $procdef;
    $StepId = $result["StepId"];
    $DeadSoft = $result["deadsoftdatetime"];
    $DeadHard = $result["deadharddatetime"];
    $DeadHardest = $result["deadhardestdatetime"];
    $NDeadSoft = $procdef->Steps[$StepId]["DeadSoft"];
    $NDeadHard = $procdef->Steps[$StepId]["DeadHard"];
    $NDeadHardest = $procdef->Steps[$StepId]["DeadHardest"];
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

    $Estourado = ((date_diff_i($DeadSoft, Date("Y-m-d H:i")) > 0) && ($NDeadSoft));
    if ($AdHoc != 1) {
        $Estourado = $Estourado || ((date_diff_i($DeadHard, Date("Y-m-d H:i")) > 0) & ($NDeadHard));
        $Estourado = $Estourado || ((date_diff_i($DeadHardest, Date("Y-m-d H:i")) > 0) & ($NDeadHardest));
    }

    return $Estourado;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Monta Funcao para acessar o Caso">
function AcessoCaso($CaseNum) {
    global $Action, $AdHoc, $PaginaAtual, $StepId, $ProcId, $procdef, $PageEdit;
    //$ValorAcesso = str_replace("'","/*%/*", $Valor);
    //$ValorAcesso = str_replace(";","/*#/*", $ValorAcesso);
    $ProcName = htmlspecialchars($procdef->ProcName);
    $Acesso = "AcessaCaso('', '', $AdHoc, $ProcId, $CaseNum, $StepId, '$ProcName', 'Queue', $PaginaAtual, '$PageEdit', $Action)";
    return $Acesso;
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Tratamento de Variaveis recebidas GET / POST">
$userdef = $_SESSION["userdef"];
if (!empty($_GET["PaginaAtual"])) {
    $PaginaAtual = $_GET["PaginaAtual"];
}

if (!empty($_POST["PaginaAtual"])) {
    $Pagina = $_POST["PaginaAtual"];
}

// </editor-fold>

$S_Processos = $_SESSION["S_Processos"];
$ProcName = htmlspecialchars($ProcName);
foreach ($S_Processos as $Processo) {
    if ($Processo["ProcId"] == $ProcId) {
        break;
    }
}

// <editor-fold defaultstate="collapsed" desc="Defini��o do Template de Saida">
// Gerar Template como XML
$t_body = new Template("templates/template_queue.html");
$t_body->addFile("TOOLBAR", "templates/template_toolbar.html");

// </editor-fold>

$t_body->PROCCOLOR = $Processo["ProcColor"];

// <editor-fold defaultstate="collapsed" desc="Define o Icone do Processo">
$t_body->PROCICON = "fas fa-tasks";
if ($Processo["ProcIcon"] != "") {
    $t_body->PROCICON = $Processo["ProcIcon"];
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Come�o Criacao do Template">
$t_body->PROCESSODESC = "Processo - " . $Processo["ProcDesc"];
$t_body->TITULOPAGINA = htmlspecialchars($ProcessName) . "Processo " . htmlspecialchars($ProcName) . " " . htmlspecialchars($ProcessInstance);
$t_body->CUSTOMCSS = "styles/$CustomizedCSS";
$t_body->QUEUECSS = "styles/$BPMQueueXMLcss";
$t_body->INSTANCIA = "N�mero $InstanceName";
$t_body->INSTANCENAME = $InstanceName;
$t_body->NOMEPROC = $ProcName;
$t_body->USERNAME = $userdef->UserName;
$t_body->PAGINAATUAL = $PaginaAtual;
$t_body->PROCID = $ProcId;
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Pega Passos Novo caso">
$SQL = "( select SD.StepId, StepName, PageAction from stepaddresses as SA, stepdef as SD where SD.ProcId = $ProcId and SA.PRocId = SD.ProcId and SA.StepId = SD.StepId and GroupId = $userdef->UserId and NewDoc = 1 and GrpFld = 'U' and Active = 1) 
		union 
		( select SD.StepId, StepName, PageAction from stepaddresses as SA, stepdef as SD where SD.ProcId = $ProcId and SA.PRocId = SD.ProcId and SA.StepId = SD.StepId and GroupId in ($userdef->usergroups) and NewDoc = 1 and GrpFld = 'G' and Active = 1 )";
$QUERY_GRUPOS = mysqli_query($connect, $SQL);
$TotalPassosNovoDoc = mysqli_num_rows($QUERY_GRUPOS);

switch ($TotalPassosNovoDoc) {
    case 0:
        $Menu = 48;
        if (count($S_Processos) == 1 && !$userdef->Admin && !$ForcarSelecaoProcesso) {
            $Menu = 49;
        }
        break;

    case 1:
        $Menu = 1;
        if (count($S_Processos) == 1 && !$userdef->Admin && !$ForcarSelecaoProcesso) {
            $Menu = 27;
        }
        $Result = mysqli_fetch_array($QUERY_GRUPOS);
        $StepIdNovoCaso = $Result["StepId"];
        $t_body->STEPIDNOVOCASO = $StepIdNovoCaso;
        $t_body->block("BLOCK_STEPIDNOVOCASO");
        break;

    default:
        $Menu = 50;
        if (count($S_Processos) == 1 && !$userdef->Admin && !$ForcarSelecaoProcesso) {
            $Menu = 51;
        }
        while ($Result = mysqli_fetch_array($QUERY_GRUPOS)) {
            $NovoCasoStepId = $Result["StepId"];
            $NovoCasoStepName = $Result["StepName"];
            
            // Define a P�gina de Edi��o do Caso
            $PageEdit = (empty($Result["PageAction"])) ? $Result["PageAction"] : "BPMEditCase.php";
                
            $t_body->STEPIDNOVOCASO = $StepIdNovoCaso;
            $t_body->NOVOCASOACAO = "NovoCaso(0, '', '', $ProcId, 0, '$ProcName', 'Queue', $PaginaAtual, '$PageEdit', 'Edit')";
            $t_body->NOVOCASOSTEPID = $NovoCasoStepId;
            $t_body->NOVOCASOSTEPNAME = $NovoCasoStepName;
            $t_body->block("BLOCK_STEPIDNOVOCASO");
        }        
        break;
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Pega Passos Usuario">
$Steps = PassosDoUsuario($ProcId, $userdef->Origem, $Filtros);
// </editor-fold>

// // <editor-fold defaultstate="collapsed" desc="Popula Toolbar">
//

include("include/Toolbarcreate.php");

$t_body->MENU = $Menu;
        
// Coloca os Bot�es a Esquerda
for ($i = 0; $i < 5; $i++) {
    if (!empty($acao[$i])) {
        if ($Collapse[$i] == false) {
            $t_body->BUTTON_ACTION = "onclick=\"" . $acao[$i] . "\"";
            $t_body->BUTTON_IMAGEM = $Imagem[$i];
            $t_body->BUTTON_TEXT = $texto[$i];
            $t_body->BUTTON_TITLE = $textoDesc[$i];
            if (isset($class_Icon[$i])) {
                $t_body->BUTTON_ICON = $class_Icon[$i];
                $t_body->block("BLOCK_BOTAO_ICON_LEFT");
            }
            $t_body->block("BLOCK_BOTAO_LEFT");
        } else {
            $t_body->BUTTON_IMAGEM = $Imagem[$i];
            $t_body->BUTTON_TEXT = $texto[$i];
            $t_body->BUTTON_TITLE = $textoDesc[$i];
            if (isset($class_Icon[$i])) {
                $t_body->BUTTON_ICON = $class_Icon[$i];
                $t_body->block("BLOCK_BOTAO_ICON_COLLAPSE");
            }
            $t_body->TARGETCOLLAPSE = $Target[$i];
            $t_body->block("BLOCK_BOTAO_COLLAPSE");            
        }
    }
}

// Coloca o Ultimo Bot�o a Direita
if (!empty($acao[5])) {
    $t_body->BUTTON_ACTION = "onclick=\"" . $acao[5] . "\"";
    $t_body->BUTTON_IMAGEM = $Imagem[5];
    $t_body->BUTTON_TEXT = $texto[5];
    $t_body->BUTTON_TITLE = $textoDesc[5];

    if (isset($class_Icon[5])) {
        $t_body->BUTTON_ICON = $class_Icon[5];
        $t_body->block("BLOCK_BOTAO_ICON_RIGHT");
    }
    $t_body->block("BLOCK_BOTAO_RIGHT");
}

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Filtros Queue">
if (is_array($Filtros)) {
    //echo "\t<FILTROS>$TextoFiltro</FILTROS>\n";
}

if (!empty($Filtros)) {
    while ($Filtro = each($Filtros)) {
        $TextoFiltro .= $Filtro["value"]["Nome"] . " = " . $Filtro["value"]["ValorDisplay"] . " ";
    }
    //echo "<filtros>$TextoFiltro</filtros>\n";
}
// Se o Passo atualmente selecionado n�o tem casos, coloca a sele��o no Primeiro Passo que o Usu�rio 
// tem acesso
$RemoverStepFiltro = true;
foreach ($Steps as $StepExiste) {
    if ($StepExiste["StepId"] == $StepFiltro) {
        $RemoverStepFiltro = false;
        break;
    }
}

if (!is_numeric($StepFiltro) || $RemoverStepFiltro) {
    if (count($Steps) > 0) {
        $StepFiltro = $Steps[0]["StepId"];
    }
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Ordena��o dos Casos">
if (!isset($CampoOrdem)) {
    $CampoOrdemStep = $procdef->Steps[$StepFiltro]["FieldOrder"];
    $OrdemStep = $procdef->Steps[$StepFiltro]["OrderDesc"];
    if ($CampoOrdemStep != "") {
        $CampoOrdem = "Campo" . PegaFieldIdByCode($ProcId, $CampoOrdemStep);
        if ($OrdemStep == 1) {
            $Ordem = "Desc";
        }
    }
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Seleciona os Casos na Fila">
if (!empty($userdef->UserId_Process)) {
    $QUERY = CasosNaFila($ProcId, $StepFiltro, 1, $CampoOrdem, $Ordem, $userdef->Origem, $Filtros, 1);
    $TotalRegistros = mysqli_num_rows($QUERY);
} else {
    $UserId = 0;
    $QUERY = CasosNaFila($ProcId, $StepFiltro, 1, $CampoOrdem, $Ordem, $userdef->Origem, $Filtros, 1);
    $TotalRegistros = mysqli_num_rows($QUERY);
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Calculo de Pagina��o">
$ContadorCasos = 1;
$_TotalLinhas = mysqli_num_rows($QUERY);
$PosCursor = $Linhas * ($Pagina - 1);
if ($_TotalLinhas > 0 && $PosCursor < $_TotalLinhas) {
    mysqli_data_seek($QUERY, $PosCursor);
} else {
    $Pagina = 1;
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Pega Referencias">
$CamposRef = $S_procdef["Referencias"];
$ColSpan = count($CamposRef) + 1;
for ($i = 0; $i < count($CamposRef); $i++) {
    if ("Campo" . $CamposRef[$i]["FieldId"] == $CampoOrdem)
        if ($Ordem == "Desc") {
            $ImagemRef = "seta_baixo.gif";
            $Ordem_Form = '';
        } else {
            $ImagemRef = "seta_cima.gif";
            $Ordem_Form = 'Desc';
        } else
        $ImagemRef = "seta_cima_gray.gif";

    if (!isset($Ordem_Form)) {
        $Ordem_Form = "";
    }
    $MudaOrdem = "MudaOrdem('Campo" . $CamposRef[$i]["FieldId"] . "', '$Ordem_Form')";
    $Code = $CamposRef[$i]["FieldCod"];
    //$Name = htmlentities($CamposRef[$i]["FieldName"]);
    $Name = htmlspecialchars($CamposRef[$i]["FieldName"]);

    $t_body->FIELDREFNAME = $Name;
    $t_body->FIELDREFCODE = $Code;
    $t_body->FIELDREFIMG = "images/" . $ImagemRef;
    $t_body->FIELDREFID = $CamposRef[$i]["FieldId"];
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Pega Casos na Fila">
$StepOld = "";
$StepCodeOld = "";
$Queue = array();
$ACaseNum = array();
while ($result = mysqli_fetch_array($QUERY)) {
    $XmlCaso = "";
    if ($ContadorCasos > $Linhas) {
        break;
    }
    $CaseNum = $result["CaseId"];
    $AdHoc = $result["AdHoc"];
    $Estourado = CalculoDeadTimeExtourado($result, $AdHoc);
    $ContadorCasos++;
    if (!isset($AReferencias[$CaseNum])) {
        array_push($ACaseNum, $CaseNum);
    }

    $StepId = $result["StepId"];
    if ($StepId == 0) {
        $StepId = PegaDefaultStep($ProcId);
    }
    $PageEdit = PegaPageEdit($StepId);

    $Action = $result["Action"];
    $StepCode = $procdef->Steps[$StepId]["StepCod"];
    $StepName = $procdef->Steps[$StepId]["StepName"];

    $Locked = $result["lockedbyid"];

    $Lock = "";
    if (($Locked <> 0)) {
        if ($AdHoc == 3 || $AdHoc == 0) {
            $XmlCaso["LOCK"] = "Caso em uso ou bloqueado por " . PegaNomeUsuario($Locked);
        } else {
            $XmlCaso["LOCKADHOC"] = "Caso $CaseNum com AdHoc";
        }
    }

    if ($StepCodeOld != $StepCode) {
        $StepName = htmlspecialchars($StepName);
        $XmlCaso["STEPNAME"] = $StepName;
        $StepCodeOld = $StepCode;
        $Step["CODE"] = $StepCode;
        $Step["NAME"] = $StepName;
        $Step["STEPID"] = $StepId;
    }

    if ($Estourado) {
        $XmlCaso["DEADTIME"] = 1;
    }

    $Acesso = AcessoCaso($CaseNum);
    $XmlCaso["ACESSOCASO"] = $Acesso;
    $XmlCaso["COLSPAN"] = $ColSpan;
    $XmlCaso["CASENUM"] = $CaseNum;
    array_push($Queue, $XmlCaso);
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Pega Campos Referencias ">
PegaArrayReferencias($ProcId, $ACaseNum, 1, 1, $StepCode);
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Popula Campos Referencia">
foreach ($S_procdef["Referencias"] as $CampoReferencia) {
    $t_body->FIELDREFNAME = $CampoReferencia["FieldName"];
    $t_body->FIELDREFCODE = $CampoReferencia["FieldCode"];
    $t_body->FIELDREFIMG = "images/seta_cima.gif";
    $t_body->FIELDREFID = $CampoReferencia["FieldId"];
    $t_body->block("BLOCK_REFERENCIAS");
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Percorre os Casos">
foreach ($Queue as $Dados) {
    $CaseNum = $Dados["CASENUM"];
    $Campos = $AReferencias[$CaseNum];
    $t_body->CASENUM = $CaseNum;

    if (!empty($Dados["LOCK"])) {
        $t_body->LOCK = $Dados["LOCK"];
        $t_body->block("BLOCK_LOCK");
    }

    if ($Dados["DEADTIME"]) {
        $t_body->DEADTIME = $Dados["DEADTIME"];
        $t_body->block("BLOCK_DEADTIME");
    }

    $t_body->COLSPAN = $Dados["COLSPAN"];
    $t_body->ACESSOCASO = $Dados["ACESSOCASO"];

    if (is_array($Campos)) {
        foreach ($Campos as $Ref) {
            $Valor = $Ref["FieldValue"];
            $Code = $Ref["FieldCod"];

            $t_body->CODE = $Code;
            if ($Ref["Empty"]) {
                $t_body->SCRIPT = $Valor;
                $t_body->block("BLOCK_SCRIPT");
            } else {
                $t_body->VALUE = $Valor;
                $t_body->block("BLOCK_VALOR");
            }
            $t_body->FIELDID = 1;
            $t_body->CASENUM > $CaseNum;
            $t_body->block("BLOCK_CAMPOS");
        }
    }
    $t_body->block("BLOCK_CASO");
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Passos Para sele��o na queue">
$DescPassos = "N�o h� casos no Processo";
if (count($Steps) > 0) {
    $DescPassos = "Passos";
    reset($Steps);
    foreach ($Steps as $Step) {
        $t_body->STEPID = $Step["StepId"];
        $t_body->STEPNAME = htmlentities($Step["StepName"]);
        if ($StepFiltro == $Step["StepId"]) {
            $t_body->SELECTED = 1;
            $t_body->STEPNAMESEL = htmlentities($Step["StepName"]);
            $t_body->STEP_ACTIVE = "active";
        } else {
            $t_body->SELECTED = 0;
            $t_body->STEP_ACTIVE = "";
        }
        $t_body->block("BLOCK_VALOR_PASSOS");
        $t_body->DESCPASSOS = $DescPassos;
        $t_body->block("BLOCK_SELECAO_PASSOS");
    }
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Pagina��o da Queue">

$Paginacao = IndicePaginas($PaginaAtual, $TotalRegistros, $Linhas, "BPMQueue.php", "Visualizar=$Visualizar", true);

foreach ($Paginacao as $IndicePagina) {
    $t_body->PAGINAINDICE = $IndicePagina["pagina"];
    $t_body->PAGINAACAO = $IndicePagina["acao"];
    $t_body->PAGINACLASS = $IndicePagina["class"];
    $t_body->block("BLOCK_PAGINAS");
}

// </editor-fold>

$IncluirSelecaodePasso = false;

// <editor-fold defaultstate="collapsed" desc="Preenche Formul�rio de Pesquisa">
for ($i = 0; $i < count($CamposRef); $i++) {
    $Campo = 'FCampo' . $CamposRef[$i]["FieldId"];
    $t_body->CAMPOPESQUISANOME = $CamposRef[$i]["FieldName"];
    $t_body->CAMPOPESQUISAID = "Field" . $CamposRef[$i]["FieldId"];

    if ($CamposRef[$i]["FieldType"] == "US") {
        
    }
    if ($CamposRef[$i]["FieldType"] == "DT") {
        //IncluiCalendarioXML("FCampo" . $CamposRef[$i]["FieldId"]);
    }
    $t_body->block("BLOCO_CAMPO_PESQUISA");
}

// </editor-fold>
//include("BPMSearchFormXML.php");
ob_clean();

$parse = $t_body->parse();

// <editor-fold defaultstate="collapsed" desc="Envia o Resultado para um arquivo (DESATIVADO)">
/*
  $myFile = "output_raw.xml";
  $fh = fopen($myFile, 'w');
  fwrite($fh, $parse);
  fclose($fh);

 */
// </editor-fold>

echo $parse;
?>

