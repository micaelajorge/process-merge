/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 21/08/2018
 Sistema: Creditus
 */

function jsInicializacaoPagina()
{

}


function jsRecarregaPagina()
{
    location.reload();
}

function jsRemoveArquivoLog()
{
    $.post("errorlog/apagar", jsRecarregaPagina());    
}

$(document).ready(function () {
    jsInicializacaoPagina();
});