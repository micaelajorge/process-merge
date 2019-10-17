<?php
include_once(FILES_ROOT . "includes_ws/wserror.inc");

function AuthenticateWs($AuthUser)
{
    global $userdef, $connect;
    include(FILES_ROOT . "include/config.config.php");

    $headers = getallheaders();
    if (key_exists("token", $headers)) {
        $validadoToken = false;
        $token = $headers["token"];
        if ($token != "") {
            $userdef = new userdef;
            $userdef->Create('token', $connect, $token);
            $validadoToken = $userdef->Validado;
            $_SESSION["userdef"] = $userdef;            
        }
    } else {
        if (!isset($userdef)) {
            $userdef = new userdef;
            $userdef->Create($OrigemLogon, $connect, $AuthUser);
            if ($userdef->Validado) {
                return $userdef;
            }
        } else {
            return $userdef;
        }
    }
    GeraErro(4, $OrigemLogon);
    return false;
}
