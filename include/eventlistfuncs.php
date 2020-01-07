<?php
// //Versao 1.0.0 /Versao
$EventDesc[1] = "Caso Iniciado";
$EventDesc[2] = "Caso Processado";
$EventDesc[3] = "Caso Salvo";
$EventDesc[4] = "<span class=\"PassoIniciado\">Passo Iniciado</span>";
$EventDesc[6] = "<span class=\"PassoIniciado\">Passo Iniciado</span>";
$EventDesc[7] = "Download Arquivo";
$EventDesc[704] = "<span class=\"PassoIniciado\">Passo Iniciado</span>";
$EventDesc[706] = "<span class=\"PassoIniciado\">Passo Iniciado</span>";
$EventDesc[2000] = "<span class=\"PassoIniciado\">Macro Executada</span>";
$EventDesc[2001] = "<span class=\"PassoIniciado\">Macro Executada na edição pelo Administrador</span>";

//$EventDesc[8] = "Caso removido do Passo $Audit";
$EventDesc[9] = "Caso Salvo e Bloqueado";
$EventDesc[20] = "Caso Aberto";
$EventDesc[100] = "Caso não processado";
$EventDesc[200] = "Prazo 1 Expirou";
$EventDesc[201] = "Prazo 1 de AdHoc Expirou";
$EventDesc[202] = "Passo Iniciado pelo Prazo 1 ";
$EventDesc[300] = "Prazo 2 Expirou";
$EventDesc[301] = "Prazo 2 do AdHoc Expirou";
$EventDesc[302] = "Prazo 3 Expirou";
$EventDesc[303] = "Passo Iniciado pelo Prazo 2 ";
$EventDesc[304] = "Passo Iniciado pelo Prazo 3 ";
$EventDesc[305] = "Removido do Passo pelo Prazo 1";
$EventDesc[306] = "Removido do Passo pelo Prazo 2";
$EventDesc[307] = "Removido do Passo pelo Prazo 3";
$EventDesc[501] = "Condição";
$EventDesc[502] = "Dados Recebidos de Fonte Externa";
$EventDesc[503] = "Dados Recebidos do Formulario ";

$EventDesc[400] = "AdHoc criado";
$EventDesc[40] = "AdHoc redirecionado";
$EventDesc[402] = "AdHoc encerrado";
$EventDesc[500] = "Caso encerrado";
$EventDesc[600] = "Colocado no Passo";
$EventDesc[610] = "Caso em todos os passos bloqueado pelo Administrador";
$EventDesc[611] = "Caso no Passo bloqueado pelo Administrador";
$EventDesc[612] = "Caso no Passo desbloqueado pelo Administrador";
$EventDesc[613] = "Removido do Passo pelo Administrador";
$EventDesc[614] = "Removido de AdHoc pelo Administrador";
$EventDesc[617] = "Pedido de impressão de Comprovante";
$EventDesc[618] = "<span class=\"SaiuDaFila\">Caso Saiu do Passo</span>";
$EventDesc[619] = "Origem do caso alterada pelo Administrador";
$EventDesc[620] = "<span class=\"ColocadoNaFilaAdministrador\">Colocado no Passo pelo Administrador</span>";
$EventDesc[900] = "";
$EventDesc[1000] = "";
$EventDesc[1010] = "Campo Alterado";
$EventDesc[8010] = "<span class=\"SaiuDaFila\">Caso Removido do Passo</span>";
$EventDesc[8011] = "<span class=\"SaiuDaFila\">Remover do Passo</span>";
$EventDesc[2010] = "<span class=\"SaiuDaFila\">Caso Removido do Passo</span>";
$EventDesc[2011] = "<span class=\"SaiuDaFila\">Remover do Passo</span>";


$EventDesc[21] = "Criando Novo Caso";
$EventDesc[22] = "Criado e Processado Novo Caso";
$EventDesc[23] = "Caso Alterado";
$EventDesc[24] = "Caso Alterado e Processado";
$EventDesc[25] = "Caso não estava no Passo";

function CriaEventDesc($ProcId, $CaseNum, $EventId, $StepName, $StepId, $StartStep, $ActionDesc, $ConditionName, $Audit, $TipoAdmin, $html = true)
{
    global $EventDesc;
    $Evento = $EventDesc[$EventId];
    switch ($EventId) {
        case 4:
        case 6:
            $NomePasso = trim($StartStep);
            if (empty($NomePasso)) {
                $NomePasso = PegaNomeStep($ProcId, $Audit);
            }
            if ($html) {
                $Evento .= "<br><span class=\"PassoIniciadoNome\">" . htmlentities($NomePasso) . "</span>";
            } else {
                $Evento .= $NomePasso;
            }
            break;
        case 7:
            $Evento = "Download Arquivo - $ActionDesc";
            break;
        case 8:
            $Evento = "Arquivo Apagado - $ActionDesc";
            break;
        case 704:
        case 706:
            $NomePasso = trim($StartStep);
            if (empty($NomePasso)) {
                $NomePasso = PegaNomeStep($ProcId, $Audit);
            }
            if ($html) {
                $Evento .= "<br><span class=\"PassoIniciadoNome\">" . htmlentities($NomePasso) . "</span><br />Caso já estava no passo";
            } else {
                $Evento .= "$NomePasso \n Caso já estava no passo";
            }
            break;

        case 900:
            if ($html) {
                $Evento = htmlentities($ActionDesc);
            } else {
                $Evento = $ActionDesc;
            }

            $NomeCondicao = trim($ConditionName);
            if ($TipoAdmin == "Admin" & is_numeric($Audit)) {
                if (empty($NomeCondicao)) {
                    $NomeCondicao = PegaNomeCondition($ProcId, $StepId, $Audit);
                }
                if ($html) {
                    $Evento = "Inserido pela Condição " . htmlentities($NomeCondicao) . "\n" . $Evento;
                } else {
                    $Evento = "Inserido pela Condição $NomeCondicao \n $Evento";
                }
            }
            break;

        case 5:
        case 5000:
            $NomeCondicao = trim($ConditionName);
            if (empty($NomeCondicao)) {
                $NomeCondicao = PegaNomeCondition($ProcId, $StepId, $Audit);
            }
            if ($html) {
                $Evento = "Avaliação da condição<br>" . htmlentities($NomeCondicao) . "\nResultado: <span class=\"text-green\">Verdadeiro</span>";
            } else {
                $Evento = "Avaliação da condição \n $NomeCondicao \n Resultado: Verdadeiro";
            }
            break;
        case 7:
        case 7000:
            $NomeCondicao = trim($ConditionName);
            if (empty($NomeCondicao)) {
                $NomeCondicao = PegaNomeCondition($ProcId, $StepId, $Audit);
            }
            if ($html) {
                $Evento = "Avaliação da condição<br>" . htmlentities($NomeCondicao) . "\n<span class=\"text-warning\">Resultado: Falso</span>";
            } else {
                $Evento = "Avaliação da condição \n$NomeCondicao\n CondicaoFalse\n Resultado: Falso";
            }
            break;

        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
            if ($html) {
                $Evento .= "<br>" . htmlentities($ActionDesc);
            } else {
                $Evento = $ActionDesc;
            }
            break;

        case 202:
        case 303:
        case 304:
            $NomePasso = trim($StartStep);
            if (empty($NomePasso)) {
                $NomePasso = PegaNomeStep($ProcId, $Audit);
            }
            if ($html) {
                $Evento .= "<br>$NomePasso";
            } else {
                $Evento .= "\n$NomePasso";
            }
            break;

        case 501:
            $NomeCondicao = trim($ConditionName);
            if (empty($NomeCondicao)) {
                $NomeCondicao = PegaNomeCondition($ProcId, $StepId, $Audit);
            }
            if ($html) {
                $Evento = $EventDesc[$EventId] . " <b>$ConditionName</b>\n $ActionDesc";
            } else {
                $Evento = $EventDesc[$EventId] . " $ConditionName \n$ActionDesc";
            }
            break;

        case 1000:
            $NomeCondicao = trim($ConditionName);
            if (empty($NomeCondicao)) {
                $NomeCondicao = PegaNomeCondition($ProcId, $StepId, $Audit);
            }
            if ($html) {
                $Evento = "Condição: <b>$ConditionName</b>\n" . htmlentities($ActionDesc);
            } else {
                $Evento = "Condição: $ConditionName \n $ActionDesc";
            }
            break;

        case 501:
            $Evento = htmlentities($ConditionName) . "\n";
            if ($html) {
                $Evento .= "\n" . htmlentities($ActionDesc);
            } else {
                $Evento .= "\n $ActionDesc";
            }
            break;
        case 502:
        case 503:
        case 1010:
        case 619:
            if ($html) {
                $Evento .= "\n" . htmlentities($ActionDesc);
            } else {
                $Evento .= "\n$ActionDesc";
            }
            break;

        case 899:
            $Evento = "Status do Processo alterado para <b>" . $StepName . " </b";
            break;
            
        case 2000:
        case 2001:
            if ($html) {
                $Evento .= "<br><span class=\"text-primary\">$StepName</span>";
            } else {
                $Evento .= "\n$StepName";
            }
            break;

        case 8010:
        case 2010:
            $Evento .= "\n" . htmlentities($ActionDesc);
            break;

        case 2011:
        case 8011:
            if ($html) {
                $Evento .= "\n$ActionDesc<br /> Caso não estava no Passo";
            } else {
                $Evento .= "\n$ActionDesc\nCaso não estava no Passo";
            }
            break;            
        default:
            if (empty($Evento)) {
                $Evento = "Evento sem Descrição EventId: $EventId";
            }
            break;
    }
    return $Evento;
}