<?php

use raelgc\view\Template;

define("MENU_PROCESSAR_PROXIMO", 55);
define("MENU_ROTEAR_PROCESSO", 58);
define("MENU_ROTEAR_NOVO_PROCESSO", 59);

function populaToolBarTemplate(Template $t_body, $menu, $optionsMenu = array())
{    
    $botoesToolbar = definicaoToolBar($menu);

    $t_body->Menu = "Menu - $menu";
// Coloca os Botões a Esquerda
    $contadorBotoes = 0;
    foreach ($botoesToolbar as $botaoToolbar) {
        if ($contadorBotoes == count($botoesToolbar) - 1) {
            break;
        }
        $contadorBotoes++;

        // Se não tiver ação definida pula o botão
        if (empty($botaoToolbar["acao"])) {
            continue;
        }

        $t_body->BUTTON_TEXT = $botaoToolbar["texto"];

        $tituloBotao = $botaoToolbar["textoDesc"];
        if ($botaoToolbar["textoDesc"] == "") {
            $tituloBotao = $botaoToolbar["texto"];
        }
        $t_body->BUTTON_TITLE = $tituloBotao;
        $t_body->BUTTON_ICON = $botaoToolbar["class_Icon"];
        $t_body->BUTTON_ACTION = "onclick=\"" . $botaoToolbar["acao"] . "\"";

        $tipoBotao = "";

//      Se for botão que espande 
        switch ($botaoToolbar["TipoBotao"]) {
            case "Collapse":
                $tipoBotao = "COLLAPSE";
                break;

            case "showModal":
                $tipoBotao = "MODAL";
                break;

            case "DROPDOWN_ITENS":
                $tipoBotao = "DROPDOWN_ITENS";
                break;

            default:
                $tipoBotao = "LEFT";
                break;
        }
        $t_body->BUTTON_ICON = $botaoToolbar["class_Icon"];
        $t_body->block("BLOCK_BOTAO_ICON_$tipoBotao");

        //Coloca Opções do Menu
        if (is_array($botaoToolbar["opcoesMenu"])) {
            foreach ($botaoToolbar["opcoesMenu"] as $opcaoMenu) {
                $t_body->OPCAO_TEXTO = $opcaoMenu["texto"];
                $t_body->OPCAO_ICONE = $opcaoMenu["icone"];
                $t_body->OPCAO_ACAO = $opcaoMenu["acao"];
                $t_body->OPCAO_TITULO = $opcaoMenu["titulo"];
                $t_body->OPCAO_CLASSE = $opcaoMenu["classe"];
                $t_body->block("MENU_TREE_BLOCK_ITEM");
            }
            $t_body->block("BLOCK_MENU_TREE");
        }
        if ($botaoToolbar["make_block"] != "") {
            $t_body->block($botaoToolbar["make_block"]);
        }
        $t_body->TOOLBAR_BUTTON_ID = $botaoToolbar["button_id"];
        $t_body->block("BLOCK_BOTAO_$tipoBotao");
    }

    //  Coloca o Ultimo Botão a Direita
    $botaoToolbar = $botoesToolbar[$contadorBotoes];

    if (!empty($botaoToolbar["acao"])) {
        $t_body->BUTTON_ACTION = "onclick=\"" . $botaoToolbar["acao"] . "\"";
        $t_body->BUTTON_TEXT = $botaoToolbar["texto"];

        $tituloBotao = $botaoToolbar["textoDesc"];
        if ($botaoToolbar["textoDesc"] == "") {
            $tituloBotao = $botaoToolbar["texto"];
        }
        $t_body->BUTTON_TITLE = $tituloBotao;

        if (isset($botaoToolbar["class_Icon"])) {
            $t_body->BUTTON_ICON = $botaoToolbar["class_Icon"];
            $t_body->block("BLOCK_BOTAO_ICON_RIGHT");
        }
        $t_body->block("BLOCK_BOTAO_RIGHT");
    }
}

// <editor-fold defaultstate="collapsed" desc="Texto toolbar">
function TextosToolBar001()
{
    global $Cancelar, $Atualizar, $Anterior, $_msgNovoDoc;
    global $SalvarDoc, $ProcessarDoc, $NovaMsg, $SalvarMsg, $EnviarMsg;
    global $PrintDoc, $Rascunhos, $SairPesq, $ReturnPesq, $FilterPesq;
    global $FiltroQueue, $LogOff, $NovoNota, $NovoAnexo, $AdHocButton, $InserirNaFila, $capturarImagem, $mostrarProcessos;
    global $EncerraAdHocButton, $TxRedirecionaAdHoc, $Historico, $EditarDados, $SalvarBloquear, $Relatorio, $permissoesProcessos, $configseguranca;
    global $GerarRelatorio, $OperacaoLote, $_msgFechar, $EditarUsuarios, $EditarGrupos, $textos_origemdominio, $CriarUsuarios, $CriarGrupo;

    $mostrarProcessos[0] = "Soluçoes";
    $mostrarProcessos[1] = "Mostrar todos as soluçoes do usuário";

    $capturarImagem[0] = "Capturar";
    $capturarImagem[1] = "Capturar Documento";

    $textos_origemdominio[0] = "Origens/Dominios";
    $textos_origemdominio[1] = "Editar Origens/Dominios de documentos";

    $permissoesProcessos[0] = "Permissões nas Soluções";
    $permissoesProcessos[1] = "Permissões nas Soluções";

    $EditarDados[0] = "Editar Dados";
    $EditarDados[1] = "Editar Dados";

    $InserirNaFila[0] = "Inserir na Fila";
    $InserirNaFila[1] = "Inserir na Fila";


    $Cancelar[0] = "Voltar";
    $Cancelar[1] = "Voltar para Tela anterior";

    $Rascunhos[0] = "Salvas";
    $Rascunhos[1] = "Mensagens Salvas";

    $Anterior[0] = "Voltar";
    $Anterior[1] = "Voltar para Tela Anterior";

    $_msgNovoDoc[0] = "Criar Novo";
    $_msgNovoDoc[1] = "Criar Novo";

    $SalvarDoc[0] = "Salvar";
    $SalvarDoc[1] = "Salvar";

    $ProcessarDoc[0] = "Concluido";
    $ProcessarDoc[1] = "Encerrar";

    $NovaMsg[0] = "Nova";
    $NovaMsg[1] = "Nova Mensagem";

    $SalvarMsg[0] = "Salvar";
    $SalvarMsg[1] = "Salvar Menssagem";

    $_msgFechar[0] = "Fechar";
    $_msgFechar[1] = "Fechar Janela";

    $EnviarMsg[0] = "Enviar";
    $EnviarMsg[1] = "Enviar Menssagem";

    $Atualizar[0] = "Atualizar";
    $Atualizar[1] = "Atualizar a Tela";

    $PrintDoc[0] = "Imprimir";
    $PrintDoc[1] = "Imprimir";

    $SairPesq[0] = "Sair";
    $SairPesq[1] = "Sair da Pesquisa";

    $ReturnPesq[0] = "Busca";
    $ReturnPesq[1] = "Busca de Itens";

    $FilterPesq[0] = "Filtro";
    $FilterPesq[1] = "Aplicar filtros nos resultados da Pesquisa";

    $FiltroQueue[0] = "Filtro";
    $FiltroQueue[1] = "Filtrar a Fila";

    $LogOff[0] = "Sair";
    $LogOff[1] = "Sair do Sistema";

    $NovoNota[0] = "Nova Nota";
    $NovoNota[1] = "Criar nova Nota";

    $NovoAnexo[0] = "Novo Anexo";
    $NovoAnexo[1] = "Anexar arquivo";

    $AdHocButton[0] = "Criar Ad Hoc";
    $AdHocButton[1] = "Criar Ad Hoc";

    $TxRedirecionaAdHoc[0] = "Redirecionar";
    $TxRedirecionaAdHoc[1] = "Redirecionar AdHoc";

    $EncerraAdHocButton[0] = "Responder AdHoc";
    $EncerraAdHocButton[1] = "Responder AdHoc";

    $Historico[0] = "Histórico";
    $Historico[1] = "Histórico";

    $SalvarBloquear[0] = "Bloquear e Salvar";
    $SalvarBloquear[1] = "Bloquear e Salvar";

    $GerarRelatorio[0] = "Gerar Relatório";
    $GerarRelatorio[1] = "Gerar Relatório";

    $Relatorio[0] = "Relatório";
    $Relatorio[1] = "Relatório";

    $OperacaoLote[0] = "Operações Lote";
    $OperacaoLote[1] = "Operações Lote";

    $EditarUsuarios[0] = "Edição de Usuários";
    $EditarUsuarios[1] = "Edição de Usuários";

    $CriarUsuarios[0] = "Criar Usuário";
    $CriarUsuarios[1] = "Criar Usuário";

    $CriarGrupo[0] = "Criar Grupo";
    $CriarGrupo[1] = "Criar Grupo";

    $EditarGrupos[0] = "Edição de Grupos";
    $EditarGrupos[1] = "Edição de Grupos";

    $configseguranca[0] = "Configurações Segurança";
    $configseguranca[1] = "Configurações Segurança";
}

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Definições TOOLBAR">
function definicaoToolBar($Menu)
{
    global $Classbtntoolbar, $words;
    global $Cancelar, $Atualizar, $Anterior, $_msgNovoDoc;
    global $SalvarDoc, $ProcessarDoc, $NovaMsg, $SalvarMsg, $EnviarMsg;
    global $PrintDoc, $Rascunhos, $SairPesq, $ReturnPesq, $FilterPesq, $capturarImagem, $mostrarProcessos;
    global $FiltroQueue, $LogOff, $NovoNota, $NovoAnexo, $AdHocButton, $InserirNaFila;
    global $EncerraAdHocButton, $TxRedirecionaAdHoc, $Historico, $EditarDados, $SalvarBloquear, $Relatorio;
    global $GerarRelatorio, $OperacaoLote, $_msgFechar, $MostrarBotoes, $permissoesProcessos, $configseguranca;
    global $CaseNum, $EditarUsuarios, $EditarGrupos, $textos_origemdominio, $CriarUsuarios, $CriarGrupo;
    global $opcoesMenu, $processosEscondidos, $S_Processos, $userdef, $TipoBotao, $make_block, $button_id, $workSpaceSelByUser;

    TextosToolBar001();

    $entrada = $_SESSION["entrada"];
    $PaginaAnterior = $_SESSION["PaginaAnterior"];
    $PaginaOrigem = $_SESSION["PaginaQueueOriginal"];

    if (empty($Classbtntoolbar)) {
        $Classbtntoolbar = "btntoolbar";
    }
    if (isset($words)) {
        $words = htmlspecialchars($words);
    }

// <editor-fold defaultstate="collapsed" desc="Incialização das Variaveis">
    $acao[0] = "";
    $acao[1] = "";
    $acao[2] = "";
    $acao[3] = "";
    $acao[4] = "";
    $acao[5] = "";

    $texto[0] = "&nbsp;";
    $texto[1] = "&nbsp;";
    $texto[2] = "&nbsp;";
    $texto[3] = "&nbsp;";
    $texto[4] = "&nbsp;";
    $texto[5] = "&nbsp;";

    $Imagem[0] = "";
    $Imagem[1] = "";
    $Imagem[2] = "";
    $Imagem[3] = "";
    $Imagem[4] = "";
    $Imagem[5] = "";

    $textoDesc[0] = "";
    $textoDesc[1] = "";
    $textoDesc[2] = "";
    $textoDesc[3] = "";
    $textoDesc[4] = "";
    $textoDesc[5] = "";

    $AcionaMenu[0] = "0";
    $AcionaMenu[1] = "0";
    $AcionaMenu[2] = "0";
    $AcionaMenu[3] = "0";
    $AcionaMenu[4] = "0";
    $AcionaMenu[5] = "0";

    $tipoBotao[0] = "";
    $tipoBotao[1] = "";
    $tipoBotao[2] = "";
    $tipoBotao[3] = "";
    $tipoBotao[4] = "";
    $tipoBotao[5] = "";

    $dropDown[0] = false;
    $dropDown[1] = false;
    $dropDown[2] = false;
    $dropDown[3] = false;
    $dropDown[4] = false;
    $dropDown[5] = false;


    $Target = array("", "", "", "", "", "");


// Icones Padrãoo para os Botões
    $class_Icon[0] = "fa fa-star";
    $class_Icon[1] = "fa fa-recycle";
    $class_Icon[2] = "fa fa-filter";
    $class_Icon[5] = "fa fa-arrow-circle-o-right";

// </editor-fold>
//    $Class[5] = "nvoltar";
    switch ($Menu) {
        case MENU_PROCESSAR_PROXIMO:
            $acao[0] = "Processar()";
            $texto[0] = $ProcessarDoc[0];
            $textoDesc[0] = $ProcessarDoc[1];
            $MouseOver[0] = "";
            $AcionaMenu[0] = 1;
            $class_Icon[0] = "fa fa-check";
            $Imagem[0] = " <img src=\"images/salvar.gif\" />";

            $acao[5] = "document.location = '$PaginaOrigem'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;



        /** Botoes para Novo Caso
         * 
         */
        case 1: // WF
            $acao[0] = "jsCriaNovoCaso('editcase', 0)";
            $texto[0] = $_msgNovoDoc[0];
            $textoDesc[0] = $_msgNovoDoc[1];
            $AcionaMenu[0] = 1;
            $Imagem[0] = "images/novo.gif";
            $class_Icon[0] = "fa fa-star";

            $acao[1] = "jsAtualizarQueue('queue')";
            $texto[1] = $Atualizar[0];
            $textoDesc[1] = $Atualizar[1];
            $AcionaMenu[1] = 1;
            $Imagem[1] = "images/atualizar.gif";
            $class_Icon[1] = "fa fa-recycle";

            $acao[2] = "javascript";
            $texto[2] = $FiltroQueue[0];
            $textoDesc[2] = $FiltroQueue[1];
            $AcionaMenu[2] = 1;
            $Imagem[2] = "images/filtro.gif";
            $class_Icon[2] = "fa fa-filter";


            $acao[5] = "document.location = '$entrada'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 2: // WF
            if ($MostrarAdHoc == 1) {
                if ($AdHoc == 1) {
                    $acao[1] = "FinalizaAdHoc()";
                    $texto[1] = $EncerraAdHocButton[0];
                    $textoDesc[1] = $EncerraAdHocButton[1];
                    $AcionaMenu[1] = 1;

                    $acao[3] = "RedirecionarAdHoc()";
                    $texto[3] = $TxRedirecionaAdHoc[0];
                    $textoDesc[3] = $TxRedirecionaAdHoc[1];
                    $AcionaMenu[3] = 1;
                }
            }

            $acao[3] = "JSmostraHistoricoCaso($CaseNum)";
            $texto[3] = $Historico[0];
            $testoDesc[3] = $Historico[1];
            $AcionaMenu[3] = 1;
            $Imagem[3] = " <img src=\"images/historico.gif\" />";
            $tipoBotao[3] = "showModal";
            $class_Icon[3] = "fa fa-arrow-circle-o-right";

            $acao[5] = "document.location = '$PaginaOrigem'";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = " <img src=\"images/seta_voltar.gif\" />";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 3: // WF
            if ($MostrarBotoes["OK"]) {
                $acao[0] = "Processar()";
                $texto[0] = $ProcessarDoc[0];
                $textoDesc[0] = $ProcessarDoc[1];
                $MouseOver[0] = "";
                $AcionaMenu[0] = 1;
                $class_Icon[0] = "fa fa-check";
                $Imagem[0] = " <img src=\"images/salvar.gif\" />";
            }

            if ($MostrarBotoes["SALVAR"]) {
                $acao[1] = "Salvar()";
                $texto[1] = $SalvarDoc[0];
                $textoDesc[1] = $SalvarDoc[1];
                $AcionaMenu[1] = 1;
                $class_Icon[1] = "fa fa-save";
            }

            if ($MostrarBotoes["BLOQUEAR"]) {
                $acao[2] = "SalvarLock()";
                $texto[2] = $SalvarBloquear[0];
                $textoDesc[2] = $SalvarBloquear[1];
                $AcionaMenu[2] = 1;
                $class_Icon[2] = "fa fa-lock";
            }

            if ($MostrarBotoes["HISTORICO"]) {
                $acao[3] = "$('.sidebar-toggle').click(); JSmostraHistoricoCaso($CaseNum)";
                $texto[3] = $Historico[0];
                $testoDesc[3] = $Historico[1];
                $AcionaMenu[3] = 1;
                $Imagem[3] = " <img src=\"images/historico.gif\" width=\"20px\" height=\"20px\" />";
                $tipoBotao[3] = "showModal";
                $class_Icon[3] = "fa fa-list-alt";
            }

            if ($MostrarAdHoc == 1 & $userdef->AdHoc) {
                $acao[4] = "MostraAdHoc()";
                $texto[4] = $AdHocButton[0];
                $textoDesc[4] = $AdHocButton[1];
                $AcionaMenu[4] = 1;
                $Imagem[4] = " <img src=\"images/adhoc.gif\"/ >";
            }

            if ($MostrarBotoes["CANCELAR"]) {
                $acao[5] = "document.location = '$PaginaOrigem'";
                $texto[5] = $Cancelar[0];
                $textoDesc[5] = $Cancelar[1];
                $AcionaMenu[5] = 1;
                $Imagem[5] = "<img src=\"images/seta_voltar.gif\" border=\"0\" />";
                $class_Icon[5] = "fa fa-arrow-circle-o-left";
            }
            break;

        case "voltar";
        case 97:
            $acao[5] = "document.location = 'entrada'";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            $class_Icon[5] = "fa fa-arrow-circle-o-left";
            break;

        case 4:
            $acao[5] = "$PaginaAnterior";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = " <img src=\"images/seta_voltar.gif\" />";
            break;

        case 5:
            $acao[5] = "document.location = 'flow0018.php'";
            $texto[5] = $SairPesq[0];
            $textoDesc[5] = $SairPesq[1];
            $AcionaMenu[5] = 1;
            break;

        case 6:
            $acao[5] = "document.location = 'flow0018.php'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 7:
            $acao[0] = "document.location = 'BPMCriarNovo.php?Menu=6'";
            $texto[0] = $_msgNovoDoc[0];
            $textoDesc[0] = $_msgNovoDoc[1];
            $AcionaMenu[0] = 1;

            $acao[1] = "document.location='flow0013.php?&words=$words'";
            $texto[1] = $ReturnPesq[0];
            $textoDesc[1] = $ReturnPesq[1];
            $AcionaMenu[1] = 1;

            $acao[3] = " document.getElementById('DivFiltro').style.top = 100 + document.body.scrollTop; document.getElementById('DivFiltro').style.display=''; document.formFiltro.FilterWords.focus()";
            $texto[3] = $FilterPesq[0];
            $textoDesc[3] = $FilterPesq[1];
            $AcionaMenu[3] = 1;

            $acao[4] = "window.open('relbiblisearch.php?Pesquisa=$words&ProcId=$ProcId&UserId=$UserId','relatorio')";
            $texto[4] = $PrintDoc[0];
            $textoDesc[4] = $PrintDoc[1];
            $AcionaMenu[4] = 1;

            $acao[5] = "document.location = 'flow0018.php'";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 8:
            if ($Action == "Edit") {
                $acao[0] = "function processar() { if (valida(document.textos)) { document.textos.Acao.value = 'Salvar';  document.textos.submit()} }; processar()";
                $texto[0] = $SalvarDoc[0];
                $textoDesc[0] = $SalvarDoc[1];
                $AcionaMenu[0] = 1;
            }
            $acao[2] = "window.open('flow0016.php?CaseNum=' + document.textos.CaseNum.value + '&ProcId=' + document.textos.ProcId.value + '&StepId=' + document.textos.StepId.value )";
            $texto[2] = $PrintDoc[0];
            $textoDesc[2] = $PrintDoc[1];
            $AcionaMenu[2] = 1;

            $acao[5] = "document.location = '$PaginaOrigem'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            break;

        case 9:
            $acao[0] = "document.location = 'BPMCriarNovo.php';";
            $texto[0] = $NovaMsg[0];
            $textoDesc[0] = $NovaMsg[1];
            $AcionaMenu[0] = 1;

            $acao[1] = "document.location = 'flow0014.php?Lista=Rascunhos'";
            $texto[1] = $Rascunhos[0];
            $textoDesc[1] = $Rascunhos[1];
            $AcionaMenu[1] = 1;

            $acao[5] = "Location = '$entrada'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 10:
            $acao[5] = "document.location = 'flow0014.php?Pagina=1&Linhas=20'; document.location = 'toolbar.php?Menu=9'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 11:
            $acao[0] = "if (jsValidacaoFormulario(document.textos)) { document.textos.Acao.value = 'Salvar';  document.textos.submit()}";
            $texto[0] = $SalvarMsg[0];
            $textoDesc[0] = $SalvarMsg[1];
            $AcionaMenu[0] = 1;

            $acao[1] = "function processar() { if (jsValidacaoFormulario(document.textos)) { document.textos.Acao.value = 'Processar';  document.textos.submit()} }; processar()";
            $texto[1] = $EnviarMsg[0];
            $textoDesc[1] = $EnviarMsg[1];
            $AcionaMenu[1] = 1;

            $acao[5] = "document.location = 'flow0014.php?Pagina=1&Linhas=20'";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            break;

        case 12:
            $acao[0] = "if (jsValidacaoFormulario(document.textos)) { document.textos.Acao.value = 'Processar';  document.textos.submit(); }";
            $texto[0] = $SalvarDoc[0];
            $textoDesc[0] = $SalvarDoc[1];
            $AcionaMenu[0] = 1;

            $acao[1] = "document.location ='BPMEditCase.php?ProcId=$ProcId&StepId=1&CaseNum=0&Action=Edit'";
            $texto[1] = $_msgNovoDoc[0];
            $textoDesc[1] = $_msgNovoDoc[1];
            $AcionaMenu[1] = 1;

            $acao[5] = "document.location = 'flow0018.php'";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 13:
            $acao[0] = "document.location = 'BPMCriarNovo.php'";
            $texto[0] = $_msgNovoDoc[0];
            $textoDesc[0] = $_msgNovoDoc[1];
            $AcionaMenu[0] = 1;

            $acao[5] = "document.location = '$entrada'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 14: // Sele��o para Edi��o de Layout
            $acao[5] = "document.location = '$entrada'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 15:
            $acao[0] = "Estilo.submit();";
            $texto[0] = $SalvarDoc[0];
            $textoDesc[0] = $SalvarDoc[1];
            $AcionaMenu[0] = 1;

            $acao[5] = "document.location = 'BPMManage.php'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 16:
            $acao[0] = "jsAtualizarQueue('$PaginaPesquisa')";
            $texto[0] = $Atualizar[0];
            $textoDesc[0] = $Atualizar[0];
            $AcionaMenu[0] = 1;
            $Imagem[0] = "images/atualizar.gif";

            $acao[1] = "MostraSearchForm()";
            $texto[1] = $FiltroQueue[0];
            $textoDesc[1] = $FiltroQueue[1];
            $AcionaMenu[1] = 1;
            $Imagem[1] = "images/filtro.gif";

            $acao[3] = "document.location ='BPMAdminReports.php'";
            $texto[3] = $Relatorio[0];
            $textoDesc[3] = $Relatorio[1];
            $AcionaMenu[3] = 1;

            if ($_AdmProcs["GeralAdmin"][$ProcId]["Action"] == 1 && $TipoAdmin != 'StepAdmin') {
                $acao[4] = "document.location ='BatchOperations.php'";
                $texto[4] = $OperacaoLote[0];
                $textoDesc[4] = $OperacaoLote[1];
                $AcionaMenu[4] = 1;
            }

//$acao[5] = "document.location = '$PaginaOrigem'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $Imagem[5] = "images/seta_voltar.gif";
            $acao[5] = "document.location = '$PaginaOrigem'";
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 17:
            if ($Action == "Edit") {
                $acao[0] = "function processar() { if (jsValidacaoFormulario(document.textos)) { document.textos.Acao.value = 'Processar';  document.textos.submit()} }; processar()";
                $texto[0] = $SalvarDoc[0];
                $textoDesc[0] = $SalvarDoc[1];
                $AcionaMenu[0] = 1;

                $acao[1] = "document.location ='BPMEditCase.php?ProcId=$ProcId&StepId=1&CaseNum=0&Action=Edit&ProcName=$ProcName'";
                $texto[1] = $_msgNovoDoc[0];
                $textoDesc[1] = $_msgNovoDoc[1];
                $AcionaMenu[1] = 1;
            }
            $acao[2] = "window.open('flow0016.php?CaseNum=' + document.textos.CaseNum.value + '&ProcId=' + document.textos.ProcId.value + '&StepId=' + document.textos.StepId.value )";
            $texto[2] = $PrintDoc[0];
            $textoDesc[2] = $PrintDoc[1];
            $AcionaMenu[2] = 1;

            $acao[5] = "document.location = 'flow0018.php'";
            $texto[5] = $SairPesq[0];
            $textoDesc[5] = $SairPesq[1];
            $AcionaMenu[5] = 1;
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 18:
            $acao[5] = "window.document.location = '$entrada'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            break;

            $acao[3] = " document.getElementById('DivFiltro').style.top = 100 + document.body.scrollTop; document.getElementById('DivFiltro').style.display=''; document.formFiltro.ProcSel.focus()";
            $texto[3] = $FilterPesq[0];
            $textoDesc[3] = $FilterPesq[1];
            $AcionaMenu[3] = 1;
            break;

        case 19: // Nova Solicitacao do Usuario
            $acao[1] = "function processar() { if (jsValidacaoFormulario(document.textos)) { document.textos.Acao.value = 'Processar';  document.textos.submit()} }; processar()";
            $texto[1] = $SalvarDoc[0];
            $textoDesc[1] = $SalvarDoc[1];
            $AcionaMenu[1] = 1;

            $acao[5] = "window.close()";
            $texto[5] = $_msgFechar[0];
            $textoDesc[5] = $_msgFechar[1];
            $AcionaMenu[5] = 1;
            $class_Icon[5] = "fa fa-times-circle";
            break;

        case 20: // Salvar Preferencias Usuario
            $acao[1] = "document.Escolha.submit()";
            $texto[1] = $SalvarDoc[0];
            $textoDesc[1] = $SalvarDoc[1];
            $AcionaMenu[1] = 1;

            $acao[5] = "document.location='BPMQueue.php'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            break;
            break;

        case 21: // WF
            $acao[0] = "document.formulario.action = 'manu0013.php'; document.formulario.submit()";
            $texto[0] = $_msgNovoDoc[0];
            $textoDesc[0] = $_msgNovoDoc[1];
            $AcionaMenu[0] = 1;

            $acao[2] = "document.formulario.action = 'manu0012.php'; document.formulario.submit()";
            $texto[2] = $Atualizar[0];
            $textoDesc[2] = $Atualizar[1];
            $AcionaMenu[2] = 1;

            $acao[5] = "document.location = 'manu0015.php'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";

            break;

        case 22: // salvar novos dados em uma tabela qualquer
            $acao[0] = "document.formulario.action = 'manu0012.php'; document.formulario.submit()";
            $texto[0] = $SalvarDoc[0];
            $textoDesc[0] = $SalvarDoc[1];
            $AcionaMenu[0] = 1;

            $acao[5] = "document.formulario.action = 'manu0012.php'; document.formulario.submit()";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            break;
            break;


        case 23: // salvar altera��es em um registro de uma tabela qualquer
            $acao[0] = "document.formulario.action = 'manu0012.php'; document.formulario.submit()";
            $texto[0] = $SalvarDoc[0];
            $textoDesc[0] = $SalvarDoc[1];
            $AcionaMenu[0] = 1;

            $acao[5] = "document.formulario.action = 'manu0012.php'; document.formulario.submit()";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;


        case 24: // menu tabledef

            $acao[2] = "document.location = 'manu0015.php';";
            $texto[2] = $Atualizar[0];
            $textoDesc[2] = $Atualizar[1];
            $AcionaMenu[2] = 1;

            $acao[5] = "document.location = 'loggof.php'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 26: // voltar		
            $acao[5] = "document.location = 'BPMQueue.php?Pagina=$Pagina'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 27: // WF
            $acao[0] = "jsCriaNovoCaso('editcase', 0)";
            $texto[0] = $_msgNovoDoc[0];
            $textoDesc[0] = $_msgNovoDoc[1];
            $AcionaMenu[0] = 1;
            $Imagem[0] = "images/novo.gif";

            $acao[1] = "jsAtualizarQueue('queue')";
            $texto[1] = $Atualizar[0];
            $textoDesc[1] = $Atualizar[1];
            $AcionaMenu[1] = 1;
            $Imagem[1] = "images/atualizar.gif";

//            $acao[2] = "MostraSearchForm()";
//            $texto[2] = $FiltroQueue[0];
//            $textoDesc[2] = $FiltroQueue[1];
//            $AcionaMenu[2] = 1;
//            $Imagem[2] = "images/filtro.gif";
//            $acao[5] = "if (confirm('Sair do Sistema???')) document.location = 'logoff.php'";
//            $texto[5] = $LogOff[0];
//            $textoDesc[5] = $LogOff[1];
//            $Imagem[5] = "images/seta_voltar.gif";
//            $AcionaMenu[5] = 1;
//            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 28: // WF
            $acao[2] = "MostraSearchForm()";
            $texto[2] = $FiltroQueue[0];
            $textoDesc[2] = $FiltroQueue[1];
            $AcionaMenu[2] = 1;
            $Imagem[2] = "images/filtro.gif";

            $acao[1] = "SubmitSearchForm('$URLSearch')";
            $texto[1] = $Atualizar[0];
            $textoDesc[1] = $Atualizar[1];
            $AcionaMenu[1] = 1;
            $Imagem[1] = "images/reload0000.gif";

            $acao[5] = "window.close()";
            $texto[5] = $_msgFechar[0];
            $textoDesc[5] = $_msgFechar[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            break;

        case 29:
            $acao[0] = "AdicionaLinha($NovaLinha, $Tickler, $Edit); comentarios.submit();";
            $texto[0] = $SalvarDoc[0];
            $textoDesc[0] = $SalvarDoc[1];
            $AcionaMenu[0] = 1;

            $acao[5] = "document.location='$PaginaOrigem'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            break;

        case 31: // WF	
            $acao[1] = "document.location = 'BPMQueue.php?Pagina=$Pagina&Linhas=$Linhas&Detalhes=$Detalhes'";
            $texto[1] = $Atualizar[0];
            $textoDesc[1] = $Atualizar[1];
            $AcionaMenu[1] = 1;

            $acao[5] = "document.location = '$entrada'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 32: // WF
            $acao[0] = "document.textos.Acao.value = 'Salvar';  document.textos.submit()";
            $texto[0] = $SalvarDoc[0];
            $textoDesc[0] = $SalvarDoc[1];
            $AcionaMenu[0] = 1;

            $acao[1] = "function processar() { if (jsValidacaoFormulario(document.textos)) { document.textos.Acao.value = 'Processar';  document.textos.submit()} }; processar()";
            $texto[1] = $ProcessarDoc[0];
            $textoDesc[1] = $ProcessarDoc[1];
            $AcionaMenu[1] = 1;
            $Imagem[0] = " <img src=\"images/ok.gif\" />";


            $acao[5] = "document.location = 'BPMQueue.php?Pagina=$Pagina&Linhas=20'";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            break;
        case 33: // WF								
            $acao[2] = "window.open('rel0001-cosplastic.php?CaseNum=' + document.textos.CaseNum.value + '&ProcId=' + document.textos.ProcId.value + '&StepId=' + document.textos.StepId.value,'relatorio','toolbar=0,Location=0,directories=0,status=1,menubar=0,scrollbars=1,resizable=0,screenX=0,screenY=0,left=0,top=0,width=800,height=600')";
            $texto[2] = $PrintDoc[0];
            $textoDesc[2] = $PrintDoc[1];
            $AcionaMenu[2] = 1;

            $acao[5] = "document.location = 'flow0001-2.php?Pagina=$Pagina&Linhas=20'";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 34: // WF
            $acao[1] = "function processar() { if (jsValidacaoFormulario(document.textos)) { document.textos.Acao.value = 'Processar';  document.textos.submit()} }; processar()";
            $texto[1] = $ProcessarDoc[0];
            $textoDesc[1] = $ProcessarDoc[1];
            $AcionaMenu[1] = 1;
            $Imagem[0] = " <img src=\"images/ok.gif\" />";


            $acao[5] = "document.location = '$CustonGate'";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 35:
            $acao[5] = "jsLimpaAcaoVoltar(); window.close()";
            $texto[5] = $_msgFechar[0];
            $textoDesc[5] = $_msgFechar[1];
            $AcionaMenu[5] = 1;
            $class_Icon[5] = "fa fa-times-circle";
            break;

        case 36:
            $acao[0] = "if (jsValidacaoFormulario($Adicionar)) { document.getElementById('Adicionar').value = 1; comentarios.submit(); }";
            $texto[0] = $EnviarMsg[0];
            $textoDesc[0] = $EnviarMsg[1];
            $AcionaMenu[0] = 1;

            if ($LockAdHoc == 1) {
                $acao[2] = "document.getElementById('Adicionar').value = 0;  comentarios.submit(); ";
                $texto[2] = $EnviarMsg[0];
                $textoDesc[2] = $EnviarMsg[1];
                $AcionaMenu[2] = 1;
            }

            $acao[5] = "window.close()";
            $texto[5] = $_msgFechar[0];
            $textoDesc[5] = $_msgFechar[1];
            $AcionaMenu[5] = 1;
            $class_Icon[5] = "fa fa-times-circle";
            break;

        case 37: // AD
            $acao[0] = "SalvarAdmin()";
            $texto[0] = $SalvarDoc[0];
            $textoDesc[0] = $SalvarDoc[1];
            $AcionaMenu[0] = 1;

            $acao[5] = "$PaginaAnterior";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = " <img src=\"images/seta_voltar.gif\" />";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 38:
            $acao[5] = "document.location = '$Retorno'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            break;

        case 39:
//			$acao[1] = "document.location='flow0013.php?&words=$words'";
//			$texto[1] = $ReturnPesq[0];
//			$textoDesc[1] = $ReturnPesq[1];
//			$AcionaMenu[1] = 1;			
//			$acao[3] = " document.getElementById('DivFiltro').style.top = 100 + document.body.scrollTop; document.getElementById('DivFiltro').style.display=''; document.formFiltro.FilterWords.focus()";
//			$texto[3] = $FilterPesq[0];
//			$textoDesc[3] = $FilterPesq[1];
//			$AcionaMenu[3] = 1;			

            $acao[5] = "document.location ='flow0013.php?ProcId=$ProcId'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            break;

        case 40:
            $acao[2] = "document.location = 'flow0013.php?ProcId=$ProcId'";
            $texto[2] = $ReturnPesq[0];
            $textoDesc[2] = $ReturnPesq[1];
            $AcionaMenu[2] = 1;

            $acao[5] = "document.location = '$PaginaOrigem'";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 41:

            $acao[2] = "JSmostraHistoricoCaso($CaseNum, 'Admin')";
            $texto[2] = $Historico[0];
            $testoDesc[2] = $Historico[1];
            $AcionaMenu[2] = 1;
            $Imagem[2] = " <img src=\"images/FIND.PNG\" />";
            $tipoBotao[2] = "showModal";


            if ($Ac == 1) {
                $acao[3] = "$Editar";
                $texto[3] = $EditarDados[0];
                $textoDesc[3] = $EditarDados[1];
                $AcionaMenu[3] = 1;
                $Imagem[3] = " <img src=\"images/caseedit0000.png\" />";
            } else {
                $acao[3] = "$Visualizar";
                $texto[3] = "Visualizar";
                $textoDesc[3] = "Visualizar";
                $AcionaMenu[3] = 1;
            }

            $acao[5] = "$OrigemPesquisa";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = " <img src=\"images/seta_voltar.gif\" />";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 42: // Menu edi��o Cadastro Usu�rio
            $acao[0] = "posta(frmcadastro)";
            $texto[0] = $SalvarDoc[0];
            $textoDesc[0] = $SalvarDoc[1];
            $AcionaMenu[0] = 1;

            $acao[5] = "MudaDocument('$Volta')";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            break;

        case 43:
            $acao[5] = "MudaDocument('flow0038.php)";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            break;

        case 44:
            $acao[5] = "if (confirm('Sair do Sistema???')) document.location = 'logon.php'";
            $texto[5] = $LogOff[0];
            $textoDesc[5] = $LogOff[1];
            $AcionaMenu[5] = 1;
//$Imagem[5] = "<img src=\"images/seta_voltar.gif\" border=\"0\" />";	                
            $Imagem[5] = "images/seta_voltar.gif";
            $class_Icon[5] = "fa fa-sign-out-alt";
            break;

        case 45:
            $acao[1] = "ProcessaRelatorioCasosEnvia()";
            $texto[1] = $GerarRelatorio[0];
            $textoDesc[1] = $GerarRelatorio[1];
            $AcionaMenu[1] = 1;

            $acao[5] = "BPMAdminReports.php";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            break;

        case 46:
            $acao[5] = "$OrigemPesquisa";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "<img src=\"images/seta_voltar.gif\" border=\"0\" />";
            break;

        case 47:
            $acao[5] = "$OrigemPesquisa";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "<img src=\"images/seta_voltar.gif\" border=\"0\" />";
            break;


        case 48: // WF	
            $acao[1] = "jsAtualizarQueue('BPMQueue.php')";
            $texto[1] = $Atualizar[0];
            $textoDesc[1] = $Atualizar[1];
            $AcionaMenu[1] = 1;
            $Imagem[1] = "images/atualizar.gif";

            $acao[2] = "MostraSearchForm()";
            $texto[2] = $FiltroQueue[0];
            $textoDesc[2] = $FiltroQueue[1];
            $AcionaMenu[2] = 1;
            $Imagem[2] = "images/filtro.gif";

            $acao[5] = "document.location = '$entrada'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            break;

        case 49: // WF				
            $acao[1] = "jsAtualizarQueue('queue')";
            $texto[1] = $Atualizar[0];
            $textoDesc[1] = $Atualizar[1];
            $AcionaMenu[1] = 1;
            $Imagem[1] = "images/atualizar.gif";

            $acao[2] = "MostraSearchForm()";
            $texto[2] = $FiltroQueue[0];
            $textoDesc[2] = $FiltroQueue[1];
            $AcionaMenu[2] = 1;
            $Imagem[2] = "images/filtro.gif";

            $acao[5] = "if (confirm('Sair do Sistema???')) document.location = 'logoff.php'";
            $texto[5] = $LogOff[0];
            $textoDesc[5] = $LogOff[1];
            $Imagem[5] = "images/seta_voltar.gif";
            $AcionaMenu[5] = 1;
            break;

        /** Toolbar para Quando há vários Passos para criar caso
         * 
         */
        case 50: // WF
            $acao[0] = "SelecionarPassoNovoCaso()";
            $texto[0] = $_msgNovoDoc[0];
            $textoDesc[0] = $_msgNovoDoc[1];
            $AcionaMenu[0] = 1;
            $Imagem[0] = "images/novo.gif";
            $TipoBotao[0] = "Collapse";
            $Target[0] = "collapseSTEPSNOVOCASO";
            $class_Icon[0] = "fa fa-star-o";

            $acao[1] = "jsAtualizarQueue('BPMQueue.php')";
            $texto[1] = $Atualizar[0];
            $textoDesc[1] = $Atualizar[1];
            $AcionaMenu[1] = 1;
            $Imagem[1] = "images/atualizar.gif";

            $acao[2] = "MostraSearchForm()";
            $texto[2] = $FiltroQueue[0];
            $textoDesc[2] = $FiltroQueue[1];
            $AcionaMenu[2] = 1;
            $Imagem[2] = "images/filtro.gif";

            $acao[5] = "document.location = '$entrada'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            break;


        case 51: // WF
            $acao[0] = "SelecionarPassoNovoCaso()";
            $texto[0] = $_msgNovoDoc[0];
            $textoDesc[0] = $_msgNovoDoc[1];
            $AcionaMenu[0] = 1;
            $Imagem[0] = "images/novo.gif";

            $acao[1] = "jsAtualizarQueue('queue')";
            $texto[1] = $Atualizar[0];
            $textoDesc[1] = $Atualizar[1];
            $AcionaMenu[1] = 1;
            $Imagem[1] = "images/atualizar.gif";

            $acao[2] = "MostraSearchForm()";
            $texto[2] = $FiltroQueue[0];
            $textoDesc[2] = $FiltroQueue[1];
            $AcionaMenu[2] = 1;
            $Imagem[2] = "images/filtro.gif";

            $acao[5] = "if (confirm('Sair do Sistema???')) document.location = 'logoff.php'";
            $texto[5] = $LogOff[0];
            $textoDesc[5] = $LogOff[1];
            $Imagem[5] = "images/seta_voltar.gif";
            $AcionaMenu[5] = 1;
            break;

        case 52: // WF
            $acao[0] = "jsCriaNovoCaso('BpmCriarNovo.php')";
            $texto[0] = $_msgNovoDoc[0];
            $textoDesc[0] = $_msgNovoDoc[1];
            $AcionaMenu[0] = 1;
            $Imagem[0] = "images/novo.gif";

            $acao[1] = "jsAtualizarQueue('BPMQueueWebCapture.php')";
            $texto[1] = $Atualizar[0];
            $textoDesc[1] = $Atualizar[1];
            $AcionaMenu[1] = 1;
            $Imagem[1] = "images/atualizar.gif";

            $acao[2] = "MostraSearchForm()";
            $texto[2] = $FiltroQueue[0];
            $textoDesc[2] = $FiltroQueue[1];
            $AcionaMenu[2] = 1;
            $Imagem[2] = "images/filtro.gif";

            $acao[5] = "document.location = '$entrada'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            break;

        case 53:
            $acao[1] = "document.FormData.submit()";
            $texto[1] = $GerarRelatorio[0];
            $textoDesc[1] = $GerarRelatorio[1];
            $AcionaMenu[1] = 1;

            $acao[5] = "BPMAdminReports.php";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            break;

        case 54:
        case "admin_users":
            $acao[0] = "jsOpenEditUser()";
            $texto[0] = $CriarUsuarios[0];
            $textoDesc[0] = $CriarUsuarios[1];
            $AcionaMenu[0] = 1;
            $class_Icon[0] = "fa fa-plus text-green";
            $TipoBotao[0] = "Collapse";

            $acao[1] = "document.location = 'grupos'";
            $texto[1] = $EditarGrupos[0];
            $textoDesc[1] = $EditarGrupos[1];
            $AcionaMenu[1] = 1;
            $class_Icon[1] = "fa fa-users";

            $acao[2] = "document.location = 'origemdominio'";
            $texto[2] = $textos_origemdominio[0];
            $textoDesc[2] = $textos_origemdominio[1];
            $AcionaMenu[2] = 1;
            $class_Icon[2] = "fa fa-sitemap";

            $acao[3] = "document.location = 'permissoes'";
            $texto[3] = $permissoesProcessos[0];
            $textoDesc[3] = $permissoesProcessos[1];
            $AcionaMenu[3] = 1;
            $class_Icon[3] = "fa fa-gears";

            $acao[4] = "document.location = 'adminconfig'";
            $texto[4] = $configseguranca[0];
            $textoDesc[4] = $configseguranca[1];
            $AcionaMenu[4] = 1;
            $class_Icon[4] = "fa fa-lock";

            $acao[5] = "document.location = '$entrada'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 55:
            $acao[0] = "jsOpenEditUser()";
            $texto[0] = $CriarGrupo[0];
            $textoDesc[0] = $CriarGrupo[1];
            $AcionaMenu[0] = 1;
            $class_Icon[0] = "fa fa-plus text-green";

            $acao[1] = "document.location = 'grupos'";
            $texto[1] = $EditarGrupos[0];
            $textoDesc[1] = $EditarGrupos[1];
            $AcionaMenu[1] = 1;
            $class_Icon[1] = "fa fa-users";

            $acao[2] = "document.location = 'origemdominio'";
            $texto[2] = $textos_origemdominio[0];
            $textoDesc[2] = $textos_origemdominio[1];
            $AcionaMenu[2] = 1;
            $class_Icon[2] = "fa fa-sitemap";

            $acao[3] = "document.location = 'permissoes'";
            $texto[3] = $permissoesProcessos[0];
            $textoDesc[3] = $permissoesProcessos[1];
            $AcionaMenu[3] = 1;
            $class_Icon[3] = "fa fa-gears";

            $acao[4] = "document.location = 'adminconfig'";
            $texto[4] = $configseguranca[0];
            $textoDesc[4] = $configseguranca[1];
            $AcionaMenu[4] = 1;
            $class_Icon[4] = "fa fa-lock";

            $acao[5] = "document.location = '$entrada'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 57:
            $acao[0] = "jsOpenEditGroup()";
            $texto[0] = $CriarGrupo[0];
            $textoDesc[0] = $CriarGrupo[1];
            $AcionaMenu[0] = 1;
            $class_Icon[0] = "fa fa-plus text-green";

            $acao[1] = "document.location = 'usuarios'";
            $texto[1] = $EditarUsuarios[0];
            $textoDesc[1] = $EditarUsuarios[1];
            $AcionaMenu[1] = 1;
            $class_Icon[1] = "fa fa-user";

            $acao[2] = "document.location = 'origemdominio'";
            $texto[2] = $textos_origemdominio[0];
            $textoDesc[2] = $textos_origemdominio[1];
            $AcionaMenu[2] = 1;
            $class_Icon[2] = "fa fa-sitemap";

            $acao[3] = "document.location = 'permissoes'";
            $texto[3] = $permissoesProcessos[0];
            $textoDesc[3] = $permissoesProcessos[1];
            $AcionaMenu[3] = 1;
            $class_Icon[3] = "fa fa-gears";

            $acao[4] = "document.location = 'adminconfig'";
            $texto[4] = $configseguranca[0];
            $textoDesc[4] = $configseguranca[1];
            $AcionaMenu[4] = 1;
            $class_Icon[4] = "fa fa-lock";

            $acao[5] = "document.location = 'permissoes'";
            $texto[5] = "Lista Soluções";
            $textoDesc[5] = "Lista Soluções";
            $AcionaMenu[5] = 1;
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case "config_seguranca":
            $acao[1] = "document.location = 'usuarios'";
            $texto[1] = $EditarUsuarios[0];
            $textoDesc[1] = $EditarUsuarios[1];
            $AcionaMenu[1] = 1;
            $class_Icon[1] = "fa fa-user";

            $acao[2] = "document.location = 'origemdominio'";
            $texto[2] = $textos_origemdominio[0];
            $textoDesc[2] = $textos_origemdominio[1];
            $AcionaMenu[2] = 1;
            $class_Icon[2] = "fa fa-sitemap";

            $acao[3] = "document.location = 'permissoes'";
            $texto[3] = $permissoesProcessos[0];
            $textoDesc[3] = $permissoesProcessos[1];
            $AcionaMenu[3] = 1;
            $class_Icon[3] = "fa fa-gears";

            $acao[5] = "document.location = '$entrada'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;


        case 56:
            $acao[0] = "jsOpenEditGroup()";
            $texto[0] = $CriarGrupo[0];
            $textoDesc[0] = $CriarGrupo[1];
            $AcionaMenu[0] = 1;
            $class_Icon[0] = "fa fa-plus text-green";

            $acao[1] = "document.location = 'usuarios'";
            $texto[1] = $EditarUsuarios[0];
            $textoDesc[1] = $EditarUsuarios[1];
            $AcionaMenu[1] = 1;
            $class_Icon[1] = "fa fa-user";

            $acao[2] = "document.location = 'origemdominio'";
            $texto[2] = $textos_origemdominio[0];
            $textoDesc[2] = $textos_origemdominio[1];
            $AcionaMenu[2] = 1;
            $class_Icon[2] = "fa fa-sitemap";

            $acao[3] = "document.location = 'permissoes'";
            $texto[3] = $permissoesProcessos[0];
            $textoDesc[3] = $permissoesProcessos[1];
            $AcionaMenu[3] = 1;
            $class_Icon[3] = "fa fa-gears";

            $acao[4] = "document.location = 'adminconfig'";
            $texto[4] = $configseguranca[0];
            $textoDesc[4] = $configseguranca[1];
            $AcionaMenu[4] = 1;
            $class_Icon[4] = "fa fa-lock";

            $acao[5] = "document.location = '$entrada'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;


        // Edição de Origem Dominio
        case 61:
        case "admin_origem_dominio":
            $acao[0] = " ";
            $texto[0] = "Adicionar";
            $textoDesc[0] = "Adicionar";
            $AcionaMenu[0] = 1;
            $class_Icon[0] = "fa fa-plus text-green";
            $TipoBotao[0] = "Collapse";

            $opcoes = array();
            $opcaoMenu["icone"] = "fa fa-plus text-primary";
            $opcaoMenu["class"] = "text-primary";
            $opcaoMenu["texto"] = "Dominio";
            $opcoes[] = $opcaoMenu;
            $opcaoMenu["texto"] = "Origem";
            $opcoes[] = $opcaoMenu;
            $opcoesMenu[0] = $opcoes;

            $acao[1] = "document.location = 'usuarios'";
            $texto[1] = $EditarUsuarios[0];
            $textoDesc[1] = $EditarUsuarios[1];
            $AcionaMenu[1] = 1;
            $class_Icon[1] = "fa fa-user";

            $acao[2] = "document.location = 'grupos'";
            $texto[2] = $EditarGrupos[0];
            $textoDesc[2] = $EditarGrupos[1];
            $AcionaMenu[2] = 1;
            $class_Icon[2] = "fa fa-users";

            $acao[3] = "document.location = 'permissoes'";
            $texto[3] = $permissoesProcessos[0];
            $textoDesc[3] = $permissoesProcessos[1];
            $AcionaMenu[3] = 1;
            $class_Icon[3] = "fa fa-gears";

            $acao[4] = "document.location = 'adminconfig'";
            $texto[4] = $permissoesProcessos[0];
            $textoDesc[4] = $permissoesProcessos[1];
            $AcionaMenu[4] = 1;
            $class_Icon[4] = "fa fa-lock";

            $acao[5] = "document.location = '$entrada'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;


        /** Botoes para Novo Caso
         * 
         */
        case 57: // WF
            $acao[0] = "jsCarregaWebCaptura()";
            $texto[0] = $capturarImagem[0];
            $textoDesc[0] = $capturarImagem[1];
            $AcionaMenu[0] = 1;
            $Imagem[0] = "images/novo.gif";
            $class_Icon[0] = "fa fa-camera";

            $acao[1] = "jsAtualizarQueue('queue')";
            $texto[1] = $Atualizar[0];
            $textoDesc[1] = $Atualizar[1];
            $AcionaMenu[1] = 1;
            $Imagem[1] = "images/atualizar.gif";
            $class_Icon[1] = "fa fa-recycle";

            $acao[2] = "javascript";
            $texto[2] = $FiltroQueue[0];
            $textoDesc[2] = $FiltroQueue[1];
            $AcionaMenu[2] = 1;
            $Imagem[2] = "images/filtro.gif";
            $class_Icon[2] = "fa fa-filter";

            $acao[5] = "document.location = '$entrada'";
            $texto[5] = $Anterior[0];
            $textoDesc[5] = $Anterior[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "images/seta_voltar.gif";
            $class_Icon[5] = "fa fa-arrow-circle-o-right";
            break;

        case 58:
            $acao[0] = "jsRotearCaso()";
            $texto[0] = $ProcessarDoc[0];
            $textoDesc[0] = $ProcessarDoc[1];
            $MouseOver[0] = "";
            $AcionaMenu[0] = 1;
            $class_Icon[0] = "fa fa-check";
            $Imagem[0] = " <img src=\"images/salvar.gif\" />";

            $acao[3] = "JSmostraHistoricoCaso($CaseNum)";
            $texto[3] = $Historico[0];
            $testoDesc[3] = $Historico[1];
            $AcionaMenu[3] = 1;
            $Imagem[3] = " <img src=\"images/historico.gif\" />";
            $tipoBotao[3] = "showModal";
            $class_Icon[3] = "fa fa-arrow-circle-o-right";

            $acao[5] = "document.location = '$PaginaOrigem'";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "<img src=\"images/seta_voltar.gif\" border=\"0\" />";
            $class_Icon[5] = "fa fa-arrow-circle-o-left";
            break;

        case 59:
            $acao[0] = "jsProcessarCT()";
            $texto[0] = $ProcessarDoc[0];
            $textoDesc[0] = $ProcessarDoc[1];
            $MouseOver[0] = "";
            $AcionaMenu[0] = 1;
            $class_Icon[0] = "fa fa-check";
            $Imagem[0] = " <img src=\"images/salvar.gif\" />";

            $acao[3] = "JSmostraHistoricoCaso($CaseNum)";
            $texto[3] = $Historico[0];
            $testoDesc[3] = $Historico[1];
            $AcionaMenu[3] = 1;
            $Imagem[3] = " <img src=\"images/historico.gif\" />";
            $tipoBotao = "showModal";
            $class_Icon[3] = "fa fa-arrow-circle-o-right";

            $acao[5] = "document.location = '$PaginaOrigem'";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            $Imagem[5] = "<img src=\"images/seta_voltar.gif\" border=\"0\" />";
            $class_Icon[5] = "fa fa-arrow-circle-o-left";
            break;

        case 96:
            $acao[3] = " ";
            $texto[3] = $mostrarProcessos[0] . " ($processosEscondidos)";
            $testoDesc[3] = $mostrarProcessos[1];
            $AcionaMenu[3] = 1;
            $tipoBotao[3] = "DROPDOWN_ITENS";
            $class_Icon[3] = "fa fa-eye";
            break;

        case 97:
            $acao[0] = "document.location = 'busca'";
            $texto[0] = $ReturnPesq[0];
            $testoDesc[0] = $ReturnPesq[1];
            $AcionaMenu[0] = 1;
            $tipoBotao = "showModal";
            $class_Icon[0] = "fa fa-search";
            break;

        case 98:
            $acao[1] = "document.location = 'busca'";
            $texto[1] = $ReturnPesq[0];
            $testoDesc[1] = $ReturnPesq[1];
            $AcionaMenu[1] = 1;
            $class_Icon[1] = "fa fa-search";

            $acao[2] = "jsCarregaWebCaptura()";
            $texto[2] = $capturarImagem[0];
            $testoDesc[2] = $capturarImagem[1];
            $AcionaMenu[2] = 1;
            $class_Icon[2] = "fa fa-camera";

            $acao[3] = " ";
            $texto[3] = $mostrarProcessos[0] . " ($processosEscondidos)";
            $testoDesc[3] = $mostrarProcessos[1];
            $AcionaMenu[3] = 1;
            $tipoBotao[3] = "DROPDOWN_ITENS";
            $class_Icon[3] = "fa fa-list";
            $make_block[3] = "BLOCK_TOOLBAR_PROCESSOS";

            $acao[4] = " ";
            $texto[4] = ($workSpaceSelByUser == "") ? "Workplaces" : "Workplace -" . $workSpaceSelByUser;
            $testoDesc[4] = $mostrarProcessos[1];
            $AcionaMenu[4] = 1;
            $tipoBotao[4] = "DROPDOWN_ITENS";
            $class_Icon[4] = "fa fa-th-large";
            $make_block[4] = "BLOCK_TOOLBAR_WORKSPACES";
            $button_id[4] = "TOOLBAR-BUTTON-WORKSPACES";

//            $acao[5] = " ";
//            $texto[5] = "Administração";
//            $testoDesc[5] = $mostrarProcessos[1];
//            $AcionaMenu[5] = 1;
//            $tipoBotao[5] = "DROPDOWN_ITENS";
//            $class_Icon[5] = "fa fa-gears";
//            $make_block[5] = "BLOCK_TOOLBAR_ADMINISTRACAO";
//            $button_id[5] = "TOOLBAR-BUTTON-ADMINISTRACAO";
            break;

        case 99:
            $acao[1] = "EnviaDadosAjax()";
            $texto[1] = $GerarRelatorio[0];
            $textoDesc[1] = $GerarRelatorio[1];
            $AcionaMenu[1] = 1;

            $acao[5] = "BPMAdminReports.php";
            $texto[5] = $Cancelar[0];
            $textoDesc[5] = $Cancelar[1];
            $AcionaMenu[5] = 1;
            break;

        case "entradamobile":
            $jsonBotoes = file_get_contents(FILES_ROOT . "assets/config/botoes_entradamobile.json");
            $listaBotoes = json_decode($jsonBotoes, true);
            break;
    }

    for ($i = 0; $i < 6; $i++) {
        if ($AcionaMenu[$i]) {
//		$Link[$i] = " <a href=\"#\" onclick=\"" . $acao[$i] . "\" title=\"" . $textoDesc[$i] . "\" onmouseOver=\"mostra_desc_toolbar('" . $textoDesc[$i] . "');" . $MouseOver[0] . ";\" onmouseout=\"window.status= '';this.style.textDecoration=''; " . $MouseOut[$i] ."\" id=\"bttoolbar0\">" . $Imagem[$i] . $texto[$i] . "</a>";			
            $Link[$i] = "<a href=\"" . $acao[$i] . "\" >" . $Imagem[$i] . $texto[$i] . "</a>";
            $Link_Float[$i] = "<a href=\"" . $acao[$i] . "\" >" . $Imagem[$i] . $texto[$i] . "</a>";
        }
    }

    // Popula a lista de Botões se ela não foi criada
    if (!isset($listaBotoes)) {
        $listaBotoes = array();
        $itemBotao = array();
        for ($i = 0; $i < 6; $i++) {
            $itemBotao["acao"] = $acao[$i];
            $itemBotao["texto"] = $texto[$i];
            $itemBotao["textoDesc"] = $textoDesc[$i];
            $itemBotao["AcionaMenu"] = $AcionaMenu[$i];
            $itemBotao["Link"] = $Link[$i];
            $itemBotao["Link_Float"] = $Link_Float[$i];
            $itemBotao["Imagem"] = $Imagem[$i];
            $itemBotao["class_Icon"] = $class_Icon[$i];
            $itemBotao["TipoBotao"] = $tipoBotao[$i];
            $itemBotao["Target"] = $Target[$i];
            $itemBotao["opcoesMenu"] = $opcoesMenu[$i];
            $itemBotao["make_block"] = $make_block[$i];
            $itemBotao["button_id"] = $button_id[$i];            
            array_push($listaBotoes, $itemBotao);
        }
    }
    return $listaBotoes;
}

// </editor-fold>

