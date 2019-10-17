/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 01/07/2019
 Sistema: Process_XAMPP
 */


const t_body = new Template('assets/templates/gd_queue_lotes.html');


function jsCriaTemplateResumoLotes(listaLotes)
{
    console.log("Gerando Template Resumo Lotes");
    const t_body = new Template('assets/templates/t_gd_status_lotes.html');
    novaListaLotes = {};
    novaListaLotes.DATA = [];
    novaListaLotes.DATA = listaLotes;
    t_body.parseJSON(novaListaLotes);
    var parsed = t_body.parse();
    $("#divResumoLotes").html(parsed);
}


function jsCarregaListaResumoLotes()
{
    endApiStatus = 'api/v1/countbyfield/GATEWAY_DIGITAL/10';
    $.ajax({
        url: endApiStatus,
        type: "POST",
        data: dadosEnvio,
        dataType: 'json'
    })
            .done((listaLotes) => {         
                console.log("Gerar Template Resumo Lotes");
                jsCriaTemplateResumoLotes(listaLotes);
            });
}

function jsCriaTemplateLotes(listaLotes)
{
    novaListaLotes = {};
    novaListaLotes.DATA = [];
//    dados = {};
//    dados.referencias = listaLotes[0].referencias;
//    dados.casenum = "Nome Processo";
//    novaListaLotes.DATA.push(dados);
//    dados.referencias = listaLotes[1].referencias;
//    dados.casenum = "Nome Processo2";    
//    novaListaLotes.DATA.push(dados);

    novaListaLotes.DATA = listaLotes;
    t_body.parseJSON(novaListaLotes);
    var parsed = t_body.parse();
    $("#divListaLotes").html(parsed);
    $(".loading").hide();
}


function jsCarregaListaLotes(filter)
{
    endApiLotes = 'queuelist/GATEWAY_DIGITAL/';
    $(".loading").show();
    // Cria o Filtro se não houver filtros selecionados
    if (typeof filter === "undefined" | filter === false)
    {
        filter = {"columns":
                    {
                        "filter":
                                {
                                    "campo": "29",
                                    "valor": "1",
                                    "tipo": "BO"
                                }
                    }
        };
    }
    dadosEnvio = {
        "where": filter
    };
    $.ajax({
        url: endApiLotes,
        type: "POST",
        data: dadosEnvio,
        dataType: 'json'
    })
            .done((listaLotes) => {
                jsCriaTemplateLotes(listaLotes);
            });
}

function jsInicializacaoPagina()
{
    jsCarregaListaLotes();
//    jsCarregaListaResumoLotes();
}

$(document).ready(function () {
    jsInicializacaoPagina();
});


function jsRegistrosQueueFilter()
{
    var aFiltros = {};
    clearFilter = false;
    aFiltros.campos = [];

    /**
     *  Campo Cliente
     */
    if ($("#t20").val() !== "")
    {
        tipoCampo = 'TX';
        idCampo = 20;
        nomeCampo = 'Cliente';
        valorCampo = $("#t20").val();
        aFiltros = jsAddFiltro(aFiltros, tipoCampo, "t" + idCampo, null, idCampo, nomeCampo);
    }

    /**
     *  Data
     */
    if ($("#dataInicio").val() !== "")
    {
        
        tipoCampo = 'DT';
        idCampo = 14;
        nomeCampo = 'filtro Data';
        valorCampo = $("#dataInicio").val();
        if ($("#dataFim").val() !== "")
        {
            aFiltros = jsAddFiltro(aFiltros, tipoCampo, "dataInicio", "dataFim", idCampo, nomeCampo);
        } else {
            aFiltros = jsAddFiltro(aFiltros, tipoCampo, "dataInicio", null, idCampo, nomeCampo);
        }
        
    }

    /**
     *  Campo Praça
     */
    if ($("#praca").val() !== "")
    {
        tipoCampo = 'TX';
        idCampo = 13;
        nomeCampo = 'praca';
        valorCampo = $("#praca").val();
        aFiltros = jsAddFiltro(aFiltros, tipoCampo, "praca", null, idCampo, nomeCampo);
    }

    /**
     *  Campo Status
     */
    if ($("#status").val() !== "")
    {
        tipoCampo = 'TX';
        idCampo = 10;
        nomeCampo = 'status';
        valorCampo = $("#status").val();
        aFiltros = jsAddFiltro(aFiltros, tipoCampo, "status", null, idCampo, nomeCampo);
    }


    aWhere = jsCriaFiltros(aFiltros, false); ///< Cria o filtro pelos campos
    jsCarregaListaLotes(aWhere);
}