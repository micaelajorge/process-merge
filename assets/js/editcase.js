// //Versao 1.2.0 /Versao
// JavaScript Document
var menuwidth = 110;
var offsetleft = 350;
var offsettop = 50;
var IdCampo = 0;
var IdCampo = 0;
var IdTCampo = 0;
var IdRCampo = 0;
var EditingDoc = true;
var WinSelect = 0;
var PosX;
var PosY;
var mostrar = true;
var TextoToolTip;
var AdHoc_oldOnclick;
var AdHoc_Status = false;
var EncerrarAdHoc = true;
var StepIdNovoCaso;
var janelaAuxiliarWidth = 780;
var janelaAuxiliarheight = 550;

function acaoAposUpload()
{

}

// Objeto para SCANNER
var DWObject;

if (typeof Validacao === "undefined")
{
    var Validacao = new Object();
}

var janelaImagemFormalizacao;
var extendDataLoaded = false;

function jsFieldFileAcquireImage() {
    DWObject = Dynamsoft.WebTwainEnv.GetWebTwain('dwtcontrolContainer');
    if (DWObject) {
        DWObject.SelectSource(function () {
            var OnAcquireImageFailure = function () {
                DWObject.CloseSource();
            };
            var OnAcquireImageSuccess = function () {
//              $("#btnSendScanFile").removeAttr("disabled");
                $("#btnSendScanFile").removeAttr("disabled");
                DWObject.CloseSource();
            };
            DWObject.OpenSource();
            DWObject.IfDisableSourceAfterAcquire = true;
            DWObject.AcquireImage(OnAcquireImageSuccess, OnAcquireImageFailure);
            $("#btnRemoveScanFiles").removeAttr("disabled");
        }, function () {
            console.log('SelectSource failed!');
        });
    }
}


function jsCarregaFieldSelectFile(ProcId, CaseNum, FieldId, valueId, userId)
{
    $("#DivFolderFiles").hide();
    $("#DivFolderSelFile").show();
}

/**
 * 
 * @returns {undefined}
 */
function jsMontaCamposFormularioEnvioScan()
{
    // Limpa o Formulario interno do Objeto TWAIN
    DWObject.ClearAllHTTPFormField();
    // Inclui Campo do ProcId
    procIdPai = $("#procId").val();
    DWObject.SetHTTPFormField("procId", procIdPai);
    StepIdForm = $("#stepIdForm").val();
    DWObject.SetHTTPFormField("stepIdForm", StepIdForm);
    DWObject.SetHTTPFormField("procId", procId);
    camposFormularioWebCapture = getFormValuesObjectList("formDataCaptura");
    camposFormularioWebCapture.forEach(function (campo, indice) {
        DWObject.SetHTTPFormField(campo["name"], campo["value"]);
    });
}

function jsFileUploadScan(formSource, ProcId, StepId, CaseNum, FieldId, UserId) {
    if (DWObject) {

        //Monta o Formulário com dados para upload
        DWObject.SetHTTPFormField("ProcId", ProcId);
        DWObject.SetHTTPFormField("CaseNum", CaseNum);
        DWObject.SetHTTPFormField("FieldId", FieldId);
        DWObject.SetHTTPFormField("UserId", UserId);



        var strHTTPServer = location.hostname; //The name of the HTTP server. 
        var CurrentPathName = unescape(location.pathname);
        var CurrentPath = CurrentPathName.replace(/editcase.*/, "");
        var strActionPage = CurrentPath + "foldersendfile";


        DWObject.IfSSL = location.protocol === 'https' // Set whether SSL is used
//        DWObject.HTTPPort = location.port == "" ? 80 : location.port;

        totalPaginasScan = DWObject.HowManyImagesInBuffer
        DWObject.SelectedImagesCount = totalPaginasScan;

        for (i = 0; i < totalPaginasScan; i++)
        {
            DWObject.SetSelectedImageIndex(i, i);
        }
        DWObject.GetSelectedImagesSize(4); // 4 - PDF format. Calculate the size of selected images in PDF format.

        acaoAposUpload = () => {
            jsCarregaFolder(ProcId, StepId, CaseNum, FieldId, 0);
        };


//        // Upload the selected images to the server asynchronously
        DWObject.HTTPUploadThroughPostAsMultiPagePDF(
                strHTTPServer,
                strActionPage,
                "imageData.pdf",
                OnHttpUploadSuccess,
                OnHttpUploadFailureFieldFiles
                );
//        DWObject.HTTPUploadThroughPost(
//                strHTTPServer,
//                DWObject.CurrentImageIndexInBuffer,
//                strActionPage,
//                "imageData.pdf",
//                () => { 
//                    jsCarregaFolder(ProcId, StepId, CaseNum, FieldId, 0);
//                },
//                OnHttpUploadFailure
//                );
    }
}

function OnHttpUploadFailureFieldFiles(errorCode, errorString, sHttpResponse) {
    if (errorCode == -2003)
    {
        acaoAposUpload();
    } else {
        alert(errorString + sHttpResponse);
    }
}


function jsCarregaFieldWebCaptura(ProcId, CaseNum, FieldId, valueId, userId)
{
    // Indica se a função deve carregar o modal, ou esperar o TWAIN ser carregado
    mostraWebCapture = false;
    if (typeof Dynamsoft === "undefined")
    {
        // carrega Scripts Webcaptura
        url = "Resources/" + dynamic_twain_config;
        $.ajax({
            url: url,
            dataType: "script"
        });

    }
    // carrega Scripts Webcaptura
    // carrega Scripts Webcaptura

    url = "Resources/dynamsoft.webtwain.initiate.js";
    $.ajax({
        url: url,
        dataType: "script"
    });

    $("#DivFolderFiles").hide();
    $("#DivFolderScanFile").show();
}

function jsExecutaRoleValidarFormulario(roleCode, callBack)
{
    url = "api/v1/rules/1/" + roleCode;
    dadosEnvio = jsPegaDadosFormByCode('frmEditarCaso');
    $.ajax(
            {
                url: url,
                type: "POST",
                data: dadosEnvio,
                dataType: 'json'
            })
            .done(function (dataRetorno) {
                if (dataRetorno.resultado)
                {
                    jsMudaStatusDocumentos(dataRetorno);
                    callBack();
                } else {
                    alert(dataRetorno.retornoMensagem + "\n" + dataRetorno.rouleCode);
                }
            }
            )
            .fail(function (jqXHR, textStatus) {
                alert(textStatus);
            });
}

function jsExecutaRoleDocumentosObrigatorios(roleCode, callBack)
{
    $("#documentos_processo_icone").addClass("fa-spin");
    url = "api/v1/rules/1/" + roleCode;
    dadosEnvio = jsPegaDadosFormByCode('textos');
    $.ajax(
            {
                url: url,
                type: "POST",
                data: dadosEnvio,
                dataType: 'json'
            })
            .done(function (dataRetorno) {
//                if (dataRetorno.resultado)
//                {
//                    jsMudaStatusDocumentos(dataRetorno);
//                } else {
//                    alert(dataRetorno.retornoMensagem + "\n" + dataRetorno.rouleCode);
//                }
                jsMudaStatusDocumentos(dataRetorno);
                $("#documentos_processo_icone").removeClass("fa-spin");
            }
            )
            .fail(function (jqXHR, textStatus) {
                alert(textStatus);
                $("#documentos_processo_icone").removeClass("fa-spin");
            });
}


function jsLimpaTodosStatusDocumentos(listaDocumentosObrigatorios)
{
    listaDocumentosExistentes = $("[aria-document-code]").not("[aria-documento-obrigatorio='true']");
    for (i = 0; i < listaDocumentosExistentes.length; i++)
    {
        if (listaDocumentosObrigatorios.indexOf(listaDocumentosExistentes[i]) > -1)
        {
            continue;
        }
        jsMudaClasses(listaDocumentosExistentes[i], false);
    }
}

function jsMudaStatusDocumentos(dadosRetorno)
{
    if (typeof dadosRetorno.dados === "undefined")
    {
        return;
    }
    listaDocumentos = dadosRetorno.dados;
    jsLimpaTodosStatusDocumentos(listaDocumentos);
    for (i = 0; i < listaDocumentos.length; i++)
    {
        jsMudaStatusDocumento(listaDocumentos[i].DOCUMENTOS, true);
    }
}

function jsMudaStatusDocumento(codigoDocumento, obrigatorio)
{
    jsMudaClasses($("[aria-document-code='" + codigoDocumento + "']"), obrigatorio);
}

function jsMudaClasses(objeto, obrigatorio)
{
    totalDocs = parseInt($(objeto).find(".total-imagens").html());
    if (obrigatorio)
    {
        $(objeto).addClass("box-solid");
        if (totalDocs === 0)
        {
            $(objeto).addClass("box-warning");
        } else {
            $(objeto).addClass("box-success");
        }
    } else {
        $(objeto).removeClass("box-warning");
        $(objeto).removeClass("box-solid");
        if (totalDocs > 0)
        {
            $(objeto).addClass("box-success");
        }
    }
}

function jsPegaDadosFormByCode(formDados)
{
    dadosRetorno = {};
    camposFormulario = $("#" + formDados).find("[aria-code]");
//    camposFormulario = $("#" + formDados).find("[aria-code]");
    for (i = 0; i < camposFormulario.length; i++)
    {
        campo = camposFormulario[i];
        if ($(campo).attr("type") === "checkbox")
        {
            valorCampo = $(campo).attr("checked") == "true" ? "1" : "0";
        } else {
            valorCampo = $(campo).val();
        }

        typeConversion = $(campo).attr("aria-conversion");
        switch (typeConversion)
        {
            case "JSON":
                if (valorCampo === "")
                {
                    valorCampo = [];
                } else {
                    valorCampo = JSON.parse(valorCampo);
                }
                break;
        }
        //        valor = valorCampo.replace(/"/g, '\\"');

        dadosRetorno[$(campo).attr("aria-code")] = valorCampo;
//        console.log(+ " : " + valorCampo);
    }
    return dadosRetorno;
}

function jsSelectFiles()
{
    $("#spanTextoEnvioArquivo").html("Aguardando Arquivos");
    $("#divOverlaySendFile").show();
    $('#fileMultiple').click();
    $("#btnSelFiles").on("focus", jsBackFromFiles);
}

function jsBackFromFiles()
{
    $("#divOverlaySendFile").hide();
    $("#btnSelFiles").on("focus", null);
}

function jsArquivoSelecionado(idInputFile)
{
    //Este evento é chamado cada vez que a operação de leitura é completada com sucesso.
    var reader = new FileReader();
    canvasTarget = $("#pdfPreviewFile");
    if (['application/pdf'].indexOf($("#" + idInputFile).get(0).files[0].type) == -1) {
        reader.onload = function (e) {
            $('#imgPreviewFile').attr('src', e.target.result);
            $("#imgPreviewFile").show();
            $("#pdfPreviewFile").hide();
        };
        reader.readAsDataURL($("#" + idInputFile)[0].files[0]);
    } else {
        showPreviwPDF(URL.createObjectURL($("#" + idInputFile).get(0).files[0]));
        if ($('#' + idInputFile).prop('files').length > 0)
        {
            $("#totalArquivosSel").html($('#' + idInputFile).prop('files').length + " arquivos selecionados");
        } else {
            $("#totalArquivosSel").html($('#' + idInputFile).prop('files')[0].name);
        }
        $("#totalArquivosSel").html();
        $("#imgPreviewFile").hide();
        $("#pdfPreviewFile").show();
    }
    $(".btn_executa_upload").removeAttr("disabled");
}

function jsEscondeViewImages(docId)
{
    $("#divImagens" + docId).hide();
}

function jsCreateTumbsPDF(docId)
{
    listaCanvasObj = $("#BODY_" + docId).find('[aria-url]');
    if (typeof listaImagensPDF === "undefined")
    {
        listaImagensPDF = [];
    }
    for (i = 0; i < listaCanvasObj.length; i++)
    {
        item = listaCanvasObj[i];
        var urlPDF = $(item).attr("aria-url");
        var idIMG = $(item).attr("id");
        dadosImagem = {url: urlPDF, objImagem: idIMG};
        $(item).removeAttr("aria-url");
        listaImagensPDF.push(dadosImagem);
    }
    jsEncadeiaPDF();
}

function jsViewFilesDocCT(procDoc, docId, procId, caseNum)
{
    // Mostra os documentos se já foi carregado
    if (typeof $("#BODY_" + docId).html() !== "undefined")
    {
        $("#divImagens" + docId).show();
        return;
    }
    acaoDocumentos = $("#textos").find('[name="Acao"]').val();
    url = "documentosprocesso_list";
    dadosChamada = {
        CaseNum: caseNum,
        ProcId: procId,
        procDoc: procDoc,
        docId: docId,
        acaoDocumentos: acaoDocumentos
    };
    $.post(url, dadosChamada,
            function (dataRetornados) {
                $("#divImagens" + docId).html(dataRetornados);
                jsCreateTumbsPDF(docId);
            });
}

/**
 * 
 * @returns {undefined}
 */
function jsCarregaDocumentosProcesso()
{
//    if (typeof tipoProcesso === "undefined")
//    {
//        return;
//    }
//    if (tipoProcesso !== "CT")
//    {
//        return;
//    }

    $("#documentos_processo_icone").addClass("fa-spin");
    ExecutandoAcao = 1;
    procId = $("#textos").find('[name="ProcId"]').val();
    stepId = $("#textos").find('[name="t5"]').val();
    caseNum = $("#textos").find('[name="CaseNum"]').val();
    acaoDocumentos = $("#textos").find('[name="Acao"]').val();
    dadosEnvio = {
        procId: procId,
        stepId: stepId,
        caseNum: caseNum,
        acaoDocumentos: acaoDocumentos
    };

    url = "documentosprocesso";

    $.ajax(
            {
                url: url,
                type: "POST",
                data: dadosEnvio,
                dataType: 'html',
                success: function (resultado) {
                    $("#divDocumentosProcesso").html(resultado);
                    procCod = $("#ProcCode").val();
                    stepCod = $("#StepCode").val();
                    jsExecutaRoleDocumentosObrigatorios(procCod + "_" + stepCod, null);
                    $("#documentos_processo_icone").removeClass("fa-spin");
                    ExecutandoAcao = 0;
                }
            });
}

function jsAnexarArquivoRP()
{

}

function jsAlteraStatusCaso(SITUACAO_ID, SITUACAO_TEXTO)
{
    $("#t6").val(SITUACAO_ID);
    $("#SITUACAO_ATUAL_TOPO").html(SITUACAO_TEXTO);
}

function jsAbreModalEditarDadosCaso(ProcId, CaseNum)
{
    StepId = $("#t5").val();
    if (StepId == 0)
    {
        StepId = $("#StepId").val();
    }
    JSmontaFormulario(ProcId, CaseNum, StepId, "divFormEdit", 'Edit', "", "", "", "col-lg-6");
    $("#btnSalvarModalEditDadosCaso").attr("disabled", "true");
    $("#modalEditDadosCaso").modal('show');
}

function jsDefineonClose()
{
    window.onbeforeunload = confirmExit;
    document.onunload = confirmExit;
}

function confirmExit()
{
    return "Alterações não salvas serão perdidas";
}

function jsCarregaDadosExtendidosCaso(procId, stepId, caseNum)
{
    if (extendDataLoaded)
    {
        return;
    }

    dadosEnvio = {
        procId: procId,
        stepId: stepId,
        caseNum: caseNum
    };

    $.ajax({
        type: "POST",
        url: "dadosextended/",
        data: dadosEnvio
    }).done(function (dadosRetorno) {
        $("#divDadosExtended").html(dadosRetorno);
    });
}


function getFormValuesObjectList(form)
{
    str = {};
    var Valores = new Array;
    var fobj = document.forms[form];
    for (var i = 0; i < fobj.elements.length; i++)
    {
        if (fobj.elements[i].disabled === true)
        {
            continue;
        }
        switch (fobj.elements[i].type)
        {
            case "text":
            case "textarea":
            case "hidden":
                if (fobj.elements[i].value === "")
                {
                    continue;
                }
                str["name"] = fobj.elements[i].name;
                str["value"] = escape(fobj.elements[i].value);
                break;
            case "checkbox":
                if (!fobj.elements[i].checked)
                {
                    continue;
                }
                str["name"] = fobj.elements[i].name;
                str["value"] = escape(fobj.elements[i].value);
                break;
            case "select-one":
                str["name"] = fobj.elements[i].name;
                str["value"] = fobj.elements[i].options[fobj.elements[i].selectedIndex].value;
                break;
        }
        Valores.push(str);
        str = {};
    }
    return Valores;
}


function jsCarregaFolder(ProcId, StepId, CaseNum, Campo, ReadOnly, multipleFiles)
{
    console.log("Carregando Folder");
    url = "folderlist";
    dadosChamada = {
        CaseNum: CaseNum,
        ProcId: ProcId,
        StepId: StepId,
        FieldId: Campo,
        ReadOnly: ReadOnly,
        MultipleFiles: multipleFiles
    };
    $.post(url, dadosChamada,
            function (dataRetornados) {
                var conteudo = document.getElementById('DivModalFD');
                conteudo.innerHTML = dataRetornados;
            });
}

function jsAtualizaCampoListaArquivos(ProcId, CaseNum, Campo, Total)
{
    // Atualiza o display de total de arquivos
    $("#nrArquivos" + Campo).html(Total);
    // Atualiza o Campo Hidden de total de arquivos
    $("#r" + Campo).val(Total);

    url = "folderlist";
    dadosChamada = {
        CaseNum: CaseNum,
        ProcId: ProcId,
        FieldId: Campo,
        ReadOnly: 1,
        tipoFolderList: "botao"
    };
    $.post(url, dadosChamada,
            function (dataRetornados) {
                var conteudo = document.getElementById('listaArquivos' + Campo);
                conteudo.innerHTML = dataRetornados;
            });
}

function jsCasoBloqueado(dadosRecebidos)
{
    $("#desbloqueioModal").on('hide.bs.modal', function () {
        return false;
    });
    userId = $("#userId").val();
    if (dadosRecebidos.bloqueio != userId)
    {
        window.onbeforeunload = null;
        document.onunload = null;
        $("#desbloqueioModal").modal();
    }
}

function jsbuscaBloqueioCaso()
{
    procId = $("#ProcId").val();
    stepId = $("#StepId").val();
    caseId = $("#CaseNum").val();
    dadosEnvio = {
        "procId": procId,
        "stepId": stepId,
        "caseNum": caseId
    };
    $.ajax({
        type: "POST",
        url: "api/casobloqueado/",
        context: document.body,
        data: dadosEnvio
    }).done(function (dadosRetorno) {
        //jsCasoBloqueado(dadosRetorno);
    });
}

function jsDevolveObjetoParaJanela(objOrigem)
{
    console.log($("#divVisualizacaoFormalizacao"));
    $("#divWrapEditar").addClass("col-lg-6");
    $("#divWrapEditar").removeClass("col-lg-12");
    $("#divVisualizacaoFormalizacao")[0].appendChild(objOrigem.document.getElementById("divCanvasFORMALIZACAO"));
    $(iconeBotao).addClass("fa-expand");
    $(iconeBotao).removeClass("fa-compress");
    $(iconeBotao).parent("button").attr("onclick", "jsAbreJanelaFormalizacao()");
    $(iconeBotao).parent("button").attr("title", "Abrir em outra janela");
    objOrigem.close();
}

function jsMoveObjetoParaJanelaFormalizacao(janelaDestino)
{
    objDestino = janelaDestino.document.getElementById("sectionPage");
    iconeBotao = $(".fa-expand");
    $(iconeBotao).removeClass("fa-expand");
    $(iconeBotao).addClass("fa-compress");
    $(iconeBotao).parent("button").attr("onclick", "window.opener.jsDevolveObjetoParaJanela(window)");
    $(iconeBotao).parent("button").attr("title", "Voltar para janela");
    var objeto = $('#divCanvasFORMALIZACAO')[0];
    objDestino.appendChild(objeto);
    $("#divWrapEditar").removeClass("col-lg-6");
    $("#divWrapEditar").addClass("col-lg-12");
    janelaDestino.canvas = janelaDestino.document.getElementsByTagName('canvas')[0];
    janelaDestino.ctx = janelaDestino.canvas.getContext('2d');
    janelaDestino.trackTransforms(janelaDestino.ctx);
}

function jsAbreJanelaFormalizacao()
{
    leftNewWindow = 1800;
    janelaImagemFormalizacao = window.open('viewformalizacao', 'imagens', "width=400, height = 400, top=0, left=" + leftNewWindow);
}


function jsMudaTextoDivProcura(objeto)
{
    idObjetoTexto = $(objeto).attr("aria-campo");
    if (idObjetoTexto !== "")
    {
        objetoDados = $("[aria-code='" + idObjetoTexto + "']");
        valorTexto = objetoDados.val();
        nomeCampo = objetoDados.attr("aria-label");
        $("#divTextoProcura").html(nomeCampo + ": " + valorTexto);
    }
}

function jsMudaStatusButtonBOCheck(objeto, checkBoxAlvo)
{
    if ($(objeto).attr("aria-reverso") === "0")
    {
    } else {
        if ($(objeto).hasClass("btn-success"))
        {
            $("#" + checkBoxAlvo).val("1");
            $(objeto).removeClass("btn-success");
            $(objeto).addClass("btn-danger");
            $(objeto).find("i").removeClass();
            $(objeto).find("i").addClass("fa fa-close");
        } else
        {
            $("#" + checkBoxAlvo).val("0");
            $(objeto).removeClass("btn-danger");
            $(objeto).addClass("btn-success");
            $(objeto).find("i").removeClass();
            $(objeto).find("i").addClass("fa fa-check");
        }
    }
}

function jsMudaStatusButtonBO(objeto, checkBoxAlvo)
{
    if ($(objeto).attr("aria-reverso") === "0")
    {
        if ($(objeto).hasClass("btn-success"))
        {
            $("#" + checkBoxAlvo).val("0");
            $(objeto).removeClass("btn-success");
            $(objeto).addClass("btn-danger");
            $(objeto).html("Não");
        } else
        {
            $("#" + checkBoxAlvo).val("1");
            $(objeto).removeClass("btn-danger");
            $(objeto).addClass("btn-success");
            $(objeto).html("Sim");
        }
    } else {
        if ($(objeto).hasClass("btn-success"))
        {
            $("#" + checkBoxAlvo).val("1");
            $(objeto).removeClass("btn-success");
            $(objeto).addClass("btn-danger");
            $(objeto).html("Não");
        } else
        {
            $("#" + checkBoxAlvo).val("0");
            $(objeto).removeClass("btn-danger");
            $(objeto).addClass("btn-success");
            $(objeto).html("Sim");
        }
    }
}


function jsFieldFileRemoveImages()
{
    if (confirm('Descartar imagens'))
    {
        DWObject.RemoveAllImages();
    }
}

function jsRemoveFile(procId, caseNum, fieldId, valueId, fileName)
{
    if (!confirm("Remover o arquivo?"))
    {
        return;
    }
    urlExecutao = `removefile/${procId}/${caseNum}/${fieldId}/${valueId}/${fileName}`;
    $.ajax({
        xhrFields: {
            onprogress: function (e) {
                if (e.lengthComputable) {
                    console.log("Loaded " + Number((e.loaded / e.total * 100)) + "%");
                } else {
                    console.log("Length not computable.");
                }
            }
        },
        url: urlExecutao,
        type: "GET",
        success: function (retorno) {
            jsCarregaFolder(procId, '', caseNum, fieldId, 0, 0);
        },
        error: function (xhr, status, error) {
            alert(status);
        }
    });
    console.log(`${procId}, ${caseNum}, ${fieldId}, ${valueId}`);

}

function jsViewFile(procId, caseNum, fieldId, fileNameStorage, fileName, fieldIdCase, abrirNovaJanela)
{
    if (fileName === "")
    {
        if (typeof fieldIdCase !== "undefined")
        {
            jsonDadosArquivo = $("#t" + fieldIdCase).attr("aria-file-data");
            caseNum = $("#r" + fieldIdCase).val();
            if (typeof jsonDadosArquivo === "undefined")
            {
                jsonDadosArquivo = $("#r" + fieldIdCase).attr("aria-file-data");
                caseNum = $("#r" + fieldIdCase).val();
            }

        } else {
            jsonDadosArquivo = $("#r" + fieldId).val();
            if (typeof jsonDadosArquivo === "undefined")
            {
                jsonDadosArquivo = $("#t" + fieldId).val();
            }
        }
        dados = JSON.parse(jsonDadosArquivo);
        fileNameStorage = dados.fileNameStorage;
        fileName = dados.fileName;
    }

    janelaDestino = !!abrirNovaJanela;
    if (janelaDestino)
    {
        janelaDestino = "Auxiliar" + fieldId + fileName;
        Auxiliar = window.open("", janelaDestino, "scrollbars=1,resizable=0, width=" + janelaAuxiliarWidth + ",height= " + janelaAuxiliarheight + ", left=" + 100 + ",top=" + 100);
    }

    dadosVisualizar = "ProcId=" + procId + "&CaseNum=" + caseNum + "&fieldId=" + fieldId + "&fileNameStorage=" + fileNameStorage + "&fileName=" + fileName;
    jsCriaFormVisualizar("viewfile/" + fileName, dadosVisualizar, janelaDestino);
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

    if (!!janelaDestino)
    {
        mapForm.target = janelaDestino;
    } else {
        mapForm.target = "_blank";
    }

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


function jsValidacaoFormulario()
{
    return ValidacaoArray(Validacao);
}

var EsconderOpcao;
function jsAtualizaCampoArquivo(fieldId, nomeArquivo, dadosArquivo)
{
    $("#divBoxAcaoEnviarArquivo").show();
    $("#OverlayWaitingSendFile").hide();
    $("#I" + fieldId).val(nomeArquivo);
    $("#t" + fieldId).val(dadosArquivo);
    $("#Remove" + fieldId).removeAttr("disabled");
    $("#Link" + fieldId).removeAttr("disabled");
    $('.close').click(); // Esconde o Modal

}

function jsAjaxUploadOld(idForm, ProcId, StepId, CaseNum, Campo)
{
    var formObj = document.getElementById(idForm);
    var formData = new FormData(formObj);
    $.ajax({
        xhrFields: {
            onprogress: function (e) {
                if (e.lengthComputable) {
                    console.log("Loaded " + Number((e.loaded / e.total * 100)) + "%");
                } else {
                    console.log("Length not computable.");
                }
            }
        },
        url: "foldersendfile",
        type: "POST",
        data: formData,
        dataType: 'html',
        processData: false,
        contentType: false,
        success: function (retorno) {
            alert(retorno);
            jsCarregaFolder(ProcId, StepId, CaseNum, Campo, 1);
        },
        error: function (xhr, status, error) {
            alert(status);
        }
    });
}

var contadorArquivosUpload = 0;
var listaArquivosUpload = [];
function jsMultipleUpload(idForm, ProcId, StepId, CaseNum, Campo, paginaEnvioArquivo, multipleFiles)
{
    var novoFormObj = document.getElementById(idForm);
    var formDataUpload = new FormData(novoFormObj);
    $(formDataUpload).find('file').remove();
    formDataUpload.append('file', listaArquivosUpload[contadorArquivosUpload]);
    $.ajax({
        //url: "foldersendfile",
        url: paginaEnvioArquivo,
        type: "POST",
        data: formDataUpload,
        xhr: function () {
            var xhr = $.ajaxSettings.xhr();
            xhr.upload.onprogress = function (e) {
                // For uploads
                if (listaArquivosUpload.length > 1)
                {
                    porcentagemCarregado = Math.round(contadorArquivosUpload / listaArquivosUpload.length * 100);
                } else {
                    if (e.lengthComputable) {
                        porcentagemCarregado = Math.round(e.loaded / e.total * 100);
                    }
                }
                jsAtualizaBarradeProgresso("progressBarFolder", "porcentagemValorFolder", porcentagemCarregado);
            };
            return xhr;
        },
        dataType: 'html',
        processData: false,
        contentType: false,
        success: function (retorno) {
            if (contadorArquivosUpload <= listaArquivosUpload.length)
            {
                $("#spanDadosArquivoUpload").html(contadorArquivosUpload + " / " + listaArquivosUpload.length);
                contadorArquivosUpload++;
                jsMultipleUpload(idForm, ProcId, StepId, CaseNum, Campo, paginaEnvioArquivo, multipleFiles);
            } else {
                jsCarregaFolder(ProcId, StepId, CaseNum, Campo, 0);
            }
        },
        error: function (xhr, status, error) {
            alert(status);
        }
    });
}

function jsAjaxUploadToFilesList(idForm, ProcId, StepId, CaseNum, Campo, MultipleFiles)
{
    $("#divBotoesAcao").hide();
    $("#divOverlaySendFile").show();
    $("#progressBarFolder").show();
    listaArquivosUpload = $('#fileMultiple').prop('files');
    contadorArquivosUpload = 0;


//    Estava enviando sempre para sendfiletodir mesmo não sendo um campo Diretorio
//    
//    if (listaArquivosUpload.length > 1 | MultipleFiles)
//    {
//        paginaUpload = "sendfiletodir";
//    } else {
//        paginaUpload = "foldersendfile";
//    }

    if (MultipleFiles == "0")
    {
        paginaUpload = "foldersendfile";
    } else {
        paginaUpload = "sendfiletodir";
    }
    jsMultipleUpload(idForm, ProcId, StepId, CaseNum, Campo, paginaUpload);
}

/**
 * 
 * @param {type} objBarraProgresso
 * @param {type} objTextoPorcentagem
 * @param {type} porcentagemCarregado
 * @returns {undefined}
 */
function jsAtualizaBarradeProgresso(objBarraProgresso, objTextoPorcentagem, porcentagemCarregado)
{
    $("#" + objBarraProgresso).css("width", porcentagemCarregado + "%");
    $("#" + objTextoPorcentagem).html(porcentagemCarregado);
}

/**
 * 
 * @param {type} idForm
 * @param {type} Campo
 * @returns {undefined}
 */
function jsUploadFile(idForm, Campo)
{
    var novoFormObj = document.getElementById(idForm);
    var formDataUpload = new FormData(novoFormObj);
    $("#divBoxAcaoEnviarArquivo").hide();
    $("#OverlayWaitingSendFile").show();
    $("#progressBarFile").show();
    $.ajax({
        xhr: function () {
            var xhr = $.ajaxSettings.xhr();
            xhr.onprogress = function e() {
                // For downloads
                if (e.lengthComputable) {
                    porcentagemCarregado = Math.round(e.loaded / e.total * 100);
                    jsAtualizaBarradeProgresso("progressBarFile", "porcentagemValorFile", porcentagemCarregado);
                }
            };
            xhr.upload.onprogress = function (e) {
                // For uploads
                if (e.lengthComputable) {
                    porcentagemCarregado = Math.round(e.loaded / e.total * 100);
                    jsAtualizaBarradeProgresso("progressBarFile", "porcentagemValorFile", porcentagemCarregado);
                }
            };
            return xhr;
        },
        url: "fileattachupload",
        type: "POST",
        data: formDataUpload,
        dataType: 'html',
        processData: false,
        contentType: false,
        upload: function (e) {
            $("#DivFolderSelFile").children(".modal-title").html(e.loaded);
        }
        ,
        success: function (retorno) {
            eval(retorno);
        }
        ,
        error: function (xhr, status, error) {
            alert(status);
        }
    }
    );
}

function jsValidaExtencao(idCampoFile)
{
    // Pega o nome do arquivo selecionado
    var nomeArquivoSelecionado = $('#' + idCampoFile).val();

    // Pega o Tipo de arquivo aceito
    tipoAceito = $('#' + idCampoFile).attr("accept");
    if (tipoAceito == '')
    {
        return true;
    }

    // pega o Nome do Arquivo
    fileName = nomeArquivoSelecionado.replace(/C:\\fakepath\\/i, '');

    // Extrai a extenção do arquivo
    var resultadoRegex = /^.+\.([^.]+)$/.exec(fileName);
    extencaoArquivo = (resultadoRegex === null) ? "" : resultadoRegex[1];

    // Existe nos tipos aceitos
    retorno = (tipoAceito.indexOf(extencaoArquivo) > -1);
    return retorno;
}

function jsValidaArquivoEnviado(idCampoFile, fileMax)
{
    tamanhoOk = jsVerificaTamanhoArquivo(idCampoFile, fileMax);
    extencaoOk = jsValidaExtencao(idCampoFile);
    if (tamanhoOk & extencaoOk)
    {
        $('#divNomeArquivo').removeClass("has-warning");
        $('#buttonEnviar')[0].disabled = false;
    } else {
        $('#divNomeArquivo').addClass("has-warning");
        $('#buttonEnviar')[0].disabled = true;
    }
}

function jsVerificaTamanhoArquivo(idCampoFile, fileMax)
{
    var file = $('#' + idCampoFile).val();
    fileName = file.replace(/C:\\fakepath\\/i, '');
    $('#nomeArquivo').val(fileName);
    size = $('#' + idCampoFile)[0].files[0].size;
    return (size < fileMax);
}

function JScarregaCampoCR(procId, fieldId, caseNum, fieldName)
{
    url = "callajax.php?script=montaCampoCR.inc&procId=" + procId + "&fieldId=" + fieldId + "&caseNum=" + caseNum + "&fieldName=" + fieldName;
    jsCarregaConteudoEmDiv(url, 'DivModal' + fieldId);
}

//function MudaEnterParaTab(Event)
//{
//    if (event.keyCode === 13)
//    {
//        TotalElementos = document.textos.elements.length;
//        for (i = 0; i < TotalElementos; i++)
//        {
//            if (document.textos.elements[i] === this)
//            {
//                if (TotalElementos > i)
//                {
//                    if (document.textos.elements[i + 1].type !== "hidden" && !document.textos.elements[i + 1].readOnly)
//                    {
//                        document.textos.elements[i + 1].focus();
//                        break;
//                    }
//                }
//            }
//        }
//        event.keyCode = 0;
//        if (i === TotalElementos)
//        {
//            Processar();
//        }
//    }
//    if (this.TipoCampo)
//    {
//        FormataData(this, event);
//    }
//}

function MudaEnterParaTab(Event)
{
    listaTiposCamposPermitidos = ["text", "select"];
    if (event.keyCode === 13)
    {
        TotalElementos = document.textos.elements.length;
        for (i = 0; i < TotalElementos; i++)
        {
            if (document.textos.elements[i] === this)
            {
                break;
            }
        }
        for (j = i; j < TotalElementos; j++)
        {
            if (TotalElementos > j)
            {
                tipoCampo = document.textos.elements[j + 1].type;
                $tipoPermiteFocus = listaTiposCamposPermitidos.indexOf(tipoCampo) > -1;
                if ($tipoPermiteFocus && !document.textos.elements[j + 1].readOnly)
                {
                    document.textos.elements[j + 1].focus();
                    break;
                }
            }
        }
    }
    event.keyCode = 0;
    if (i === TotalElementos)
    {
        Processar();
    }

    if (this.TipoCampo)
    {
        FormataData(this, event);
    }
}


function SetaEnterParaTab()
{
    if (typeof document.textos === 'undefined')
    {
        return;
    }
    TotalElementos = document.textos.elements.length;
    for (i = 0; i < TotalElementos; i++)
    {
        if (document.textos.elements[i].type === "select-one" | document.textos.elements[i].type === "text")
        {
            if (document.textos.elements[i].onkeypress === null)
            {
                document.textos.elements[i].onkeypress = MudaEnterParaTab;
            }
        }
    }
}

function setaFocusPrimeiroCampo(campo)
{
    try
    {
        setFocus = "document.textos.t" + campo + ".focus()";
        eval(setFocus);
    } catch (e)
    {
    }
}

function EscondeDivFullImage(campo)
{
    document.getElementById("DivShowFullImage" + campo).style.display = "none";
}

function MostraImagem(Dir, NomeImagem, campo, width, height, NrImagem, Total)
{
    SRC = "BpmWaterMarkImage.php?Imagem=" + Dir + NomeImagem;
    Conteudo = "<a href=\"#\" onclick=\"EscondeDivFullImage(" + campo + ")\">Fechar</a>";
    //Conteudo += "<div><a href=\"#\">Anterior</a> <a href=\"#\">Proxima</a></div>";
    Conteudo += "<div id=\"DivFullImageScrool" + campo + "\" class=\"FullImageScrool\">";
    Conteudo += "<img src=\"" + SRC + "\" /></div>";
    Conteudo += "<div style=\"text-align:center\">" + NomeImagem + "</div>";
    height = height + 50;
    if (height > document.documentElement.clientHeight)
    {
        height = document.documentElement.clientHeight - 100;
    }

    if (width > document.documentElement.clientWidth)
    {
        width = document.documentElement.clientWidth - 100;
    }
    document.getElementById("DivShowFullImage" + campo).style.height = height + "px";
    document.getElementById("DivShowFullImage" + campo).style.width = width + "px";
    document.getElementById("DivShowFullImage" + campo).style.display = ""
    document.getElementById("DivFullImage" + campo).innerHTML = Conteudo;
    document.getElementById("DivFullImageScrool" + campo).style.height = height + "px";
    document.getElementById("DivFullImageScrool" + campo).style.width = width + "px";
}

function CarregarImagens(ProcId, campo, FieldCode, CaseNum)
{
    EscondeDivFullImage(campo);
    url = "callajax.php?script=BPMLoadImagesFromFolder.php&ProcId=" + ProcId + "&campo=" + campo + "&FieldCode=" + FieldCode + "&CaseNum=" + CaseNum + "&LoadNewImages=1";
    jsCarregaConteudoEmDiv(url, "Div" + campo);
}

function ExecutaValidacao(FuncaoEval)
{
    retornoVal = eval(FuncaoEval);
    return retornoVal;
}


function ValidacaoArray(ItemsValidacao)
{
    var retornoVal = true;
    if (ItemsValidacao instanceof Object)
    {
        for (Indice in ItemsValidacao)
        {
            var Item = ItemsValidacao[Indice];
            retornoVal = retornoVal && ValidacaoArray(Item);
            if (!retornoVal)
            {
                break
            }
        }
    } else
    {
        var retExecuta = ExecutaValidacao(ItemsValidacao);
        retornoVal = retornoVal && retExecuta;
    }
    return retornoVal;
}

function MudaCheckBox(Object, Id)
{
    var Valor = 0
    if (Object.checked)
    {
        Valor = 1;
    }
    document.getElementById(Id).value = Valor;
}

function getposXY()
{
    PosX = event.clientX + document.body.scrollLeft;
    PosY = event.clientY + document.body.scrollTop;
}

function ftooltip(texto)
{
    window.status = texto;
    mostrar = false;
}

function fescondtooltip()
{
    document.getElementById("ToolTip").style.display = "none";
    TextoToolTip = '';
}

function Ajusta()
{
    document.getElementById("fundo3").style.height = document.body.clientHeight + "px";
    document.getElementById("fundo1").style.height = document.body.clientHeight + "px";
}

function MudaItemLista(Valor, Display)
{
    JSmudaDoc(Valor, Display)
    EscondeDivSelLista()
}

function ContaCaracteres(cols, rows, Objeto)
{
    if (cols === 0 | rows === 0)
    {
        return true;
    }
    tamanho = cols * rows;
    intCaracteres = tamanho - Objeto.value.length;
    if (intCaracteres < 0)
    {
        Objeto.value = Objeto.value.substr(0, tamanho);
        return false;
    }
    return true;
}

function AbrePagina(Pagina)
{
    WinSelect = window.open(Pagina, "Auxiliar", "toolbar=0,Location=0,directories=0,status=1,menubar=0,scrollbars=1,resizable=0,screenX=0,screenY=0,left=" + PegaPosX(800) + ",top=" + PegaPosY(600) + ",width=800,height=600");
    WinSelect.focus();
}

function MudaPrioridade(obj)
{
    document.textos.Prioridade.value = obj.selectedIndex;
}

function mostra(Objeto, imagem)
{
    document.getElementById(Objeto).src = imagem;
}

function sai(Acao)
{
    if (jsValidacaoFormulario(document.textos))
    {
        document.textos.Action.value = Acao;
        document.textos.submit();
        return;
    }
    return;
}

function apagar()
{
    if (confirm("Deseja apagar esta mensagem?!?"))
    {
        document.textos.Action.value = "Apagar";
        document.textos.submit();
        return;
    }
    return;
}

function setfocus(Campo)
{
    Campo.focus();
    return;
}

function VisualizaReferencias(ProcId, SourceField, Campo, CaseNum, nome, DisplayField)
{
    width = 780;
    height = 350;
    Auxiliar = window.open("flow0033.php?ProcSource=" + ProcId + "&FieldSource=" + SourceField + "&CaseNum=" + CaseNum + "&nome=" + nome + "&DisplayField=" + DisplayField, "Select", "resizable=1,scrollbars=1, width=" + width + " ,height=" + height + ", left=" + PegaPosX(width) + ",top=" + PegaPosY(height));
    Auxiliar.focus();
}

function JSvisualizaDoc(CaseNum, ProcId)
{
//Auxiliar = window.open("editcase?ProcId=" + ProcId + "&CaseNum=" + CaseNum + "&TipoDoc=CT&Fechar=1&Action=View", "Auxiliar", "scrollbars=1,resizable=0, width=" + width + ",height= " + height + ", left=" + 100 + ",top=" + 100);
    Auxiliar = window.open("", "Auxiliar", "scrollbars=1,resizable=0, width=" + janelaAuxiliarWidth + ",height= " + janelaAuxiliarheight + ", left=" + 100 + ",top=" + 100);
    dadosVisualizar = "ProcId=" + ProcId + "&CaseNum=" + CaseNum + "&TipoDoc=CT&Fechar=1&Action=View";
    jsCriaFormVisualizar("viewcase", dadosVisualizar, "Auxiliar");
    Auxiliar.focus();
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



function JSvisualizar(CaseNum, ProcId)
{
    width = 780;
    height = 550;
    Auxiliar = window.open("BPMEditCase.php?ProcId=" + ProcId + "&CaseNum=" + CaseNum + "&TipoDoc=CT&Fechar=1", "Auxiliar", "scrollbars=1,resizable=0, width=" + width + ",height= " + height + ", left=" + PegaPosX(width) + ", top=" + PegaPosY(height));
    Auxiliar.focus();
}

function JSselecionarDoc(valor, desc)
{
    JSmudaDoc(valor, desc);
}

function jsCarregaSelecaoCaso(procId, fieldIdSearch, fieldId, caseNum)
{
    IdTCampo = "t" + fieldId;
    IdRCampo = "r" + fieldId;
    url = "include/selCasoCampoDC.php?ProcSource=" + procId + "&FieldSource=" + fieldIdSearch + "&CaseSource=" + caseNum;
    dadosEnvio = {ProcSource: procId, FieldSource: fieldIdSearch, CaseSource: caseNum, fieldid: fieldId};
    $.post("selcasocampodc", dadosEnvio,
            function (data) {
                $("#DivModal" + fieldId).html(data);
                jsAcionaDataTableSelCaso(procId, fieldIdSearch, fieldId);
            });
}


function SelecionaOpcao(ProcId, Campo, Valor)
{
    IdTCampo = "t" + Campo;
    IdRCampo = "r" + Campo;
    url = "fieldlistsel?ProcId=" + ProcId + "&FieldId=" + Campo + "&Valor=" + Valor;
    xmlhttp1 = CriaAjax();
    xmlhttp1.open("GET", url, true);
    xmlhttp1.onreadystatechange = function ()
    {
        if (xmlhttp1.readyState == 4)
        {
            //L� o texto
            var texto = xmlhttp1.responseText

            //Desfaz o urlencode
            texto = texto.replace(/\+/g, " ");
            texto = unescape(texto);
            //Exibe o texto no div conte�do
            var master = document.getElementById("DivMasterLista");
            master.style.display = "";
            var conteudo = document.getElementById("DivConteudoLista");
            conteudo.innerHTML = texto;
            left = document.body.clientWidth / 2 - master.clientWidth / 2 + document.body.scrollLeft;
            var top = document.body.scrollTop + document.body.document.body.clientHeight / 2 - master.clientHeight / 2;
            master.style.top = top + "px";
            master.style.left = left + "px";
        }
    };
    xmlhttp1.send(null);
}

function JSmudaDoc(Valor, Display)
{
    document.getElementById(IdTCampo).value = Valor;
    document.getElementById(IdRCampo).value = Display;
    document.getElementById(IdRCampo + "BUTTON_VISUALIZAR").disabled = false;
    $('.close').click(); // Esconde o Modal
}

function MontaValidacaoFormulario(ProcId, StepId, Campo, Action)
{
    xmlhttpNew = CriaAjax();
    xmlhttpNew.open("GET", "subformvalidacao.php?ProcId=" + ProcId + "&StepId=" + StepId + "&CaseNum=" + CaseNum + "&Action=" + Action, true);
    xmlhttpNew.onreadystatechange = function ()
    {
        if (xmlhttpNew.readyState === 4)
        {
            //L� o texto
            var texto = xmlhttpNew.responseText;
            //Desfaz o urlencode
            //texto=texto.replace(/\+/g," ");
            //texto=unescape(texto);
            Validacao = Function(texto);
            for (i = 0; i < Validacoes.length; i++)
            {
                cmd = 'retorno = validaFormulario' + Campo + ' == Validacoes[' + i + ']';
                eval(cmd);
                if (retorno)
                {
                    cmd = 'validaFormulario' + Campo + '= Validacao';
                    eval(cmd);
                    cmd = 'Validacoes[i] = validaFormulario' + Campo;
                    eval(cmd);
                    break;
                }
            }
        }
    };
    xmlhttpNew.send(null);
}

/**
 * 
 * @param {type} ProcId
 * @param {type} CaseNum
 * @param {type} Campo
 * @param {type} Formulario
 * @param {type} Action
 * @param {type} FormsPai
 * @param {type} FieldName
 * @param {type} Optional
 * @param {type} ClasseSubstituta
 * @param {type} tipoLabelCampo
 * @returns {undefined}
 */
function JSmontaFormulario(ProcId, CaseNum, Campo, Formulario, Action, FormsPai, FieldName, Optional, ClasseSubstituta, tipoLabelCampo)
{
    if (isNaN(Campo))
    {
        var StepId = document.getElementById(Campo).value;
    } else {
        var StepId = Campo;
    }
    document.getElementById(Formulario).innerHTML = '<div class="text-center" id="divLoadingForm"><i class="fa fa-refresh fa-spin fa-4x"></i> <br>Carregando dados</div>';
    //MontaValidacaoFormulario(ProcId, StepId, Campo, Action);
    if (StepId < 1)
    {
        AForms = FormsPai.split(",");
        var Endereco = "";
        for (i = 0; i < AForms.length; i++)
        {
            Endereco = Endereco + "[\"" + AForms[i] + "\"]";
        }
        cmd = "Validacao" + Endereco + " = new Object()\n";
        cmd = cmd + "Validacao" + Endereco + " = \" ValidaLista(document." + Formulario + "." + Campo + ", '" + FieldName + "', '" + Campo + "', " + Optional + ") \"";
        eval(cmd);
        document.getElementById(Formulario).innerHTML = '';
        return;
    }
    xmlhttp2 = CriaAjax();
    url = "subform?ProcId=" + ProcId + "&StepId=" + StepId + "&CaseNum=" + CaseNum + "&Action=" + Action + "&Formulario=" + Formulario + "&FormsPai=" + FormsPai + "&CampoForm=" + Campo + "&FieldName=" + FieldName + "&classeSubstituta=" + ClasseSubstituta + "&tipoLabelCampo=" + tipoLabelCampo;
    console.log(url);
    xmlhttp2.open("GET", url, true);
    xmlhttp2.onreadystatechange = function ()
    {
        if (xmlhttp2.readyState === 4)
        {
            var texto = xmlhttp2.responseText;
            try {
                eval(texto);
                if (typeof callBackSRF !== "undefined")
                {
                    $("#btnSalvarModalEditDadosCaso").removeAttr("disabled");
                    callBackSRF();
                }
            } catch (err)
            {
                alert(err);
            }
        }
    };
    xmlhttp2.send(null);
}

function UploadImagem(Imagem, Campo, ProcId, CaseNum)
{
    IdImagem = Imagem;
    IdCampo = Campo;
    url = "callajax.php?script=BPMAttachFile.php&Tipo=IM&ProcId=" + ProcId + "&CaseNum=" + CaseNum;
    var xmlhttp = CriaAjax();
    var xmlhttp = CriaAjax();
    xmlhttp.open("GET", url, true);
    xmlhttp.onreadystatechange = function ()
    {
        if (xmlhttp.readyState === 4)
        {
            //L� o texto
            var texto = xmlhttp.responseText;
            //Desfaz o urlencode
            texto = texto.replace(/\+/g, " ")
            texto = unescape(texto)

            //Exibe o texto no div conte�do
            var master = document.getElementById("DivMasterUpload");
            master.style.display = "";
            var conteudo = document.getElementById("DivConteudoUpload");
            conteudo.innerHTML = texto;
            left = document.body.clientWidth / 2 - master.clientWidth / 2 + document.body.scrollLeft;
            var top = document.body.scrollTop + document.body.document.body.clientHeight / 2 - master.clientHeight / 2;
            master.style.top = top + "px";
            master.style.left = left + "px";
        }
    };
    xmlhttp.send(null);
}

function jsSelecionarArquivo(fieldId, ProcId, CaseNum, acceptFiles, StepIdSel)
{
    IdCampo = fieldId;
    if (typeof acceptFiles === "undefined" | acceptFiles === undefined)
    {
        acceptFiles = "";
    }

    tipoCampo = $('[aria-field-id="' + fieldId + '"]').attr("aria-field-type");

    url = "fileattachsel?tipoCampo=" + tipoCampo + "&ProcId=" + ProcId + "&CaseNum=" + CaseNum + "&StepId=" + StepIdSel + "&FieldId=" + fieldId + "&acceptFiles=" + acceptFiles;
    $.get(url, "",
            function (dadosRetorno) {
                var conteudo = document.getElementById("DivModalFD");
                conteudo.innerHTML = dadosRetorno;
                $("#fdModal").modal();
            });
}

function jsSelecionaArquivoUploadRepositorio(fieldId, ProcId, CaseNum, acceptFiles, StepRepositorio)
{
    jsSelecionarArquivo(fieldId, ProcId, CaseNum, acceptFiles, StepRepositorio);
}

function jsSelecionaArquivoUpload(fieldId, ProcId, CaseNum, acceptFiles)
{
    jsSelecionarArquivo(fieldId, ProcId, CaseNum, acceptFiles, StepId);
}

function LimpaCamposArquivo(IdCampo)
{
    if (confirm('Remover o arquivo???'))
    {
        document.getElementById("I" + IdCampo).value = '';
        document.getElementById("t" + IdCampo).value = '';
    }
}

function LimpaCampoImagem(IdCampo)
{
    if (confirm('Remover a  imagem???'))
    {
        document.getElementById("t" + IdCampo).value = '';
        muda_imagem(IdImagem, '');
    }
}


function AbreDocRM(url, Arquivo)
{
    if (Arquivo.length === 0)
    {
        alert("Não há arquivo para Abrir/Salvar");
        return;
    }
    WinSelect = window.open(url + Arquivo, "Select", "");
    WinSelect.focus();
}
;
function AnexaDocRM(Source)
{
    WinSelect = window.open(Source, "Select", "toolbar=0,Location=0,directories=0,status=1,menubar=0,scrollbars=1");
    WinSelect.focus();
}

function AlteraCampoRM(valor, objeto)
{
    document.getElementById("IA" + IdCampo).value = valor;
    document.getElementById("IB" + IdCampo).value = objeto;
    document.getElementById("t" + IdCampo).value = valor + ';' + objeto;
}

function muda_arquivo_upload(nome, ProcId, CaseNum)
{
    document.getElementById("img" + IdCampo).value = nome;
    document.getElementById("t" + IdCampo).value = nome;
    document.getElementById("Link" + IdCampo).href = "BPMViewFile.php?ProcId=" + ProcId + "&CaseNum=" + CaseNum + "&Target=" + nome;
    alert("Arquivo: " + nome + " carregado");
    EscondeUpload();
}
;
function muda_imagem_upload(arquivo, valor)
{
    muda_imagem(IdImagem, arquivo);
    IdCampo.value = valor;
    EscondeUpload();
}
;
function muda_imagem(imagem, arquivo)
{
    imga = new Image();
    imga.src = arquivo;
    imagem.src = imga.src;
}
;
function verifica_fechado(form)
{
    if (form.fechado.value === "1")
    {
        location.href = "BPMQueue.php";
    }
    return;
}
;
function fecha(form)
{
    form.fechado.value = "1";
}
;
function PosicionaAdHoc()
{
    var Master = document.getElementById("DivMasterAdHoc");
    Master.style.display = "";
    var left = document.body.clientWidth / 2 - Master.clientWidth / 2;
    var top = document.body.scrollTop + document.body.document.body.clientHeight / 2 - Master.clientHeight / 2;
    Master.style.left = left + document.body.scrollLeft + "px";
    Master.style.top = top + "px";
}

function AdHocOver()
{
    AdHoc_Status = true;
    return true;
}

function AdHocOut()
{
    AdHoc_Status = false;
    return true;
}

function CriarAdHoc()
{
    document.getElementById("CloseAdHoc").value = 0;
    MostraAdHoc();
}

function RedirecionarAdHoc()
{
    document.getElementById("DivDestinatarioAdHoc").style.display = "";
    document.getElementById("DivDeadTimeAdHoc").style.display = "none";
    document.getElementById("DivTituloAdHoc").innerHTML = "Redirecionar AdHoc";
    document.getElementById("CloseAdHoc").value = 0;
    MostraAdHoc();
}

function FinalizaAdHoc()
{
    document.getElementById("DivDestinatarioAdHoc").style.display = "none";
    document.getElementById("DivDeadTimeAdHoc").style.display = "none";
    document.getElementById("DivTituloAdHoc").innerHTML = "Responder AdHoc";
    document.getElementById("CloseAdHoc").value = 1;
    MostraAdHoc();
}

function MostraAdHoc()
{
    PosicionaAdHoc();
    document.getElementById("DivMasterAdHoc").style.display = "";
}

function EscondeAdHoc()
{
    document.getElementById("DivMasterAdHoc").style.display = "none";
}

function ValidaComentario()
{
    if (document.getElementById('comentario').value === '')
    {
        alert("Digite a mensagem");
        document.getElementById('comentario').focus();
        return false;
    }
}

function ValidaAdHoc(TipoAdHoc)
{
// TipoAdHoc
// 1 = Criar AdHoc
// 2 = Responder AdHoc
    var ValidarDestinatarioDeadTime = true;
    if (document.getElementById("CloseAdHoc").value === 1)
    {
        ValidarDestinatarioDeadTime = false;
    }

    if (TipoAdHoc === 0)
    {
        return true;
    }

    if (document.getElementById('rDestinatario').value === '' && ValidarDestinatarioDeadTime)
    {
        alert("Selecione o Destinat�rio");
        return false;
    }

    if (ValidaComentario() === false)
    {
        return false;
    }

    if (TipoAdHoc === 2)
    {
        return true;
    }


    if (document.getElementById('DeadTimeSoft').value === '' && ValidarDestinatarioDeadTime)
    {
        alert("Digite os minutos para Expirar");
        document.getElementById('DeadTimeSoft').focus();
        return false;
    }

    var DeadTimeSoft = parseInt(document.getElementById('DeadTimeSoft').value);
    if (document.getElementById('DeadTimeSoft').value !== DeadTimeSoft && ValidarDestinatarioDeadTime)
    {
        alert("Digite os minutos para Expirar");
        document.getElementById('DeadTimeSoft').focus();
        return false;
    }
    if (DeadTimeSoft < 1 && ValidarDestinatarioDeadTime)
    {
        alert("Valor de Expiração inválido");
        document.getElementById('DeadTimeSoft').focus();
        return false;
    }
    EnviaAdHoc();
    return true;
}

function ComparaDeads()
{
    if (document.getElementById('DeadTimeHard').value === '')
    {
        alert("Digite os minutos para Expirar");
        document.getElementById('DeadTimeHard').focus();
        return false;
    }

    var DeadTimeSoft = parseInt(document.getElementById('DeadTimeSoft').value);
    var DeadTimeHard = parseInt(document.getElementById('DeadTimeHard').value);
    if (DeadTimeSoft > 0 && DeadTimeHard > 0)
    {
        if (DeadTimeSoft <= DeadTimeHard)
        {
            alert("Valor do Primeiro Prazo deve ser Maior que o Valor para o Segundo Prazo ");
            document.getElementById('DeadTimeSoft').focus();
            return false;
        }
    }
    return true;
}


function EnviaAdHoc()
{
    url = "BPMAdHocFetchAjax.php?" + ValoresForm(document.FormAdHoc);
    var xmlhttp = CriaAjax();
    xmlhttp.open("GET", url, true);
    xmlhttp.onreadystatechange = function ()
    {
        if (xmlhttp.readyState === 4)
        {
            //L� o texto
            var texto = xmlhttp.responseText;
            //Desfaz o urlencode
            texto = texto.replace(/\+/g, " ");
            texto = unescape(texto);
            var conteudo = document.getElementById("DivMasterAdHoc");
            conteudo.innerHTML = texto;
            document.location = 'BpmQueue.php';
        }
    };
    xmlhttp.send(null);
}

function JSmostraHistoricoCampo(ProcId, FieldId, CaseNum)
{
    url = "multivalueAjax?ProcId=" + ProcId + "&FieldId=" + FieldId + "&CaseNum=" + CaseNum;
    var conteudo = document.getElementById("DivConteudoHistoricoCampo" + FieldId);
    var xmlhttp = CriaAjax();
    xmlhttp.open("GET", url, true);
    xmlhttp.onreadystatechange = function ()
    {
        if (xmlhttp.readyState === 4)
        {
            //L� o texto
            var texto = xmlhttp.responseText;
            //Desfaz o urlencode
            texto = texto.replace(/\+/g, " ");
            texto = unescape(texto);
            //Exibe o texto no div conte�do
            conteudo.style.display = "";
            conteudo.innerHTML = texto;
        }
    };
    xmlhttp.send(null);
}

function EscondeHistoricoCampo()
{
    document.getElementById("DivMasterHistoricoCampo").style.display = "none";
}

function EscondeFolder()
{
    IDObj = "DivMasterFolder";
    document.getElementById(IDObj).style.display = "none";
}

function CarregaTickler(ProcId, CaseNum)
{
    url = "BPMTicklerAjax.php?ProcId=" + ProcId + "&CaseNum=" + CaseNum;
    var xmlhttp = CriaAjax();
    xmlhttp.open("GET", url, true);
    xmlhttp.onreadystatechange = function ()
    {
        if (xmlhttp.readyState === 4)
        {
            //L� o texto
            var texto = xmlhttp.responseText;
            //Desfaz o urlencode
            texto = texto.replace(/\+/g, " ");
            texto = unescape(texto);
            var Master = document.getElementById("DivMasterTickler");
            var left = document.body.clientWidth / 2 - Master.clientWidth / 2;
            var top = document.body.scrollTop + document.body.document.body.clientHeight / 2 - Master.clientHeight / 2;
            Master.style.left = left + document.body.scrollLeft + "px";
            Master.style.top = top + "px";
            Master.style.display = "";
            //Exibe o texto no div conte�do
            var conteudo = document.getElementById("DivConteudoTickler");
            conteudo.innerHTML = texto;
        }
    };
    xmlhttp.send(null);
}

function MostraTickler(ProcId, CaseNum)
{
    CarregaTickler(ProcId, CaseNum);
}

function EscondeTickler()
{
    document.getElementById("DivMasterTickler").style.display = "none";
}

function AbreArquivo_(ProcId, CaseNum, Source)
{
    if (Source.length === 0)
    {
        alert("Não há arquivo para Abrir/Salvar");
        return;
    }
    WinSelect = window.open("BPMViewFile.php?ProcId=" + ProcId + "&CaseNum=" + CaseNum + "&Target=" + Source, "Select");
    WinSelect.focus();
}

function jsCancelaSelArquivo()
{
    $("#DivFolderSelFile").hide();
    $("#DivFolderScanFile").hide();
    $("#DivFolderFiles").show();
}

function ApagarArquivo(Arquivo)
{
    if (confirm('Remover o arquivo "' + Arquivo + '"?'))
    {
        document.getElementById('DivAcaoFolder').innerHTML = "Apagando Arquivo " + Arquivo;
        document.getElementById('DivAcaoFolder').style.display = '';
        return true;
    } else
    {
        return false;
    }
}

function CarregandoArquivo()
{
    if (document.getElementById('file').value === '') {
        alert('Selecione o Arquivo');
        return false;
    }
    document.getElementById('DivAcaoFolder').innerHTML = "Carregando Arquivo";
    document.getElementById('DivAcaoFolder').style.display = '';
    return true;
}

function EscondeUpload()
{
    document.getElementById('DivMasterUpload').style.display = 'none';
}

function EscondeDivSelLista()
{
    document.getElementById('DivMasterLista').style.display = "none";
}

function ChangeValue(url, Div, PersistValues)
{
    var xmlhttp1 = CriaAjax();
    xmlhttp1.open("GET", url, true);
    xmlhttp1.onreadystatechange = function ()
    {
        if (xmlhttp1.readyState === 4)
        {
            //L� o texto
            var texto = xmlhttp1.responseText;
            //Desfaz o urlencode
            texto = texto.replace(/\+/g, " ");
            texto = unescape(texto);
            //Exibe o texto no div conte�do
            var conteudo = document.getElementById(Div);
            if (PersistValues === false)
            {
                conteudo.innerHTML = texto;
            } else {
                conteudo.innerHTML = conteudo.innerHTML + texto;
            }
        }
    };
    xmlhttp1.send(null);
}

function AtualizaParentes(Fields, Valor, NaoFilhos)
{

}

function AtualizaCampo(FieldId, Valor, Div)
{
    url = "callajax.php?file=BPMChangeValuesTB.php&FieldId=" + FieldId + "&KeyValue=" + Valor;
    var conteudo = document.getElementById('DivConteudo' + Div)
    if (conteudo !== null)
    {
        conteudo.innerHTML = 'Carregando...';
        ChangeValue(url, Div, false);
    }
}

function CriaNovoCampoArray(FieldId, Valor, Div)
{
    url = "callajax.php?file=BPMChangeValuesTB.php&FieldId=" + FieldId + "&KeyValue=" + Valor;
    var conteudo = document.getElementById('DivConteudo' + Div);
    if (conteudo != null)
    {
        ChangeValue(url, Div, true);
    }
}

function jsAdicionaCampoArray(fieldId, fieldType)
{
    url = "callajax.php?EchoExeFunc=adicionaCampoArray&fieldId=" + fieldId + "&fieldType=" + fieldType;
    idObjDestino = 'conteudoA' + fieldType + fieldId;
    ChangeValue(url, idObjDestino, true);
}

$(document).ready(
        function () {
            //jsCarregaDocumentosProcesso();
            SetaEnterParaTab();
        }
);

function jsValidaDadosSignatario()
{
    $("#div_alert_campos").hide();
    if ($("#signatario_nome").val() === "")
    {
        $("#signatario_nome").focus();
        $("#div_alert_campos").show();
        $("#div_alert_campos").html("Preencha o nome Signatario");
        return false;
    }
    if ($("#signatario_email").val() === "")
    {
        $("#signatario_email").focus();
        $("#div_alert_campos").show();
        $("#div_alert_campos").html("Preencha o e-mail do Signatario");
        return false;
    }
    if ($("#signatario_cpf").val() === "")
    {
        $("#signatario_cpf").focus();
        $("#div_alert_campos").show();
        $("#div_alert_campos").html("Preencha o Cpf do Signatario");
        return false;
    }

    if (!jsValidaCPF($("#signatario_cpf").val()))
    {
        $("#signatario_cpf").focus();
        $("#div_alert_campos").show();
        $("#div_alert_campos").html("Cpf inválido");
        return false;
    }

    if ($("#signatario_datanasc").val() === "")
    {
        $("#signatario_datanasc").focus();
        $("#div_alert_campos").show();
        $("#div_alert_campos").html("Preencha a data de nascimento do Signatario");
        return false;
    }

    if (!checkDate($("#signatario_datanasc").val()))
    {
        $("#signatario_datanasc").focus();
        $("#div_alert_campos").show();
        $("#div_alert_campos").html("Data de Nascimento inválida");
        return false;
    }

    /**
     *  Verifica as participações
     */
    participacaoErro = false;
    if ($("#signatario_participacoes").val() === "")
    {
        participacaoErro = true;
    } else {
        listaParticipacoes = JSON.parse($("#signatario_participacoes").val());
        participacaoErro = listaParticipacoes.length == 0;
    }
    if (participacaoErro)
    {
        $("#sel_particiacao_signatario").focus();
        $("#div_alert_campos").show();
        $("#div_alert_campos").html("Selecione uma Participação");
        return false;
    }

    return true;
}