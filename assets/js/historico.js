// //Versao 1.0.0 /Versao
// JavaScript Document
var MostrarHistorico = false;
var HistoricoCarregado = false;

function JScarregaHistoricoCaso(CaseNum, Tipo)
{
    var xmlhttp = CriaAjax();
    xmlhttp.open("GET", "eventlist?CaseNum=" + CaseNum + "&Tipo=" + Tipo, true);
    xmlhttp.onreadystatechange = function ()
    {
        if (xmlhttp.readyState === 4)
        {
            //L� o texto
            var texto = xmlhttp.responseText;

            //Desfaz o urlencode
            texto = texto.replace(/\+/g, " ")
            //texto=unescape(texto)

            //Exibe o texto no div conte�do
            var conteudo = document.getElementById("EventListScrool");
            conteudo.innerHTML = texto;
            HistoricoCarregado = true;
        }
    };
    xmlhttp.send(null);
}

function JSmostraHistoricoCaso(CaseNum, Tipo)
{
    MostrarHistorico = true;
    //document.getElementById("EventListScrool").innerHTML = '';
    Link = "<a href=\"include/eventListWindow.php?CaseNum=" + CaseNum + "&Tipo=" + Tipo + "\" target=\"_blank\" onclick=\"EscondeHistorico()\">Abrir em nova janela</a>";
    document.getElementById("DivNewWindowHistorico").innerHTML = Link;
    if (!HistoricoCarregado || Tipo === "Admin")
    {
        JScarregaHistoricoCaso(CaseNum, Tipo);
    }    
    return;
}

function EscondeHistorico()
{
    document.getElementById("DivMasterHistorico").style.display = "none";
}