<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 05/02/2019
  Sistema: Process_XAMPP
 */
include_once(FILES_ROOT . "include/resource01.php");

/**
 * @rota api/ocrdefs
 * @global type $connect
 */
function ocr_pegaDefinicoes()
{
    global $connect;
    $SQL = "select extendPropsProc, proccod from procdef where extendPropsProc <> ''";
    $Query = mysqli_query($connect, $SQL);
    $linhas = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $ocrDefs = array();
    foreach ($linhas as $item) {
        $json_extendProps = $item["extendPropsProc"];
        $extendProps = json_decode($json_extendProps, true);
        if (json_last_error() !== JSON_ERROR_NONE) {
            continue;
        }
        if (!$extendProps["OCR"]["enabled"]) {
            continue;
        }
        $extendProps["OCR"]["proccode"] = $item["proccod"];
        $ocrDefs[] = $extendProps["OCR"];
    }
    header('Content-Type: application/json');
    $json_ocrDefs = json_encode($ocrDefs);
    echo $json_ocrDefs;
}

/**
 * /api/ocr
 * @global type $connect
 * @param type $defOcr
 * @return array
 */
function ocr_pegaQueueOcrProc($defOcr)
{
    global $connect;
    $retorno = array();

    $procId = PegaProcIdByCode($defOcr["proccode"]);
    $stepId = PegaStepIdByCode($procId, $defOcr["stepRun"]);
    $SQL = "select caseid from casequeue where procid = $procId and stepid = $stepId limit 10";
    $Query = mysqli_query($connect, $SQL);
    $casos = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    if (count($casos) == 0) {
        return $retorno;
    }
    $camposProcesso = PegaCamposProcesso($procId);

    $campos = array();
    foreach ($defOcr["fields"] as $field) {
        if ($field["sourceData"] == "field") {
            $codigos = array_column($camposProcesso, 'fieldcod');
            $indiceCampo = array_search($field["data"], $codigos);
            $campos[] = $camposProcesso[$indiceCampo]["fieldid"];
        }
    }

    if (count($campos) == 0) {
        return $retorno;
    }

    $listaCasos = implode(", ", array_column($casos, "caseid"));
    $listaCampos = implode(", ", $campos);
    $SQL = "select casenum, fieldcod, fieldvalue from casedata, procfieldsdef where procfieldsdef.procid = $procId and casedata.procid = $procId and procfieldsdef.fieldid = casedata.fieldid and casenum in ($listaCasos) and casedata.fieldid in ($listaCampos) order by casenum, fieldcod";
    $Query = mysqli_query($connect, $SQL);
    $valoresCasos = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $caseNum = 0;
    $dadosCaso = array();
    $campos = array();
    foreach ($valoresCasos as $valor) {
        if ($caseNum != 0 & $caseNum != $valor["casenum"]) {
            $dadosCaso["casenum"] = $caseNum;
            $dadosCaso["fields"] = $campos;
            $retorno[] = $dadosCaso;
            $caseNum = $valor["casenum"];
            $campos = array();
            $dadosCaso = array();
        }
        $caseNum = $valor["casenum"];
        $dadosCaso["fieldCode"] = $valor["fieldcod"];
        $dadosCaso["fieldValue"] = $valor["fieldvalue"];
        $campos[] = $dadosCaso;
    }
    $dadosCaso["casenum"] = $caseNum;
    $dadosCaso["fields"] = $campos;
    $retorno[] = $dadosCaso;
    return $retorno;
}

function ocr_queue()
{
    $dadosEntrada = apiTrataDadosEntrada();
    $dadosOcr = $dadosEntrada["dadosOcr"];
    $processosOcr = array();
    foreach ($dadosOcr as $processoOcr) {
        $listaCasos = ocr_pegaQueueOcrProc($processoOcr);
        if (!count($listaCasos)) {
            continue;
        }
        $processoOcr["listaCasos"] = $listaCasos;
        $processosOcr[] = $processoOcr;
    }
    $json_retorno = json_encode($processosOcr);
    echo $json_retorno;
}
