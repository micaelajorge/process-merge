var colunasSelCartorio = [
    "casenum",
    "casenum",
    "cartorio_nome",
    "cidade"
];
var defColunasSelCartorio = [
    {
        "targets": 0,
        "render": jsSelecionarCartorio,
        "width": "50px",
        "orderable": false
    },
    {
        "targets": [1, 2, 3],
        "orderable": false
    }
];
/**
 * Usado pela função js_ext_busca_cartorios_SelecionarCartorio, 
 * para selecionar o Campo do formulário com dados do Cartório
 */
var FieldIdCartorio;
// Id da ROW sendo renderizada
var rowIdRender = 0;
// caseNum cartorio selecionado
var cartorioSelecionadoCaseNum = 0;

function js_ext_busca_cartorios_SelecionarCartorio(rowIdSelecionado)
{
    table = $("#" + tabelaDados).DataTable(); ///< Pega a Tabela atual
    jsonData = table.ajax.json();
    
    while (rowIdSelecionado >= 10)
    {
        rowIdSelecionado = rowIdSelecionado - 10;
    }
    
    $('#valorDisplay' + FieldIdCartorio).val(jsonData.data[rowIdSelecionado].cartorio_nome);
    $('#t' + FieldIdCartorio).val(jsonData.data[rowIdSelecionado].casenum);
    $('#btn_view_cartorio').removeAttr('disabled');
    $('#modal' + FieldIdCartorio).modal('hide');
}


/**
 * 
 * @param {type} data
 * @param {type} type
 * @param {type} row
 * @param {type} meta
 * @returns {fieldData|String}
 */
function jsSelecionarCartorio(data, type, row, meta)
{
    fieldData = "";
    if (data != cartorioSelecionadoCaseNum)
    {
        fieldData = '<button type="button" class="btn btn-success btn-sm" onclick="js_ext_busca_cartorios_SelecionarCartorio(' + rowIdRender + ')")><i class="fa fa-check"></i></button>';
    }
    return fieldData;
}

/**
 * 
 * @param {type} FieldId
 * @returns {undefined}
 */
function js_ext_busca_cartorios_MostrarBusca(FieldId)
{
    FieldIdCartorio = FieldId;
    pracaSelecionada = $('[aria-code="PRACA_CIDADE"]').val();
    cartorioSelecionadoCaseNum = $('[aria-field-id="' + FieldId + '"]').val();
    dadosEnvio = {
        "pracaSelecionada": pracaSelecionada,
        "cartorioSelecionado": cartorioSelecionadoCaseNum
    };
    url = "buscacartorios";
    $.ajax(
            {
                url: url,
                type: "post",
                data: dadosEnvio,
                dataType: 'html'
            })
            .done(function (dataRetorno) {
                $('#modal' + FieldId).html(dataRetorno);
                $('#modal' + FieldId).modal();
                setTimeout(function () {
                    js_ext_busca_cartorios_ExecutaBusca(FieldId);
                },
                        500);
            })
            .fail(function (jqXHR, textStatus) {
                alert(textStatus);
            });
}

/**
 * 
 * @param {type} ProcId
 * @param {type} FieldSource
 * @param {type} fieldDestino
 * @param {type} selCidade
 * @param {type} selCartorio
 * @returns {undefined}
 */
function js_ext_busca_cartorios_lista_cartorios(ProcId, FieldSource, fieldDestino, selCidade, selCartorio)
{
    rowIdRender = 0;
    endApi = SITE_LOCATION + "listacartorios";
    tabelaDados = 'tableCartorios';
    sort = null;
    where = {
        "global": true,
        "ProcSource": ProcId,
        "FieldSource": FieldSource,
        "pracaSelecionada": selCidade,
        "selCartorio": selCartorio,
        "fieldDestino": fieldDestino
    };
    extraPameters = {
        "scrollX": false,
        "pageLength": 5
    };
    table = $("#" + tabelaDados).DataTable(); ///< Pega a Tabela atual
    table.destroy();
    tableInit(tabelaDados, endApi, colunasSelCartorio, defColunasSelCartorio, where, sort, rowCallBack, null, extraPameters);
}

/**
 * 
 * @param {type} FieldId
 * @returns {undefined}
 */
function js_ext_busca_cartorios_ExecutaBusca(FieldId)
{
    pracaSelecionada = $('#selCidade').val();
    processoSelecionar = "CARTORIOS_GATEWAY_REGISTROS";
    fieldOrigem = "CARTORIO_NOME";
    js_ext_busca_cartorios_lista_cartorios(processoSelecionar, fieldOrigem, FieldId, pracaSelecionada, cartorioSelecionadoCaseNum);
}

function rowCallBack(row, data, index)
{
    if (data.casenum == cartorioSelecionadoCaseNum)
    {
        $(row).css('background-color', '#99ff9c');
    }
    rowIdRender++;
}