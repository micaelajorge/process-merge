/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 01/07/2019
 Sistema: Process_XAMPP
 */

function jsCriaTemplateArquivosOferta(listArquivos, divDestino, templateView, proximaFuncao)
{
    let t_body = new Template(templateView);
    novaListaLotes = {};
    novaListaLotes.DATA = [];

    novaListaLotes.DATA = listArquivos;
    t_body.parseJSON(novaListaLotes);
    var parsed = t_body.parse();
    $("#" + divDestino).html(parsed);
    $("#loading_" + divDestino).hide();
    if (proximaFuncao !== undefined)
    {
        proximaFuncao();
    }
}

function jsCarregaListaArquivosOferta()
{
    let divDestino = 'divArquivosOferta';
    let templateView = 'assets/templates/t_gateway_lista_arquivos_oferta.html';

    endApiLotes = 'queuelist/SEC_CNABS/';
    $("#loading_" + divDestino).show();

    let filters = {"columns":
                [
                    {
                        "valor": "6",
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
            .done((listArquivos) => {
                jsCriaTemplateArquivosOferta(listArquivos, divDestino, templateView);
            });
}

function jsCarregaListaArquivosRecusados()
{
    let divDestino = 'divArquivosRecusados';

    // Testa se existe a div de Arquivos Recusados
    if (!!$("#divArquivosRecusados").length)
    {
        return;
    }


    let templateView = 'assets/templates/t_gateway_lista_arquivos_retorno.html';

    endApiLotes = 'queuelist/ENVIO_CNAB_ACEITE/';
    $("#loading_" + divDestino).show();

    let filters = {"columns":
                [
                    {
                        "valor": "1",
                        "tipo": "LT",
                        "campo": "15"
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
            .done((listArquivos) => {
                jsCriaTemplateArquivosOferta(listArquivos, divDestino, templateView);
            });
}

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

function jsSecuritiesArquivoEnviado(param1, param2)
{
    $(".loading").show();
    jsCarregaListaArquivosOferta();
    alert("Arquivo enviado com sucesso");
}

function jsSecuritiesErroUpload(param1, param2)
{
    retorno = JSON.parse(param1.response);
    alert("Erro no envio do arquivo \n" + retorno.Error);
}

async function jsInicializacaoPagina()
{
    jsCarregaListaArquivosOferta();
}

$(document).ready(function () {
    jsInicializacaoPagina();
});


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