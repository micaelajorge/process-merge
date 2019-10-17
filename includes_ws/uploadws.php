<?php
$server->register('Upload',          // method name
    array('FileName' => 'xsd:string', 'fileContent' => 'xsd:base64Binary'),        // input parameters
    array('return' => 'xsd:string'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#Upload',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Upload a file'            // documentation
);

$server->register('Merge',          // method name
    array('FileName' => 'tns:ArrayOfString', 'finalFileName' => 'xsd:string'),        // input parameters
    array('return' => 'xsd:string'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#Merge',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Upload a file'            // documentation
);

$server->wsdl->addComplexType('ArrayOfString',   
	'complexType',    
	'array',    
	'',    
	'SOAP-ENC:Array',    
	array(),    
	array(        
		array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'xsd:string[]')    
		),    
	'xsd:string'
	);

function Upload($FileName, $fileContent )
{
	include("config.php");
	$fh = fopen("$temp_dir\\$FileName", 'w');
	fwrite($fh, $fileContent);
	fclose($fh);
	return 'OK';	
}

function Merge($FileName, $FinalFileName)
{
	include("config.php");
	$fh = fopen("$merge_dir\\$FinalFileName", 'w');
	foreach ($FileName as $Files) {
		if (!is_array($Files))
		{
			$Files = array($Files);
		}
		foreach ($Files as $File)
			{
			$PartName = "$temp_dir\\" . $File;
			//error_log("Arquivo $File", 0);
			if (!file_exists($PartName))
			{
				fclose($fh);
				unlink("$merge_dir\\$FinalFileName");
				return "$File n�o encontrado";
			}
			$Part = fopen($PartName, "r");
			$fileContent = fread($Part, filesize($PartName));
			$fileContent = base64_decode($fileContent);
			fwrite($fh, $fileContent);	
			fclose($Part);
			unlink($PartName);
			}
	}
	error_log("Merge Dir $merge_dir\\$FinalFileName", 0);
	fclose($fh);
	return 'Ok';
}
?>