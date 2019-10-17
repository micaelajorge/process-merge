<?php
class CListaOpcoes
{
var $Opcoes;
	function CListaOpcoes();
	function AdicionaOpcoes($Nome, $Desc, $href)
	{
		
	}
}

?>
<link href="../styles/ListaProcesso.css" rel="stylesheet" type="text/css">
<div id="BordaProcessos" style="width:<?= $WidthDiv ?>;height:<?= $HeightDiv ?>">
<div id="ListaProcessos" style="width:<?= $WidthDiv ?>;height:<?= $HeightDiv ?>">
<ul id="lista-opcoes">
<?php
  for ($contador = 0; $contador < count($ListaOpcoes); $contador++)
  	{
	?>
  <li><a href="<?= $ListaOpcoes[$contador]["../href"] ?>" title="<?= $ListaOpcoes[$contador]["Descricao"] ?>"><?= $ListaOpcoes[$contador]["Descricao"] ?> </a></li>
	<?php
	}
	?>  
</ul>
</div>
</div>