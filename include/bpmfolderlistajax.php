<?php
// //Versao 1.0.0 /Versao
$gmtDate = gmdate("D, d M Y H:i:s"); 
header("Expires: {$gmtDate} GMT"); 
header("Last-Modified: {$gmtDate} GMT"); 
header("Cache-Control: no-cache, must-revalidate"); 
header("Pragma: no-cache");
header("Content-Type: text/html; charset=ISO-8859-1",true);
require_once("include/common.php");
include("include/validasessao.php");
function DownloadFile($filename) 
	{
    // Check filename 
    if (empty($filename) || !file_exists($filename)) 
    	{ 
        return FALSE; 
    } 
    // Create download file name to be displayed to user 
    $saveasname = basename($filename); 
    // Send binary filetype HTTP header 
    header('Content-Type: application/octet-stream'); 
    // Send content-length HTTP header 
    header('Content-Length: '.filesize($filename)); 
    // Send content-disposition with save file name HTTP header 
    header('Content-Disposition: attachment; filename="'.$saveasname.'"'); 
    // Output file 
    readfile($filename); 
    // Done 
    return TRUE; 
} 
require_once("include/resource01.php");
$SQL = "select FieldValue, ValueId from casedata where ProcId = $ProcId and FieldId = $FieldId and CaseNum = $CaseNum order by ValueId desc";
$Query = mysqli_query($connect, $SQL);
$Incluir = "BPMFolderFile.php?ProcId=$ProcId&CaseNum=$CaseNum&FieldId=$FieldId";
$DirName = formataCaseNum($ProcId, 3) . formataCaseNum($CaseNum,8);

$t_body->MAXFILESIZE = ini_get('upload_max_filesize');
$t_body->PROCID = $ProcId;
$t_body->CaseNum = $CaseNum;

  $contador = 0;
  while ($Linha = mysqli_fetch_array($Query)) {
    $Arquivo = $Linha["FieldValue"];
    $Arquivo = explode(';', $Arquivo);
    $ValueId = $Linha["ValueId"];
    $Abrir = $Arquivo[0];
    $t_body->$Excluir = "BPMFolderSendFile.php?ProcId=$ProcId&StepId=$StepId&CaseNum=$CaseNum&FieldId=$FieldId&ValueId=$ValueId&Arquivo=" . str_replace(" ", '%20', $Arquivo[0]);
    $t_body->ABRIR = $Abrir;
    $t_body->ARQUIVO_1 = $Arquivo[1];
    $t_body->ARQUIVO_2 = $Arquivo[2];
    $t_body->ARQUIVO_3 = $Arquivo[3];
    if ($ReadOnly <> 1) {
        $ARQUIVO_4 = $Arquivo[4];
        if ($ReadOnly <> 1) {
            $t_body->block("BLOCK_APAGAR_ARQUIVO");
        }
    }
    if ($ReadOnly != 1) {
        $t_body->block("BLOCK_OPCOES_EDIT");
    }
    ?>
<tr> 
          <td align="center">
          <a onMouseMove="window.status='Abrir Arquivo'"  onMouseOut="window.status=''" target="_blank" href="BPMViewFile.php?ProcId=<?= $ProcId ?>&CaseNum=<?= $CaseNum ?>&Target=<?= htmlspecialchars($Abrir) ?>" class="MVHistorico"><?= htmlentities($Abrir) ?></a>
          </td>
          <td align="center"><?= $Arquivo[1] ?></td>
          <td align="center"><?= $Arquivo[2] ?></td>
          <td align="center"><?= $Arquivo[3] ?></td>
          <td align="center"><?= $Arquivo[4] ?></td>
		  <td align="center"><?php if ($ReadOnly <> 1) { ?>
                      <a target="iframeFolder" onclick="return ApagarArquivo('<?= htmlentities($Abrir) ?>')" href="<?= $Excluir ?>">Excluir</a> <?php } ?>
          </td>
        </tr>
        <?php
  	}
	?>
   </table>    
<?php
if ($ReadOnly == 0)
	{
	?>

<?php
	}
	?>
	<div class="DivFolderListFilesClose">
		<a href="javascript:" onClick="EscondeFolder()">Fechar</a>
	</div>
</div>
</div>
<?php if (!$ReadOnly)
{
	?>
<div id="DivFolderSelFile" style="display:none;margin-top:20px">
<table border="0" cellpadding="0" cellspacing="0">
     <form method="POST" enctype="multipart/form-data" target="iframeFolder" action="BPMFolderSendFile.php">
        <tr>           
        <td colspan="3" class="ColNomeSelFile">Sele&ccedil;&atilde;o de Arquivo</td>
        </tr>
        <tr> 
          <td colspan="3">&nbsp;</td>
        </tr>          
        <tr> 
          <td width="147" align="right" class="ColNomeSelFile"> 
              <?php if ($Tipo == "IM") { ?>
              Imagem: 
              <?php } else { ?>
              Arquivo: 
              <?php } ?>
           </td>
          <td width=>&nbsp;</td>
          <td width=><input name="file" type="file" size="40" id="file"> 
          	<input name="Tipo" type="hidden" value="<?php echo $Tipo ?>"> 
            <input name="ProcId" type="hidden" id="ProcId" value="<?php echo $ProcId ?>"> 
            <input name="StepId" type="hidden" id="StepId" value="<?php echo $StepId ?>"> 
            <input name="CaseNum" type="hidden" id="CaseNum" value="<?php  echo $CaseNum ?>"> 
            <input name="FieldId" type="hidden" value="<?php echo $FieldId ?>"></td>
        </tr>
        <tr> 
          <td align="right" class="ColNomeSelFile">Descri&ccedil;&atilde;o:</td>
          <td>&nbsp;</td>
          <td ><input name="descricao" type="text" size="30" maxlength="30"></td>
        </tr>
        <tr> 
          
        <td align="right" class="ColNomeSelFile">Vers&atilde;o:</td>
          <td>&nbsp;</td>
          <td><input name="versao" type="text" id="versao" size="10" maxlength="10"></td>
        </tr>
        <tr> 
          <td colspan="3">&nbsp;</td>
        </tr>
        <tr> 
          <td colspan="3">&nbsp;</td>
        </tr>
        <tr> 
          <td colspan="3">&nbsp;</td>
        </tr>       
        <tr> 
          <td> <input name="Input" type="submit" class="buttonOk" value="Enviar" onclick="return CarregandoArquivo()"></td>
          <td>&nbsp;</td>
          <td align="right"><input name="Input2" type="button" class="buttonCancel" value="Cancelar" onClick="CancelaSelArquivo()"></td>
        </tr>        
     </form>
    </table>
    Tamanho m�ximo do arquivo:
</div>
<?php
}
?>
