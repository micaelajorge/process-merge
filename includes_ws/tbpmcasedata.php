<?php
$server->wsdl->addComplexType(
	'TBPMcasedata',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'Fields' => array('name' => 'Fields', 'type' => 'tns:TBPMFields'),
		'Error' => array('name' => 'Error', 'type' => 'tns:TBPMError'),
		'NumFields' => array('name' => 'NumFields', 'type' => 'xsd:integer')
    )
);

$server->wsdl->addComplexType(
	'TBPMFieldData',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'FieldId' => array('name' => 'FieldId', 'type' => 'xsd:string'),
        'Value' => array('name' => 'Value', 'type' => 'xsd:string')
    )
);

$server->wsdl->addComplexType('TBPMFields',   
	'complexType',    
	'array',    
	'',    
	'SOAP-ENC:Array',    
	array(),    
	array(        
		array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'tns:TBPMFieldData[]')    
		),    
	'tns:TBPMFieldData'
	);
	

$server->wsdl->addComplexType(
	'TBPMCase',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'CaseNum' => array('name' => 'CaseNum', 'type' => 'xsd:integer'),
        'Error' => array('name' => 'Error', 'type' => 'tns:TBPMError')
    )
);
?>