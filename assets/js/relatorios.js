/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 10/12/2018
 Sistema: Process_XAMPP
 */



function jsGeraRelatorioPDF(formulario, urlRelatorio, nomeRelatorio)
{
    dadosEnvio = getFormValues(formulario);
    
    dataInicio = $("#" + formulario + "_data_inicio").val();
    dataFim = $("#" + formulario + "_data_fim").val();
    nomeRelatorio = nomeRelatorio + " Período: " + dataInicio + " até " + dataFim;
    $.ajax({
        type: "POST",
        url: urlRelatorio,
        data: dadosEnvio
    }).done(function (dadosRetorno) {
//        var w = window.open('');
//        w.document.open();
//        w.document.write(dadosRetorno);
//        w.document.close();        
        $("#divRelPreview").html(dadosRetorno);
        jsGeraPdf(nomeRelatorio);
    });
}

function jsGerarRelatorioCsvXls(formSubmit, type, formAction)
{
    $("#" + formSubmit).attr('action', formAction + "_" + type);     
    $("#" + formSubmit).submit();    
}

function jsGeraPdf(nomeRelatorio)
{
    var doc = new jsPDF('p', 'pt');
    var elem = document.getElementById("tableRelatorio");
    var res = doc.autoTableHtmlToJson(elem);
    doc.autoTable(res.columns, res.data);
    doc.setFontSize(10);
    doc.text(nomeRelatorio, 50, 25);
    doc.save("relatorio.pdf");
}

function jsGeraDocumentoPdf(data)
{
    var doc = new jsPDF({fontSize: 8});
    var totalPagesExp = "{total_pages_count_string}";

    var pageContent = function (data) {
        // HEADER
        doc.setFontSize(8);
        doc.setTextColor(40);
        doc.setFontStyle('normal');
        if (base64Img) {
            doc.addImage(base64Img, 'JPEG', data.settings.margin.left, 15, 10, 10);
        }
        doc.text("Relatorio", data.settings.margin.left + 15, 22);

        // FOOTER
        var str = "Page " + data.pageCount;
        // Total page number plugin only available in jspdf v1.0+
        if (typeof doc.putTotalPages === 'function') {
            str = str + " of " + totalPagesExp;
        }
        doc.setFontSize(10);
        var pageHeight = doc.internal.pageSize.height || doc.internal.pageSize.getHeight();
        doc.text(str, data.settings.margin.left, pageHeight - 10);
    };

    doc.autoTable(getColumns(), getData(40), {
        addPageContent: pageContent,
        margin: {top: 30}
    });

    // Total page number plugin only available in jspdf v1.0+
    if (typeof doc.putTotalPages === 'function') {
        doc.putTotalPages(totalPagesExp);
    }
    return doc;
}