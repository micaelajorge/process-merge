<?php
class _RelatCasos {	
	var $Campos;
	var $CaseNum;
	var $NomeArquivo;
	var $connect;
	var $ProcId;
	var $Tipos;
	var $FieldsSel;
	var $FieldNames;
	var $Fields;
	var $File;
	var $CasosProcessados;
	var $CasosPorVez;
	var $NrAtualizacoes;
	var $ListaCasos = array();
	var $CasosProcessadosVez;
	var $TotalCasosVez;
	var $ProgressBar;
	
	function Processar()
	{
		$retorno = "";
		if ($this->CasosProcessadosVez == $this->TotalCasosVez)
			{
			$this->MontaProgressBar();
			}
		
		if ($this->TotalCasosVez == 0)
			{
			if ($this->CasosProcessados == 0)
				{
				$this->NenhumCaso();
				return;
				}
			else 
				{
				$this->Concluido();						
				return;
				}
			}
		$this->ProcessaCaso($CaseNum);
		return $this->Processado();
	}
		
	function Processado()
		{
		echo "document.getElementById('TD$this->CasosProcessadosVez').className = \"EX\";\n";
		$this->CasosProcessadosVez++;
		$this->CasosProcessados++;
		echo "document.getElementById('Processados').innerHTML = $this->CasosProcessados";	
		echo "ProcessaExport('ExportCVSAjax.php?Acao=Processar')";
		}
	
	function Concluido()
		{
		echo "Alert(\"Relat�rio gerado\");\n";
		echo "document.getElementById('Processados').innerHTML = $this->CasosProcessados";	
		}
		
	function NenhumCaso()
		{
		echo "Alert(\"Nenhum caso atendeu aos par�metros selecionados\");\n";
		}
			
	function MontaProgressBar()
	{
            
			$Query = $this->PegaCasosProcessar();
			$this->TotalCasosVez = mysqli_num_rows($Query);
			$ProgressBar = "<table style=\"height:20px;width:97%\" cellpadding=\"0\" cellspacing=\"0\"><tr>";
			$i = 0;
			while ($Result = mysqli_query($connect, $Query))
         {
                            array_push($this->ListaCasos, $Result["CaseId"]);
                            $ProgressBar .= "<td id=\"TD$i\" class=\"NEX\"><td>";			
			}
			$ProgressBar .= "</tr></table>";		
			$this->ProgressBar = $ProgressBar;
	}
		
	function PegaCasosProcessar()
	{
		$Top = $this->NrAtualizacoes + 1 * $this->CasosPorVez;
		$this->CasosProcessadosVez = 0;
		$this->NrAtualizacoes++;
		$SQL = "select Top $Top casequeue.CaseId from casequeue "; 
		if ($TipoAdmin == "ProcAdmin")
		{
			$SQLOrigeml = " , exportkeys, ";
			$SQLOrigeml .= " origemdominio ";
			$SQLOrigem2 = " and ";
			$SQLOrigem2 .= " exportkeys.Origem = origemdominio.Origem_DOC ";
			$SQLOrigem2 .= " and ";
			$SQLOrigem2 .= " origemdominio.Origem_uSER = '$Origem' ";
			$SQLOrigem2 .= " and ";
			$SQLOrigem2 .= " exportkeys.ProcId = casequeue.ProcId ";
			$SQLOrigem2 .= " and ";
			$SQLOrigem2 .= " exportkeys.CaseNum = casequeue.CaseId ";	
		}		
		$SQL .= $SQLOrigeml;
		$SQL .= " where StepId = 0 and casequeue.ProcId = $ProcId "; 

		$this->StatusTodos = "";
		if ($this->Ativos <> true || $this->Encerrados <> true)
		{
			if ($this->Ativos)
			{
				$StatusTodos = "Ativo";
				$SQL .= " and casequeue.CaseId in (select CaseId from casequeue  where ProcId = $ProcId group by CaseId having count(*) > 1)";
			}
			if ($this->Encerrados)
			{
				$StatusTodos = "Encerrado";
				$SQL .= " and casequeue.CaseId in (select CaseId from casequeue  where ProcId = $ProcId group by CaseId having count(*) = 1)";
			}
		}
				
		if (!empty($CaseNum))
		{
			$SQL .= " and CaseId = $CaseNum";
		}

		$SQL .= $SQLOrigem2;
		if (!empty($InicioCaso))
		{
			$Data = FormataData($InicioCaso);
			$SQL .= "and InsertDate >= '$Data'";
		}
	
		if (!empty($FimCaso))
		{
			$Data = FormataData($FimCaso);
			$Data = incDays($Data, 1);
			$SQL .= "and InsertDate <= '$Data'";
		}	
		$Query = mysqli_query($this->connect, $SQL);
		mysqli_data_seek($Query, $this->CasosProcessados);		
		return $Query;	
	}

	function MontaProcessamento()
		{
		$Query = $this->PegaCasos();
		}
	
	function CriaArquivo($NomeArquivo)
	{
		// Cria o Arquivo
		$NomeArquivo = date('y-m-d h-i-s') . " " . $NomeArquivo;
		$TipoArquivo  = filetype($NomeArquivo);
		if (empty($TipoArquivo))
		{
			$NomeArquivo .= ".csv";
		}
		$this->NomeArquivo = ".\\reports\\User " . $userdef->UserId . " " . $NomeArquivo;
		$this->File= fopen($file,"w+");				
	}
	
	function _RelatCasos($ProcId, $FieldsRelat, $InicioCaso, $FimCaso, $NomeArquivo, $TipoAdmin, $connect)
	{			
		$this->ProcId = $ProcId;
		$this->CasosProcessados = 0;	
		$this->CasosPorVez = 100;	
		$this->NrAtualizacoes = 0;
		$this->connect = $connect;
		$this->PegaCamposRelatorio($FieldsRelat);
		$this->CriaArquivo($NomeArquivo);
		$this->GravaCabecalho();
	}
	
	function ReabreArquivo()
	{
		$this->File = fopen($NomeArquivo, "a+");
	}
	
	function PegaStatusCaso($StatusTodos, $CaseNum)
	{
		global $ProcId, $connect;
		if (!empty($StatusTodos))		
		{
			return $StatusTodos;
		}
		$SQL = "Select count(*) as Total from casequeue where ProcId = $ProcId and CaseId = $CaseNum";
		$Query = mysqli_query($connect, $SQL);
		$Result = mysqli_fetch_array($Query);
		if ($Result["Total"] > 1) 
		{
			return "Ativo";
		}
		else 
		{
			return "Encerrado";
		}
	}
		
	function PegaPassosCaso($CaseNum)
	{
		global $connect, $ProcId;
		$SQL = "select StepName from 
		casequeue, 
		stepdef 
		where 
		CaseId = $CaseNum 
		and
		casequeue.ProcId = $ProcId 
		and
		casequeue.StepId > 0 
		and 
		casequeue.ProcId = stepdef.ProcId 
		and
		casequeue.StepId = stepdef.StepId
		order by StepName";
		$Query = mysqli_query($connect, $SQL);		
		$Steps = array();
		while ($Result = mysqli_fetch_array($Query))
		{
			array_push($Steps, $Result["StepName"]);
		}
		return implode(" / ", $Steps);		
	}		
//		
//	function PegaPassosCaso($CaseNum)
//	{
//		$SQL = "select StepName from 
//		casequeue, 
//		stepdef 
//		where 
//		CaseId = $CaseNum 
//		and
//		casequeue.ProcId = $this->ProcId 
//		and
//		casequeue.StepId > 0 
//		and 
//		casequeue.ProcId = stepdef.ProcId 
//		and
//		casequeue.StepId = stepdef.StepId
//		order by StepName";
//		$Query = mysqli_query($this->connect, $SQL);		
//		$Steps = array();
//		while ($Result = mysqli_fetch_array($Query))
//		{
//			array_push($Steps, $Result["StepName"]);
//		}
//		return implode(" / ", $Steps);		
//	}		
//	
	function ProcessaCaso($CaseNum)
		{	
		$CaseNum = $this->ListaCasos[$this->CasosProcessadosVez];	
		$Campos = $this->PegaValoes($CaseNum);
		$this->GravaLinha($Campos);
		}
		
	function Pegavalores($CaseNum)
	{
		$SQLData = "select CaseNum, FD.FieldId, FieldValue, FieldCod from procfieldsdef as FD, casedata as CD where CD.FieldId = FD.FieldId and CD.ProcId = Fd.ProcId and CaseNum = 50 and FD.FieldId in (1,2,3,4,5,6, 33, 35)";
		$Query = mssql_query($SQLData, $this->connect);
		$Campos["CaseNum"] = $CaseNum;
		if ($this->StepsCaso)
		{
			$Campos["Passos do Caso"] = PegaPassosCaso($Campos["CaseNum"]);
		}	
		if ($this->StatusCaso)
		{
			$Campos["Status"] = PegaStatusCaso($StatusTodos, $Campos["CaseNum"]);
		}
							
		while ($Result = mysqli_fetch_array($Query));
		{
			$Valor = FormataCampoRef($this->ProcId, $Result["FieldId"], $Result["FieldValue"], $this->Tipos[$Result["FieldId"]], 1, 0);				
			$Valor = ereg_replace("\n", " ", $Valor);
			$Valor = ereg_replace("\r", " ", $Valor);
			$Valor = str_replace("|",  " ", $Valor);
			$Valor = str_replace(",",  " ", $Valor);
			$Campos[$Result["FieldCod"] . $Result["FieldId"]] = "\"" . $Valor . "\"";			
		}	
		return $Campos;
	}	
	
	function PegaCamposRelatorio($FieldsRelat)
	{	
		$this->Fields = array();	
			
		$this->FieldsSel = "(" . implode(',', $FieldsRelat) . ")";
		$SQL = "select FieldType, FieldName, FieldId, FieldCod from procfieldsdef where ProcId = $ProcId  ";
		$SQL .= "and FieldId in $FieldsSel order by FieldName";
		$Query = mysqli_query($connect, $SQL);

		$FieldNames["CaseNum"] = "Caso";
		if ($this->StatusCaso)
		{
			$this->FieldNames["Status"] = "Status do Caso";
		}
		
		if ($this->StepsCaso)
		{
			$this->FieldNames["Passos do Caso"] = "Passos do Caso";
		}
			
		while ($Result = mysqli_fetch_array($Query))
		{
			$this->Tipos[$Result["FieldId"]] = $Result["FieldType"];
			array_push($this->Fields, $Result["FieldName"]);
			$this->Campos[$Result["FieldCod"] . $Result["FieldId"]] = "";
		}
	}

	function EnviaArquivo()
	{
    	header('Content-Type: application/octet-stream'); 
    	header('Content-Length: '.filesize($file)); 
    	header('Content-Disposition: attachment; filename="'.$this->NomeArquivo.'"'); 
    	readfile($this->File); 		
	}
	
	function GravaCabecalho()
	{
		$String = implode("|", $this->FieldNames) . "\n";	
		fwrite($this->File, $String);			
		fclose($this->File);						
	}
		
	function GravaLinha($Campos)
	{
		$String = implode("|", $this->FieldNames) . "\n";	
		fwrite($this->File, $String);
		fclose($this->File);						
	}	
}
?>