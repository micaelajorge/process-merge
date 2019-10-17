<? 
/* 
P é o dia da Pascoa 
Domingo de Carnaval P - 49 dias 
Terça-feira de Carnaval P - 47 dias 
Quarta-feira de Cinzas P - 46 dias 
Domingo de Ramos P - 7 dias 
Sexta-feira da Paixão P - 2 dias 
Corpus Christi P + 60 dias 
*/ 

function DatasMoveis($Y)
{
	$G = ($Y % 19) + 1; // Numero Áureo 
	$C = intval(($Y/100) + 1); // Seculo 
	$X=  intval(((3*$C)/4) - 12); // Primeira Correção 
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

echo "O dia da Pascóa é :".date("Y-m-d",mktime (0, 0, 0, $diames , $diapascoa, $Y))."<br>";//Pascoa 
echo "O dia da Domingo de Carnaval é :".date("d  F Y ,l",mktime (0, 0, 0, $diames , $diapascoa-49, $Y))."<br>";//Domingo de Carnaval 
echo "Domingo de Ramos é no dia :".date("d  F Y ,l",mktime (0, 0, 0, $diames , $diapascoa-7, $Y))."<br>";//Domingo de Ramos 
echo "Sexta-feira da Paixão é no dia:".date("d  F Y ,l",mktime (0, 0, 0, $diames , $diapascoa-2, $Y))."<br>";//Sexta-feira da Paixão 
echo "Corpus Christi é no dia:".date("d  F Y ,l",mktime (0, 0, 0, $diames , $diapascoa+60, $Y))."<br>";//Corpus Christi 
?> 