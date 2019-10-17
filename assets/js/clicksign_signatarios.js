function jsSignatarioCad_Editar(indiceSignatario)
{
    jsSignatariosCad_LimpaFormulario();
    valorCampo = $('[aria-fieldType="SCC"]').val();
    if (valorCampo === "")
    {
        valorCampo = "{}";
    }
    signatarios = JSON.parse(valorCampo);
    valoresSignatario = signatarios[indiceSignatario];

    keysDados = Object.keys(valoresSignatario);
    for (i = 0; i < keysDados.length; i++)
    {
        key = keysDados[i];
        valorKey = valoresSignatario[key];
        if (typeof valorKey === "object")
        {
            valorKey = JSON.stringify(valorKey);
        }
        $("#" + key).val(valorKey);
    }

    try {
        jsSignatarioCad_MostrarParticipacoes();
    } catch (e) {

    }
    $("#btnSalvarSignatario").html("Salvar");
    jsSignatarioCad_Mostrar();
}

function jsSignatarioCad_GeraHTMLParticipacoes(jsonParticipacoes)
{
    templateParticipacao = new Template('assets/templates/t_signatario_participacao.html');
    templateParticipacao.parseJSON(jsonParticipacoes);
    retorno = templateParticipacao.parse();
    return retorno;
}

function jsSignatarioCad_MostrarParticipacoes()
{
    jlistaParticipacoes = $("#signatario_participacoes").val();
    if (jlistaParticipacoes === "")
    {
        jlistaParticipacoes = "[]";
    }

    participacoes = JSON.parse(jlistaParticipacoes);
    listaParticipacoes = {};
    listaParticipacoes["BLOCK_PARTICIPACAO"] = participacoes;
    htmlParticipacao = jsSignatarioCad_GeraHTMLParticipacoes(listaParticipacoes);
    $("#listaParticipacoes").html(htmlParticipacao);
}

function jsSignatarioCad_adicionaParticipacao()
{
    descricao = $("#sel_particiacao_signatario option:selected").html();
    valorParticipacao = $("#sel_particiacao_signatario").val();
    if (valorParticipacao === "")
    {
        return;
    }

    dadosParticipacao = {};
    dadosParticipacao.PARTICIPACAO = valorParticipacao;
    dadosParticipacao.DESCRICAO = descricao;

    jlistaParticipacoes = $("#signatario_participacoes").val();
    if (jlistaParticipacoes === "")
    {
        jlistaParticipacoes = "[]";

    }
    listaParticipacoes = JSON.parse(jlistaParticipacoes);
    listaParticipacoes.push(dadosParticipacao);

    jlistaParticipacoes = JSON.stringify(listaParticipacoes);
    $("#signatario_participacoes").val(jlistaParticipacoes);
    jsSignatarioCad_MostrarParticipacoes();

}

function jsSignatarioCad_removerParticipacao(participacaoSelecionada)
{
    jparticipacoes = $("#signatario_participacoes").val();
    participacoes = JSON.parse(jparticipacoes);
    novas_participacoes = [];
    for (i = 0; i < participacoes.length; i++)
    {
        participacao = participacoes[i];
        if (participacao.PARTICIPACAO === participacaoSelecionada)
        {
            continue;
        }
        novas_participacoes.push(participacao);
    }
    jparticipacoes = JSON.stringify(novas_participacoes);
    $("#signatario_participacoes").val(jparticipacoes);
    $("#participacao_" + participacaoSelecionada).remove();
}

function jsRemoveSignatario(indiceSignatario, signatarios)
{
    retorno = [];
    j = 0;
    for (i = 0; i < signatarios.length; i++)
    {
        if (signatarios[i].indiceSignatario == indiceSignatario)
        {
            continue;
        }
        signatarios[i].indiceSignatario = j;
        retorno.push(signatarios[i]);
        j++;
    }
    return retorno;
}

function jsSignatarioCad_Remover(indiceSignatario)
{
    valorCampo = $('[aria-fieldType="SCC"]').val();
    if (valorCampo === "")
    {
        valorCampo = "{}";
    }
    signatarios = JSON.parse(valorCampo);

    novoSignatarios = jsRemoveSignatario(indiceSignatario, signatarios);
    valorCampo = JSON.stringify(novoSignatarios);
    $("#t7").val(valorCampo);
    jsRecarregarSignatarios();
}


function jsRecarregarSignatarios()
{
    valorCampo = $('[aria-fieldType="SCC"]').val();
    if (valorCampo === "")
    {
        valorCampo = "{}";
    }
    signatarios = {};
    dados = JSON.parse(valorCampo);
    dados_edit = dados;
    signatarios.BLOCK_SIGNATARIO = dados;
    templateLista = new Template('assets/templates/t_signatarios_lista.html');
    templateLista.parseJSON(signatarios);
    var parsed = templateLista.parse();
    $("#divListaSignatariosCaso").html(parsed);
    $("#divListaSignatariosEdicao").html(parsed);
}

/**
 * Incluir Siganatario na Lista
 * @returns {undefined}
 */
function jsSignatariosCad_Inclui()
{
    objetosCadastro = $("#signatarioCadastro").find("input, select");

    // Pega os signatarios já cadastrados
    jValorCampo = $('[aria-fieldType="SCC"]').val();
    if (jValorCampo === "")
    {
        jValorCampo = "[]";
    }
    ValorCampo = JSON.parse(jValorCampo);

    valores = {};
    for (i = 0; i < objetosCadastro.length; i++)
    {
        nomeObjeto = $(objetosCadastro[i]).attr("name");
        if (nomeObjeto === "signatario_participacoes")
        {
            jvalorObjeto = $(objetosCadastro[i]).val();
            if (jvalorObjeto === "")
            {
                jvalorObjeto = "[]";
            }
            valorObjeto = JSON.parse(jvalorObjeto);
        } else {
            valorObjeto = $(objetosCadastro[i]).val();
        }

        valores[nomeObjeto] = valorObjeto;
    }

    if (valores["indiceSignatario"] === "-1")
    {
        valores.indiceSignatario = ValorCampo.length;
        ValorCampo.push(valores);
    } else {
        ValorCampo[valores.indiceSignatario] = valores;
    }

    jValorCampo = JSON.stringify(ValorCampo);
    $('[aria-fieldType="SCC"]').val(jValorCampo);
}

// Novo Signatario
function jsSignatarioCad_Novo()
{
    jsSignatariosCad_LimpaFormulario();
    $("#indiceSignatario").val(-1);
    $("#btnSalvarSignatario").html("Incluir");
    $("#listaParticipacoes").html("");
    jsSignatarioCad_Mostrar();
}

// Salvar Signatario
function jsSignatarioCad_Incluir()
{
    if (!jsValidaDadosSignatario())
    {
        return;
    }
    jsSignatariosCad_Inclui();
    jsSignatarioCad_Cancelar();
    jsRecarregarSignatarios();
}

function jsSignatariosCad_LimpaFormulario()
{
    $("#signatarioCadastro").find(':input, select')
            .val('')
            .removeAttr('checked')
            .removeAttr('selected');
    $("#listaParticipacoes").html('');
}

function jsSignatarioCad_Mostrar()
{
    $("#signatarioLista").hide();
    $("#signatarioCadastro").show();
    $("#signatario_nome").focus();
    //Bloqueia fechar o MODAL quando clicar fora da Janela
    $('#modalSignatarios').modal({backdrop: 'static', keyboard: true});
}

function jsSignatarioCad_Cancelar()
{
    $('#modalSignatarios').modal();
    $("#signatarioCadastro").hide();
    $("#signatarioLista").show();

    //Libera fechar o MODAL quando clicar fora da Janela    
    $("#modalSignatarios").removeAttr("data-backdrop");
    $("#modalSignatarios").removeAttr("data-keyboard");
}
