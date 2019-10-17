/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 15/11/2018
 Sistema: Process_XAMPP
 */

function jsCarregaDataTableDetalhe(nomeDimensao, enderecoApi)
{    
    tabelaDestino = "tableData_" + nomeDimensao;
    parametroDimencao = {
        "initComplete": jsmoveDimensao
    };
    parametros = jsTrataParametrosExtra(extraParameters, parametroDimencao);
    //tableInit(tabelaDestino, endApi, colunasDimensao, colunasDimencaoDef, nomeDimensao, null, null, null, parametros);
    dataTableDetalhes = tableInit(tabelaDestino, enderecoApi, colunasDetalhe, colunasDetalheDef, nomeDimensao, null, null, null, parametros);
}

function jsInsereDimencao(nomeDimensao, filtroDetalhe, titulo)
{
    dadosEnvio = {
        dimensao: nomeDimensao
    };
    
    // Guarda o Titulo do datatable para incluir nos arquivos exportados
    tituloDetails = titulo;
    
    enderecoAPi = SITE_LOCATION + "api/dimensao/" + filtroDetalhe;    
    if ($("#tableData_" + nomeDimensao).length === 0)
    {
        $.post("dimensao",
                dadosEnvio,
                function (dadosRetorno) {
                    //$(".content-wrapper").append(dadosRetorno);                    
                    $(".content").append(dadosRetorno);
                    $("#detail-title").html(titulo);
//                    $(".box-body").append("<br/>" + dadosRetorno);                    
                    jsCarregaDataTableDetalhe(nomeDimensao, enderecoAPi);
                }
        );
    } else {
        dataTableDetalhes.ajax.url(enderecoAPi).load();
        $("#detail-title").html(titulo);
    }    
    return false;
}

function jsmoveDimensao()
{
    //$("#divDimensao").replaceWith($("#divDimensaoTemporaria").children().show());
    //$("#divDimensaoTemporaria").remove();    
}

