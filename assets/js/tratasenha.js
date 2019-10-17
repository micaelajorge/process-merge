function calcula_forca(campoSenha) {
    senha = $(campoSenha).val();
    forca = 0;
    if ((senha.length >= 4) && (senha.length <= 7)) {
        forca += 10;
    }
//    } else if (senha.length > 7) {
//        forca += 25;
//    }
    if (senha.match(/[a-z]+/)) {
        forca += 10;
    }
    if (senha.match(/[A-Z]+/)) {
        forca += 20;
    }
    if (senha.match(/\d+/)) {
        forca += 20;
    }
    if (senha.match(/\W+/)) {
        forca += 20;
    }
    return forca;
}

function trataForcaSenha(campoSenha) {
    forcaSenha = calcula_forca(campoSenha);
    $("#forcaSenha").removeClass();
    if (forcaSenha < 30) {
        forcaSenhaTexto = "Fraca";
        forcaSenhaClass = "text-danger";
        $("#btnSalvarSenha")[0].disabled = true;
    } else if ((forcaSenha >= 30) && (forcaSenha < 60)) {
        $("#btnSalvarSenha")[0].disabled = true;
        forcaSenhaClass = "text-warning";
        forcaSenhaTexto = "Justa";
    } else if ((forcaSenha >= 75) && (forcaSenha < 85)) {
        $("#btnSalvarSenha")[0].disabled = false;
        forcaSenhaClass = "text-green";
        forcaSenhaTexto = "Forte";
    } else {
        $("#btnSalvarSenha")[0].disabled = false;
        forcaSenhaClass = "text-primary";
        forcaSenhaTexto = "Excelente";
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
