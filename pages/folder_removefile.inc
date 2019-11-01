<?php

/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 01/11/2019
Sistema: Process_XAMPP
*/
$sql = "delete from casedata where casenum = $caseNum and fieldId = $fieldId and valueId = $valueId";
mysqli_query($connect, $sql);