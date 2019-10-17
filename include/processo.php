<?php

class procdef {

    var $DeadSoft;
    var $DeadHard;
    var $DeadHardest;
    var $InstanceName;
    var $connect;
    var $ProcId;
    var $ProcName;
    var $Steps;
    var $DefaultView;
    var $ProcCod;
    var $URL_CONTENT;
    var $Ticker;
    var $ProcColor;
    var $ProcIcon;
    var $ProcDesc;
    var $fieldUnique;
    var $fieldCasoFechado;
    var $TipoProc;
    var $StepFiltro;
    var $filterQueue;

    function Create($ProcId, $connect) {
        $this->connect = $connect;
        $this->ProcId = $ProcId;
        $this->DadosProc();
        $this->Steps();
        $this->StepFiltro = "";
    }

    function DadosProc() {
        // Verifica se veio o Código ou o Id do Processo
        if (is_numeric($this->ProcId))
        {
            $SQL_PROC = "ProcId = $this->ProcId ";
        }
        else
        {
            $SQL_PROC = "ProcCod = '$this->ProcId' ";
        }
        $SQL = "select * from procdef where $SQL_PROC";
        $Query = mysqli_query($this->connect, $SQL);
        $result = mysqli_fetch_array($Query);
        $this->ProcId = $result["ProcId"];
        $this->DeadSoft = $result['ProcDeadSoft'];
        $this->DeadHard = $result['ProcDeadHard'];
        $this->DeadHardest = $result['ProcDeadHardest'];
        $this->InstanceName = $result['InstanceName'];
        $this->ProcName = $result['ProcName'];
        $this->ProcCod = trim($result["ProcCod"]);
        $this->TipoProc = $result['TipoProc'];
        $this->DefaultView = 0;
        if (isset($result['Ticker']))
        {
            $this->Ticker = $result['Ticker'];
        }                   
        $this->URL_CONTENT = trim($result['ContentAddress']);
        $this->ProcDesc = $result["ProcDesc"];
        if ($result["ProcIcon"] == "") {
            $result["ProcIcon"] = "fa fa-tasks";
        }
        $this->ProcIcon = $result["ProcIcon"];
        $this->ProcColor = $result["ProcColor"];
        $this->ExendProps = $result["extendPropsProc"];
    }

    function Steps() {
        $SQL = "select * from stepdef where ProcId = $this->ProcId order by StepId";
        $Query = mysqli_query($this->connect, $SQL);
        while ($result = mysqli_fetch_array($Query, MYSQLI_ASSOC)) {
            $this->Steps[$result["StepId"]] = $result;
            // Default View
            if ($result["DefaultView"] == '1') {
                $this->DefaultView = $result["StepId"];
            }
        }
    }

}

?>