/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 03/06/2018
 Sistema: Creditus
 */

///<editor-fold defaultstate="collapsed" desc="Definições do script">

var endApi = null;
var tabelaDados = null;

/**
 * Definição da apresentação dos campos
 * @type Array
 */
var columnDefs = [
    {"targets": 10,
        "className": "text-center",
        "render": jsTrataRenderCampoDataHora
    },
    {"targets": [6, 7],
        "className": "text-center",
        "render": jsTrataRenderCampoAtu
    },
    {"targets": [8, 9],
        "className": "text-center",
        "render": jsTrataRenderCampoSimNao
    },
    {"targets": [5],
        "className": "text-center",
        "render": jsTrataRenderIntervalo
    },
    {"targets": [1],
        "className": "text-center",
        "orderable": false,
        "render": jsTrataRenderEditarAutomato
    }
];

// Colunas Apresentadas no DataTable
colunas = [
    "auto_sequencial",
    "",
    "id_automato",
    "empresa",
    "ip",
    "periodo_atu",
    "last_atu",
    "next_atu",
    "ativo",
    "notificar",
    "data_registro"
];
//</editor-fold>

function jsGetInputValuesFromObjects(objeto)
{
    var dados = {};
    $(objeto).each(function (index, item) {
        if ($(item).attr("type") === "checkbox")
        {
            if ($(item).prop("checked"))
            {
                dados[$(item).attr("id")] = $(item).val();
            }
        } else {
            dados[$(item).attr("id")] = $(item).val();
        }

    });
    return dados;
}

function jsSalvarAutomato()
{
    var dados = jsGetInputValuesFromObjects("#divModalEditAutomato input");
    $.ajax({
        method: "POST",
        url: "api/automatos/alterarAutomato",
        data: dados
    }).done(function (data) {
        $("#modal-editAutomato").modal("hide");
        jsFilter();
    });
}

function jsVisualizarEditAutomato(data)
{
    $("#divModalEditAutomato").html(data);
    $("#modal-editAutomato").modal();
}

function jsAbreEditarAutomato(auto_sequencial)
{
    $.ajax({
        method: "POST",
        url: "automatos/editAutomato",
        data: {auto_sequencial: auto_sequencial}
    }).done(jsVisualizarEditAutomato);
}

///<editor-fold defaultstate="collapsed" desc="Funções Tratamento de Linha / Colunas DATATABLE">

/*
 * Trata a coluna Editar do Automato
 * @param {type} data
 * @param {type} type
 * @param {type} row
 * @param {type} meta
 * @returns {String}
 */
function jsTrataRenderEditarAutomato(data, type, row, meta)
{
    return '<il class="fa fa-edit" title="Editar Automato ' + row.id_automato + '" onclick="jsAbreEditarAutomato(' + row.auto_sequencial + ')"></il>';
}

function jsTracaCorTextoLinha(row, data, index)
{
    var dataLida = moment(data.last_atu);
    var diferenca = moment().diff(dataLida);
    var minutosDecorridos = parseInt(moment.duration(diferenca).asMinutes());
    var horasDecorridas = parseInt(moment.duration(diferenca).asHours());
    cor = "text-primary ";
    if (minutosDecorridos > 10)
    {
        cor = 'text-danger ';
    }
    //console.log(row);
    $(row).addClass(cor);
}

function jsTrataRenderCampoAtu(data, type, row, meta)
{
    var dataLida = moment(data);
    var diferenca = moment().diff(dataLida);
    cor = "text-primary";
    var minutosDecorridos = parseInt(moment.duration(diferenca).asMinutes());
    var horasDecorridas = parseInt(moment.duration(diferenca).asHours());
    var formato = "HH:mm:ss";
    if (minutosDecorridos > 10)
    {
        cor = "text-danger font-weight-bold";
        if (horasDecorridas > 10)
        {
            formato = "DD/MM/Y HH:mm:ss";
        }
    }
    fieldValue = '<span class="' + cor + '">' + dataLida.format(formato) + '</span> ';
    return fieldValue;
}
//</editor-fold>

///<editor-fold defaultstate="collapsed" desc="Tratamento de filtro de dados">
/**
 *  Filtra os dados do database pelas seleções nos componentes
 * @returns {undefined}
 */
function jsFilter()
{
    var aFiltros = {}; ///< Objeto para criação dos Filtros para o DataTable
    aFiltros.campos = [];
    aFiltros = jsAddFiltro(aFiltros, 'SELECT', 'selStatusAutomato', '', 'ativo', 'Status Ocorrencia');
    aFiltros = jsAddFiltro(aFiltros, 'TEXT', 'selEmpresa', '', 'empresa', 'Empresa');

    /*
     aFiltros = jsAddFiltro(aFiltros, 'DATA', 'txtDataMovimentoInicio', 'txtDataMovimentoInicio', 'DATAMOV', 'Data Movimento');
     aFiltros = jsAddFiltro(aFiltros, 'SELECT', 'selProduto', '', 'PRODUTO');
     aFiltros = jsAddFiltro(aFiltros, 'SELECT', 'selModulo', '', 'MODULO');
     aFiltros = jsAddFiltro(aFiltros, 'SELECT', 'selAlerta', '', 'NIVEL');
     
     */
    table = $("#" + tabelaDados).DataTable(); ///< Pega a Tabela atual
    table.destroy(); ///< destroy a tabela para criar com os dados Filtrados

    aWhere = jsCriaFiltros(aFiltros); ///< Cria o filtro pelos campos

    //console.log(aWhere);
    // Seta a Ordenação pela coluna DATAHORA
    var sort = [0, "desc"];
    tableInit(tabelaDados, endApi, colunas, columnDefs, aWhere, sort, jsTracaCorTextoLinha); ///< Inicializa a Tabela como DataTable        
}
//</editor-fold>

///<editor-fold defaultstate="collapsed" desc="Inicializacao da Página">
function jsInicializacaoPagina()
{
    endApi = SITE_LOCATION + "api/automatos";
    tabelaDados = 'tableData';
    where = {"ativo =": 1};
    tableInit(tabelaDados, endApi, colunas, columnDefs, where, null, jsTracaCorTextoLinha, 'auto_sequencial');
}

$(document).ready(function () {
    jsInicializacaoPagina();
});
//</editor-fold>

