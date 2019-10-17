var listaStepsNovoCaso = [];

function jsCriaNovoCaso(PageEdit, indiceStepId)
{
    StepIdNovoCaso = listaStepsNovoCaso[indiceStepId];
    document.FormAcessoCaso.action = PageEdit;
    document.FormAcessoCaso.CaseNum.value = 0;
    document.FormAcessoCaso.StepId.value = StepIdNovoCaso;
    document.FormAcessoCaso.submit();
}

function SelecionarPassoNovoCaso()
{
    document.getElementById("DivSelStepNovoCaso").style.display = "";
    document.getElementById("DivSelStep").style.display = "none";
}

function EscondeSelStepNovoCaso()
{
    document.getElementById("DivSelStepNovoCaso").style.display = "none";
    document.getElementById("DivSelStep").style.display = "";
}


function SelecionaStepNovoCaso(StepId, PageEdit)
{
//    StepIdNovoCaso = StepId
    jsCriaNovoCaso(PageEdit, StepId);
}

// //Versao 1.0.0 /Versao
// JavaScript Document

var LoadValues;
var Descarregando = 0;

function MudaStepSel(Objeto)
{
    $("#StepFiltro").attr("value", $(Objeto).attr("value"));
    $("#LimparFiltros").value = 0;
    $("#SearchForm").submit();
}


function MudaKeyFiltro(ObjetoValor, IdFiltro)
{
    document.getElementById('FCampo' + IdFiltro).value = ObjetoValor.value;
    document.SearchForm.LimparFiltros.value = 0;
    document.SearchForm.submit();
}

function changecss(theClass, element, value)
{
    var cssRules;
    if (document.all)
    {
        cssRules = 'rules';
    } else if (document.getElementById)
    {
        cssRules = 'cssRules';
    }
    for (var S = 0; S < document.styleSheets.length; S++)
    {
        for (var R = 0; R < document.styleSheets[S][cssRules].length; R++)
        {
            if (document.styleSheets[S][cssRules][R].selectorText == theClass)
            {
                document.styleSheets[S][cssRules][R].style[element] = value;
            }
        }
    }
}

function MostraFieldKeys()
{
    changecss('.trFieldKeys', 'display', '')
    var MostraMaisFiltros = document.getElementById('MostraMaisFiltros');
    if (MostraMaisFiltros != null)
    {
        MostraMaisFiltros.style.display = 'none';
    }
}

function PosicionaSearchForm()
{
    var Master = document.getElementById("DivMasterSearch");
    Master.style.display = "";
    var left = document.body.clientWidth / 2 - Master.clientWidth / 2;
    var top = document.body.scrollTop + document.body.document.body.clientHeight / 2 - Master.clientHeight / 2;
    Master.style.left = left + document.body.scrollLeft + "px";
    Master.style.top = top + "px";
}

function PegaNomeUser(Objeto, ID)
{
    url = "callajax.php?file=GetNameUser.php&UserId=" + ID;
    var xmlhttp = CriaAjax();
    xmlhttp.open("GET", url, true);
    xmlhttp.onreadystatechange = function ()
    {
        if (xmlhttp.readyState == 4)
        {
            //L o texto
            var texto = xmlhttp.responseText

            //Desfaz o urlencode
            texto = texto.replace(/\+/g, " ");

            var conteudo = document.getElementById(Objeto)
            conteudo.innerHTML = texto
        }
    }
    xmlhttp.send(null)
}

function AddArray(Valores, Item)
{
    if (Valores == null)
    {
        var Temp = new Array(1);
        i = 0;
    } else
    {
        var Temp = new Array(Valores.length + 1);
        for (i = 0; i < Valores.length; i++)
        {
            Temp[i] = Valores[i];
        }
    }
    Temp[i] = Item;
    return Temp;
}

function dPegaValorCampo(Objeto, ProcId, CaseNum, FieldId, FieldType, AtualizaExportKey)
{
    url = "callajax.php?file=GetFieldValue.php&ProcIdField=" + ProcId
    url = url + "&CaseNumField=" + CaseNum
    url = url + "&FieldId=" + FieldId
    url = url + "&Tipo=" + FieldType
    url = url + "&AtualizaExportKey=" + AtualizaExportKey
    var Dado
    Dado = AddArray(Dado, url);
    Dado = AddArray(Dado, Objeto);
    LoadValues = AddArray(LoadValues, Dado);
}

function MostraDados()
{
    /*	
     n = LoadValues.length;
     for (i = 0; i < n; i++)
     {
     Dado = LoadValues[i];
     document.write(Dado + '---- <br>');
     }
     */
}

function PegaValorCampo(Objeto, ProcId, CaseNum, FieldId, FieldType, AtualizaExportKey)
{
    url = "callajax.php?file=GetFieldValue.php&ProcIdField=" + ProcId
    url = url + "&CaseNumField=" + CaseNum
    url = url + "&FieldId=" + FieldId
    url = url + "&Tipo=" + FieldType
    url = url + "&AtualizaExportKey=" + AtualizaExportKey

    var xmlhttp = CriaAjax();

    var conteudo = document.getElementById(Objeto)
    if (conteudo != null)
    {
        conteudo.innerHTML = 'Carregando...';
    }
    xmlhttp.open("GET", url, true);
    xmlhttp.onreadystatechange = function ()
    {
        if (Descarregando)
        {
            xmlhttp.abort();
            xmlhttp = null;
            result;
        }
        if (xmlhttp.readyState == 4)
        {
            //L o texto
            var texto = xmlhttp.responseText
            //Desfaz o urlencode
            //texto=texto.replace(/\+/g," ");

            var conteudo = document.getElementById(Objeto)
            conteudo.innerHTML = texto
        }
    }
    xmlhttp.send(null)
}

function MudaOrdem(CampoOrdem, Ordem)
{
    document.SearchForm.LimparFiltros.value = 0;
    document.SearchForm.CampoOrdem.value = CampoOrdem;
    document.SearchForm.Ordem.value = Ordem;
    document.SearchForm.submit();
}


function MudaSelecaoStatus(Destino)
{
    document.SearchForm.Selecionado.value = document.getElementById('Status').value;
    document.SearchForm.LimparFiltros.value = 0;
    document.SearchForm.Pagina.value = 1;
    SubmitSearchFormXML(Destino);
}

function MudaPagina(Pagina, Destino)
{
    document.SearchForm.LimparFiltros.value = 0;
    document.SearchForm.Pagina.value = Pagina;
    Destino = Destino + '?Pagina=' + Pagina;
    SubmitSearchFormXML(Destino);
}


function MostraSearchForm()
{
    //PosicionaSearchForm();
    var DivSearch = document.getElementById('DivSearch');
    if (DivSearch.style.display == '')
    {
        EscondeSearchForm()
        return
    }
    document.getElementById('DivSearch').style.display = '';
    var DivSelStep = document.getElementById('DivSelStep');
    if (DivSelStep != null)
    {
        DivSelStep.style.display = 'none';
    }
    document.SearchForm.Pagina.value = 1;
}

function EscondeSearchForm()
{
    document.getElementById("DivSearch").style.display = "none";
    var DivSelStep = document.getElementById('DivSelStep');
    if (DivSelStep != null)
    {
        DivSelStep.style.display = '';
    }
}

function LimpaCampo(Campo)
{
    document.getElementById(Campo).value = ""
}

function LimpaPesquisa()
{
    for (i = 0; i < document.SearchForm.elements.length; i++)
    {
        if ((document.SearchForm.elements[i].type == 'text' || document.SearchForm.elements[i].type == 'hidden') && document.SearchForm.elements[i].name.indexOf('FCampo') >= 0)
        {
            document.SearchForm.elements[i].value = '';
        }
    }
}

function jsAtualizarQueue()
{
    if (oTable !== null)
    {
        oTable.ajax.reload();
    }
}

/*
 function jsAtualizarQueue(Pagina)
 {
 document.SearchForm.LimparFiltros.value = 0;
 document.SearchForm.Refresh.value = 1;
 document.SearchForm.action = Pagina;
 document.SearchForm.submit();
 }
 */

function SubmitSearchFormXML(Pagina)
{
    document.SearchForm.action = Pagina;
    document.SearchForm.submit();
}

var Selecionar = 1;

function jsNovoCaso(Campo, Valor, AdHoc, ProcId, CaseNum, ProcName, From, Pagina, PageEdit, AC)
{
    StepId = StepIdNovoCaso;

//	objSel = document.getElementById('SelStepNewCase')
//	Indice = objSel.selectedIndex;
//	if (Indice == 0)
//		{
//		alert('Selecione um Passo para iniciar um novo caso')
//		return
//		}
//	StepId = objSel.options[Indice].value;
    AcessaCaso(Campo, Valor, AdHoc, ProcId, CaseNum, StepId, ProcName, From, Pagina, PageEdit, AC);
}

// TODO: ESTÀ BLOQUANDO CASO MAS NÂO verfificando se é o mesmo usuário
function jsVerificaBloqueioCaso()
{
    procId = $("#ProcId").val();
    stepId = $("#StepId").val();
    caseId = $("#CaseNum").val();

    dadosEnvio = {
        "procId": procId,
        "stepId": stepId,
        "caseNum": caseId
    };

    $.ajax({
        type: "POST",
        url: "api/casobloqueado/",
        context: document.body,
        data: dadosEnvio
    }).done(function (dadosRetorno) {
        jsAbrirCasoEdicao(dadosRetorno);
    });
}

function jsAbrirCasoEdicao(dadosRecebidos)
{
    if (dadosRecebidos.bloqueio == 0)
    {
        document.FormAcessoCaso.submit();
    } else {
        alert("Caso está aberto por outro usuário");
    }
}

function jsAcessaCaso(Campo, Valor, AdHoc, ProcId, CaseNum, StepId, ProcName, From, Pagina, PageEdit, AC)
{

    if (Selecionar == 1)
    {
        document.FormAcessoCaso.AdHoc.value = AdHoc;
        document.FormAcessoCaso.ProcId.value = ProcId;
        document.FormAcessoCaso.StepId.value = StepId;
        document.FormAcessoCaso.CaseNum.value = CaseNum;
        document.FormAcessoCaso.From.value = From;
        document.FormAcessoCaso.Pagina.value = Pagina;
        document.FormAcessoCaso.ProcName.value = ProcName;
        document.FormAcessoCaso.action = PageEdit + '/Pagina/' + Pagina;
        jsVerificaBloqueioCaso();
    } else
    {
        document.getElementById('F' + Campo).value = Valor;
        document.formPesquisa.submit();
    }
}

function SubmitSearchForm(Pagina)
{
    url = "callajax.php?file=" + Pagina + "&" + ValoresForm(document.SearchForm);
    var xmlhttp = CriaAjax();
    xmlhttp.open("GET", url, true);
    EscondeSearchForm();
    xmlhttp.onreadystatechange = function ()
    {
        if (xmlhttp.readyState == 4)
        {
            //L o texto
            var texto = xmlhttp.responseText

            //Desfaz o urlencode
            texto = texto.replace(/\+/g, " ");
            //texto=unescape(texto);

            var conteudo = document.getElementById("DivConteudoQueue")
            conteudo.innerHTML = texto
            document.getElementById("Carregando").style.visibility = "hidden";
            //CustonOnScroll();
        }
    };
    xmlhttp.send(null);
}

function jsShowFilters(procId)
{
    url = "queuefilters/" + procId;
    $.ajax({
        url: url,
        method: "POST"
    })
            .done(function (retorno) {
                // Sucesso em gravar
                $("#ModalContent").html(retorno);
                $("#crModalQueueFilter").modal("show");
            })
            .fail(function (retorno) {
                // Falha ao gravar
                alert(retorno);
            });
}

function jsFilterQueue()
{
    url = "queuefilter/";
    $.ajax({
        url: url,
        method: "POST"
    })
            .done(function (retorno) {
                // Sucesso em gravar
                $("#ModalContent").html(retorno);
                $("#crModalQueueFilter").modal("show");
            })
            .fail(function (retorno) {
                // Falha ao gravar
                alert(retorno);
            });
}