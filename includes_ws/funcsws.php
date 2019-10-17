<?php

function PegaPropCampos($ProcId, $StepId)
{
    global $connect;
    $Campos = array();
    $SQL = "select stepfieldsdef.FieldId, Optional, ReadOnly, FieldType from procfieldsdef, stepfieldsdef where stepfieldsdef.ProcId = $ProcId and stepfieldsdef.StepId = $StepId and procfieldsdef.ProcId = stepfieldsdef.ProcId and procfieldsdef.FieldId = stepfieldsdef.FieldId ";
    $Query = mysqli_query($connect, $SQL);
    while ($Result = mysqli_fetch_array($Query)) {
        $Campos[$Result["FieldId"]]["Optional"] = $Result["Optional"];
        $Campos[$Result["FieldId"]]["ReadOnly"] = $Result["ReadOnly"];
        $Campos[$Result["FieldId"]]["FieldType"] = $Result["FieldType"];
        $Campos[$Result["FieldId"]]["Valido"] = 0;
        $Campos[$Result["FieldId"]]["Recebido"] = 0;
    }
    return $Campos;
}

/**
 * Converte de um array de 
 * codigo do Campo/valor 
 * para 
 * Id do Campo/Valor 
 * 
 * @param type $ProcId Id do Processo
 * @param type $dadosDoCaso Array de Campos codigo/valor
 * @return type Array de Campos FieldId/valor 
 */
function TrataArrayCamposWebService($ProcId, $dadosDoCaso)
{
    $j = 0;
    $FieldsValidos = array();

    $camposRetorno["Fields"] = array();
    $camposRetorno["NumFields"] = 0;

    if (!is_array($dadosDoCaso)) {
        return $camposRetorno;
    }

    if (!key_exists("Fields", $dadosDoCaso)) {
        return $camposRetorno;
    }

    if (!is_array($dadosDoCaso["Fields"])) {        
        return $camposRetorno;
    }

    for ($i = 0; $i < count($dadosDoCaso["Fields"]); $i++) {        
        //error_log(var_export($dadosDoCaso["Fields"][$i], true));
        if (!key_exists("fieldCode", $dadosDoCaso["Fields"][$i])) {
            //error_log("Sem FieldCode");
            continue;
        }

        $fieldCode = $dadosDoCaso["Fields"][$i]["fieldCode"];
        if (!is_numeric($fieldCode))
        {
            $FieldId = PegaFieldIdByCode($ProcId, $fieldCode);
        } else {
            $FieldId = $fieldCode;
        }

        if (empty($FieldId) || $FieldId == '') {
            //error_log("Sem FieldId");
            continue;
        }

        $dadosDoCaso["Fields"][$i]["FieldId"] = $FieldId;

        //Verifica se o Field Id eh valido, senao continua;
        $FieldsValidos[$j] = $dadosDoCaso["Fields"][$i];
        $j++;
    }

    // Remonta os Campos apenas com os Campos validos
    $camposRetorno["Fields"] = $FieldsValidos;
    $camposRetorno["NumFields"] = count($FieldsValidos);
    //error_log("Campos Validos: " . var_export($camposRetorno, true));
    return $camposRetorno;
}

function ValidaCampos($ProcId, $StepId, $dadosDoCaso)
{
    $Retorno = true;
    $PropCampos = PegaPropCampos($ProcId, $StepId);

    // Se não tiver campos, pode ser apenas processamento
    if (!is_array($PropCampos)) {
        return true;
    }

    if (count($PropCampos) == 0) {
        return true;
    }
    
    if (count($dadosDoCaso["Fields"]) == 0)
    {
        return true;
    }

    if ($dadosDoCaso["NumFields"] <> count($dadosDoCaso["Fields"])) {
        GeraErro(8, 'NumFields diferente de Campos Enviados');	
    }


    if ($dadosDoCaso["NumFields"] > count($PropCampos)) {
        GeraErro(8, 'Numero de Campos passados maior q o esperado');
    }


    $j = 0;
    for ($i = 0; $i < count($dadosDoCaso["Fields"]); $i++) {
        //error_log('funcws - Code: '. $dadosDoCaso["Fields"][$i]["FieldId"] . "- Value: " . $dadosDoCaso["Fields"][$i]["Value"], 0);

        $FieldId = $dadosDoCaso["Fields"][$i]["FieldId"];
        if (!is_numeric($FieldId)) {
            if (!is_numeric($FieldId)) {
                continue;
            }
        }

        // Cria nova pilha de campos, mantendo apenas os validos
        $FieldsValidos[$j] = $dadosDoCaso["Fields"][$i];
        $j++;

        // Seta que o Valor do Campo foi enviado
        $PropCampos[$FieldId]["Recebido"] = 1;

        // Campos Read Only s�o tratados na fun��o Pega Dados Array		
        $ReadOnly = $PropCampos[$FieldId]["ReadOnly"];
        if ($ReadOnly) {
            GeraErro(7, 'Campo ReadOnly');
            $dadosDoCaso["Fields"][$i]["ReadOnly"] = 1;
            continue;
        }

        $Optional = $PropCampos[$FieldId]["Optional"];
        $fieldValue = $dadosDoCaso["Fields"][$i]["fieldValue"];
        if (!$Optional && empty($fieldValue)) {
            GeraErro(8, "Campo " . $dadosDoCaso["Fields"][$i]["FieldId"]);
            $Retorno = false;
            continue;
        } else {
            $PropCampos[$FieldId]["Recebido"] = 0;
        }
        $FieldType = $PropCampos[$FieldId]["FieldType"];

        // Valida Campos
        $PropCampos[$FieldId]["Valido"] = false;
        switch ($FieldType) {
            case "BO":
                $PropCampos[$FieldId]["Valido"] = ($fieldValue == "true" | $fieldValue == "false" | $fieldValue == "0" | $fieldValue == "1");
                break;
            case "NU":
                $PropCampos[$FieldId]["Valido"] = is_numeric($fieldValue);
                break;
            default:
                $PropCampos[$FieldId]["Valido"] = true;
                break;
        }
    }

    // Procura campos não válidos
    while (list($fieldId, $requisitos) = each($PropCampos)) {
        // Trata campos não enviados e não obrigatórios
        if ($requisitos["Optional"] == 1 & $requisitos["Recebido"] == 0) {
            continue;
        }
        if ($requisitos["Valido"] == false) {
            GeraErro(6, $fieldId);
            $Retorno = false;
        }
    }
    return $Retorno;
}
