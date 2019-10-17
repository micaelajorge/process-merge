var listaCanvasPdf = undefined;
var contadorPaginas = 0;
function jsCarregaControlesImagem(totalImagensPdf, id_imagem)
{
    return new Promise((resolv, reject) => {
        url = "controlesimagens";
        dadosEnvio = {
            totalPaginas: totalImagensPdf,
            id_imagem: "_" + id_imagem,
            inicioContador: contadorPaginas
        };
        $.ajax(
                {
                    url: url,
                    type: "POST",
                    data: dadosEnvio,
                    dataType: 'html'
                })
                .done(function (dataRetorno) {
                    console.log(dataRetorno);
                    $("#" + id_imagem).html(dataRetorno);
                    resolv();
                })
                .fail(function (jqXHR, textStatus) {
                    reject("erro");
                });

    });
}

async function jsEncadeiaPDF()
{
    if (listaImagensPDF.length === 0)
    {
        return;
    }
    item = listaImagensPDF.pop();
    var canvas = criaCanvas();
    await jsGeraPaginaPdf(item, canvas);
    setTimeout(jsEncadeiaPDF, 1);
}

/**
 * 
 * @returns {undefined}
 */
function jsGeraPagiansPdf()
{
    if (typeof listaImagensPDF === "undefined")
    {
        return;
    }
    jsEncadeiaPDF();
//    listaImagensPDF.forEach((item, indice) => {        
//        var canvas = criaCanvas();
//        jsGeraPaginaPdf(item, canvas);
//    });
}

/**
 * 
 * @param {type} objetoPdf
 * @param {type} canvas
 * @returns {undefined}
 */
async function jsGeraPaginaPdf(objetoPdf, canvas)
{
    return new Promise((resolv, reject) => {
        var loadingTask = pdfjsLib.getDocument(objetoPdf.url);
        loadingTask.promise.then(async function (pdf) {
            var numPages = pdf.numPages;
            console.log(numPages);
            await jsCarregaControlesImagem(numPages, objetoPdf.objImagem);
            for (var i = 1; i <= numPages; i++)
            {
                // Fetch the first page
                var pageNumber = i;
                await pdf.getPage(pageNumber).then(async function (page) {
                    console.log('Page loaded');
                    var scale = 1.5;
                    var viewport = page.getViewport(scale);
                    // Prepare canvas using PDF page dimensions            
                    var context = canvas.getContext('2d');
                    canvas.height = viewport.height;
                    canvas.width = viewport.width;
                    // Render PDF page into canvas context
                    var renderContext = {
                        canvasContext: context,
                        viewport: viewport
                    };
                    var renderTask = page.render(renderContext);
                    await renderTask.then(function () {
                        var imageData = canvas.toDataURL("image/png").replace("image/png", "image/octet-stream"); // here is the most important part because if you dont replace you will get a DOM 18 exception.
                        var id_obj = "#IMG" + i + "_" + objetoPdf.objImagem;
                        //console.log('Id Obj', id_obj);
                        $(id_obj)[0].src = imageData;
                        $("#loading_" + objetoPdf.objImagem).hide();
                    });
                });
            }
            contadorPaginas = contadorPaginas + numPages;
        }, function (reason) {
            // PDF loading error
            console.error(reason);
        });
        resolv();
    });
}

function criaCanvas()
{
    var canvasCriado = document.createElement("canvas");
    canvasCriado.id = "the-canvas";
    //$("#divDados").append(canvasCriado);
    //document.appendChild(canvasCriado);
    return canvasCriado;
}

jsGeraPagiansPdf();
var __PDF_DOC,
        __PAGE_RENDERING_IN_PROGRESS = 0;
function showPreviwPDF(pdf_url) {
    $("#pdf-loader").show();
    __CANVAS = $('#pdfPreviewFile').get(0), __CANVAS_CTX = __CANVAS.getContext('2d');
    pdfjsLib.getDocument({url: pdf_url}).then(function (pdf_doc) {
        __PDF_DOC = pdf_doc;
        __TOTAL_PAGES = __PDF_DOC.numPages;
        // Hide the pdf loader and show pdf container in HTML

        // Show the first page
        showPage(1);
    }).catch(function (error) {
        // If error re-show the upload button

        alert(error.message);
    });
    ;
}

function showPage(page_no) {
    __PAGE_RENDERING_IN_PROGRESS = 1;
    __CURRENT_PAGE = page_no;
    // Fetch the page
    __PDF_DOC.getPage(page_no).then(function (page) {
        // As the canvas is of a fixed width we need to set the scale of the viewport accordingly
        var scale_required = __CANVAS.width / page.getViewport(1).width;
        // Get viewport of the page at required scale
        var viewport = page.getViewport(scale_required);
        // Set canvas height
        __CANVAS.height = viewport.height;
//        height = Math.round(viewport.height) + "px";
//        __CANVAS.height = height;

        var renderContext = {
            canvasContext: __CANVAS_CTX,
            viewport: viewport
        };
        // Render the page contents in the canvas
        page.render(renderContext).then(function () {
            __PAGE_RENDERING_IN_PROGRESS = 0;
        });
    });
}
