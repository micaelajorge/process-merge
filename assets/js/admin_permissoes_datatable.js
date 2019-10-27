/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 03/06/2018
 Sistema: Process
 */

var colunas = [
    {"data": "procid"},
    {"data": "procname"},
    {"data": "procdesc"},
    {"data": "proccod"}
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

var columnDefs = [
    {
        "targets": 1,
        "render": jsTrataAcoes
    }
];
 

function jsTrataAcoes(data, type, row, meta)
{
    switch (row.tipoproc)
    {
        case "WF":
            tipoBotao = "btn-primary";
            iconeProc = "fa-gears";
            break;
        case "RP":
            tipoBotao = "btn-success";
            iconeProc = "fa-files-o";
            break;
        case "CT":
            tipoBotao = "btn-warning";
            iconeProc = "fa-tags";
            break;
        default:
            tipoBotao = "btn-default";
            iconeProc = "fa-ban";
            break;            
    }
    
    edicao = '<button type="button" class="btn  btn-xs ' + tipoBotao + '" onclick="jsEditPermissaoProc(\'' + row.procid + '\')" title="Editar Permissões da Solução"><i class=\"fa ' + iconeProc + '\"></i> ' + data + '</button> ';      
    relatorio = '<button type="button" class="btn btn-default btn-xs" onclick="jsRelatorioPermissoes(\'' + row.procid + '\')" title="Lista de Permissões do Solução"><i class=\"fa fa-list\" ></i></button> ';      
    fieldValue = relatorio + ' ' + edicao;
    return fieldValue;
}

function jsInicializacaoPagina()
{
    endApi = SITE_LOCATION + "api/permissoes/procs";
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