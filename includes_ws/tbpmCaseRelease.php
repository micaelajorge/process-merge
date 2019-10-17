<?php
$server->register('CaseRelease',          // method name
    array('AuthUser' => 'tns:TBPMUser', 'ProcId' => 'xsd:string', 'StepId' => 'xsd:string', 'CaseNum' => 'xsd:int', 'casedata' => 'tns:TBPMcasedata'),        // input parameters
    array('return' => 'tns:TBPMCase'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#CaseRelase',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Save e Release a case'            // documentation
);
?>