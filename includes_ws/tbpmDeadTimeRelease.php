<?php
$server->register('DeadTimeRelease',          // method name
    array('AuthUser' => 'tns:TBPMUser'),        // input parameters
    array('return' => 'tns:TBPMError'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#DeadTimeRelease',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Receive e Inbox'            // documentation
);
?>