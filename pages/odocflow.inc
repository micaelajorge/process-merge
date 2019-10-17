<?php

// //Versao 1.0.0 /Versao
/* * **************************************************************************
 * Software: Docflow                                                        *
 * Version:  1.0                                                             *
 * Date:     2002/10/06                                                      *
 * Author:   Marcelo Mosczynski                                              *
 *                                                                           *
 *                                                                           *
 * Criado em 2002/10/06                                                      *
 * acao: 0																*		
 * *********************************1***************************************** */

define('Docflow', '1.0');
include_once(FILES_ROOT . "pages/ofieldsdoc.inc");
include_once(FILES_ROOT . "include/resource01.php");
include_once(FILES_ROOT . "include/calcdeadtime.php");

class DOCFLOW {

    var $CaseNum;
    var $ProcId;
    var $Fields;
    var $FieldsCount;
    var $connect;
    var $UserId;
    var $UserName;
    var $UserDesc;
    var $Prioridade;
    var $NovoCaso;
    var $ProcName;
    var $ProcDesc;
    var $Serverwww;
    var $TipoProc;
    var $samaccountname;
    var $userdef;
    var $FieldCodes;
    var $FieldValuesByCode;
    var $EventId;
    var $Origem;
    var $GravarEntradaDados;
    var $StepId;
    var $LogarAlteracoesFormulario = false;
    var $LogarAlteracoesCondicao = false;
    var $URL_ContentBr;
    var $URL_RMWS;
    var $URL_RMFEEDER;
    var $MacroAdminEditExec;
    var $Constants;

    function EnviarNotificacaoNovoCaso($StepId)
    {
        $retorno = 0;
        $SQL = "select count(*) as Send from notifications where ProcId = $this->ProcId and StepId = $StepId and NotificationType = 'NC'";
        $Query = mysqli_query($this->connect, $SQL);
        $Result = mysqli_fetch_array($Query);
        if ($Result["Send"] == 1) {
            $retorno = 1;
        }
        return $retorno;
    }

    function CasoEstaNaFila($StepId = 0)
    {
        /* 	if ($this->NovoCaso)
          {
          return true;
          }

          if ($StepId == 0)
          {
          $StepId = $this->StepId;
          }

         */
        $SQL = "select StepId from casequeue where ProcId = $this->ProcId and StepId = $StepId and CaseId = $this->CaseNum ";
        $Query = mysqli_query($this->connect, $SQL);
        return mysqli_num_rows($Query) > 0;
    }

    function IniciaStep($StepId, $InsertDate, $WaitForEvent)
    {
        if ($this->CasoEstaNaFila($StepId)) {
            return;
        }
        $EnviarNotificacao = $this->EnviarNotificacaoNovoCaso($StepId);
        $QueueDate = date("Y-m-d H:i:s");
        $SQL = "insert into ";
        $SQL .= "casequeue ";
        $SQL .= "( ";
        $SQL .= "ProcId, ";
        $SQL .= "StepId, ";
        $SQL .= "CaseId, ";
        $SQL .= "InsertDate, ";
        $SQL .= "AdHoc, ";
        $SQL .= "Priority, ";
        $SQL .= "SendNotification, ";
        $SQL .= "HideQueue, ";
        $SQL .= "QueueDate";
        $SQL .= ") ";
        $SQL .= "values ";
        $SQL .= "( ";
        $SQL .= "$this->ProcId, ";
        $SQL .= "$StepId, ";
        $SQL .= "$this->CaseNum, ";
        $SQL .= "'$InsertDate', ";
        $SQL .= "0, ";
        $SQL .= "$this->Prioridade, ";
        if ($WaitForEvent == 1) {
            $SQL .= " $EnviarNotificacao, ";
            $SQL .= " 1 ";
            $SQL .= " ) ";
        } else {
            $SQL .= " $EnviarNotificacao, ";
            $SQL .= " 0, ";
            $SQL .= "' $QueueDate'";
            $SQL .= " ) ";
        }
        if (!mysqli_query($this->connect, $SQL)) {
            error_log("Falha em Inicia STEP $SQL", 0);
        }
        $this->AtualizaDeadTimes($StepId);
    }

    function GravaOrigem()
    {
        $SQL = "update exportkeys set Origem = '$this->Origem' where CaseNum = $this->CaseNum";
        mysqli_query($this->connect, $SQL);
        for ($i = 0; $i < $this->FieldsCount; $i++) {
            if ($this->Fields->Field[$i]->FieldOrigem) {
                $this->Fields->SetFieldValue($this->connect, $i, $this->Origem);
                $this->Fields->Field[1]->AtualizaCampo($this->connect, $this->ProcId, $this->CaseNum);
                break;
            }
        }
    }

    function PegaFieldIdsByCode()
    {
        $SQL = "select FieldId, FieldCod from procfieldsdef where ProcId = $this->ProcId and Active = 1";
        $Query = mysqli_query($this->connect, $SQL);
        while ($Result = mysqli_fetch_array($Query)) {
            $FieldsCodes[trim($Result["FieldCod"])] = $Result["FieldId"];
        }
        return $FieldsCodes;
    }

    function GravaAlteracoesCampo($ConditionId = 0, $ConditionName = '')
    {
        if (is_array($this->Fields->CamposAlterados) && count($this->Fields->CamposAlterados) > 0) {
            $Campos = implode(",\n", $this->Fields->CamposAlterados);
            $Campos = "Alteração de Campos \n" . $Campos;
            $this->Atualizaaudittrail($this->StepId, $ConditionId, $Campos, $ConditionName);
        }
        $this->Fields->CamposAlterados = array();
    }

    function PegaDadosDePost($valores)
    {
        $contador = 1;
        while (list($key, $value) = each($valores)) {
            $$key = $value;
            if (substr($key, 0, 1) <> 't') {
                $contador = $contador + 1;
                continue;
            }
            if (!is_array($valores)) {
                $value = trim($value);
            }
            if (substr($key, 0, 1) <> "I") {
                if (substr($key, 0, 1) === 'h') {
                    $this->Fields->SetFieldValueAuxiliar($this->connect, substr($key, 1), $value);
                } else {
                    $this->Fields->SetFieldValue($this->connect, substr($key, 1), $value);
                }
            }
            $contador = $contador + 1;
        }
        if ($this->LogarAlteracoesFormulario) {
            $this->EventId = 503;
            $this->GravaAlteracoesCampo();
        }
    }

    function PegaDadosDeArray($dadosDoCaso) // casedata
    {
        if (!is_array($dadosDoCaso)) {
            return 0;
        }
        if (!key_exists("Fields", $dadosDoCaso)) {
            return 0;
        }
        if (!is_array($dadosDoCaso["Fields"])) {
            return 0;
        }
        for ($contador = 0; $contador < count($dadosDoCaso["Fields"]); $contador++) {
            if (isset($dadosDoCaso["Fields"][$contador]["FieldId"]))
            {
                $Field = $dadosDoCaso["Fields"][$contador]["FieldId"];
            } else {
                $Field = $dadosDoCaso["Fields"][$contador]["FieldCode"];
            }
            
            if (!is_numeric($Field)) {
                $Field = $this->FieldCodes[$Field];
                if (!is_numeric($Field)) {
                    continue;
                }
            }

            if (key_exists("Value", $dadosDoCaso["Fields"][$contador])) {
                $ValorDoCampo = $dadosDoCaso["Fields"][$contador]["Value"];
            } else {
                $ValorDoCampo = $dadosDoCaso["Fields"][$contador]["fieldValue"];
            }
            //error_log("Campo: $Field - Valor - $ValorDoCampo");
            if (!is_array($ValorDoCampo))
            {
                $ValorDoCampo = addslashes($ValorDoCampo);
            } else {
                for ($i = 0; $i < count($ValorDoCampo); $i++)
                {
                $ValorDoCampo[$i] = addslashes($ValorDoCampo[$i]);    
                }
            }            
            $this->Fields->SetFieldValue($this->connect, $Field, $ValorDoCampo);
        }
        if ($this->LogarAlteracoesFormulario) {
            $this->EventId = 502;
            $this->GravaAlteracoesCampo();
        }
        return 0;
    }

    function PegaInsertDate()
    {
        $SQL = "select InsertDate from casequeue where ";
        $SQL .= "ProcId = $this->ProcId ";
        $SQL .= "and ";
        $SQL .= "CaseId = $this->CaseNum ";
        $SQL .= "and ";
        $SQL .= "StepId = 0 ";
        $Query = mysqli_query($this->connect, $SQL);
        $Linha = mysqli_fetch_array($Query);
        return $Linha["InsertDate"];
    }

    function PegaDeadTime($StepId, $DeadTime)
    {
        $Valor = 0;
        $SQL = "select ";
        $SQL .= " Jornada, ";
        $SQL .= " JEntrada, ";
        $SQL .= " JSaida ";
        $SQL .= " from procdef ";
        $SQL .= " where ";
        $SQL .= " ProcId = $this->ProcId ";
        $Query = mysqli_query($this->connect, $SQL);
        $Linha = mysqli_fetch_array($Query);
        $JEntrada = $Linha["JEntrada"];
        $JSaida = $Linha["JSaida"];
        $Jornada = $Linha["Jornada"];
        if ($Jornada != 2) {
            $JEntrada = "2004-01-01 00:00";
            $JSaida = "2004-01-01 23:59";
        }


        // TODO: Unir para ganhar alguns millis
        $SQL = "select ";
        $SQL .= "Proc$DeadTime as DeadTime ";
        $SQL .= "from ";
        $SQL .= "procdef ";
        $SQL .= "where ";
        $SQL .= "ProcId = $this->ProcId ";

        $Query = mysqli_query($this->connect, $SQL);
        $Linha = mysqli_fetch_array($Query);
        $DeadTimeProc = $Linha["DeadTime"];

        $DeadTimeField = $DeadTime . "Field";
        $DeadStartField = $DeadTime . "StartField";

        $SQL = "select ";
        $SQL .= "$DeadTime as DeadTime, ";
        $SQL .= "$DeadTimeField as DeadField, ";
        $SQL .= "$DeadStartField as StartField ";
        $SQL .= "from ";
        $SQL .= "stepdef ";
        $SQL .= "where ";
        $SQL .= "StepId = $StepId ";
        $SQL .= "and ";
        $SQL .= "ProcId = $this->ProcId ";
        $Query = mysqli_query($this->connect, $SQL);
        $Linha = mysqli_fetch_array($Query);
        $DeadTimeStep = $Linha["DeadTime"];
        $DeadTimeField = $Linha["DeadField"];
        $DeadStartField = $Linha["StartField"];

        if (empty($DeadStartField)) {
            $StartDate = date("Y-m-d H:i");
        } else {
            $StartDate = PegaValorCampo($this->ProcId, $this->CaseNum, $DeadStartField);
        }

        if (empty($StartDate)) {
            $StartDate = date("Y-m-d H:i");
        }


        if ($StepId == 0 && $DeadTimeProc > 0) {
            return calcula_deaddatetime($StartDate, $DeadTimeProc, $JEntrada, $JSaida, $Jornada);
        }

        if ($DeadTimeStep != 0) {
            return calcula_deaddatetime($StartDate, $DeadTimeStep, $JEntrada, $JSaida, $Jornada);
        }

        if ($DeadTimeField > 0) {
            //$Data = date("Y-m-d H:i");
            $Valor = PegaValorCampo($this->ProcId, $this->CaseNum, $DeadTimeField);
            if ($Valor == 0) {
                return 0;
            } else {
                if (isdate($Valor)) {
                    if (!strpos($Valor, ":")) {
                        return $Valor . " 00:00:00";
                    }
                } else {
                    return calcula_deaddatetime($StartDate, $Valor, $JEntrada, $JSaida, $Jornada);
                }
            }
        } else {
            if ($DeadTimeProc > 0) {
                $SQL = "select ";
                $SQL .= $DeadTime . "DateTime as DeadTime ";
                $SQL .= "from ";
                $SQL .= "casequeue ";
                $SQL .= "where ";
                $SQL .= "StepId = 0 ";
                $SQL .= "and ";
                $SQL .= "ProcId = $this->ProcId ";
                $SQL .= "and ";
                $SQL .= "CaseId = $this->CaseNum ";
                $Query = mysqli_query($this->connect, $SQL);
                $Linha = mysqli_fetch_array($Query);
                return $Linha["DeadTime"];
            }
        }
        return $Valor;
    }

    function AtualizaDeadTimes($StepId)
    {
        // TODO: Unir para uma resposta em Array        
        $DeadSoftDate = $this->PegaDeadTime($StepId, "DeadSoft");
        $DeadHardDate = $this->PegaDeadTime($StepId, "DeadHard");
        $DeadHardestDate = $this->PegaDeadTime($StepId, "DeadHardest");
        $DeadSoftWait = 1;
        $DeadHardWait = 1;
        $DeadHardestWait = 1;
        $DeadSoftExec = 1;
        $DeadHardExec = 1;
        $DeadHardestExec = 1;

        if ($DeadSoftDate == 0) {
            $DeadSoftDate = date("Y-m-d H:i");
            $DeadTimes["SoftDateTime"] = 0;
            $DeadSoftWait = 0;
            $DeadSoftExec = 0;
        }

        if ($DeadHardDate == 0) {
            $DeadHardDate = date("Y-m-d H:i");
            $DeadTimes["HardDateTime"] = 0;
            $DeadHardWait = 0;
            $DeadHardExec = 0;
        }

        if ($DeadHardestDate == 0) {
            $DeadHardestDate = date("Y-m-d H:i");
            $DeadTimes["HardestDateTime"] = 0;
            $DeadHardestWait = 0;
            $DeadHardestExec = 0;
        }

        $SQL = " update ";
        $SQL .= " casequeue ";
        $SQL .= " set ";
        $SQL .= " lockedbyid = 0, ";
        $SQL .= " LockedBysamaccountname = '', ";
        $SQL .= " lockeddatetime = '', ";
        $SQL .= " deadsoftdatetime ='$DeadSoftDate', ";
        $SQL .= " deadharddatetime ='$DeadHardDate', ";
        $SQL .= " deadhardestdatetime ='$DeadHardestDate', ";
        $SQL .= " deadsoftWait = $DeadSoftWait, ";
        $SQL .= " deadHardWait = $DeadHardWait, ";
        $SQL .= " deadHardestWait = $DeadHardestWait, ";
        $SQL .= " deadsoftExec = $DeadSoftExec, ";
        $SQL .= " deadHardExec = $DeadHardExec, ";
        $SQL .= " deadHardestExec = $DeadHardestExec ";
        $SQL .= " where ";
        $SQL .= " ProcId = $this->ProcId ";
        $SQL .= " and ";
        $SQL .= " CaseId = $this->CaseNum ";
        $SQL .= " and ";
        $SQL .= " StepId = $StepId ";
        mysqli_query($this->connect, $SQL);
    }

    function _CriaCaso()
    {
        $this->EventId = 1;
        $this->Date = date("Y-m-d H:i");
        $Date = date("Y-m-d H:i");

        $CaseNum = $this->CaseNum;

        $this->WaitForEvent = $this->GetWaitForEvent($this->StepId);
        $this->Prioridade = 0;
        $this->IniciaStep($this->StepId, $Date, $this->WaitForEvent);

        $SQL = "insert into exportkeys (ProcId, CaseNum, Origem) values ($this->ProcId, $CaseNum, '')";
        mysqli_query($this->connect, $SQL);

        $SQL = "insert into exportkeysdisplay (ProcId, CaseNum) values ($this->ProcId, $CaseNum)";
        mysqli_query($this->connect, $SQL);

        $this->Atualizaaudittrail($this->StepId);
    }

    function Atualizaaudittrail($FieldId = 0, $Audit = 0, $textoDescritivo = '', $ConditionName = '', $StartStep = '')
    {
        $mensagem = addslashes($textoDescritivo);

        $StepName = PegaNomeStep($this->ProcId, $this->StepId);
        if (empty($FieldId)) {
            $FieldId = 0;
        }
        if ($this->StepId == '') {
            $this->StepId = 0;
        }
        $SQL = "insert into ";
        $SQL .= " audittrail ";
        $SQL .= " ( ";
        $SQL .= " ProcId, ";
        $SQL .= " StepId, ";
        $SQL .= " CaseNum,";
        $SQL .= " EventId,";
        $SQL .= " UserId,";
        $SQL .= " EventDatetime,";
        $SQL .= " FieldId,";
        $SQL .= " Audit, ";
        $SQL .= " UserName, ";
        $SQL .= " StepName, ";
        $SQL .= " ActionDesc, ";
        $SQL .= " ConditionName, ";
        $SQL .= " StartStep ";
        $SQL .= " ) ";
        $SQL .= " values ";
        $SQL .= " (";
        $SQL .= " $this->ProcId,";
        $SQL .= " $this->StepId,";
        $SQL .= " $this->CaseNum,";
        $SQL .= " $this->EventId,";
        $SQL .= " $this->UserId,";
        $SQL .= " CURRENT_TIMESTAMP, ";
        $SQL .= " $FieldId, ";
        $SQL .= " $Audit, ";
        $SQL .= " '$this->UserDesc', ";
        $SQL .= " '$StepName', ";
        $SQL .= " '$mensagem', ";
        $SQL .= " '$ConditionName', ";
        $SQL .= " '$StartStep' ";
        $SQL .= " ) ";
        $Query = mysqli_query($this->connect, $SQL);
        if (!$Query) {
            error_log("Falha em Atualiza audittrail $SQL");
        }
    }

    function SetConnection($connect)
    {
        $this->connect = $connect;
    }

    function SetProc($ProcId)
    {
        $this->ProcId = $ProcId;
        $this->_GetProc();
    }

    function _GetProc()
    {
        $SQL = " select ";
        $SQL .= " ProcName, ";
        $SQL .= " ProcDesc, ";
        $SQL .= " serverwww, ";
        $SQL .= " ContentAddress, ";
        $SQL .= " MacroAdminEditExec ";
        $SQL .= " from ";
        $SQL .= " procdef ";
        $SQL .= " where ";
        $SQL .= " ProcId = $this->ProcId ";
        $Query = mysqli_query($this->connect, $SQL);
        $Linha = mysqli_fetch_array($Query);
        $this->ProcName = $Linha["ProcName"];
        $this->ProcDesc = $Linha["ProcName"];
        $this->Serverwww = $Linha["serverwww"];
        $this->MacroAdminEditExec = $Linha["MacroAdminEditExec"];
//        $this->URL_ContentBr = $Linha["ContentAddress"];
//        if (empty($this->URL_ContentBr)) {
//            $RMWS = "";
//            include("include/conf_rm.php");
//            $this->URL_RMWS = $RMWS;
//            $this->URL_RMFEEDER = $RM_FEEDER;
//        } else {
//            $BarraFinalURL = substr($this->URL_ContentBr, -1, 1);
//            if ($BarraFinalURL != "/") {
//                $BarraFinalURL = "/";
//            } else {
//                $BarraFinalURL = "";
//            }
//            $this->URL_RMWS = $this->URL_ContentBr . $BarraFinalURL . "WS/rmws.asmx?WSDL";
//            $this->URL_RMFEEDER = $this->URL_ContentBr . $BarraFinalURL . "RMFEEDER.asmx?WSDL";
//        }

        mysqli_free_result($Query);
    }

    function CarregaConstants()
    {
        $this->Constants = array();
        $SQL = "select * from internalconstants where ProcId = $this->ProcId";
        $Query = mysqli_query($this->connect, $SQL);
        while ($Result = mysqli_fetch_array($Query)) {
            array_push($this->Constants, $Result);
        }
    }

    function PegaValorConstant($Codigo)
    {
        $Valor = "";
        if (!is_array($this->Constants)) {
            $this->CarregaConstants();
        }
        foreach ($this->Constants as $Constant) {
            if ($Constant["ConstCode"] == $Codigo) {
                $Valor = $Constant["ConstValue"];
            }
        }
        return $Valor;
    }

    function SetDocNumber($DocNumber) // Inicia o Caso com os valores dos Campos
    {
        if (!is_numeric($this->ProcId)) {
            error_log("ProId não setado Previamente - Erro SetDocNumber - docflow.php");
            return;
        }
        $this->DocNumber = $DocNumber;
        $this->Fields->fetch_values($this->connect, $this->DocNumber, $this->ProcId);
    }

    function FetchValuesRef()
    {
        if (!is_numeric($this->DocNumber)) {
            error_log("DocNumber não setado Previamente - Erro FetchValues - docflow.php");
            return;
        }
        $this->Fields->fetch_values_ref($this->connect, $this->DocNumber, $this->ProcId);
    }

    function FetchValues() // Carrega os valores dos Campos de um caso j� Definido
    {
        if (!is_numeric($this->DocNumber)) {
            error_log("DocNumber não setado Previamente - Erro FetchValues - docflow.php");
            return;
        }
        $this->Fields->fetch_values($this->connect, $this->DocNumber, $this->ProcId);
    }

    function open()
    {
        $this->_create();
    }

// Funcoes Privadas
    function _create()
    {
        $this->Fields = new FIELDS;
        $this->Fields->Cria($this->connect, $this->ProcId);
        $this->FieldsCount = $this->Fields->FieldsCount;
        $this->FieldCodes = $this->PegaFieldIdsByCode();
    }

}
