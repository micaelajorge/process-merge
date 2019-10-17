<?php

// //Versao 1.0.0 /Versao
/* * **************************************************************************
 * Software: fielddef                                                        *
 * Version:  1.0                                                             *
 * Date:     2002/10/06                                                      *
 * Author:   Marcelo Mosczynski                                              *
 *                                                                           *
 *                                                                           *
 * Criado em 2002/10/06                                                      *
 * Alteracao: 1  															*
 * Data: 28/11/2002															*
 * Alterado o  + 1 depois do FieldsCount no loop da fun��o AtualizaCampos	*
 * Pois N�o estava atualizando o campo emiss�o de etiqueta					*
 * ************************************************************************** */
define('fielddef', '1.0');

class FIELD {

    var $FieldName;
    var $FieldDesc;
    var $FieldType;
    var $FieldLength;
    var $FieldSourceField;
    var $FieldDisplayField;
    var $FieldRefOrder;
    var $FieldRef;
    var $FieldSourceTable;
    var $FieldSpecial;
    var $FieldValue;
    var $FieldValueDB;
    var $FieldValueOld;
    var $FieldValueDBOld;
    var $Alterado;
    var $ProcId;
    var $FieldId;
    var $FieldKey;
    var $FieldOrigem;
    var $FieldValueAuxiliar;
    var $FieldValueDados;
    var $FieldCode;
    var $ExtendProps;
    var $FieldMask;

    function FormataMascara()
    {
        $ValorFormatado = $this->FieldValue;
        switch ($this->FieldType) {
            case 'NU':
                $ValorFormatado = FormataMascaraNU($this->FieldValue, $this->FieldMask);
                break;
            case 'STS':
            case 'PRC':
                $ValorFormatado = PegaNomeStep($this->ProcId, $this->FieldValue);
                break;
        }
        return $ValorFormatado;
    }

    function atualizaCamposAddress($connect, $ProcId, $CaseNum)
    {
        $Tipos = array('GR', 'US');
        if (in_array($this->FieldType, $Tipos)) {
            $SQLLimpa = "delete from fieldaddresses where ProcId = $ProcId and CaseNum = $CaseNum and FieldId = $this->FieldId";
            mysqli_query($connect, $SQLLimpa);
            $SQLaddresses = "insert into fieldaddresses (ProcId, CaseNum, FieldId, FieldValue) values ($ProcId, $CaseNum, $this->FieldId, '$this->FieldValueDB')";
            mysqli_query($connect, $SQLaddresses);
        }
    }

    /**
     * 
     * @param type $connect
     * @param type $ProcId
     * @param type $CaseNum
     */
    function AtualizaCampoExportKey($connect, $ProcId, $CaseNum)
    {
        if ($this->FieldRef || $this->FieldKey) {
            $Campo = "Campo" . $this->FieldId;
            $SQL = "UPDATE exportkeys set $Campo ='$this->FieldValueDB' where ProcId = $ProcId and CaseNum = $CaseNum ";
            IF (!mysqli_query($connect, $SQL)) {
                $SQL2 = "alter table exportkeys add $Campo varchar(255)";
                $QUERY = mysqli_query($connect, $SQL2);
                $QUERY = mysqli_query($connect, $SQL);
            }
            $Campo = "CampoDisplay" . $this->FieldId;
            $ValorDisplay = $this->FormataMascara();
            $SQL_DISPLAY = "UPDATE exportkeysdisplay set $Campo ='$ValorDisplay' where CaseNum = $CaseNum and ProcId = $ProcId";
            IF (!mysqli_query($connect, $SQL_DISPLAY)) {
                $SQL_CRIACAMPO = "alter table exportkeysdisplay add $Campo varchar(255)";
                mysqli_query($connect, $SQL_CRIACAMPO);
                mysqli_query($connect, $SQL_DISPLAY);
            }
        }
    }

    /**
     * 
     * @param type $connect
     * @param type $ProcId
     * @param type $CaseNum
     */
    function AtualizaCampoOrigem($connect, $ProcId, $CaseNum)
    {
        if ($this->FieldOrigem) {
            $SQL = "update exportkeys set Origem = '$this->FieldValueDB' where CaseNum = $CaseNum and ProcId = $ProcId";
            $QUERY = mysqli_query($connect, $SQL);
        }
    }

    /**
     * 
     * @param type $connect
     * @param type $ProcId
     * @param type $CaseNum
     * @return int
     */
    function pegaTotalDeRegistrosCampo($connect, $ProcId, $CaseNum)
    {
        $tiposCamposRepetitivos = array("TM", "FD", "RD");

        if (in_array($this->FieldType, $tiposCamposRepetitivos)) {
            $SQL = " select ";
            $SQL .= " count(fieldid) AS total ";
            $SQL .= " from ";
            $SQL .= " casedata ";
            $SQL .= " where ";
            $SQL .= " ProcId = $ProcId ";
            $SQL .= " AND ";
            $SQL .= " CaseNum = $CaseNum ";
            $SQL .= " AND ";
            $SQL .= " fieldid = $this->FieldId ";
            $QUERY = mysqli_query($connect, $SQL);
            $linha = mysqli_fetch_array($QUERY);
        } else {
            $SQL = "delete from casedata where CaseNum = $CaseNum and FieldId = $this->FieldId ";
            mysqli_query($connect, $SQL);
            $linha["total"] = 0;
        }
        $totalRegistros = $linha["total"];
        return $totalRegistros;
    }

    function pegaValueId($valorItem)
    {
        if ($this->FieldType != "FD") {
            return false;
        }
        $jsonCampo = json_decode($valorItem, true);
        if (is_array($jsonCampo)) {
            if (key_exists("ValueId", $jsonCampo)) {
                return $jsonCampo["ValueId"];
            }
        }
        return -1;
    }

    function AtualizaCampo($connect, $ProcId, $CaseNum)
    {
        if ($this->FieldType == 'TM') {
            $Val = explode(';', $this->FieldValueDB);
            if (empty($Val[2])) {
                $this->Alterado = 0;
                return;
            }
        }

        $totalRegistrosCampo = $this->pegaTotalDeRegistrosCampo($connect, $ProcId, $CaseNum);

        if (!is_array($this->FieldValueDB)) {
            //$this->FieldValueDB = str_replace("'", "''", $this->FieldValueDB);
        }
        if ($this->FieldType == 'MA') {
            if (is_array($this->FieldValueAuxiliar)) {
                $this->FieldValueDB = ',' . implode(',', $this->FieldValueAuxiliar) . ';' . $this->FieldValueDB;
            } else {
                $this->FieldValueDB = ',' . $this->FieldValueAuxiliar . ';' . $this->FieldValueDB;
            }
        }

        // Define se Precisa ou não Atualizar as tabelas de Display
        $AtuDisplay = 1;
        if ($this->FieldRef || $this->FieldKey) {
            $AtuDisplay = 0;
        }

        $ValueId = $totalRegistrosCampo;

        // GravaDados na Tabela casedata
        if (!is_array($this->FieldValueDB)) {
            $this->comitaDadosTabelacasedata($connect, $ProcId, $CaseNum, $AtuDisplay, $ValueId, $this->FieldValueDB, false);
        } else {
            foreach ($this->FieldValueDB as $valorItem) {
                $ValueIdRecebido = $this->pegaValueId($valorItem);
                if ($ValueIdRecebido > -1) {
                    $ValueId = $ValueIdRecebido;
                    $atualizarCampo = true;
                }
                /*
                  $extendData = "";
                  if ($this->FieldType == "FD")
                  {
                  $aDados = json_decode($valorItem, true);
                  $extedData = $valorItem;
                  $valorItem = $aDados["fileName"];
                  }
                 * 
                 */
                $this->comitaDadosTabelacasedata($connect, $ProcId, $CaseNum, $AtuDisplay, $ValueId, $valorItem, $atualizarCampo, $extedData);
                $ValueId++;
            }
        }

        //Atualiza fieldaddresses
        $this->atualizaCamposAddress($connect, $ProcId, $CaseNum);

        // Aualiza Export Key e Campos Referencia
        $this->AtualizaCampoExportKey($connect, $ProcId, $CaseNum);

        $this->AtualizaCampoOrigem($connect, $ProcId, $CaseNum);

        $this->Alterado = 0;
    }

    /**
     * 
     * @param type $connect
     * @param type $ProcId
     * @param type $CaseNum
     * @param type $valorCampo
     * @param type $AtuDisplay
     * @param type $ValueId
     * @param type $totalRegistrosCampo
     */
    function comitaDadosTabelacasedata($connect, $ProcId, $CaseNum, $AtuDisplay, $ValueId, $valorCampo, $atualizar = true, $extendData = "")
    {
        if ($this->FieldType == 'TM' | $atualizar == false) {
            $SQL = "INSERT INTO "
                    . "casedata "
                    . "( "
                    . "ProcId, "
                    . "CaseNum, "
                    . "FieldId, "
                    . "FieldValue, "
                    . "CaseNum2, "
                    . "ValueId, "
                    . "AtuDisplay, "
                    . "extenddata"
                    . " ) "
                    . "values "
                    . "( "
                    . "$ProcId, "
                    . "$CaseNum, "
                    . "$this->FieldId, "
                    . "'$valorCampo', "
                    . "0, "
                    . "$ValueId, "
                    . "$AtuDisplay, "
                    . "'$extendData' "
                    . ") ";
        } else {
            $SQL = "UPDATE "
                    . "casedata "
                    . "set "
                    . "FieldValue= '$valorCampo', "
                    . "AtuDisplay = $AtuDisplay, "
                    . "extenddata = '$extendData' "
                    . "where ProcId = $ProcId "
                    . "and "
                    . "CaseNum = $CaseNum "
                    . "and "
                    . "FieldId = $this->FieldId "
                    . "and "
                    . "CaseNum2 = 0 "
                    . "and "
                    . "ValueId = $ValueId";
        }
        $resultado = mysqli_query($connect, $SQL);
        //error_log("Alteracao campo: " . $SQL);
        if (!$resultado)
        {
            error_log("Falha : \n $SQL \n" . mysqli_error($connect));
        }
    }

}

class FIELDS {

    var $FieldList;
    var $Field;      // Array associativo com as Propriedades dos Campos
    //var $field;
    var $FieldsCount;  // Quantidade de Campos
    var $StatusValues; // Determina Que dados foram lidos
    // valores Possiveis - 	OnlyRef   - valores apenas dos Campos de Referencia
    // AllFields - valores de Todos os Casos					
    var $Values;
    var $CamposAlterados;
    var $FieldsByCode;

    /**
     * 
     * @param type $CODIGO
     * @return type
     */
    function PegaValorCampoByCode($CODIGO)
    {
        if (!key_exists($CODIGO, $this->FieldsByCode)) {
            return "";
        }
        $FieldId = $this->FieldsByCode[$CODIGO];
        $Tipo = $this->Field[$FieldId]->FieldType;
        $ValorCampo = $this->Field[$FieldId]->FieldValue;
        if ($Tipo == "RM" || $Tipo == "RF") {
            $ValorCampo = $this->Field[$FieldId]->FieldValueDB;
            $ValorCampo = str_replace("^;^", "^/*#/*^", $ValorCampo);
            $AValor = explode("^/*#/*^", $ValorCampo);
            $ValorCampo = $AValor[0];
        }
        return $ValorCampo;
    }

    /**
     * 
     * @param type $CODIGO
     * @return type
     */
    function PegaValorDBCampoByCode($CODIGO)
    {
        $FieldId = $this->FieldsByCode[$CODIGO];
        $Tipo = $this->Field[$FieldId]->FieldType;
        $ValorCampo = $this->Field[$FieldId]->FieldValueDB;
        $ValorCampo = str_replace("^;^", "^/*#/*^", $ValorCampo);
        if ($Tipo == "RM" || $Tipo == "RF") {
            $AValor = explode("^/*#/*^", $ValorCampo);
            if (empty($AValor[1])) {
                $ValorCampo = $AValor[0];
            } else {
                $ValorCampo = $AValor[1];
            }
        }
        return $ValorCampo;
    }

    /**
     * 
     * @param type $Data
     * @return type
     */
    function FormataData($Data)
    {
        $result = $Data;
        if (strrpos($Data, "/")) {
            $AData = explode(" ", $Data);
            $result = substr($AData[0], 6, 4) . "-" . substr($AData[0], 3, 2) . "-" . substr($AData[0], 0, 2);
            if ($AData[1]) {
                $result = $result . " " . substr($AData[1], 0, 5);
            } else {
                //$result = $result . " 00:00";
            }
        } else {
            $AData = explode(" ", $Data);
            if (count($AData) > 1) {
                $result = $AData[0];
                $result = $result . " " . $AData[1];
            } else {
                if (!empty($Data)) {
                    $result = $Data . " 00:00";
                }
            }
        }
        return trim($result);
    }

    /**
     * 
     * @param type $connect
     * @param type $FieldId
     * @param type $Valor
     */
    function SetFieldValueAuxiliar($connect, $FieldId, $Valor)
    {
        $this->Field[$FieldId]->FieldValueAuxiliar = $Valor;
    }

    /**
     * 
     * @param type $FieldId
     */
    function SetFieldValue_LT($FieldId)
    {
        $valores = explode(";", $this->Field[$FieldId]->ExtendProps);
        $valores = explode(",", $valores[0]);
        $valores = explode(".", $valores[0]);
        foreach ($valores as $Valor) {
            $Display = $Valor;
            $ValorItem = $Valor;
            $Dado = explode("|", $Valor);
            if (count($Dado) > 1) {
                $Display = $Dado[0];
                $ValorItem = $Dado[1];
            }
            if ($ValorItem == $this->Field[$FieldId]->FieldValueDB || $Display == $this->Field[$FieldId]->FieldValueDB) {
                $this->Field[$FieldId]->FieldValue = $Display;
                $this->Field[$FieldId]->FieldValueDB = $ValorItem;
                break;
            }
        }
    }

    /**
     * 
     * @global type $userdef
     * @param type $connect
     * @param type $FieldId
     * @param type $ValorCampo
     */
    function SetFieldValue($connect, $FieldId, $pValorCampo)
    {
        global $userdef;

        if (is_array($pValorCampo)) {
            for ($i = 0; $i < count($pValorCampo); $i++) {
                if (is_utf8($pValorCampo[$i])) {
                    //$Valor[$i] = utf8_decode($Valor[$i]);
                }                
            }
            $Valor = $pValorCampo;
        } else {
            if (is_utf8($pValorCampo)) {
                //$Valor = utf8_decode($Valor);
            }
        }

        $this->Field[$FieldId]->FieldValueDB = $pValorCampo;
        $tipoCampo = $this->Field[$FieldId]->FieldType;

        // TODO Tratar depois melhor para quando vier via POST do formulário
//        if (in_array($tipo, array("AR", "IM"))) {
//            return;
//        }

        $ValorCampo = str_replace("/*!/*", ",", str_replace("\'", "/*%/*", $pValorCampo));
        if (!($tipoCampo == "RM" || $tipoCampo == "RF" || $tipoCampo == "DC")) {
            $ValorCampo = str_replace(";", "/*#/*", $ValorCampo);
        }

        $this->Field[$FieldId]->FieldValue = "";

        switch ($tipoCampo) {
//            case "FD":
//                $this->Field[$FieldId]->FieldValue = str_replace("\"\"","\"", $this->Field[$FieldId]->FieldValue); 
//                break;
            case "LT":
                $this->SetFieldValue_LT($FieldId);
                break;
            case "AR":
                $this->Field[$FieldId]->FieldValue = $this->Field[$FieldId]->FieldValueDB;
                break;
            case "TB":
                $this->Field[$FieldId]->FieldValue = $this->_fetch_Value_Field_TB($connect, $this->field);
                break;
            case "BO":
                $this->Field[$FieldId]->FieldValue = $this->_fetch_Value_Field_BOOL($this->Field[$FieldId]->FieldValueDB);
                break;
            case "GR":
                $this->Field[$FieldId]->FieldValue = $this->_fetch_Value_Field_GR($connect, $this->Field[$FieldId]->FieldValueDB);
                //$this->Field[$FieldId]->FieldValue = $this->Field[$FieldId]->FieldValueDB;
                break;
            case "US":
                $this->Field[$FieldId]->FieldValue = $this->_fetch_Value_Field_US($connect, $this->Field[$FieldId]->FieldValueDB);
                break;
            case "LT":
                $this->Field[$FieldId]->_fetch_Value_Field_LT();
                break;
            case "DT":
                $this->Field[$FieldId]->FieldValueDB = $this->FormataData($this->Field[$FieldId]->FieldValueDB);
                $this->Field[$FieldId]->FieldValue = ConvDate($this->Field[$FieldId]->FieldValueDB, false);
                //$this->Field[$FieldId]->FieldValue = $this->Field[$FieldId]->FieldValueDB;			
                break;
            case "NU":
            case "TX":
            case "TL":
                // Trata qdo tiver aspas e barras
                $this->Field[$FieldId]->FieldValueDB = str_replace("\\\"", "\"", $this->Field[$FieldId]->FieldValueDB);
                $this->Field[$FieldId]->FieldValueDB = str_replace("\\\\", "\\", $this->Field[$FieldId]->FieldValueDB);
                $this->Field[$FieldId]->FieldValue = $this->Field[$FieldId]->FieldValueDB;
                break;
            case "TM";
                $this->Field[$FieldId]->FieldValueDB = date("Y-m-d H:i") . ";" . $userdef->UserName . ";" . str_replace(";", "/*#/*", $this->Field[$FieldId]->FieldValueDB);
                break;
            case "MC";
                if (is_array($pValorCampo)) {
                    $Valor[0] = '';
                    $this->Field[$FieldId]->FieldValueDB = "'" . str_replace(",", "','", trim(implode(",", $Valor))) . "'";
                } else {
                    $this->Field[$FieldId]->FieldValueDB = "'" . str_replace(",", "','", trim($Valor)) . "'";
                }
                break;
            case "MA";
                $Valor[0] = '';
                $this->Field[$FieldId]->FieldValueDB = trim(implode(",", $Valor));
                break;
            case "ATB";
                $Valor = str_replace('-1,', '', str_replace(',-1', '', trim(implode(",", $Valor))));
                $this->Field[$FieldId]->FieldValueDB = $Valor;
                break;
            case "ATX";
                if (!is_array($Valor)) {
                    $AValor = array($Valor);
                    $Valor = $AValor;
                }                
                // Remove Valores em Branco do Array                
                $Valor = array_filter($Valor);
                
                // Converte para Json o Valor
                $Valor = json_encode($Valor);
                $this->Field[$FieldId]->FieldValueDB = $Valor;
                break;
            case "AUS";
                $Valor = str_replace('-1,', '', str_replace(',-1', '', trim(implode(",", $Valor))));
                $this->Field[$FieldId]->FieldValueDB = $Valor;
                break;
            case "AGR";
                $Valor = str_replace('-1,', '', str_replace(',-1', '', trim(implode(",", $Valor))));
                $this->Field[$FieldId]->FieldValueDB = $Valor;
                break;
            case "ANU";
                $Valor = str_replace('-1,', '', str_replace(',-1', '', trim(implode(",", $Valor))));
                $this->Field[$FieldId]->FieldValueDB = $Valor;
                break;
            case "ATB";
                $Valor = str_replace('-1,', '', str_replace(',-1', '', trim(implode(",", $Valor))));
                $this->Field[$FieldId]->FieldValueDB = $Valor;
                break;
            case "ADT":
                for ($i = 0; $i < count($Valor); $i++) {
                    $Valor[$i] = $this->FormataData($Valor[$i]);
                }
                $this->Field[$FieldId]->FieldValueDB = implode(",", $Valor);
                $this->Field[$FieldId]->FieldValue = $this->Field[$FieldId]->FieldValueDB;
                break;
            case "SFR":
                $valores = explode(";", $this->Field[$FieldId]->ExtendProps);
                $valores = explode(",", $valores[0]);
                foreach ($valores as $Valor) {
                    $Valor = explode("|", $Valor);
                    if ($Valor[1] == $this->Field[$FieldId]->FieldValueDB) {
                        $this->Field[$FieldId]->FieldValue = $Valor[0];
                        break;
                    }
                }
                break;
            default:
                $this->Field[$FieldId]->FieldValue = $this->Field[$FieldId]->FieldValueDB;
                break;
        }
        if (!is_array($this->CamposAlterados)) {
            $this->CamposAlterados = array();
        }

        if ($this->Field[$FieldId]->FieldType <> 'TM' & $this->Field[$FieldId]->FieldType <> "DC") {
            if (trim($this->Field[$FieldId]->FieldValueDBOld) != trim($this->Field[$FieldId]->FieldValueDB)) {
                $ValorOldDB = $this->Field[$FieldId]->FieldValueDBOld;
                if ($ValorOldDB == '') {
                    $ValorOld = "(vazio)";
                } else {
                    $ValorOld = $this->Field[$FieldId]->FieldValueOld;
                    if ($this->Field[$FieldId]->FieldValueDBOld != $this->Field[$FieldId]->FieldValueOld) {
                        $ValorOld = $this->Field[$FieldId]->FieldValueOld . " (" . $this->Field[$FieldId]->FieldValueDBOld . ")";
                    }
                }
                $NovoValor = $this->Field[$FieldId]->FieldValueDB;
                if ($this->Field[$FieldId]->FieldValueDB != $this->Field[$FieldId]->FieldValue) {
                    $NovoValor = $this->Field[$FieldId]->FieldValue . " (" . $this->Field[$FieldId]->FieldValueDB . ")";
                }
                $Campo = $this->Field[$FieldId]->FieldName . " (" . $this->Field[$FieldId]->FieldCode . ")" . " alterado de $ValorOld para $NovoValor";
                array_push($this->CamposAlterados, $Campo);
            }
        }
        $this->Field[$FieldId]->Alterado = 1;
    }

    /**
     * 
     * @param type $connect
     * @param type $ProcId
     * @param type $StepId
     */
    function Cria($connect, $ProcId, $StepId)
    {
        $this->_create($connect, $ProcId);
    }

// Funcoes Privadas
    /**
     * 
     * @param type $connect
     * @param type $ProcId
     */
    function _create($connect, $ProcId)
    {
        $SQL = "select ";
        $SQL .= " * ";
        $SQL .= " from ";
        $SQL .= " procfieldsdef ";
        $SQL .= " where ";
        $SQL .= " ProcId = $ProcId ";
//	$SQL.=" and";
//	$SQL.=" Active = 1";
        $Query = mysqli_query($connect, $SQL);
        $this->FieldsCount = mysqli_num_rows($Query);
        $contador = 1;
        while ($Linha = mysqli_fetch_array($Query)) {
            $this->Field[$Linha["FieldId"]] = new FIELD;
            $this->setPropsField($Linha["FieldId"], $Linha);
            $this->FieldList[$contador] = $this->Field[$Linha["FieldId"]]->FieldId;
            $contador++;
        }
    }

    /**
     * 
     * @param type $FieldId
     * @param type $Linha
     */
    function setPropsField($FieldId, $Linha)
    {
        $this->Field[$Linha["FieldId"]]->FieldId = $Linha["FieldId"];
        $this->Field[$Linha["FieldId"]]->FieldName = $Linha["FieldName"];
        $this->Field[$Linha["FieldId"]]->FieldDesc = $Linha["FieldDesc"];
        $this->Field[$Linha["FieldId"]]->FieldType = $Linha["FieldType"];
        $this->Field[$Linha["FieldId"]]->FieldLength = $Linha["FieldLength"];
        $this->Field[$Linha["FieldId"]]->FieldSourceField = $Linha["FieldSourceField"];
        $this->Field[$Linha["FieldId"]]->FieldDisplayField = $Linha["FieldDisplayField"];
        $this->Field[$Linha["FieldId"]]->FieldSpecial = $Linha["FieldSpecial"];
        $this->Field[$Linha["FieldId"]]->FieldSourceTable = str_replace("''", "'", $Linha["FieldSourceTable"]);
        $this->Field[$Linha["FieldId"]]->FieldRef = $Linha["FieldRef"];
        $this->Field[$Linha["FieldId"]]->FieldRefOrder = $Linha["FieldRefOrder"];
        $this->Field[$Linha["FieldId"]]->ProcId = $Linha["ProcId"];
        $this->Field[$Linha["FieldId"]]->Alterado = 0;
        $this->Field[$Linha["FieldId"]]->FieldKey = $Linha["FieldKey"];
        $this->Field[$Linha["FieldId"]]->FieldOrigem = $Linha["FieldOrigem"];
        $this->Field[$Linha["FieldId"]]->FieldCode = $Linha["FieldCod"];
        $this->Field[$Linha["FieldId"]]->ExtendProps = $Linha["ExtendProp"];
        $this->Field[$Linha["FieldId"]]->FieldMask = $Linha["FieldMask"];
        $this->FieldsByCode[$Linha["FieldCod"]] = $Linha["FieldId"];
    }

    /**
     * 
     * @param type $DocNumber
     * @param type $ProcId
     */
    function fetch_values_ref($DocNumber, $ProcId)
    {
        //error_log('Carregando Valor de Campos', 0);
        while (list($key, $this->Field) = each($this->Field)) {
            $this->Field[$this->Field->FieldId]->FieldValue = $this->_fetch_Value_Field($DocNumber, $ProcId, $this->Field->FieldId);
        }
        //error_log('Valor de Campos carregados', 0);
        $this->StatusValues = "OnlyRef";
    }

    /**
     * 
     * @param type $FieldId
     * @param type $ValorDB
     * @return type
     */
    function _fetch_Value_Field_TL($FieldId, $ValorDB)
    {
        $valores = explode(";", $this->Field[$FieldId]->ExtendProps);
        $valores = explode(",", $valores[0]);
        foreach ($valores as $Valor) {
            $Display = $Valor;
            $ValorItem = $Valor;
            $Dado = explode("|", $Valor);
            if (count($Dado) > 1) {
                $Display = $Dado[0];
                $ValorItem = $Dado[1];
            }
            if ($ValorItem == $ValorDB || $Display == $ValorDB) {
                return $Display;
            }
        }
        return $ValorDB;
    }

    /**
     * 
     * @param type $connect
     * @return string
     */
    function _fetch_Value_Field_TB($connect)
    {
        if (!empty($this->field->FieldValueDB)) {
            return $this->_fetch_Value_Field_Table($connect, $this->field->FieldValueDB, $this->field->FieldSourceTable, $this->field->FieldDisplayField, $this->field->FieldSourceField);
        }
        return "";
    }

    /**
     * 
     * @param type $connect
     * @param type $FieldValueDB
     * @return string
     */
    function _fetch_Value_Field_US($connect, $FieldValueDB)
    {
        if (!empty($FieldValueDB)) {
            return PegaNomeUsuario($FieldValueDB);
            //return $this->_fetch_Value_Field_Table($connect, $FieldValueDB, "userdef", "UserName", "UserId");
        }
        return "ND";
    }

    /**
     * 
     * @param type $connect
     * @return string
     */
    function _fetch_Value_Field_GR($connect, $FieldValueDB)
    {
        if (!empty($FieldValueDB)) {
            return $this->_fetch_Value_Field_Table($connect, $FieldValueDB, "groupdef", "GroupName", "GroupId");
        }
        return "ND";
    }

    /**
     * 
     * @param type $connect
     * @param type $FieldId
     * @param type $ProcId
     * @param type $ValorDB
     * @return type
     */
    function _fetch_Value_Field_LT($connect, $FieldId, $ProcId, $ValorDB)
    {
        return $ValorDB;
        /*
          if (!empty($field->FieldValueDB))
          {
          return "ND";
          }
          $SQL = " select ";
          $SQL.="  ExtendProp as OptValues ";
          $SQL.="  from ";
          $SQL.="  procfieldsdef ";
          $SQL.="  where ";
          $SQL.="  ProcId = $ProcId";
          $SQL.="  and ";
          $SQL.="  FieldId = $FieldId ";
          $Query = mysqli_query($connect, $SQL);
          $result = mysqli_fetch_array($Query);
          mysqli_free_result($Query);
          $valores = str_replace("\"","",$result["OptValues"]);
          $valores = explode(",",$valores);
          //	echo "Valor" . $valores[$ValorDB] . "<br>";
          return $valores[$ValorDB];
         */
    }

    /**
     * 
     * @param type $connect
     * @param type $Value
     * @param type $SourceTable
     * @param type $DisplayField
     * @param type $SourceField
     * @return type
     */
    function _fetch_Value_Field_Table($connect, $Value, $SourceTable, $DisplayField, $SourceField)
    {
        $SQL = "select $DisplayField from $SourceTable where $SourceField = '$Value'";
        $Query = mysqli_query($connect, $SQL);
        $Linha = mysqli_fetch_array($Query);
        mysqli_free_result($Query);
        return $Linha[$DisplayField];
    }
        
    /**
     * 
     * @param type $connect
     * @param type $CaseNum
     * @return type
     */
    function FieldValues($connect, $CaseNum)
    {
        $SQL = "select FieldValue, FieldId, ValueId from casedata where CaseNum = $CaseNum order by FieldId, ValueId desc";
        $Query = mysqli_query($connect, $SQL);
        $fieldIdOld = "";
        while ($Linha = mysqli_fetch_array($Query)) {
            if ($fieldIdOld != $Linha["FieldId"])
            {
                $Campos[$Linha["FieldId"]] = $Linha["FieldValue"];
            } else {                
                $valorCampo = $Campos[$Linha["FieldId"]];
                if (!is_array($valorCampo))
                {
                    $valorCampo = array();
                    $valorCampo[] = $Campos[$Linha["FieldId"]];
                }                  
                $valorCampo[] = $Linha["FieldValue"];
                $Campos[$Linha["FieldId"]] = $valorCampo;
            }
            $fieldIdOld = $Linha["FieldId"];
        }
        return $Campos;
    }

    /**
     * 
     * @param type $connect
     * @param type $DocNumber
     * @param type $ProcId
     */
    function fetch_values($connect, $DocNumber, $ProcId)
    {
        $Campos = $this->FieldValues($connect, $DocNumber);
        foreach ($this->Field as $Field) {
            if (key_exists($Field->FieldId, $Campos)) {
                $Field->FieldValueDB = $Campos[$Field->FieldId];
            } else {
                $Field->FieldValueDB = "";
            }

            switch ($Field->FieldType) {
                case "FD":
                    break;
                case "TB":
                    $Field->FieldValue = $this->_fetch_Value_Field_TB($connect);
                    break;
                case "BO":
                    $Field->FieldValue = $this->_fetch_Value_Field_BOOL($Field->FieldValueDB);
                    break;
                case "US":
                    $Field->FieldValue = $this->_fetch_Value_Field_US($connect, $Field->FieldValueDB);
                    break;
                case "DT":
                    $Field->FieldValueDB = $this->FormataData($Field->FieldValueDB);
                    $Field->FieldValue = ConvDate($Field->FieldValueDB);
                    break;
                case "RM":
                case "RF":
                    if (strpos($Field->FieldValueDB, "^")) {
                        $Field->FieldValue = substr($Field->FieldValueDB, strrpos($Field->FieldValueDB, "^") + 1);
                    }
                    break;
                case "LT":
                    $Field->FieldValue = $this->_fetch_Value_Field_TL($Field->FieldId, $Field->FieldValueDB);
                    break;
                case "DC":
                    $Field->FieldValue = $Field->FieldValueDB;
                    break;
                default:
                    $Field->FieldValue = $Field->FieldValueDB;
                    break;
            }
            $this->Field[$Field->FieldId]->FieldValueDBOld = $Field->FieldValueDB;
            $this->Field[$Field->FieldId]->FieldValueOld = $Field->FieldValue;
            $this->Field[$Field->FieldId]->FieldValueDB = $Field->FieldValueDB;
            $this->Field[$Field->FieldId]->FieldValue = $Field->FieldValue;
        }
        $this->StatusValues = "AllFields";
    }

    /**
     * 
     * @param type $Valor
     * @return string
     */
    function _fetch_Value_Field_BOOL($Valor)
    {
        if ($Valor == 1) {
            return "Sim";
        }
        return "Não";
    }

    /**
     * 
     * @param type $connect
     * @param type $DocNumber
     * @param type $ProcId
     * @param type $Field
     * @return type
     */
    function _fetch_Value_Field($connect, $DocNumber, $ProcId, $Field)
    {
        $SQL = "select ";
        $SQL .= " FieldValue ";
        $SQL .= " from ";
        $SQL .= " casedata ";
        $SQL .= " where ";
        $SQL .= " CaseNum = $DocNumber ";
        $SQL .= " and ";
        $SQL .= " ProcId = $ProcId ";
        $SQL .= " and ";
        $SQL .= " FieldId = $Field ";
        @$Query = mysqli_query($connect, $SQL);
        @$Linha = mysqli_fetch_array($Query);
        return $Linha["FieldValue"];
    }

    /**
     * 
     * @param type $Valor
     * @return string
     */
    function _fetch_Value_Field_LIST($Valor)
    {
        if ($Valor == 1) {
            return "Sim";
        }
        return "N�o";
    }

    /**
     * 
     * @param type $connect
     * @param type $ProcId
     * @param type $CaseNum
     */
    function AtualizaCampos($connect, $ProcId, $CaseNum)
    {
        $Values = "";
        for ($contador = 1; $contador < $this->FieldsCount + 1; $contador++) {
            $Values = $Values . $this->Field[$this->FieldList[$contador]]->FieldValueDB . "!%!";
            if ($this->Field[$this->FieldList[$contador]]->Alterado == 1) {
                $this->Field[$this->FieldList[$contador]]->AtualizaCampo($connect, $ProcId, $CaseNum);
            }
        }
        $Values = str_replace("'", "''", $Values);
    }

}
