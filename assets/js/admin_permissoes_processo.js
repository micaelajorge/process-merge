function jsAdicionarPermissaoProc(procId, stepId, stepName)
{
    url = "permissao/processo/adicionar/" + procId + "/" + stepId + "/" + stepName + "/UG/";
    jsCarregaConteudoEmDiv(url, "ModalContent", "", "", "", '$("#crModalEditPermissoes").modal("show")');    
}

function jsClickPermissaoEditar()
{
    if (!$("#EDIT_ITEM")[0].checked)
    {
        $("#CRIAR_NOVO")[0].checked = false;
        $("#CRIAR_NOVO")[0].disabled = true;
    } else {
        $("#CRIAR_NOVO")[0].disabled = false;
    }
}

function jsKeyUpFiltro(event)
{
if (event.keyCode === 13) {
    // Cancel the default action, if needed
    event.preventDefault();
    // Trigger the button element with a click
    $("#btnEfetuaBusca").click();
  }    
}

function jsRemoverEntidade(entidade)
{
    if (!confirm("Remover as permissões de '" + entidade + "'?"))
    {
        return;
    }
    $("#ADDEDIT").val("D");    
    jsSalvarPermissoes();
}

function jsSalvarPermissoes()
{
    url = "api/permissoes/entidade";
    dadosFormulario = jsGetDataFromForm("frmDadosEntidade");
    $.ajax({
        url: url,
        method: "POST",
        data: dadosFormulario
    }, dadosFormulario)
            .done(function (retorno) {
                // Sucesso em gravar
                datatablePermissoes.ajax.reload();
                $("#crModalEditPermissoes").modal("hide");
            })            
            .fail(function (retorno) {
                // Falha ao gravar
                alert(retorno);
            });
}

function jsEditPermissoesEntidade(procId, stepId, entidadeId, grpFld, entidade, stepName)
{
    url = "permissao/processo/" + procId + "/" + stepId + "/" + entidadeId + "/" + grpFld + "/" + entidade + "/" + stepName;
    jsCarregaConteudoEmDiv(url, "ModalContent", "", "", "", '$("#crModalEditPermissoes").modal("show")');
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
