// //Versao 1.0.0 /Versao
// JavaScript Document
var ExecutandoAcao = 0
var overContextMenu;
var mousePosX = 0;
var mousePosY = 0;
var Opcao;

function jsExecutaRouleProcessar(callBack)
{
    procCod = $("#ProcCode").val();
    stepCod = $("#StepCode").val();

    ruleCode = procCod + "_" + stepCod;
    url = "api/v1/rules/1/" + ruleCode;
    dadosEnvio = jsPegaDadosFormByCode('textos');
    $.ajax(
            {
                url: url,
                type: "POST",
                data: dadosEnvio,
                dataType: 'json'
            })
            .done(function (dataRetorno) {
                if (dataRetorno.resultado)
                {
                    callBack();
                } else {
                    //alert(dataRetorno.retornoMensagem + "\n" + dataRetorno.rouleCode);
                    try {
                        dadosRetornados = dataRetorno.dados;
                        for (i = 0; i < dadosRetornados.length; i++)
                        {
                            dados = dadosRetornados[i];
                            if (dados.resultado === "false" | dados.resultado === false)
                            {
                                alert(dados.mensagem + "\nRegra: " + ruleCode);
                                ExecutandoAcao = 0;
                                return;
                            }
                        }
                    } catch (e) {
                        alert("Falha Execução Regra\nRegra: " + ruleCode);
                    }
                    callBack();
                }

            }
            )
            .fail(function (jqXHR, textStatus) {
                alert("Falha Execução Regra\n" + textStatus + "\nRegra: " + ruleCode);
                ExecutandoAcao = 0;
            });
}

function AtualizaCarga()
{

}

function FecharSessao()
{

}

function jsLimpaAcaoVoltar()
{
    window.onbeforeunload = null;
    document.onunload = null;
}

function SalvarAdmin()
{
    if (ExecutandoAcao == 0)
    {
        ExecutandoAcao = 1;
        if (jsValidacaoFormulario(document.textos))
        {
            jsLimpaAcaoVoltar();
            document.textos.Acao.value = 'SalvarAdmin';
            document.textos.submit();
        } else
        {
            ExecutandoAcao = 0;
        }
    }
}

function jsSalvarDadosProcessoCT()
{
    procCode = $("#ProcCode").val();
    stepCode = $("#StepCode").val();
    jsExecutaRoleValidarFormulario(procCode + "_" + stepCode, jsSalvarDadosProcessoCT_callBack);
}

function jsSalvarDadosProcessoCT_callBack()
{
    if (ExecutandoAcao == 0)
    {
        //ExecutandoAcao = 1;
        if (jsValidacaoFormulario(document.frmEditarCaso))
        {
            jsLimpaAcaoVoltar();
            url = "fetchdata.php";
            dadosChamada = $("#frmEditarCaso").serializeArray();
            $.post(url, dadosChamada,
                    function (dataRetorno) {
                        $("#divCabecalhoProcesso").html(dataRetorno);
                        $("#modalEditDadosCaso").modal('hide');
                    });
        } else
        {
            ExecutandoAcao = 0;
        }
    }
}

function jsRotearCaso()
{
    if (ExecutandoAcao == 0)
    {
        ExecutandoAcao = 1;
        if (jsValidacaoFormulario(document.textos))
        {
            jsLimpaAcaoVoltar();
            document.textos.Acao.value = 'RotearCaso';
            document.textos.submit();
        } else
        {
            ExecutandoAcao = 0;
        }
    }
}

function jsProcessarCT()
{
    if (ExecutandoAcao === 0)
    {
        ExecutandoAcao = 1;
        if (!jsValidaDocumentos())
        {
            ExecutandoAcao = 0;
            return;
        }
        if (jsValidacaoFormulario(document.textos))
        {
            jsLimpaAcaoVoltar();
            document.textos.Acao.value = 'RotearCaso';
            document.textos.submit();
        } else
        {
            ExecutandoAcao = 0;
        }
    }
}

function callBackProcessar()
{
    jsLimpaAcaoVoltar();
    document.textos.Acao.value = 'processar';
    document.textos.submit();
}

function Processar()
{
    if (ExecutandoAcao === 0)
    {
        ExecutandoAcao = 1;
        if (!jsValidaDocumentos())
        {
            ExecutandoAcao = 0;
            return;
        }
        if (jsValidacaoFormulario(document.textos))
        {
            jsExecutaRouleProcessar(callBackProcessar);

        } else
        {
            ExecutandoAcao = 0;
        }
    }
}

function Salvar()
{
    window.onbeforeunload = null;
    document.onunload = null;
    document.textos.Acao.value = 'Salvar';
    document.textos.submit();
}

function SalvarLock()
{
    if (ExecutandoAcao == 0)
    {
        ExecutandoAcao = 1;
        if (jsValidacaoFormulario(document.textos))
        {
            document.textos.Acao.value = 'SalvarLockar';
            document.textos.submit();
        } else
        {
            ExecutandoAcao = 0;
        }
    }
}

function mostra_desc_toolbar(texto)
{
    window.status = texto;
}

function EscondeOpcao()
{
    if (EsconderOpcao == 1)
    {
        document.getElementById(Opcao).style.visibility = 'hidden';
    }
}

function TimeEscondeOpcao()
{
    EsconderOpcao = 1;
    setTimeout("EscondeOpcao()", 1000);
}


function MostraOpcao(NovaOpcao)
{
    if (Opcao != NovaOpcao)
    {
        if (Opcao != null)
        {
            EsconderOpcao = 1;
            EscondeOpcao();
        }
    }
    EsconderOpcao = 0;
    Opcao = NovaOpcao;
    getposXY();
    document.getElementById(Opcao).style.Left = PosX
    document.getElementById(Opcao).style.Top = PosY
    document.getElementById(Opcao).style.visibility = 'visible';
}


function AbreWindow(link, Nome)
{
    width = 780;
    height = 520;
    window.open(link, Nome, "toolbar=0,Location=0,directories=0,status=1,menubar=0,scrollbars=1,resizable=1,screenX=0,screenY=0,left=" + PegaPosX(width) + ",top=" + PegaPosY(height) + ",width=" + width + ", height=" + height);
}

overContextMenu = false;
function contextMenuEventHandler()
{
    MostraMenu();
    return false;
}

document.onmousemove = getMouseXY;
document.onmousedown = customContext
//document.oncontextmenu = contextMenuEventHandler;		
function MostraMenu()
{
    Menu = document.getElementById('MenuFloat').style;
    Menu.left = mousePosX;
    Menu.top = mousePosY;
    Menu.display = '';
}

function customContext()
{
    if (!overContextMenu)
    {
        Menu = document.getElementById('MenuFloat');
        if (Menu != null)
        {
            Menu.style.display = 'none';
        }
    }
}

function getMouseXY(e)
{
    try
    {
        if (document.all) {
            mousePosX = event.clientX + document.body.scrollLeft;
            mousePosY = event.clientY + document.body.scrollTop;
        } else {
            mousePosX = e.pageX;
            mousePosY = e.pageY;
        }
//		dynCalendar_mouseX = mousePosX;
//		dynCalendar_mouseY = mousePosY;
        mousePosY = mousePosY - 20;
        mousePosX = mousePosX - 50;
    } catch (e)
    {

    }
}

//window.onscroll = CustonOnScroll;
function CustonOnScroll()
{
    document.getElementById("ToolBarMenu").style.top = document.body.scrollTop + 30;
}

function MudaDocument($Document)
{
    document.location = $Document;
}

function CopyToClipBoard()
{
    var CopiedTxt = document.selection.createRange();
    CopiedTxt.execCommand("Copy");
    document.getElementById("MenuFloat").style.display = "none";
}

function PastFromClipBoard()
{
    alert(document.selection.type)
    if (document.selection.type == "Text")
    {
        CopiedTxt = document.selection.createRange();
        CopiedTxt.execCommand("Paste");
    }
    document.getElementById("MenuFloat").style.display = "none";
}