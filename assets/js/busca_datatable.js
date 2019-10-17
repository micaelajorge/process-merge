/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 03/06/2018
 Sistema: Process
 */

colunas = [
    {"data": "casenum"},
    {"data": "procid", className: "hidden-xs"},
    {"data": "procid", className: "hidden-xs"},
    {"data": "procname", className: "hidden-xs"},
    {"data": "referencias", className: "hidden-xs"}
];

endApi = SITE_LOCATION + "api/busca_resultado";

extraPameters = {"scrollX": true};

function jsFilter(procId, termoPesquisa)
{
    var aFiltros = {}; ///< Objeto para criação dos Filtros para o DataTable
    aFiltros.campos = [];
//    aWhere = [];
//    aWhere.push("procId = " + procId + " and fieldValue like '%" + termoPesquisa + "%'");

    
    aWhere = {" procId = " : procId, " fieldValue like " : "%" + termoPesquisa + "%"};
//    aWhere = "procId = " + procId + " and fieldValue like '%" + termoPesquisa + "%'";
    table = $("#" + tabelaDados).DataTable(); ///< Pega a Tabela atual
    table.destroy();    
    _extraParameters = {"searching" : false }; 
    tableInit(tabelaDados, endApi, colunas, columnDefs, aWhere, sort, null, null, _extraParameters); ///< Inicializa a Tabela como DataTable        
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


var sort = [0, "desc"];
var columnDefs = [

    {
        "targets": 2,
        "render": jsTrataStatusCaso,
        "width" : "25px"
    },
    {
        "targets": 3,
        "render": jsTrataFiltroPasso,
        "width" : "100px"
    },
    {
        "orderable": false, "targets": [1, 2]
    },
    {
        "targets": 1,
        "render": jsTrataAcoes,
        "width" : "50px"
    },
    {
        "targets": 4,
        "render": jsTrataReferencias
    },
    {
        "targets": 0,
        "render": jsTrataCaseNum,
        "width" : "50px"
    }
];
function jsTrataCaseNum(data, type, row, meta)
{
    var fieldValue = row.casenum;
    // Se estiver em modo Mobile, muda
    if ($(window).width() < 500)
    {
        fieldValue = "#<b>" + fieldValue + "</b> ";
        fieldValue = fieldValue + "<small>" + jsTrataReferencias(data, type, row, meta, 2) + "</small>";
        fieldValue = "<a  onclick=\"" + row.ACESSOCASO + "\">" + fieldValue + "</a>";
    }
    return fieldValue;
}

function jsTrataFiltroPasso(data, type, row, meta)
{
    var fieldValue;
    //fieldValue = '<a href="javascript:" onclick="" title="Filtrar">' + row.procname + '</a> / <a href="" onclick="" title="Filtrar">' + row.stepname + '</a>';
    //fieldValue = '<a href="" onclick="" title="Filtrar">' + row.stepname + '</a>';
    fieldValue = row.stepname;
    return fieldValue;
}

function jsTrataStatusCaso(data, type, row, meta)
{
    var fieldValue = "";
    if (row.DEADTIME === 1)
    {
        fieldValue = '<i class="fa fa-clock-o text-danger" title="SLA Excedido"></i> ';
    }
    if (row.lock.locked === 1)
    {
        fieldValue = fieldValue + '<i class="fa fa-user text-danger" title="' + row.lock.text + '"></i> ';
    }
    return fieldValue;
}

function jsTrataAcoes(data, type, row, meta)
{
    if (row.lock.locked > 0)
    {
        acessoCaso = '<a title="' + row.lock.text + '" class="text-danger"><i class="fa fa-lock"></i></a>';
    } else {
        acessoCaso = '<a onclick="' + row.ACESSOCASO + '" title="Abrir ' + row.instancename + '" class="primary"><i class="fa fa-edit"></i></a>';
    }
    fieldValue = acessoCaso;
    return fieldValue;
}

function jsTrataReferencias(data, type, row, meta, linhas) {
    var fieldValue = "";
    var separador = "";
    dadosColuna = row.referencias;
    fieldValue = "<div class=\"visible-xs\">Tarefa: <b>" + row.stepname + "</b></div>";
    if (linhas === undefined)
    {
        linhas = 10;
    }

    if (dadosColuna !== undefined & dadosColuna !== null)
    {
        for (i = 0; i < dadosColuna.length; i++)
        {
            fieldValue = fieldValue + separador + dadosColuna[i].FieldName + ': <b>' + dadosColuna[i].FieldValue + "</b>";
            var separador = ", <span class=\"visible-xs\"></span>";
            //var separador = ", ";
            if (i >= linhas)
            {
                break;
            }
        }
    }

    return fieldValue;
}

function jsInicializacaoPagina()
{    
    tabelaDados = 'tableData';
    where = null;
//    if (ProcId !== undefined)
//    {
//        where = {global: true, ProcId: ProcId};
//    }

    // Se a página form menor 500 modifica a dataTable
    if ($(window).width() < 500) {
        extraPameters = {
            "scrollX": true,
            "pageLength": 5,
            "oPaginate": {
                "sNext": ">",
                "sPrevious": "<",
                "sFirst": "Prim",
                "sLast": "Últ"
            }
        };
    }
    $("#divMedidas").html($(window).width() + " ");

}

$(document).ready(function () {
    jsInicializacaoPagina();
});