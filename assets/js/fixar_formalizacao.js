/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 11/07/2019
 Sistema: Process_XAMPP
 */

function jsInicializacaoPagina()
{
    tamanhoAnterior = $("#divVisualizacaoFormalizacao").width();
    $("#divVisualizacaoFormalizacao").css("position", "absolute");
    $("#divVisualizacaoFormalizacao").css("width", tamanhoAnterior + "px");
    $(window).scroll(function () {
        $("#divVisualizacaoFormalizacao").css("top", $(window).scrollTop() + "px");
    });
}

$(document).ready(function () {
    jsInicializacaoPagina();
});