/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 11/01/2019
 Sistema: Process_XAMPP
 */

function jsMudaStatusAutomato(novoStatus)
{
    url = "api/automatomudastatus";
    dadosEnvio = {
        statusAutomato: !!novoStatus
    };
    $.ajax({
        url: url,
        method: "POST",
        data: dadosEnvio
    })
            .done(function (retorno) {
                textoStatusAutomato = "Status Automato mudado para: ";
                if (retorno.statusAutomado)
                {
                    textoStatusAutomato = textoStatusAutomato + "Pausado";
                } else {
                    textoStatusAutomato = textoStatusAutomato + "Executando";
                }
                alert(textoStatusAutomato);
                location.reload();
            })
            .fail(function (retorno) {
                // Falha ao gravar
                alert(retorno);
            });
}