/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


function jsSalvaConfigSeguranca()
{
    dadosEnvio = {
        maxFalhaLogon: $("#max_falha_logon").val(),
        mesesInatividade: $("#meses_iniatividade").val(),
        emailNotificationUser: $("#email_notification_user").val(),
        emailNotificationDesc: $("#email_notification_desc").val(),      
        emailNotificationPassword: $("#email_notification_password").val(),
        emailNotificationPort: $("#email_notification_port").val(),
        emailNotificationServer: $("#email_notification_server").val(),
        diasTrocaSenha: $("#dias_troca_senha").val()
        
    };
    url = "api/v1/secconfig";
    $.post(url, dadosEnvio);
}
