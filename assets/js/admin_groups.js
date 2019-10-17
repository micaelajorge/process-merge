function jsOpenEditGroup(groupId)
{
    url = "editgroup/" + groupId;
    jsCarregaConteudoEmDiv(url, "ModalContent", "", "", "", '$("#crModalEditUser").modal("show")');
}

function jsSalvarGrupo()
{
    dados_formulario = jsGetDataFromForm("frmDadosGrupo");
    url = "api/manager/salvardadosgrupo";
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
