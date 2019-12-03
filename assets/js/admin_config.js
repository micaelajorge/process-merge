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
        emailNotificationPassword: $("#email_notification_password").val(),
        emailNotificationPort: $("#email_notification_port").val(),
        emailNotificationServer: $("#email_notification_server").val()
    };
    url = "api/v1/secconfig";
    $.post(url, dadosEnvio);
}
