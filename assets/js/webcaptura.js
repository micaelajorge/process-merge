/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 17/12/2018
 Sistema: Process_XAMPP
 */

//  //-->t0140TQMAACCAswpcQz7hTxUQCP/lFX5u+Xx/bGfUyzk02Zx7UwopyV3WGv5Gj0IFtMl092UQiUswBm2KaZgs/OHuoXA6LGIyM3UIPj4DJtKHHSPWrDKM/jGVX3ux0WdbcSHfSMCob8jNI7B2z3hmTpaRgFHfXJnn5s48NedCpuGAkYDRudnKyqpCOzuwq+E=<--//


var DWObject;
var camposFormularioWebCapture = [];

function jsHabilitaBotaoEnvio()
{
    $("#btnEnvio").removeAttr("disabled");
    $("#divAlertDocumentoEnviado").hide();
    $("#divAlertDocumentoEnviadoErro").hide();
    $("#nomeArquivoSelecionado").html($("#arquivoUpload")[0].files[0].name);
    jsArquivoSelecionado('arquivoUpload');
    jsAtualizaBarradeProgresso("progressBarCaptura", "porcentagemValorCaptura", 0);
}

function jsCarregaImagemDocumento(caseNumFilho, document_view)
{
    dadosEnvio = {
        CaseNum: caseNumFilho
    };

    // Verifica se a visualizcao do Documento está aberta
    documentosVisualizacao = $("#ROW_" + document_view);
    if (typeof $(documentosVisualizacao).html() === "undefined")
    {
        return;
    }

    url = "documentosprocesso_getdoc";
    $.ajax(
            {
                url: url,
                type: "POST",
                data: dadosEnvio,
                dataType: 'html'
            })
            .done(function (resultado) {
                htmlBody = $(documentosVisualizacao).html();
                $(documentosVisualizacao).html(htmlBody + resultado);
                jsCreateTumbsPDF(document_view);
            });
}

function jsAnexaArquivoProcessoPai(numeroCasoPai)
{
    procId = $("#textos").find('[name="ProcId"]').val();
    stepId = $("#textos").find('[name="StepId"]').val();
    caseNum = $("#textos").find('[name="CaseNum"]').val();
    dadosEnvio = {
        procId: procId,
        stepId: stepId,
        caseNum: caseNum,
        numeroCasoPai: numeroCasoPai
    };
    url = "api/webcapture_anexar";

    $.ajax(
            {
                url: url,
                type: "POST",
                data: dadosEnvio,
                dataType: 'html'
            })
            .done(function (caseNumFilho) {
                $("#divAlertDocumentoEnviado").show();
                $("#pdfPreviewFile").hide();
                $("#imgPreviewFile").hide();
                jsAtualizaBarradeProgresso("progressBarCaptura", "porcentagemValorCaptura", 0);
                var arquivoAnterior = $("#nomeArquivoSelecionado").html();
                $("#nomeArquivoSelecionado").html("");
                $("#nomeArquivoAnterior").html(arquivoAnterior);
            });

}


function jsUploadDocumentToRepository(funcaoAposUpload, nomeFormularioOrigem, tipoRetorno)
{
    if (typeof nomeFormularioOrigem === 'undefined')
    {
        nomeFormularioOrigem = 'formDataCaptura';
    }
    
    if (typeof tipoRetorno === 'undefined')
    {
        tipoRetorno = false;
    }
    var novoFormObj = document.getElementById(nomeFormularioOrigem);
    var formDataUpload = new FormData(novoFormObj);
    $("#btnEnvio").attr("disabled", true);

    url = "api/webcapture_send";
    $.ajax(
            {
                url: url,
                type: "POST",
                data: formDataUpload,
                dataType: 'html',
                processData: false,
                contentType: false,
                xhr: function () {
                    var xhr = $.ajaxSettings.xhr();
                    xhr.onprogress = function e() {
                        // For downloads
                        if (e.lengthComputable) {
                            porcentagemCarregado = Math.round(e.loaded / e.total * 100);
                            jsAtualizaBarradeProgresso("progressBarCaptura", "porcentagemValorCaptura", porcentagemCarregado);
                        }
                    };
                    xhr.upload.onprogress = function (e) {
                        // For uploads
                        if (e.lengthComputable) {
                            porcentagemCarregado = Math.round(e.loaded / e.total * 100);
                            jsAtualizaBarradeProgresso("progressBarCaptura", "porcentagemValorCaptura", porcentagemCarregado);
                        }
                    };
                    return xhr;
                }
            })
            .done(function (dadosRetornados) {
                funcaoAposUpload(dadosRetornados, novoFormObj);
            });
}

function jsAtualizaCampoArquivoRepositorio(jsonRetorno, novoFormObj)
{    
    dadosRetorno = JSON.parse(jsonRetorno);
    dadosArquivo = JSON.parse(dadosRetorno.fileData[0].fieldValue);
    fieldId = $(novoFormObj.FieldId).val();
    
    // Coloca o Nome do caso criado no Repositorio
    $("#t" + fieldId).val(dadosRetorno.caseNum);
    
    // Seta os dados para acesso ao arquivo
    $("#t" + fieldId).attr('aria-file-data', dadosRetorno.fileData[0].fieldValue);
    
    // Mostra o Nome do arquivo
    $("#I" + fieldId).val(dadosArquivo.fileName);
    
    $("#Remove" + fieldId).removeAttr("disabled");
    $("#Link" + fieldId).removeAttr("disabled");
    $('.close').click(); // Esconde o Modal
}

/**
 * Atualiza documento de Lista de Documentos
 * 
 * @param {type} caseNumCriado
 * @returns {undefined}
 */
function jsAtualizaDocsEditCase(caseNumCriado)
{
    // Pega o Id do Documento que está sendo anexado
    documentoAnexar = $("#stepIdForm").val();
    if (caseNumCriado < 0)
    {
        $("#divAlertDocumentoEnviadoErro").show();
        $("#erroAnexar").html(caseNumCriado);
    } else {
        jsAnexaArquivoProcessoPai(caseNumCriado);
        jsCarregaImagemDocumento(caseNumCriado, documentoAnexar);
        totalDocs = parseInt($("#TOTAL_DOCS_" + documentoAnexar).html()) + 1;
        $("#TOTAL_DOCS_" + documentoAnexar).html(totalDocs);
        $("#TOTAL_DOCS_" + documentoAnexar).parents(".panel").removeClass("box-warning");
        $("#TOTAL_DOCS_" + documentoAnexar).parents(".panel").addClass("box-success");
    }
}

function jsUploadDocumentArquivoRepositorio(caseNumCriado, dadosFormulario)
{

}

// Anexar arquivo a processo CT
function jsUploadDocument() {
    jsUploadDocumentToRepository(jsAtualizaDocsEditCase);
}

function jsCarregaDadosForumarioWebcaptura()
{
    // Pega campos de dados
    $("#nomeArquivoSelecionado").html("");
    $("#btnEnvio").attr("disabled", "true");
    $("#pdfPreviewFile").hide();
    $("#imgPreviewFile").hide();
    $("#divAlertDocumentoEnviado").hide();


    camposWebCaptura = $("#divFormCaptura").find('[aria-type="field"]');
    for (i = 0; i < camposWebCaptura.length; i++)
    {
        campoWebCaptura = camposWebCaptura[i];
        codigoCampo = $(campoWebCaptura).attr("aria-code");
        valor = $("#divCabecalhoProcesso").find('[aria-code="' + codigoCampo + '"]').val();
        $(campoWebCaptura).val(valor);
    }
}

/**
 * 
 * @param {type} documentoProcId
 * @param {type} documentoStepId
 * @param {type} documentCaseNumPai
 * @returns {undefined}
 */
function jsWebCapturaDocumentoCT(documentoProcId, documentoStepId, documentCaseNumPai)
{

    // Verifica se a função deve carregar o modal, ou esperar o TWAIN ser carregado
    if (typeof Dynamsoft === "undefined")
    {
        // carrega Scripts Webcaptura
        url = "Resources/dynamsoft.webtwain.initiate.js";
        $.ajax({
            url: url,
            dataType: "script"
        });

        // carrega Scripts Webcaptura
        url = "Resources/dynamsoft.webtwain.config.js";
        $.ajax({
            url: url,
            dataType: "script"
        });
    }

    url = "assets/js/validacao.js";
    $.ajax({
        url: url,
        dataType: "script"
    });

    url = "assets/js/validadata.js";
    $.ajax({
        url: url,
        dataType: "script"
    });

    url = "editcase_webcaptura";
    dadosEnvio = {
        documentoProcId: documentoProcId,
        documentoStepId: documentoStepId,
        documentCaseNumPai: documentCaseNumPai
    };

    $.ajax({
        type: "POST",
        url: url,
        data: dadosEnvio
    }).done(function (dadosRetorno) {
        $("#divFormCaptura").html(dadosRetorno);
        jsPosLoadDynamicTwain();
        jsCarregaDadosForumarioWebcaptura();
    });
}

function jsPreencheCamposFormulario()
{
    camposFormularioWebCapture.forEach(function (campo, indice) {
        if ($("#" + campo["name"]).attr("aria-type-field") !== "SFR")
        {
            console.log(campo["name"] + " : " + campo["value"]);
            $("#" + campo["name"]).val(campo["value"]);

        }
    });
}

function callBackSRF()
{
    jsPreencheCamposFormulario();
}


function AcquireImage() {
    DWObject = Dynamsoft.WebTwainEnv.GetWebTwain('dwtcontrolContainer');
    if (DWObject) {
        DWObject.SelectSource(function () {
            var OnAcquireImageFailure = function () {
                DWObject.CloseSource();
            };
            var OnAcquireImageSuccess = function () {
                alert("Captura Executada");
                $("#buttonDigitalizar").hide();
                $("#buttonArmazenar").show();
                $("#buttonDescartar").removeAttr("disabled");
                $("#buttonEnviarWebCaptura").removeAttr("disabled");
                DWObject.CloseSource();
            };
            DWObject.OpenSource();
            DWObject.IfDisableSourceAfterAcquire = true;
            DWObject.AcquireImage(OnAcquireImageSuccess, OnAcquireImageFailure);
        }, function () {
            console.log('SelectSource failed!');
        });
    }
}

function jsCarregaWebCaptura()
{
    // Indica se a função deve carregar o modal, ou esperar o TWAIN ser carregado
    mostraWebCapture = false;
    if (typeof Dynamsoft === "undefined")
    {
        // carrega Scripts Webcaptura
        url = "Resources/dynamsoft.webtwain.initiate.js";
        $.ajax({
            url: url,
            dataType: "script"
        });

        // carrega Scripts Webcaptura
        url = "Resources/dynamsoft.webtwain.config.js";
        $.ajax({
            url: url,
            dataType: "script"
        });
    } else {
        mostraWebCapture = true;
    }

    url = "assets/js/validacao.js";
    $.ajax({
        url: url,
        dataType: "script"
    });

    url = "assets/js/validadata.js";
    $.ajax({
        url: url,
        dataType: "script"
    });


    $.get("webcaptura", function (dataRecebidos) {
        $("#divDadosWebCaptura").html(dataRecebidos);
        if (mostraWebCapture)
        {
            jsPosLoadDynamicTwain();
        }
    });
}

function jsPosLoadDynamicTwain()
{
    $("#capturaModal").modal("show");
}

function jsSelecionaProcesso()
{
    $("#buttonDigitalizar")[0].disabled = false;
    procId = $("#selProcWebCapture").val();
    url = "webcapture/selecionaprocesso";
    dadosEnvio = {
        ProcId: procId
    };
    $.ajax({
        url: url,
        method: "POST",
        data: dadosEnvio
    })
            .done(function (retorno) {
                $("#camposFormulario").html(retorno);
            })
            .fail(function (retorno) {
                // Falha ao gravar
                alert(retorno);
            });
}

function jsGuardarImagem()
{
    $("#buttonDigitalizar").show();
    $("#buttonArmazenar").hide();
    $("#buttonDescartar")[0].disabled = true;
}


function OnHttpUploadSuccess() {
    console.log('successful');
}
function OnHttpUploadFailure(errorCode, errorString, sHttpResponse) {
    if (errorCode == -2003)
    {
        jsAtualizaDocsEditCase(sHttpResponse);
    } else {
        alert(errorString + sHttpResponse);
    }
    
    
}

function jsUploadImage(editCase) {
    if (typeof editCase === "undefined")
    {
        editCase = false;
    }
    if (!editCase)
    {
        if (!jsValidacaoFormulario('textos'))
        {
            return;
        }
    }
    if (DWObject) {
        jsMontaCamposFormularioEnvioScan();
        var strHTTPServer = location.hostname; //The name of the HTTP server. 
        var CurrentPathName = unescape(location.pathname);
        var CurrentPath = CurrentPathName.replace(/editcase.*/, "");
        var strActionPage = CurrentPath + "api/webcapture_send";

        DWObject.IfSSL = false; // Set whether SSL is used
        DWObject.HTTPPort = location.port == "" ? 80 : location.port;
        DWObject.SelectedImagesCount = 3;
        DWObject.GetSelectedImagesSize(4); // 4 - PDF format. Calculate the size of selected images in PDF format.

//        // Upload the selected images to the server asynchronously
//        DWObject.HTTPUploadThroughPostAsMultiPagePDF(
//                strHTTPServer,
//                strActionPage,
//                "imageData.pdf",
//                OnHttpUploadSuccess,
//                OnHttpUploadFailure
//                );
        DWObject.HTTPUploadThroughPost(
                strHTTPServer,
                DWObject.CurrentImageIndexInBuffer,
                strActionPage,
                "imageData.jpg",
                OnHttpUploadSuccess,
                OnHttpUploadFailure
                );
    }
}

function pegaCamposFormularioEnvioDocumento()
{
    listaCamposFormulario = $("#formDataCaptura").find('aria-type=["field"]');
    campos = new Array();
    for (i = 0; i < listaCamposFormulario.length; i++)
    {
        objCampo = listaCamposFormulario[i];
        codigo = $(objCampo).attr("aria-code");
        valor = $(objCampo).val();
        item = {FieldCode: codigo, FieldValue: valor};
        campos.push(item);
    }
    return campos;
}

/**
 * 
 * @returns {undefined}
 */
function jsMontaCamposFormularioEnvioEditCase()
{
    // Limpa o Formulario interno do Objeto TWAIN
    DWObject.ClearAllHTTPFormField();
    // Inclui Campo do ProcId
    procIdPai = $("#selProcWebCapture").val();
    DWObject.SetHTTPFormField("procId", procIdPai);
    StepIdForm = $("[aria-type-field]").val();
    DWObject.SetHTTPFormField("stepIdForm", StepIdForm);
    DWObject.SetHTTPFormField("procId", procId);
    camposFormularioWebCapture = getFormValuesObjectList("textos");
    camposFormularioWebCapture.forEach(function (campo, indice) {
        DWObject.SetHTTPFormField(campo["name"], campo["value"]);
    });
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

