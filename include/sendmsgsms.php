<?
//echo "<p><br><br><b>Retorno da Função:</b> ". 
//EnviaMensagem("Teste de envio de mensagem com espaco e acentuacao.",
//			  "551181849875",
//			  "mnet:timsul-br:pref");

///echo "<p><br><br><b>Retorno da Função:</b> ". 
///EnviaMensagem("Teste de envio de mensagem para Mosca.",
///			  "556292291310",
///			  "mnet:americel-br:pref");

function EnviaMensagem($MsgText,$NumCel,$CodOperadora)
{
	$MsgTextHTTP = rawurlencode($MsgText);
	$strURL="http://sx.yavox.com:9080/sc/input";
	$strXml="<?xml%20version=\"1.0\"%20encoding=\"iso-8859-1\"?><sxsys%20xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"%20%20xsi:noNamespaceSchemaLocation=\"/app/sc/java/pgm/webapps/sc/res/sx.xsd\"><node%20id=\"msg1\"%20class=\"message\"%20name=\"fwd\"><generic><generic_message>$MsgTextHTTP</generic_message><generic_origin>datafix</generic_origin><generic_destination>$NumCel</generic_destination><generic_splitmode>split</generic_splitmode><generic_splitlimit>999</generic_splitlimit></generic><reference%20reftype=\"merge\">$CodOperadora</reference></node></sxsys>";
	//echo "<b>Chamada query: </b>$strURL?xml=$strXml";
	return sendMsg('200.225.91.241', 9080,"/sc/input?xml=$strXml");
	//'200.225.91.241'	
};


function sendMsg($host,$port,$path,$param='',$ua='')
{
	// Build the request string
	if(is_array($param))
	{
		$request = array();
		foreach($param as $key => $val)
		{
			$request[] = $key . "=" . urlencode($val);
		}
		$request = join('&',$request);
	}
	else
	{
		$request = $param;
	}
	$request_length = strlen($request);
	$header = "POST $path HTTP/1.0\r\n";
	$header .= "Host: $host\r\n";
	if($ua) $header .= "User-Agent: $ua\r\n";
	$header .= "Content-type:application/x-www-form-urlencoded\r\n";
	$header .= "Content-length: $request_length\r\n";
	$header .= "\r\n";
	
	// Open the connection
	//echo "<p><b>$header . $request";
	$fp = fsockopen($host,$port,&$err_num,&$err_msg,30);
	
	// No Connection Error
	if(!$fp)
	{
		return -666;//die('Sorry, the server is not currently available!');
	}
	
	// Send everything
	fputs($fp, $header . $request);
	
	// Discard the HTTP header
	while(trim(fgets($fp,4096)) != '');
	
	// Get the response
	while(!feof($fp))
	{
		$response .= fgets($fp,4096);
	}
	//Returns the response as Integer
	if (strlen($response) == 0) 
	{
		return -1; 
	}
	else 
	{ 
		return intval($response); 
	}
}
?>