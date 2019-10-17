<?php
function incTime($Data, $Minutos, $HorasCorridas = 0)
	{	
	$Dias = 0;
	if ($Minutos > 1440)
		{
		$Dias = $Minutos / 1440;
		$Minutos =  $Minutos % 1440;		
		}		
	if ($Minutos != 0)
		{
		$Data = incMinutos($Data, $Minutos);
		}
		
	if ($HorasCorridas == 1)
		{
		return incDays($Data,$Dias);
		}
		
    if ($Dias != 0)
		{
      	for ($contador = 1; $contador <= $Dias; $contador++)
			{
	        $Data = incDays($Data,1);
        	while (!DiaUtil($Data))
				{
          		$Data = incDays($Data, 1);
				}
      		}
		}
    while (!DiaUtil($Data))
		{
      	$Data = incDays($Data,1);
		}
	return $Data;
	}
	
function incDays($Data, $dias)
	{
	$Data = explode(" ", $Data);
        

	$Horas = explode(":", $Data[1]);
        if (count($Horas) == 1)
        {
            $Horas[0] = 0;
            $Horas[1] = 0;
        }
	$Data = explode("-", $Data[0]);   
        $retorno = date("Y-m-d H:i", mktime($Horas[0], $Horas[1], 0, $Data[1], $Data[2] + $dias, $Data[0]));
	return 	$retorno;
	}

function incMinutos($Data, $Minutos)
	{
	$Data = explode(" ",$Data);
	$Horas = explode(":",$Data[1]);
	$Data = explode("-",$Data[0]);
	return date("Y-m-d H:i", mktime($Horas[0], $Horas[1] + $Minutos,0,$Data[1], $Data[2], $Data[0]));
	}
	
function DiaUtil($Data)
	{
	$Retorno = true;
	$Data1 = explode(" ",$Data);
	$Horas = explode(":",$Data1[1]);
	$Data1 = explode("-",$Data1[0]);
	$Time = mktime($Horas[0], $Horas[1] ,0, $Data1[1], $Data1[2], $Data1[0]);
	if ((date("w", $Time) == 0) || (date("w", $Time) == 6))
		{
		return false;
		}
	if (Feriado($Data))
		{
		return false;
		}
	return true;
	}
	
	
function Feriado($data)
	{
	global $connect;
	$data = explode(" ",$data);
	$data = explode("-",$data[0]);	
	$Mes = $data[1];
	$Ano = $data[0];
	$Dia = $data[2];
	$SQL = "select * from feriadosdef where (Dia = $Dia and Mes = $Mes and Ano = 0) or (Dia = $Dia and Mes = $Mes and Ano = $Ano)";
	$Query = mysqli_query($connect, $SQL);
	if (mysqli_num_rows($Query) > 0) 
		{
		return true;
		}
	else
		{
		return false;
		}
	}


function DatasMoveis($Y)
{
	$G = ($Y % 19) + 1; // Numero �ureo 
	$C = intval(($Y/100) + 1); // Seculo 
	$X=  intval(((3*$C)/4) - 12); // Primeira Corre��o 
	$Z = intval((((8*$C)+5)/25) -5);//Epacta 
	$E = ((11*$G) + 20 + $Z - $X) % 30; 
	
	if((($E == 25) AND ($G > 11)) OR ($E == 24)){ 
	   $E+=1; 
	} 
	$N=44-$E; // Lua Cheia 
	if($N < 21){ 
	   $N+=30; 
	} 
	$D=intval(((5*$Y)/4)) -($X + 10);//Domingo 
	$N=($N+7)-($D+$N)%7; 
	if($N > 31){ 
	  $diapascoa=$N-31; 
	  $diames=4; 
	}else{ 
	  $diapascoa=$N; 
	  $diames=3; 
	} 
	$Datas[1] = date("Y-m-d",mktime (0, 0, 0, $diames , $diapascoa - 2, $Y));
	$Datas[2] = date("Y-m-d",mktime (0, 0, 0, $diames , $diapascoa-2, $Y));
	$Datas[3] = date("Y-m-d",mktime (0, 0, 0, $diames , $diapascoa + 60, $Y));
	return $Datas;
}
?>