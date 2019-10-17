/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 03/06/2018
 Sistema: Process
 */

var colunas = [
    {"data": "stepid"},
    {"data": "stepname"},
    {"data": "permissoes"}
];


var tabelaDados = null;
var endApi = "";

var datatablePermissoes;

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
        "targets": 2,
        "render": jsTrataPermissoes
    },
    {
        "targets": 1,
        "render": jsTrataNomePasso
    },
    {
        "targets": "_all",
        "orderable": false
    }

];

function jsTrataNomePasso(data, type, row, meta) {
    fieldValue = data;
    switch (row.steptype)
    {
        case "P":
            iconeStep = "fa-gears";
            corStep = "text-primary";
            preTexto = "";
            titulo = "Processo - ";
            break;
        case "T":
            iconeStep = "fa-tag";
            corStep = "text-green";
            preTexto = "";
            titulo = "Status - ";
            break;
        case "D":
            iconeStep = "fa-files-o";
            corStep = "text-green";
            preTexto = "";
            titulo = "Documento - ";
            break;
        case "R":
            iconeStep = "fa-file-text-o";
            corStep = "text-warning";
            preTexto = "";
            titulo = "Relatório - ";
            break;            
        case "S":
            iconeStep = "fa-gears";
            corStep = "text-primary";
            titulo = "Tarefa - ";
            preTexto = "";
            break;
        case "F":
            iconeStep = "fa-eye";
            corStep = "text-primary";
            titulo = "Formalizacao - ";
            preTexto = "";
            break;
        default:
            iconeStep = "fa-gears";
            corStep = "text-primary";
            titulo = row.steptype + " - ";
            preTexto = "";
    }
    fieldValue = "<span title=\"" + titulo + data + "\" class=\"" + corStep + "\"><i class=\"fa " + iconeStep + "\"></i> " + preTexto + data + "</span>";
    return fieldValue;
}

function jsTrataPermissoes(data, type, row, meta)
{
    botaoAdicionar = '<button type="button" class="btn btn-primary btn-xs" onclick="jsAdicionarPermissaoProc(' + row.procid + ', ' + row.stepid + ', \'' + row.stepname + '\')" title="Adicionar Usuário / Grupo"><i class=\"fa fa-plus\"></i></button> ';
    fieldValue = "";
    data.forEach((item, indice) => {
        acaoEdicao = "jsEditPermissoesEntidade(" + row.procid + "," + row.stepid + "," + item.groupid + ",'" + item.grpfld + "','" + item.entidade + "', '" + row.stepname + "')";
        switch (item.grpfld)
        {
            case "U":
                tipo = "Usuário";
                iconeUsuario = "<span class=\"text-orange\"><i class=\"fa fa-user\"></i> " + item.entidade + "</span>";
                break;
            case "G":
                tipo = "Grupo";
                iconeUsuario = "<span class=\"text-primary\"><i class=\"fa fa-users\"></i> " + item.entidade + "</span>";
                break;
            case "F":
                tipo = "Campo";
                iconeUsuario = "<span class=\"text-success\"><i class=\"fa fa-user-secret\"></i> " + item.entidade + "</span>";
                break;
        }
        botaoAcao = "<button type=\"button\" title=\"Editar permissões do " + tipo + " '" + item.entidade + "'\" class=\"btn btn-default btn-xs\" onclick=\"" + acaoEdicao + "\">";
        fieldValue += botaoAcao + iconeUsuario + "</button> ";
    });
    return botaoAdicionar + fieldValue;
}

function jsInicializacaoPagina()
{
    endApi = SITE_LOCATION + "api/permissoes/proc";
    tabelaDados = 'tableData';
    where = null;
    sort = [2, 'asc']; // Seta a Coluna de Nome para ordenação
    allowSearch = true; // Seta que vai poder fazer Pesquisa

    extraPameters = {
        "scrollX": true,
        "searching": false
    };

    mobileParameters = {};
    if ($(window).width() < 500) {
        mobileParameters = {
            "pageLength": ($(window).width() < 500) ? 5 : 10,
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
    datatablePermissoes = tableInit(tabelaDados, endApi, colunas, columnDefs, where, sort, null, 'groupid', extraPameters);
}

$(document).ready(function () {
    jsInicializacaoPagina();
});