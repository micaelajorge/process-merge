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

function jsTrataValorExpandirContrair(data, type, row, meta)
{
    // Determina se é uma Dimensão ou Detalhe, Dimensoes são Numero
    retorno = "";
    if (row.dimensionDisplay === "1")
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

function jsClickRow(data)
{
    dadosFormulario = jsGetDataFromForm("frmDadosEntidade");
    url = "relatorioonlinedimensao/" + data.dimensionDisplay + "/" + data.caption ;
    $.ajax({
        url: url,
        method: "POST"
    })
            .done(function (retorno) {
                // Sucesso em gravar
                $("#divDimensao" + data.dimensionDisplay).html(retorno);
                jsCriaDataTable_Dimensao("dimensao" + data.dimensionDisplay, data.endApiComplemento);
            })
            .fail(function (retorno) {
                // Falha ao gravar
                alert(retorno);
            });
}

function jsSelDimensao(dimensionDisplay, caption, endApiComplemento)
{
    
    url = "relatorioonlinedimensao/" + dimensionDisplay + "/" + caption ;
    $.ajax({
        url: url,
        method: "POST"
    })
            .done(function (retorno) {
                // Sucesso em gravar
                $("#divDimensao" + dimensionDisplay).html(retorno);
                jsCriaDataTable_Dimensao("dimensao" + dimensionDisplay, endApiComplemento, colunasCubo, columnDefs_);
            })
            .fail(function (retorno) {
                // Falha ao gravar
                alert(retorno);
            });
}

function jsSelIrregularidades(dimensionDisplay, caption, endApiComplemento)
{
    
    url = "relatorioonlineirregularidades/" + dimensionDisplay + "/" + caption ;
    $.ajax({
        url: url,
        method: "POST"
    })
            .done(function (retorno) {
                // Sucesso em gravar
                $("#divDimensao" + dimensionDisplay).html(retorno);
                jsCriaDataTable_Irregularidades("dimensao" + dimensionDisplay, endApiComplemento, colunasIrregularidade, columnDefs_Irregularidades);
            })
            .fail(function (retorno) {
                // Falha ao gravar
                alert(retorno);
            });
}



function jsClickRow(data)
{
    dadosFormulario = jsGetDataFromForm("frmDadosEntidade");
    url = "relatorioonlinedimensao/" + data.dimensionDisplay + "/" + data.caption ;
    $.ajax({
        url: url,
        method: "POST"
    })
            .done(function (retorno) {
                // Sucesso em gravar
                $("#divDimensao" + data.dimensionDisplay).html(retorno);
                jsCriaDataTable_Dimensao("dimensao" + data.dimensionDisplay, data.endApiComplemento);
            })
            .fail(function (retorno) {
                // Falha ao gravar
                alert(retorno);
            });
}

function jsCreatedRow(row, data, dataIndex)
{
    $(row).css("cursor", "pointer");
    
    // Define se a dimensao pode expandir
    if (data.dimensaoExpandivel === 1) {
        $(row).on("click", function () {
            jsClickRow(data);
        });
    }
}

var dadosLinha;





function jsCriaDataTable_Dimensao(dataTableSelecionado, parametrosRota)
{
    let endApi = SITE_LOCATION + "api/v1/relatorioonline";
    jsCriaDataTable(dataTableSelecionado, parametrosRota, colunasCubo, columnDefs_, endApi);
}

function jsCriaDataTable_Irregularidades(dataTableSelecionado, parametrosRota, colunasDatatable, colunasDef)
{
    let endApi = SITE_LOCATION + "api/v1/relatorioirregularidades";
    jsCriaDataTable(dataTableSelecionado, parametrosRota, colunasDatatable, colunasDef, endApi);
}

function jsCriaDataTable(dataTableSelecionado, parametrosRota, colunasDatatable,  colunasDef, endApi)
{
    
    if (typeof parametrosRota !== "undefined")
    {
        endApi = endApi + parametrosRota;
    }
    where = null;
//    if (ProcId !== undefined)
//    {
//        where = {global: true, ProcId: ProcId};
//    }

    extraParameters = {
        "ordering": false,
//        "scrollX": false,
//        "scrollY": '350px',
//        "scrollCollapse": true,
        "paging": false,
        "select": true,
        "info": false,
        dom: 'Bfrtip',
        "buttons": [
            'excel'
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
    funcAfterLoadDataTable = jsDepoisDeCarregarDataTable;
//    tableInit(dataTableSelecionado, endApi, colunasCubo, colunasCubo, where, sort, null, 'DT_RowId', extraParameters);
    tableInit(dataTableSelecionado, endApi, colunasDatatable, colunasDef, where, sort, null, 'DT_RowId', extraParameters);
}

function jsInicializacaoPagina()
{
    funcAfterLoadDataTable = jsSetaSelectRow;
    jsCriaDataTable_Dimensao('tableData');
}

function jsDepoisDeCarregarDataTable(dataTable)
{
    jsSetaSelectRow();
    // Move o documento para o fim da tela    
    $(".buttons-excel").addClass("btn btn-primary");
    $(".buttons-excel").html("<i class=\"fa fa-file-excel-o\"></i>");
    $(".buttons-excel").attr("title", "Exportar para excel");
    $(".content-wrapper").scrollTop( $(window).innerHeight());
}

function jsSetaSelectRow()
{
    $('.datatable tbody').on('click', 'tr', function () {
        $(this).parents(".datatable").find("tr").removeClass("selected");
        $(this).addClass('selected');
    });
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
