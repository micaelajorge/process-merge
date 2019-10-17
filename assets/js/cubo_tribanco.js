var colunasCubo = [
    {"data": null, "orderable": false},
    {"data": "periodo", className: "text-center"},
    {"data": "periodoSel", className: "hidden"},
    {"data": "totalCasos", className: "hidden-xs"},
    {"data": "SEM IMAGEM", className: "text-center"},    
    {"data": "AGUARDANDO ANALISE", className: "text-center"},
    {"data": "pendente_porcent", className: "text-center"},
    {"data": "analisadas", className: "text-center"},
    {"data": "REGULAR", className: "text-center"},
    {"data": "IRREGULAR", className: "text-center"},
    {"data": "pendente_porcent", className: "text-center"},
    {"data": "IRREGULAR", className: "text-center"},
    {"data": "desvioTotal", className: "text-center"}    
];

var colunasDimensao = [
    {
        "orderable": false,
        "data": null,
        "defaultContent": ""
    },
    {"data": "dimensionDisplay"},
    {"data": "totalCasos"}
];

var colunasDimencaoDef = [
    {
        "targets": [11],
        "render": jsTrataValorCampos,
        "visible": false

    },
    {
        "targets": [0],
        "render": jsTrataValorExpandirContrair,
        "width": "25px"
    }
];

var sort = null;
var columnDefs = [
    {
        "targets": [0],
        "render": jsTrataValorExpandirContrair,
        "width": "60px"
    },
    {
        "targets": [2],
        "visible": false
    }
];
var columnDefs_ = [
    {
        "targets": [3],
        "render": jsTrataValorCampos,
        "width": "305px"
    },
    {
        "targets": [2],
        "visible": false
    },
    {
        "targets": [0],
        "render": jsTrataValorExpandirContrair,
        "width": "60px"
    }
];

var colunasDetalheDef = [
    {
        "targets": [0],
        "render": jsTrataViewCase
    },
    {
        "targets": [5, 6, 3],
        "render": jsTrataRenderCampoData
    }
];

var colunasDetalhe = [
    {"data": "casenum"},
    {"data": "campoDisplay8"},
    {"data": "campoDisplay9"},
    {"data": "campoDisplay15"},
    {"data": "campoDisplay35"},
    {"data": "campoDisplay36"},
    {"data": "campoDisplay17"},
    {"data": "campoDisplay34"},
    {"data": "campoDisplay37"}
];
