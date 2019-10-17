<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 12/12/2018
  Sistema: Process_XAMPP
 */

$sql = "select "
        . "procid, "
        . "stepid, "
        . "caseid, "
        . "(deadsoftdatetime < now() and deadsoftexec = 1) as deadsoft, "
        . "(deadharddatetime < now() and deadhardexec = 1) as deadhard, "
        . "(deadhardestdatetime < now() and deadhardestexec = 1) as deadhardest  "
        . "from "
        . "casequeue "
        . "where "
        . "(deadsoftdatetime < now() and deadsoftexec = 1) "
        . "or "
        . "(deadharddatetime < now() and deadhardexec = 1) "
        . "or "
        . "(deadhardestdatetime < now() and deadhardestexec = 1)";

//error_log("DeadTimes \n$sql");
$query = mysqli_query($connect, $sql);

$dadosDeadTime = mysqli_fetch_all($query, MYSQLI_ASSOC);

$dados = array();

foreach ($dadosDeadTime as $linha)
{
    $deadTime["procid"] = $linha["procid"];
    $deadTime["stepid"] = $linha["stepid"];
    $deadTime["caseid"] = $linha["caseid"];
    if ($linha["deadsoft"] == 1)
    {
        $deadTime["deadtime"] = "S";
        $dados[] = $deadTime;
    }
    if ($linha["deadhard"] == 1)
    {
        $deadTime["deadtime"] = "H";
        $dados[] = $deadTime;
    }
    if ($linha["deadhardest"] == 1)
    {
        $deadTime["deadtime"] = "D";
        $dados[] = $deadTime;
    }    
}

header('Content-Type: application/json');
$jsonData = json_encode($dados);
echo $jsonData;
        