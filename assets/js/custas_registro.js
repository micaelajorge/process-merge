function jsSelecionaTipoDespesa()
{
    tipoDespesa = $("#CUSTAS_REGISTRO_TipoDespesa").val();
    $("#CUSTAS_REGISTRO_Despesa").val("");
    $('[aria-despesa]').hide();
    $('[aria-tipo="' + tipoDespesa + '"]').show();
}

function jsCustasRegistroSalvar(fieldId)
{
    json_listaDespesas = $("#CUSTAS_REGISTRO_LISTADESPESAS").val();
    listaDespesas = JSON.parse(json_listaDespesas);
    $("#modal" + fieldId).modal('hide');

    dadosEnvio = {};
    dadosEnvio.listaDespesas = listaDespesas;
    

    valorTotal = 0;
// take the third column
    listaDespesas.forEach(function (item, index) {
        valorTotal += parseFloat(item.valorDespesa);
    });    

    $("#total_custos_registro" + fieldId).val("R$ " + valorTotal.toFixed(2));
    $("#t" + fieldId).val(encodeURI(json_listaDespesas));
    jsCustasRegistroCarregaTabela("#divCustasRegistroTableDetalhe", dadosEnvio);
}

function jsCustosRegistroEditarDespesa(indiceDespesa)
{
    listaDespesas = [];
    json_listaDespesas = decodeURIComponent($("#CUSTAS_REGISTRO_LISTADESPESAS").val());
    listaDespesas = JSON.parse(json_listaDespesas);
    itemDespesa = listaDespesas[indiceDespesa];

    $("#btnAddDespesa").hide();
    $("#btnSalvaDespesa").show();

    $("#CUSTAS_REGISTRO_TipoDespesa").val(itemDespesa.tipoDespesa);
    $("#CUSTAS_REGISTRO_Despesa").val(itemDespesa.despesa);
    $("#CUSTAS_REGISTRO_ValorDespesa").val(itemDespesa.valorDespesa);
    $("#CUSTAS_REGISTRO_INDICE_DESPESA").val(indiceDespesa);
}

function jsCustosRegistroShowDetalhes(fieldId)
{
    $("#divDetalhes" + fieldId).show();
    $("#divResumo" + fieldId).hide();
}

function jsCustosRegistroHideDetalhes(fieldId)
{
    $("#divDetalhes" + fieldId).hide();
    $("#divResumo" + fieldId).show();
}

function jsShowModalCustas(fieldId)
{
    dadosEnvio = {};
    dadosEnvio.fieldId = fieldId;

    json_listaDespesas = decodeURIComponent($("#t" + fieldId).val());
    dadosEnvio.listaDespesas = JSON.parse(json_listaDespesas);
    dadosEnvio.editar = 1;
    url = "custoregistroedit";
    $.ajax(
            {
                url: url,
                type: "POST",
                data: dadosEnvio,
                dataType: 'html'
            })
            .done(function (dataRetorno) {
                $("#modal" + fieldId).html(dataRetorno);
                $("#modal" + fieldId).modal();
            }
            )
            .fail(function (jqXHR, textStatus) {
                alert(textStatus);
            });
}

function jsCustasRegistroAdicionaDespesa()
{
    itemDespesa = {};
    itemDespesa.tipoDespesa = $("#CUSTAS_REGISTRO_TipoDespesa").val();
    itemDespesa.despesa = $("#CUSTAS_REGISTRO_Despesa").val();
    itemDespesa.valorDespesa = $("#CUSTAS_REGISTRO_ValorDespesa").val();

    indiceDespesa = $("#CUSTAS_REGISTRO_INDICE_DESPESA").val();

    // Trata o campo de lista de despesas
    listaDespesas = [];
    json_listaDespesas = decodeURIComponent($("#CUSTAS_REGISTRO_LISTADESPESAS").val());
    if (json_listaDespesas !== "")
    {
        listaDespesas = JSON.parse(json_listaDespesas);

    }
    if (indiceDespesa == -1)
    {
        listaDespesas.push(itemDespesa);
    } else {
        listaDespesas[indiceDespesa] = itemDespesa;
    }

    json_listaDespesas = JSON.stringify(listaDespesas);

    $("#CUSTAS_REGISTRO_LISTADESPESAS").val(json_listaDespesas);

    dadosEnvio = {};
    dadosEnvio.listaDespesas = listaDespesas;
    dadosEnvio.fieldId = $("#CUSTAS_REGISTRO_FIELDID").val();
    dadosEnvio.editar = 1;

    jsCustasRegistroCancelarEdicao();

    jsCustasRegistroCarregaTabela("#divCustasRegistroModalTable", dadosEnvio);
}

function jsCustasRegistroCancelarEdicao()
{
    $("#btnAddDespesa").show();
    $("#btnSalvaDespesa").hide();
    $("#CUSTAS_REGISTRO_TipoDespesa").val("");
    $("#CUSTAS_REGISTRO_Despesa").val("");
    $("#CUSTAS_REGISTRO_ValorDespesa").val("");
    $("#CUSTAS_REGISTRO_INDICE_DESPESA").val(-1);
}

function jsCustasRegistroRemoverDespesa(indiceItem)
{
    json_listaDespesas = decodeURIComponent($("#CUSTAS_REGISTRO_LISTADESPESAS").val());
    listaDespesas = JSON.parse(json_listaDespesas);

    listaDespesas.splice(indiceItem, 1);

    json_listaDespesas = JSON.stringify(listaDespesas);

    $("#CUSTAS_REGISTRO_LISTADESPESAS").val(json_listaDespesas);

    dadosEnvio = {};
    dadosEnvio.listaDespesas = listaDespesas;
    dadosEnvio.fieldId = $("#CUSTAS_REGISTRO_FIELDID").val();
    dadosEnvio.editar = 1;
    jsCustasRegistroCarregaTabela("#divCustasRegistroModalTable", dadosEnvio);
}

function jsCustasRegistroCarregaTabela(tabelaAlvo, dadosEnvio)
{
    url = "custoregistrotabela";
    $.ajax(
            {
                url: url,
                type: "POST",
                data: dadosEnvio,
                dataType: 'html'
            })
            .done(function (dataRetorno) {
                $(tabelaAlvo).html(dataRetorno);
            })
            .fail(function (jqXHR, textStatus) {
                alert(textStatus);
            });
}