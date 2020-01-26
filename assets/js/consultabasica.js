/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 04/12/2019
 Sistema: auto_securities
 */


function jsCarregaFiltrosConsultaBasica()
{
    if (ProcId === undefined)
    {
        procCode = location.href.replace($("base")[0].baseURI + "consultabasica/", "");
    } else {
        procCode = ProcId
    }
    
    $.get("queuefilters/" + procCode + "/t_consultabasica_form.html/t_queue_filter_custon_1.html", (dadosRetornados) =>
    {
        $("#camposConsulta").html(dadosRetornados);
    });
}

//function jsInicializacaoPagina()
//{
//    jsCarregaFiltrosConsultaBasica();
//}
//
//$(document).ready(function () {
//    jsInicializacaoPagina();
//});

if (!Array.isArray(funcoes_final))
{
    var funcoes_final = [];
}

funcoes_final.push(jsCarregaFiltrosConsultaBasica);