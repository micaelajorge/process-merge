<?php
$server->register('GetFieldValue',          // method name
    array('AuthUser' => 'tns:TBPMUser', 'CaseNum' => 'xsd:int', 'FieldId' => 'xsd:string',),        // input parameters
    array('return' => 'tns:TBPMFieldValue'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#GetFieldValue',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Pega o Valor de um campo em um caso'            // documentation
);

$server->wsdl->addComplexType(
	'TBPMFieldValue',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'FieldValue' => array('name' => 'FieldValue', 'type' => 'xsd:string'),
		'Error' => array('name' => 'Error', 'type' => 'tns:TBPMError')
    )
);
?>