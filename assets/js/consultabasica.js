/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 04/12/2019
 Sistema: auto_securities
 */

function jsInicializacaoPagina()
{
    $.get("queuefilters/tribanco/t_consultabasica_form.html/t_queue_filter_custon_1.html", (dadosRetornados) =>
    {
        $("#camposConsulta").html(dadosRetornados);
    });
}

$(document).ready(function () {
    jsInicializacaoPagina();
});