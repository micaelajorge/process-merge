/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 30/10/2019
 Sistema: Process_XAMPP
 */


/**
 * 
 * @param {type} fieldId
 * @param {type} idItem
 * @returns {undefined}
 */
function js_ext_campo_json_array_edit(fieldId, idItem)
{

    if (idItem === -1)
    {
        $("#myModalLabel_Novo" + fieldId).show();
        $("#myModalLabel_Editar" + fieldId).hide();
        $("#btn_apagar" + fieldId).attr("disabled", "true");
        $("#DivModal" + fieldId).find("input:text").val("");
    } else {
        $("#myModalLabel_Editar" + fieldId).show();
        $("#myModalLabel_Novo" + fieldId).hide();
        $("#btn_apagar" + fieldId).removeAttr("disabled");
        let valorCampo = $("#t" + fieldId).val();
        let dataSet = JSON.parse(valorCampo);

        dadosEditar = dataSet[idItem];

        colunas = Object.keys(dadosEditar);

        for (let i = 0; i < colunas.length; i++)
        {
            let coluna = colunas[i];
            $("#" + coluna).val(dadosEditar[coluna]);
        }
    }
    $("#idItem" + fieldId).val(idItem);
    $("#DivModal" + fieldId).modal();
}

function js_ext_campo_json_array_save(fieldId)
{
    let idItem = $("#idItem" + fieldId).val();
    let valorCampo = $("#t" + fieldId).val();
    let dataSet = JSON.parse(valorCampo);

    listaCamposEdicao = $("#DivModal206").find("input:text");

    dadoEditado = {};

    for (let i = 0; i < listaCamposEdicao.length; i++)
    {
        nomeDado = $(listaCamposEdicao[i])[0].name;
        valorDado = $(listaCamposEdicao[i]).val();
        dadoEditado[nomeDado] = valorDado;
    }

    if (idItem == -1)
    {
        dataSet.push(dadoEditado);
    } else {
        dataSet[idItem] = dadoEditado;
    }

    $("#t" + fieldId).val(JSON.stringify(dataSet));
    js_ext_campo_json_array_montaTabela(fieldId);
    $("#DivModal" + fieldId).modal('hide');
}

function js_ext_campo_json_array_delete(fieldId)
{
    let idItem = $("#idItem" + fieldId).val();
    let valorCampo = $("#t" + fieldId).val();
    let dataSet = JSON.parse(valorCampo);

    if (idItem == 0 & dataSet.length == 1)
    {
        dataSetFinal = [];
    } else {
        dataSetFinal = dataSet.slice(idItem, 1);
    }

    $("#t" + fieldId).val(JSON.stringify(dataSetFinal));
    js_ext_campo_json_array_montaTabela(fieldId);
    $("#DivModal" + fieldId).modal('hide');
}

/**
 * 
 * @param {type} data
 * @param {type} type
 * @param {type} row
 * @param {type} meta
 * @returns {String}
 */
function js_ext_campo_json_array_render_edit(data, type, row, meta)
{
    return `<a href="javascript:js_ext_campo_json_array_edit(${data},${meta.row})" title="Editar Item"><i class="fa fa-edit"></i></a>`;
}

/**
 * 
 * @param {type} fieldId
 * @returns {undefined}
 */
function js_ext_campo_json_array_montaTabela(fieldId)
{
    let valorCampo = $("#t" + fieldId).val();
    let colunasView = JSON.parse($("#COLUNAS_" + fieldId).val());

    colunaEdit = {
        data: "fieldId",
        render: js_ext_campo_json_array_render_edit
    };

    colunasEdit = [];
    colunasEdit.push(colunaEdit);

    colunasTabela = colunasEdit.concat(colunasView);

    let dataSet = JSON.parse(valorCampo);

    dataSet = dataSet.map((item, index) => {
        item["fieldId"] = fieldId;
        return item;
    });


    $('#dataTable' + fieldId).DataTable({
        destroy: true,
        data: dataSet,
        columns: colunasTabela,
        scrollY: 200,
        searching: false,
        ordering: false,
        paging: false,
        bInfo: false,
        language: {
            emptyTable: "Sem itens Cadastrados"
        }
    });
}