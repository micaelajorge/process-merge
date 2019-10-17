<?php
require_once("include/common.php");
require_once("include/ldap.php");
include("carga.php");
flush();
if ($Filtro <> "")
	{
	switch ($OrigemLogon)
		{
		case "AD":
			if ($Grupo <> 0)
				{
				$GrupoSam = PegaSamAccountNameGrupo($Grupo);
				$Users = PegaMembrosGrupo($GrupoSam, $Filtro);
				}
			else
				{
				$Users = PegaUsuariosAD($Filtro);				
				}
			break;
		case "ProcessLogon":
			$Users = PegaUsuariosProcess($Filtro, $Grupo);					
			break;
		case "External":
			$Users = PegaUsuariosExterno($Filtro, $Grupo);
			break;		
		}
		
	$Total = count($Users);
	}
	
if ($Total < 10)
	{
	$Espaco = "<span class=\"CelOp\"></span>";
	}
?>
<html>
<head>
<title>Sele&ccedil;&atilde;o de Usu&aacute;rio</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link href="STYLES/TableRazed.css" rel="stylesheet" type="text/css">
<link href="STYLES/flow.css" rel="stylesheet" type="text/css">
<style type="text/css">
<!--
.DivLista {
	overflow: auto;
	height: 250px;
	width: 700px;
	border: none;
}
-->
</style>
<style type="text/css">
<!--
.DivPanel {
	width: 516px;
}
-->
</style>
<style type="text/css">
<!--
.DivMargem {
	padding: 5px;
}
.TableLista {
	border: none;
}
-->
</style>
<style type="text/css">
<!--
.ItemLista {
	padding: 3px;
}
.CelulaLista {
	padding: 3px;
	font-family: Arial, Helvetica, sans-serif;
	font-size: 12px;
	color: #003399;
}
-->
</style>
<style type="text/css">
<!--
.CelNome {
	width: 260px;
	border: 2px outset;
	height: 25px;
	padding: 3px;
	font-family: Arial, Helvetica, sans-serif;
	font-size: 14px;
}
.CelDesc {
	width: 409px;
	border: 2px outset;
	height: 25px;
	padding: 3px;
	font-family: Arial, Helvetica, sans-serif;
	font-size: 14px;
}
.CelOp {
	width: 15px;
	border: 2px outset;
	height: 25px;
	padding: 3px;
	font-family: Arial, Helvetica, sans-serif;
	font-size: 14px;
	font-weight: bold;
}
-->
</style>
<style type="text/css">
<!--
.DivExp {
	border: inset;
	overflow: auto;
	margin: 5px;
}
.DivTitulos {
	overflow: auto;
	width: 700px;
	border: none;
	font-family: Arial, Helvetica, sans-serif;
	font-size: 14px;
	color: #0033FF;
	font-weight: bolder;
}
-->
</style>
<style type="text/css">
<!--
.LinhaSel {
	background-color: #0033FF;
	font-family: Arial, Helvetica, sans-serif;
	font-size: 14px;
	color: #FFFFFF;
}
-->
</style>
<script language="JavaScript">
var selLinha = "";
var uValor = "<?= $Nome ?>"; 
var dValor;
function Sel(Linha, valor, Display)
	{
	uValor = valor;
	dValor = Display;
	if (selLinha != "")
		{
		unsel = document.getElementById(selLinha);
		unsel.className = "";		
		}
	sel = document.getElementById(Linha);
	sel.className = "LinhaSel";
	selLinha = Linha;
	}
</script>
</head>

<body onLoad="document.getElementById('Filtro').focus()">
<div class="DivMargem">
<div class="TableRaized">
<div class="DivExp">
<div class="DivTitulos">
<span class="CelOp"></span><span class="CelNome">Nome</span><span class="CelDesc">Descrição</span><span class="CelOp"></span>
</div>
<div class="DivLista">
<?php
for ($i = 0; $i < $Total; $i++)
	{
	$NomeLista = $Users[$i]["Nome"];
	$accountLista = $Users[$i]["samaccountname"];
	if (!empty($Users[$i]["UserId"])){
		$valor = $Users[$i]["UserId"];
	}
	else {
		$valor = $Users[$i]["samaccountname"];
	}
	if ($Nome == $accountLista)
		{
		$classe = "LinhaSel";
		$ItemSel = $i;
		}
	?>		
	<div onclick="Sel('Linha<?=$i ?>', '<?= $valor ?>', '<?= $NomeLista ?>')" class="<?= $classe ?>" id="Linha<?=$i ?>"><span class="CelOp"></span><span class="CelNome"> 
	<?= $accountLista ?>
	</span><span class="CelDesc"> 
	<?= $NomeLista ?>
	</span>
	<?= $Espaco ?>
	</div>
	<?php
	$classe = "";
	}
	?>
</div>
   </div>
    <div class="DivPanel"> 
      <table width="100%" border="0">
        <tr> 
          <td width="4%">&nbsp;</td>
          <td colspan="2">&nbsp;</td>
          <td width="7%">&nbsp;</td>
        </tr>
        <tr> 
          <td>&nbsp;</td>
          <td colspan="2"> <div align="center"> 
              <form>
                <input name="Filtro" type="text" id="Filtro" tabindex="0" size="50" maxlength="20" value="<?= $Filtro ?>">
                <input type="submit" name="Submit3" value="Localizar">
                <input type="hidden" name="Grupo" value="<?= $Grupo ?>">
                <input type="hidden" name="Nome" value="<?= $Nome ?>">
			  </form>
            </div></td>
          <td>&nbsp;</td>
        </tr>
        <tr> 
          <td colspan="2"> <div align="center"> 
              <input name="Submit22" type="button" class="buttonOk" value="Ok" onclick="window.opener.AtualizaUser(uValor, dValor); window.close()">
            </div></td>
          <td colspan="2"><div align="center"> 
              <input name="Submit2" type="button" class="buttonCancel" value="Cancelar" onclick="window.close()">
            </div></td>
        </tr>
        <tr> 
          <td>&nbsp;</td>
          <td colspan="2"><div align="center"> </div>
            <div align="center"> </div></td>
          <td>&nbsp;</td>
        </tr>
      </table>
    </div>
</div>
</div>
<?php
	if (isset($ItemSel))
		{
		echo "<script language=\"javascript\"> selLinha = \"Linha$ItemSel\"</script>";
		}
?>
</body>
</html>
<?php include("cargafooter.html"); ?>