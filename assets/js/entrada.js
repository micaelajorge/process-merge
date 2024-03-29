/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 01/02/2019
 Sistema: Process_XAMPP
 */

function jsMostraTodosProcessos()
{
    if ($(".PROC_VAZIO").hasClass("hidden"))
    {
    $(".PROC_VAZIO").removeClass("hidden");
    } else {
        $(".PROC_VAZIO").addClass("hidden");
    }
}

function jsFiltraProcessoEntrada()
{
    valorFiltro = $("#txtFiltroProcessos").val().toLowerCase();
    $(".selProc").css("display", "");
    if (valorFiltro.length > 0)
    {
        $(".selProc:not(:contains('" + valorFiltro + "'))").css("display", "none");
    }
    return true;
}

function jsSelectWorkSpace(workSpaceSel)
{
    // Seta o cookie de workSpace
    setCookie('workSpace', workSpaceSel);       
    
    workSpaceName = workSpaceSel.replace(/ /, '-');
    if (workSpaceSel === "")
    {
        $(".PROCESS_VIEW").show();
        $("#TOOLBAR-BUTTON-WORKSPACES").find(".toolbar-button-text").html("Workspaces");
    } else {
        $(".PROCESS_VIEW").show();
        $(".PROCESS_VIEW:not(." + workSpaceName).hide();            
        $("#TOOLBAR-BUTTON-WORKSPACES").find(".toolbar-button-text").html("Workplace - " + workSpaceSel);
    }    
}