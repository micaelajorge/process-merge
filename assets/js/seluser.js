// //Versao 1.0.0 /Versao
var selLinhaUsuarioSelecionadoAnterior = null;
var uValor;
var dValor;
var CaseNum;
var GrupoFiltro = '';
var idBotaoSelecionarUsuario;

function LimpaCampoUser(CampoDesc, CampoValue)
{
    LimpaCampo(CampoDesc);
    LimpaCampo(CampoValue);
}

// TODO: Mudar seleção de Processo na lista em 
function JSselecionaUsuarioEditCase(CampoDesc, CampoValue, Grupo, Valor, Origem)
{
    GrupoFiltro = Grupo;
    IdTCampo = CampoValue;
    IdRCampo = CampoDesc;
    document.getElementById("Filtrar").value = '';
    idBotaoSelecionarUsuario = "selButton" + CampoDesc;
    $("#" + idBotaoSelecionarUsuario).attr("disabled", "true");
    PosicionaListaUsuario();
}

//function JSselecionaUsuarioGrupoPermissao(linhaUsuarioSelecionada, userId, userName, grpFld)
//{
//    $("#tituloEntidade").show();
//    $("#nomeUsuario").html(userName);
//    $("#TITULO_EDITAR").show();
//    $("#TITULO_INSERIR").hide();
//    $("#searchEntidade").hide();
//    $("#permissoesEntidade").show();
//    $("#BTN_SALVAR")[0].disabled = false;JSremoveUsuarioGrupoPermissao
//    $("#GROUPID").val(userId);
//    $("#GRPFLD").val(grpFld);
//}


function JSselecionaUsuarioGrupoPermissao(linhaUsuarioSelecionada, userId, nomeUsuario, grpFld)
{
    if (selLinhaUsuarioSelecionadoAnterior !== null)
    {
        unsel = document.getElementById(selLinhaUsuarioSelecionadoAnterior);
        if (unsel !== null)
        {
            unsel.className = "";
        }
    }
    $("#GROUPID").val(userId);
    $("#GRPFLD").val(grpFld);
    $("#" + idBotaoSelecionarUsuario).removeAttr("disabled");
    $("#" + linhaUsuarioSelecionada).className = "alert-info";
    $("#BTN_SALVAR")[0].disabled = false;
    selLinhaUsuarioSelecionado = linhaUsuarioSelecionada;
}

function LimpaListaUsers()
{
    document.getElementById("DivMasterUser").style.display = "none";
    document.getElementById("DivConteudoUser").innerHTML = '<div class="DivListaUser"></div>';
}


function jsAtualizaUser(Valor, Display)
{
    if (Valor !== null)
    {
        document.getElementById(IdTCampo).value = Valor;
        document.getElementById(IdRCampo).value = Display;
        LimpaListaUsers();
    }
}

function PosicionaListaUsuario()
{
    var Master = document.getElementById("DivMasterUser");
    Master.style.display = "";
    var left = document.body.clientWidth / 2 - Master.clientWidth / 2;
    scrool = (document.body.scrollTop + document.body.clientHeight) / 2;
    MeioMaster = (Master.clientHeight / 2);
    if (scrool < MeioMaster)
    {
        var top = scrool;
    } else
    {
        ScrollXY = getScrollXY();
        var top = ScrollXY[1] + (document.documentElement.clientHeight / 2 - MeioMaster);
    }
    Master.style.left = left + document.body.scrollLeft + "px";
    Master.style.top = top + "px";
    document.getElementById('Filtrar').focus();

}

function getScrollXY() {
    var scrOfX = 0, scrOfY = 0;
    if (typeof (window.pageYOffset) === 'number') {
        //Netscape compliant
        scrOfY = window.pageYOffset;
        scrOfX = window.pageXOffset;
    } else if (document.body && (document.body.scrollLeft || document.body.scrollTop)) {
        //DOM compliant
        scrOfY = document.body.scrollTop;
        scrOfX = document.body.scrollLeft;
    } else if (document.documentElement && (document.documentElement.scrollLeft || document.documentElement.scrollTop)) {
        //IE6 standards compliant mode
        scrOfY = document.documentElement.scrollTop;
        scrOfX = document.documentElement.scrollLeft;
    }
    return [scrOfX, scrOfY];
}

function JSBuscaListaUsersGroups()
{
    TXFiltro = document.getElementById('Filtrar').value;
    TXtipoFiltro = "U";
    
    procId = $("#PROCID").val();
    if (document.getElementById('TIPOFILTRO') != null)
    {
        TXtipoFiltro = document.getElementById('TIPOFILTRO').value;
    }

    //url = "include/BPMSelUserAjax.php?TipoFiltro=" + TipoFiltro + "&Filtrar=" + TXFiltro + "&Origem=" + Origem + "&Grupo=" + GrupoFiltro;
    url = "selectusersgroups?TipoFiltro=" + TXtipoFiltro + "&Filtrar=" + TXFiltro + "&Grupo=" + GrupoFiltro + "&procId=" + procId;
    xmlhttp.open("GET", url, true);
    xmlhttp.onreadystatechange = function ()
    {
        if (xmlhttp.readyState === 4)
        {
            //L� o texto
            var texto = xmlhttp.responseText

            //Desfaz o urlencode
            texto = texto.replace(/\+/g, " ");
            texto = unescape(texto);

            var conteudo = document.getElementById("DivConteudoUser");
            conteudo.innerHTML = texto
        }
    };
    xmlhttp.send(null);
}


