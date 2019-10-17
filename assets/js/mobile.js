/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 02/08/2018
 Sistema: Process_XAMPP
 Funções para operação mobile do sistema
 
 **/

/**
 * Prevenir de entrar em outra página se o menu estiver aberto
 * @returns {Boolean}
 */
function jsPrevineEntradaMenuAberto() 
{
    return !($(document.body).hasClass('sidebar-open'));
}