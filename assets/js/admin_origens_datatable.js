/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 03/06/2018
 Sistema: Process
 */

colunas = [
    {"data": "ORIGEM_USER"},
    {"data": "ORIGEM_USER"},
    {"data": "ORIGEM_DOC"}
];

var tabelaDados = null;
var endApi = "";


function jsFilter()
{
    var aFiltros = {}; ///< Objeto para criação dos Filtros para o DataTable
    aFiltros.campos = [];
    aFiltros = jsAddFiltro(aFiltros, 'DATA', 'selDataEventoInicio', 'selDataEventoFim', 'auditoria_datahora', 'Data hora do Evento');
    aFiltros = jsAddFiltro(aFiltros, 'TEXT', 'selDescricao', '', 'auditoria_descricao', 'Descrição Evento');
    table = $("#" + tabelaDados).DataTable(); ///< Pega a Tabela atual
    table.destroy();
    aWhere = jsCriaFiltros(aFiltros); ///< Cria o filtro pelos campos


    tableInit(tabelaDados, endApi, colunas, columnDefs, aWhere, sort); ///< Inicializa a Tabela como DataTable        
}


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
function jsTrataRenderCodigo(data, type, row, meta)
{
    fieldValue = jCodigoAuditoria[data];
    return fieldValue;
}

/**
 * Trata o render do Campo Nivel
 * @param {type} data
 * @param {type} type
 * @param {type} row
 * @param {type} meta
 * @returns {fieldValue}
 */
function jsTrataRenderNivel(data, type, row, meta)
{
    fieldValue = jNivelAuditoria[data];
    return fieldValue;
}



function jsTrataAcoes(data, type, row, meta)
{
    edicao = '<button type="button" class="btn btn-warning btn-xs"><i class=\"fa fa-trash\" title="Apagar Origem"></i></button> ';
    fieldValue = edicao;
    return fieldValue;
}

var columnDefs = [
    {
        "targets": 0,
        "render": jsTrataAcoes
    },
    {
        "targets": [0, 2],
        "orderable": false
    }    
];



function jsInicializacaoPagina()
{
    endApi = SITE_LOCATION + "api/manager/origens";
    tabelaDados = 'tableData';
    where = null;

    allowSearch = true; // Seta que vai poder fazer Pesquisa

    extraPameters = {
        "scrollX": true,
        "searching": allowSearch
    };

    mobileParameters = {};
    if ($(window).width() < 500) {
        mobileParameters = {
            "pageLength": ($(window).width() < 500) ? minPageNumRows : maxPageNumRows,
            "oPaginate": {
                "sNext": ">",
                "sPrevious": "<",
                "sFirst": "Prim",
                "sLast": "Últ"
            }
        };
    }
    extraPameters = Object.assign(extraPameters, mobileParameters);
    $("#divMedidas").html($(window).width() + " ");
    sort = [1, 'asc']; // Habilita o sort pela primeira coluna
    tableInit(tabelaDados, endApi, colunas, columnDefs, where, sort, null, null, extraPameters);
}

$(document).ready(function () {
    jsInicializacaoPagina();
});