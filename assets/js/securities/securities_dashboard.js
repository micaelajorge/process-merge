/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


function jsCarregaSinatariosCessao(cessionUUID)
{
    $("#dashboard_lista_assinatutas_overlay").show();
    url = "securities/cession_signors";
    if (cessionUUID !== undefined)
    {
        url += "/" + cessionUUID;
    }
    var jqxhr = $.get(url, function (dadosRetornados) {
        $("#dashboard_lista_assinatutas").html(dadosRetornados);
        $("#dashboard_lista_assinatutas_overlay").hide();
        $("#dashboard_lista_assinatutas_box").removeClass("box-default");
        $("#dashboard_lista_assinatutas_box").addClass("box-primary");
    });
}

$(document).ready(function () {
    jsCarregaSinatariosCessao();
    jsDashboardSecurities_IniciaListaCessoes();
});
