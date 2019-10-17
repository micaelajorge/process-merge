<?php
//Modulo de envio de e-mail
function ContentType($arquivo)
	{
	$extencao = substr($arquivo, -3);
	$retorno = exec("./contenttype.sh $extencao");
	$retorno = substr($retorno,0,strpos($retorno,"\t"));
	return "Content-Type: image/jpeg";
	//return $retorno;
	}

function email($to, $from, $subject, $msg_body, $attach = "none")
	{
	$replay = $from;	
	if (empty($attach))
		{
		$attach = "none";
		}
		
	if ($attach != "none")
		{ 
		$attach_name = substr($attach,strrpos($attach,"/")+1);
		$ContentType = ContentType($attach_name);
		$file = fopen($attach, "r"); 
		$attach_size = filesize($attach);
		$contents = fread($file, $attach_size); 
		$encoded_attach = chunk_split(base64_encode($contents)); 
		fclose($file); 
		}

	$headers = "From: $from\r\n"; 
	$headers .= "MIME-Version: 1.0\r\n"; 
	$boundary = uniqid("HTMLDEMO"); 
	$headers .= "Content-Type: multipart/mixed; boundary = $boundary\r\n\r\n"; 
	$headers .= "This is a MIME encoded message.\r\n\r\n"; 
	$headers .= "--$boundary\r\n"; 
	$headers .= "Content-Type: text/plain; charset=ISO-8859-1\r\n";
	$headers .= "Content-Transfer-Encoding: base64\r\n\r\n"; 	
	$body = $msg_body[0];
	$headers .= chunk_split(base64_encode($body)); 
	$headers .= "--$boundary\r\n";
	$headers .=  "Content-Type: text/html; charset=ISO-8859-1\r\n"; 
	$headers .=  "Content-Transfer-Encoding: base64\r\n\r\n"; 
	$body = $msg_body[1];
	$headers .= chunk_split(base64_encode($body)); 
	$headers .= "--$boundary\r\n";
	if ($attach != "none")
		{
		$headers.="Content-Type: $ContentType; name=\"$attach_name\"\r\n";
		$headers.="Content-Transfer-Encoding: base64\r\n";
		$headers.="Content-disposition: attachment\r\n";
		$headers.="Content-ID: <1>\r\n";
		$headers.="filename=\"$attach_name\"\r\n";
		$headers.="\n";
		$headers.="$encoded_attach\r\n";
		$headers.="\r\n";
		$headers.="--$boundary--\r\n";	
		}

	$mailheaders = $headers;
	mail($to, $subject, "", $mailheaders); 
	return $msg_body;
	}
?>