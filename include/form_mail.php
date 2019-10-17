<?php
include("include\common.php");
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<title>4 e final</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>

<body>

<?
if ($enviar == 1) {
	include("mail.php");
	if($attach <> 'none') 
		{ /*verifica se veio vazio */
		if(($attach_type <> 'image/gif') AND ($attach_type <> 'image/pjpeg')) 
			{ /*verifica o tipo da attach*/
			$erros++;
			$errors = $errors."Tipo de arquivo inválido";
			}
		
		if($attach_size > 300000) 
			{ /*verifica o tamanho da attach enviada */
			$erros++;
			$errors = $errors."O tamanho do arquivo é maior que 300 K<br>";
			}		
		$arquivo = "/home/tcpcomc/public_html/upload/$attach_name"; /*caminho pra onde vai a attach*/
		
		if (file_exists($arquivo)) 
			{ /*verifica se o arquivo existe no diretório */
			$erros++;
			$errors = $errors."O arquivo já existe, por favor renomeie o arquivo<br>";
			} /*fecha verificação do arquivo*/
		if($erros == 0) 
			{
			copy($attach, "/home/tcpcomc/public_html/upload/$attach_name"); /*envia a attach para a pasta*/
			}/* fecha acao=enviar*/
		} 

$attach = "/home/tcpcomc/public_html/upload/$attach_name";		
// FECHA ENVIO DE FOTOS 1


email($to, $from, $subject, $msg_body, $attach, $attach_name);

echo "$to";
echo "<BR>";
echo "$from";
echo "<BR>";
echo "$subject";
echo "<BR>";
echo "$msg_body";
echo "<BR>";
echo "$attach_name";
echo "<BR>";
echo "$enviar";
echo "<BR>";
echo "Content Type - $ContentType";
echo "<BR>";
echo "Arquivo - $arquivo";
echo "<BR>";
echo "Retorno - $retorno";
}


?>
<form name="form1" method="post" action="form_mail.php?enviar=1" enctype="multipart/form-data">
  <table width="95%"  border="0" cellspacing="0" cellpadding="0">
    <tr>
      <td>Para</td>
      <td><input name="to" type="text" id="to" value="<?php echo $to ?>"></td>
    </tr>
    <tr>
      <td>De</td>
      <td><input name="from" type="text" id="from" value="<?php echo $from ?>"></td>
    </tr>
    <tr>
      <td><p>T&iacute;tulo</p>      </td>
      <td><input name="subject" type="text" id="subject" value="<?php echo $subject ?>"></td>
    </tr>
    <tr>
      <td>Mensagem</td>
      <td><input name="msg_body" type="text" id="msg_body" value="<?php echo $msg_body ?>"></td>
    </tr>
    <tr>
      <td>&nbsp;</td>
      <td> <input name="attach" type="file" id="attach"></td>
    </tr>
    <tr>
      <td>&nbsp;</td>
      <td><input type="text" name="textfield6"></td>
    </tr>
    <tr>
      <td>&nbsp;</td>
      <td><input type="submit" name="Submit" value="Submit"> <input type="reset" name="Submit2" value="Reset">        </td>
    </tr>
    <tr>
      <td>&nbsp;</td>
      <td><a href="form_mail.php">voltar</a></td>
    </tr>
  </table>
</form>
</body>
</html>
