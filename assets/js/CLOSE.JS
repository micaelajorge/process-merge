// JavaScript Document
var normal = 0;
function LogOff()
	{
	if (confirm('Sair do Sistema???')) 
		{ 
//		normal=1; document.location='logoff.php'; 
		window.close();
		};
	}
function FecharSessao()
	{
	if (window.name == "ProcessBR")
		{
		if ((window.event.clientY < 0) || (window.event.clientX < 0))
			{
			window.open("logoff.php","","width=300,height=300,left=" + PegaPosX(300) + ",top=" + PegaPosY(300));
			}
		}
	}

function PegaPosX(Width)
	{
	return Math.round(screen.Width / 2 - Width / 2)
	}	
	
function PegaPosY(Height)
	{
	return Math.round(screen.Height / 2 - Height / 2)-50
	}