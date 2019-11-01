<?php

/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 01/11/2019
Sistema: Process_XAMPP
*/

insereEntradaAuditTrail(0, $caseNum, 0, $userdef->UserId, $userdef->Desc, 8, "$fileName");
$sql = "delete from casedata where casenum = $caseNum and fieldId = $fieldId and valueId = $valueId";
mysqli_query($connect, $sql);