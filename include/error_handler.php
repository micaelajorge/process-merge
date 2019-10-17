<?php
//error_reporting(E_);
function CustonError($errno, $errstr, $errfile, $errline)
{
	global $PHP_SELF, $userdef;
	
    $ERRO = "\r\n$PHP_SELF\r\n";
    switch ($errno) {
    case E_USER_ERROR:
        $ERRO .= "ERROR [$errno] $errstr\r\n";
        $ERRO .= "  Fatal error on line $errline in file $errfile";
        $ERRO .= ", PHP " . PHP_VERSION . " (" . PHP_OS . ")\r\n";
        $ERRO .= "Aborting...";
	    error_log("Erro - " . $ERRO);
        break;

    case E_USER_WARNING:
        $ERRO .=  "WARNING [$errno] $errstr";
	    error_log("Erro - " . $ERRO);
		break;

	case 8:
        break;

    case E_USER_NOTICE:
        $ERRO .= "NOTICE [$errno] $errstr";
        error_log("Erro - " . $ERRO);
	    break;

    default:
        $ERRO .= "Unknown error type: [$errno] $errstr\r\nLinha $errline $errfile";
    error_log("Erroo: " . $ERRO);
        break;
    }

    /* Don't execute PHP internal error handler */
    return true;
}

//set_error_handler("CustonError");
?>