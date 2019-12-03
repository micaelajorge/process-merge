/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


var dashboardSecuritesListaCessoes_Colunas = [
    {
        "data": null,
        "orderable": false
    },
    {
        "data": null,
        "orderable": false
    }
];

var dashboardSecuritesListaCessoes_columnsDefs = [
    {
        targets: 0,
        render: (data, type, row, meta) => {
            return jsDashboardSecurities_SelecionaCessao(row);         
        },     
        "width": "120px"
    },
    {
        targets: 1,
        render: (data, type, row, meta) => {
            return jsDashboardSecurities_RenderizaDadosCessao(row["referencias"]);         
        },
        "orderable": false
    }
];

extraPameters = {
    "scrollX": false,
    "scrollXInner": false,
    "pageLength": 5,
    "initComplete": jsDashboardSecurities_RenderizaCompletada,
    "ordering": false
};

var sort = [0, "desc"];

function jsDashboardSecurities_SelecionaCessao(dados)
{
    cessionUUID = dados.referencias[3].FieldValue;
    return '<a href="javascript:jsCarregaSinatariosCessao(\''+ cessionUUID +'\')" title="Visualizar Assinaturas da Cessão">Status assinaturas</a>';
}

function jsDashboardSecurities_RenderizaCompletada()
{
    $("#dashboard_lista_cessoes_box").removeClass("box-default");
    $("#dashboard_lista_cessoes_box").addClass("box-primary");
}

function jsDashboardSecurities_RenderizaDadosCessao(dadosReferencias)
{
    return 'Cessão: ' + dadosReferencias[0]["FieldValue"] + ', Status: ' + dadosReferencias[1]["FieldValue"];
}

function jsDashboardSecurities_IniciaListaCessoes()
{
    endApi = "queuelist/SEC_CESSOES";
    tableInit('dashboardSecurities_ListaCessoes', endApi, dashboardSecuritesListaCessoes_Colunas, dashboardSecuritesListaCessoes_columnsDefs, {}, sort, 0, 'casenum', extraPameters);
}

