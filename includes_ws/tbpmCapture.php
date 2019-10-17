<?php
$server->wsdl->addComplexType(
	'TBPMCaptureData',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'Docs' => array('name' => 'FieldId', 'type' => 'tns:TBPMDocs'),
		'NumDocs' => array('name' => 'NumFields', 'type' => 'xsd:integer')
    )
);

$server->wsdl->addComplexType(
	'TBPMDoc',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'LoteId' => array('name' => 'LoteId', 'type' => 'xsd:integer'),
        'Origem' => array('name' => 'Origem', 'type' => 'xsd:string'),
        'UserName' => array('name' => 'UserName', 'type' => 'xsd:string'),
        'ProcId' => array('name' => 'ProcId', 'type' => 'xsd:integer'),
        'ProcName' => array('name' => 'ProcName', 'type' => 'xsd:string'),
        'ProcCode' => array('name' => 'ProcCode', 'type' => 'xsd:string'),
        'DocName' => array('name' => 'DocName', 'type' => 'xsd:string'),
        'DocCode' => array('name' => 'DocCode', 'type' => 'xsd:string'),
        'PageNum' => array('name' => 'PageNum', 'type' => 'xsd:integer')       
    )
);

$server->wsdl->addComplexType('TBPMDocs',   
	'complexType',    
	'array',    
	'',    
	'SOAP-ENC:Array',    
	array(),    
	array(        
		array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'tns:TBPMDoc[]')    
		),    
	'tns:TBPMFieldData'
	);
?>