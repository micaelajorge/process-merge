<?php
$efetuarLogon = false;
if (!isset($_SESSION["userdef"]) & empty($Direct)) {
    $userdef_Registrado = isset($_SESSION["userdef"]);
    if ($dadosURL != "logon" & $dadosURL != "log")
    {
    header("location: " . SITE_ROOT. "/logon");
    exit;
    }    
}
?>