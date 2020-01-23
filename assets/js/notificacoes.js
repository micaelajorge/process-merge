/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 23/07/2018
 Sistema: Process_XAMPP
 */

/**
 * 
 * @param {type} data
 * @returns {undefined}
 */
function jsAtualizaNotificacoes(data)
{
    if (data.total > 0)
    {
        $("#notificacoesUsuario").addClass("text-danger");
        $("#notificacoesUsuario").removeClass("hidden");
        $("#notificacoesUsuario").html(data.total);
        $("#notificacoesNr").html(data.total);
    }
}

/**
 * 
 * @returns {undefined}
 */
function jsSetaNotificacoesLidas()
{
    $.ajax({
        url: "notificacoeslidas",
        context: document.body
    }).done(function (data) {
        $("#notificacoesUsuario").addClass("hidden");
        $("#notificacoesNr").html(0);
    });
}

/*
 * 
 * @param {type} data
 * @returns {undefined}
 */
function jsAtualizaNotificacoes(data)
{
    if (data.total > 0)
    {
        $("#notificacoesUsuario").addClass("text-danger");
        $("#notificacoesUsuario").removeClass("hidden");
        totalIcone = data.total;
        if (data.total > 100)
        {
            totalIcone = "+ 99";
        }
        $("#notificacoesUsuario").html(totalIcone);
        $("#notificacoesNr").html(data.total);
    } else {
        $("#notificacoesUsuario").removeClass("text-danger");
        $("#notificacoesUsuario").addClass("hidden");
        $("#notificacoesUsuario").html("data.total");
        $("#notificacoesNr").html("");
    }
}

function jsAtualizaFlags(data)
{
    a = moment(data.ultimaAtualizacao);
    b = moment();
    diferenca = b.diff(a, 'minutes');
    if (diferenca > 6)
    {
        $("#iconStatusAutomato").removeClass("hidden");
        texto = "Automato Desatualizado - " + moment(data.ultimaAtualizacao).format("DD/MM/Y HH:mm:ss");
    } else {
        status = "executando";
        if (data.automatoPausado === true)
        {
            $("#iconStatusAutomato").removeClass("hidden");
            status = "pausado";
        } else {
            $("#iconStatusAutomato").addClass("hidden");
        }        
        texto = "Automato " + status + " - " + moment(data.ultimaAtualizacao).format("DD/MM/Y HH:mm:ss");
    }    
    $("#divStatusAutomato").html(texto);
}

function jsVerificaStatus()
{

    $.ajax({
        method: "POST",
        url: "api/checkstatus",
        context: document.body
    }).done(function (data) {
        jsAtualizaFlags(data);
    });    
    
}


/**
 * 
 * @returns {undefined}
 */
function jsVerficaNotificacoesUsuario()
{    

    setTimeout(jsVerficaNotificacoesUsuario, 300000);

    jsVerificaStatus();
    
    $.ajax({
        url: "notificacoes",
        context: document.body
    }).done(function (data) {
        jsAtualizaNotificacoes(data);
    }).fail(function (data) {
//        $("#modalSessaoExpirada").show();;
        window.location = "cessaoencerrada";
        return;
    });
    
    if (typeof jsbuscaBloqueioCaso === 'function')
    {
        jsbuscaBloqueioCaso();
    }

    if (typeof jsAtualizarQueue === 'function')
    {
        jsAtualizarQueue('queue');
    }    
}

//setTimeout(jsVerficaNotificacoesUsuario, 30000);

function jsInicializacaoPaginaQueue()
{        
    jsVerficaNotificacoesUsuario();
}

$(document).ready(function () {
    jsInicializacaoPaginaQueue();
});
