<?php
// //Versao 1.0.0 /Versao
include(FILES_ROOT . "include/common.php");
require_once(FILES_ROOT . "include/resource02.php");

$SessionId = md5(session_id());
$userdef = $_SESSION["userdef"];
RemoveLocksTemporarios($userdef->UserId);
if ($CriarSessionIntegration) {
    switch ($OrigemLogon) {
        case "ProcessLogon":
            if (!isset($ALTERNATIVE_SERVER_userlogins)) {
                $connect = AtivaDBExterno(true);
            } 
            $SQL = " delete from userlogins where UserId = $userdef->UserId or SessionId = '$SessionId'";
            $Result = mysqli_query($connect, $SQL);
            break;
        case "External":
            $connect = AtivaDBExterno();
            $SQL = " delete from userlogins where UserId = $userdef->UserId or SessionId = '$SessionId'";
            $Result = mysqli_query($connect, $SQL);
            break;
    }
}

session_destroy();
session_write_close();
setcookie(session_name(), '', 0, '/');
session_regenerate_id(true);
$_SESSION = array();
if ($OrigemLogon == "ProcessLogon" or $OrigemLogon == "Server") {
    header("location: logon");    
    exit;
}
?>
<html>
    <head>
    </head>
    <body>
        Sistema Encerrado
    </body>
</html>