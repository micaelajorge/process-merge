<?php
include('includes_ws/tbpmerror.php');
include('includes_ws/wsbpmerror.php');
include('includes_ws/wsbpmAutostep.php');

$server->register('autostepqueue',          			// method name
    array('AuthUser' => 'tns:TBPMUser', 'NumItens' => 'xsd:integer'),        // input parameters
    array('return' => 'tns:TBPMautostepqueue'),      // output parameters
    'urn:wsbpmAutoStep',                      // namespace
    'urn:wsbpmAutoStep#autostepqueue',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Retorna a Lista de casos em AutoStep'            // documentation
);

$server->register('AutoStepProcs',          // method name
    array('AuthUser' => 'tns:TBPMUser', 'NumItens' => 'xsd:integer'),        // input parameters
    array('return' => 'tns:TBPMautostepqueue'),      // output parameters
    'urn:wsbpmAutoStep',                      // namespace
    'urn:wsbpmAutoStep#AutoStepProcs',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Retorna a Lista de Processos com AutoStep'  // documentation
);

$server->register('AutoStepCases',          // method name
    array('AuthUser' => 'tns:TBPMUser', 'ProcId' => 'xsd:integer', 'NumItens' => 'xsd:integer'),        // input parameters
    array('return' => 'tns:TBPMAutoStepCases'),      // output parameters
    'urn:wsbpmAutoStep',                      // namespace
    'urn:wsbpmAutoStep#AutoStepCases',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Retorna a Lista de Casos em um Processo com AutoStep'  // documentation
);

$server->wsdl->addComplexType(
	'TBPMautostepqueueItem',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'ProcId' => array('name' => 'ProcId', 'type' => 'xsd:integer'),
		'StepId' => array('name' => 'StepId', 'type' => 'xsd:integer'),
		'CaseNum' => array('name' => 'CaseNum', 'type' => 'xsd:integer')
    )
);


$server->wsdl->addComplexType('TBPMautostepqueueItems',   
	'complexType',    
	'array',    
	'',    
	'SOAP-ENC:Array',    
	array(),    
	array(        
		array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'tns:TBPMautostepqueueItem[]')    
		),    
	'tns:TBPMautostepqueueItem'
	);

$server->wsdl->addComplexType(
	'TBPMautostepqueue',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'NumCount' => array('name' => 'NumCount', 'type' => 'xsd:integer'),
        'Items' => array('name' => 'Items', 'type' => 'tns:TBPMautostepqueueItems'),
        'Error' => array('name' => 'Error', 'type' =>'tns:TBPMError')
    )
);

?>
