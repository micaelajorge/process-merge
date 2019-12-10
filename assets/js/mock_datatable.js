/*
 Criação:
 Data Criação 
 Sistema: Process
 */


if (typeof sort === "undefined") 
{
    var sort = [0, "asc"];
}
var tabelaDados = 'tabelaResultadoConsulta';
//<editor-fold defaultstate="collapsed" desc="Dados Criacao Datatable Normal">
var colunas_Default = [
    {"data": "#"},
    {"data": ""},
    {"data": "Pedido"},
    {"data": "DataCadastro"},
    {"data": "Status"},
    {"data": "Natureza"},
    {"data": "Tipo"},
    {"data": "Solicitante"},
    {"data": "Notificacao"}
];

var columnsDefs_Default = [
    {
        "targets": 1,
        "render": jsTrataAcoes
    },
    {
        "targets": [0, 1, 2, 3, 4],
        "orderable": false
    }
];

extraPameters_Default = {
    "scrollY": ($(window).height() < 700) ? "400px" : "700px",
    "scrollXInner": true
};

extraPameters_less500_Default = {
    "scrollY": "400px",
    "oPaginate": {
        "sNext": ">",
        "sPrevious": "<",
        "sFirst": "Prim",
        "sLast": "Últ"
    }
};

var colunas;
var columnDefs;

colunas = colunas_Default;
columnDefs = columnsDefs_Default;
extraPameters = extraPameters_Default;
extraPameters_less500 = extraPameters_less500_Default;


/**
 * Definição da apresentação dos campos
 * @type Array
 */

/**
 * Trata o render do Campo Código
 * @param {type} data
 * @param {type} type
 * @param {type} row
 * @param {type} meta
 * @returns {fieldValue}
 */

/**
 * Trata o render do Campo Nivel
 * @param {type} data
 * @param {type} type
 * @param {type} row
 * @param {type} meta
 * @returns {fieldValue}
 */


function jsTrataAcoes(data, type, row, meta)
{
    acessoCaso = '<a href="pedidos_detalhe" class="btn btn-primary btn-xs" role="button">Detalhe</a href>';
        
    fieldValue = acessoCaso;
    return fieldValue;
}

function jsShowDivDataTableWrapper(dataTable)
{
    jsCalculaTamanhoBodyScrollDatatable();
    $('div.loading').css("display", "none");
    if (dataTableResize)
    {
        $("#" + tabelaDados).parents(".row").height($(".content-wrapper").height() - offSetTamanhoPagina);
    }
    $("#box-content-datatable").removeClass("hidden");
    $("#divDataTableWrapper").removeClass("hidden");
    if (funcAfterLoadDataTable !== null)
    {
        funcAfterLoadDataTable(dataTable);
    }
}

function jsInicializacaoPagina()
{
    endApi = SITE_LOCATION + "mockcreditas";
    tabelaDados = 'tabelaResultadoConsulta';
    where = null;
    // Se a página form menor 500 modifica a dataTable
    if ($(window).width() < 500) {
    }
    $("#divMedidas").html($(window).width() + " ");
    tableInit(tabelaDados, endApi, colunas_Default, columnsDefs_Default, null, 1, null, null, extraPameters_Default);//(tabelaId, url, colunas, columnDefs, where, sort, createdRow, rowId, _extraParameters) 
}

$(document).ready(function () {
    jsInicializacaoPagina();
    $('#pedidos').removeClass('hidden');
});
