// //Versao 1.0.0 /Versao
// JavaScript Document
var WindowH;
var WindowX;

function PegaPosX(Width)
	{
	return Math.round(screen.Width / 2 - Width / 2)
	}	
	
function PegaPosY(Height)
	{
	return Math.round(screen.Height / 2 - Height / 2) - 20
	}

function PegaDimensoes()
	{
	if ( typeof( window.innerWidth ) == 'number' ) //Non-IE
		{
        WindowW = window.innerWidth;
    	WindowH = window.innerHeight;
  		} 
	else 
		{
    	if ( document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) ) //IE 6+ in 'standards compliant mode'
			{    
      		WindowW = document.documentElement.clientWidth;
      		WindowH = document.documentElement.clientHeight;
    		} 
		else 
			{
      		if ( document.body && ( document.body.clientWidth || document.body.clientHeight ) ) //IE 4 compatible
				{
                WindowW = document.body.clientWidth;
        		WindowH = document.body.clientHeight;
      			}
    		}
  		}  
	}
	
function Posiciona()
	{
	//PegaDimensoes()
	//window.moveTo(Math.round(screen.Width / 2 - WindowW / 2)-10,Math.round(screen.Height / 2 - WindowH / 2)-50); 
	}