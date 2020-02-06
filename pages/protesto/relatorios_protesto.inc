<?php

/*
  Criacao: Micaela Fernanda
  Data Criacao 19/01/2020
  Sistema: Process
  ROTA: relatorios_protesto
 */
require_once(FILES_ROOT . '/include/resource01.php');
require_once(FILES_ROOT . '/include/inctime.php');

use raelgc\view\Template;

/**
 * Gera o arquivo em XLS
 * 
 * @global type $connect
 * @global type $STEPNAME
 * @global type $PROCNAME
 * @global type $estado
 * @global type $nomerelatorio
 */
function relatorioXLS()
{
    global $connect, $STEPNAME, $PROCNAME, $estado, $nomerelatorio;
    $t_body = new Template(FILES_ROOT . "assets/templates/protesto/t_relatorio_campos.html");

    $tipoArquivo = "excel";
    $nomerelatorio = "";
    $PROCCOD = "PROTESTO_TITULOS";
    $STEPCOD = "TRIADO";
    $procId = PegaProcIdByCode($PROCCOD);    
    $stepId = PegaStepIdByCode($procId, $STEPCOD);
    $fieldSelecao = PegaFieldIdByCode($procId, "UF_COMARCA");
    $UF = $estado;   
    $TODAY = "";
    $campoEstado = "";
    $t_body->STEPNAME = $STEPNAME;
    $t_body->PROCNAME = $PROCNAME;
  
// <editor-fold defaultstate="collapsed" desc="Campos do Relatorio">
//    $SQL_CAMPOS = "select "
//            . "procfieldsdef.fieldId, "
//            . "fieldtype, "
//            . "fieldname, "
//            . "readonly as agrupar, "
//            . "optional as somar "
//            . "from "
//            . "stepfieldsdef, procfieldsdef  "
//            . "where "
//            . "procfieldsdef.procId = $procId and procfieldsdef.procid = stepfieldsdef.procid "
//            . "and procfieldsdef.fieldId = stepfieldsdef.fieldid and StepId = $stepId order by orderId";
//    $Query = mysqli_query($connect, $SQL_CAMPOS);
//    $camposSelecionados = mysqli_fetch_all($Query, MYSQLI_ASSOC);
//    
//    

    $camposRelatorio[] = array();
    $camposBD[] = array();
    switch ($UF){
       case "AC": 
            break;
        case "AL": 
            break;
        case "AP": 
            break;
        case "AM": 
            break;
        case "BA": 
            break;
        case "CE": 
            break;
        case "DF": 
                        // <editor-fold defaultstate="collapsed" desc="fieldname">
            $camposRelatorio[0]["fieldname"] = "NOMESACADOR";	
            $camposRelatorio[1]["fieldname"] = "DOCUMENTOSACADOR";	
            $camposRelatorio[2]["fieldname"] = "ENDERECOSACADOR";	
            $camposRelatorio[3]["fieldname"] = "CEPSACADOR";	
            $camposRelatorio[4]["fieldname"] = "CIDADESACADOR";	
            $camposRelatorio[5]["fieldname"] = "UFSACADOR";	
            $camposRelatorio[6]["fieldname"] = "CEDENTE";	
            $camposRelatorio[7]["fieldname"] = "DEVEDOR";	
            $camposRelatorio[8]["fieldname"] = "SEQUENCIALDEVEDOR";	
            $camposRelatorio[9]["fieldname"] = "CPFCNPJ";	
            $camposRelatorio[10]["fieldname"] = "ENDERECO";	
            $camposRelatorio[11]["fieldname"] = "CEP";	
            $camposRelatorio[12]["fieldname"] = "BAIRRO";	
            $camposRelatorio[13]["fieldname"] = "MUNICIPIO";	
            $camposRelatorio[14]["fieldname"] = "ESTADO";	
            $camposRelatorio[15]["fieldname"] = "NUMEROTITULO";	
            $camposRelatorio[16]["fieldname"] = "EMISSAO";	
            $camposRelatorio[17]["fieldname"] = "VENCIMENTO";	
            $camposRelatorio[18]["fieldname"] = "VALOR";	
            $camposRelatorio[19]["fieldname"] = "SALDO";	
            $camposRelatorio[20]["fieldname"] = "NOSSONUMERO";
            $camposRelatorio[21]["fieldname"] = "ESPECIE";	
            $camposRelatorio[22]["fieldname"] = "COMARCA";	
            $camposRelatorio[23]["fieldname"] = "DECLARACAO";	
            $camposRelatorio[24]["fieldname"] = "ENDOSSO";
            // </editor-fold>
            
            // <editor-fold defaultstate="collapsed" desc="fieldcode">
                        $camposBD[0] = "NOME_CREDOR";	$camposBD[1] = "DOC_CREDOR";	$camposBD[2] = "";	$camposBD[3] = "";	$camposBD[4] = "";	$camposBD[5] = "";	$camposBD[6] = "NOME_CEDENTE";	$camposBD[7] = "NOME_DEVEDOR_0";	$camposBD[8] = "SEQUENCIAL_DEVEDOR";	$camposBD[9] = "DOC_DEVEDOR_0";	$camposBD[10] = "ENDERECO_DEVEDOR_0";	$camposBD[11] = "CEP_DEVEDOR_0";	$camposBD[12] = "BAIRRO_DEVEDOR_0";	$camposBD[13] = " CIDADE_IBGE_0";	$camposBD[14] = "UF_DEVEDOR_0";	$camposBD[15] = "NUM_TITULO";	$camposBD[16] = "DATA_EMISSAO";	$camposBD[17] = "DATA_VENCIMENTO";	$camposBD[18] = "VALOR_TITULO";	$camposBD[19] = "VALOR_PROTESTADO";	$camposBD[20] = "NOSSO_NUMERO";	$camposBD[21] = "ESPECIE_TITULO";	$camposBD[22] = "COMARCA";	$camposBD[23] = "DECLARACAO";	$camposBD[24] = "ENDOSSO";
            // </editor-fold>
            
            $campoEstado = "UF_DEVEDOR_0";
            $TODAY = date("dm.y");
            $nomerelatorio = "XCCC".$TODAY."1";
            break;
        case "ES": 
            break;
        case "GO": 
            // <editor-fold defaultstate="collapsed" desc="fieldname">
            $camposRelatorio[0]["fieldname"] = "NOMESACADOR";	
            $camposRelatorio[1]["fieldname"] = "DOCUMENTOSACADOR";	
            $camposRelatorio[2]["fieldname"] = "ENDERECOSACADOR";	
            $camposRelatorio[3]["fieldname"] = "CEPSACADOR";	
            $camposRelatorio[4]["fieldname"] = "CIDADESACADOR";	
            $camposRelatorio[5]["fieldname"] = "UFSACADOR";	
            $camposRelatorio[6]["fieldname"] = "CEDENTE";	
            $camposRelatorio[7]["fieldname"] = "DEVEDOR";	
            $camposRelatorio[8]["fieldname"] = "SEQUENCIALDEVEDOR";	
            $camposRelatorio[9]["fieldname"] = "CPFCNPJ";	
            $camposRelatorio[10]["fieldname"] = "ENDERECO";	
            $camposRelatorio[11]["fieldname"] = "CEP";	
            $camposRelatorio[12]["fieldname"] = "BAIRRO";	
            $camposRelatorio[13]["fieldname"] = "MUNICIPIO";	
            $camposRelatorio[14]["fieldname"] = "ESTADO";	
            $camposRelatorio[15]["fieldname"] = "NUMEROTITULO";	
            $camposRelatorio[16]["fieldname"] = "EMISSAO";	
            $camposRelatorio[17]["fieldname"] = "VENCIMENTO";	
            $camposRelatorio[18]["fieldname"] = "VALOR";	
            $camposRelatorio[19]["fieldname"] = "SALDO";	
            $camposRelatorio[20]["fieldname"] = "NOSSONUMERO";
            $camposRelatorio[21]["fieldname"] = "ESPECIE";	
            $camposRelatorio[22]["fieldname"] = "COMARCA";	
            $camposRelatorio[23]["fieldname"] = "DECLARACAO";	
            $camposRelatorio[24]["fieldname"] = "ENDOSSO";
            // </editor-fold>
            
            // <editor-fold defaultstate="collapsed" desc="fieldcode">
                        $camposBD[0] = "NOME_CREDOR";	$camposBD[1] = "DOC_CREDOR";	$camposBD[2] = "";	$camposBD[3] = "";	$camposBD[4] = "";	$camposBD[5] = "";	$camposBD[6] = "NOME_CEDENTE";	$camposBD[7] = "NOME_DEVEDOR_0";	$camposBD[8] = "SEQUENCIAL_DEVEDOR";	$camposBD[9] = "DOC_DEVEDOR_0";	$camposBD[10] = "ENDERECO_DEVEDOR_0";	$camposBD[11] = "CEP_DEVEDOR_0";	$camposBD[12] = "BAIRRO_DEVEDOR_0";	$camposBD[13] = " CIDADE_IBGE_0";	$camposBD[14] = "UF_DEVEDOR_0";	$camposBD[15] = "NUM_TITULO";	$camposBD[16] = "DATA_EMISSAO";	$camposBD[17] = "DATA_VENCIMENTO";	$camposBD[18] = "VALOR_TITULO";	$camposBD[19] = "VALOR_PROTESTADO";	$camposBD[20] = "NOSSO_NUMERO";	$camposBD[21] = "ESPECIE_TITULO";	$camposBD[22] = "COMARCA";	$camposBD[23] = "DECLARACAO";	$camposBD[24] = "ENDOSSO";
            // </editor-fold>
            $campoEstado = "UF_DEVEDOR_0";
            $TODAY = date("dm.y");
            $nomerelatorio = "X60J".$TODAY."1";
            break;
        case "MA": 
            break;
        case "MT": 
            // <editor-fold defaultstate="collapsed" desc="fieldname">
            $camposRelatorio[0]["fieldname"] = "NOMESACADOR";	
            $camposRelatorio[1]["fieldname"] = "DOCUMENTOSACADOR";	
            $camposRelatorio[2]["fieldname"] = "ENDERECOSACADOR";	
            $camposRelatorio[3]["fieldname"] = "CEPSACADOR";	
            $camposRelatorio[4]["fieldname"] = "CIDADESACADOR";	
            $camposRelatorio[5]["fieldname"] = "UFSACADOR";	
            $camposRelatorio[6]["fieldname"] = "CEDENTE";	
            $camposRelatorio[7]["fieldname"] = "DEVEDOR";	
            $camposRelatorio[8]["fieldname"] = "SEQUENCIALDEVEDOR";	
            $camposRelatorio[9]["fieldname"] = "CPFCNPJ";	
            $camposRelatorio[10]["fieldname"] = "ENDERECO";	
            $camposRelatorio[11]["fieldname"] = "CEP";	
            $camposRelatorio[12]["fieldname"] = "BAIRRO";	
            $camposRelatorio[13]["fieldname"] = "MUNICIPIO";	
            $camposRelatorio[14]["fieldname"] = "ESTADO";	
            $camposRelatorio[15]["fieldname"] = "NUMEROTITULO";	
            $camposRelatorio[16]["fieldname"] = "EMISSAO";	
            $camposRelatorio[17]["fieldname"] = "VENCIMENTO";	
            $camposRelatorio[18]["fieldname"] = "VALOR";	
            $camposRelatorio[19]["fieldname"] = "SALDO";	
            $camposRelatorio[20]["fieldname"] = "NOSSONUMERO";
            $camposRelatorio[21]["fieldname"] = "ESPECIE";	
            $camposRelatorio[22]["fieldname"] = "COMARCA";	
            $camposRelatorio[23]["fieldname"] = "DECLARACAO";	
            $camposRelatorio[24]["fieldname"] = "ENDOSSO";
//</editor-fold>            
            
            // <editor-fold defaultstate="collapsed" desc="fieldcode">
            $camposBD[0] = "NOME_CREDOR";	$camposBD[1] = "DOC_CREDOR";	$camposBD[2] = "";	$camposBD[3] = "";	$camposBD[4] = "";	$camposBD[5] = "";	$camposBD[6] = "NOME_CEDENTE";	$camposBD[7] = "NOME_DEVEDOR_0";	$camposBD[8] = "SEQUENCIAL_DEVEDOR";	$camposBD[9] = "DOC_DEVEDOR_0";	$camposBD[10] = "ENDERECO_DEVEDOR_0";	$camposBD[11] = "CEP_DEVEDOR_0";	$camposBD[12]["fieldname"] = "BAIRRO_DEVEDOR_0";	$camposBD[13] = " CIDADE_IBGE_0";	$camposBD[14] = "UF_DEVEDOR_0";	$camposBD[15] = "NUM_TITULO";	$camposBD[16] = "DATA_EMISSAO";	$camposBD[17] = "DATA_VENCIMENTO";	$camposBD[18] = "VALOR_TITULO";	$camposBD[19] = "VALOR_PROTESTADO";	$camposBD[20] = "NOSSO_NUMERO";	$camposBD[21] = "ESPECIE_TITULO";	$camposBD[22] = "COMARCA";	$camposBD[23] = "DECLARACAO";	$camposBD[24] = "ENDOSSO";
            // </editor-fold>
            $campoEstado = "UF_DEVEDOR_0";
            $TODAY = date("dm.y");
            $nomerelatorio = "XCCC".$TODAY."1";


            break;
        case "MS": 
            
            // <editor-fold defaultstate="collapsed" desc="fieldname">            
            $camposRelatorio[0]["fieldname"] = "NOMESACADOR";	
            $camposRelatorio[1]["fieldname"] = "DOCUMENTOSACADOR";	
            $camposRelatorio[2]["fieldname"] = "ENDERECOSACADOR";	
            $camposRelatorio[3]["fieldname"] = "CEPSACADOR";	
            $camposRelatorio[4]["fieldname"] = "CIDADESACADOR";	
            $camposRelatorio[5]["fieldname"] = "UFSACADOR";	
            $camposRelatorio[6]["fieldname"] = "CEDENTE";	
            $camposRelatorio[7]["fieldname"] = "DEVEDOR";	
            $camposRelatorio[8]["fieldname"] = "SEQUENCIALDEVEDOR";	
            $camposRelatorio[9]["fieldname"] = "CPFCNPJ";	
            $camposRelatorio[10]["fieldname"] = "ENDERECO";	
            $camposRelatorio[11]["fieldname"] = "CEP";	
            $camposRelatorio[12]["fieldname"] = "BAIRRO";	
            $camposRelatorio[13]["fieldname"] = "MUNICIPIO";	
            $camposRelatorio[14]["fieldname"] = "ESTADO";	
            $camposRelatorio[15]["fieldname"] = "NUMEROTITULO";	
            $camposRelatorio[16]["fieldname"] = "EMISSAO";	
            $camposRelatorio[17]["fieldname"] = "VENCIMENTO";	
            $camposRelatorio[18]["fieldname"] = "VALOR";	
            $camposRelatorio[19]["fieldname"] = "SALDO";	
            $camposRelatorio[20]["fieldname"] = "NOSSONUMERO";
            $camposRelatorio[21]["fieldname"] = "ESPECIE";	
            $camposRelatorio[22]["fieldname"] = "COMARCA";	
            $camposRelatorio[23]["fieldname"] = "DECLARACAO";	
            $camposRelatorio[24]["fieldname"] = "ENDOSSO";
//</editor-fold>            
            
            // <editor-fold defaultstate="collapsed" desc="fieldcode">
            $camposBD[0] = "NOME_CREDOR";	$camposBD[1] = "DOC_CREDOR";	$camposBD[2] = "";	$camposBD[3] = "";	$camposBD[4] = "";	$camposBD[5] = "";	$camposBD[6] = "NOME_CEDENTE";	$camposBD[7] = "NOME_DEVEDOR_0";	$camposBD[8] = "SEQUENCIAL_DEVEDOR";	$camposBD[9] = "DOC_DEVEDOR_0";	$camposBD[10] = "ENDERECO_DEVEDOR_0";	$camposBD[11] = "CEP_DEVEDOR_0";	$camposBD[12] = "BAIRRO_DEVEDOR_0";	$camposBD[13] = " CIDADE_IBGE_0";	$camposBD[14] = "UF_DEVEDOR_0";	$camposBD[15] = "NUM_TITULO";	$camposBD[16] = "DATA_EMISSAO";	$camposBD[17] = "DATA_VENCIMENTO";	$camposBD[18] = "VALOR_TITULO";	$camposBD[19] = "VALOR_PROTESTADO";	$camposBD[20] = "NOSSO_NUMERO";	$camposBD[21] = "ESPECIE_TITULO";	$camposBD[22] = "COMARCA";	$camposBD[23] = "DECLARACAO";	$camposBD[24] = "ENDOSSO";
            // </editor-fold>
            $campoEstado = "UF_DEVEDOR_0";
             $TODAY = date("dm.y");
            $nomerelatorio = "XCCC".$TODAY."1";
            
            break;
        case "MG": 
            
            // <editor-fold defaultstate="collapsed" desc="fieldname">
            $camposRelatorio[0]["fieldname"] = "NOME SACADOR";	
            $camposRelatorio[1]["fieldname"] = "DOCUMENTOSACADOR";	
            $camposRelatorio[2]["fieldname"] = "ENDERECOSACADOR";	
            $camposRelatorio[3]["fieldname"] = "CEPSACADOR";	
            $camposRelatorio[4]["fieldname"] = "CIDADESACADOR";	
            $camposRelatorio[5]["fieldname"] = "UFSACADOR";	
            $camposRelatorio[6]["fieldname"] = "CEDENTE";	
            $camposRelatorio[7]["fieldname"] = "DEVEDOR";	
            $camposRelatorio[8]["fieldname"] = "SEQUENCIALDEVEDOR";	
            $camposRelatorio[9]["fieldname"] = "CPFCNPJ";	
            $camposRelatorio[10]["fieldname"] = "ENDERECO";	
            $camposRelatorio[11]["fieldname"] = "CEP";	
            $camposRelatorio[12]["fieldname"] = "BAIRRO";	
            $camposRelatorio[13]["fieldname"] = "MUNICIPIO";	
            $camposRelatorio[14]["fieldname"] = "ESTADO";	
            $camposRelatorio[15]["fieldname"] = "NUMEROTITULO";	
            $camposRelatorio[16]["fieldname"] = "EMISSAO";	
            $camposRelatorio[17]["fieldname"] = "VENCIMENTO";	
            $camposRelatorio[18]["fieldname"] = "VALOR";	
            $camposRelatorio[19]["fieldname"] = "SALDO";	
            $camposRelatorio[20]["fieldname"] = "NOSSONUMERO";	
            $camposRelatorio[21]["fieldname"] = "ESPECIE";	
            $camposRelatorio[22]["fieldname"] = "COMARCA";	
            $camposRelatorio[23]["fieldname"] = "DECLARACAO";	
            $camposRelatorio[24]["fieldname"] = "ENDOSSO";
//</editor-fold>            
            
            // <editor-fold defaultstate="collapsed" desc="fieldcode">
           $camposBD[0] = "NOME_CREDOR";	$camposBD[1] = "DOC_CREDOR";	$camposBD[2] = "";	$camposBD[3] = "";	$camposBD[4] = "";	$camposBD[5] = "";	$camposBD[6] = "NOME_CEDENTE";	$camposBD[7] = "NOME_DEVEDOR_0";	$camposBD[8] = "SEQUENCIAL_DEVEDOR";	$camposBD[9] = "DOC_DEVEDOR_0";	$camposBD[10] = "ENDERECO_DEVEDOR_0";	$camposBD[11] = "CEP_DEVEDOR_0";	$camposBD[12] = "BAIRRO_DEVEDOR_0";	$camposBD[13] = " CIDADE_IBGE_0";	$camposBD[14] = "UF_DEVEDOR_0";	$camposBD[15] = "NUM_TITULO";	$camposBD[16] = "DATA_EMISSAO";	$camposBD[17] = "DATA_VENCIMENTO";	$camposBD[18] = "VALOR_TITULO";	$camposBD[19] = "VALOR_PROTESTADO";	$camposBD[20] = "NOSSO_NUMERO";	$camposBD[21] = "ESPECIE_TITULO";	$camposBD[22] = "COMARCA";	$camposBD[23] = "DECLARACAO";	$camposBD[24] = "ENDOSSO";
            //</editor-fold>
            $campoEstado = "UF_DEVEDOR_0";
            $TODAY = date("dm.y");
            $nomerelatorio = "XD76".$TODAY."1";
            break;
        case "PA": 
            break;
        case "PB": 
            break;
        case "PR": 
            break;
        case "PE": 
            break;
        case "PI": 
            break;
        case "RJ": 
            
            // <editor-fold defaultstate="collapsed" desc="fieldname">            
            $camposRelatorio[0]["fieldname"] = "NOMESACADOR";	
            $camposRelatorio[1]["fieldname"] = "DOCUMENTOSACADOR";	
            $camposRelatorio[2]["fieldname"] = "ENDERECOSACADOR";	
            $camposRelatorio[3]["fieldname"] = "CEPSACADOR";	
            $camposRelatorio[4]["fieldname"] = "CIDADESACADOR";	
            $camposRelatorio[5]["fieldname"] = "UFSACADOR";	
            $camposRelatorio[6]["fieldname"] = "CEDENTE";	
            $camposRelatorio[7]["fieldname"] = "DEVEDOR";	
            $camposRelatorio[8]["fieldname"] = "SEQUENCIALDEVEDOR";	
            $camposRelatorio[9]["fieldname"] = "CPFCNPJ";	
            $camposRelatorio[10]["fieldname"] = "ENDERECO";	
            $camposRelatorio[11]["fieldname"] = "CEP";	
            $camposRelatorio[12]["fieldname"] = "BAIRRO";	
            $camposRelatorio[13]["fieldname"] = "MUNICIPIO";	
            $camposRelatorio[14]["fieldname"] = "ESTADO";	
            $camposRelatorio[15]["fieldname"] = "NUMEROTITULO";	
            $camposRelatorio[16]["fieldname"] = "EMISSAO";	
            $camposRelatorio[17]["fieldname"] = "VENCIMENTO";	
            $camposRelatorio[18]["fieldname"] = "VALOR";	
            $camposRelatorio[19]["fieldname"] = "SALDO";	
            $camposRelatorio[20]["fieldname"] = "NOSSONUMERO";
            $camposRelatorio[21]["fieldname"] = "ESPECIE";	
            $camposRelatorio[22]["fieldname"] = "COMARCA";	
            $camposRelatorio[23]["fieldname"] = "DECLARACAO";	
            $camposRelatorio[24]["fieldname"] = "ENDOSSO";
//</editor-fold>            
            
            // <editor-fold defaultstate="collapsed" desc="fieldcode">
            $camposBD[0] = "NOME_CREDOR";	$camposBD[1] = "DOC_CREDOR";	$camposBD[2] = "";	$camposBD[3] = "";	$camposBD[4] = "";	$camposBD[5] = "";	$camposBD[6] = "NOME_CEDENTE";	$camposBD[7] = "NOME_DEVEDOR_0";	$camposBD[8] = "SEQUENCIAL_DEVEDOR";	$camposBD[9] = "DOC_DEVEDOR_0";	$camposBD[10] = "ENDERECO_DEVEDOR_0";	$camposBD[11] = "CEP_DEVEDOR_0";	$camposBD[12] = "BAIRRO_DEVEDOR_0";	$camposBD[13] = " CIDADE_IBGE_0";	$camposBD[14] = "UF_DEVEDOR_0";	$camposBD[15] = "NUM_TITULO";	$camposBD[16] = "DATA_EMISSAO";	$camposBD[17] = "DATA_VENCIMENTO";	$camposBD[18] = "VALOR_TITULO";	$camposBD[19] = "VALOR_PROTESTADO";	$camposBD[20] = "NOSSO_NUMERO";	$camposBD[21] = "ESPECIE_TITULO";	$camposBD[22] = "COMARCA";	$camposBD[23] = "DECLARACAO";	$camposBD[24] = "ENDOSSO";
            // </editor-fold>
            $campoEstado = "UF_DEVEDOR_0";
            $TODAY = date("dm.y");
            $nomerelatorio = "XT48".$TODAY."1";
            break;
        case "RN": 
            break;
        case "RS": 
            
            // <editor-fold defaultstate="collapsed" desc="fieldname">            
            $camposRelatorio[0]["fieldname"] = "NUMEROTITULO";	
            $camposRelatorio[1]["fieldname"] = "NOSSONUMERO";	
            $camposRelatorio[2]["fieldname"] = "VALOR";	
            $camposRelatorio[3]["fieldname"] = "SALDO";	
            $camposRelatorio[4]["fieldname"] = "DEVEDOR";	
            $camposRelatorio[5]["fieldname"] = "CPFCNPJ";	
            $camposRelatorio[6]["fieldname"] = "ENDERECO";	
            $camposRelatorio[7]["fieldname"] = "BAIRRO";	
            $camposRelatorio[8]["fieldname"] = "MUNICIPIO";	
            $camposRelatorio[9]["fieldname"] = "CEP";	
            $camposRelatorio[10]["fieldname"] = "ESTADO";	
            $camposRelatorio[11]["fieldname"] = "EMISSAO";	
            $camposRelatorio[12]["fieldname"] = "VENCIMENTO";	
            $camposRelatorio[13]["fieldname"] = "ESPECIE";	
            $camposRelatorio[14]["fieldname"] = "COMARCA";	
            $camposRelatorio[15]["fieldname"] = "DECLARACAO";
//</editor-fold>            
            
            // <editor-fold defaultstate="collapsed" desc="fieldcode">   
            $camposBD[0] = "NUM_TITULO";	$camposBD[1] = "NOSSO_NUMERO";	$camposBD[2] = "VALOR_TITULO";	$camposBD[3] = "VALOR_PROTESTADO";	$camposBD[4] = "NOME_DEVEDOR_0";	$camposBD[5] = "DOC_DEVEDOR_0";	$camposBD[6] = "ENDERECO_DEVEDOR_0";	$camposBD[7] = "BAIRRO_DEVEDOR_0";	$camposBD[8] = " CIDADE_IBGE_0";	$camposBD[9] = "CEP_DEVEDOR_0";	$camposBD[10] = "UF_DEVEDOR_0";	$camposBD[11] = "DATA_EMISSAO";	$camposBD[12] = "DATA_VENCIMENTO";	$camposBD[13] = "ESPECIE_TITULO";	$camposBD[14] = "COMARCA";	$camposBD[15] = "DECLARACAO";
            //</editor-fold>   
            $campoEstado = "UF_DEVEDOR_0";
            $TODAY = date("dmy");
            $nomerelatorio = "XL66".$TODAY;
            break;
        case "RO": 
            break;
        case "RR": 
            break;
        case "SC": 
            
            // <editor-fold defaultstate="collapsed" desc="fieldname">            
            $camposRelatorio[0]["fieldname"] = "DEVEDOR";	
            $camposRelatorio[1]["fieldname"] = "CPFCNPJ";	
            $camposRelatorio[2]["fieldname"] = "ENDERECO";	
            $camposRelatorio[3]["fieldname"] = "CEP";	
            $camposRelatorio[4]["fieldname"] = "BAIRRO";	
            $camposRelatorio[5]["fieldname"] = "MUNICIPIO";	
            $camposRelatorio[6]["fieldname"] = "ESTADO";	
            $camposRelatorio[7]["fieldname"] = "NUMEROTITULO";	
            $camposRelatorio[8]["fieldname"] = "EMISSAO";	
            $camposRelatorio[9]["fieldname"] = "VENCIMENTO";	
            $camposRelatorio[10]["fieldname"] = "VALOR";	
            $camposRelatorio[11]["fieldname"] = "SALDO";	
            $camposRelatorio[12]["fieldname"] = "ESPECIE";	
            $camposRelatorio[13]["fieldname"] = "ENDOSSO";	
            $camposRelatorio[14]["fieldname"] = "ACEITE";	
            $camposRelatorio[15]["fieldname"] = "NOSSONUMERO";	
            $camposRelatorio[16]["fieldname"] = "FALIMENTAR";	
            $camposRelatorio[17]["fieldname"] = "NOMESACADOR";	
            $camposRelatorio[18]["fieldname"] = "DOCUMENTOSACADOR";	
            $camposRelatorio[19]["fieldname"] = "ENDERECOSACADOR";	
            $camposRelatorio[20]["fieldname"] = "CEPSACADOR";	
            $camposRelatorio[21]["fieldname"] = "CIDADESACADOR";	
            $camposRelatorio[22]["fieldname"] = "UFSACADOR";	
            $camposRelatorio[23]["fieldname"] = "CEDENTE";
            //</editor-fold>      
            
            // <editor-fold defaultstate="collapsed" desc="fieldcode">   
            $camposBD[0] = "NOME_DEVEDOR_0";	$camposBD[1] = "DOC_DEVEDOR_0";	$camposBD[2] = "ENDERECO_DEVEDOR_0";	$camposBD[3] = "CEP_DEVEDOR_0";	$camposBD[4] = "BAIRRO_DEVEDOR_0";	$camposBD[5] = " CIDADE_IBGE_0";	$camposBD[6] = "UF_DEVEDOR_0";	$camposBD[7] = "NUM_TITULO";	$camposBD[8] = "DATA_EMISSAO";	$camposBD[9] = "DATA_VENCIMENTO";	$camposBD[10] = "VALOR_TITULO";	$camposBD[11] = "VALOR_PROTESTADO";	$camposBD[12] = "ESPECIE_TITULO";	$camposBD[13] = "ENDOSSO";	$camposBD[14] = "ACEITE";	$camposBD[15] = "NOSSO_NUMERO";	$camposBD[16] = "FALIMENTAR";	$camposBD[17] = "NOME_CREDOR";	$camposBD[18] = "DOC_CREDOR";	$camposBD[19] = "";	$camposBD[20] = "";	$camposBD[21] = "";	$camposBD[22] = "";	$camposBD[23] = "NOME_CEDENTE";
            //</editor-fold>  
            
            $TODAY = date("dmy");
            $nomerelatorio = "XNQ6".$TODAY."1";
            break;
        case "SP": 
            
            // <editor-fold defaultstate="collapsed" desc="fieldname">            
                $camposRelatorio[0]["fieldname"] = "nosso_numero";
                $camposRelatorio[0]["fieldcode"] = "NOSSO_NUMERO";	
                $camposRelatorio[1]["fieldname"] = "tipo_protesto";
                $camposRelatorio[2]["fieldname"] = "especie";	
                $camposRelatorio[3]["fieldname"] = "num_titulo";	
                $camposRelatorio[4]["fieldname"] = "data_emissao";	
                $camposRelatorio[5]["fieldname"] = "data_vencimento";	
                $camposRelatorio[6]["fieldname"] = "valor";	
                $camposRelatorio[7]["fieldname"] = "valor_protestado";	
                $camposRelatorio[8]["fieldname"] = "praca_protesto_ibge";	
                $camposRelatorio[9]["fieldname"] = "tipo_divida_CDA";	
                $camposRelatorio[10]["fieldname"] = "identificacao_divida_CDA";	
                $camposRelatorio[11]["fieldname"] = "CCB_emitida_por_CBI";	
                $camposRelatorio[12]["fieldname"] = "tp_doc_credor";	
                $camposRelatorio[13]["fieldname"] = "doc_credor";	
                $camposRelatorio[14]["fieldname"] = "nome_credor";	
                $camposRelatorio[15]["fieldname"] = "tp_doc_cedente";	
                $camposRelatorio[16]["fieldname"] = "doc_cedente";	
                $camposRelatorio[17]["fieldname"] = "nome_cedente";	
                $camposRelatorio[18]["fieldname"] = "tp_doc_devedor_principal";	
                $camposRelatorio[19]["fieldname"] = "doc_devedor_principal";	                
                $camposRelatorio[20]["fieldname"] = "nome_devedor_principal";	
                $camposRelatorio[21]["fieldname"] = "cep_devedor_principal";	
                $camposRelatorio[22]["fieldname"] = "endereco_devedor_principal";	
                $camposRelatorio[23]["fieldname"] = "numero_end_devedor_principal";	
                $camposRelatorio[24]["fieldname"] = "complem_end_devedor_principal";	
                $camposRelatorio[25]["fieldname"] = "bairro_devedor_principal";	
                $camposRelatorio[26]["fieldname"] = "cidade_ibge_devedor_principal";	
                $camposRelatorio[27]["fieldname"] = "uf_devedor_principal";	
                $camposRelatorio[28]["fieldname"] = "comarca_cartório";	
                $camposRelatorio[29]["fieldname"] = "num_cartório";	
                $camposRelatorio[30]["fieldname"] = "protocolo_cartorio";	
                $camposRelatorio[31]["fieldname"] = "data_protocolo";	
                $camposRelatorio[32]["fieldname"] = "data_ocorrencia";	
                $camposRelatorio[33]["fieldname"] = "codigo_ocorrencia";	
                $camposRelatorio[34]["fieldname"] = "ocorrencia";	
                $camposRelatorio[35]["fieldname"] = "codigo_irregular";	
                $camposRelatorio[36]["fieldname"] = "taxa";	
                $camposRelatorio[37]["fieldname"] = "tp_doc_devedor_02";	
                $camposRelatorio[38]["fieldname"] = "doc_devedor_02";	
                $camposRelatorio[39]["fieldname"] = "nome_devedor _02";	
                $camposRelatorio[40]["fieldname"] = "cep_devedor_02";	
                $camposRelatorio[41]["fieldname"] = "endereco_devedor_02";	
                $camposRelatorio[42]["fieldname"] = "numero_end_devedor_02";	
                $camposRelatorio[43]["fieldname"] = "complem_end_devedor_02";	
                $camposRelatorio[44]["fieldname"] = "bairro_devedor_02";	
                $camposRelatorio[45]["fieldname"] = "cidade_ibge_devedor_02";	
                $camposRelatorio[46]["fieldname"] = "uf_devedor_02";	
                $camposRelatorio[47]["fieldname"] = "tp_doc_devedor_03";	
                $camposRelatorio[48]["fieldname"] = "doc_devedor_03";	
                $camposRelatorio[49]["fieldname"] = "nome_devedor_03";	
                $camposRelatorio[50]["fieldname"] = "cep_devedor_03";	
                $camposRelatorio[51]["fieldname"] = "endereco_devedor_03";	
                $camposRelatorio[52]["fieldname"] = "numero_end_devedor_03";	
                $camposRelatorio[53]["fieldname"] = "complem_end_devedor_03";	
                $camposRelatorio[54]["fieldname"] = "bairro_devedor_03";	
                $camposRelatorio[55]["fieldname"] = "cidade_ibge_devedor_03";	
                $camposRelatorio[56]["fieldname"] = "uf_devedor_03";	
                $camposRelatorio[57]["fieldname"] = "tp_doc_devedor_04";	
                $camposRelatorio[58]["fieldname"] = "doc_devedor_04";	
                $camposRelatorio[59]["fieldname"] = "nome_devedor_04";	
                $camposRelatorio[60]["fieldname"] = "cep_devedor_04";	
                $camposRelatorio[61]["fieldname"] = "endereco_devedor_04";	
                $camposRelatorio[62]["fieldname"] = "numero_end_devedor_04";	
                $camposRelatorio[63]["fieldname"] = "complem_end_devedor_04";	                
                $camposRelatorio[64]["fieldname"] = "bairro_devedor_04";	
                $camposRelatorio[65]["fieldname"] = "cidade_ibge_devedor_04";	
                $camposRelatorio[66]["fieldname"] = "uf_devedor_04";	
                $camposRelatorio[67]["fieldname"] = "tp_doc_devedor_05";	
                $camposRelatorio[68]["fieldname"] = "doc_devedor_05";	
                $camposRelatorio[69]["fieldname"] = "nome_devedor_05";	
                $camposRelatorio[70]["fieldname"] = "cep_devedor_05";	
                $camposRelatorio[71]["fieldname"] = "endereco_devedor_05";	
                $camposRelatorio[72]["fieldname"] = "numero_end_devedor_05";	
                $camposRelatorio[73]["fieldname"] = "complem_end_devedor_05";	
                $camposRelatorio[74]["fieldname"] = "bairro_devedor_05";	
                $camposRelatorio[75]["fieldname"] = "cidade_ibge_devedor_05";	
                $camposRelatorio[76]["fieldname"] = "uf_devedor_05";	
                $camposRelatorio[77]["fieldname"] = "tp_doc_devedor_06";	
                $camposRelatorio[78]["fieldname"] = "doc_devedor_06";	
                $camposRelatorio[79]["fieldname"] = "nome_devedor_06";	
                $camposRelatorio[80]["fieldname"] = "cep_devedor_06";	
                $camposRelatorio[81]["fieldname"] = "endereco_devedor_06";	
                $camposRelatorio[82]["fieldname"] = "numero_end_devedor_06";	
                $camposRelatorio[83]["fieldname"] = "complem_end_devedor_06";	
                $camposRelatorio[84]["fieldname"] = "bairro_devedor_06";	
                $camposRelatorio[85]["fieldname"] = "cidade_ibge_devedor_06";	
                $camposRelatorio[86]["fieldname"] = "uf_devedor_06";	
                $camposRelatorio[87]["fieldname"] = "tp_doc_devedor_07";	
                $camposRelatorio[88]["fieldname"] = "doc_devedor_07";	
                $camposRelatorio[89]["fieldname"] = "nome_devedor_07";	
                $camposRelatorio[90]["fieldname"] = "cep_devedor_07";	
                $camposRelatorio[91]["fieldname"] = "endereco_devedor_07";	
                $camposRelatorio[92]["fieldname"] = "numero_end_devedor_07";	
                $camposRelatorio[93]["fieldname"] = "complem_end_devedor_07";	
                $camposRelatorio[94]["fieldname"] = "bairro_devedor_07";	
                $camposRelatorio[95]["fieldname"] = "cidade_ibge_devedor_07";	
                $camposRelatorio[96]["fieldname"] = "uf_devedor_07";	
                $camposRelatorio[97]["fieldname"] = "tp_doc_devedor_08";	
                $camposRelatorio[98]["fieldname"] = "doc_devedor_08";	
                $camposRelatorio[99]["fieldname"] = "nome_devedor_08";	
                $camposRelatorio[100]["fieldname"] = "cep_devedor_08";	
                $camposRelatorio[101]["fieldname"] = "endereco_devedor_08";	
                $camposRelatorio[102]["fieldname"] = "numero_end_devedor_08";	
                $camposRelatorio[103]["fieldname"] = "complem_end_devedor_08";	
                $camposRelatorio[104]["fieldname"] = "bairro_devedor_08";	
                $camposRelatorio[105]["fieldname"] = "cidade_ibge_devedor_08";	
                $camposRelatorio[106]["fieldname"] = "uf_devedor_08";	
                $camposRelatorio[107]["fieldname"] = "tp_doc_devedor_09";	
                $camposRelatorio[108]["fieldname"] = "doc_devedor_09";	
                $camposRelatorio[109]["fieldname"] = "nome_devedor_09";	
                $camposRelatorio[110]["fieldname"] = "cep_devedor_09";	
                $camposRelatorio[111]["fieldname"] = "endereco_devedor_09";	
                $camposRelatorio[112]["fieldname"] = "numero_end_devedor_09";	
                $camposRelatorio[113]["fieldname"] = "complem_end_devedor_09";	
                $camposRelatorio[114]["fieldname"] = "bairro_devedor_09";	
                $camposRelatorio[115]["fieldname"] = "cidade_ibge_devedor_09";	
                $camposRelatorio[116]["fieldname"] = "uf_devedor_09";
//</editor-fold>
                
            // <editor-fold defaultstate="collapsed" desc="fieldcode">   
            //$camposBD[0]["fieldname"] = "NOSSO_NUMERO";	$camposBD[1]["fieldname"] = "TIPO_PROTESTO";	$camposBD[2]["fieldname"] = "ESPECIE_TITULO";	$camposBD[3]["fieldname"] = "NUM_TITULO";	$camposBD[4]["fieldname"] = "DATA_EMISSAO";	$camposBD[5]["fieldname"] = "DATA_VENCIMENTO";	$camposBD[6]["fieldname"] = "VALOR_TITULO";	$camposBD[7]["fieldname"] = "VALOR_PROTESTADO";	$camposBD[8]["fieldname"] = "PRACA_PROTESTO_IBGE";	$camposBD[9]["fieldname"] = "TIPO_DIVIDA_CDA";	$camposBD[10]["fieldname"] = "IDENTIFICACAO_DIVIDA_CDA";	$camposBD[11]["fieldname"] = "CCB_EMITIDA_POR_CBI";	$camposBD[12]["fieldname"] = "TP_DOC_CREDOR";	$camposBD[13]["fieldname"] = "DOC_CREDOR";	$camposBD[14]["fieldname"] = "NOME_CREDOR";	$camposBD[15]["fieldname"] = "TP_DOC_CEDENTE";	$camposBD[16]["fieldname"] = "DOC_CEDENTE";	$camposBD[17]["fieldname"] = "NOME_CEDENTE";	$camposBD[18]["fieldname"] = "TP_DOC_DEVEDOR_0";	$camposBD[19]["fieldname"] = "DOC_DEVEDOR_0";	$camposBD[20]["fieldname"] = "NOME_DEVEDOR_0";	$camposBD[21]["fieldname"] = "CEP_DEVEDOR_0";	$camposBD[22]["fieldname"] = "ENDERECO_DEVEDOR_0";	$camposBD[23]["fieldname"] = "NUMERO_ENDERECO_DEVEDOR_0";	$camposBD[24]["fieldname"] = "COMPLEM_END_DEVEDOR_0";	$camposBD[25]["fieldname"] = "BAIRRO_DEVEDOR_0";	$camposBD[26]["fieldname"] = " CIDADE_IBGE_0";	$camposBD[27]["fieldname"] = "UF_DEVEDOR_0";	$camposBD[28]["fieldname"] = "";	$camposBD[29]["fieldname"] = "";	$camposBD[30]["fieldname"] = "";	$camposBD[31]["fieldname"] = "";	$camposBD[32]["fieldname"] = "";	$camposBD[33]["fieldname"] = "";	$camposBD[34]["fieldname"] = "";	$camposBD[35]["fieldname"] = "";	$camposBD[36]["fieldname"] = "";	$camposBD[37]["fieldname"] = "TP_DOC_DEVEDOR_1";	$camposBD[38]["fieldname"] = "DOC_DEVEDOR_1";	$camposBD[39]["fieldname"] = "NOME_DEVEDOR_1";	$camposBD[40]["fieldname"] = "CEP_DEVEDOR_1";	$camposBD[41]["fieldname"] = "ENDERECO_DEVEDOR_1";	$camposBD[42]["fieldname"] = "NUMERO_ENDERECO_DEVEDOR_1";	$camposBD[43]["fieldname"] = "COMPLEM_END_DEVEDOR_1";	$camposBD[44]["fieldname"] = "BAIRRO_DEVEDOR_1";	$camposBD[45]["fieldname"] = " CIDADE_IBGE_1";	$camposBD[46]["fieldname"] = "UF_DEVEDOR_1";	$camposBD[47]["fieldname"] = "TP_DOC_DEVEDOR_2";	$camposBD[48]["fieldname"] = "DOC_DEVEDOR_2";	$camposBD[49]["fieldname"] = "NOME_DEVEDOR_2";	$camposBD[50]["fieldname"] = "CEP_DEVEDOR_2";	$camposBD[51]["fieldname"] = "ENDERECO_DEVEDOR_2";	$camposBD[52]["fieldname"] = "NUMERO_ENDERECO_DEVEDOR_2";	$camposBD[53]["fieldname"] = "COMPLEM_END_DEVEDOR_2";	$camposBD[54]["fieldname"] = "BAIRRO_DEVEDOR_2";	$camposBD[55]["fieldname"] = " CIDADE_IBGE_2";	$camposBD[56]["fieldname"] = "UF_DEVEDOR_2";	$camposBD[57]["fieldname"] = "TP_DOC_DEVEDOR_3";	$camposBD[58]["fieldname"] = "DOC_DEVEDOR_3";	$camposBD[59]["fieldname"] = "NOME_DEVEDOR_3";	$camposBD[60]["fieldname"] = "CEP_DEVEDOR_3";	$camposBD[61]["fieldname"] = "ENDERECO_DEVEDOR_3";	$camposBD[62]["fieldname"] = "NUMERO_ENDERECO_DEVEDOR_3";	$camposBD[63]["fieldname"] = "COMPLEM_END_DEVEDOR_3";	$camposBD[64]["fieldname"] = "BAIRRO_DEVEDOR_3";	$camposBD[65]["fieldname"] = " CIDADE_IBGE_3";	$camposBD[66]["fieldname"] = "UF_DEVEDOR_3";	$camposBD[67]["fieldname"] = "TP_DOC_DEVEDOR_4";	$camposBD[68]["fieldname"] = "DOC_DEVEDOR_4";	$camposBD[69]["fieldname"] = "NOME_DEVEDOR_4";	$camposBD[70]["fieldname"] = "CEP_DEVEDOR_4";	$camposBD[71]["fieldname"] = "ENDERECO_DEVEDOR_4";	$camposBD[72]["fieldname"] = "NUMERO_ENDERECO_DEVEDOR_4";	$camposBD[73]["fieldname"] = "COMPLEM_END_DEVEDOR_4";	$camposBD[74]["fieldname"] = "BAIRRO_DEVEDOR_4";	$camposBD[75]["fieldname"] = " CIDADE_IBGE_4";	$camposBD[76]["fieldname"] = "UF_DEVEDOR_4";	$camposBD[77]["fieldname"] = "TP_DOC_DEVEDOR_5";	$camposBD[78]["fieldname"] = "DOC_DEVEDOR_5";	$camposBD[79]["fieldname"] = "NOME_DEVEDOR_5";	$camposBD[80]["fieldname"] = "CEP_DEVEDOR_5";	$camposBD[81]["fieldname"] = "ENDERECO_DEVEDOR_5";	$camposBD[82]["fieldname"] = "NUMERO_ENDERECO_DEVEDOR_5";	$camposBD[83]["fieldname"] = "COMPLEM_END_DEVEDOR_5";	$camposBD[84]["fieldname"] = "BAIRRO_DEVEDOR_5";	$camposBD[85]["fieldname"] = " CIDADE_IBGE_5";	$camposBD[86]["fieldname"] = "UF_DEVEDOR_5";	$camposBD[87]["fieldname"] = "TP_DOC_DEVEDOR_6";	$camposBD[88]["fieldname"] = "DOC_DEVEDOR_6";	$camposBD[89]["fieldname"] = "NOME_DEVEDOR_6";	$camposBD[90]["fieldname"] = "CEP_DEVEDOR_6";	$camposBD[91]["fieldname"] = "ENDERECO_DEVEDOR_6";	$camposBD[92]["fieldname"] = "NUMERO_ENDERECO_DEVEDOR_6";	$camposBD[93]["fieldname"] = "COMPLEM_END_DEVEDOR_6";	$camposBD[94]["fieldname"] = "BAIRRO_DEVEDOR_6";	$camposBD[95]["fieldname"] = " CIDADE_IBGE_6";	$camposBD[96]["fieldname"] = "UF_DEVEDOR_6";	$camposBD[97]["fieldname"] = "TP_DOC_DEVEDOR_7";	$camposBD[98]["fieldname"] = "DOC_DEVEDOR_7";	$camposBD[99]["fieldname"] = "NOME_DEVEDOR_7";	$camposBD[100]["fieldname"] = "CEP_DEVEDOR_7";	$camposBD[101]["fieldname"] = "ENDERECO_DEVEDOR_7";	$camposBD[102]["fieldname"] = "NUMERO_ENDERECO_DEVEDOR_7";	$camposBD[103]["fieldname"] = "COMPLEM_END_DEVEDOR_7";	$camposBD[104]["fieldname"] = "BAIRRO_DEVEDOR_7";	$camposBD[105]["fieldname"] = " CIDADE_IBGE_7";	$camposBD[106]["fieldname"] = "UF_DEVEDOR_7";	$camposBD[107]["fieldname"] = "TP_DOC_DEVEDOR_8";	$camposBD[108]["fieldname"] = "DOC_DEVEDOR_8";	$camposBD[109]["fieldname"] = "NOME_DEVEDOR_8";	$camposBD[110]["fieldname"] = "CEP_DEVEDOR_8";	$camposBD[111]["fieldname"] = "ENDERECO_DEVEDOR_8";	$camposBD[112]["fieldname"] = "NUMERO_ENDERECO_DEVEDOR_8";	$camposBD[113]["fieldname"] = "COMPLEM_END_DEVEDOR_8";	$camposBD[114]["fieldname"] = "BAIRRO_DEVEDOR_8";	$camposBD[115]["fieldname"] = " CIDADE_IBGE_8";	$camposBD[116]["fieldname"] = "UF_DEVEDOR_8";
            $camposBD[0] = "NOSSO_NUMERO";	$camposBD[1]= "TIPO_PROTESTO";	$camposBD[2] = "ESPECIE_TITULO";	$camposBD[3] = "NUM_TITULO";	$camposBD[4] = "DATA_EMISSAO";	$camposBD[5] = "DATA_VENCIMENTO";	$camposBD[6] = "VALOR_TITULO";	$camposBD[7] = "VALOR_PROTESTADO";	$camposBD[8] = "PRACA_PROTESTO_IBGE";	$camposBD[9] = "TIPO_DIVIDA_CDA";	$camposBD[10] = "IDENTIFICACAO_DIVIDA_CDA";	$camposBD[11] = "CCB_EMITIDA_POR_CBI";	$camposBD[12] = "TP_DOC_CREDOR";	$camposBD[13] = "DOC_CREDOR";	$camposBD[14] = "NOME_CREDOR";	$camposBD[15] = "TP_DOC_CEDENTE";	$camposBD[16] = "DOC_CEDENTE";	$camposBD[17] = "NOME_CEDENTE";	$camposBD[18] = "TP_DOC_DEVEDOR_0";	$camposBD[19] = "DOC_DEVEDOR_0";	$camposBD[20] = "NOME_DEVEDOR_0";	$camposBD[21] = "CEP_DEVEDOR_0";	$camposBD[22] = "ENDERECO_DEVEDOR_0";	$camposBD[23] = "NUMERO_ENDERECO_DEVEDOR_0";	$camposBD[24] = "COMPLEM_END_DEVEDOR_0";	$camposBD[25] = "BAIRRO_DEVEDOR_0";	$camposBD[26] = " CIDADE_IBGE_0";	$camposBD[27] = "UF_DEVEDOR_0";	$camposBD[28] = "";	$camposBD[29] = "";	$camposBD[30] = "";	$camposBD[31] = "";	$camposBD[32] = "";	$camposBD[33] = "";	$camposBD[34] = "";	$camposBD[35] = "";	$camposBD[36] = "";	$camposBD[37] = "TP_DOC_DEVEDOR_1";	$camposBD[38] = "DOC_DEVEDOR_1";	$camposBD[39] = "NOME_DEVEDOR_1";	$camposBD[40] = "CEP_DEVEDOR_1";	$camposBD[41] = "ENDERECO_DEVEDOR_1";	$camposBD[42] = "NUMERO_ENDERECO_DEVEDOR_1";	$camposBD[43] = "COMPLEM_END_DEVEDOR_1";	$camposBD[44] = "BAIRRO_DEVEDOR_1";	$camposBD[45] = " CIDADE_IBGE_1";	$camposBD[46] = "UF_DEVEDOR_1";	$camposBD[47] = "TP_DOC_DEVEDOR_2";	$camposBD[48] = "DOC_DEVEDOR_2";	$camposBD[49] = "NOME_DEVEDOR_2";	$camposBD[50] = "CEP_DEVEDOR_2";	$camposBD[51] = "ENDERECO_DEVEDOR_2";	$camposBD[52] = "NUMERO_ENDERECO_DEVEDOR_2";	$camposBD[53] = "COMPLEM_END_DEVEDOR_2";	$camposBD[54] = "BAIRRO_DEVEDOR_2";	$camposBD[55] = " CIDADE_IBGE_2";	$camposBD[56] = "UF_DEVEDOR_2";	$camposBD[57] = "TP_DOC_DEVEDOR_3";	$camposBD[58] = "DOC_DEVEDOR_3";	$camposBD[59] = "NOME_DEVEDOR_3";	$camposBD[60] = "CEP_DEVEDOR_3";	$camposBD[61] = "ENDERECO_DEVEDOR_3";	$camposBD[62] = "NUMERO_ENDERECO_DEVEDOR_3";	$camposBD[63] = "COMPLEM_END_DEVEDOR_3";	$camposBD[64] = "BAIRRO_DEVEDOR_3";	$camposBD[65] = " CIDADE_IBGE_3";	$camposBD[66] = "UF_DEVEDOR_3";	$camposBD[67] = "TP_DOC_DEVEDOR_4";	$camposBD[68] = "DOC_DEVEDOR_4";	$camposBD[69] = "NOME_DEVEDOR_4";	$camposBD[70] = "CEP_DEVEDOR_4";	$camposBD[71] = "ENDERECO_DEVEDOR_4";	$camposBD[72] = "NUMERO_ENDERECO_DEVEDOR_4";	$camposBD[73] = "COMPLEM_END_DEVEDOR_4";	$camposBD[74] = "BAIRRO_DEVEDOR_4";	$camposBD[75] = " CIDADE_IBGE_4";	$camposBD[76] = "UF_DEVEDOR_4";	$camposBD[77] = "TP_DOC_DEVEDOR_5";	$camposBD[78] = "DOC_DEVEDOR_5";	$camposBD[79] = "NOME_DEVEDOR_5";	$camposBD[80] = "CEP_DEVEDOR_5";	$camposBD[81] = "ENDERECO_DEVEDOR_5";	$camposBD[82] = "NUMERO_ENDERECO_DEVEDOR_5";	$camposBD[83] = "COMPLEM_END_DEVEDOR_5";	$camposBD[84] = "BAIRRO_DEVEDOR_5";	$camposBD[85] = " CIDADE_IBGE_5";	$camposBD[86] = "UF_DEVEDOR_5";	$camposBD[87] = "TP_DOC_DEVEDOR_6";	$camposBD[88] = "DOC_DEVEDOR_6";	$camposBD[89] = "NOME_DEVEDOR_6";	$camposBD[90] = "CEP_DEVEDOR_6";	$camposBD[91] = "ENDERECO_DEVEDOR_6";	$camposBD[92] = "NUMERO_ENDERECO_DEVEDOR_6";	$camposBD[93] = "COMPLEM_END_DEVEDOR_6";	$camposBD[94] = "BAIRRO_DEVEDOR_6";	$camposBD[95] = " CIDADE_IBGE_6";	$camposBD[96] = "UF_DEVEDOR_6";	$camposBD[97] = "TP_DOC_DEVEDOR_7";	$camposBD[98] = "DOC_DEVEDOR_7";	$camposBD[99] = "NOME_DEVEDOR_7";	$camposBD[100] = "CEP_DEVEDOR_7";	$camposBD[101] = "ENDERECO_DEVEDOR_7";	$camposBD[102] = "NUMERO_ENDERECO_DEVEDOR_7";	$camposBD[103] = "COMPLEM_END_DEVEDOR_7";	$camposBD[104] = "BAIRRO_DEVEDOR_7";	$camposBD[105] = " CIDADE_IBGE_7";	$camposBD[106] = "UF_DEVEDOR_7";	$camposBD[107] = "TP_DOC_DEVEDOR_8";	$camposBD[108] = "DOC_DEVEDOR_8";	$camposBD[109] = "NOME_DEVEDOR_8";	$camposBD[110] = "CEP_DEVEDOR_8";	$camposBD[111] = "ENDERECO_DEVEDOR_8";	$camposBD[112] = "NUMERO_ENDERECO_DEVEDOR_8";	$camposBD[113] = "COMPLEM_END_DEVEDOR_8";	$camposBD[114] = "BAIRRO_DEVEDOR_8";	$camposBD[115] = " CIDADE_IBGE_8";	$camposBD[116] = "UF_DEVEDOR_8";
            $campoEstado = "UF_DEVEDOR_0";
                //</editor-fold>       
                
            $TODAY = date("dmy");
            $nomerelatorio = "X".$TODAY."1";
            break;
        case "SE": 
            break;
        case "TO": 
            break;
       default:
           // $nomerelatorio = "relatorio_protesto";
           break;
    }
    
    
// </editor-fold>
    

// <editor-fold defaultstate="collapsed" desc="Seleção dados do relatório">
       
            $filtros_where[0]["campo"]= $campoEstado;
            $filtros_where[0]["valor"]=$UF;

            
            $camposSelecionados[] = array();  
            $camposSelecionados = $camposBD;
            $Query = pegaListaCasosNaFila($procId, $stepId, 1, '' , '', '', '', 1, true);
            $retorno = mysqli_fetch_all($Query, MYSQLI_ASSOC);

            foreach ($retorno as $resultado){

                $caso[] = array();   
                $caseNum = $resultado["CaseId"];

                $caso = pegaDadosCaso($procId, $caseNum, $camposSelecionados);
                if(strtoupper((string)$caso[$campoEstado]) == $UF){
                    foreach ($camposSelecionados as $campo)
                    {
                        $t_body->COLSPAN = 1;
                        $t_body->CAMPO_VALOR = $caso[$campo];
                        $t_body->block("BLOCO_CAMPO");
                    }
                     $t_body->block("BLOCO_LINHA");
                      
                }
  
            }         

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Monta Cabecalho do relatorio">

    for ($i = 0; $i < count($camposRelatorio); $i++) {
        $t_body->CAMPO_NOME = $camposRelatorio[$i]["fieldname"];
        $t_body->block("BLOCO_CABECALHO");
    }

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Geração Relatorio">
//
    

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Envio dados BROWSER">
    $dadosArquivo = $t_body->parse();


    EnviaArquivoBrowser("$nomerelatorio.xls", "$STEPNAME", $dadosArquivo, 'excel');

    // </editor-fold>
}



