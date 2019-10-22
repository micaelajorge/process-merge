/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 03/06/2018
 Sistema: Process
 */


if (typeof sort === "undefined") 
{
    var sort = [0, "asc"];
}


//<editor-fold defaultstate="collapsed" desc="Dados Criacao Datatable Normal">
var colunas_Default = [
    {"data": "casenum"},
    {"data": "procid", className: "hidden-xs"},
    {"data": "procid", className: "hidden"},
    {"data": "stepname", className: "hidden-xs"},
    {"data": "referencias" , className: "hidden-xs"}
];

var columnsDefs_Default = [
    {
        "targets": 2,
        "render": jsTrataStatusCaso,
        "orderable": false
    },
    {
        "targets": 3,
        "render": jsTrataFiltroPasso,
        "orderable": false
    },
    {
        "targets": 1,
        "render": jsTrataAcoes,
        "orderable": false
    },
    {
        "targets": 4,
        "render": jsTrataReferencias,
        "orderable": false
    },
    {
        "targets": 0,
        "render": jsTrataCaseNum
    }
];

/*
 var columnsDefs_Default = [
 {
 "targets": 2,
 "render": jsTrataStatusCaso,
 "width": "25px",
 "orderable": false
 },
 {
 "targets": 3,
 "render": jsTrataFiltroPasso,
 "width": "100px",
 "orderable": false
 },
 {
 "targets": 1,
 "render": jsTrataAcoes,
 "width": "50px",
 "orderable": false
 },
 {
 "targets": 4,
 "render": jsTrataReferencias,
 "orderable": false
 },
 {
 "targets": 0,
 "render": jsTrataCaseNum,
 "width": "50px"
 },
 {
 "targets": 5,
 "render": jsTrataReferenciasShort,
 "orderable": false
 }
 ];
 */

colunas_CT = [
    {"data": "casenum"},
    {"data": "procid", className: "hidden-xs"},
    {"data": "procid", className: "hidden-xs"},
    {"data": "stepname", className: "hidden-xs"},
    {"data": "referencias", className: "hidden-xs"}
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

//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Dados Criacao Datatable CT">
var columnsDefs_CT = [
    {
        "targets": 0,
        "render": jsTrataCaseNum,
        "width": "50px"
    },
    {
        "targets": 1,
        "render": jsTrataAcoes,
        "width": "50px",
        "orderable": false
    },
    {
        "targets": 2,
        "render": jsTrataProcessoCaso,
        "width": "25px",
        "orderable": true
    },
    {
        "targets": 3,
        "render": jsTrataStatusCaso,
        "width": "25px",
        "orderable": true
    },
    {
        "targets": 4,
        "render": jsTrataReferencias,
        "orderable": false
    }
];

extraPameters_CT = {
    "scrollX": false,
    "scrollXInner": false
};

extraPameters_less500_CT = {
    "scrollX": false,
    "pageLength": 5,
    "oPaginate": {
        "sNext": ">",
        "sPrevious": "<",
        "sFirst": "Prim",
        "sLast": "Últ"
    }
};

//</editor-fold>



var colunas;
var columnDefs;

switch (tipoProcesso)
{
    case "CT":
        colunas = colunas_CT;
        columnDefs = columnsDefs_CT;
        extraPameters = extraPameters_CT;
        extraPameters_less500 = extraPameters_less500_CT;
        break;
    default:
        colunas = colunas_Default;
        columnDefs = columnsDefs_Default;
        extraPameters = extraPameters_Default;
        extraPameters_less500 = extraPameters_less500_Default;
        break;
}

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

function jsAtualizaNavFilter(aFiltros)
{
    if (aFiltros.length > 0)
    {
        $("#extendedNav").find("a").attr('href', "javascript:jsQueueFilter()");
        $("#extendedNav").removeClass("hidden");
        $("#extendedNav").find("i").addClass("fa fa-filter");
    }
    var concatenador = "";
    var textoNav = "";
    aFiltros.forEach(function (item, indice) {
        $("#extendedNav").removeClass("hidden");
        textoNav = concatenador + " " + item.txtMensagem + " = " + $("#" + item.obj1).val();
        concatenador = " e ";
    });
    $("#extendedNav").find("span").html(textoNav);
}

function jsLimparFiltros()
{
    $("#frmQueueFilter").find("input").each(function (indice, item) {
        $(item).val("");
    });
    jsQueueFilter(true);
}

function jsQueueFilter(clearFilter)
{
    var aFiltros = {};
    aFiltros.campos = [];
    $("#extendedNav").addClass("hidden");
    $("#frmQueueFilter").find("input, select").each(function (indice, item)
    {
        tipoCampo = $(item).attr("aria-field-type");
        idCampo = $(item).attr("aria-field-id");
        nomeCampo = $(item).attr("aria-field-name");
        valorCampo = $(item).val();
        if (valorCampo !== "")
        {
            aFiltros = jsAddFiltro(aFiltros, tipoCampo, "Campo" + idCampo, null, idCampo, nomeCampo);
        }
    });
    table = $("#" + tabelaDados).DataTable(); ///< Pega a Tabela atual
    table.destroy();
    jsAtualizaNavFilter(aFiltros.campos);
    if (clearFilter)
    {
        aWhere = "clearFilter";
    } else {
        aWhere = jsCriaFiltros(aFiltros, false); ///< Cria o filtro pelos campos
    }


    tableInit(tabelaDados, endApi, colunas, columnDefs, aWhere, sort); ///< Inicializa a Tabela como DataTable        
    $("#crModalQueueFilter").modal("hide");
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

function jsTrataProcessoCaso(data, type, row, meta)
{
    var fieldValue = "";
    if (row.referencias === null)
    {
        return "";
    }
    for (i = 0; i < row.referencias.length; i++) {
        if (row.referencias[i].FieldId === "5")
        {
            fieldValue = row.referencias[i].FieldValue;
            break;
        }
    }
    return fieldValue;
}

function jsTrataStatusCaso(data, type, row, meta)
{
    var fieldValue = "";
    if (row.referencias === null)
    {
        return "";
    }
    for (i = 0; i < row.referencias.length; i++) {
        if (row.referencias[i].FieldId === "6")
        {
            fieldValue = row.referencias[i].FieldValue;
            break;
        }
    }
    return fieldValue;
}

function jsTrataAcoes(data, type, row, meta)
{
    if (row.lock.locked > 0)
    {
        acessoCaso = '<a href="javascript:" title="' + row.lock.text + '" class="text-danger"><i class="fa fa-lock"></i></a>';
    } else {
        acessoCaso = '<a href="javascript:" onclick="' + row.ACESSOCASO + '" title="Editar ' + row.instancename + '" class="primary"><i class="fa fa-edit"></i></a>';
    }
    fieldValue = acessoCaso;
    return fieldValue;
}

function jsTrataReferenciasShort(data, type, row, meta, linhas) {
    var fieldValue = "";
    var separador = "";
    dadosColuna = row.referencias;
    fieldValue = "<div class=\"visible-xs\">Tarefa: <b>" + row.stepname + "</b></div>";
    if (linhas === undefined)
    {
        linhas = 10;
    }

    //fieldValue = fieldValue + "Short ";
    if (dadosColuna !== null & dadosColuna.length > 0)
    {
        for (i = 0; (i < 2 & i < dadosColuna.length); i++)
        {
            if (tipoProcesso === "CT")
            {
                if (dadosColuna[i].FieldId === "5" | dadosColuna[i].FieldId === "6")
                {
                    continue;
                }
            }
            classNameVisualizarReferencia = "visible-inline-xs visible-inline-md";
            if (i > 1)
            {
                classNameVisualizarReferencia = "hidden-xs hidden-sm hidden-md hidden-lg";
            }
            fieldValue = fieldValue + separador + dadosColuna[i].FieldName + ': <b>' + dadosColuna[i].FieldValue + "</b>";
            var separador = ", <span class=\"" + classNameVisualizarReferencia + "\"></span>";
            //var separador = ", ";
            if (i >= linhas)
            {
                break;
            }
        }
    }
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

    //fieldValue = fieldValue + "Ref "; 
    if (dadosColuna !== null)
    {
        for (i = 0; i < dadosColuna.length; i++)
        {
            if (tipoProcesso === "CT")
            {
                if (dadosColuna[i].FieldId === "5" | dadosColuna[i].FieldId === "6")
                {
                    continue;
                }
            }
            classNameVisualizarReferencia = "visible-inline-xs visible-inline-md visible-inline-lg";
            if (i > 1)
            {
                classNameVisualizarReferencia = "hidden-xs hidden-md";
            }
            fieldValue = fieldValue + "<span class=\"" + classNameVisualizarReferencia + "\">" + separador + dadosColuna[i].FieldName + ': <b>' + dadosColuna[i].FieldValue + "</b></span>";
            //fieldValue = fieldValue + separador + dadosColuna[i].FieldName + ': <b>' + dadosColuna[i].FieldValue + "</b>";
            var separador = ", ";
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
    endApi = SITE_LOCATION + "queuelist" + "/" + ProcId + "/" + StepId;
    tabelaDados = 'tableData';
    where = null;
//    if (ProcId !== undefined)
//    {
//        where = {global: true, ProcId: ProcId};
//    }

    // Se a página form menor 500 modifica a dataTable
    if ($(window).width() < 500) {
    }
    $("#divMedidas").html($(window).width() + " ");
    tableInit(tabelaDados, endApi, colunas, columnDefs, where, sort, 0, 'casenum', extraPameters);
}

$(document).ready(function () {
    jsInicializacaoPagina();
});