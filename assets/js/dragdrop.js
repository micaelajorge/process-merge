/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 03/11/2018
 Sistema: Process_XAMPP
 */

function allowDrop(ev) {
    ev.preventDefault();
}

function drag(ev) {
    ev.dataTransfer.setData("text", ev.target.id);
}

function jsSetaStatusTipificacao()
{
    $(".box-tipificacao").each(function (indice, obj) {
        if ($(obj).find("img").length == 0)
        {
            $(obj).find(":hidden").val(0);
        } else {
            $(obj).find(":hidden").val(1);
        }
    });
}

function jsMoveDocumentosTipificados()
{
    imagensDisponiveis = $(".box-disp-formalizacao").find("img");
    console.log("Total Imagens" + imagensDisponiveis.length);
    imagensDisponiveis.each(function (id, objetoMover) {
        codigoDocumento = $(objetoMover).attr("aria-tipodoc");
        console.log($(objetoMover)[0].naturalWidth + ' - ' + $(objetoMover)[0].naturalHeight + " : " + $(objetoMover)[0].naturalWidth / $(objetoMover)[0].naturalHeight);
        if (codigoDocumento != "")
        {
            $("[aria-code='" + codigoDocumento + "']").append(objetoMover);
            $("[aria-code='" + codigoDocumento + "']").parents(".box-tipificacao").removeClass("box-warning");
            $("[aria-code='" + codigoDocumento + "']").parents(".box-tipificacao").addClass("box-success");            
        } else {
            
        }
        $(objetoMover).show();
    });
    jsSetaStatusTipificacao();
}

function jsMudaCorBoxTipificacao(objDrag, objRecebendo)
{
    // Faz a Tipificacao do Documento
    jsSetaTipoDocumento(objDrag, objRecebendo);    
    $(objRecebendo).parents(".box-tipificacao").removeClass("box-warning");
    $(objRecebendo).parents(".box-tipificacao").addClass("box-success");    
    paiAtual = $(objDrag).parents(".box-tipificacao");
    if (paiAtual.length > 0)
    {                
        if ($(paiAtual).find("img").length <= 1)
        {
        $(paiAtual).removeClass("box-success");
        $(paiAtual).addClass("box-warning");                
        }
    }
}

function jsSetaTipoDocumento(objDrag, objRecebendo)
{
    codigoTipificacao = $(objRecebendo).attr("aria-code");
    $(objDrag).attr("aria-tipo", codigoTipificacao);
    nrPagina = $(objDrag).attr("aria-nr-pagina");
    jsonImagem = $("#TX" + nrPagina).val();
    objJson = JSON.parse(jsonImagem);
    objJson["tipoDocumento"] = codigoTipificacao;
    $("#TX" + nrPagina).val(JSON.stringify(objJson));
}


function drop(ev) {
    ev.preventDefault();    
    var data = ev.dataTransfer.getData("text");
    objDrag = document.getElementById(data);
    target = ev.target;
    if ($(target)[0].nodeName !== "DIV")
    {
        target = $(target).parent("div")[0];
    }
    jsMudaCorBoxTipificacao(objDrag, target);    
    $(target)[0].appendChild(objDrag);
    jsSetaStatusTipificacao();
}

$(document).ready(function () {
    jsMoveDocumentosTipificados();
});