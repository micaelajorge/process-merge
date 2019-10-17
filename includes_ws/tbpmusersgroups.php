<?php
$server->wsdl->addComplexType(
	'TBPMUserData',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'UserName' => array('name' => 'UserName', 'type' => 'xsd:string'),
		'UserDesc' => array('name' => 'UserDesc', 'type' => 'xsd:string'),
		'email' => array('name' => 'email', 'type' => 'xsd:string')
    )
);

$server->wsdl->addComplexType(
	'TBPMcasedata',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'Fields' => array('name' => 'Fields', 'type' => 'tns:TBPMGroups'),
		'Error' => array('name' => 'Error', 'type' => 'tns:TBPMError'),
		'NumGroups' => array('name' => 'NumFields', 'type' => 'xsd:integer')
    )
);

$server->wsdl->addComplexType(
	'TBPMGroup',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'Group' => array('name' => 'FieldId', 'type' => 'xsd:string'),
    )
);

$server->wsdl->addComplexType('TBPMGroups',   
	'complexType',    
	'array',    
	'',    
	'SOAP-ENC:Array',    
	array(),    
	array(        
		array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'tns:TBPMGroup[]')    
		),    
	'tns:TBPMGroup'
	);
?>