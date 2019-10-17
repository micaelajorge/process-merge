<?php
// TBMPAuth
$server->wsdl->addComplexType(
	'TBPMAuth',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'UserName' => array('name' => 'UserName', 'type' => 'xsd:string'),
		'Password' => array('name' => 'Password', 'type' => 'xsd:string')
    )
);
?>