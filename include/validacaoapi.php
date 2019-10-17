<?php
    $sSessionId = $_COOKIE["PHPSESSID"];
    $SessionId = md5($sSessionId);
    switch ($OrigemLogon)
    {
        case "ProcessLogon":
            $connect = AtivaDBProcess();
            $SQL = "select UserId from userlogins where SessionId = '$SessionId'";
            $Query = mysqli_query($connect, $SQL);
            $numRows = mysqli_num_rows($Query);
            if ($numRows == 0)
            {
                header("HTTP/1.0 403 Nao Validado - ProcessLogon");
                die();
            }
            break;
        
        case "External":            
            $connect = AtivaDBExterno();
            $SQL = "select UserId from userlogins where SessionId = '$SessionId'";
            $Query = mysqli_query($connect, $SQL);
            if (mysqli_num_rows($Query) == 0)
            {
                header("HTTP/1.0 403 Não Validado");
                die();
            }
            AtivaDBProcess();
            break;
    }