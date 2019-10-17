<?php
$server->register('GravaEstatisticasCapture',          // method name
    array('Docs' => 'tns:TBPMCaptureData'),        // input parameters
    array('return' => 'xsd:integer'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#GravaEstatisticasCapture',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Gravacao estatisticas Capture'            // documentation
);

function GravaEstatisticasCapture($Docs)
{
	global $connect;
	AtivaDBFaturamento();
	$Data = date('Y-m-d');
	foreach ($Docs["Docs"] as $Doc)
	{
		$SQL = "insert into EstatisticasCapture (";
		$SQL .= " Data,";	
		$SQL .= " UserName,";
		$SQL .= " Origem,";
		$SQL .= " ProcId,";
		$SQL .= " ProcName,";
		$SQL .= "ProcCode,";
		$SQL .= "DocName,";
		$SQL .= "DocCode,";
		$SQL .= "Paginas,";
		$SQL .= "LoteId";		
		$SQL .= " ) values (";
		$SQL .= "'$Data',";
		$SQL .= "'" . $Doc["UserName"] . "',";
		$SQL .= "'" . $Doc["Origem"] . "',";
		$SQL .= "" . $Doc["ProcId"] . ",";	
		$SQL .= "'" . $Doc["ProcName"] . "',";	
		$SQL .= "'" . $Doc["ProcCode"] . "',";	
		$SQL .= "'" . $Doc["DocName"] . "',";	
		$SQL .= "'" . $Doc["DocCode"] . "',";	
		$SQL .= "" . $Doc["PageNum"] . ",";	
		$SQL .= "" . $Doc["LoteId"] . "";			
		$SQL .= ")";	
		if (!mysqli_query($connect, $SQL))
		{
			//error_log('Erro ' . $SQL,0);		
		}
	}
	AtivaDBProcess();
	return 0;
}
?>