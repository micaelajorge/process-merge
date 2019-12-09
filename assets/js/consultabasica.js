/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 04/12/2019
 Sistema: auto_securities
 */


function jsCarregaFiltrosConsultaBasica()
{
   procCode = location.href.replace($("base")[0].baseURI + "consultabasica/", "");
    $.get("queuefilters/" + procCode + "/t_consultabasica_form.html/t_queue_filter_custon_1.html", (dadosRetornados) =>
    {
        $("#camposConsulta").html(dadosRetornados);
    });
}

function jsInicializacaoPagina()
{
    jsCarregaFiltrosConsultaBasica();
}

$(document).ready(function () {
    jsInicializacaoPagina();
});