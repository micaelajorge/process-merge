/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 01/07/2019
 Sistema: Process_XAMPP
 */



/**
 * 
 * @param {type} listaLotes
 * @param {type} divDestino
 * @param {type} proximaFuncao
 * @returns {undefined}
 */
function jsCriaTemplateLotes(listaLotes, divDestino, proximaFuncao)
{
    let t_body = new Template('assets/templates/gateway_registros/t_gateway_lista_arquivos_retorno.html');
    novaListaLotes = {};
    novaListaLotes.DATA = [];

    novaListaLotes.DATA = listaLotes;


    t_body.parseJSON(novaListaLotes);
    var parsed = t_body.parse();
    $("#" + divDestino).html(parsed);
    $("#loading_" + divDestino).hide();
    if (proximaFuncao !== undefined)
    {
        proximaFuncao();
    }
}


function jsCarregaListaLotesRetorno()
{
    endApiLotes = 'queuelist/SEC_CNABS/';
    $("#loading_divArquivosRetorno").show();

    filters = {"columns":
                [
                    {
                        "valor": "2",
                        "tipo": "LT",
                        "campo": "21"
                    }
                ]
    };


    dadosEnvio = {
        "where": filters,
        "order": [
            {
                "column": "0",
                "data": "CaseNum",
                "dir": "desc"
            }
        ],
        "columns": [
            {
                "data": "CaseNum"
            }
        ]
    };
    $.ajax({
        url: endApiLotes,
        type: "POST",
        data: dadosEnvio,
        dataType: 'json'
    })
            .done((listaLotes) => {

                //Inclui a origem dos dados para setar a URL de download do arquivo
                jsCriaTemplateLotes(listaLotes, "divArquivosRetorno");
            });
}


function jsCarregaListaLotesOferta()
{
    endApiLotes = 'queuelist/SEC_CNABS/';
    $("#loading_divArquivosOferta").show();

    filters = {"columns":
                [
                    {
                        "valor": "1",
                        "tipo": "LT",
                        "campo": "21"
                    }
                ]
    };

    dadosEnvio = {
        "where": filters,
        "order": [
            {
                "column": "0",
                "data": "CaseNum",
                "dir": "desc"
            }
        ],
        "columns": [
            {
                "data": "CaseNum"
            }
        ]
    };
    $.ajax({
        url: endApiLotes,
        type: "POST",
        data: dadosEnvio,
        dataType: 'json'
    })
            .done((listaLotes) => {
                //Inclui a origem dos dados para setar a URL de download do arquivo
                jsCriaTemplateLotes(listaLotes, "divArquivosOferta", jsCarregaListaLotesRetorno);
            });
}

function jsInicializacaoPagina()
{
    jsCarregaListaLotesOferta();    
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
    jsCarregaListaLotesOferta(aWhere);
}

function jsSecuritiesArquivoEnviado(param1, param2)
{
    jsCarregaListaLotesRetorno();
    alert("Arquivo enviado com sucesso");
}

function jsSecuritiesErroUpload(param1, param2)
{
    retorno = JSON.parse(param1.response);
    alert("Erro no envio do arquivo \n" + retorno.Error);
}

function jsFidicSelecionado()
{
    let fidicDestino = $("#targetFIDC").val();
    if (fidicDestino !== "") {
        createDropZone(fidicDestino);
        $("#selFileUpLoad").show();        
    } else {
        $("#selFileUpLoad").hide();
    }
}