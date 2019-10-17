var colunas = [
    {
        "class": "details-control",
        "orderable": false,
        "data": null,
        "defaultContent": ""
    },
    {"data": "dimensionDisplay"},
    {"data": "nrRegistros", className: "hidden-xs"}
];

var colunasDimensao = [
    {
        "class": "details-control",
        "orderable": false,
        "data": null,
        "defaultContent": ""
    },
    {"data": "dimensionDisplay"},
    {"data": "nrRegistros"}
];

var colunasDimencaoDef = [
    {
        "targets": [1],
        "render": jsTrataValorCampos,
        "width": "300px"
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
        "targets": [1],
        "render": jsTrataValorCampos,
        "width": "305px"
    },
    {
        "targets": [0],
        "render": jsTrataValorExpandirContrair,
        "width": "25px"
    }

];