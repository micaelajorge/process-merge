// //Versao 1.0.0 /Versao
var PopUpAtivo;
try {
    xmlhttp = new XMLHttpRequest();
} catch (ee) {
    try {
        xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");
    } catch (e) {
        try {
            xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
        } catch (E) {
            xmlhttp = false;
        }
    }
}

function CriaconexaoAjax()
{
    try
    {
        _xmlhttp = new XMLHttpRequest();
    } catch (ee)
    {
        try
        {
            _xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");
        } catch (e)
        {
            try
            {
                _xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
            } catch (E)
            {
                _xmlhttp = false;
            }
        }
    }
    return _xmlhttp;
}

function CriaAjax()
{
    return CriaconexaoAjax();
}

function getFormValues(form, valFunc, asArray)
{
    var str = "";
    var valueArr = null;
    var val = "";
    var cmd = "";
    var fobj = document.forms[form];
    var sRetorno = new Array;
    for (var i = 0; i < fobj.elements.length; i++)
    {
        if (fobj.elements[i].disabled === true)
            continue;
        switch (fobj.elements[i].type)
        {
            case "text":
            case "textarea":
            case "hidden":
                if (valFunc)
                {
                    //use single quotes for argument so that the value of 
                    //fobj.elements[i].value is treated as a string not a literal 
                    cmd = valFunc + "(" + 'fobj.elements[i].value' + ")";
                    val = eval(cmd);
                }
                if (fobj.elements[i].value !== "")
                {
                    str += fobj.elements[i].name + "=" + escape(fobj.elements[i].value) + "&";
                }
                break;
            case "checkbox":
                if (!fobj.elements[i].checked)
                    continue;
                str += fobj.elements[i].name +
                        "=" + escape(fobj.elements[i].value) + "&";
                break;
            case "select-one":
                str += fobj.elements[i].name +
                        "=" + fobj.elements[i].options[fobj.elements[i].selectedIndex].value + "&";
                break;
        }
    }
    str = str.substr(0, (str.length - 1));
    return str;
}

function getFormValuesArray(form, valFunc)
{
    var Valores = new Array;
    var Retorno = new Array;
    var str = "";
    var cmd = "";
    var fobj = document.forms[form];
    for (var i = 0; i < fobj.elements.length; i++)
    {
        if (fobj.elements[i].disabled === true)
            continue;
        switch (fobj.elements[i].type)
        {
            case "text":
            case "textarea":
            case "hidden":
                if (valFunc)
                {
                    //use single quotes for argument so that the value of 
                    //fobj.elements[i].value is treated as a string not a literal 
                    cmd = valFunc + "(" + 'fobj.elements[i].value' + ")";
                    val = eval(cmd)
                }
                if (fobj.elements[i].value !== "")
                {
                    str = fobj.elements[i].name + "=" + escape(fobj.elements[i].value) + "&";
                }
                break;
            case "checkbox":
                if (!fobj.elements[i].checked)
                    continue;
                str = fobj.elements[i].name +
                        "=" + escape(fobj.elements[i].value) + "&";
                break;
            case "select-one":
                str = fobj.elements[i].name +
                        "=" + fobj.elements[i].options[fobj.elements[i].selectedIndex].value + "&";
                break;
        }
        Valores.push(str);
    }

    if (Valores.length === 0)
    {
        return;
    }
    UltimoValor = Valores[Valores.length - 1];
    Valores[Valores.length - 1] = UltimoValor.substr(0, UltimoValor.length - 1);
    str = Valores[0];
    for (i = 1; i < Valores.length; i++)
    {
        Provisorio = str + Valores[i];
        if (Provisorio.length > 1000)
        {
            Retorno.push(str + ' ');
            str = '';
        }
        str += Valores[i];
    }
    Retorno.push(str);
    return Retorno;
}


function ValoresForm(Form)
{
    concatenador = '';
    valor = '';
    for (i = 0; i < Form.length; i++)
    {
        concatenador = '&';
        switch (Form.elements[i].type)
        {
            case "text":
            case "hidden":
            case "textarea":
                if (Form.elements[i].value !== "")
                {
                    valor = valor + Form.elements[i].name + '=' + Form.elements[i].value + concatenador;
                }
                break;
            case "select-one":
                valor = valor + Form.elements[i].name + '=' + Form.elements[i].value + concatenador;
                break;
            case "radio":
            case "checkbox":
                if (Form.elements[i].checked)
                {
                    valor = valor + Form.elements[i].name + '=' + Form.elements[i].value + concatenador;
                }
                break;
            case "list":
                valor = valor + Form.elements[i].name + '=' + Form.elements[i].value + concatenador;
                break;
        }
    }
    return valor;
}

function PopUp(Url, idDiv, CampoFocus)
{
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
            document.getElementById(idDivShow).style.display = "";
        }
        if (idDivHide === "" || idDivHide === '')
        {
            idDivHide = null;
        }
        if (idDivHide !== null)
        {
            document.getElementById(idDivHide).style.display = "none";
        }
        SetaFocus(CampoFocus);
    } catch (err)
    {

    }
}