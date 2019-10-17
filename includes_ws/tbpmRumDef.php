<?php
$server->register('GetRumDef',          // method name
    array('AuthUser' => 'tns:TBPMUser', 'ProcId' => 'xsd:integer'),        // input parameters
    array('return' => 'tns:TBPMRumDef'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#GetRumDef',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Receive Definiчуo dos Processos Rum'            // documentation
);

$server->wsdl->addComplexType(
	'TBPMRumDef',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'ProcId' => array('name' => 'ProcId', 'type' => 'xsd:integer'),
		'Itens' => array('name' => 'Itens', 'type' => 'xsd:TBPMARums')
    )
);

$server->wsdl->addComplexType('TBPMARums',   
	'complexType',    
	'array',    
	'',    
	'SOAP-ENC:Array',    
	array(),    
	array(        
		array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'tns:TBPMRum[]')    
		),    
	'tns:TBPMRum'
);
$server->wsdl->addComplexType(
	'TBPMRum',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'RumCod' => array('name' => 'StepId', 'type' => 'xsd:string')
    )
);
?>