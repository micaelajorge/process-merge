/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 08/12/2018
 Sistema: Process_XAMPP
 */

var OPERADORES_US_GR = new Array("=", "#");
var OPERADORES_TX = new Array("=", "#", "%");
var OPERADORES_BO = new Array("=");
var OPERADORES_NU_DT = new Array("=", "#", "><", "<", ">", ">=", "<=");

function jsSelectFieldBusca()
{
    $("#COMPARADOR")[0].disabled = false;

    tipoCampo = $("#CAMPO_SEL option:selected").attr("aria-field-type");

    switch (tipoCampo)
    {
        case "US":
        case "GR":
            $("#PRIMEIRO_VALOR").show();
            $("#BOLEANO_VALOR").hide();
            operadores = OPERADORES_US_GR;
            break;
        case "TX":
            $("#PRIMEIRO_VALOR").show();
            $("#BOLEANO_VALOR").hide();
            operadores = OPERADORES_TX;
            break;
        case "BO":
            operadores = OPERADORES_BO;
            $("#PRIMEIRO_VALOR").hide();
            $("#BOLEANO_VALOR").show();
            break;
        case "NU":
        case "DT":
            $("#PRIMEIRO_VALOR").show();
            $("#BOLEANO_VALOR").hide();
            operadores = OPERADORES_NU_DT;
            break;
    }
    $("#BTN_ADDCRITERIO")[0].disabled = false;
    $("#COMPARADOR").children("option").each(function (indice, item) {
        (operadores.indexOf(item.value) === -1) ? $(item).hide() : $(item).show();
    });
}

function jsSelOperador()
{
    $("#SEGUNDO_VALOR")[0].disabled = true;
    $("#PRIMEIRO_VALOR")[0].disabled = false;
    switch ($("#COMPARADOR").val())
    {
        case "><":
            $("#SEGUNDO_VALOR")[0].disabled = false;
            break;
    }
}


function jsAddCriterioBusca()
{
    html = $("#criteriosBusca").html();
    txCampoSel = $("#CAMPO_SEL option:selected").html();

    tipoCampo = $("#CAMPO_SEL option:selected").attr("aria-field-type");

    aspas = "";
    if (tipoCampo == "TX" | tipoCampo == "DT")
    {
        aspas = '"';
    }

    campoSel = $("#CAMPO_SEL").val();
    primeiroValor = $("#PRIMEIRO_VALOR").val();
    comparador = $("#COMPARADOR").val();
    if (comparador === "><")
    {
        segundoValor = $("#SEGUNDO_VALOR").val();
        operacao = aspas + primeiroValor + aspas + ' entre ' + aspas + segundoValor + aspas;
    } else {
        operacao = comparador + " " + aspas + primeiroValor + aspas;
    }

    html = html + "<span class=\"badge bg-green\">" + txCampoSel + " " + operacao + "</span> ";
    $("#criteriosBusca").html(html);
}

function jsExecutaBuscaSimples()
{
    procId = $("#SELPROC").val();
    termoBusca = $('#termoBuscaSimples').val();
    if (termoBusca.length < 3)
    {
        return;
    }
    jsFilter(procId, termoBusca);
}

function jsSelProcesso()
{
    $("#botaoBusca")[0].disabled = false;
}