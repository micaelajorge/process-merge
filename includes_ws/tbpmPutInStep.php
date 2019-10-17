<?php
$server->register('PutInStep',          // method name
    array('AuthUser' => 'tns:TBPMUser', 'ProcId' => 'xsd:string', 'CaseNum' => 'xsd:integer', 'StepId' => 'xsd:string'),        // input parameters
    array('return' => 'tns:TBPMCase'),      // output parameters
    'urn:wsbpmAdmin',                      // namespace
    'urn:wsbpmAdmin#PutInStep',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Coloca Caso na Fila'            // documentation
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