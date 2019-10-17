/*
 * Sincronica Sistemas Integrados Ltda
 * Projeto: checkScan | Arquivo: datatables.init.js
 * Autor: Alexandre Unruh <alexandre@sincronica.com.br> | Data: 24/07/2017
 */


// Nome Padrão para a variavel que contem o dataTable
var oTable = null;
var tabelaDados;
var offSetTamanhoPagina = 160;
var dataTableResize = true;
var janelaAuxiliar;
var janelaAuxiliarWidth = 1000;
var janelaAuxiliarheight = 550;
var funcAfterLoadDataTable = null;

function jsRenderTemplate(data, type, row, meta)
{

}

function jsTrataViewCase(data, type, row, meta)
{
    fieldValue = '<a href="javascript:JSvisualizaDoc(' + row.casenum + ', ' + row.procid + ')">' + data + '</a>';
    return fieldValue;
}

/**
 * 
 * @param {type} url
 * @param {type} colunas
 * @param {type} colunasDefinicao
 * @param {type} where
 * @param {type} rowId
 * @param {type} createdRow
 * @param {type} sort
 * @returns {parametrosPadraoDataTable}
 */
function jsParametrizacaoPadrao(url, colunas, colunasDefinicao, where, rowId, createdRow, sort)
{
    if (sort === null)
    {
        sort = {};
    }
    if (colunas.length > 0)
    {
        if (colunas[0].data === undefined)
        {
            var tablecolumns = [];
            for (var i = 0; i < colunas.length; i++) {
                tablecolumns.push({'data': colunas[i]});
            }
        } else {
            tablecolumns = colunas;
        }
    } else {
        tablecolumns = null;
    }
       
    var filterServerSide = url ? true : false;
    var filterWhere = (where) ? where : null;
    var options = {'where': filterWhere};
    var filterAjax = url ? {url: url, type: "post", data: options} : false;

    /*
     * "rowId": 'auto_sequencial',
     */
    parametrosPadraoDataTable = {
        "order" : (sort) ? sort : null,
        "searching": false,
        "processing": filterServerSide,
        "serverSide": filterServerSide,
        "ajax": filterAjax,
        "columns": url ? tablecolumns : null,
        "responsive": false,
        "lengthChange": false,
        "columnDefs": colunasDefinicao ? colunasDefinicao : null,
        "createdRow": createdRow ? createdRow : null,
        "rowId": rowId ? rowId : null,
        "initComplete": jsShowDivDataTableWrapper,
        "drawCallback": jsCalculaTamanhoBodyScrollDatatable,
        "language": {
            "sEmptyTable": "Nenhum registro encontrado",
            "sInfo": "Mostrando de</span> _START_ até _END_ de _TOTAL_ registros",
            "sInfoEmpty": "Mostrando 0 até 0 de 0 registros",
            "sInfoFiltered": "(<span class='visible-lg-inline'>Filtrados de</span> _MAX_ <span class='visible-lg-inline'>registros</span>)",
            "sInfoPostFix": "",
            "sInfoThousands": ".",
            "sLengthMenu": "_MENU_ resultados por página",
            "sLoadingRecords": "Carregando...",
            "sProcessing": "<div><i class=\"fa fa-spin fa-refresh fa-4x\"></i></div>Processando",
            "sZeroRecords": "Nenhum registro encontrado",
            "sSearch": "Pesquisar ",
            "oAria": {
                "sSortAscending": ": Ordenar colunas de forma ascendente",
                "sSortDescending": ": Ordenar colunas de forma descendente"
            },
            "oPaginate": {
                "sNext": ">",
                "sPrevious": "<",
                "sFirst": "Prim",
                "sLast": "Últ"
            }
        }
    };
    return parametrosPadraoDataTable;
}

function jsCalculaTamanhoDivDataTable()
{
    heightConteudoTopo = $("#divTopo").outerHeight();
    if (heightConteudoTopo === undefined)
    {
        heightConteudoTopo = 0;
    }
    offSetTamanhoPagina = heightConteudoTopo;

    heightFooter = $(".main-footer").outerHeight();
    if (heightFooter === undefined)
    {
        heightFooter = 0;
    }
    offSetTamanhoPagina = offSetTamanhoPagina + heightFooter;

}

function jsCalculaTamanhoBodyScrollDatatable()
{
    dataTable_scroll_height = $(".dataTables_scroll").height();
    dataTable_scroll_head_height = $(".dataTables_scrollHead").height();
    $(".datatables_scrollBody").css("height", dataTable_scroll_height - dataTable_scroll_head_height + "px");
}

/*
 * 
 * @returns {undefined}
 */
function jsShowDivDataTableWrapper(dataTable)
{
    jsCalculaTamanhoBodyScrollDatatable();
    $('div.loading').css("display", "none");
    if (dataTableResize)
    {
        $("#" + tabelaDados).parents(".row").height($(".content-wrapper").height() - offSetTamanhoPagina);
    }
    $("#box-content-datatable").removeClass("hidden");
    $("#divDataTableWrapper").removeClass("hidden");
    if (funcAfterLoadDataTable !== null)
    {
        funcAfterLoadDataTable(dataTable);
    }
}

/**
 * Formatação de Campo data
 * @param {type} data
 * @param {type} type
 * @param {type} row
 * @param {type} meta
 * @returns {defines@arr;sl_paticipante|valorF|String|fieldValue}
 */
function jsTrataRenderCampoData(data, type, row, meta)
{
    if (data == "" | data == null)
    {
        return "";
    }

    dataHoraRecebida = data;
    if (dataHoraRecebida.indexOf("/") > -1)
    {
        fieldValue = moment(dataHoraRecebida, "DD/MM/Y").format("DD/MM/Y");
    } else {
        fieldValue = moment(dataHoraRecebida).format("DD/MM/Y");
    }

    return fieldValue;
}

/**
 * Formatação de Campo data e HORA
 * @param {type} data
 * @param {type} type
 * @param {type} row
 * @param {type} meta
 * @returns {defines@arr;sl_paticipante|valorF|String|fieldValue}
 */
function jsTrataRenderCampoDataHora(data, type, row, meta)
{
    //console.log(row);
    fieldValue = moment(data).format("DD/MM/Y HH:mm:ss");
    return fieldValue;
}

/**
 * Formatação de Campo HORA
 * @param {type} data
 * @param {type} type
 * @param {type} row
 * @param {type} meta
 * @returns {defines@arr;sl_paticipante|valorF|String|fieldValue}
 */
function jsTrataRenderCampoHora(data, type, row, meta)
{
    fieldValue = moment(data).format("HH:mm:ss");
    return fieldValue;
}

/**
 * Formatação de Campo Sim/Nao
 * @param {type} data
 * @param {type} type
 * @param {type} row
 * @param {type} meta
 * @returns {defines@arr;sl_paticipante|valorF|String|fieldValue}
 */
function jsTrataRenderCampoSimNao(data, type, row, meta)
{
    fieldValue = (data === "1") ? "Sim" : "Não";
    return fieldValue;
}

/**
 * Formatação de Campo Valor
 * @param {type} data
 * @param {type} type
 * @param {type} row
 * @param {type} meta
 * @returns {defines@arr;sl_paticipante|valorF|String|fieldValue}
 */
function jsTrataRenderCampoValor(data, type, row, meta)
{
    var valorF = (parseFloat(data) / 100).toFixed(2);
    fieldValue = valorF.replace(".", ",");
    return fieldValue;
}

/**
 * Formatação de Campo Intervalo
 * @param {type} data
 * @param {type} type
 * @param {type} row
 * @param {type} meta
 * @returns {defines@arr;sl_paticipante|valorF|String|fieldValue}
 */
function jsTrataRenderIntervalo(data, type, row, meta)
{
    var valorInt = (parseInt(data) / 60000); //TODO: Mudar para aceitar 1 hora ou intervalos 
    fieldValue = valorInt + " min";
    return fieldValue;
}


/**
 * Executa o Filtro nos Dados para serem apresentados no DATATABLE
 * 
 * @param {type} dataTable  DATATABLE onde será aplicado o filtro
 * @param {type} origemDados rota que retornara os dados
 * @param {type} aFiltros    array com os filtros aplicados 
 * @param {type} colunas     colunas a serem retornadas
 * @param {type} colunasDefs definição das colunas a serem retornadas
 * @returns {undefined}
 */
function jsExecutaFiltro(dataTable, origemDados, aFiltros, colunas, colunasDefs)
{
    aWhere = jsCriaFiltros(aFiltros); // Cria o filtro pelos campos              
    table = $("#" + tabelaDados).DataTable();
    table.destroy();
    tableInit(dataTable, origemDados, colunas, aWhere, colunasDefs); // Inicializa a Tabela como DataTable        
}

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * 
 * @param {type} objDataInicio
 * @param {type} objDataFim
 * @param {type} campoDB
 * @param {type} descCampo
 * @param {type} columns
 * @returns {Array|jsValidaIntervaloData.parametro|Boolean}
 */
function jsValidaIntervaloData(objDataInicio, objDataFim, campoDB, descCampo, columns)
{
    retorno = false;

    var aWhere = [];
    var parametro = [];
    var retornarDataInicio = false;
    var retornarDataFim = false;
    var dtInicio = null;
    var dtFim = null;
    var hrInicio = null;
    var hrFim = null;
    if (objDataInicio.val() !== '')
    {
        var retornarDataInicio = true;
        dateTimeInicio = moment(objDataInicio.val(), "DD-MM-YYYY HH:mm:ss"); ///< Data/hora formatada sistema BR
        if (!dateTimeInicio._isValid)
        {
            alert(descCampo + " inválido");
            objDataInicio.focus();
            return false;
        }

        dtInicio = dateTimeInicio.format("YYYY-MM-DD"); ///< Data formato USA
        hrInicio = dateTimeInicio.format(" HH:mm:ss"); ///< Hora formato USA   
    }

    if (objDataFim !== null)
    {
        if (objDataFim.val() !== '')
        {
            var retornarDataFim = true;
            dateTimeFim = moment(objDataFim.val(), "DD-MM-YYYY HH:mm:ss");
            if (!dateTimeFim._isValid)
            {
                alert(descCampo + " inválido");
                objDataFim.focus();
                return false;
            }

            dtFim = dateTimeFim.format("YYYY-MM-DD");
            hrFim = dateTimeFim.format(" HH:mm:ss");
            if (hrFim === " 00:00:00")
            {
                hrFim = " 23:59:59";
            }
        }
    }

    /**
     *   Verifica se é um dia de inicio 
     */
    if ((retornarDataInicio & !retornarDataFim)) ///< (retornarDataInicio & retornarDataFim & dtInicio === dtFim & (hrInicio === " 00:00:00" | hrFim === " 00:00:00")))
    {
        dtFim = dtInicio;
        hrFim = " 23:59:59";
        retornarDataFim = true;
    }

    /**
     *  Cria o array de datas para seleção
     */
//    if (columns !== undefined & !columns)
    if (columns !== undefined & !columns)
    {
        (retornarDataInicio) ? parametro[campoDB + " >= "] = dtInicio + hrInicio : null;
        (retornarDataFim) ? parametro[campoDB + " <= "] = dtFim + hrFim : null;
    } else {
        if (retornarDataInicio)
        {
            if (!retornarDataFim)
            {
                parametro = {
                    filtro: {
                        campo: campoDB,
                        valor: dtInicio,
                        tipo: "DT",
                        nome: descCampo
                    }
                };
            } else {
                parametro = {
                    filtro: {
                        campo: campoDB,
                        valor: [ dtInicio, dtFim ],
                        tipo: "DT",
                        nome: descCampo
                    }
                };                
            }            
        }
        
    }
    return (!retornarDataInicio & !retornarDataFim) ? true : parametro; ///< Se não houve nenhuma data inserida retorna false
}

function jsAddFiltro(aFiltros, tipoCampo, objFiltro1, objFiltro2, campoTabela, txtMensagem)
{
    var filtro = {};
    filtro.tipoCampo = tipoCampo;
    filtro.obj1 = objFiltro1;
    filtro.obj2 = objFiltro2;
    filtro.campoTabela = campoTabela;
    filtro.txtMensagem = txtMensagem;
    aFiltros.campos.push(filtro);
    return aFiltros;
}

function jsFiltroCampoSelect(objFiltro, campoTabela)
{
    var valorSel = $('#' + objFiltro).val();
    var selecao = [];
    if (valorSel !== "")
    {
        selecao[campoTabela + ' = '] = valorSel;
    }
    return selecao;
}

function jsFiltroCampoText(objFiltro, campoTabela, $column, textMensagem)
{
    var valorSel = $('#' + objFiltro).val();
    var selecao = [];
    if (valorSel !== "")
    {

        if ($column)
        {
            selecao = {
                filtro: {
                    campo: campoTabela,
                    valor: valorSel,
                    tipo: "TX",
                    nome: textMensagem
                }
            };

        } else {
            selecao[campoTabela + ' like '] = "%" + valorSel + "%";
        }

    }
    return selecao;
}


function jsCriaFiltros(aFiltros, sql)
{
    var aWhere = [];
    for (i = 0; i < aFiltros.campos.length; i++)
    {
        retornoValida = jsCriaFiltro(aFiltros.campos[i], !sql);
        //retornoValida = jsCriaFiltro(aFiltros.campos[i], !sql);
        aWhere.push(retornoValida);
//        aWhere = $.extend(aWhere, retornoValida);
    }
    if ($.isEmptyObject(aWhere))
    {
        return false;
    }

    if (sql !== undefined | !sql)
    {
        oldWhere = aWhere;
        aWhere = {};
        aWhere["columns"] = oldWhere;
    }
    return aWhere;
}


function jsCriaFiltro(filtro, column)
{
    var retornoValida = {};
    switch (filtro.tipoCampo)
    {
        case "DATA":
        case "DT":
            obj1 = "#" + filtro.obj1;
            if (filtro.obj2 !== null)
            {
                obj2 = "#" + filtro.obj2;
                retornoValida = jsValidaIntervaloData($(obj1), $(obj2), filtro.campoTabela, filtro.txtMensagem, column);
            } else {
                retornoValida = jsValidaIntervaloData($(obj1), null, filtro.campoTabela, filtro.txtMensagem, column);
            }
            break;
        case "SELECT":
            retornoValida = jsFiltroCampoSelect(filtro.obj1, filtro.campoTabela);
            break;
        case "TX":
        case "NU":
        case "TEXT":
            retornoValida = jsFiltroCampoText(filtro.obj1, filtro.campoTabela, column, filtro.txtMensagem);
    }
    return retornoValida;
}

function jsAjustarTamanhoTabelaRedimencionarPagina()
{
    heightConteudoTopo = $("#divTopo").height();
    if (heightConteudoTopo === undefined)
    {
        heightConteudoTopo = 0;
    }

    offSetTamanhoPagina = offSetTamanhoPagina + heightConteudoTopo;

    $(window).resize(function () {
        if (tabelaDados !== undefined & dataTableResize)
        {
            $("#" + tabelaDados).parents(".row").height($(".content-wrapper").height() - offSetTamanhoPagina);
        }
    });
}

/**
 * 
 * @param {type} parametrosInicio
 * @param {type} parametrosExtra
 * @returns {unresolved}
 */
function _jsTrataParametrosExtra(parametrosInicio, parametrosExtra)
{
    if (parametrosInicio.searching === undefined)
    {
        parametrosInicio.searching = false;
    }

    if (parametrosExtra === undefined)
    {
        if (parametrosExtra === null)
        {
            return parametrosInicio;
        }
        return parametrosInicio;
    }

    chaves = Object.keys(parametrosExtra);
    chaves.forEach(function (item, indice)
    {
        parametrosInicio[item] = parametrosExtra[item];
    });
    return parametrosInicio;
}



/**
 * 
 * @param {type} parametrosInicio
 * @param {type} parametrosExtra
 * @returns {undefined}
 */
function jsTrataParametrosExtra(parametrosInicio, parametrosExtra)
{
    if (parametrosInicio.searching === undefined)
    {
        parametrosInicio.searching = false;
    }

    if (parametrosExtra === undefined)
    {
        if (parametrosExtra === null)
        {
            return parametrosInicio;
        }
        return parametrosInicio;
    }

//    parametros = Object.assign(extraParameters, parametros);
//    if (parametros.pageLength === undefined)
//    {
//        parametros.pageLength = 10;
//    }
//
//    if (sort !== undefined)
//    {
//        parametros.aaSorting = sort;
//    }
//
    if (parametrosExtra.scrollCollapse !== undefined)
    {
        parametrosInicio.scrollCollapse = parametrosExtra.scrollCollapse;
    }

    if (parametrosExtra.scrollX !== undefined)
    {
        parametrosInicio.scrollX = parametrosExtra.scrollX;
    }

    if (parametrosExtra.scrollXInner !== undefined)
    {
        parametrosInicio.scrollXInner = parametrosExtra.scrollXInner;
    }

    if (parametrosExtra.scrollY !== undefined)
    {
        parametrosInicio.scrollY = parametrosExtra.scrollY;
    }

    if (parametrosExtra.info !== undefined)
    {
        parametrosInicio.info = parametrosExtra.info;
    }

    if (parametrosExtra.select !== undefined)
    {
        parametrosInicio.select = parametrosExtra.select;
    }

    if (parametrosExtra.ordering !== undefined)
    {
        parametrosInicio.ordering = parametrosExtra.ordering;
    }

    if (parametrosExtra.buttons !== undefined)
    {
        parametrosInicio.buttons = parametrosExtra.buttons;
    }

    if (parametrosExtra.dom !== undefined)
    {
        parametrosInicio.dom = parametrosExtra.dom;
    }


    if (parametrosExtra.paging !== undefined)
    {
        parametrosInicio.paging = parametrosExtra.paging;
    }

    //    parametros = Object.assign(extraParameters, parametros);
    //    if (parametros.pageLength === undefined)
    //    {
    //        parametros.pageLength = 10;
    //    }
    //
    //    if (sort !== undefined)
    //    {
    //        parametros.aaSorting = sort;
    //    }
    //

    if (parametrosExtra.initComplete)
    {
        parametrosInicio.initComplete = parametrosExtra.initComplete;
    }

    if (parametrosExtra.select !== undefined)
    {
        parametrosInicio.select = parametrosExtra.select;
    }

    if (parametrosExtra.searching !== undefined)
    {
        parametrosInicio.searching = parametrosExtra.searching;
    }
    if (parametrosExtra.ordering !== undefined)
    {
        parametrosInicio.ordering = parametrosExtra.ordering;
    }

    if (parametrosExtra.paging !== undefined)
    {
        parametrosInicio.paging = parametrosExtra.paging;
    }

    if (parametrosExtra.oPaginate === undefined)
    {
        if (parametrosInicio.language !== undefined)
        {
            if (parametrosInicio.language.oPaginate !== undefined)
            {
                parametrosInicio.language.oPaginate = {"sNext": "Próximo", "sPrevious": "Anterior", "sFirst": "Primeiro", "sLast": "Último"};
            }
        }
    } else {
        parametrosInicio.language.oPaginate = parametrosExtra.oPaginate;
    }

    return parametrosInicio;
}


/**
 * Inicializa uma tabela com Datatables
 * 
 * @param {string} tabelaId ID da tabela
 * @param {string} url (opcional) Usado apenas para processamento server side
 * @param {array} colunas (opcional) Server Side (Nome nas colunas correspondentes na base de dados)
 * @param {object} columnDefs {optional} Objeto json com as funções de callback para tratar a formatação e apresentação de celulas individuais nas colunas 
 * @param {object} where (opcional) Objeto json com clausulas where para consulta ex: {"usuario_nome LIKE" : "A%"}
 * @param {object} sort {optional} Objeto json com as funções de callback para tratar a formatação e apresentação de celulas individuais nas colunas
 * @param {object} createdRow
 * @param {int} rowId identificador Unico de Linha
 * @param {object} _extraParameters Parametros adicionais para a criação do datatable
 * @returns void
 */
function tableInit(tabelaId, url, colunas, columnDefs, where, sort, createdRow, rowId, _extraParameters) {
    parametros = jsParametrizacaoPadrao(url, colunas, columnDefs, where, rowId, createdRow, sort);
    // Trata os Parametros Especificos para o Datatable a ser criado
    parametros = jsTrataParametrosExtra(parametrosPadraoDataTable, _extraParameters);
    oTable = $("#" + tabelaId).DataTable(parametros);
    return oTable;
    //return $("#" + tabelaId).DataTable(parametros);
}

function JSvisualizaDoc(CaseNum, ProcId)
{
//Auxiliar = window.open("editcase?ProcId=" + ProcId + "&CaseNum=" + CaseNum + "&TipoDoc=CT&Fechar=1&Action=View", "Auxiliar", "scrollbars=1,resizable=0, width=" + width + ",height= " + height + ", left=" + 100 + ",top=" + 100);
    if (typeof janelaAuxiliar === "object")
    {
        janelaAuxiliar.jsLimpaAcaoVoltar();
    }
    janelaAuxiliar = window.open("", "Auxiliar", "scrollbars=1,resizable=0, width=" + janelaAuxiliarWidth + ",height= " + janelaAuxiliarheight + ", left=" + 100 + ",top=" + 100);
    dadosVisualizar = "ProcId=" + ProcId + "&CaseNum=" + CaseNum + "&TipoDoc=CT&Fechar=1&Action=View";
    jsCriaFormVisualizar("viewcase", dadosVisualizar);
    janelaAuxiliar.focus();
}

/**
 * 
 * @param {type} paginaAcao
 * @param {type} data
 * @param {type} janelaDestino
 * @returns {undefined}
 */
function jsCriaFormVisualizar(paginaAcao, data, janelaDestino)
{
// Create a form
    var mapForm = document.createElement("form");
    //mapForm.target = "Auxiliar";
    mapForm.target = "Auxiliar";
    mapForm.method = "POST";
    mapForm.action = paginaAcao;
    $aDadosPost = data.split("&");
    for (i = 0; i < $aDadosPost.length; i++)
    {
        jsCriaElementoForm($aDadosPost[i], mapForm);
    }

// Add the form to dom
    document.body.appendChild(mapForm);
// Just submit
    mapForm.submit();
    document.body.removeChild(mapForm);
}

function jsCriaElementoForm(elemento, form)
{
    aDadosCampo = elemento.split("=");
    var elementoForm = document.createElement("input");
    elementoForm.type = "text";
    elementoForm.name = aDadosCampo[0];
    elementoForm.value = aDadosCampo[1];
    form.appendChild(elementoForm);
}
