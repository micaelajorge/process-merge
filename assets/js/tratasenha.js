var limiteSenhaFraca = 20;
var limiteSenhaJusta = 30;
var limiteSenhaForte = 40;
var minimoCaractetes = 8;

function calcula_forca(campoSenha) {
    senha = $(campoSenha).val();
    forca = 0;
    if ((senha.length < minimoCaractetes)) {
        forca = -1 * limiteSenhaForte;
    }

    if (senha.match(/[a-z]+/)) {
        forca += 10;
    }
    if (senha.match(/[A-Z]+/)) {
        forca += 10;
    }
    if (senha.match(/\d+/)) {
        forca += 10;
    }
    if (senha.match(/\W+/)) {
        forca += 10;
    }
    return forca;
}

function trataForcaSenha(campoSenha) {

    forcaSenha = calcula_forca(campoSenha);
    $("#btnSalvarSenha")[0].disabled = true;
    $("#forcaSenha").removeClass();
    if (forcaSenha < limiteSenhaJusta) {
        forcaSenhaTexto = "Fraca";
        forcaSenhaClass = "text-danger";
    } else if ((forcaSenha >= limiteSenhaJusta) && (forcaSenha < limiteSenhaForte)) {
        forcaSenhaClass = "text-warning";
        forcaSenhaTexto = "Justa";
    } else if (forcaSenha >= limiteSenhaForte) {
        $("#btnSalvarSenha")[0].disabled = false;
        forcaSenhaClass = "text-green";
        forcaSenhaTexto = "Forte";
    }
    $("#forcaSenha").html(forcaSenhaTexto);
    $("#forcaSenha").addClass(forcaSenhaClass);
}

function jsMostraTrocarSenha()
{
    var divTrocaSenha = document.createElement("div");
    divTrocaSenha.id = "divTrocaSenha";
    document.body.appendChild(divTrocaSenha);

    $.getScript("assets/js/admin_users.js");

    url = "trocarsenha";
    $.ajax(url).done(dadosRecebidos => {
        $("#divTrocaSenha").html(dadosRecebidos);
        $("#crModalEditPassword").modal("show");
    });
}

function jsTrocaSenha()
{
    if ($("#USUARIO_SENHA").val() !== $("#USUARIO_SENHA").val())
    {
        alert("Senhas não correspondem");
    }
    jsSalvarSenhaUsuario();
}
