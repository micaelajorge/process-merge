<?php
include("Toolbarcreate.php");
if (!empty($Menu))
	{
	for ($i = 0; $i < 5; $i++)
		{
		if ($AcionaMenu[$i])
			{
			$Botoes .= "<li>" . $Link[$i] . "</li>";
			$BotoesFloat .= $Link_Float[$i] . "</br>";
			}
		}
	}
?> 
<div class="botoes">
	<ul class="navs"><?= $Botoes ?></ul>
	<ul class="nvoltar"><LI><?= $Link[5]; ?></li></ul>
</div>
<div id="MenuFloat" style="position:absolute; width:108px; height:50px; z-index:1; left: 126px; top: 83px;display:none" onMouseOver="overContextMenu = true;" onMouseOut="overContextMenu = false;" > 
<?= $BotoesFloat ?>
</div>