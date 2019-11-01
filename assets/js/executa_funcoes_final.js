/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 31/10/2019
 Sistema: Process_XAMPP
 */

function jsExecutaFuncoesFinal()
{
    if (typeof funcoes_final === "undefined")
    {
        return;
    }
    if (Array.isArray(funcoes_final))
    {
        for (let i = 0; i < funcoes_final.length; i++)
        {
            funcoes_final[i]();
        }        
    }
}

$(document).ready(function () {
    jsExecutaFuncoesFinal();
});