/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

offSetTamanhoPagina = 150;

// Pega o Objeto Canvas de Formalizacao
var canvas = document.getElementsByTagName('canvas')[0];


var gkhead = new Image;

function jsHabilitaBotaoImagem(nrImagem, classBotao)
{
    ariaCode = "[aria-nr-imagem=\"" + nrImagem + "\"]";
    $(ariaCode).removeAttr("disabled");
    $(ariaCode).removeClass("btn-default");
    $(ariaCode).addClass(classBotao);
}

function CarregaPrimeiraImagemFormalizacao()
{
    jsHabilitaBotaoImagem('IMG1_IMG1', "btn-primary");
    jsMudaImagemBotao(null, "IMG1_IMG1");
}

window.onload = function () {
    redraw();
};

var ctx = canvas.getContext('2d');

trackTransforms(ctx);

var old_ctx = ctx;

var padrao = ctx.save();

var turnImage = true;

function jsRotateImage()
{
    escala = canvas.width / gkhead.width;
    if (turnImage)
    {
        moveX = $("canvas").height();
        moveY = 0;
    } else {
        moveX = $("canvas").width();
        moveY = 0;
    }
    turnImage = !turnImage;
    ctx.transform(1, 0, 0, 1, moveX, moveY);
    ctx.rotate(90 * Math.PI / 180);
    //ctx.translate(moveX, moveY);
    redraw();
}

var stopScale = true;

// Guarda o total de zoom executado
var zoomExecutado = 0;

function redraw() {

    // Clear the entire canvas
    var p1 = ctx.transformedPoint(0, 0);
    var p2 = ctx.transformedPoint(canvas.width, canvas.height);
    ctx.clearRect(p1.x, p1.y, p2.x - p1.x, p2.y - p1.y);

    ctx.save();
    ctx.setTransform(1, 0, 0, 1, 0, 0);
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.restore();

    if (stopScale & gkhead.width > 0)
    {
        ctx.scale(1, 1);
        escala = canvas.width / gkhead.width;
        ctx.scale(escala, escala);
        stopScale = false;
    }

    ctx.drawImage(gkhead, 0, 0);

}

var lastX = canvas.width / 2, lastY = canvas.height / 2;

var dragStart, dragged;

canvas.addEventListener('mousedown', function (evt) {
    document.body.style.mozUserSelect = document.body.style.webkitUserSelect = document.body.style.userSelect = 'none';
    lastX = evt.offsetX || (evt.pageX - canvas.offsetLeft);
    lastY = evt.offsetY || (evt.pageY - canvas.offsetTop);
    dragStart = ctx.transformedPoint(lastX, lastY);
    dragged = false;
}, false);

canvas.addEventListener('mousemove', function (evt) {
    lastX = evt.offsetX || (evt.pageX - canvas.offsetLeft);
    lastY = evt.offsetY || (evt.pageY - canvas.offsetTop);
    dragged = true;
    if (dragStart) {
        var pt = ctx.transformedPoint(lastX, lastY);
        ctx.translate(pt.x - dragStart.x, pt.y - dragStart.y);
        redraw();
    }
}, false);

canvas.addEventListener('mouseout', function (evt) {
    dragStart = null;
    if (!dragged)
        zoom(evt.shiftKey ? -1 : 1);
}, false);

canvas.addEventListener('mouseup', function (evt) {
    dragStart = null;
    dragged = false;
}, false);

var scaleFactor = 1.1;

var zoom = function (clicks) {
    // Quarda o Total de Zoom aplicado   
    zoomExecutado = zoomExecutado + clicks;    
    var pt = ctx.transformedPoint(lastX, lastY);
    ctx.translate(pt.x, pt.y);
    var factor = Math.pow(scaleFactor, clicks);
    ctx.scale(factor, factor);
    ctx.translate(-pt.x, -pt.y);
    redraw();
};

var handleScroll = function (evt) {
    var delta = evt.wheelDelta ? evt.wheelDelta / 40 : evt.detail ? -evt.detail : 0;
    if (delta)
        zoom(delta);
    return evt.preventDefault() && false;
};

canvas.addEventListener('DOMMouseScroll', handleScroll, false);
canvas.addEventListener('mousewheel', handleScroll, false);

function jsAvancaRetrocedeImagem(posicao)
{
    botoes = $("[aria-code='button-sel-image']");
    nrTotal = botoes.length;
    imagemAtual = botoes.filter(".btn-primary").attr("aria-nr-imagem");
    if (posicao === 1)
    {
        if (imagemAtual === nrTotal)
        {
            imagemAtual = 1;
        } else {
            imagemAtual++;
        }
    } else {
        if (imagemAtual === 1)
        {
            imagemAtual = nrTotal;
        } else {
            imagemAtual--;
        }
    }
    $("[aria-code='button-sel-image']").removeClass("btn-primary");
    $("[aria-code='button-sel-image']").addClass("btn-success");
    $("[aria-nr-imagem='" + imagemAtual + "']").removeClass("btn-success");
    $("[aria-nr-imagem='" + imagemAtual + "']").addClass("btn-primary");
    $("[aria-nr-imagem='" + imagemAtual + "']").click();
}

function jsResetZoom()
{
    zoom(-3 + (zoomExecutado * -1));
    ctx.setTransform(1, 0, 0, 1, 0, 0);
}

function jsMudaImagemBotao(obj, idImagem, resetZoom)
{
    jsResetZoom();
    if (obj !== null)
    {
        $("[aria-code='button-sel-image']").removeClass("btn-primary");
        $("[aria-code='button-sel-image']").addClass("btn-success");
        $(obj).removeClass("btn-success");
        $(obj).addClass("btn-primary");
    }
    urlImagem = $("#" + idImagem)[0];
    jsPanChangeImage(urlImagem);
}

function jsPanChangeImage(obj)
{
    jsPanSetImageSrc(obj.src);
}

function jsPanSetImageSrc(urlImagem)
{
    if (urlImagem === null)
    {
        return;
    }
    if (gkhead.src === urlImagem)
    {
        return;
    }
    gkhead.addEventListener('load', redraw, false);
    ctx.rotate(0);
    gkhead.src = urlImagem;

}

// Adds ctx.getTransform() - returns an SVGMatrix
// Adds ctx.transformedPoint(x,y) - returns an SVGPoint
function trackTransforms(ctx) {
    var svg = document.createElementNS("http://www.w3.org/2000/svg", 'svg');
    var xform = svg.createSVGMatrix();
    ctx.getTransform = function () {
        return xform;
    };

    var savedTransforms = [];
    var save = ctx.save;
    ctx.save = function () {
        savedTransforms.push(xform.translate(0, 0));
        return save.call(ctx);
    };

    var restore = ctx.restore;
    ctx.restore = function () {
        xform = savedTransforms.pop();
        return restore.call(ctx);
    };

    var scale = ctx.scale;
    ctx.scale = function (sx, sy) {
        xform = xform.scaleNonUniform(sx, sy);
        return scale.call(ctx, sx, sy);
    };

    var rotate = ctx.rotate;
    ctx.rotate = function (radians) {
        xform = xform.rotate(radians * 180 / Math.PI);
        return rotate.call(ctx, radians);
    };

    var translate = ctx.translate;
    ctx.translate = function (dx, dy) {
        xform = xform.translate(dx, dy);
        return translate.call(ctx, dx, dy);
    };

    var transform = ctx.transform;
    ctx.transform = function (a, b, c, d, e, f) {
        var m2 = svg.createSVGMatrix();
        m2.a = a;
        m2.b = b;
        m2.c = c;
        m2.d = d;
        m2.e = e;
        m2.f = f;
        xform = xform.multiply(m2);
        return transform.call(ctx, a, b, c, d, e, f);
    };

    var setTransform = ctx.setTransform;
    ctx.setTransform = function (a, b, c, d, e, f) {
        xform.a = a;
        xform.b = b;
        xform.c = c;
        xform.d = d;
        xform.e = e;
        xform.f = f;
        return setTransform.call(ctx, a, b, c, d, e, f);
    };

    var pt = svg.createSVGPoint();
    ctx.transformedPoint = function (x, y) {
        pt.x = x;
        pt.y = y;
        return pt.matrixTransform(xform.inverse());
    };
}

/**
 * 
 * Carrega as imangens de formalização 
 * 
 * @returns {undefined}
 */
function jsCarregaImagensFormalizacao()
{
    listaImagens = $('[aria-imagem-formalizacao');
    for (i = 0; i < listaImagens.length; i++)
    {
        objImagem = listaImagens[i];
        srcImagem = $(objImagem).attr('aria-imagem-formalizacao');
        $(objImagem).attr('src', srcImagem);
    }
}

function jsSetaTamanhoCanvasFormalizacao()
{
    //canvasHeight = $(".content-wrapper").height() - 25;
    canvasHeight = $("html").height() - 100;
    canvasWidth = $("#divCanvas").innerWidth();
    $(canvas).attr("height", canvasHeight);
    $(canvas).attr("width", canvasWidth);
}

$(document).ready(function () {
    jsCarregaImagensFormalizacao();
    jsSetaTamanhoCanvasFormalizacao();

    /*
     *  Quando a um resize da janela, altera o tamanho do Canvas
     */
    $(window).resize(function () {
        jsSetaTamanhoCanvasFormalizacao();
    });
});


// jsPanSetImageSrc(imgsrc);