function jsOpenEditUser(userId)
{
    url = "edituser/" + userId;
    jsCarregaConteudoEmDiv(url, "ModalContent", "", "", "", '$("#crModalEditUser").modal("show")');
}

function jsSalvarUsuario()
{
    dados_formulario = jsGetDataFromForm("frmDadosUser");
    url = "api/manager/salvardadosusuario";
    $.ajax({
        url: url,
        method: "POST",
        data: dados_formulario
    }, dados_formulario)
            .done(function (retorno) {
                // Sucesso em gravar
                datatableUsuarios.ajax.reload();
                $("#crModalEditUser").modal("hide");
            })
            .fail(function (retorno) {
                // Falha ao gravar
                alert(retorno);
            });
}

function jsEditarGruposUsuario(userId, userName)
{
    url = "gruposusuario/U/" + userId + "/" + userName;
    jsCarregaConteudoEmDiv(url, "ModalContent", "", "", "", '$("#crModalEditPermissoes").modal("show")');
}

function jsMostraAdicionarEntidade()
{
    $("#LISTA_ENTIDADES").hide();
    $("#searchEntidade").show();
    $("#ADICIONAR_ENTIDADE").hide();
    $("#CANCELAR_SEL_ENTIDADE").show();
    $("#CANCELAR_ENTIDADES").hide();
}

function jsCancelarPesquisaEntidades()
{
    $("#LISTA_ENTIDADES").show();
    $("#searchEntidade").hide();
    $("#ADICIONAR_ENTIDADE").show();
    $("#CANCELAR_SEL_ENTIDADE").hide();
    $("#CANCELAR_ENTIDADES").show();
}

function JSBuscaListaUsersGroups(userId)
{
    TXFiltro = document.getElementById('Filtrar').value;
    TXtipoFiltro = document.getElementById('TIPOFILTRO').value;
    
    procId = $("#PROCID").val();

    //url = "include/BPMSelUserAjax.php?TipoFiltro=" + TipoFiltro + "&Filtrar=" + TXFiltro + "&Origem=" + Origem + "&Grupo=" + GrupoFiltro;
    url = "selgruposuser?TipoFiltro=" + TXtipoFiltro + "&Filtrar=" + TXFiltro + "&UserId=" + userId + "&tipoPesquisa=G&procId=" + procId;
    xmlhttp.open("GET", url, true);
    xmlhttp.onreadystatechange = function ()
    {
        if (xmlhttp.readyState === 4)
        {
            //L� o texto
            var texto = xmlhttp.responseText

            //Desfaz o urlencodejsSalvarPermissoes
            texto = texto.replace(/\+/g, " ");
            texto = unescape(texto);

            var conteudo = document.getElementById("DivConteudoUser");
            conteudo.innerHTML = texto;
        }
    };
    xmlhttp.send(null);
}

function JSselecionaUsuarioGrupoPermissao(linha, groupId, nomeGrupo, grpFld)
{
    url = "api/manager/adicionausergrupo";
    usuarioGrupo = $("#entidadeId").val();
    nomeUsuario = $("#entidadeName").val();
    dadosEnvio = {usuarioGrupo: usuarioGrupo, entidadeId: groupId, grpFld: grpFld};
    $("#CANCELAR_SEL_ENTIDADE").hide();
    $("#CANCELAR_ENTIDADES").show();
    $.ajax({
        url: url,
        data: dadosEnvio,
        method: "POST"
    }).done(function () {
        url = "gruposusuario/U/" + usuarioGrupo + "/" + nomeUsuario + "/true";
        jsCarregaConteudoEmDiv(url, "listagrupos");
    });
}

function JSremoveUsuarioGrupoPermissao(linha, groupId, nomeGrupo, grpFld)
{
    url = "api/manager/removeusergrupo";
    usuarioGrupo = $("#entidadeId").val();
    nomeUsuario = $("#entidadeName").val();
    dadosEnvio = {usuarioGrupo: usuarioGrupo, entidadeId: groupId, grpFld: grpFld};
    $.ajax({
        url: url,
        data: dadosEnvio,
        method: "POST"
    }).done(function () {
        url = "gruposusuario/U/" + usuarioGrupo + "/" + nomeUsuario + "/true";
        jsCarregaConteudoEmDiv(url, "listagrupos");
    });
}

function jsEditarPassword(userId, userName)
{
    url = "userpassword/" + userId + "/" + userName + "/true";
    jsCarregaConteudoEmDiv(url, "ModalContent", "", "", "", '$("#crModalEditPassword").modal("show")');
}

function jsSalvarSenhaUsuario(userId, userPassword)
{
    senha = $("#USUARIO_SENHA").val();
    if (senha.length < 8)
    {
        alert("Senha muito curta");
        return;
    }
    url = "api/manager/savepassword";
    dadosEnvio = jsGetDataFromForm("frmDadosPassword");
    $.ajax({
        url: url,
        data: dadosEnvio,
        method: "POST"
    }).done(function () {
        $("#crModalEditPassword").modal('hide');
    });
}

function jsSalvarSenhaUsuarioRestart(userId, userPassword)
{
    senha = $("#USUARIO_SENHA").val();
    if (senha.length < 8)
    {
        alert("Senha muito curta");
        return;
    }
    url = "api/manager/savepasswordstart";
    dadosEnvio = jsGetDataFromForm("frmDadosPassword");
    $.ajax({
        url: url,
        data: dadosEnvio,
        method: "POST"
    }).done(function () {
        $("#crModalEditPassword").modal('hide');
    });
}