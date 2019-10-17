<?php
$server->register('CaseStart',          // method name
    array('AuthUser' => 'tns:TBPMUser', 'ProcId' => 'xsd:string', 'StepId' => 'xsd:string', 'casedata' => 'tns:TBPMcasedata', 'Action' => 'xsd:int'),        // input parameters
    array('return' => 'tns:TBPMCase'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#CaseStart',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Start one Case'            // documentation
);

$server->register('CaseSave',          // method name
    array('AuthUser' => 'tns:TBPMUser', 'ProcId' => 'xsd:string', 'StepId' => 'xsd:string', 'CaseNum' => 'xsd:int', 'casedata' => 'tns:TBPMcasedata', 'Acao' => 'xsd:int'),        // input parameters
    array('return' => 'tns:TBPMCase'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#CaseSave',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Save a case'            // documentation
);


/*
//$server->register('CaseStartImportRM',          // method name
    array('AuthUser' => 'tns:TBPMUser', 'ProcId' => 'xsd:string', 'StepId' => 'xsd:string', 'casedata' => 'tns:TBPMcasedata', 'Acao' => 'xsd:int', 'UNIQUECODE' => 'xsd:string', 'VALUEUNIQUE' => 'xsd:string' ),        // input parameters
    array('return' => 'tns:TBPMCase'),      // output parameters
    'urn:wsbpm',                      // namespace
    'urn:wsbpm#CaseStartImportRM',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Save a case'            // documentation
); */
?>