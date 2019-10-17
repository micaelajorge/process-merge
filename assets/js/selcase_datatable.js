/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 03/06/2018
 Sistema: Creditus
 */

colunas = [
    "casenum",
    "fieldvalue"
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

    // Seta a Ordenação pela coluna DATAHORA

    tableInit(tabelaDados, endApi, colunas, columnDefs, aWhere, sort); ///< Inicializa a Tabela como DataTable        
}


/**
 * Definição da apresentação dos campos
 * @type Array
 */


var sort = [0, "desc"];

var columnDefs = [

    {
        "targets": 0,
        "render": jsTrataAcoes
    }
];

function jsTrataFiltroPasso(data, type, row, meta)
{
    var fieldValue;
    fieldValue = '<a href="javascript:" onclick="" title="Filtrar">' + row.procname + '</a> / <a href="" onclick="" title="Filtrar">' + row.stepname + '</a>';
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
    acessoCaso = '<strong>' + data + '</strong>' + ' <a style="float:right" onclick="JSselecionarDoc(\'' + row.casenum + '\',\'' + row.fieldvalue + '\')" title="Selecionar" class="primary"><i class="fa fa-check"></i></a>';
    fieldValue = acessoCaso;
    return fieldValue;
}

function jsAcionaDataTableSelCaso(ProcId, FieldSource, fieldid)
{
    endApi = SITE_LOCATION + "apicaselist";
    tabelaDados = 'tableDataSelCaso' + fieldid;

    where = {global: true, ProcSource: ProcId, FieldSource: FieldSource};

    extraPameters = {"scrollX": true, "pageLength": 5};

    tableInit(tabelaDados, endApi, colunas, columnDefs, where, sort, null, null, extraPameters);

}

function jsTrataReferencias(data, type, row, meta)
{
    var fieldValue = "";
    var separador = "";
    if (data !== undefined)
    {
        for (i = 0; i < data.length; i++)
        {
            fieldValue = fieldValue + separador + data[i].FieldName + ': <b>' + data[i].FieldValue + "</b>";
            var separador = ", ";
        }
    }
    return fieldValue;
}
