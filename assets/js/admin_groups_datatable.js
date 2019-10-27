/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 03/06/2018
 Sistema: Process
 */

var colunas = [
    {"data": "groupid"},
    {"data": null},
    {"data": "groupname"},
    {"data": "groupdesc"}
];

var tabelaDados = null;
var endApi = "";

var datatableUsuarios;

function jsFilter()
{
    var aFiltros = {}; ///< Objeto para criação dos Filtros para o DataTable
    aFiltros.campos = [];
    aFiltros = jsAddFiltro(aFiltros, 'DATA', 'selDataEventoInicio', 'selDataEventoFim', 'auditoria_datahora', 'Data hora do Evento');
    aFiltros = jsAddFiltro(aFiltros, 'TEXT', 'selDescricao', '', 'auditoria_descricao', 'Descrição Evento');
    table = $("#" + tabelaDados).DataTable(); ///< Pega a Tabela atual
    table.destroy();
    aWhere = jsCriaFiltros(aFiltros); ///< Cria o filtro pelos campos


    tableInit(tabelaDados, endApi, colunas, columnDefs, aWhere, sort, null, null, ); ///< Inicializa a Tabela como DataTable        
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


var columnDefs = [
    {
        "targets": 1,
        "render": jsTrataAcoes
    },
    {
        "targets": [0, 1, 2, 3],
        "orderable": false
    }
];

function jsTrataAcoes(data, type, row, meta)
{
    edicao = '<button type="button" class="btn btn-primary btn-xs" onclick="jsOpenEditGroup(\'' + row.groupid + '\')"><i class=\"fa fa-edit\" title="Editar dados do Grupo"></i></button> ';

    gruposDoUsuario = '<button type="button" class="btn btn-xs btn-warning"><i class=\"fa fa-users\" title="Editar Usuários do Grupo"></i></button> ';
    
    fieldValue = edicao + gruposDoUsuario;
    return fieldValue;
}

function jsInicializacaoPagina()
{
    endApi = SITE_LOCATION + "api/manager/groups";
    tabelaDados = 'tableData';
    where = null;
    sort = [2, 'asc']; // Seta a Coluna de Nome para ordenação
    allowSearch = true; // Seta que vai poder fazer Pesquisa

    extraPameters = {
        "scrollX": true,
        "searching": allowSearch
    };
        
    // Torna o menu de oções aberto
    if ($(window).width() > 1400)
    {
        $("body").removeClass("sidebar-collapse");
    }
        
        
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
    //extraPameters = Object.assign(extraPameters, mobileParameters);
    
    $("#divMedidas").html($(window).width() + " ");
    datatableUsuarios = tableInit(tabelaDados, endApi, colunas, columnDefs, where, sort, null, 'groupid', extraPameters);
}

$(document).ready(function () {
    jsInicializacaoPagina();
});