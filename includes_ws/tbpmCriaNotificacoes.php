<?php
$server->register('CriaNotificaoes',          // method name
    array('AuthUser' => 'tns:TBPMUser'),        // input parameters
    array('return' => 'tns:TBPMcasedata'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#CriaNotificaoes',              // soapaction
    'rpc',                            // style
    'encoded',                        // use
    'Acquire one Case'                // documentation
);
?>