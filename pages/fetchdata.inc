<?php
// TODO: Não Está gravando o Usuário no exportkeys
// TODO: Não está atualizando campo Data baseado na ABERTURA DO CASO
include(FILES_ROOT . "include/config.config.php");
require_once(FILES_ROOT . "include/common.php");
require_once(FILES_ROOT . "include/resource01.php");
require_once(FILES_ROOT . "pages/ostepdoc.inc");

$contador = 1;

$post = filter_input_array(INPUT_POST);
if (key_exists("CaseNum", $_POST)) {
    if ($LogarProcessamento) {
        error_log('');
        error_log('#################################################');
        error_log("Passo: '$StepId'");
        error_log("Usuário: '$userdef->UserName'");
    }
    $CaseNum = $_POST["CaseNum"];
    $ProcId = $_POST["ProcId"];
    $StepId = $_POST["StepId"];
    $Caso = new STEPDOC;
    $Caso->SetConnection($connect);
    $Caso->SetProc($ProcId);
    $Caso->SetStep($StepId);
    $Caso->open();
    $Caso->UserId = $userdef->UserId;
    $Caso->UserName = $userdef->UserName;
    $Caso->UserDesc = $userdef->UserDesc;
    $Caso->samaccountname = $userdef->UserName;
    //$Caso->NovoCaso = $false;
    $Caso->NovoCaso = $NewCaseCreate;
    $Caso->Origem = $userdef->Origem;
    $Caso->SetCaseNum($CaseNum);
    $Caso->UserDesc = $userdef->UserDesc;

    // Verifica se o caso continua na fila
    if ($Caso->CasoEstaNaFila()) {

        // Pega os Dados enviados pelo formulario        
        $Caso->PegaDadosDePost($post);

        // Seta a Prioridade do Caso
        $Caso->SetPrioridade($Prioridade);
    } else {
        $Acao = "Ignorar";
    }

    // Executa a ação recebida do caso
    $Caso->SetAction($Acao);

    // Verifica se é uma página costumizada
    if ($Custon <> 1) {
        if (empty($Retornar)) {
            if ($userdef->UserId <> 0) {
                $Retornar = "BPMQueue.php";
            } else {
                $Retornar = "agradece.php?CaseNum=$Caso->CaseNum";
            }
        }
    }

    $CaseNum = $Caso->CaseNum;
    if ($Acao == "AdHoc") {
        $Retornar = $MontarAdHoc;
    }
}

// Verifica o que executar apos processar o caso
switch ($AcaoAposProcessar) {
    case "reeditarCT":
        include_once(FILES_ROOT . "pages/editcase.inc");
        return;
    // Edição de Cabecalho de CT
    case "SalvaPagina":
        include_once(FILES_ROOT . "pages/editcase_cabecalho.inc");
        return;

    // Edita o proximo caso na fila
    case "ProcessarNext":
        $CaseNum = 0;
        // Verifica se há casos na Mesma Fila
        $proximoCaso = PegaProximoCasoFila($ProcId, $StepId);
        if ($proximoCaso) {
            $CaseNum = $proximoCaso["CaseId"];
            include(FILES_ROOT . "pages/editcase.inc");
            exit;
        }
        break;

    // Edita o proximo caso na fila
    case "ProcessarNextAll":
        $CaseNum = 0;
        // Verifica se há casos na Mesma Fila
        $proximoCaso = PegaProximoCasoFila($ProcId, $StepId);
        if ($proximoCaso) {
            $CaseNum = $proximoCaso["CaseId"];
        } else {
            // Verifica se há casos no Processo
            $proximoCaso = PegaProximoCasoFila($ProcId);
            if ($proximoCaso) {
                $CaseNum = $proximoCaso["CaseId"];
                $StepId = $proximoCaso["StepId"];
            }
        }
        // Se o caso é maior que zero, processa o caso
        if ($CaseNum > 0) {
            include(FILES_ROOT . "pages/editcase.inc");
            exit;
        }
        break;

    // Criar novo caso
    case "ProcessarNew":
        $CaseNum = 0;
        include_once(FILES_ROOT . "pages/editcase.inc");
        return;
        break;
}
if (!isset($PaginaQueueOriginal)) {
    $PaginaQueueOriginal = $_SESSION["PaginaQueueOriginal"];
}
//include($PaginaQueueOriginal);
//include("BPMQueue.php");
?>
<html>
    <script>
        document.location = '<?= $PaginaQueueOriginal ?>';
    </script>
</html>