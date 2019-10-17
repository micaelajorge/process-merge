//var colunasCubo = [
//    {"data": "periodo", className: "text-left"},
//    {"data": "totalCasos", className: "text-center"},
//    {"data": "SEM IMAGEM", className: "text-center"},    
//    {"data": "AGUARDANDO ANALISE", className: "text-center"},
//    {"data": "pendente_porcent", className: "text-center"},
//    {"data": "analisadas", className: "text-center"},
//    {"data": "REGULAR", className: "text-center"},
//    {"data": "IRREGULAR", className: "text-center"},
//    {"data": "desvioAnalizados", className: "text-center"},
//    {"data": "IRREGULAR", className: "text-center"},
//    {"data": "desvioTotal", className: "text-center"}    
//];

var colunasCubo_ = [
    {"data": "periodo", className: "text-left"},
    {"data": "totalCasos", className: "text-center"},
    {"data": "SEM IMAGEM", className: "text-center"},    
    {"data": "AGUARDANDO ANÁLISE", className: "text-center"},
    {"data": "tratativa", className: "text-center"},
    {"data": "analisadas", className: "text-center"},
    {"data": "REGULAR", className: "text-center"},
    {"data": "eficiencia_regulares", className: "text-center"},
    {"data": "IRREGULAR", className: "text-center"},
    {"data": "eficiencia_iregulares", className: "text-center"}   
];

var colunasCubo = [
    {"data": "periodo", className: "text-left"},
    {"data": "totalCasos", className: "text-center"},
    {"data": "SEM IMAGEM", className: "text-center"},    
    {"data": "AGUARDANDO ANÁLISE", className: "text-center"},
    {"data": "tratativa", className: "text-center"},
    {"data": "analisadas", className: "text-center"},
    {"data": "REGULAR", className: "text-center"},
    {"data": "eficiencia_regulares", className: "text-center"},
    {"data": "IRREGULAR", className: "text-center"},
    {"data": "eficiencia_iregulares", className: "text-center"}   
];

var colunasIrregularidade = [
    {"data": "Irregularidade", className: "text-left"},
    {"data": "porcent", className: "text-center"},
    {"data": "total", className: "text-center"}
];


function jsSelItemDimensao(data, type, row, meta)
{
    dados = `<a href="javascript:jsSelDimensao('${row.dimensionDisplay}', '${row.caption}', '${row.endApiComplemento}')">${data}</a>`;
    return dados;
}

function jsSelItemIrregularidade(data, type, row, meta)
{
    if (data == '0')
    {
        dados = data;
    } else {
        dados = `<a href="javascript:jsSelIrregularidades('${row.dimensionDisplay}', '${row.caption}', '${row.endApiComplemento}')">${data}</a>`;        
    }
    return dados;
}

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

    }
];

var sort = null;
var columnDefs = [];
var columnDefs_ = [
    {
        "targets": [0],
        "render": jsSelItemDimensao
    },
    {
        "targets": [8],
        "render": jsSelItemIrregularidade
    }   
];

var columnDefs_Irregularidades = [
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
