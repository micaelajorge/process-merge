<?php
$server->register('GetDef',          // method name
    array('AuthUser' => 'tns:TBPMUser', 'ProcId' => 'xsd:integer', 'StepId' => 'xsd:integer'),        // input parameters
    array('return' => 'tns:TBPMDef'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#GetDef',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Receive Definiчуo dos Processos'            // documentation
);

$server->register('GetDefByCod',          // method name
    array('AuthUser' => 'tns:TBPMUser', 'ProcCod' => 'xsd:string'),        // input parameters
    array('return' => 'tns:TBPMDef'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#GetDefByCod',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Receive Definiчуo dos Processos atraves do cod'            // documentation
);

$server->wsdl->addComplexType(
	'TBPMDef',
    'complexType',
    'struct',
    'all',
    '',
    array(
		'Itens' => array('name' => 'Itens', 'type' => 'tns:TBPMAProcs'),
		'NumCount' => array('name' => 'NumCount', 'type' => 'xsd:integer'),
		'Error' => array('name' => 'Err', 'type' => 'tns:TBPMError')
    )
);

$server->wsdl->addComplexType(
	'TBPMProcs',
    'complexType',
    'struct',
    'all',
    '',
    array(
		'Itens' => array('name' => 'Itens', 'type' => 'tns:TBPMAProcs'),
		'NumCount' => array('name' => 'NumCount', 'type' => 'xsd:integer'),
		'Error' => array('name' => 'Err', 'type' => 'tns:TBPMError')
    )
);


$server->wsdl->addComplexType('TBPMAProcs',   
	'complexType',    
	'array',    
	'',    
	'SOAP-ENC:Array',    
	array(),    
	array(        
		array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'tns:TBPMprocdef[]')    
		),    
	'tns:TBPMprocdef'
	);

$server->wsdl->addComplexType(
	'TBPMprocdef',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'ProcId' => array('name' => 'ProcId', 'type' => 'xsd:integer'),
		'ProcName' => array('name' => 'ProcName', 'type' => 'xsd:string'),
		'ProcDesc' => array('name' => 'ProcDesc', 'type' => 'xsd:string'),
		'ProcCod' => array('name' => 'ProcCod', 'type' => 'xsd:string'),
		'PageAction' => array('name' => 'PageAction', 'type' => 'xsd:string')
    )
);

$server->wsdl->addComplexType(
	'TBPMProcFields',
    'complexType',
    'struct',
    'all',
    '',
    array(
		'Itens' => array('name' => 'Itens', 'type' => 'tns:TBPMAProcFields'),
		'NumCount' => array('name' => 'NumCount', 'type' => 'xsd:integer'),
    )
);

$server->wsdl->addComplexType('TBPMAProcFields',   
	'complexType',    
	'array',    
	'',    
	'SOAP-ENC:Array',    
	array(),    
	array(        
		array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'tns:TBPMProcFieldDef[]')    
		),    
	'tns:TBPMProcFieldDef'
	);

$server->wsdl->addComplexType('TBPMASteps',   
	'complexType',    
	'array',    
	'',    
	'SOAP-ENC:Array',    
	array(),    
	array(        
		array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'tns:TBPMstepdef[]')    
		),    
	'tns:TBPMstepdef'
);

$server->wsdl->addComplexType(
	'TBPMstepdef',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'StepId' => array('name' => 'StepId', 'type' => 'xsd:integer'),
		'StepName' => array('name' => 'StepName', 'type' => 'xsd:string'),
		'StepDesc' => array('name' => 'StepDesc', 'type' => 'xsd:string'),
		'StepCod' => array('name' => 'StepCod', 'type' => 'xsd:string'),
		'Edit' => array('name' => 'Edit', 'type' => 'xsd:integer'),
		'NewDoc' => array('name' => 'NewDOc', 'type' => 'xsd:integer'),
		'StepFields' => array('name' => 'StepFields', 'type' => 'tns:TBPMStepFields')
    )
);

$server->wsdl->addComplexType(
	'TBPMProcFieldDef',
    'complexType',
    'struct',
    'all',
    '',
    array(
		'FieldId' => array('name' => 'FieldId', 'type' => 'xsd:integer'),
		'FieldName' => array('name' => 'FieldName', 'type' => 'xsd:string'),
		'FieldDesc' => array('name' => 'FieldDesc', 'type' => 'xsd:string'),
		'FieldCod' => array('name' => 'FieldCod', 'type' => 'xsd:string'),
		'FieldType' => array('name' => 'FieldType', 'type' => 'xsd:string')	
    )
);

$server->wsdl->addComplexType(
	'TBPMSteps',
    'complexType',
    'struct',
    'all',
    '',
    array(
		'Itens' => array('name' => 'Itens', 'type' => 'tns:TBPMASteps'),
		'NumCount' => array('name' => 'NumCount', 'type' => 'xsd:integer'),
    )
);

$server->wsdl->addComplexType(
	'TBPMStepFields',
    'complexType',
    'struct',
    'all',
    '',
    array(
		'Itens' => array('name' => 'Itens', 'type' => 'tns:TBPMAStepFields'),
		'NumCount' => array('name' => 'NumCount', 'type' => 'xsd:integer'),
    )
);

$server->wsdl->addComplexType('TBPMAStepFields',   
	'complexType',    
	'array',    
	'',    
	'SOAP-ENC:Array',    
	array(),    
	array(        
		array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'tns:TBPMStepFieldDef[]')    
		),    
	'tns:TBPMStepFieldDef'
	);

$server->wsdl->addComplexType(
	'TBPMStepFieldDef',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'FieldId' => array('name' => 'FieldId', 'type' => 'xsd:integer'),
		'FieldName' => array('name' => 'FieldName', 'type' => 'xsd:string'),
		'FieldDesc' => array('name' => 'FieldDesc', 'type' => 'xsd:string'),
		'FieldCod' => array('name' => 'FieldCod', 'type' => 'xsd:string'),
		'ReadOnly' => array('name' => 'ReadOnly', 'type' => 'xsd:string'),
		'Optional' => array('name' => 'ReadOnly', 'type' => 'xsd:string'),
		'FieldType' => array('name' => 'FieldType', 'type' => 'xsd:string')
    )
);

?>