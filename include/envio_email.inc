<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 05/12/2019
  Sistema: Process
 */

//Import PHPMailer classes into the global namespace
use PHPMailer\PHPMailer\PHPMailer;
use PHPMailer\PHPMailer\SMTP;

require FILES_ROOT . 'vendor/autoload.php';

/**
 * 
 * @param type $emailFrom
 * @param type $emailAddress
 * @param type $emailSubject
 * @param type $bodyMessage
 */
function EnviarEmail($emailFrom, $emailAddress, $emailSubject, $bodyMessage)
{

    $dadosServidor = pegaDadosServidorEmail();

    $mail = new PHPMailer;
    $mail->isSMTP();

    //Enable SMTP debugging
    // SMTP::DEBUG_OFF = off (for production use)
    // SMTP::DEBUG_CLIENT = client messages
    // SMTP::DEBUG_SERVER = client and server messages
//    $mail->SMTPDebug = SMTP::DEBUG_SERVER;
    $mail->SMTPDebug = SMTP::DEBUG_OFF;
    $mail->Host = $dadosServidor["emailNotificationServer"];

    $mail->Port = $dadosServidor["emailNotificationPort"];
    //Set the encryption mechanism to use - STARTTLS or SMTPS
    $mail->SMTPSecure = PHPMailer::ENCRYPTION_STARTTLS;
    //Whether to use SMTP authentication
    $mail->SMTPAuth = true;
    
    //Username to use for SMTP authentication - use full email address for gmail
    $mail->Username = $dadosServidor["emailNotificationUser"];
//Password to use for SMTP authentication
    $mail->Password = $dadosServidor["emailNotificationPassword"];
//Set who the message is to be sent from
    $mail->setFrom($emailFrom["address"], $emailFrom["nome"]);
////Set an alternative reply-to address
//    $mail->addReplyTo('replyto@example.com', 'First Last');
//Set who the message is to be sent to
    $mail->addAddress($emailAddress["address"], $emailFrom["nome"]);
//Set the subject line
    $mail->Subject = $emailSubject;
//Read an HTML message body from an external file, convert referenced images to embedded,
//convert HTML into a basic plain-text alternative body
    $mail->msgHTML($bodyMessage);
//Replace the plain text body with one created manually
    $mail->AltBody = 'Texto Mensagem';
//Attach an image file
//    $mail->addAttachment('images/phpmailer_mini.png');
//send the message, check for errors
    if (!$mail->send()) {
        echo 'Mailer Error: ' . $mail->ErrorInfo;
    } else {
        echo 'Message sent!';
        //Section 2: IMAP
        //Uncomment these to save your message in the 'Sent Mail' folder.
        #if (save_mail($mail)) {
        #    echo "Message saved!";
        #}
    }
}
