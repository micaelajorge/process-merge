// //Versao 1.0.0 /Versao
// JavaScript Document
function AbreArquivo(Source, ProcId, CaseNum) 
	{ 
	if (Source.length == 0)
		{
		alert("Não há arquivo para Abrir/Salvar");
		return;
		}
	WinSelect = window.open("BPMViewFile.php?Id1=" + ProcId + "&Id2=" + CaseNum + "&File=" + Source ,"Select","toolbar=0,Location=0,directories=0,status=1,menubar=0,scrollbars=1"); 
//	WinSelect = window.open(Source,"Select","toolbar=0,Location=0,directories=0,status=1,menubar=0,scrollbars=1"); 
	WinSelect.focus();	
	};