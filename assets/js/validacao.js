// //Versao 1.0.0 /Versao
// JavaScript Document

function scroll_to_anchor(anchor_id) {
    var tag = $(anchor_id);
    if (tag.offset() !== undefined)
    {
        topAnchor = tag.offset().top;
        $('html,body').animate({
            scrollTop: topAnchor
        }
        , 'slow');
    }
}

function jsValidaDocumentos()
{
    listaDocumentos = $("#divDocumentosProcesso").find(".box-warning").find(".texto-documento");
    if (listaDocumentos.length > 0)
    {        
        alert("Necessário anexar documento " + $(listaDocumentos).html().trim());
        return false;
    }    
    return true;
}

function ValidaCampoRU()
{
    for (var i = 1; i <= RumOb; i++)
    {
        if (document.getElementById("RumObr" + i).checked == false)
        {
            alert("Há documentos obrigatórios não selecinados");
            document.getElementById('RumBox').style.visibility = 'visible';
            return false;
        }
    }
    return true;
}


function ValidaFD(campo, nome, hook, optional)
{
    if ((optional == 0) && (campo.value == 0))
    {
        alert("É obrigatório anexar arquivos ao campo " + nome);
        if (window.parent.location.href == window.location.href)
        {
            scroll_to_anchor("#Hook" + hook);
        }
        return false;
    }
    return true;
}


function ValidaIFD(campo, nome, hook, optional)
{
    if ((optional == 0) && (campo.value == 0))
    {
        alert("Não há imagens");
        if (window.parent.location.href == window.location.href)
        {
            scroll_to_anchor("#Hook" + hook);
        }
        return false;
    }
    return true;
}

function ValidaNU(campo, nome, hook, optional)
{
    if ((optional == 0) && (campo.value == ""))
    {
        alert("Campo " + nome + " deve ser preenchido");
        if (window.parent.location.href == window.location.href)
        {
            scroll_to_anchor("#Hook" + hook);
        }
        campo.focus();
        return false;
    }
    if (campo.value != "")
    {
        var valor = parseFloat(campo.value);
        if (valor != campo.value)
        {
            alert("Campo " + nome + " deve ser Numérico");
            if (window.parent.location.href == window.location.href)
            {
                scroll_to_anchor("#Hook" + hook);
            }
            campo.focus();
            return false;
        }
    }
    return true;
}


function ValidaUS(campo, nome, hook, optional)
{
    if ((optional == 0) && (campo.value == ""))
    {
        alert("Um usuário deve ser selecionado no campo " + nome);
        if (window.parent.location.href == window.location.href)
        {
            scroll_to_anchor("#Hook" + hook);
        }
        return false;
    }
    return true;
}

function ValidaTX(campo, nome, hook, optional)
{
    if ((optional == 0) && (campo.value == ""))
    {
        alert("Campo " + nome + " deve ser preenchido");
        if (window.parent.location.href == window.location.href)
        {
            scroll_to_anchor("#Hook" + hook);
        }
        $("#STATUS_F" + hook).removeClass('has-warning');
        $("#STATUS_F" + hook).addClass('has-error');
        campo.focus();
        return false;
    }
    return true;
}

function ValidaListaArray(Form, campo, nome, hook, optional)
{
    var fobj = document.forms[Form];
    var objeto = null;
    var retornoValida = false;
    if (optional == 1)
        return true;
    for (var i = 0; i < fobj.elements.length; i++)
    {
        if (campo == fobj.elements[i].name)
        {
            if (objeto == null)
            {
                objeto = fobj.elements[i];
            }
            if (fobj.elements[i].selectedIndex > 0)
            {
                retornoValida = true;
            }
        }
    }
    if (retornoValida == false)
    {
        alert("Selecione um dos Valores para " + nome);
        if (window.parent.location.href == window.location.href)
        {
            scroll_to_anchor("#Hook" + hook);
        }
        objeto.focus();
        return false;
    }
    return true;
}

function ValidaATX(campo, nome, hook, optional)
{
    listaObjetos = $("#t7");
    retornoValida = true;
    for (var i = 0; i < listaObjetos.length; i++)
    {
        if ($(listaObjetos[i]).val() !== "")
        {
            retornoValida = false;
            break;
        }
    }
    if (retornoValida == false && optional == 0)
    {
        alert("Campo " + nome + " deve ser preenchido");
        if (window.parent.location.href == window.location.href)
        {
            scroll_to_anchor("#Hook" + hook);
        }
        return false;
    }
    return true;
}

function ValidaANU(Form, campo, nome, hook, optional)
{
    var fobj = document.forms[Form];
    var objeto = null;
    retornoValida = false;
    for (var i = 0; i < fobj.elements.length; i++)
    {
        if (campo == fobj.elements[i].name)
        {
            if (objeto == null)
            {
                objeto = fobj.elements[i];
            }
            if (fobj.elements[i].value != "")
            {
                retornoValida = true;
                break;
            }
        }
    }
    if (retornoValida == false && optional == 0)
    {
        alert("Campo " + nome + " deve ser preenchido");
        if (window.parent.location.href == window.location.href)
        {
            scroll_to_anchor("#Hook" + hook);
        }
        objeto.focus();
        return false;
    }
    for (var i = 0; i < fobj.elements.length; i++)
        if (campo == fobj.elements[i].name)
        {
            if (fobj.elements[i].value != "")
            {
                var valor = parseFloat(fobj.elements[i].value);
                if (valor != fobj.elements[i].value)
                {
                    alert("Campo " + nome + " deve ser Numérico");
                    if (window.parent.location.href == window.location.href)
                    {
                        scroll_to_anchor("#Hook" + hook);
                    }
                    fobj.elements[i].focus();
                    return false;
                }
            }
        }
    return true;
}

function ValidaAR(campo, nome, hook, optional)
{
    if ((optional == 0) && (campo.value == ""))
    {
        alert("Por favor Anexe o arquivo");
        if (window.parent.location.href == window.location.href)
        {
            scroll_to_anchor("#Hook" + hook);
        }
        campo.focus();
        return false;
    }
    return true;
}


function ValidaIM(campo, nome, hook, optional)
{
    if ((optional == 0) && (campo.value === ""))
    {
        alert("É necessário carregar uma imagem no campo " + nome);
        if (window.parent.location.href == window.location.href)
        {
            scroll_to_anchor("#Hook" + hook);
        }
        return false;
    }
    return true;
}


function ValidaDC(campo, nome, hook, optional)
{
    if ((optional == 0) && (campo.value == "" || campo.value != parseFloat(campo.value) || campo.value == '0'))
    {
        alert("Selecione o valor para " + nome);
        if (window.parent.location.href == window.location.href)
        {
            scroll_to_anchor("#Hook" + hook);
        }
        return false;
    }
    return true;
}



function ValidaDT(campo, nome, hook, optional)
{
    if ((optional == 0) || (campo.value != ""))
    {
        if (!checkDate(campo.value))
        {
            alert("Campo " + nome + " deve ser preenchido no formato dd/mm/yyyy ou dd/mm/yyyy hh:mm");
            if (window.parent.location.href == window.location.href)
            {
                scroll_to_anchor("#Hook" + hook);
            }
            campo.focus();
            return false;
        }
    }
    return true;
}

function ValidaLista(campo, nome, hook, optional)
{
    try {
        if (optional == 1)
        {
            return true;
        }
        if (campo.selectedIndex == 0)
        {
            alert("Selecione um dos Valores para " + nome);
            if (window.parent.location.href == window.location.href)
            {
                scroll_to_anchor("#Hook" + hook);
            }
            $("#STATUS_F" + hook).removeClass('has-warning');
            $("#STATUS_F" + hook).addClass('has-error');

            campo.focus();
            return false;
        }
        return true;
    } catch (err)
    {
        alert(err);
        return false;
    }
}


function Verifica_CPFCNPJ(campo)
{
    resultado_cpf = Verifica_CPF(campo);
    resultado_cnpj = Verifica_CNPJ(campo);
    return resultado_cnpj | resultado_cpf;
}

function jsValidaCPF(CPF)
{
    if (CPF === '') {
        return false;
    }

// Aqui começa a checagem do CPF
    var POSICAO, I, SOMA, DV, DV_INFORMADO;
    var DIGITO = new Array(10);
    DV_INFORMADO = CPF.substr(9, 2); // Retira os dois últimos dígitos do número informado

// Desemembra o número do CPF na array DIGITO
    for (I = 0; I <= 8; I++) {
        DIGITO[I] = CPF.substr(I, 1);
    }

// Calcula o valor do 10º dígito da verificação
    POSICAO = 10;
    SOMA = 0;
    for (I = 0; I <= 8; I++) {
        SOMA = SOMA + DIGITO[I] * POSICAO;
        POSICAO = POSICAO - 1;
    }
    DIGITO[9] = SOMA % 11;
    if (DIGITO[9] < 2) {
        DIGITO[9] = 0;
    } else {
        DIGITO[9] = 11 - DIGITO[9];
    }

// Calcula o valor do 11º dígito da verificação
    POSICAO = 11;
    SOMA = 0;
    for (I = 0; I <= 9; I++) {
        SOMA = SOMA + DIGITO[I] * POSICAO;
        POSICAO = POSICAO - 1;
    }
    DIGITO[10] = SOMA % 11;
    if (DIGITO[10] < 2) {
        DIGITO[10] = 0;
    } else {
        DIGITO[10] = 11 - DIGITO[10];
    }

// Verifica se os valores dos dígitos verificadores conferem
    DV = DIGITO[9] * 10 + DIGITO[10];
    if (DV != DV_INFORMADO) {
//      alert('CPF inválido');
        return false;
    }
    return true;    
}

function Verifica_CPF(campo) {
    var CPF = campo.value; // Recebe o valor digitado no campo
    return jsValidaCPF(CPF);
}

function Verifica_CNPJ(campo) {
    var CNPJ = campo.value;
    erro = new String;
//		if (CNPJ.length < 18) erro += "E' necessarios preencher corretamente o numero do CNPJ! \n\n";
//		if ((CNPJ.charAt(2) != ".") || (CNPJ.charAt(6) != ".") || (CNPJ.charAt(10) != "/") || (CNPJ.charAt(15) != "-")){
//			if (erro.length == 0) erro += "E' necessarios preencher corretamente o numero do CNPJ! \n\n";
//		}
    //substituir os caracteres que nao sao numeros
    if (document.layers && parseInt(navigator.appVersion) == 4) {
        x = CNPJ.substring(0, 2);
        x += CNPJ.substring(3, 6);
        x += CNPJ.substring(7, 10);
        x += CNPJ.substring(11, 15);
        x += CNPJ.substring(16, 18);
        CNPJ = x;
    } else {
        CNPJ = CNPJ.replace(".", "");
        CNPJ = CNPJ.replace(".", "");
        CNPJ = CNPJ.replace("-", "");
        CNPJ = CNPJ.replace("/", "");
    }
    var nonNumbers = /\D/;
    if (nonNumbers.test(CNPJ))
        erro += "Digite apenas numeros! \n\n";
    campo.value = CNPJ;
    var a = [];
    var b = new Number;
    var c = [6, 5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2];
    for (i = 0; i < 12; i++) {
        a[i] = CNPJ.charAt(i);
        b += a[i] * c[i + 1];
    }
    if ((x = b % 11) < 2) {
        a[12] = 0
    } else {
        a[12] = 11 - x
    }
    b = 0;
    for (y = 0; y < 13; y++) {
        b += (a[y] * c[y]);
    }
    if ((x = b % 11) < 2) {
        a[13] = 0;
    } else {
        a[13] = 11 - x;
    }
    if ((CNPJ.charAt(12) != a[12]) || (CNPJ.charAt(13) != a[13])) {
        erro += "Digito verificador com problema!";
    }
    if (erro.length > 0) {
        return false;
    }
    return true;
}
