<?php
$server->register('IniciaAtividades',          // method name
    array('AuthUser' => 'tns:TBPMUser'),        // input parameters
    array('return' => 'tns:TBPMError'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#IniciaAtividades',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Inicia Atividades'            // documentation
);
?>