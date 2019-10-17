<?php
include("Toolbarcreate.php");
echo "\t\t<TOOLBAR>\n";
for ($i = 0; $i < 5; $i++)
{
		if ($AcionaMenu[$i])
		{
			echo "\t\t<BOTAO>\n";
			echo "\t\t\t<ACAO>" . $acao[$i] . "</ACAO>\n";
			echo "\t\t\t<TEXTO>" . $texto[$i] . "</TEXTO>\n";
			if (!empty($Imagem[$i]))
			{
				echo "\t\t\t<IMAGEM>" . $Imagem[$i] . "</IMAGEM>\n";
			}
			echo "\t\t</BOTAO>\n";
		}	
}
echo "\t\t<BtVoltar>\n";
echo "\t\t\t<ACAO>" . $acao[5] . "</ACAO>\n";
echo "\t\t\t<TEXTO>" . $texto[5] . "</TEXTO>\n";
if (!empty($Imagem[5]))
{
	echo "\t\t\t<IMAGEM>" . $Imagem[5] . "</IMAGEM>\n";
}
echo "\t\t</BtVoltar>\n";
echo "\t</TOOLBAR>\n";
?>