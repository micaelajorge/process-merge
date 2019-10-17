/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 03/06/2018
 Sistema: Process
 */

//columnDefs = null;
dataTableResize = false;

var dataTableDetalhes;
var tituloDetails;

function mudaDimensao(endAtualApi, dimensao)
{
    enderecoApi = endApi + endAtualApi + encodeURIComponent(dimensao);
    dataTablePrincipal.ajax.url(enderecoApi).load();
}

function jsTrataGerarRelatorio(data, type, row, meta)
{
    retorno = '<i class="fa  fa-file-excel-o text-green" title="Gerar Planilha" onclick="mudaDimensao(\'' + row.endApi + '\', \'' + row.periodo + '\')" ></i>';
    return retorno;
}

function jsTrataValorExpandirContrair_Old(data, type, row, meta)
{
    // Determina se é uma Dimensão ou Detalhe, Dimensoes são Numero
    retorno = "";
    if (row.dimensionDisplay === '1')
    {
        if (row.expandeble)
        {
            if (row.expanded)
            {
                //retorno = '<i onclick=\"jsInsereDimencao(\'' + row.dimension + '\')" class="fa fa-search-plus text-primary" title="Expandir"></i>';
                retorno = '<i class="fa fa-search-minus text-danger" title="Contrarir" onclick="mudaDimensao(\'' + row.endApi + '\', \'\')"></i>';
            } else
            {
                retorno = '<i class="fa fa-search-plus text-primary" title="Expandir" onclick="mudaDimensao(\'' + row.endApi + '\', \'' + row.periodoSel + '\')" ></i>';
            }
        }
        retorno = retorno + ' <i class="fa  fa-file-excel-o text-green" title="Gerar Planilha" onclick="" ></i>';
    } else {
        retorno = "<i class=\"fa fa-file-text-o text-primary\" title=\"Exibir Detalhes\" onclick=\"jsInsereDimencao('" + row.dimension + "', '" + encodeURIComponent(row.periodo) + "', '" + row.periodo + "' )\"></i>";
    }
    return retorno;
}

function jsTrataValorExpandirContrair(data, type, row, meta)
{
    // Determina se é uma Dimensão ou Detalhe, Dimensoes são Numero
    retorno = "";
    if (row.dimensionDisplay === '1')
    {
        retorno = "<i class=\"fa fa-file-text-o text-primary\" title=\"Exibir Detalhes\" onclick=\"jsInsereDimencao('" + row.dimension + "', '" + encodeURIComponent(row.periodo) + "', '" + row.periodo + "' )\"></i>";
    }
    return retorno;
}

function jsTrataValorCampos(data, type, row, meta)
{
    fieldValue = data;
    switch (row.dimensionType) {
        case "tx":
            fieldDisplay = row.dimensionData;
            break;
        case "dt":
            fieldDisplay = jsTrataRenderCampoData(data);
            break;
        default:
            fieldDisplay = data;
            break;
    }
    // Determina se é uma Dimensão ou Detalhe
//    if (row.dimensionDisplay === 0)
//    {
//        retorno = "<span onclick=\"jsInsereDimencao('" + row.dimension + "', '" + encodeURIComponent(row.periodo) + "', '" + row.periodo + "' )\">" + fieldDisplay + "<span>";
//    } else
//    {
//        retorno = fieldDisplay;
//    }
//    return retorno;
    retorno = fieldDisplay;
    return retorno;
}

function jsInicializacaoPagina()
{
//    endApi = SITE_LOCATION + "api/cubo" + "/" + ProcId;
    endApi = SITE_LOCATION + "api/cubo/";
    tabelaDados = 'tableData';
    where = null;
//    if (ProcId !== undefined)
//    {
//        where = {global: true, ProcId: ProcId};
//    }

    extraParameters = {
        "ordering": false,
        "scrollX": false,
//        "scrollY": '350px',
//        "scrollCollapse": true,
        "paging": false,
        "select": false,
        "info": false,
        dom: 'Bfrtip',
        "buttons": [
            'excel', 'csv'
        ]
    };
    // Se a página form menor 500 modifica a dataTable
    if ($(window).width() < 500) {
        extraParameters = {
            "ordering": false,
            "info": false,
            "paging": false,
            "select": true,
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
    dataTablePrincipal = tableInit(tabelaDados, endApi, colunasCubo, columnDefs, where, sort, 0, 'DT_RowId', extraParameters);
}

var dataTablePrincipal;

// Array com as Linhas abertas
var detailRows = [];

$(document).ready(function () {
    jsInicializacaoPagina();
});

function jsFormataDadosDetalhes(detalhesLinha)
{
    if (jsInsereDimencao(detalhesLinha.dimensionData))
    {
        //return '<div><div id="divDimensao"><div class="loading text-center"><i class="fa fa-refresh fa-spin fa-4x"></i> <br>Carregando Dados</div></div></div>';
    }
}


function jsAcoesCuboRelatorio()
{
    /*
     // Atribui a ação de CLICK na celula de abrir ou fechar dimensão Z do cubo
     $('#tableData tbody').on('click', 'tr td.details-control', function () {
     var tr = $(this).closest('tr');
     var row = dataTablePrincipal.row(tr);
     var idx = $.inArray(tr.attr('id'), detailRows);
     $(row).children('td').css("padding-left", "0px");
     
     if (row.child.isShown()) {
     $(this).html('<i class="fa fa-search-plus text-primary" title="Expandir"></i>');
     tr.removeClass('details');
     row.child.hide();
     // Remove from the 'open' array
     //detailRows.splice( idx, 1 );
     } else {
     tr.addClass('details');
     $(this).html('<i class="fa  fa-search-minus text-primary"  title="Contrair"></i>');
     
     // Add to the 'open' array
     if (idx === -1) {
     row.child(jsFormataDadosDetalhes(row.data())).show();
     detailRows.push(tr.attr('id'));
     } else
     {
     row.child.show();
     }
     //            row.child.show();
     }
     });
     */
}
