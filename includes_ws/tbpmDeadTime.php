<?php
include('includes_ws/tbpmerror.php');
include('includes_ws/wsbpmerror.php');
include('includes_ws/wsbpmDeadTime.php');

$server->register('GetDeadTimeExpireList',          			// method name
    array('AuthUser' => 'tns:TBPMUser', 'NumItens' => 'xsd:integer'),        // input parameters
    array('return' => 'tns:TBPMDeadTimeExpireList'),      // output parameters
    'urn:wsbpmDeadTime',                      // namespace
    'urn:wsbpmDeadTime#GetDeadTimeExpireList',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Retorna a lista de casos expirados'            // documentation
);

$server->register('DeadTimeNotify',          			// method name
    array('AuthUser' => 'tns:TBPMUser', 'ProcId' => 'xsd:integer', 'StepId' => 'xsd:integer', 'CaseNum' => 'xsd:integer', 'DeadType' => 'xsd:string'),        // input parameters
    array('return' => 'tns:TBPMDeadTimeNotify'),      // output parameters
    'urn:wsbpmDeadTime',                      // namespace
    'urn:wsbpmDeadTime#DeadTimeNotify',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Retorna a Lista de Destinatarios e arquivo de notificacao de um caso'            // documentation
);

$server->register('DeadTimeRelease',          			// method name
    array('AuthUser' => 'tns:TBPMUser', 'ProcId' => 'xsd:integer', 'StepId' => 'xsd:integer', 'CaseNum' => 'xsd:integer'),        // input parameters
    array('return' => 'tns:string'),      // output parameters
    'urn:wsbpmDeadTime',                      // namespace
    'urn:wsbpmDeadTime#DeadTimeRelease',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Remove um caso da Fila'            // documentation
);


$server->wsdl->addComplexType(
	'TBPMDeadTimeExpireList',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'NumCount' => array('name' => 'NumCount', 'type' => 'xsd:integer'),
        'Items' => array('name' => 'Items', 'type' => 'tns:TBPMDeadTimeExpireItems'),
        'Error' => array('name' => 'Error', 'type' =>'tns:TBPMError')
    )
);

$server->wsdl->addComplexType('TBPMDeadTimeExpireItems',   
	'complexType',    
	'array',    
	'',    
	'SOAP-ENC:Array',    
	array(),    
	array(  
		array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'tns:TBPMDeadTimeExpireItem[]')    
		),    
	'tns:TBPMDeadTimeExpireItem'
	);

$server->wsdl->addComplexType(
	'TBPMDeadTimeExpireItem',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'ProcId' => array('name' => 'ProcId', 'type' => 'xsd:integer'),
		'StepId' => array('name' => 'StepId', 'type' => 'xsd:integer'),
		'CaseNum' => array('name' => 'CaseNum', 'type' => 'xsd:integer'),
		'DeadType' => array('name' => 'DeadType', 'type' => 'xsd:string'),
		'SendNotify' => array('name' => 'SendNotify', 'type' => 'xsd:integer'),
		'ReleaseStep' => array('name' => 'ReleaseStep', 'type' => 'xsd:integer'),
		'StartStepId' => array('name' => 'ReleaseStep', 'type' => 'xsd:integer')
    )
 );

$server->wsdl->addComplexType(
	'TBPMDeadTimeNotify',
    'complexType',
    'struct',
    'all',
    '',
    array(
    	'Subject' => array('name' => 'Subject', 'type' => 'xsd:string'),
        'File' => array('name' => 'ProcId', 'type' => 'xsd:string'),
		'addresses' => array('name' => 'addresses', 'type' => 'tns:TBPMDeadTimeaddresses')
    )
);


$server->wsdl->addComplexType(
 	'TBPMDeadTimeaddresses',   
	'complexType',    
	'array',    
	'',    
	'SOAP-ENC:Array',    
	array(),    
	array(  
		array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'tns:TBPMDeadTimeAddress[]')    
		),    
	'tns:TBPMDeadTimeAddress'
	);


$server->wsdl->addComplexType(
	'TBPMDeadTimeAddress',
    'complexType',
    'struct',
    'all',
    '',
    array(
		'UserName' => array('name' => 'UserName', 'type' => 'xsd:string'),
		'email' => array('name' => 'email', 'type' => 'xsd:string'),
    )
 );
 

?>
