<?PHP
require_once(FILES_ROOT . "include/common.php");
include(FILES_ROOT . "include/validasessao.php");
if (!isset($_SESSION["ProcIdAtivo"])) {
    $ProcIdAtivo = $_SESSION["ProcIdAtivo"];
}
#Ativa o Processo
if (!empty($ProcId)) {
    $ProcIdAtivo = $ProcId;
    $_SESSION["ProcIdAtivo"] = $ProcId;
} else {
    $ProcId = $ProcIdAtivo;
}
require_once("include/resource01.php");
$Menu = 4;
// Insere os Processos do Usuario
$SQL = "( select SD.StepId from stepaddresses as SA, stepdef as SD where SD.ProcId = $ProcId and SA.PRocId = SD.ProcId and SA.StepId = SD.StepId and GroupId = 192 and NewDoc = 1 and GrpFld = 'U' and Active = 1) 
			union 
			( select SD.StepId from stepaddresses as SA, stepdef as SD where SD.ProcId = $ProcId and SA.PRocId = SD.ProcId and SA.StepId = SD.StepId and GroupId in ($userdef->usergroups) and NewDoc = 1 and GrpFld = 'G' and Active = 1 )";
$QUERY_GRUPOS = mysqli_query($connect, $SQL);
if (mysqli_num_rows($QUERY_GRUPOS) == 1) {
    $result = mysqli_fetch_array($QUERY_GRUPOS);
    $StepId = $result["StepId"];
    $Action = "Edit";
    $EditCase_CaseNum = 0;
    $EditCase_StepId = $StepId;
    $EditCase_ProcId = $ProcId;
    $PageEdit = PegaPageEdit($StepId);
    include($PageEdit);
    exit();
}
if (!empty($StepId)) {
    $Action = "Edit";
    $Edit_CaseNum = 0;
    $PageEdit = PegaPageEdit($StepId);
    include($PageEdit);
    exit();
}

$ListaOpcoes = array();
while ($result = mysqli_fetch_array($QUERY_GRUPOS)) {
    $StepId = $result["StepId"];
    $StepName = PegaStepName($StepId, $ProcId);
    $Item["Descricao"] = $StepName;
    $Item["Nome"] = $StepName;
    $Item["href"] = "javascript:EnviarForm($StepId)";
    array_push($ListaOpcoes, $Item);
}
mysqli_free_result($QUERY_GRUPOS);
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
    <head>
        <script language="JavaScript" src="javascript/showdata.js"></script>
        <script language="JavaScript" src="javascript/posicionar.js"></script>
        <script language="javascript">
            function contextMenuEventHandler()
            {
                return false;
            }
            document.oncontextmenu = contextMenuEventHandler
            function muda(imagem, arquivo, texto)
            {
                window.status = texto;

                imga = new Image();

                imga.src = arquivo;

                imagem.src = imga.src;

            }//-->
            function PreparaPagina()
            {
                //	document.body.background = "images/background.gif";
                Posiciona();
            }

            function menu_highlight(objeto, texto)
            {
                document.getElementById(objeto).className = "tdmenuHighlight";
                document.getElementById("Seta" + objeto).src = "images/seta-menu-red.gif";
                window.status = texto;
            }

            function menu_normal(objeto, texto)
            {
                document.getElementById(objeto).className = "tdmenu";
                document.getElementById("Seta" + objeto).src = "images/seta-menu-gray.gif";
                window.status = texto;
            }

            function EnviarForm(StepId)
            {
                document.getElementById("StepId").value = StepId
                document.getElementById("formCriarNovo").submit();
            }

        </script>
        <title>Process.BR</title>
        <!-- <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"> -->
        <link href="STYLES/ListaProcesso.css" rel="stylesheet" type="text/css">
            <script language="JavaScript" type="text/JavaScript">
                //-->
            </script>
            <script language="JavaScript" src="javascript/CLOSE.JS"></script>
            <link href="styles/common.css" rel="stylesheet" type="text/css">
                <link href="styles/<?= $CustomizedCSS ?>" rel="stylesheet" type="text/css" />
                </head>
                <body onUnload="FecharSessao()">
                    <div class="DivHeaderCuston">
                        <div class="DivLogoCuston"></div>
                    </div>
                    <div id="DivMasterProcess"> 
                        <div class="DIVTITULOPAGINA">
                            <table>
                                <tr>
                                    <td class="TABLETITULOPAGINA" valign="middle">Processo&nbsp;-&nbsp;<?= $ProcName ?></td>
                                </tr>
                            </table>
                        </div>
                        <?php include("include/toolbar.php") ?>
                        <?php
                        if (!count($ListaOpcoes)) {
                            echo "<div class=\"InfCaso\">";
                            echo "$userdef->UserDesc não tem direitos para Iniciar Processos ";
                            echo "</div>";
                        } else {
                            ?>      
                            <table align="center">
                                <tr>
                                    <td></td>
                                    <td width="406">
                                        <div id="DivProcessos">
                                            <div class="titulo-menu">Passos de Novo Caso</div>
                                            <?php include("ListaOpcoes.php"); ?>
                                        </div>
                                    </td>		
                                </tr>    
                            </table>
                            <?php
                        }
                        ?>
                    </div>
                    <form name="formCriarNovo" id="formCriarNovo" action="BPMEditCase.php" method="POST">
                        <input type="hidden" name="ProcId" value="<?= $ProcId ?>">
                            <input type="hidden" name="StepId" id="StepId">
                                <input type="hidden" name="CaseNum" value="0">
                                    <input type="hidden" name="Action" value="Edit">
                                        <input type="hidden" name="ProcName" value="<?= $ProcName ?>">
                                            </form>
                                            </body>
                                            </html>