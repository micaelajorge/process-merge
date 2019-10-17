<?php
$server->register('GetInBox',          // method name
    array('AuthUser' => 'tns:TBPMUser', 'Filter' => 'tns:TBPMGetFilter'),        // input parameters
    array('return' => 'tns:TBPMInbox'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#GetInbox',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Receive e Inbox'            // documentation
);

$server->register('GetInBoxFilter',          // method name
    array('AuthUser' => 'tns:TBPMUser', 'Filter' => 'tns:TBPMGetFilter', 'Conds' => 'tns:TBPMInboxConds'),        // input parameters
    array('return' => 'tns:TBPMInbox'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#GetInboxFilter',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Receive e Inbox'            // documentation
);

$server->wsdl->addComplexType(
	'TBPMGetFilter',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'ProcId' => array('name' => 'ProcId', 'type' => 'xsd:integer'),
		'StepId' => array('name' => 'StepId', 'type' => 'xsd:integer'),
		'Locked' => array('name' => 'Locked', 'type' => 'xsd:integer')
    )
);

$server->wsdl->addComplexType(
	'TBPMInbox',
    'complexType',
    'struct',
    'all',
    '',
    array(
		'NumCount' => array('name' => 'NumCount', 'type' => 'xsd:integer'),
		'InboxItens' => array('name' => 'Itens', 'type' => 'tns:TBPMInboxItens'),
		'Error' => array('name' => 'Err', 'type' => 'tns:TBPMError'),
		'Refs' => array('name' => 'Refs', 'type' => 'tns:TBPMRefs'),
		'Details' => array('name' => 'Details', 'type' => 'tns:TBPMDetails')		
	)
);


$server->wsdl->addComplexType(
	'TBPMRefItem',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'FieldName' => array('name' => 'FieldName', 'type' => 'xsd:string'),
		'FieldValue' => array('name' => 'FieldValue', 'type' => 'xsd:string')
    )
);

$server->wsdl->addComplexType('TBPMARefs',   
	'complexType',    
	'array',    
	'',    
	'SOAP-ENC:Array',    
	array(),    
	array(        
		array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'tns:TBPMRefItem[]')    
		),    
	'tns:TBPMRefItem'
	);

$server->wsdl->addComplexType(
	'TBPMRefs',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'Count' => array('name' => 'Count', 'type' => 'xsd:integer'),
        'Itens' => array('name' => 'Value', 'type' => 'tns:TBPMARefs')
    )
);

$server->wsdl->addComplexType(
	'TBPMDetailItem',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'FieldCode' => array('name' => 'FieldCode', 'type' => 'xsd:string'),
		'FieldValue' => array('name' => 'FieldValue', 'type' => 'xsd:string')
    )
);

$server->wsdl->addComplexType('TBPMADetails',   
	'complexType',    
	'array',    
	'',    
	'SOAP-ENC:Array',    
	array(),    
	array(        
		array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'tns:TBPMDetailItem[]')    
		),    
	'tns:TBPMDetailItem'
	);

$server->wsdl->addComplexType(
	'TBPMDetails',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'Count' => array('name' => 'Count', 'type' => 'xsd:integer'),
        'Itens' => array('name' => 'Value', 'type' => 'tns:TBPMADetails')
    )
);

$server->wsdl->addComplexType(
	'TBPMInboxItem',
    'complexType',
    'struct',
    'all',
    '',
    array(
    	'StepId' => array('name' => 'StepId', 'type' => 'xsd:integer'),
		'Details' => array('name' => 'Details', 'type' => 'tns:TBPMADetails'),
		'NumDetails' => array('name' => 'NumDetails', 'type' => 'xsd:integer'),		
		'CaseNum' => array('name' => 'CaseNum', 'type' => 'xsd:integer'),
		'AdHoc' => array('name' => 'AdHoc', 'type' => 'xsd:boolean'),
		'Locked' => array('name' => 'Locked', 'type' => 'xsd:boolean'),
		'Action' => array('name' => 'Action', 'type' => 'xsd:integer'),	
    )
);


$server->wsdl->addComplexType(
	'TBPMInboxItens',
	'complexType',    
	'array',    
	'',    
	'SOAP-ENC:Array',    
	array(),    
	array(        
		array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'tns:TBPMInboxItem[]')    
		),    
	'tns:TBPMInboxItem'
);























$server->wsdl->addComplexType(
	'TBPMDeadTimeItens',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'Order' => array('name' => 'Order', 'type' => 'xsd:boolean'),
		'DateTime' => array('name' => 'DateTime', 'type' => 'xsd:string'),
        'Over' => array('name' => 'Over', 'type' => 'xsd:boolean')
    )
);


$server->wsdl->addComplexType(
	'TBPMDeadTime',
    'complexType',
    'struct',
    'all',
    '',
    array(
		'Itens' => array('name' => 'Itens', 'type' => 'tns:TBPMDeadTimes'),
		'Count' => array('name' => 'Count', 'type' => 'xsd:integer')
    )
);

$server->wsdl->addComplexType(
	'TBPMInboxCond',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'FieldId' => array('name' => 'FieldId', 'type' => 'xsd:integer'),
		'Operator' => array('name' => 'Operator', 'type' => 'xsd:string'),
		'Value' => array('name' => 'Value', 'type' => 'xsd:string')
    )
);

$server->wsdl->addComplexType(
	'TBPMInboxConds',
	'complexType',    
	'array',    
	'',    
	'SOAP-ENC:Array',    
	array(),    
	array(        
		array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'tns:TBPMInboxCond[]')    
		),    
	'tns:TBPMInboxCond'
);
?>