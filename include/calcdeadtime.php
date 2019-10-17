<?Php
/**
 * 
 * @param type $Start
 * @param type $DeadTime
 * @param type $Entrada
 * @param type $Saida
 * @param type $Jornada
 * @return type
 */
function calcula_DeadDateTime($Start, $DeadTime, $Entrada, $Saida, $Jornada)
{
    $DeadDateTime = 0;

    if ($Jornada == 0) {
        return incTime($Start, $DeadTime, 1);
    }

    $Start_Valido = incTime($Start, 0);

    if ($Start_Valido <> $Start) {
        $Start_Valido = SetaHoraDia(PegaData($Start_Valido), PegaHora($Entrada));
        $Start = $Start_Valido;
    }
    $Entrada = PegaHora($Entrada);
    $Saida1 = PegaHora($Saida);
    $DtEntrada = SetaHoraDia($Start, PegaHora($Entrada));
    $DtSaida = SetaHoraDia($Start, PegaHora($Saida1));

    if (date_diff_i($Start, $DtEntrada) > 0)
        $Start = $DtEntrada;
    if (date_diff_i($DtSaida, $Start) > 0)
        $Start = incTime($DtEntrada, 1440);

    $DtEntrada = SetaHoraDia($Start, $Entrada);
    $DtSaida = SetaHoraDia($Start, $Saida);
    $dif_entrada = date_diff_i($Start, $DtEntrada);
    $dif_saida = datediff($DtSaida, $Start);
    while (($dif_entrada > 0) | ($dif_saida > 0)) {
        $dif_saida = date_diff_i($Start, $DtSaida);
        if ($dif_saida <= $DeadTime) {
            $DeadTime = $DeadTime - date_diff_i($Start, $DtSaida);
            $Start = incTime($DtEntrada, 1440);
            $DtEntrada = SetaHoraDia($Start, $Entrada);
            $DtSaida = SetaHoraDia($Start, $Saida);
        } else {
            $DeadDateTime = incTime($Start, $DeadTime);
        }
        $dif_entrada = date_diff_i($Start, $DtEntrada);
        $dif_saida = datediff($DtSaida, $Start);
    }
    $DeadDateTime = incTime($Start, $DeadTime);
    return $DeadDateTime;
}

/**
 * 
 * @param type $data
 * @param type $hora
 * @return type
 */
function SetaHoraDia($data, $hora)
{
    // Separa Data da Hora
    list($Date, $Time) = explode(" ", $data);
    return $Date . " " . $hora;
}

/**
 * 
 * @param type $Data
 * @return type
 */
function PegaData($Data)
{
    list($Date, $Time) = explode(" ", $Data);
    return $Date;
}

/**
 * 
 * @param type $Data
 * @return type
 */
function PegaHora($Data)
{
    list($Date, $Time) = explode(" ", $Data);
    if (empty($Time)) {
        $Time = $Date;
    }
    return $Time;
}

?>