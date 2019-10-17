<?php
$server->wsdl->addComplexType(
	'TBPMErrorItem',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'ErrId' => array('name' => 'ErrId', 'type' => 'xsd:integer'),
		'ErrDesc' => array('name' => 'ErrDesc', 'type' => 'xsd:string')
    )
);

$server->wsdl->addComplexType('TBPMErrors',   
	'complexType',    
	'array',    
	'',    
	'SOAP-ENC:Array',    
	array(),    
	array(        
		array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'tns:TBPMErrorItem[]')    
		),    
	'tns:TBPMErrorItem'
	);

$server->wsdl->addComplexType(
	'TBPMError',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'Count' => array('name' => 'Count', 'type' => 'xsd:integer'),
        'Itens' => array('name' => 'Value', 'type' => 'tns:TBPMErrors')
    )
);

?>