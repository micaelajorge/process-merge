/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 23/09/2019
 Sistema: Process_XAMPP
 */

function js_voto_cotista(itemVoto, votoEfetuado)
{
    // Verifica qual é a qualificação do voto
    $("#voto" + itemVoto).removeClass("text-green");
    $("#voto" + itemVoto).removeClass("text-orange");
    
    switch (votoEfetuado)
    {
        case "sim":
            $("#voto" + itemVoto).addClass("text-green");
            textoVoto = "Sim";
            break;
        case "nao":
            $("#voto" + itemVoto).addClass("text-orange");
            textoVoto = "Não";
            break;
        case "abster":
            textoVoto = "Abster-se";
            break;
    }
    
    // Seta o valor do campo
    $("#voto" + itemVoto).html(textoVoto);  
    $("#RESULTADO_VOTO_" + itemVoto).html(textoVoto);  
    
    js_valida_votos(itemVoto, votoEfetuado);
}

function js_valida_votos(itemVoto, votoEfetuado)
{
    json_votos = $("#votos").val();
    votosComputados = JSON.parse(json_votos);
    
    let votoAtual = {
        item: itemVoto,
        voto: votoEfetuado
    };
    votosComputados.push(votoAtual);

    json_votosComputados = JSON.stringify(votosComputados);
    $("#votos").val(json_votosComputados);
    $("#VOTOS_EFETUADOS").html(votosComputados.length);

    // Habilita o botão de encerrar se todos os votos foram efetuados
    if (votosComputados.length == $("#totalVotos").val())
    {
        $("#btnEncerraVoto").removeAttr("disabled");
    }    
}