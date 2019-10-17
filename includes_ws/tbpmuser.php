<?php
$server->wsdl->addComplexType(
	'TBPMUser',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'UserName' => array('name' => 'UserName', 'type' => 'xsd:string'),
		'UserPassword' => array('name' => 'UserPassword', 'type' => 'xsd:string')
    )
);

?>