var PopUpAtivo;
var PopUps;
var TableProgress;

function JSCriaIdInputs()
{
    return;
    $('input:not[id]').each(function (index, item) {
        try
        {
            (item !== null) ? item.attrib('id', item.name) : null;
        } catch (err)
        {
            console.log(err);
            console.log(item);
        }

    });
}

function jsGetDataFromForm(idForm)
{
    return data = $('#' + idForm).serializeArray().reduce(function (obj, item) {
        obj[item.name] = item.value;
        return obj;
    }, {});
}


function TableProgress(PosAtual, Total, TableProgress)
{
    var Posicao = parseInt(PosAtual * 100 / Total);
    Table = document.getElementById(TableProgress)
    i = 0;
    classTR = "JobIncomplete30";
    if (Posicao >= 60)
    {
        classTR = "JobIncomplete60";
    }
    if (Posicao >= 60)
    {
        classTR = "JobIncomplete90";
    }
    Table.tBodies[0].rows[0].className = classTR;

    for (i = 1; i <= Posicao; i++)
    {
        Table.tBodies[0].rows[0].cells[i - 1].className = "JobComplete";
    }
}


function TableProgress_Start(TableProgress)
{
    Table = document.getElementById(TableProgress)
    for (i = 1; i <= 100; i++)
    {
        Table.tBodies[0].rows[0].cells[i - 1].className = "";
    }
}


function Processando()
{
    $(".loading").show();
}

function FimProcessamento()
{
    $(".loading").hide();
}

/**
 * 
 * @param {type} url
 * @param {type} DivId
 * @param {type} DivOld
 * @param {type} Posicionar
 * @param {type} CampoFocus
 * @param {type} callBack
 * @returns {undefined}
 */
function jsCarregaConteudoEmDiv(url, DivId, DivOld, Posicionar, CampoFocus, callBack)
{
    Processando();
    Posicionar = true;
    var xmlhttp = CriaconexaoAjax();
    url = url.replace(new RegExp("\\+", "g"), "%2B");
    xmlhttp.open("GET", url, true);
    if (DivOld === "")
    {
        DivOld = null;
    }
    xmlhttp.onreadystatechange = function ()
    {
        Processando();
        if (xmlhttp.readyState === 4)
        {
            //Exibe o texto no div conte�do
            var objDestino = $("#" + DivId);
            if (objDestino !== null)
            {
                //conteudo.show();
                objDestino[0].innerHTML = xmlhttp.responseText;
            }
            if (!(DivOld === null))
            {
                $("#" + DivOld).hide();
            }
            eval(callBack);
            SetaFocus(CampoFocus);
            FimProcessamento();
            xmlhttp = null;
            JSCriaIdInputs();
        }
    };
    xmlhttp.send(null);
}

function _jsCarregaConteudoEmDiv(url, DivId, DivOld, Posicionar, CampoFocus, callBack)
{
    Processando();
    Posicionar = true;
    $.ajax({
        url: url
    }).done(function (retorno) {
        var objDestino = $("#" + DivId);
        if (objDestino !== null)
        {
            //conteudo.show();
            objDestino[0].innerHTML = retorno;
        }
        if (!(DivOld === null))
        {
            $("#" + DivOld).hide();
        }
        eval(callBack);
        SetaFocus(CampoFocus);
        FimProcessamento();
        xmlhttp = null;
        JSCriaIdInputs();

    }).fail(function () {
        console.log("Erro Carregando");
    });
}


function jsCarregaConteudoEmDivTime(path, DivId, DivOld, Posicionar, CampoFocus)
{
    Processando();
    Posicionar = true;
    var xmlhttp = CriaconexaoAjax();
    path = path.replace(new RegExp("\\+", "g"), "%2B");
    xmlhttp.open("GET", path, true);
    if (DivOld === "")
    {
        DivOld = null;
    }
    Time = "";
    xmlhttp.onreadystatechange = function ()
    {
        Processando();
        if (xmlhttp.readyState === 4)
        {
            //Exibe o texto no div conte�do
            currentTime = new Date();
            Time = Time + ' Pegando Objeto: ' + currentTime.getSeconds();
            var conteudo = document.getElementById(DivId);
            currentTime = new Date();
            Time = Time + ' Pegou Objeto: ' + currentTime.getSeconds();
            if (conteudo !== null)
            {
                conteudo.style.display = "";
                if (conteudo.style.top === '')
                {
                    conteudo.style.top = 100 + document.body.scrollTop + 'px';
                }
                currentTime = new Date();
                Time = Time + ' Encerrando: ' + currentTime.getSeconds();
                conteudo.innerHTML = xmlhttp.responseText;
                alert(Time);
            }
            if (DivOld !== null)
            {
                $("#" + DivOld).hide();
            }
            SetaFocus(CampoFocus);
            FimProcessamento();
            xmlhttp = null;
            JSCriaIdInputs();
        }
    };
    xmlhttp.send(null);
}

function SetaFocus(CampoFocus)
{
    if (CampoFocus !== null)
    {
        if (document.getElementById(CampoFocus) !== null)
        {
            document.getElementById(CampoFocus).focus();
        }
    }
}

function jsCarregaConteudoEmDiv2(path, DivId, DivOld, Posicionar)
{

    var xmlhttp = CriaconexaoAjax();
    path = path.replace(new RegExp("\\+", "g"), "%2B");
    xmlhttp.open("GET", path, true);
    if (DivOld === "")
    {
        DivOld = null;
    }
    xmlhttp.onreadystatechange = function ()
    {
        if (xmlhttp.readyState === 4)
        {
            //L� o texto
            var texto = xmlhttp.responseText;

            //Desfaz o urlencode
            texto = texto.replace(/\+/g, " ");
            texto = unescape(texto);

            //Exibe o texto no div conte�do
            var conteudo = document.getElementById(DivId);
            conteudo.style.display = "";
            if (Posicionar)
            {
                conteudo.style.top = 50 + document.body.scrollTop + 'px';
            }
            conteudo.innerHTML = texto;
            if (!(DivOld === null))
            {
                var conteudo = document.getElementById(DivOld)
                conteudo.style.display = "none";
            }
            JSCriaIdInputs();
        }
    }
    xmlhttp.send(null);
}

function CarregaFunc(path)
{
    var xmlhttp = CriaconexaoAjax();
    path = path.replace(new RegExp("\\+", "g"), "%2B");
    xmlhttp.open("GET", path, true);
    xmlhttp.onreadystatechange = function ()
    {
        Processando();
        if (xmlhttp.readyState === 4)
        {
            //L� o texto
            var texto = xmlhttp.responseText;

            var MontaFluxo = Function(texto);
            MontaFluxo();
            MontaFluxo = Function("");
            //document.getElementById('Fluxo').src = "1.php";
            MenuFloat = "MenuProc";
            FimProcessamento();
        }
    };
    xmlhttp.send(null);
}

function PopUp(Url, idDiv, CampoFocus)
{
    DesativaPopUp(PopUpAtivo);
    AdicionaPopUp(idDiv)
    PopUpAtivo = idDiv;
    if (Url === "")
    {
        ShowDiv(idDiv, '', CampoFocus);
    } else
    {
        jsCarregaConteudoEmDiv(Url, idDiv, '', true, CampoFocus);
    }
}

function ShowDiv(idDivShow, idDivHide, CampoFocus)
{
    try
    {
        if (idDivShow === "" || idDivShow === '')
        {
            idDivShow = null;
        }
        if (idDivShow !== null)
        {
            $("#" + idDivShow).show();
        }
        if (idDivHide === "" || idDivHide === '')
        {
            idDivHide = null;
        }
        if (idDivHide !== null)
        {
            $("#" + idDivHide).hide();
        }
        SetaFocus(CampoFocus);
    } catch (err)
    {

    }
}

function PopArray(Valores)
{
    if (Valores.length === 1)
    {
        Valores = null;
        return Valores;
    }
    var Temp = new Array(Valores.length - 1);
    for (i = 0; i < Valores.length - 1; i++)
    {
        Temp[i] = Valores[i];
    }
    return Temp;
}

function DesativaPopUp(idDiv)
{
    if (idDiv === null)
    {
        return;
    }
    var DescPopUp = document.getElementById("DescPopUp" + idDiv);
    if (DescPopUp !== null)
    {
        DescPopUp.className = "DivTopPopUpInactive";
        DescPopUp.onmousedown = "";
        DescPopUp.onmouseup = "";
    }
}

function AtivaPopUp(idDiv)
{
    var DescPopUp = document.getElementById("DescPopUp" + idDiv);
    if (DescPopUp !== null)
    {
        DescPopUp.className = "DivTopPopUp";
        DescPopUp.onmousedown = startDrag;
        DescPopUp.onmouseup = endDrag;
    }
}

function AdicionaPopUp(idDiv)
{
    PopUps = AddArray(PopUps, idDiv);
}

function EscondePopUp()
{
    if (PopUps === null)
        return;
    endDrag();
    PopUpAtivo = PopUps[PopUps.length - 1];
    PopUps = PopArray(PopUps)
    if (PopUpAtivo === '' || PopUp === "")
    {
        PopUpAtivo = null;
    }
    if (PopUpAtivo !== null)
    {
        var Obj = document.getElementById(PopUpAtivo);
        if (Obj !== null)
        {
            Obj.style.display = "none";
            Obj.innerHtml = "";
        }
    }
    if (PopUps !== null)
    {
        PopUpAtivo = PopUps[PopUps.length - 1];
    }
    AtivaPopUp(PopUpAtivo);
}



function HideDiv(idDivHide)
{
    EscondePopUp();
    if (idDivHide === "" || idDivHide === '')
    {
        idDivHide = null;
    }
    if (idDivHide !== null)
    {
        $("#" + idDivHide).hide();
    }
}

function AddArray(Valores, Item)
{
    if (Valores === null)
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


function ExecutaScriptFuncaoArray(Url, Valores_Array, Finalizador)
{
    var xmlhttp = CriaconexaoAjax();
    Url_Envio = Url + Valores_Array.pop();
    if (Valores_Array.length === 0)
    {
        Url_Envio += Finalizador;
    }
    Url_Envio = Url_Envio.replace(new RegExp("\\+", "g"), "%2B");
    xmlhttp.open("GET", Url_Envio, true);
    xmlhttp.onreadystatechange = function ()
    {
        Processando();
        if (xmlhttp.readyState === 4)
        {
            var texto = xmlhttp.responseText;
            if (Valores_Array.length === 0)
            {
                //alert(Url_Envio + "\n Finalizando" + Valores_Array.length);						
                ExecutaScriptFuncao(Url_Envio);
            } else
            {
                //alert(Url_Envio + "\n" + Valores_Array.length);						
                ExecutaScriptFuncaoArray(Url, Valores_Array, Finalizador);
            }
        }

    };
    xmlhttp.send(null);
}

function ExecutaScriptFuncao(Url)
{
    var xmlhttp = CriaconexaoAjax();
    Url = Url.replace(new RegExp("\\+", "g"), "%2B");
    xmlhttp.open("GET", Url, true);
    xmlhttp.onreadystatechange = function ()
    {
        Processando()
        if (xmlhttp.readyState === 4)
        {
            var texto = xmlhttp.responseText;
            eval(texto)
        }

    };
    xmlhttp.send(null);
}

function ExecutaScriptFuncaoAlt(Url, Funcao)
{
    var xmlhttp = CriaconexaoAjax();
    Url = Url.replace(new RegExp("\\+", "g"), "%2B");
    xmlhttp.open("GET", Url, true);
    xmlhttp.onreadystatechange = function ()
    {
        Processando();
        if (xmlhttp.readyState === 4)
        {
            var texto = xmlhttp.responseText;
            if (texto === "")
            {
                eval(Funcao);
            } else
            {
                eval(texto);
            }
            FimProcessamento();
        }
    };
    xmlhttp.send(null);
}



function ExecutaScript(Url, Funcao)
{
    var xmlhttp = CriaconexaoAjax();
    Url = Url.replace(new RegExp("\\+", "g"), "%2B");
    xmlhttp.open("GET", Url, true);
    xmlhttp.onreadystatechange = function ()
    {
        Processando();
        if (xmlhttp.readyState === 4)
        {
            var texto = xmlhttp.responseText;
            if (Funcao !== null)
            {
                eval(Funcao);
            }
            FimProcessamento();
        }
    }
    xmlhttp.send(null);
}

function CarregaValor(Url, Campo)
{
    var xmlhttp = CriaconexaoAjax();
    Url = Url.replace(new RegExp("\\+", "g"), "%2B");
    xmlhttp.open("GET", Url, true);
    xmlhttp.onreadystatechange = function ()
    {
        Processando();
        if (xmlhttp.readyState === 4)
        {
            //L� o texto
            var texto = xmlhttp.responseText;

            //Desfaz o urlencode
            //texto=texto.replace(/\+/g," ")
            //texto=unescape(texto)

            //Exibe o texto no div conte�do
            var conteudo = document.getElementById(Campo);
            if (conteudo.tagName === "SPAN")
            {
                conteudo.innerHTML = texto;
            } else
            {
                conteudo.value = texto;
            }
            FimProcessamento();
        }
    };
    xmlhttp.send(null);
}

function VisualizaArquivo(Arquivo)
{
    url = "callajax.php?script=" + Arquivo;
    jsCarregaConteudoEmDiv(url, "MostraArquivo");
}
