function jsEditPermissaoProc(procId)
{
    url = "permissoesprocesso/" + procId;
    document.location = url;
}

function jsRelatorioPermissoes(procId)
{
    url = "permissoes/relatorio/" + procId;
    window.open(url);
}

function jsSalvarUsuario()
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
