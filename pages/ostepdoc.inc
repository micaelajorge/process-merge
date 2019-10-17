<?php

// //Versao 1.0.0 /Versao
// CONTENTCHANGECHILDSINDEX
// CONTENTMODIFYOBJECT
// CONTENTCREATEOBJECT
// UNFILEOBJECT
// FILEOBJECT
// ContentFileUnfileObjectContent
// GETCASEIDBYFIELDVALUE
// GETFIELDVALUE
// CONTENTOBJECTCHECKOUTRECEIVED
// CONTENTOBJECTCHECKIN
// DATE_DIFF
// GETUSERID
// 
if (!class_exists("STEPDOC")) {

    require_once(FILES_ROOT . "pages/odocflow.inc");
    require_once(FILES_ROOT . "pages/ostepfieldsdoc.inc");
    require_once(FILES_ROOT . "include/sendmail.php");
    require_once(FILES_ROOT . "include/inctime.php");
    require_once(FILES_ROOT . "include/deadtimetelemig.php");
    require_once(FILES_ROOT . "include/customwebservices.inc");

    class STEPDOC extends DOCFLOW {

        var $StepId;
        var $StepName;
        var $StepDesc;
        var $Date;
        var $SDate;
        var $DeadSoft;
        var $DeadHard;
        var $DeadHardest;
        var $DefaultView;
        var $Action;
        var $LockedBy;
        var $DeadSoftDate;
        var $DeadHardDate;
        var $DeadHardestDate;
        var $StartStepId;
        var $InsertDate;
        var $NovoCaso;
        var $NomeTabela;
        var $ConditionName;
        var $ConditionId;

        // <editor-fold defaultstate="collapsed" desc="Contar páginas em campo FD">
        function COUNT_PAGES($fieldCode)
        {
            $totalPaginas = 0;
            $jDadosArquivo = $this->FieldValuesByCode[$fieldCode]["Value"];
            $idDocumentField = $this->FieldCodes[$fieldCode];

            $listaArquivos = array();
            if (!is_array($jDadosArquivo)) {
                $listaArquivos[] = $jDadosArquivo;
            } else {
                $listaArquivos = $jDadosArquivo;
            }
            $dirName = criaDiretorioCampoArquivo($this->ProcId, $this->CaseNum, $idDocumentField);


            foreach ($listaArquivos as $dadosArquivo) {
                try {
                    $dadoArquivo = json_decode($dadosArquivo, true);

                    if (json_last_error() !== 0)
                    {
                        continue;
                    }
                    
                    // Extrai a extenção do arquivo
                    $extencao = pathinfo($dadoArquivo["fileName"], PATHINFO_EXTENSION);

                    // Conta paginas se for PDF
                    if ($extencao == "pdf") {
                        $nomeArquivo = $dirName . '/' . $dadoArquivo["fileNameStorage"];
                        $totalPaginas = $totalPaginas + pdfCountPages($nomeArquivo);
                    } else {
                        $totalPaginas = $totalPaginas + 1;
                    }
                } catch (Exception $ex) {
                    $totalPaginas = -99;
                }
            }
            return $totalPaginas;
        }

        // </editor-fold>
        // <editor-fold defaultstate="collapsed" desc="Criar Documento ClickSign">
        function CLICKSIGN_CRIADOCUMENTO($documentField, $deadTimeAt = "", $autoClose = "")
        {
            /**
             *  Trata dados do Arquivo
             */
            $idDocumentField = $this->FieldCodes[$documentField];
            $jDadosArquivo = $this->FieldValuesByCode[$documentField]["Value"];
            $dadosArquivo = json_decode($jDadosArquivo, true);
            $nomeArquivoStorage = $dadosArquivo["fileNameStorage"];
            $nomeArquivo = $dadosArquivo["fileName"];

            $path_parts = pathinfo($nomeArquivo);

            // <editor-fold defaultstate="collapsed" desc="Determina a Extenção do Arquivo">
            $ExtencaoDoArquivo = $path_parts['extension'];
            switch ($ExtencaoDoArquivo) {
                case "pdf":
                    $mineType = 'application/pdf';
                    break;
                case "doc":
                case "docx":
                    $mineType = 'application/msword';
                    break;
            }
            // </editor-fold>  

            $dirName = FILES_UPLOAD . "/" . formataCaseNum($this->ProcId, 6) . "/" . formataCaseNum($this->CaseNum, 10) . "/" . formataCaseNum($idDocumentField, 6) . "/";

            $handle = fopen($dirName . $nomeArquivoStorage, "r");
            $dadosBase64 = fread($handle, filesize($dirName . $nomeArquivoStorage));

            $urlEnvioCrua = "!{url_clicksign}!/api/v1/documents?access_token=!{key_clicksign}!";

            $urlEnvio = $this->ParseValuesFields($urlEnvioCrua);

            $POST_DATA = array(
                'document' => array(
                    "content_base64" => "data:$mineType;base64," . base64_encode($dadosBase64),
                    'path' => "/" . $nomeArquivo
                )
            );
            $jPostData = json_encode($POST_DATA);

            $curl = curl_init();
            curl_setopt($curl, CURLOPT_URL, $urlEnvio);
            curl_setopt($curl, CURLOPT_TIMEOUT, 30);
            curl_setopt($curl, CURLOPT_POST, 1);
            curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
            curl_setopt($curl, CURLOPT_POSTFIELDS, $jPostData);
            curl_setopt($curl, CURLOPT_HTTPHEADER, array(
                'Content-Type: application/json',
                'Content-Length: ' . strlen($jPostData))
            );
            $respostaChamada = curl_exec($curl);

            file_put_contents("dados_envio.txt", $jPostData);

            $lastErrorCode = curl_getinfo($curl, CURLINFO_HTTP_CODE);

            if ($lastErrorCode != 201) {
                $textoDescritivo = "Falha Criar Documento\nErro: $lastErrorCode " . "\nResposta: $respostaChamada" . "\nURL: $urlEnvio";
                $this->Atualizaaudittrail(0, 0, $textoDescritivo, $this->ConditionName);
                $resposta = "erro";
            } else {
                $dadosResposta = json_decode($respostaChamada, true);
                $resposta = $dadosResposta["document"]["key"];

                $textoDescritivo = "Documento Criado\nKey: " . $resposta;
                $this->Atualizaaudittrail(0, 0, $textoDescritivo, $this->ConditionName);
            }
            curl_close($curl);
            return $resposta;
        }

        // </editor-fold>
        // <editor-fold defaultstate="collapsed" desc="Criar Signatario">
        function CLICKSIGN_SIGNER_CREATE($signerName, $signerEmail, $signerBirthday, $signerCpf, $signerFone, $campoRetorno)
        {
            /**
             *  Trata dados do Arquivo
             */
            $urlEnvioCrua = "!{url_clicksign}!/api/v1/signers?access_token=!{key_clicksign}!";
            $urlEnvio = $this->ParseValuesFields($urlEnvioCrua);

            $POST_DATA = array();
            $POST_DATA["signer"] = array();
            $POST_DATA["signer"]["email"] = $this->FieldValuesByCode[$signerEmail]["Value"];
            $POST_DATA["signer"]["phone_number"] = $this->FieldValuesByCode[$signerFone]["Value"];
            $POST_DATA["signer"]["birthday"] = $this->FieldValuesByCode[$signerBirthday]["Value"];
            $POST_DATA["signer"]["name"] = $this->FieldValuesByCode[$signerName]["Value"];
            $POST_DATA["signer"]["documentation"] = $this->FieldValuesByCode[$signerCpf]["Value"];
            $POST_DATA["signer"]["has_documentation"] = "true";
            $POST_DATA["signer"]["auths"] = array("email");
            $POST_DATA["signer"]["delivery"] = "none";

            if (is_array($POST_DATA)) {
                $jPostData = json_encode($POST_DATA);

                $curl = curl_init();
                curl_setopt($curl, CURLOPT_URL, $urlEnvio);
                curl_setopt($curl, CURLOPT_TIMEOUT, 30);
                curl_setopt($curl, CURLOPT_POST, 1);
                curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
                curl_setopt($curl, CURLOPT_POSTFIELDS, $jPostData);
                curl_setopt($curl, CURLOPT_HTTPHEADER, array(
                    'Content-Type: application/json',
                    'Content-Length: ' . strlen($jPostData))
                );
                $jResposta = curl_exec($curl);
                $lastErrorCode = curl_getinfo($curl, CURLINFO_HTTP_CODE);
            } else {
                $erro_json = json_last_error_msg();
                $lastErrorCode = 500;
                $jResposta = "Falha Criação de Parâmetros\nDados Envio:$erro_json";
            }


            if ($lastErrorCode != 201) {
                $textoDescritivo = "Falha Criar Signatario\nErro: $lastErrorCode " . "\nResposta: $jResposta" . "\nURL: $urlEnvio\nDados Envio: $jPostData";
                $this->Atualizaaudittrail(0, 0, $textoDescritivo, $this->ConditionName);
                $this->Fields->Field[$idCampoRetorno]->fieldValueDB = "";
            } else {
                /**
                 *  Extrai o valor da Chave do Usuário
                 */
                $resposta = json_decode($jResposta, true);
                $key_usuario = $resposta["signer"]["key"];

                $idCampoRetorno = $this->FieldCodes[$campoRetorno];

                $textoDescritivo = "Documento Criado - " . $jResposta;
                $this->Atualizaaudittrail(0, 0, $textoDescritivo, $this->ConditionName);

                // Salva os dados do Campo
                $this->Fields->SetFieldValue($this->connect, $idCampoRetorno, $key_usuario);
                $this->Fields->AtualizaCampos($this->connect, $this->ProcId, $this->CaseNum);
            }
            curl_close($curl);
            return $jResposta;
        }

        // </editor-fold>
        // <editor-fold defaultstate="collapsed" desc="Adicionar Signatario ao Documento">
        function CLICKSIGN_SIGNER_ADD($fieldCodedocumentKey, $signerKey, $signerList)
        {
            $urlEnvioCrua = "!{url_clicksign}!/api/v1/lists?access_token=!{key_ckicksign}!";
            $urlEnvio = $this->ParseValuesFields($urlEnvioCrua);

            $jSignerList = $this->FieldValuesByCode[$signerList]["Value"];

            $signerList = json_decode($jSignerList);

            $responseList = array();

            $jPostDataOriginal = ' { "list": { "document_key": "!%' . $fieldCodedocumentKey . '!%", "signer_key": "!%' . $signerKey . '!%", "sign_as": "%SignerAs%"   }}';
            foreach ($signerList as $signerAs) {

                // Cria Json de envio

                $jPostData = $this->ParseValuesFields($jPostDataOriginal);
                $jPostData = str_replace("%SignerAs%", $signerAs, $jPostData);

                $curl = curl_init();
                curl_setopt($curl, CURLOPT_URL, $urlEnvio);
                curl_setopt($curl, CURLOPT_TIMEOUT, 30);
                curl_setopt($curl, CURLOPT_POST, 1);
                curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
                curl_setopt($curl, CURLOPT_POSTFIELDS, $jPostData);
                curl_setopt($curl, CURLOPT_HTTPHEADER, array(
                    'Content-Type: application/json',
                    'Content-Length: ' . strlen($jPostData))
                );

                $resposta = curl_exec($curl);
                $responseList[] = $resposta;
                $lastErrorCode = curl_getinfo($curl, CURLINFO_HTTP_CODE);

                if ($lastErrorCode != 201) {
                    $textoDescritivo = "Falha Criar Signatario\nErro: $lastErrorCode " . "\nResposta: $resposta" . "\nDados Envio: $jPostData";
                    $this->Atualizaaudittrail(0, 0, $textoDescritivo, $this->ConditionName);
                } else {
                    $
                            $textoDescritivo = "Signatario Criado - " . $resposta;
                    $this->Atualizaaudittrail(0, 0, $textoDescritivo, $this->ConditionName);
                }
                curl_close($curl);
            }

            $resposta = json_encode($responseList);
            return $resposta;
        }

        // </editor-fold>
        // <editor-fold defaultstate="collapsed" desc="Pega ProcId By Code">
        function PegaProcIdByCode($ProcCode)
        {
            if (is_numeric($ProcCode)) {
                return $ProcCode;
            }
            $ProcIdSearch = trim($ProcCode);
            $SQL = "select ProcId from procdef where ProcCod = '$ProcIdSearch' and Active = 1";
            $Query = mysqli_query($this->connect, $SQL);
            $Result = mysqli_fetch_array($Query);
            $ProcIdFound = $Result["ProcId"];
            if (!is_numeric($ProcIdFound)) {
                $ProcIdFound = 0;
            }
            return $ProcIdFound;
        }

        // </editor-fold>

        function PegaStepIdByCode($ProcId, $StepCode)
        {
            if (is_numeric($StepCode)) {
                return $StepCode;
            }
            if (!is_numeric($ProcId)) {
                $ProcId = $this->PegaProcIdByCode($ProcId);
            }
            if ($ProcId == 0) {
                return 0;
            }

            $StepCodeSearch = trim($StepCode);
            $SQL = "select StepId from stepdef where ProcId = $ProcId and StepCod = '$StepCodeSearch' and Active = 1";
            $Query = mysqli_query($this->connect, $SQL);
            $Result = mysqli_fetch_array($Query);
            $StepIdFound = $Result["StepId"];
            if (!is_numeric($StepIdFound)) {
                $StepIdFound = 0;
            }
            return $StepIdFound;
        }

        function ProcessoExiste($ProcId)
        {
            if (!is_numeric($ProcId)) {
                $ProcId = $this->PegaProcIdByCode($ProcId);
            }
            if ($ProcId == 0) {
                return false;
            }
            $SQL = "Select count(*) as NrTotal from procdef where ProcId = $ProcId";
            $Query = mysqli_query($this->connect, $SQL);
            $result = mysqli_fetch_array($Query);
            if ($result["NrTotal"] > 0) {
                return true;
            }
            return false;
        }

        function PassoExiste($ProcId, $StepId)
        {
            if (!is_numeric($ProcId)) {
                $ProcId = $this->PegaProcIdByCode($ProcId);
            }
            if ($ProcId == 0) {
                return false;
            }
            if (!is_numeric($StepId)) {
                $StepId = $this->PegaStepIdByCode($ProcId, $StepId);
            }
            if ($StepId == 0) {
                return false;
            }
            $SQL = "Select count(*) as NrTotal from stepdef where ProcId = $ProcId";
            $Query = mysqli_query($this->connect, $SQL);
            $result = mysqli_fetch_array($Query);
            if ($result["NrTotal"] > 0) {
                return true;
            }
            return false;
        }

        function SETFIELDVALUE($ProcCod, $CaseNum, $FieldId, $FieldValue)
        {
            $valores["Fields"][0]["FieldId"] = $FieldId;
            $valores["Fields"][0]["Value"] = $FieldValue;
            if (!is_numeric($ProcCod)) {
                $ProcId = PegaProcIdByCode($ProcCod);
            }
            if (CasoCasoEstaNaFila($ProcId, 0, $CaseNum)) {
                $Caso = new STEPDOC;
                $Caso->SetConnection($this->connect);
                $Caso->SetProc($ProcId);
                $Caso->SetStep(0);
                $Caso->open();
                $Caso->UserId = $this->UserId;
                $Caso->UserName = $this->UserName;
                $Caso->UserDesc = $this->UserDesc;
                $Caso->samaccountname = $this->UserName;
                $Caso->SetCaseNum($CaseNum);
                $Caso->PegaDadosDeArray($valores);
                $Caso->SetAction(6);
                $this->EventId = 1000;
                $MSG = "Alterado Campo $FieldId para ($FieldValue) do Caso $CaseNum Processo $ProcCod";
            } else {
                $MSG = "Nçã encontrado Caso $CaseNum Processo $ProcCod";
            }
            $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
        }

        function OBJECTCHECKIFVIEWED($connect, $CaseNum, $ProcId, $RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $RMOBJECTIDCODE)
        {
            require_once(FILES_ROOT . "vendor/lib/nusoap.php");
            $RMOBJECTID = $RMOBJECTIDCODE;
            $RMUSERNAME = $RMOBJECTUSERNAMECODE;
            $RMUSERPWD = $RMOBJECTUSERPWDCODE;

            $Parametros = "RMOBJECTID: $RMOBJECTID; userName: $RMUSERNAME; password: $RMUSERPWD ";
            $retorno = 0;
            if (empty($RMOBJECTID)) {
                $MSG = "-2012 Falha ao Verificar Visualização do Objeto - ObjectID inválido: valor \"$RMOBJECTID\"";
                error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                $retorno = -2012;
            }

            if (!is_numeric($RMOBJECTID)) {
                $MSG = "-2009 Falha ao Verificar Visualização do Objeto - ObjectID inválido: valor \"$RMOBJECTID\"";
                error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                $retorno = -2009;
            }

            if (empty($RMUSERNAME)) {
                $MSG = "-2007 Falha ao Verificar Visualização do Objeto - UserName inválido: valor \"$RMUSERNAME\"";
                error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                $retorno = -2007;
            }

            if (empty($RMUSERPWD)) {
                $MSG = "-2008 Falha ao Verificar Visualização do Objeto - senha inv�lida: valor \"$RMUSERPWD\"";
                error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                $retorno = -2008;
            }

            if ($retorno != 0) {
                $this->EventId = 1000;
                $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
                return $retorno;
            }


            $WSDL = $this->ParseValuesFields($this->URL_RMWS);
            $client = new soapclient($WSDL, 'wsdl');
            $client->setDefaultRpcParams(true);
            //$soap_proxy = $client->getProxy();	
            $Params["userName"] = $RMUSERNAME;
            $Params["userPWD"] = $RMUSERPWD;
            $Params["objectID"] = $RMOBJECTID;

            $result = $client->call("ObjectCheckIfViewed", array($Params));
            if (!is_null($result["faultstring"])) {
                $Dados = "";
                while (list($key, $value) = each($Params)) {
                    $Dados .= "$key - $value; ";
                }
                $retorno = $result["faultstring"];
                $MSG = "-2010 Falha ao Verificar Visualização do Objeto no Content ObjectID:$RMOBJECTID Erro:" . $retorno;
                error_log("Caso: $this->CaseNum - $MSG - Dados $Dados", 0);
                $retorno = -2010;
            } else {
                if (!$result) {
                    $Dados = "";
                    while (list($key, $value) = each($Params)) {
                        $Dados .= "$key - $value ; ";
                    }
                    $MSG = "-2011 Falha ao Verificar Visualização do Objeto no Content ObjectID:$RMOBJECTID, não houve retorno";
                    error_log("Caso: $this->CaseNum - $MSG - Dados $Dados", 0);
                    $retorno = -2011;
                } else {
                    $retorno = $result["ObjectCheckIfViewedResult"];
                    if ($retorno < 0) {
                        $Dados = "";
                        while (list($key, $value) = each($Params)) {
                            $Dados .= "$key - $value ; ";
                        }
                        if ($retorno == -5) {
                            $MSG = "-2001 Falha ao Verificar Visualização do Objeto no Content ObjectID:$RMOBJECTID, Objeto não existe, Retorno: $retorno";
                            error_log("Caso: $this->CaseNum - $MSG - Dados $Dados", 0);
                        } else {
                            $MSG = "-2000 Falha ao Verificar Visualização do Objeto no Content ObjectID:$RMOBJECTID, Retorno: $retorno";
                            error_log("Caso: $this->CaseNum - $MSG - Dados $Dados", 0);
                        }
                        $retorno = -2000;
                    } else {
                        $MSG = "Verificada Visualização do Objeto no Content ObjectID: $RMOBJECTID, Retorno: $retorno";
                    }
                }
            }
            $this->EventId = 1000;
            $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
            return $retorno;
        }

        function RMModifyObject($connect, $CaseNum, $ProcId, $FieldCodes, $RMOBJECTIDCODE, $RMOBJECTSOURCECODE, $RMOBJECTTYPECODE, $RMOBJECTCLASSCODE, $RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $Subst = true)
        {
            require_once(FILES_ROOT . "vendor/lib/nusoap.php");

            // Se for na vers�o antiga do webdesigner, substitui os codigos pelos valores
            if ($Subst) {
                $RMOBJECTID = $FieldCodes[$RMOBJECTIDCODE]["Value"];
                if (strpos($RMOBJECTID, "^")) {
                    $RMOBJECTID = substr($RMOBJECTID, strrpos($RMOBJECTID, "^") + 1);
                }
                $SOURCECODE = $FieldCodes[$RMOBJECTSOURCECODE]["Value"];
                $TYPECODE = $FieldCodes[$RMOBJECTTYPECODE]["Value"];
                $CLASSCODE = $FieldCodes[$RMOBJECTCLASSCODE]["Value"];
                $RMUSERNAME = $FieldCodes[$RMOBJECTUSERNAMECODE]["Value"];
                $RMUSERPWD = $FieldCodes[$RMOBJECTUSERPWDCODE]["Value"];
            } else {
                if (strpos($RMOBJECTID, "^")) {
                    $RMOBJECTID = substr($RMOBJECTID, strrpos($RMOBJECTID, "^") + 1);
                }
                $RMOBJECTID = $RMOBJECTIDCODE;
                $SOURCECODE = $RMOBJECTSOURCECODE;
                $TYPECODE = $RMOBJECTTYPECODE;
                $CLASSCODE = $RMOBJECTCLASSCODE;
                $RMUSERNAME = $RMOBJECTUSERNAMECODE;
                $RMUSERPWD = $RMOBJECTUSERPWDCODE;
            }

            $Parametros = "RMOBJECTID: $RMOBJECTID; SOURCECODE: $SOURCECODE; TYPECODE: $TYPECODE; CLASSCODE: $CLASSCODE ";

            $retorno = 0;
            if (empty($RMOBJECTID)) {
                $MSG = "-1003 Falha ao Modificar Objeto - ObjectID inválido: valor \"$RMOBJECTID\"";
                error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                $retorno = -1003;
            }

            if (!is_numeric($RMOBJECTID)) {
                $MSG = "-1009 Falha ao Modificar Objeto - ObjectID inválido: valor \"$RMOBJECTID\"";
                error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                $retorno = -1009;
                return $retorno;
            }

            if (empty($SOURCECODE)) {
                $MSG = "-1004 Falha ao Modificar Objeto - SOURCECODE inválido: valor \"$SOURCECODE\"";
                error_log("MSG $MSG - Dados $Parametros", 0);
                $retorno = -1004;
            }

            if (empty($TYPECODE)) {
                $MSG = "-1005 Falha ao Modificar Objeto - TYPECODE inválido: valor \"$TYPECODE\"";
                error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                $retorno = -1005;
                $TYPECODE = '';
            }

            if (empty($CLASSCODE)) {
                $MSG = "-1006 Falha ao Modificar Objeto - CLASSCODE inválido: valor \"$CLASSCODE\"";
                error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                $retorno = -1006;
            }

            if (empty($RMUSERNAME)) {
                $MSG = "-1007 Falha ao Modificar Objeto - UserName inválido: valor \"$RMUSERNAME\"";
                error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                $retorno = -1007;
            }

            if (empty($RMUSERPWD)) {
                $MSG = "-1008 Falha ao Modificar Objeto - senha inv�lida: valor \"$RMUSERPWD\"";
                error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                $retorno = -1008;
            }

            if ($retorno != 0) {
                $this->EventId = 1000;
                $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
                return $retorno;
            }


            $Indices = '';
            $Checks = '';
            $CountIndices = 0;
            $CountChecks = 0;
            foreach ($FieldCodes as $Field) {
                switch ($Field["FieldType"]) {
                    case "TX":
                    case "NU":
                    case "DT":
                    case "LT": {
                            if ($CountIndices > 0) {
                                $Indices .= "|";
                            }
                            $Indices .= trim($Field["FieldCode"]);
                            $Indices .= '=';
                            $Indices .= $Field["Value"];
                            $CountIndices++;
                            break;
                        }
                    case "BO": {
                            if ($CountChecks > 0) {
                                $Checks .= "|";
                            }
                            $Checks .= trim($Field["FieldCode"]);
                            $Checks .= "=";
                            $Checks .= $Field["Value"];
                            //error_log("CheckItem " . $Field["FieldCode"] . " Valor (" . $Field["Value"] . ")");
                            $CountChecks++;
                            break;
                        }
                }
            }
            $WSDL = $this->ParseValuesFields($this->URL_RMWS);
            $client = new soapclient($WSDL, 'wsdl');
            $client->setDefaultRpcParams(true);
            //$soap_proxy = $client->getProxy();	
            $Params["userName"] = $RMUSERNAME;
            $Params["userPWD"] = $RMUSERPWD;
            $Params["objectID"] = $RMOBJECTID;
            $Params["classCode"] = $CLASSCODE;
            $Params["sourceCode"] = $SOURCECODE;
            $Params["typeCode"] = $TYPECODE;
            $Params["indices"] = $Indices;
            $Params["checkItems"] = $Checks;

            $result = $client->call("ObjectModify", array($Params));
            if (!is_null($result["faultstring"])) {
                $Dados = "";
                while (list($key, $value) = each($Params)) {
                    $Dados .= "$key - $value ; ";
                }
                $retorno = $result["faultstring"];
                $MSG = "-1000 Falha ao Modificar Objeto no Content ObjectID:$RMOBJECTID Erro:" . $retorno;
                error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                $retorno = -1000;
            } else {
                if (!$result) {
                    $Dados = "";
                    while (list($key, $value) = each($Params)) {
                        $Dados .= "$key - $value ; ";
                    }
                    $MSG = "-1001 Falha ao Modificar Objeto no Content ObjectID:$RMOBJECTID, não houve retorno";
                    error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                    $retorno = -1001;
                } else {
                    $retorno = $result["ObjectModifyResult"];
                    if ($retorno != "OK") {
                        $Dados = "";
                        while (list($key, $value) = each($Params)) {
                            $Dados .= "$key - $value ; ";
                        }
                        $MSG = "-1002 Falha ao Modificar Objeto no Content ObjectID:$RMOBJECTID, Retorno: $retorno";
                        error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                        $retorno = -1002;
                    } else {
                        $MSG = "Modificado Objeto no Content ObjectID:$RMOBJECTID";
                        $retorno = 1;
                    }
                }
            }
            $this->EventId = 1000;
            $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
            return $retorno;
        }

        function FILEOBJECT($RMOBJECTPARENTCODE, $RMOBJETCHILDCODE, $RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $Subst = true)
        {
            return $this->ContentFileUnfileObjectContent($RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $RMOBJECTPARENTCODE, $RMOBJETCHILDCODE, true, $Subst);
        }

        function UNFILEOBJECT($RMOBJECTPARENTCODE, $RMOBJETCHILDCODE, $RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $Subst = true)
        {
            return $this->ContentFileUnfileObjectContent($RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $RMOBJECTPARENTCODE, $RMOBJETCHILDCODE, false, $Subst);
        }

        function ContentFileUnfileObjectContent($RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $RMOBJECTPARENTCODE, $RMOBJETCHILDCODE, $FILEUNFILE, $Subst = true)
        {
            $retorno = $this->RMFileUnfileObject($this->connect, $this->CaseNum, $this->ProcId, $this->FieldValuesByCode, $RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $RMOBJECTPARENTCODE, $RMOBJETCHILDCODE, $FILEUNFILE, $Subst);
            return $retorno;
        }

        function RMFileUnfileObject($connect, $CaseNum, $ProcId, $FieldCodes, $RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $RMOBJECTPARENTCODE, $RMOBJETCHILDCODE, $FILEUNFILE, $Subst = true)
        {
            require_once(FILES_ROOT . "vendor/lib/nusoap.php");
            if ($Subst) {
                $PARENTID = $FieldCodes[$RMOBJECTPARENTCODE]["Value"];
                $CHIELDID = $FieldCodes[$RMOBJETCHILDCODE]["Value"];
                $RMUSERNAME = $FieldCodes[$RMOBJECTUSERNAMECODE]["Value"];
                $RMUSERPWD = $FieldCodes[$RMOBJECTUSERPWDCODE]["Value"];
            } else {
                $PARENTID = $RMOBJECTPARENTCODE;
                $CHIELDID = $RMOBJETCHILDCODE;
                $RMUSERNAME = $RMOBJECTUSERNAMECODE;
                $RMUSERPWD = $RMOBJECTUSERPWDCODE;
            }

            if (strpos($PARENTID, "^")) {
                $PARENTID = substr($PARENTID, strrpos($PARENTID, "^") + 1);
            }

            if (strpos($CHIELDID, "^")) {
                $CHIELDID = substr($CHIELDID, strrpos($CHIELDID, "^") + 1);
            }


            $WSDL = $this->ParseValuesFields($this->URL_RMWS);
            $client = new soapclient($WSDL, 'wsdl');

            $client->setDefaultRpcParams(true);
            //$soap_proxy = $client->getProxy();	
            $Params["userName"] = $RMUSERNAME;
            $Params["userPWD"] = $RMUSERPWD;
            $Params["parentID"] = $PARENTID;
            $Params["childID"] = $CHIELDID;
            $Params["divisionCode"] = "";
            $Params["isFile"] = $FILEUNFILE;

            $Parametros = "Parent - $PARENTID; Child - $CHIELDID";

            $retorno = 0;
            if (empty($PARENTID)) {
                $MSG = "-1002 Falha ao inserir/remover Objeto - PARENT inválido: valor \"$PARENTID\"";
                error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                $retorno = -1102;
            }

            if (!is_numeric($PARENTID)) {
                $MSG = "-1006 Falha ao inserir/remover Objeto - PARENT inválido: valor \"$PARENTID\"";
                error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                $retorno = -1106;
            }


            if (empty($CHIELDID)) {
                $MSG = "-1103 Falha ao inserir/remover Objeto - CHIELD inválido: valor \"$CHIELDID\"";
                error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                $retorno = -1103;
            }

            if (!is_numeric($CHIELDID)) {
                $MSG = "-1107 Falha ao inserir/remover Objeto - CHIELD inválido: valor \"$CHIELDID\"";
                error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                $retorno = -1107;
            }

            if (empty($RMUSERNAME)) {
                $MSG = "-1104 Falha ao inserir/remover Objeto - Usuário inválido: valor \"$RMUSERNAME\"";
                error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                $retorno = -1104;
            }

            if (empty($RMUSERPWD)) {
                $MSG = "-1105 Falha ao inserir/remover Objeto - senha inválida: valor \"$RMUSERPWD\"";
                error_log("Caso: $this->CaseNum - $MSG - Dados $Parametros", 0);
                $retorno = -1105;
            }

            if ($retorno != 0) {
                $this->EventId = 1000;
                $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
                return $retorno;
            }

            $result = $client->call("ObjectFileUnfile", array($Params));

            //error_log("Movendo Objetos $CHIELDID -> $PARENTID", 0);
            if (!is_null($result["faultstring"])) {
                $MSG = "-1100 Falha ao inserir/remover objeto no Content ObjectID:$CHIELDID em ObjectID:$PARENTID - Erro: " . $result["faultstring"];
                error_log("Caso: $this->CaseNum - " . $MSG, 0);
                $retorno = -1100;
            } else {
                $retorno = $result["ObjectFileUnfileResult"];
                if ($retorno != "true") {
                    $MSG = "-1101 Falha ao inserir/remover objeto Dados: $Parametros Retorno $retorno";
                    error_log("Caso: $this->CaseNum - " . $MSG, 0);
                    $retorno = -1101;
                } else {
                    $MSG = "Inserido objeto Dados: $Parametros Retorno $retorno";
                    $retorno = 1;
                }
            }
            $this->EventId = 1000;
            $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
            return $retorno;
        }

        function RMCreateObject($connect, $CaseNum, $ProcId, $FieldCodes, $RMOBJECTSOURCECODE, $RMOBJECTTYPECODE, $RMOBJECTCLASSCODE, $RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $Subst = true)
        {
            require_once(FILES_ROOT . "vendor/lib/nusoap.php");


            if ($Subst) {
                $SOURCECODE = $FieldCodes[$RMOBJECTSOURCECODE]["Value"];
                $TYPECODE = $FieldCodes[$RMOBJECTTYPECODE]["Value"];
                $CLASSCODE = $FieldCodes[$RMOBJECTCLASSCODE]["Value"];
                $RMUSERNAME = $FieldCodes[$RMOBJECTUSERNAMECODE]["Value"];
                $RMUSERPWD = $FieldCodes[$RMOBJECTUSERPWDCODE]["Value"];
            } else {
                $SOURCECODE = $RMOBJECTSOURCECODE;
                $TYPECODE = $RMOBJECTTYPECODE;
                $CLASSCODE = $RMOBJECTCLASSCODE;
                $RMUSERNAME = $RMOBJECTUSERNAMECODE;
                $RMUSERPWD = $RMOBJECTUSERPWDCODE;
            }

            $retorno = 0;
            if (empty($SOURCECODE)) {
                $MSG = "-1203 Falha ao Criar Objeto - SOURCECODE inválido: Valor \"$SOURCECODE\"";
                error_log("Caso: $this->CaseNum - " . "$MSG", 0);
                $retorno = -1203;
            }

            if (empty($TYPECODE)) {
                $MSG = "-1204 Falha ao Criar Objeto - TYPECODE inválido: Valor \"$TYPECODE\"";
                error_log("Caso: $this->CaseNum - " . "$MSG", 0);
                $retorno = -1204;
                $TYPECODE = '';
            }

            if (empty($CLASSCODE)) {
                $MSG = "-1205 Falha ao Criar Objeto - CLASSCODE inválido: Valor \"$CLASSCODE\"";
                error_log("Caso: $this->CaseNum - " . "$MSG", 0);
                $retorno = -1205;
            }

            if (empty($RMUSERNAME)) {
                $MSG = "-1206 Falha ao Criar Objeto - Usu�rio inválido: Valor \"$RMUSERNAME\"";
                error_log("Caso: $this->CaseNum - " . "$MSG", 0);
                $retorno = -1206;
            }

            if (empty($RMUSERPWD)) {
                $MSG = "-1207 Falha ao Criar Objeto - senha inv�lida: Valor \"$RMUSERPWD\"";
                error_log("Caso: $this->CaseNum - " . "$MSG", 0);
                $retorno = -1207;
            }

            if ($retorno != 0) {
                $this->EventId = 1000;
                $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
                return $retorno;
            }

            $CountIndices = 0;
            $CountChecks = 0;
            foreach ($FieldCodes as $Field) {
                switch ($Field["FieldType"]) {
                    case "TX":
                    case "NU":
                    case "DT":
                    case "LT": {
                            if ($CountIndices > 0) {
                                $Indices .= "|";
                            }
                            $Indices .= trim($Field["FieldCode"]);
                            $Indices .= '=';
                            $Indices .= $Field["Value"];
                            $CountIndices++;
                            break;
                        }
                    case "BO": {
                            if ($CountChecks > 0) {
                                $Checks .= "|";
                            }
                            $Checks .= trim($Field["FieldCode"]);
                            $Checks .= "=";
                            $Checks .= $Field["Value"];
                            $CountChecks++;
                            break;
                        }
                }
            }
            $WSDL = $this->ParseValuesFields($this->URL_RMWS);
            $client = new soapclient($WSDL, 'wsdl');

            $client->setDefaultRpcParams(true);
            //$soap_proxy = $client->getProxy();	
            $Params["userName"] = $RMUSERNAME;
            $Params["userPWD"] = $RMUSERPWD;
            $Params["classCode"] = $CLASSCODE;
            $Params["sourceCode"] = $SOURCECODE;
            $Params["typeCode"] = $TYPECODE;
            $Params["indices"] = $Indices;
            $Params["checkItems"] = $Checks;
            $Params["fileContents"] = "";
            $Params["fileExtension"] = "";

            $result = @$client->call("ObjectCreateStringArray", array($Params));



            if (!is_null($result["faultstring"])) {
                $MSG = "-1200 Falha ao Criar Objeto - Erro faultstring: " . $result["faultstring"];
                $Dados = "";
                while (list($key, $value) = each($Params)) {
                    $Dados .= "$key $value; ";
                }
                error_log("Caso: $this->CaseNum - " . "$MSG - Dados $Dados", 0);
                $retorno = -1200;
            } else {
                if (!$result) {
                    $Dados = "";
                    while (list($key, $value) = each($Params)) {
                        $Dados .= "$key $value; ";
                    }
                    $MSG = "-1201 Falha ao criar Objeto não houve retorno na Chamada";
                    error_log("Caso: $this->CaseNum - " . "$MSG Dados - $Dados", 0);
                    $retorno = -1201;
                } else {
                    $retorno = $result["ObjectCreateStringArrayResult"];
                    if ($retorno < 0) {
                        $MSG = "-1202 Falha ao criar Objeto no Content, Retorno: $retorno";
                        $Dados = "";
                        while (list($key, $value) = each($Params)) {
                            $Dados .= "$key $value; ";
                        }
                        error_log("Caso: $this->CaseNum - " . "$MSG - Dados $Dados", 0);
                    } else {
                        $MSG = "Criado Objeto no Content ObjectID: $retorno";
                    }
                }
            }
            $this->EventId = 1000;
            $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
            return $retorno;
        }

        function RMChangeChildsIndex($RMOBJECTID, $INDEXCODE, $INDEXVALUE, $RMUSERNAME, $RMUSERPWD, $DEEP = 0)
        {
            require_once(FILES_ROOT . "vendor/lib/nusoap.php");

            if (strpos($RMOBJECTID, "^") !== false) {
                $RMOBJECTID = substr($RMOBJECTID, strrpos($RMOBJECTID, "^") + 1);
            }

            $Params["userName"] = $RMUSERNAME;
            $Params["userPWD"] = $RMUSERPWD;
            $Params["objectID"] = $RMOBJECTID;
            $Params["indexCode"] = $INDEXCODE;
            $Params["indexValue"] = $INDEXVALUE;
            $Params["deepness"] = $DEEP;

            $Parametros = "RMOBJECTID: $RMOBJECTID; INDEXCODE: $INDEXCODE; INDEXVALUE: $INDEXVALUE";

            $retorno = 0;
            if (empty($RMOBJECTID)) {
                $retorno = -1303;
                $MSG = "$retorno Falha ao Modificar Indice do Objeto - OBJECTID inválido Valor: \"$RMOBJECTID\"";
                error_log("Caso: $this->CaseNum - " . "$MSG - Dados $Parametros", 0);
            }

            if (!is_numeric($RMOBJECTID)) {
                $retorno = -1307;
                $MSG = "$retorno Falha ao Modificar Indice do Objeto - OBJECTID inválido Valor: \"$RMOBJECTID\"";
                error_log("Caso: $this->CaseNum - " . "$MSG - Dados $Parametros", 0);
            }

            if (empty($INDEXCODE)) {
                $retorno = -1304;
                $MSG = "$retorno Falha ao Modificar Indice do Objeto - INDEXCODE inválido Valor: \"$INDEXCODE\"";
                error_log("Caso: $this->CaseNum - " . "$MSG - Dados $Parametros", 0);
            }

            if (empty($RMUSERNAME)) {
                $retorno = -1305;
                $MSG = "$retorno Falha ao Modificar Indice do Objeto - Usu�rio inválido Valor: \"$RMUSERNAME\"";
                error_log("Caso: $this->CaseNum - " . "$MSG - Dados $Parametros", 0);
            }

            if (empty($RMUSERPWD)) {
                $retorno = -1306;
                $MSG = "$retorno Falha ao Modificar Indice do Objeto - Senha inv�lida Valor: \"$RMUSERPWD\"";
                error_log("Caso: $this->CaseNum - " . "$MSG - Dados $Parametros", 0);
            }

            if ($retorno != 0) {
                $this->EventId = 1000;
                $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
                return $retorno;
            }

            $WSDL = $this->ParseValuesFields($this->URL_RMWS);
            $client = new soapclient($WSDL, 'wsdl');
            $client->setDefaultRpcParams(true);
            //$soap_proxy = $client->getProxy();			
            $result = @$client->call("ObjectModifyChildIndices", array($Params));

            if (!is_null($result["faultstring"])) {
                $MSG = "-1300 Falha ao Modificar Indice do Objeto - $RMOBJECTID Erro:" . $result["faultstring"];
                error_log("Caso: $this->CaseNum - " . $MSG, 0);
                $retorno = -1300;
            } else {
                if (!$result) {
                    $MSG = "-1301 Falha alterando indice de Objeto no Content, ObjectID: $RMOBJECTID Indice:$INDEXCODE Valor:$INDEXVALUE, não houve retorno na chamada";
                    error_log("Caso: $this->CaseNum - " . $MSG, 0);
                    $retorno - 1301;
                } else {
                    $retorno = $result["ObjectModifyChildIndicesResult"];
                    if ($retorno != "OK") {
                        $MSG = "-1302 Falha alterando indice de Objeto no Content, ObjectID: $RMOBJECTID Indice:$INDEXCODE Valor:$INDEXVALUE, Retorno:" . $retorno;
                        error_log("Caso: $this->CaseNum - $MSG", 0);
                        $retorno - 1302;
                    } else {
                        $MSG = "Alterando indice de Objeto no Content, ObjectID: $RMOBJECTID Indice:$INDEXCODE Valor:$INDEXVALUE, Retorno:" . $retorno;
                        $retorno = 1;
                    }
                }
            }
            $this->EventId = 1000;
            $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
            return $retorno;
        }

        function RMObjectCheckOutReceived($RMOBJECTID, $DATARETORNO, $RMUSERNAME, $RMUSERPWD)
        {
            require_once(FILES_ROOT . "vendor/lib/nusoap.php");

            if (strpos($RMOBJECTID, "^")) {
                $RMOBJECTID = substr($RMOBJECTID, strrpos($RMOBJECTID, "^") + 1);
            }

            $Params["userName"] = $RMUSERNAME;
            $Params["userPWD"] = $RMUSERPWD;
            $Params["objectID"] = $RMOBJECTID;
            $Params["dataRetorno"] = $DATARETORNO;

            $Parametros = "objectID: $RMOBJECTID; dataRetorno: $DATARETORNO";

            $retorno = 0;
            if (empty($RMOBJECTID)) {
                $retorno = -1402;
                $MSG = "$retorno Falha ao executar ObjectCheckOutReceived - OBJECTID inválido Valor: \"$RMOBJECTID\"";
                error_log("Caso: $this->CaseNum - " . "$MSG - Dados $Parametros", 0);
            }

            if (!is_numeric($RMOBJECTID)) {
                $retorno = -1406;
                $MSG = "$retorno Falha ao executar ObjectCheckOutReceived - OBJECTID inválido Valor: \"$RMOBJECTID\"";
                error_log("Caso: $this->CaseNum - " . "$MSG - Dados $Parametros", 0);
            }

            if (empty($DATARETORNO)) {
                $retorno = -1403;
                $MSG = "$retorno Falha ao executar ObjectCheckOutReceived - dataRetorno inv�lida Valor: \"$DATARETORNO\"";
                error_log("Caso: $this->CaseNum - " . "$MSG - Dados $Parametros", 0);
            }

            if (empty($RMUSERNAME)) {
                $retorno = -1404;
                $MSG = "$retorno Falha ao executar ObjectCheckOutReceived - usu�rio inválido Valor: \"$RMUSERNAME\"";
                error_log("Caso: $this->CaseNum - " . "$MSG - Dados $Parametros", 0);
            }

            if (empty($RMUSERPWD)) {
                $retorno = -1405;
                $MSG = "$retorno Falha ao executar ObjectCheckOutReceived - senha inv�lida Valor: \"$RMUSERPWD\"";
                error_log("Caso: $this->CaseNum - " . "$MSG - Dados $Parametros", 0);
            }

            if ($retorno != 0) {
                $this->EventId = 1000;
                $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
                return $retorno;
            }

            $client = new soapclient($this->URL_RMFEEDER, 'wsdl');
            $client->setDefaultRpcParams(true);
            //$soap_proxy = $client->getProxy();			
            $result = @$client->call("ObjectCheckOutReceived", array($Params));

            if (!is_null($result["faultstring"])) {
                $MSG = "-1400 Falha ao executar ObjectCheckOutReceived ObjectID:$RMOBJECTID Erro:" . $result["faultstring"];
                error_log("Caso: $this->CaseNum - " . $MSG, 0);
                $retorno = -1400;
            } else {
                if (!$result) {
                    $MSG = "-1401 Falha ao executar ObjectCheckOutReceived ObjectID:$RMOBJECTID - N�o houve retorno da chamada";
                    error_log("Caso: $this->CaseNum - " . "$MSG", 0);
                    $retorno = -1401;
                } else {
                    $retorno = $result["ObjectCheckOutReceivedResult"];
                    $MSG = "Executado ObjectCheckOutReceived ObjectID:$RMOBJECTID - Retorno:$retorno";
                }
            }
            $this->EventId = 1000;
            $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
            return $retorno;
        }

        function RMObjectCheckIn($RMOBJECTID, $CODCAIXA, $RMUSERNAME, $RMUSERPWD)
        {
            require_once(FILES_ROOT . "vendor/lib/nusoap.php");
            if (strpos($RMOBJECTID, "^")) {
                $RMOBJECTID = substr($RMOBJECTID, strrpos($RMOBJECTID, "^") + 1);
            }

            $Params["userName"] = $RMUSERNAME;
            $Params["userPWD"] = $RMUSERPWD;
            $Params["objectID"] = $RMOBJECTID;
            $Params["codCaixa"] = $CODCAIXA;

            $Parametros = "objectID: $RMOBJECTID; codCaixa: $CODCAIXA";

            $retorno = 0;
            if (empty($RMOBJECTID)) {
                $retorno = -1502;
                $MSG = "$retorno Falha ao executar ObjectCheckIn - OBJECTID inválido Valor: \"$RMOBJECTID\"";
                error_log("Caso: $this->CaseNum - " . "$MSG - Dados $Parametros", 0);
            }

            if (!is_numeric($RMOBJECTID)) {
                $retorno = -1507;
                $MSG = "$retorno Falha ao executar ObjectCheckIn - OBJECTID inválido Valor: \"$RMOBJECTID\"";
                error_log("Caso: $this->CaseNum - " . "$MSG - Dados $Parametros", 0);
            }

            /* 				
              if (empty($CODCAIXA))
              {
              $retorno = -1503;
              $MSG = "$retorno Falha ao executar ObjectCheckIn - codCaixa inválido Valor: \"$CODCAIXA\"";
              error_log("Caso: $this->CaseNum - " . "$MSG - Dados $Parametros", 0);
              }
             */

            if (empty($RMUSERNAME)) {
                $retorno = -1504;
                $MSG = "$retorno Falha ao executar ObjectCheckIn - usu�rio inválido Valor: \"$RMUSERNAME\"";
                error_log("Caso: $this->CaseNum - " . "$MSG - Dados $Parametros", 0);
            }

            if (empty($RMUSERPWD)) {
                $retorno = -1505;
                $MSG = "$retorno Falha ao executar ObjectCheckIn - senha inv�lida Valor: \"$RMUSERPWD\"";
                error_log("Caso: $this->CaseNum - " . "$MSG - Dados $Parametros", 0);
            }

            if ($retorno != 0) {
                $this->EventId = 1000;
                $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
                return $retorno;
            }

            $client = new soapclient($this->URL_RMFEEDER, 'wsdl');
            $client->setDefaultRpcParams(true);
            //$soap_proxy = $client->getProxy();				
            $result = @$client->call("ObjectCheckIn", array($Params));

            if (!is_null($result["faultstring"])) {
                $MSG = "-1500 Falha ao executar ObjectCheckIn ObjectID:$RMOBJECTID Erro:" . $result["faultstring"];
                error_log("Caso: $this->CaseNum - " . "$MSG - $Parametros", 0);
                $retorno = -1500;
            } else {
                if (!$result) {
                    $MSG = "-1501 Falha ao executar ObjectCheckIn ObjectID:$RMOBJECTID";
                    error_log("Caso: $this->CaseNum - " . "$MSG - $Parametros", 0);
                    $retorno = -1501;
                } else {
                    $retorno = $result["ObjectCheckInResult"];
                    $MSG = "Executado ObjectCheckIn ObjectID:$RMOBJECTID Retorno: $retorno";
                }
            }
            $this->EventId = 1000;
            $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
            return $retorno;
        }

        function IsInternalFunction($Valor)
        {
            $InternalFunctions = array(
                'CONTENTCHANGECHILDSINDEX',
                'CONTENTMODIFYOBJECT',
                'CONTENTCREATEOBJECT',
                'UNFILEOBJECT',
                'FILEOBJECT',
                'ContentFileUnfileObjectContent',
                'GETCASEIDBYFIELDVALUE',
                'GETFIELDVALUE',
                'CONTENTOBJECTCHECKOUTRECEIVED',
                'CONTENTOBJECTCHECKIN',
                'DATE_DIFF',
                'GETUSERID',
                'GETSOURCEUSER',
                'SETFIELDVALUE',
                'IFF',
                'CONTENTOBJECTCHECKIFVIEWED',
                //'PEGAIRREGULARIDADES',
                'PEGAIRREGULARIDADES_TAC',
                'PEGAIRREGULARIDADES_CARUANA',
                'CLICKSIGN_CRIADOCUMENTO',
                'CLICKSIGN_SIGNER_CREATE',
                'CLICKSIGN_SIGNER_ADD',
                'PEGAIRREGULARIDADES_DOC',
                'COUNT_PAGES'
            );
            $Function = strtoupper(substr($Valor, 0, strpos($Valor, "(")));
            $retorno = in_array($Function, $InternalFunctions);
            return $retorno;
        }

        function GETSOURCEUSER($USERID)
        {
            if ($USERID <> trim($USERID)) {
                $MSG = "Removidos espa�os para chamada de  GETSOURCEUSER UserID:\"$USERID\"";
                $this->EventId = 1000;
                $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
                $USERID = trim($USERID);
            }
            $retorno = PegaOrigemUserById($USERID);
            $MSG = "Executado GETSOURCEUSER UserID:\"$USERID\" Retorno: \"$retorno\"";
            $this->EventId = 1000;
            $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
            return $retorno;
        }

        function GETUSERID($USERNAME)
        {
            return PegaIdPeloNome($USERNAME);
        }

        function date_diff_i($FROM, $TO, $UNIT)
        {
            return date_diff_i($FROM, $TO, $UNIT);
        }

        function pegaIrregularidades($listaCampos = '', $tipo_check = 'check_reverse')
        {
            global $connect;
            $casenum = $this->CaseNum;
            $SQL = "select FieldName from casedata, procfieldsdef where CaseNum = $casenum and procfieldsdef.FieldId = casedata.FieldId and procfieldsdef.FieldMask = 'check_reverse' and FieldValue = 1 and procfieldsdef.ProcId = casedata.ProcId and FieldCod in ($listaCampos)";
            $Query = mysqli_query($connect, $SQL);
            $lista = mysqli_fetch_all($Query, MYSQLI_ASSOC);
            $retorno = "";
            $quebra = "";
            foreach ($lista as $item) {
                $retorno .= $quebra . $item["FieldName"];
                error_log("Retorno Campo: " . $item["FieldName"]);
                $quebra = " | ";
            }
            error_log("Retorno Campo: " . $retorno);
            return $retorno;
        }

        function pegaIrregularidades_DOC()
        {
            $retorno = $this->pegaIrregularidades("'FC001', 'FC002', 'FC004', 'FC006', 'FC008', 'FC0009'");
            return $retorno;
        }

        function pegaIrregularidades_TAC()
        {
            $retorno = $this->pegaIrregularidades("'FC017', 'FC018', 'FC023', 'FC026', 'TACPRECORR'");
            return $retorno;
        }

        function pegaIrregularidades_Caruana()
        {
            $retorno = $this->pegaIrregularidades("'F001_RES', 'F002_CT', 'FOO2_DES', 'F001_CT', 'F001_DES', 'F001_DOC'");
            return $retorno;
        }

        function CONTENTCHANGECHILDSINDEX($OBJECTIDCODE, $INDEXCODE, $INDEXVALUECODE, $RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $DEEP = 0)
        {
            $retorno = $this->RMChangeChildsIndex($OBJECTIDCODE, $INDEXCODE, $INDEXVALUECODE, $RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $DEEP);
            return $retorno;
        }

        function CONTENTOBJECTCHECKOUTRECEIVED($RMOBJECTID, $DATARETORNO, $RMUSERNAME, $RMUSERPWD)
        {
            $retorno = $this->RMObjectCheckOutReceived($RMOBJECTID, $DATARETORNO, $RMUSERNAME, $RMUSERPWD);
            return $retorno;
        }

        function CONTENTOBJECTCHECKIN($RMOBJECTID, $CODCAIXA, $RMUSERNAME, $RMUSERPWD)
        {
            $retorno = $this->RMObjectCheckIn($RMOBJECTID, $CODCAIXA, $RMUSERNAME, $RMUSERPWD);
            return $retorno;
        }

        function ProcessoDoCaso()
        {
            $SQL = "select ProcId from casemap where CaseId = $this->CaseNum";
            $Query = mysqli_query($this->connect, $SQL);
            $Result = mysqli_fetch_array($Query);
            if (is_array($Result)) {
                return $Result["ProcId"];
            }
            return 0;
        }

        /**
         * 
         * @param type $ConditionId
         * @param type $resultadoAvaliacao
         * @param type $webServiceName
         * @param type $Wsdl
         * @param type $funcaoWebService
         * @param type $campoRetorno
         * @param type $ParamsComplex
         * @return type
         */
        function ExecutaWebServiceSOAP($ConditionId, $resultadoAvaliacao, $webServiceName, $Wsdl, $funcaoWebService, $FieldReturn, $ParamsComplex)
        {
            require_once(FILES_ROOT . "vendor/lib/nusoap.php");

            $WSDL = $this->ParseValuesFields($Wsdl);

            $client = new soapclient($WSDL, 'WSDL');
            if (!empty($client->error_str)) {
                $this->EventId = 1000;
                $MSG = "Falha ao Executar WebService - $webServiceName \nErro:" . $client->error_str;
                $this->Atualizaaudittrail(0, $ConditionId, $MSG);
                error_log("Erro MSG: $MSG");
                error_log("WebService não pode ser executado");
                return;
            }

            $client->setDefaultRpcParams(true);

            //$soap_proxy = $client->getProxy();	
            $SQL = "select "
                    . "ParamName, "
                    . "ParamValue "
                    . "from "
                    . "actionexecwebserviceparams "
                    . "where "
                    . "ProcId = $this->ProcId "
                    . "and ";
            $SQL .= " StepId = $this->StepId and ";
            $SQL .= "ConditionId = $ConditionId and  ";
            $SQL .= "TrueFalse = '$resultadoAvaliacao' ";
            $SQL .= "Order by ParamId";
            $Query = mysqli_query($this->connect, $SQL);

            //error_log("Parametros");
            $SParams = "";
            while ($Result = mysqli_fetch_array($Query)) {
                $ValorParametro = $Result["ParamValue"];
                $pValorParametro = $this->ParseValuesFields($ValorParametro);
                $NomeParametro = $Result["ParamName"];
                $Params[$NomeParametro] = $pValorParametro;
                $Param = "$NomeParametro - Valor \"$pValorParametro\" ($ValorParametro)\n";
                //error_log("Param $Param");
                $Param = "$NomeParametro - Valor \"$pValorParametro\" ($ValorParametro)\n";
                $SParams .= $Param;
            }

            //Se os parametros forem um complex, coloca dentro de outro array
            if ($ParamsComplex) {
                $Params = array($Params);
            }

            // Executa a Função do WebService
            $client->timeout = 60;
            $result = $client->call($funcaoWebService, $Params);

            if (!empty($client->error_str)) {
                $this->EventId = 1000;
                $MSG = "Falha ao Executar WebService - $webServiceName\nURL: $Wsdl\nParametros\n$SParams $\nErro:" . $client->error_str;
                $this->Atualizaaudittrail(0, $ConditionId, $MSG);
                return;
            }

            //$MSG = "Executado WebService - $Name\nURL: $WSDL\nParametros\n$SParams";
            $MSG = "Executado WebService - $webServiceName\nURL: $WSDL";
            $this->EventId = 1000;
            $this->Atualizaaudittrail(0, $ConditionId, $MSG);

            $contador = 0;
            if (!empty($FieldReturn)) {
                if (is_array($result)) {
                    $Aretorno = $result;
                    $retorno = "";
                    foreach ($Aretorno as $ItemRetorno) {
                        $retorno .= $ItemRetorno;
                        if ($contador == 11) {
                            break;
                        }
                        $contador++;
                    }
                } else {
                    $retorno = $result;
                }
                $this->Fields->SetFieldValue($this->connect, $FieldReturn, $retorno);
                $this->Fields->Field[$FieldReturn]->AtualizaCampo($this->connect, $this->ProcId, $this->CaseNum);
            }
        }

        function ExecutaWebServices($ConditionId, $resultadoAvaliacao)
        {
            $SQL = "select Name, WebService, Func, FieldReturn, ParamsComplex from actionexecwebservice where ProcId = $this->ProcId and ";
            $SQL .= " StepId = $this->StepId and ";
            $SQL .= "ConditionId = $ConditionId and  ";
            $SQL .= "TrueFalse = '$resultadoAvaliacao' ";
            $Query = mysqli_query($this->connect, $SQL);
            $Result = mysqli_fetch_array($Query);

            //error_log("P$this->StepId C$ConditionId Execução de WebService: " . mysqli_num_rows($Query));

            if (mysqli_num_rows($Query) == 0) {
                return;
            }

            $urlWebService = $Result["WebService"];
            $webServiceName = $Result["Name"];
            $paramsComplex = $Result["ParamsComplex"];
            $campoRetorno = $Result["FieldReturn"];

            if (LOG_DATA) {
                error_log("Nome: $webServiceName");
                error_log("url: $urlWebService");
                error_log("Parametros Complexos: $paramsComplex");
                error_log(" ");
            }

            $funcaoWebService = $Result["Func"];

            if (!empty($urlWebService)) {
                /*
                 * Sem função definida executa como SOAP
                 * 
                 */
                if (trim($funcaoWebService) != "") {
                    $this->ExecutaWebServiceSOAP($ConditionId, $resultadoAvaliacao, $webServiceName, $urlWebService, $funcaoWebService, $campoRetorno, $paramsComplex);
                } else {
                    $retornoWebService = $webServiceName($this, $urlWebService);
                    if ($retornoWebService["result"] !== false) {
                        $msgRetorno = "Executado";
                        $valorRetorno = $retornoWebService["valorRetorno"];
                        $this->Fields->SetFieldValue($this->connect, $campoRetorno, $valorRetorno);
                        $this->Fields->Field[$campoRetorno]->AtualizaCampo($this->connect, $this->ProcId, $this->CaseNum);
                    } else {
                        if (key_exists("erroRetorno", $retornoWebService)) {
                            $msgRetorno = $retornoWebService;
                        }
                    }
                    $MSG = "Resultado: WebService - $webServiceName\nURL: $urlWebService Retorno: " . var_export($msgRetorno, true);
                    $this->EventId = 1000;
                    $this->Atualizaaudittrail(0, $ConditionId, $MSG);
                }
            }
        }

        function AlteraCampoParaValorCampo($linha, $FieldId, $NewValue)
        {
            if ($NewValue > 0) {
                $NewValue = $this->Fields->Field[$NewValue]->FieldValueDB;
            } else {
                switch ($linha["NewValue"]) {
                    case -1: // Valor do CaseNum
                        $NewValue = $this->CaseNum;
                        break;
                    case -2: // Usu�rio que processou o caso
                        if ($this->UserId == -1) {
                            $NewValue = $this->UserName;
                        } else {
                            $NewValue = $this->UserId;
                        }
                        break;
                    case -3: // Data Processamento
                        if ($this->Fields->Field[$FieldId]->FieldType == 'DT') {
                            $NewValue = substr($this->Date, 0, 10);
                        } else {
                            $NewValue = substr($this->Date, 0, 10);
                        }
                        break;
                    case -4: // Hora Processamento dd/mm/yyyy hh:00
                        if ($this->Fields->Field[$FieldId]->FieldType == 'DT') {
                            $NewValue = substr($this->SDate, 11, 5);
                        } else {
                            $NewValue = substr($this->Date, 11, 5);
                        }
                        break;
                    case -10: // Data Hora Processamento dd/mm/yyyy hh:00
                        if ($this->Fields->Field[$FieldId]->FieldType == 'DT') {
                            $NewValue = $this->SDate;
                        } else {
                            $NewValue = $this->Date;
                        }
                        break;
                    case -5: // Nome do Passo Atual
                        $NewValue = $this->PegaNomePasso($this->StepId);
                        break;
                    case -6: // Nome do Passo Destino
                        $linha = $this->PegaStartStepId($ConditionId, $Acao);
                        if ($linha["StartStepId"]) {
                            $NewValue = $this->PegaNomePasso($linha["StartStepId"]);
                        } else {
                            $NewValue = '';
                        }
                        break;
                    case -7: // Id do Passo Atual
                        $NewValue = $this->StepId;
                        break;
                    case -8: // Id do passo Destino
                        $NewValue = $this->StartStepId;
                        break;
                    case -9: // Origem do Usu�rio
                        $NewValue = $this->Origem;
                        break;
                }
            }
            return $NewValue;
        }

        function AlteraCampoParaFuncao($ConditionId, $FieldId, $NewValue)
        {
            $ExecuteChange = true;
            $this->ConditionId = $ConditionId;
            $Funcao = $this->ParseValuesFields($NewValue);
            $NewValue = '';
            $Funcao = trim($Funcao);
            $AFuncoes = explode("\r\n", $Funcao);
            if (!is_array($AFuncoes)) {
                $AFuncoes[0] = $Funcao;
            }
            foreach ($AFuncoes as $Funcao) {
                if ($this->IsInternalFunction($Funcao)) {
                    $Funcao = '$this->' . $Funcao;
                } else {
                    $posicaoParenteses = strpos($Funcao, "(");
                    $nomeFuncaoTesteFuncao = substr($Funcao, 0, $posicaoParenteses);
                    if (!function_exists($nomeFuncaoTesteFuncao)) {
                        $this->EventId = 1000;
                        $MSG = "Função '$Funcao' não existe";
                        $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
                        return;
                    }
                }

                /* Melhorar essa funcionalidade	
                  else {
                  $Funcoes_Disponiveis = get_defined_functions();
                  if (array_search($Funcao, $Funcoes_Disponiveis["internal"]) == null)
                  {
                  $Funcao = "\"" . $Funcao . "\"";
                  }
                  }
                 */
                $RetornoAvaliacao = "";
                $this->EventId = 1000;

                try {
                    if (!eval('$RetornoAvaliacao = ' . $Funcao . "; return true;")) {
                        error_log("Falha ao executar $Funcao", 0);
                        $MSG = "Falha ao executar $Funcao; \n Não foi executada alteração no Valor do Campo " . $this->Fields->Field[$FieldId]->FieldName . "(" . $this->Fields->Field[$FieldId]->FieldCod . ")";
                        $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
                        $ExecuteChange = false; // mnpede a execução da alteração
                    }
                } catch (Exception $ex) {
                    
                }
                $NewValue .= $RetornoAvaliacao;
            }
            return $NewValue;
        }

        function AlteraCampo($ConditionId, $Acao)
        {
            $SQL = " select ";
            $SQL .= " FieldId, ";
            $SQL .= " NewValue, ";
            $SQL .= " TypeAttrib,";
            $SQL .= " FieldIdValue ";
            $SQL .= " from ";
            $SQL .= " actionalterfielddef ";
            $SQL .= " where ";
            $SQL .= " ProcID = $this->ProcId ";
            $SQL .= " and ";
            $SQL .= " StepId = $this->StepId ";
            $SQL .= " and ";
            $SQL .= " ConditionId = $ConditionId ";
            $SQL .= " and ";
            $SQL .= " TrueFalse = '$Acao' ";
            $QUERY = mysqli_query($this->connect, $SQL);
            $NrRecords = mysqli_num_rows($QUERY);
            if ($this->ProcId == 50 && $this->StepId == 1 && $NrRecords > 0) {
                error_log("CaseNum - $this->CaseNum Condição $ConditionId AlteraValor de Campo Total Registro $NrRecords");
            }
            $this->FieldValuesByCode = $this->CriaValuesByCode();
            while ($linha = mysqli_fetch_array($QUERY)) {
                $ExecuteChange = true;
                $NewValue = $linha["NewValue"];
                $FieldId = $linha["FieldId"];
                switch ($linha["TypeAttrib"]) {
                    case "C":
                        $NewValue = $this->AlteraCampoParaValorCampo($linha, $FieldId, $NewValue);
                        break;

                    case "F":
                        $NewValue = $this->AlteraCampoParaFuncao($ConditionId, $FieldId, $NewValue);
                        break;

                    default:
                        $NewValue = $this->ParseValuesFields($NewValue);
                        break;
                }
                $this->Fields->SetFieldValue($this->connect, $FieldId, $NewValue);
            }
            $this->Fields->AtualizaCampos($this->connect, $this->ProcId, $this->CaseNum);
        }

        function CriaValuesByCode()
        {
            $CODES = array();
            for ($i = 1; $i <= $this->FieldsCount; $i++) {
                $CODES[$this->Fields->Field[$i]->FieldCode]["FieldCode"] = $this->Fields->Field[$i]->FieldCode;
                $CODES[$this->Fields->Field[$i]->FieldCode]["Value"] = $this->Fields->Field[$i]->FieldValueDB;
                $CODES[$this->Fields->Field[$i]->FieldCode]["FieldType"] = $this->Fields->Field[$i]->FieldType;
            }
            return $CODES;
        }

        function ParseValuesFields_Old($texto)
        {
            // Se vazio sai da funcao
            if (empty($texto)) {
                return $texto;
            }
            // Se não tem substituição sai da funcao
            if (strpos($texto, '!%') === false && strpos($texto, '!$') === false) {
                return $texto;
            }

            // Substitui o CASENUM
            $texto = str_replace("!%CaseNum!%", $this->CaseNum, $texto);
            $texto = str_replace('!$CaseNum!$', $this->CaseNum, $texto);

            $texto = str_replace("!%SYSTEMDATETIME!%", date('d/m/Y H:i'), $texto);
            $texto = str_replace("!%DATA!%", date('d/m/Y H:i'), $texto);
            $texto = str_replace("!%SYSTEMDATE!%", date('d/m/Y'), $texto);
            $texto = str_replace("!%SYSTEMTIME!%", date('H:i'), $texto);


            $texto = str_replace('!%ProcId!%', $this->ProcId, $texto);


            for ($i = 1; $i <= $this->FieldsCount; $i++) {
                $FieldType = $this->Fields->Field[$i]->FieldType;
                if (strpos($texto, '!%') === false && strpos($texto, '!$') === false) {
                    return $texto;
                }
                $CODE = "!%" . $this->Fields->Field[$i]->FieldCode . "!%";
                $Valor = $this->Fields->Field[$i]->FieldValue;

                // Substitui o Nome do Campo
                $SubsName = '!$NAME_' . $this->Fields->Field[$i]->FieldCode . '!$';
                $Nome = $this->Fields->Field[$i]->FieldName;
                $texto = str_replace($SubsName, $Nome, $texto);

                // Substitui o Nome do Campo
                $SubsName = '!$NAME_' . $this->Fields->Field[$i]->FieldCode . '!$';
                $Nome = $this->Fields->Field[$i]->FieldName;
                $texto = str_replace($SubsName, $Nome, $texto);

                // Trata Campos Content
                if ($FieldType == "RM" || $FieldType == "RF") {
                    if (strpos($Valor, "^") !== false) {
                        $Valor = substr($Valor, 0, strpos($Valor, "^"));
                    }
                }

                if ($FieldType == "DT") {
                    $Valor = substr($Valor . " 00:00", 0, 16);
                }

                $Valor = str_replace('/*#/*', ";", $Valor);
                $texto = str_replace("$CODE", $Valor, $texto);

                $CODE = "!$" . $this->Fields->Field[$i]->FieldCode . "!$";
                $Valor = $this->Fields->Field[$i]->FieldValueDB;
                // Trata Campos Content
                if ($this->Fields->Field[$i]->FieldType == "RM" || $this->Fields->Field[$i]->FieldType == "RF") {
                    if (strpos($Valor, "^") !== false) {
                        $Valor = substr($Valor, strrpos($Valor, "^") + 1);
                    }
                }

                $texto = str_replace("$CODE", $Valor, $texto);

                // Faz a substituição de ORIGEM
                $CODE_ORIGEM = "!%GETORIGEM_" . $this->Fields->Field[$i]->FieldCode . "!%";
                $pos = strpos($texto, $CODE_ORIGEM);
                if (!($pos === false)) {
                    if ($this->Fields->Field[$i]->FieldType == 'US') {
                        $Valor = $this->Fields->Field[$i]->FieldValueDB;
                        if (is_numeric($Valor)) {
                            $Origem = PegaOrigemUserById($Valor);
                            $CODE = "!%GETORIGEM_" . $this->Fields->Field[$i]->FieldCode . "!%";
                            $texto = str_replace("$CODE", $Origem, $texto);
                        }
                    }
                }
            }
            return $texto;
        }

        function ParseValuesFields($texto)
        {
            // Se vazio sai da funcao
            if (empty($texto)) {
                return $texto;
            }
            // Se não tem substituição sai da funcao
            if (strpos($texto, '!{') === false && strpos($texto, '!%') === false && strpos($texto, '!$') === false && strpos($texto, '!{') === false || (substr_count($texto, '!$') % 2 > 0 || substr_count($texto, '!%') % 2 > 0)) {
                return $texto;
            }

            if (substr_count($texto, '!{') != substr_count($texto, '}!')) {
                return $texto;
            }


            // Substitui o CASENUM
            $texto = str_replace("!%CaseNum!%", $this->CaseNum, $texto);
            $texto = str_replace('!$CaseNum!$', $this->CaseNum, $texto);

            $texto = str_replace("!%SYSTEMDATETIME!%", date('d/m/Y H:i'), $texto);
            $texto = str_replace("!%DATA!%", date('d/m/Y H:i'), $texto);
            $texto = str_replace("!%SYSTEMDATE!%", date('d/m/Y'), $texto);
            $texto = str_replace("!%SYSTEMTIME!%", date('H:i'), $texto);

            $texto = str_replace('!%ProcId!%', $this->ProcId, $texto);

            while (strpos($texto, '!{') !== false && (substr_count($texto, '!{') == substr_count($texto, '}!'))) {
                $inicio = strpos($texto, '!{');
                $fim = strpos($texto, '}!', $inicio + 2);
                $CodigoCampo = substr($texto, $inicio + 2, $fim - $inicio - 2);
                $Valor = $this->PegaValorConstant($CodigoCampo);
                $texto = str_replace("!{" . $CodigoCampo . "}!", $Valor, $texto);
            }

            while (strpos($texto, '!%') !== false && (substr_count($texto, '!%') % 2) == 0) {
                $inicio = strpos($texto, '!%');
                $fim = strpos($texto, '!%', $inicio + 2);
                $CodigoCampo = substr($texto, $inicio + 2, $fim - $inicio - 2);
                $Valor = $this->Fields->PegaValorCampoByCode($CodigoCampo);
                $texto = str_replace("!%" . $CodigoCampo . "!%", $Valor, $texto);
            }

            while (strpos($texto, '!$') !== false && (substr_count($texto, '!$') % 2) == 0) {
                $inicio = strpos($texto, '!$');
                $fim = strpos($texto, '!$', $inicio + 2);
                $CodigoCampo = substr($texto, $inicio + 2, $fim - $inicio - 2);
                $Valor = $this->Fields->PegaValorDBCampoByCode($CodigoCampo);
                $texto = str_replace("!$" . $CodigoCampo . "!$", $Valor, $texto);
            }
            return $texto;
        }

        function ParseValuesFieldsMask($texto)
        {
            if (empty($texto)) {
                return $texto;
            }

            for ($i = 1; $i <= $this->FieldsCount; $i++) {
                $CODE = "!%" . $this->Fields->Field[$i]->FieldCode . "!%";
                $Valor = $this->Fields->Field[$i]->FieldValue;
                $texto = str_replace("$CODE", $Valor, $texto);


                // Faz a substituição de ORIGEM
                $CODE_ORIGEM = "!%GETORIGEM_" . $this->Fields->Field[$i]->FieldCode . "!%";
                $pos = strpos($texto, $CODE_ORIGEM);
                if (!($pos === false)) {
                    if ($this->Fields->Field[$i]->FieldType == 'US') {
                        $Valor = $this->Fields->Field[$i]->FieldValueDB;
                        if (is_numeric($Valor)) {
                            $Origem = PegaOrigemUserById($Valor);
                            $CODE = "!%GETORIGEM_" . $this->Fields->Field[$i]->FieldCode . "!%";
                            $texto = str_replace("$CODE", $Origem, $texto);
                        }
                    }
                }
            }

            $textoParse = str_replace("!%CaseNum!%", $this->CaseNum, $texto);

            return $textoParse;
        }

        function Insereaudittrail($ConditionId, $Acao)
        {
            $SQL = "select ";
            $SQL .= "Texto ";
            $SQL .= " from ";
            $SQL .= " actionaudittraildef ";
            $SQL .= " where ";
            $SQL .= " ProcId = $this->ProcId ";
            $SQL .= " and ";
            $SQL .= " StepId = $this->StepId ";
            $SQL .= " and ";
            $SQL .= " ConditionId = $ConditionId ";
            $SQL .= " and ";
            $SQL .= " TrueFalse = '$Acao' ";
            $Query = mysqli_query($this->connect, $SQL);
            $Result = mysqli_fetch_array($Query);

            if (mysqli_num_rows($Query) > 0) {
                $this->EventId = 900;
                $texto = $Result["Texto"];
                $texto = $this->ParseValuesFields($texto);
                $this->Atualizaaudittrail(0, $ConditionId, $texto);
            }
        }

        function PegaStartStepId($ConditionId, $Acao)
        {
            $SQL = " select ";
            $SQL .= " StartStepId, ";
            $SQL .= " rendezvous ";
            $SQL .= " from ";
            $SQL .= " actionstartstepdef, stepdef ";
            $SQL .= " where ";
            $SQL .= " actionstartstepdef.ProcId = $this->ProcId ";
            $SQL .= " and ";
            $SQL .= " actionstartstepdef.StepId = $this->StepId ";
            $SQL .= " and ";
            $SQL .= " actionstartstepdef.ConditionId = $ConditionId ";
            $SQL .= " and ";
            $SQL .= " actionstartstepdef.TrueFalse = '$Acao' ";
            $SQL .= " and ";
            $SQL .= " stepdef.StepId = StartStepId";
            $SQL .= " and ";
            $SQL .= " stepdef.ProcId = $this->ProcId";
            $QUERY = mysqli_query($this->connect, $SQL);
            $linha = mysqli_fetch_array($QUERY);
            return $linha;
        }

        function PegaRemoveFila($ConditionId, $Acao)
        {
            $SQL = " select ";
            $SQL .= " releasestepid ";
            $SQL .= " from ";
            $SQL .= " actionreleasestepdef";
            $SQL .= " where ";
            $SQL .= " actionreleasestepdef.ProcId = $this->ProcId ";
            $SQL .= " and ";
            $SQL .= " actionreleasestepdef.StepId = $this->StepId ";
            $SQL .= " and ";
            $SQL .= " actionreleasestepdef.ConditionId = $ConditionId ";
            $SQL .= " and ";
            $SQL .= " actionreleasestepdef.TrueFalse = '$Acao' ";
            $QUERY = mysqli_query($this->connect, $SQL);
            $linha = mysqli_fetch_array($QUERY);
            return $linha;
        }

        function PegaNomePasso($StepId)
        {
            $SQL = "select StepName from stepdef where ProcId = $this->ProcId and StepId = $StepId";
            $Query = mysqli_query($this->connect, $SQL);
            $Result = mysqli_fetch_array($Query);
            return $Result["StepName"];
        }

        function CriaStepAdministrador()
        {
            if ($this->CasoEstaNaFila(0)) {
                return;
            }
            $Date = date("Y-m-d H:i:s");
            $SQL = "insert into "
                    . " casequeue "
                    . " ("
                    . " ProcId, "
                    . " StepId, "
                    . " CaseId, "
                    . " LockedById, "
                    . " InsertDate, "
                    . " SendNotification, "
                    . " AdHoc,"
                    . " HideQueue,"
                    . " LockedBysamaccountname "
                    . ") "
                    . "values "
                    . "("
                    . " $this->ProcId, "
                    . " 0, "
                    . "$this->CaseNum"
                    . ", 0,"
                    . " '$Date',"
                    . " 0,"
                    . " 0,"
                    . " 0,"
                    . " ''"
                    . ")";
            mysqli_query($this->connect, $SQL);
        }

        function AlteraDeadCond($Tipo)
        {
            $SQL = "update casequeue set $Tipo = 0 where CaseId = $this->CaseNum and StepId = $this->StepId ";
            mysqli_query($this->connect, $SQL);
        }

        function CarregaCamposDaOrigem()
        {
            $SQL = " select FieldId, FieldSourceTable, FieldSourceField, FieldDisplayField from procfieldsdef where fieldUserId = -1 and ProcId = $this->ProcId";
            $Query = mysqli_query($this->connect, $SQL);
            while ($Result = mysqli_fetch_array($Query)) {
                $Campo = $Result["FieldId"];
                $Valor = $Result["FieldSourceField"];
                $Sel = PegaValorCampo($this->ProcId, $this->CaseNum, $Result["FieldDisplayField"]);
                $SQL = "select Grupo from origemgrupos where Origem = '$Sel' and TipoGrupo = $Valor";
                $Query2 = @mysqli_query($this->connect, $SQL);
                if ($Result = @mysqli_fetch_array($Query2)) {
                    $this->Fields->SetFieldValue($this->connect, $Campo, $Result["Grupo"]);
                    $this->Fields->Field[$Campo]->AtualizaCampo($this->connect, $this->ProcId, $this->CaseNum);
                }
            }
        }

        function RemoveCriadosDead($StepId)
        {
            $SQL = " select * from actionstartstepdef where ProcId = $this->ProcId and StepId = $StepId and (TrueFalse = 'S' or TrueFalse = 'H' or TrueFalse = 'D')";
            $Query = mysqli_query($this->connect, $SQL);
            while ($result = mysqli_fetch_array($Query)) {
                $StepRemover = $result["StartStepID"];
                $SQL = "select CaseId from casequeue where ProcId = $this->ProcId and StepId = $StepRemover and CaseId = $this->CaseNum ";
                $Query2 = mysqli_query($this->connect, $SQL);
                if (mysqli_num_rows($Query2) > 0) {
                    if ($result["NewInstance"] == 1) {
                        $SQL = " delete from casequeue where ProcId = $this->ProcId and StepId = $StepRemover and CaseId = $this->CaseNum ";
                        mysqli_query($this->connect, $SQL);
                        $this->EventId = 8;
                        $this->Atualizaaudittrail(0, $StepRemover);
                    }
                }
            }
        }

        function Erendezvous()
        {
            $SQL = " select ";
            $SQL .= " StepSource";
            $SQL .= " from ";
            $SQL .= " rendezvous, ";
            $SQL .= " casequeue ";
            $SQL .= " where ";
            $SQL .= " rendezvous.ProcId = $this->ProcId ";
            $SQL .= " and ";
            $SQL .= " StepTarget = $this->StartStepId";
            $SQL .= " and ";
            $SQL .= " casequeue.CaseId = $this->CaseNum ";
            $SQL .= " and ";
            $SQL .= " casequeue.StepId = rendezvous.StepSource";
            $SQL .= " and ";
            $SQL .= " casequeue.ProcId = $this->ProcId ";
            $SQL .= " and ";
            $SQL .= " StepSource <> $this->StepId ";
            $Query = mysqli_query($this->connect, $SQL);
            $rendezvous = mysqli_num_rows($Query) == 0;
            return $rendezvous;
        }

        function RemoveInstancia($rendezvous)
        {
            if ($rendezvous == 1) {
                $SQL = " select ";
                $SQL .= " count(*) as Instancias";
                $SQL .= " from ";
                $SQL .= " casequeue ";
                $SQL .= " where ";
                $SQL .= " ProcId = $this->ProcId ";
                $SQL .= " and ";
                $SQL .= " CaseId = $this->CaseNum ";
                $Query = mysqli_query($this->connect, $SQL);
                $Linha = mysqli_fetch_array($Query);
                if ($Linha['Instancias'] > 1) {
                    return true;
                }
            }
            return false;
        }

        function Open()
        {
            $this->_create();
        }

        function SetCaseNum($CaseNum = 0)
        {
            if ($this->NovoCaso) {
                if (!is_null($CaseNum) && $CaseNum != '0') {
                    $this->CaseNum = $CaseNum;
                    // Grava Origem do Usu�rio
                    $this->GravaOrigem();
                } else {
                    // Grava Origem do Usu�rio
                    $this->CaseNum = $this->CriaNovoCaso();

                    // Grava Origem do Usu�rio
                    $this->GravaOrigem();
                }
                $this->CriaStepAdministrador();
                $this->Open();
                $this->IniciaCaso();
            } else {
                $this->CaseNum = $CaseNum;
                $this->SetDocNumber($this->CaseNum);
            }
            return $CaseNum;
        }

        function IniciaCaso()
        {
            $this->NovoCaso = 1;
            $this->_CriaCaso();
            $this->SetDocNumber($this->CaseNum);
            $this->InicializaCamposInternos();
        }

        /**
         * 
         */
        function InicializaCamposInternos()
        {
            // Cria Campo Criador do Caso
            if ($this->NovoCaso) {
                if ($this->UserId == -1) {
                    $this->Fields->SetFieldValue($this->connect, 1, -1);
                } else {
                    $this->Fields->SetFieldValue($this->connect, 1, $this->UserId);
                    $this->Fields->Field[1]->AtualizaCampo($this->connect, $this->ProcId, $this->CaseNum);
                }

                // Cria Campo Data de Criação
                $this->Fields->SetFieldValue($this->connect, 2, $this->Date);
                $this->Fields->Field[2]->AtualizaCampo($this->connect, $this->ProcId, $this->CaseNum);
                $this->GravaOrigem();
            }
        }

        /**
         * 
         * @param type $StepId
         */
        function SetStep($StepId)
        {
            $this->StepId = $StepId;
            $this->_GetStep();
        }

        function GetStepDoc()
        {
            $SQL = "select ";
            $SQL .= " StepId ";
            $SQL .= " from ";
            $SQL .= " casequeue ";
            $SQL .= " where ";
            $SQL .= " CaseId = $this->DocNumber ";
            $SQL .= " and ";
            $SQL .= " ProcId = $this->ProcId ";
            $Query = mysqli_query($this->connect, $SQL);
            $Linha = mysqli_fetch_array($Query);
            $this->SetStep($Linha["StepId"]);
        }

        function _GetStep()
        {
            $SQL = " select ";
            $SQL .= " StepName, ";
            $SQL .= " StepDesc, ";
            $SQL .= " DeadSoft, ";
            $SQL .= " DeadHard, ";
            $SQL .= " DeadHardest, ";
            $SQL .= " DefaultView, ";
            $SQL .= " LogChangeFields ";
            $SQL .= " from ";
            $SQL .= " stepdef ";
            $SQL .= " where ";
            $SQL .= " StepId = $this->StepId ";
            $SQL .= " and ";
            $SQL .= " ProcId = $this->ProcId ";
            $Query = mysqli_query($this->connect, $SQL);
            $Linha = mysqli_fetch_array($Query);
            $this->StepName = $Linha["StepName"];
            $this->StepDesc = $Linha["StepDesc"];
            $this->DeadSoft = $Linha["DeadSoft"];
            $this->DeadHard = $Linha["DeadHard"];
            $this->DeadHardest = $Linha["DeadHardest"];
            $this->DefaultView = $Linha["DefaultView"];
            $GravarAlteracoes = $Linha["LogChangeFields"] == 1;
            $this->LogarAlteracoesCondicao = $GravarAlteracoes;
            $this->LogarAlteracoesFormulario = $this->LogarAlteracoesFormulario || $GravarAlteracoes;
            mysqli_free_result($Query);
        }

        /**
         * 
         */
        function _create()
        {
            $this->Fields = new STEPFIELDS;
            //error_log("Criando Campos $this->CaseNum", 0);
            $this->Fields->Cria($this->connect, $this->ProcId, $this->StepId);
            $this->FieldsCount = $this->Fields->FieldsCount;
            $this->FieldCodes = $this->PegaFieldIdsByCode();
            //error_log("Criando Campos $this->CaseNum", 0);
        }

        /**
         * 
         * @param type $indice
         * @return type
         */
        function fieldI($indice)
        {
            $field = new STEPFIELD;
            $field = $this->Fields->Field[$this->Fields->FieldList[$indice]];
            return $field;
        }

        /**
         * 
         * @param type $attach
         * @return type
         */
        function PegaNomeArquivos($attach)
        {
            $attach = $this->Fields->Field[$attach]->FieldValueDB;
            return $attach;
        }

        /**
         * 
         * @param type $ConditionId
         * @param type $resultadoCondicao
         */
        function Enviaemails($ConditionId, $resultadoCondicao)
        {
            $SQL = " select ";
            $SQL .= " Subject, ";
            $SQL .= " Body, ";
            $SQL .= " attachs ";
            $SQL .= " from ";
            $SQL .= " actionsendemail ";
            $SQL .= " where ";
            $SQL .= " ProcId = $this->ProcId ";
            $SQL .= " and ";
            $SQL .= " StepId = $this->StepId ";
            $SQL .= " and ";
            $SQL .= " ConditionId = $ConditionId ";
            $SQL .= " and ";
            $SQL .= " TrueFalse = '$resultadoCondicao' ";
            $Query = mysqli_query($this->connect, $SQL);
            if (mysqli_num_rows($Query) > 0) {
                $SQL = "insert into ";
                $SQL .= " actionsendemailqueue ";
                $SQL .= " ( ";
                $SQL .= " ProcId, ";
                $SQL .= " StepId, ";
                $SQL .= " ConditionId, ";
                $SQL .= " TrueFalse, ";
                $SQL .= " CaseNum, ";
                $SQL .= " emailsent ";
                $SQL .= " )  ";
                $SQL .= " values  ";
                $SQL .= " ( ";
                $SQL .= " $this->ProcId, ";
                $SQL .= " $this->StepId, ";
                $SQL .= " $ConditionId, ";
                $SQL .= " '$resultadoCondicao', ";
                $SQL .= " $this->CaseNum, ";
                $SQL .= " 0 ";
                $SQL .= " ) ";
                mysqli_query($this->connect, $SQL);
            }
        }

        function ExecutaCondicoes($Tipo)
        {
            $Remover = false;
            //error_log("Executando Condiçães - $this->CaseNum", 0);
            // Seleciona as Condiçães
            $SQL = " Select ";
            $SQL .= " ConditionName, ";
            $SQL .= " ConditionId, ";
            $SQL .= " TableCreate, ";
            $SQL .= " StopTrue, ";
            $SQL .= " StopFalse, ";
            $SQL .= " RemoveTrue, ";
            $SQL .= " RemoveFalse, ";
            $SQL .= " RemoveTStepId, ";
            $SQL .= " RemoveFStepId ";
            $SQL .= " from ";
            $SQL .= " stepconditiondef ";
            $SQL .= " where ";
            $SQL .= " ProcId = $this->ProcId ";
            $SQL .= " and ";
            $SQL .= " StepId = $this->StepId ";
            $SQL .= " and ";
            $SQL .= " ConditionType = '$Tipo'";
            $SQL .= " order by ";
            $SQL .= " ConditionOrder ";
            $QUERY_CONDICOES = mysqli_query($this->connect, $SQL);

            $ACondicoes = array();
            // error_log("Condições Passo $this->StepId :" . mysqli_num_rows($QUERY_CONDICOES));

            while ($linha = mysqli_fetch_array($QUERY_CONDICOES)) {
                $ConditionId = $linha["ConditionId"];
                //$this->ConditionName = PegaNomeCondition($this->ProcId, $this->StepId, $ConditionId);
                $this->ConditionName = $linha["ConditionName"];
                if (in_array($ConditionId, $ACondicoes)) {
                    error_log("Caso: $this->CaseNum, Condição já foi avaliada Processo: $this->ProcId, Passo: $this->StepId Condicao: $ConditionId ");
                    continue;
                }
                array_push($ACondicoes, $ConditionId);
                $StopTrue = $linha["StopTrue"];
                $StopFalse = $linha["StopFalse"];
                $RemoveTrue = $linha["RemoveTrue"];
                $RemoveFalse = $linha["RemoveFalse"];
                $RemoveTStepId = $linha["RemoveTStepId"];
                $RemoveFStepId = $linha["RemoveFStepId"];
                $Status = true;

                $retornoAvaliacaoCondicao = $this->AvaliacaoExecucaoCondicao($ConditionId, $RemoveTrue, $RemoveFalse, $RemoveTStepId, $RemoveFStepId);

                if (($retornoAvaliacaoCondicao == true && $StopTrue == 1) || ($retornoAvaliacaoCondicao == false && $StopFalse == 1)) {

                    break;
                }
            }
            return $Remover;
        }

        /**
         * 
         */
        function EncerraCaso()
        {
            $SQL = "select count(*) as Filas from casequeue where ProcId = $this->ProcId and StepId > 0 and CaseId = $this->CaseNum ";

            $Query = mysqli_query($this->connect, $SQL);
            $linha = mysqli_fetch_array($Query);
            if ($linha['Filas'] == 0) {
                $this->EventId = 500;
                $this->Atualizaaudittrail(0, 0);
                $SQL = "select FieldId from procfieldsdef where FieldRazao = 1 and ProcId = $this->ProcId order by FieldRazaoOrder";
                $Query = mysqli_query($this->connect, $SQL);
                $Contador = 0;
                while ($linha = mysqli_fetch_array($Query)) {
                    $Contador++;
                    $CampoRazao = $linha['FieldId'];
                    $Campo = "Razao" . $Contador;
                    $Valor = substr($this->Fields->Field[$this->Fields->FieldList[$CampoRazao]]->FieldValue, 0, 20);
                    $SQL = "update audittrail set $Campo = '$Valor' where ProcId = $this->ProcId and CaseNum = $this->CaseNum and EventId = 500";
                    mysqli_query($this->connect, $SQL);
                }
            }
        }

        /**
         * 
         * @param type $StepId
         * @return type
         */
        function GetWaitForEvent($StepId)
        {
            $SQL = "select ";
            $SQL .= " WaitForEvent ";
            $SQL .= " from ";
            $SQL .= " stepdef ";
            $SQL .= " Where ";
            $SQL .= " ProcId = $this->ProcId ";
            $SQL .= " and ";
            $SQL .= " StepId = $StepId ";
            $Query = mysqli_query($this->connect, $SQL);
            $result = mysqli_fetch_array($Query);
            return $result['WaitForEvent'];
        }

        /**
         * 
         * @param type $StepId
         * @return boolean
         */
        function CasoEstaCasoEstaNaFila($StepId)
        {
            $SQL = " select count(*) as Fila from casequeue where procId = $this->ProcId and StepId = $StepId and CaseId = $this->CaseNum ";
            $QUERY = mysqli_query($this->connect, $SQL);
            $Linha = mysqli_fetch_array($QUERY);
            if ($Linha['Fila'] > 0) {
                return true;
            }
            return false;
        }

        /**
         * 
         * @param type $ConditionId
         * @param type $Tipo
         * @return type
         */
        function AlterarCaso($ConditionId, $Tipo)
        {
            $SQL = "select "
                    . " * "
                    . "from "
                    . "actionaltercase "
                    . "where "
                    . "ProcId = $this->ProcId "
                    . " and "
                    . "StepId = $this->StepId "
                    . " and "
                    . "ConditionId = $ConditionId "
                    . " and "
                    . "TrueFalse = '$Tipo' ";
            $Query = mysqli_query($this->connect, $SQL);

            if (mysqli_num_rows($Query) == 0) {
                return;
            }

            error_log("P$this->StepId C$ConditionId Nr. de Alteraçõoes Campo Processo: " . mysqli_num_rows($Query));

            $Result = mysqli_fetch_array($Query);
            $ProcId = $Result["ProcIdCase"];
            $StepId = $Result["StepIdCase"];
            $ReleaseStep = $Result["ReleaseStep"];

            $FieldCaseNum = $Result["FieldCaseNum"];

            $NumerosCasoAlterar = array();
            if ($this->Fields->Field[$FieldCaseNum]->FieldType != "DC") {
                $NumerosCasoAlterar[] = trim($this->Fields->Field[$FieldCaseNum]->FieldValueDB);
            } else {
                $casosAlterar = $this->Fields->Field[$FieldCaseNum]->FieldValueDB;
                if (!is_array($casosAlterar)) {
                    $NumerosCasoAlterar = array($casosAlterar);
                } else {
                    $NumerosCasoAlterar = $casosAlterar;
                }
            }

            $ProcId = $this->PegaProcIdByCode($ProcId);
            $StepId = $this->PegaStepIdByCode($ProcId, $StepId);

            if (!$this->ProcessoExiste($ProcId)) {
                $Desc = "Processo: ($ProcId) Caso: $this->CaseNum não existe";
                $this->Atualizaaudittrail(0, $this->StepId, $Desc);
                return;
            }

            if (!$this->PassoExiste($ProcId, $StepId)) {
                $Desc = "Processo: ($ProcId) Passo: ($StepId) Caso: $this->CaseNum não existe";
                $this->Atualizaaudittrail(0, $this->StepId, $Desc);
                return;
            }

            if (empty($NumerosCasoAlterar)) {
                return;
            }

            $SQL = "select * from actionaltercasemap where ProcId = $this->ProcId ";
            $SQL .= " and StepId = $this->StepId ";
            $SQL .= " and ConditionId = $ConditionId ";
            $SQL .= " and TrueFalse = '$Tipo' ";
            $Query = mysqli_query($this->connect, $SQL);
            //	if (mysqli_num_rows($Query) == 0)
            //		return;
            $i = 0;
            while ($Result = mysqli_fetch_array($Query)) {
                $Field["FieldSource"] = $Result["FieldSource"];
                $Field["FieldDest"] = $Result["FieldDest"];
                $Fields[$i] = $Field;
                $i++;
            }
            $valores = $this->Pegavalores($Fields);

            $Acao = 1;
            if ($ReleaseStep == 1) {
                $Acao = 0;
            }

            foreach ($NumerosCasoAlterar as $numeroCaso) {
                $this->AtualizarCaso($ProcId, $StepId, $numeroCaso, $valores, $Acao);
            }
        }

        /**
         * Cria Casos em Processo 
         * 
         * @param type $ConditionId
         * @param type $Tipo
         * @return type
         */
        function CriaSignatarios($ConditionId = 0, $Tipo = "T")
        {
            $Erro = 0; // Define que não houve erro

            $SQL = "select * from condition_signdocument where "
                    . " ProcId = $this->ProcId "
                    . " and StepId = $this->StepId "
                    . " and ConditionId = $ConditionId "
                    . " and TrueFalse = '$Tipo' ";
            $Query = mysqli_query($this->connect, $SQL);
            if (mysqli_num_rows($Query) == 0) {
                return;
            }
            $Result = mysqli_fetch_array($Query);

            $codeFieldListCasosSignatarios = $Result["fieldListCaseNumSigners"];
            $idFieldListCasosSignatarios = $this->Fields->FieldsByCode[$codeFieldListCasosSignatarios];

            $ProcId = $Result["procSignersManager"];
            if (!is_numeric($ProcId)) {
                $ProcId = PegaProcIdByCode($ProcId);
            }

            if (!$this->ProcessoExiste($ProcId)) {
                $Erro = -1900;
            }

            $StepId = $Result["stepSignersManager"];
            if (!is_numeric($StepId)) {
                $StepId = $this->PegaStepIdByCode($ProcId, $StepId);
            }

            if (!$this->PassoExiste($ProcId, $StepId)) {
                $Erro = -1901;
            }

            if ($Erro == 0) {

                $fieldSigners = $Result["signersFieldCode"];

                $fieldId = $this->Fields->FieldsByCode[$fieldSigners];


                $jSignatarios = $this->Fields->Field[$fieldId]->FieldValueDB;

                // Lista de Campos 
                $newCaseFields = array();

                // Campo para criacao
                $newCaseField = array();

                $listaSignatarios = json_decode($jSignatarios, true);

                $listaCasosSignatarios = array();
                foreach ($listaSignatarios as $dadosSignatario) {

                    // Nome do Signatario                    
                    $nomeSignatario = $dadosSignatario["signatario_nome"];
                    $newCaseField["FieldCode"] = "signer.name";
                    $newCaseField["Value"] = $nomeSignatario;
                    $newCaseFields[] = $newCaseField;

                    // e-mail do Signatario
                    $newCaseField["FieldCode"] = "signer.email";
                    $newCaseField["Value"] = $dadosSignatario["signatario_email"];
                    $newCaseFields[] = $newCaseField;

                    // Cpf do Signatario
                    $newCaseField["FieldCode"] = "signer.cpf";
                    $newCaseField["Value"] = $dadosSignatario["signatario_cpf"];
                    $newCaseFields[] = $newCaseField;

                    // Data Nascimento do Signatario
                    $newCaseField["FieldCode"] = "signer.birthday";
                    $newCaseField["Value"] = $dadosSignatario["signatario_datanasc"];
                    $newCaseFields[] = $newCaseField;

                    // Telefone do Signatario
                    $newCaseField["FieldCode"] = "signer.phone_number";
                    $newCaseField["Value"] = $dadosSignatario["signatario_telefone"];
                    $newCaseFields[] = $newCaseField;

                    // Documento Referencia
                    $newCaseField["FieldCode"] = "DOCUMENTO";
                    $newCaseField["Value"] = $this->CaseNum;
                    $newCaseFields[] = $newCaseField;


                    /**
                     *  Extrai as Participações do Signatario
                     */
                    $participacosSignatario = $dadosSignatario["signatario_participacoes"];
                    $codigosParticipacao = array_column($participacosSignatario, "PARTICIPACAO");
                    $descicaoParticipacao = array_column($participacosSignatario, "DESCRICAO");

                    // Tipo de Assinatura                                         
                    $newCaseField["FieldCode"] = "TIPO_ASSINATURA";
                    $newCaseField["Value"] = implode(", ", $descicaoParticipacao);
                    $newCaseFields[] = $newCaseField;

                    // Tipo de Assinatura Interna
                    $newCaseField["FieldCode"] = "INT_TIPO_ASSINATURA";
                    $newCaseField["Value"] = json_encode($codigosParticipacao);
                    $newCaseFields[] = $newCaseField;

                    // Cria a estrutura de Campos
                    $fieldData["Fields"] = $newCaseFields;
                    $fieldData["NumFields"] = count($newCaseFields);

                    $Acao = 0;
                    $numeroCasoCriado = $this->CriaCaso($ProcId, $StepId, $fieldData, $Acao);

                    $listaCasosSignatarios[] = $numeroCasoCriado;

                    /**
                     *  Não insere no momento Depois mudar o código para uma entrada SIMPLES no Histórico
                     */
                    //$Desc = "Criado Signatário $nomeSignatario ($NewCaseNum)";
                    //$this->Atualizaaudittrail(0, $this->StepId, $Desc);
                }
            } else {
                $numeroCasoCriado[] = $Erro;
            }
            $this->Fields->SetFieldValue($this->connect, $idFieldListCasosSignatarios, $listaCasosSignatarios);
            $this->Fields->AtualizaCampos($this->connect, $this->ProcId, $this->CaseNum);
        }

        /**
         * Cria Casos em Processo 
         * 
         * @param type $ConditionId
         * @param type $Tipo
         * @return type
         */
        function CriaCasos($ConditionId = 0, $Tipo = "T")
        {
            $Erro = 0; // Define que não houve erro

            $SQL = "select * from actionstartprocid where ProcId = $this->ProcId ";
            $SQL .= " and StepId = $this->StepId ";
            $SQL .= " and ConditionId = $ConditionId ";
            $SQL .= " and TrueFalse = '$Tipo' ";
            $Query = mysqli_query($this->connect, $SQL);
            if (mysqli_num_rows($Query) == 0) {
                return;
            }

            $Result = mysqli_fetch_array($Query);
            $ProcId = $Result["ProcIdStart"];
            if (!is_numeric($ProcId)) {
                $ProcId = PegaProcIdByCode($ProcId);
            }

            if (!$this->ProcessoExiste($ProcId)) {
                $Erro = -1900;
            }

            $StepId = $Result["StepIdStart"];
            if (!is_numeric($StepId)) {
                $StepId = $this->PegaStepIdByCode($ProcId, $StepId);
            }

            if (!$this->PassoExiste($ProcId, $StepId)) {
                $Erro = -1901;
            }

            $ReleaseStep = $Result["ReleaseStep"];
            $FieldCaseNum = $Result["FieldResult"];

            $QuantCasos = $this->Fields->Field[$Result["FieldQuant"]]->FieldValueDB;
            if (empty($QuantCasos)) {
                $QuantCasos = $Result["NumQuant"];
            }

            if ($Erro == 0) {
                $SQL = "select * from actionstartprocidmap where ProcId = $this->ProcId ";
                $SQL .= " and StepId = $this->StepId ";
                $SQL .= " and ConditionId = $ConditionId ";
                $SQL .= " and TrueFalse = '$Tipo' ";
                $Query = mysqli_query($this->connect, $SQL);
                $i = 0;
                while ($Result = mysqli_fetch_array($Query)) {
                    $Field["FieldSource"] = $Result["FieldSource"];
                    $Field["FieldDest"] = $Result["FieldDest"];
                    $Fields[$i] = $Field;
                    //error_log('Campos: ' . $Field["FieldSource"] . ' - ' . $Field["FieldDest"]);
                    $i++;
                }
                $valores = $this->Pegavalores($Fields);

                $Acao = 1;
                if ($ReleaseStep == 1) {
                    $Acao = 0;
                }
                $NewCaseNum = $this->CriaCaso($ProcId, $StepId, $valores, $Acao);
            } else {
                $NewCaseNum = $Erro;
            }
            $this->Fields->SetFieldValue($this->connect, $FieldCaseNum, $NewCaseNum);
            $this->Fields->AtualizaCampos($this->connect, $this->ProcId, $this->CaseNum);
        }

        function CriaNovoCaso()
        {
            $SQL = "insert into casemap (ProcId, UserId, samaccountname) values ($this->ProcId, $this->UserId, '$this->UserName')";
            mysqli_query($this->connect, $SQL);

            $SQL = "select max(CaseId) as NRCaso from casemap where ProcId = $this->ProcId and UserId = $this->UserId and samaccountname = '$this->UserName'";
            $Query = mysqli_query($this->connect, $SQL);
            $Result = mysqli_fetch_array($Query);
            $NrCaso = $Result["NRCaso"];
            return $NrCaso;
        }

        function CriaCaso($ProcId, $StepId, $valores, $Acao)
        {
            $Caso = new STEPDOC;
            $Caso->SetConnection($this->connect);
            $Caso->SetProc($ProcId);
            $Caso->SetStep($StepId);
            $Caso->open();
            $Caso->UserId = $this->UserId;
            $Caso->UserName = $this->UserName;
            $Caso->UserDesc = $this->UserDesc;
            $Caso->samaccountname = $this->UserName;
            $Caso->NovoCaso = 1;
            $Caso->Origem = $this->Origem;
            $CaseNum = $Caso->CriaNovoCaso();
            $Caso->SetCaseNum($CaseNum);
            $Caso->LogarAlteracoesFormulario = true;
            $Caso->PegaDadosDeArray($valores);
            $this->EventId = 21;
            $Caso->SetAction($Acao);
            if ($Acao = 1) {
                $this->EventId = 22;
            }
            $Desc = "Processo: " . PegaNomeProc($ProcId) . " Caso $CaseNum";
            $this->Atualizaaudittrail(0, $this->StepId, $Desc);
            return $CaseNum;
        }

        function AtualizarCaso($ProcId, $StepId, $CaseNum, $valores, $Acao)
        {
            if (!CasoCasoEstaNaFila($ProcId, $StepId, $CaseNum)) {
                $this->EventId = 25;
                $Desc = "Processo: " . PegaNomeProc($ProcId) . "\nPasso: " . PegaNomeStep($ProcId, $StepId) . "\nCaso $CaseNum";
                $this->Atualizaaudittrail(0, $this->StepId, $Desc);
                $Acao = 1;
            }
            $Caso = new STEPDOC;
            $Caso->SetConnection($this->connect);
            $Caso->SetProc($ProcId);
            $Caso->SetStep($StepId);
            $Caso->open();
            $Caso->UserId = $this->UserId;
            $Caso->UserName = $this->UserName;
            $Caso->UserDesc = $this->UserDesc;
            $Caso->samaccountname = $this->UserName;
            $Caso->SetCaseNum($CaseNum);
            $Caso->LogarAlteracoesFormulario = true;
            $Caso->PegaDadosDeArray($valores);
            $this->EventId = 23;
            $Caso->SetAction($Acao);
            if ($Acao == 1) {
                $this->EventId = 24;
            }
            $Desc = "Processo: " . PegaNomeProc($ProcId) . "\nPasso: " . PegaNomeStep($ProcId, $StepId) . "\nCaso $CaseNum";
            $this->Atualizaaudittrail(0, $this->StepId, $Desc);
        }

        function Pegavalores($Fields)
        {
            for ($i = 0; $i < count($Fields); $i++) {
                $Field["FieldId"] = $Fields[$i]["FieldDest"];
                $Field["Value"] = $this->Fields->Field[$Fields[$i]["FieldSource"]]->FieldValueDB;
                //error_log('Campos: ' . $Field["FieldId"] . ' - ' . $Field["Value"]);			
                $valores["Fields"][$i] = $Field;
                $valores["NumFields"] = $i + 1;
            }
            return $valores;
        }

        function IniciaStepDireto($ConditionId, $dadosStep, $EventId)
        {
            $this->EventId = $EventId;
            $this->InsertDate = $this->PegaInsertDate();
            $this->Prioridade = 0;
            if ($dadosStep["StartStepId"]) {
                $this->StartStepId = $dadosStep["StartStepId"];
                if ($this->CasoEstaCasoEstaNaFila($this->StartStepId)) {
                    $StartStepName = $this->PegaNomePasso($this->StartStepId);
                    $this->EventId = $EventId + 700;
                    $this->Atualizaaudittrail($ConditionId, $this->StartStepId, "", $this->ConditionName, $StartStepName);
                    return;
                }
                $rendezvous = $this->Erendezvous($dadosStep["rendezvous"]);
                $this->WaitForEvent = $this->GetWaitForEvent($this->StartStepId);
                if ($rendezvous) {
                    $StartStepName = $this->PegaNomePasso($this->StartStepId);
                    $this->Atualizaaudittrail($ConditionId, $this->StartStepId, "", $this->ConditionName, $StartStepName);
                    $this->Prioridade = 0;
                    $this->IniciaStep($this->StartStepId, $this->InsertDate, $this->WaitForEvent);
                }
            }
        }

        function RemoveCasoFila($ConditionId, $Acao)
        {
            // TODO: ACRESCENTAR Entrada em Eventos
            $linha = $this->PegaRemoveFila($ConditionId, $Acao);
            if (count($linha) == 0) {
                return;
            }
            $StepId = $linha["releasestepid"];
            $SQL = "delete from casequeue where procid = $this->ProcId and CaseId = $this->CaseNum and StepId = $StepId";
            $Query = mysqli_query($this->connect, $SQL);
            error_log("Tira da Fila: " . $SQL . " Resultado: $Query");
        }

        function IniciarStep($ConditionId, $Acao, $EventId)
        {
            $linha = $this->PegaStartStepId($ConditionId, $Acao);
            $this->IniciaStepDireto($ConditionId, $linha, $EventId);
        }

        function Substituicoes($Body)
        {
            for ($contador = 0; $contador <= count($this->Fields->FieldList); $contador++) {
                $Campo = $this->Fields->Field[$this->Fields->FieldList[$contador]]->FieldName;
                $Valor = $this->Fields->Field[$this->Fields->FieldList[$contador]]->FieldValue;
                $Body = str_replace("%C_$Campo%", "$Valor", $Body);
            }

            // Numero Caso
            $Body = str_replace("%E_NumeroDoc%", $this->CaseNum, $Body);

            // Criador do Caso	
            $Body = str_replace("%E_UsuarioCriadorDoc%", $this->Fields->Field[$this->Fields->FieldList[1]]->FieldValue, $Body);

            // Data Processamento
            $Body = str_replace("%E_DataProcessoForm%", Date("d/m/Y"), $Body);

            // Nome Step
            $Body = str_replace("%E_NomeForm%", $this->StepName, $Body);

            // Nome Processo
            $Body = str_replace("%E_NomeProc%", $this->ProcName, $Body);

            // Desc Step
            $Body = str_replace("%E_DescForm%", $this->StepDesc, $Body);

            // Desc Processo
            $Body = str_replace("%E_DescProc%", $this->ProcDesc, $Body);

            //DeadSoft
            $Body = str_replace("%E_DataHoraDeadSoft%", $this->DeadSoftDate, $Body);

            //DeadHard
            $Body = str_replace("%E_DataHoraDeadHard%", $this->DeadHardDate, $Body);

            //DeadHardest
            $Body = str_replace("%E_DataHoraDeadHardest%", $this->DeadHardestDate, $Body);

            //Usuario que Procecessou
            $Body = str_replace("%E_UsuarioProcessoForm%", $this->ProcName, $Body);

            $BodyTxt = $Body;
            $BodyHtml = $Body;

            $Pagina = "index.php";
            $Parte = "http://" . $this->Serverwww . "$Pagina?ProcId=$this->ProcId&StepId=$this->StartStepId&TD=WF&CaseNum=$this->CaseNum";

            //Link edit
            $BodyTxt = str_replace("%E_LinkEdit%", $Parte . "&Action=Edit", $BodyTxt);
            $BodyHtml = str_replace("%E_LinkEdit%", "<a href='" . $Parte . "&Action=Edit'>Editar Caso $this->CaseNum</a>", $BodyHtml);

            //Link view
            $BodyTxt = str_replace("%E_LinkView%", $Parte . "&Action=View", $BodyTxt);
            $BodyHtml = str_replace("%E_LinkView%", "<a href='" . $Parte . "&Action=View'>Visualizar Caso $this->CaseNum</a>", $BodyHtml);

            // Link Direct 
            $Pagina = "BPMEditCase.php";
            $Parte = $this->Serverwww . "$Pagina?ProcId=$this->ProcId&StepId=$this->StartStepId&TD=WF&CaseNum=$this->CaseNum";

            $BodyTxt = str_replace("%E_LinkDirect%", $Parte . "&Action=View&Direct=1'", $BodyTxt);
            $BodyHtml = str_replace("%E_LinkDirect%", "<a href='http://" . $Parte . "&Action=View&Direct=1'>Visualizar Caso $this->CaseNum</a>", $BodyHtml);

            $BodyHtml = str_replace("\n", "<br>", $BodyHtml);
            $BodyHtml = $BodyHtml . "<br><hr style='border:thin groove #0099FF; width:50%;'><br><center><a href='http://$this->Serverwww'>$this->ProcName</a></center>";
            $BodyHtml = "<html><body>" . $BodyHtml . "</body></html>";

            $Bodys[0] = $BodyTxt;
            $Bodys[1] = $BodyHtml;
            return $Bodys;
        }

        function PegaNome($nome)
        {
            $nome = str_replace('-', '_', $nome);
            $nome = str_replace('.', '_', $nome);
            return $nome;
        }

        function RemoveTabela()
        {
            $SQL = " drop table ";
            $SQL = $SQL . $this->NomeTabela;
            @mysqli_query($this->connect, $SQL);
        }

        function MontaTabela($Create)
        {
            $SQL = "CREATE TABLE ";
            $SQL = $SQL . $this->NomeTabela;
            $SQL .= " ( ";
            $SQL .= " $Create ";
            $SQL .= " ) ";
            @mysqli_query($this->connect, $SQL);
        }

        function MontaAvaliacao($QUERY_CONDICOES)
        {
            $contador = 0;
            $SQL = " select * from ";
            $SQL = $SQL . $this->NomeTabela;
            $SQL .= " where ";
            foreach ($QUERY_CONDICOES as $linha) {
                //while ($linha = mysqli_fetch_array($QUERY_CONDICOES))
                $OpenBracket = $linha["OpenBracket"];
                $CloseBracket = $linha["CloseBracket"];
                $Concat = $linha["Concat"];
                $Operador = $linha["Operator"];
                $Campo = $linha["Field1Id"];
                $Campo_Comparacao = "campo_$Campo" . "_$contador";
                $Valor_Comparacao = " valor_campo_$Campo" . "_$contador ";
                if ($Operador == "contêm") {
                    $Operador = "like";
                    $Valor_Comparacao = "'%' + $Valor_Comparacao + '%'";
                }
                $SQL .= " $Concat ";
                $SQL .= " $OpenBracket ";
                $SQL .= " $Campo_Comparacao ";
                $SQL .= " $Operador ";
                $SQL .= " $Valor_Comparacao";
                $SQL .= " $CloseBracket ";
                $contador++;
            }
            return $SQL;
        }

        function RemoveQueue($StepId)
        {
            $SQL = " delete from casequeue ";
            $SQL .= " where ";
            $SQL .= " StepId = $StepId ";
            $SQL .= " and ";
            $SQL .= " CaseId = $this->CaseNum ";
            $SQL .= " and ";
            $SQL .= " ProcId = $this->ProcId ";
            mysqli_query($this->connect, $SQL);
        }

        function ExecutaAcoes($ConditionId, $Acao)
        {
            //error_log("Executando Açães $ConditionId", 0);
            if ($Acao) {
                $this->AlteraCampo($ConditionId, "T");
                $this->IniciarStep($ConditionId, "T", 6);
                $this->Enviaemails($ConditionId, "T");
                $this->Insereaudittrail($ConditionId, "T");
                $this->ExecutaWebServices($ConditionId, "T");
                $this->CriaCasos($ConditionId, "T");
                $this->AlterarCaso($ConditionId, "T");
                $this->RemoveCasoFila($ConditionId, "T");

                // Cria Signatarios DOCUMENTO
                $this->CriaSignatarios($ConditionId, "T");
            } else {
                $this->AlteraCampo($ConditionId, "F");
                $this->IniciarStep($ConditionId, "F", 4);
                $this->Enviaemails($ConditionId, "F");
                $this->Insereaudittrail($ConditionId, "F");
                $this->ExecutaWebServices($ConditionId, "F");
                $this->CriaCasos($ConditionId, "F");
                $this->AlterarCaso($ConditionId, "F");
                $this->RemoveCasoFila($ConditionId, "F");

                // Cria Signatarios DOCUMENTO
                $this->CriaSignatarios($ConditionId, "F");
            }
            if ($this->LogarAlteracoesCondicao) {
                $this->EventId = 501;
                $this->GravaAlteracoesCampo($ConditionId, $this->ConditionName);
            }
        }

        function ExecutaAcoesCondicao($ConditionId, $TrueFalse, $RemoveQueue, $RemoveSteId)
        {
            if ($RemoveQueue) {
                $this->RemoverdaFila();
            }
            if ($RemoveSteId > 0) {
                $this->RemoverdaFilaViaMacro($RemoveSteId);
            }
            $this->EventId = 5000;
            $this->Atualizaaudittrail(0, $ConditionId, '', $this->ConditionName);
            $this->ExecutaAcoes($ConditionId, $TrueFalse);
        }

        function AvaliacaoExecucaoCondicao($ConditionId, $RemoveTrue, $RemoveFalse, $RemoveTStepId, $RemoveFStepId)
        {
            $SQL = " SELECT ";
            $SQL .= " * ";
            $SQL .= " FROM ";
            $SQL .= " stepconditiondevelop ";
            $SQL .= " where ";
            $SQL .= " ProcId = $this->ProcId ";
            $SQL .= " and ";
            $SQL .= " StepId = $this->StepId ";
            $SQL .= " and ";
            $SQL .= " ConditionId = $ConditionId ";
            $SQL .= " order by ";
            $SQL .= " LineOrder ";
            $QUERY1 = mysqli_query($this->connect, $SQL);

            $RetornoNovaAvaliacaoCondicao = true;

            // Pega dos dados para Avaliar a Condicao
            if (mysqli_num_rows($QUERY1) > 0) {
                $Condicoes = mysqli_fetch_all($QUERY1, MYSQLI_ASSOC);
                $RetornoNovaAvaliacaoCondicao = $this->NovoAvaliacondicoes($Condicoes);
            }

            if ($RetornoNovaAvaliacaoCondicao) {
                $this->ExecutaAcoesCondicao($ConditionId, true, $RemoveTrue, $RemoveTStepId);
            } else {
                $this->ExecutaAcoesCondicao($ConditionId, false, $RemoveFalse, $RemoveFStepId);
            }
            return $RetornoNovaAvaliacaoCondicao;
        }

        function GETCASEIDBYFIELDVALUE($ProcCod, $FieldCod, $FieldValue)
        {
            global $connect;
            $connect = $this->connect;
            $ProcId = PegaProcIdByCode($ProcCod);
            $FieldId = PegaFieldIdByCode($ProcId, $FieldCod);
            $Parametros = "Processo: $ProcCod ($ProcId); Campo: $FieldCod ($FieldId); Valor: $FieldValue";

            $retorno = 0;
            if (empty($ProcId)) {
                $retorno = -1601;
                $MSG = "Falha ao executar a função GETCASEIDBYFIELDVALUE: $retorno - Processo inválido";
                error_log("CaseNum: $this->CaseNum $MSG - Parametros $Parametros", 0);
            }

            if (empty($FieldId)) {
                $retorno = -1602;
                $MSG = "Falha ao executar a função GETCASEIDBYFIELDVALUE: $retorno - Campo inválido";
                error_log("CaseNum: $this->CaseNum $MSG - Parametros $Parametros", 0);
            }

            if ($retorno != 0) {
                $this->EventId = 1000;
                $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
                return $retorno;
            }

            $SQL = "select CaseNum from exportkeys where ProcId = $ProcId and Campo$FieldId = '$FieldValue' order by CaseNum";
            $Query = mysqli_query($this->connect, $SQL);

            if (!$Query) {
                error_log("2476 - SQL: $SQL");
                $MSG = "Falha ao executar a função GETCASEIDBYFIELDVALUE: -1600 - Erro no retorno";
                error_log("CaseNum: $this->CaseNum $MSG - Parametros $Parametros", 0);
                $retorno = -1600;
            }

            if (mysqli_num_rows($Query) == 0) {
                $MSG = "Executada função GETCASEIDBYFIELDVALUE - N�o encontrado Caso - Parametros $Parametros";
                $CaseNum = 0;
            } else {
                $Result = mysqli_fetch_array($Query);
                $CaseNum = $Result["CaseNum"];
                $MSG = "Executada função GETCASEIDBYFIELDVALUE - Encontrado Caso  - ($CaseNum) - Parametros $Parametros";
            }
            $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
            return $CaseNum;
        }

        function IFF($Avaliacao, $ValorTrue, $ValorFalse = '')
        {
            if ($Avaliacao) {
                return $ValorTrue;
            } else {
                return $ValorFalse;
            }
        }

        function GETFIELDVALUE($ProcCod, $CaseNum, $FieldCod)
        {
            global $connect;
            $connect = $this->connect;
            $ProcId = PegaProcIdByCode($ProcCod);
            $FieldId = PegaFieldIdByCode($ProcId, $FieldCod);

            $Parametros = "Processo: $ProcCod ($ProcId); CaseNum: $CaseNum; Campo: $FieldCod ($FieldId)";

            $retorno = 0;
            if (empty($ProcId)) {
                $retorno = -1700;
                $MSG = "Falha ao executar a função GETFIELDVALUE: $retorno - Processo inválido - Parâmetros: $Parametros";
                error_log("Caso: $this->CaseNum $MSG - Parâmetros $Parametros", 0);
            }

            if (empty($FieldId)) {
                $retorno = -1701;
                $MSG = "Falha ao executar a função GETFIELDVALUE: $retorno - Campo inválido - Parâmetros: $Parametros";
                error_log("Caso: $this->CaseNum $MSG - Parâmetros $Parametros", 0);
            }

            if (empty($CaseNum)) {
                $retorno = -1702;
                $MSG = "Falha ao executar a função GETFIELDVALUE: $retorno - CaseNum inválido - Parâmetros: $Parametros";
                error_log("Caso: $this->CaseNum $MSG - Parâmetros $Parametros", 0);
            }

            if (!is_numeric($CaseNum)) {
                $retorno = -1703;
                $MSG = "Falha ao executar a função GETFIELDVALUE: $retorno - CaseNum inválido - Parâmetros: $Parametros";
                error_log("Caso: $this->CaseNum $MSG - Parâmetros $Parametros", 0);
            }

            $this->EventId = 1000;
            if ($retorno != 0) {
                $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
                return '';
            }

            $Valor = PegaValorCampo($ProcId, $CaseNum, $FieldId);
            $MSG = "Executada função GETFIELDVALUE, Parâmetros: $Parametros - Retorno ($Valor)";
            $this->Atualizaaudittrail(0, $this->ConditionId, $MSG);
            return $Valor;
        }

        function CONTENTOBJECTCHECKIFVIEWED($OBJECTIDCODE, $RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE)
        {
            $retorno = $this->OBJECTCHECKIFVIEWED($this->ConditionId, $this->CaseNum, $this->ProcId, $RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $OBJECTIDCODE);
            return $retorno;
        }

        function CONTENTCREATEOBJECT($RMOBJECTSOURCECODE, $RMOBJECTTYPECODE, $RMOBJECTCLASSCODE, $RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $Subst = true)
        {
            $retorno = $this->RMCreateObject($this->connect, $this->CaseNum, $this->ProcId, $this->FieldValuesByCode, $RMOBJECTSOURCECODE, $RMOBJECTTYPECODE, $RMOBJECTCLASSCODE, $RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $Subst);
            return $retorno;
        }

        function CONTENTMODIFYOBJECT($OBJECTIDCODE, $RMOBJECTSOURCECODE, $RMOBJECTTYPECODE, $RMOBJECTCLASSCODE, $RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $SUBST = true)
        {
            $retorno = $this->RMModifyObject($this->connect, $this->CaseNum, $this->ProcId, $this->FieldValuesByCode, $OBJECTIDCODE, $RMOBJECTSOURCECODE, $RMOBJECTTYPECODE, $RMOBJECTCLASSCODE, $RMOBJECTUSERNAMECODE, $RMOBJECTUSERPWDCODE, $SUBST);
            return $retorno;
        }

        function GETFIELDSVALUES()
        {
            $SQL = "select FieldName, FieldCod from procfieldsdef where ProcId = $this->ProcId and ";
            $SQL .= "FieldType not in ('TM', 'FD') order by FieldName";
            $Query = mysqli_query($this->connect, $SQL);
            while ($Linha = mysqli_fetch_array($Query)) {
                $valores .= $Linha["FieldName"] . ": " . "!%" . $Linha["FieldCod"] . "!%" . "\n";
            }
            return $this->ParseValuesFields($valores);
        }

        function formataCaseNum($CaseNum, $NrZeros)
        {
            $Size = strlen($CaseNum);
            $Zeros = "";
            for ($contador = 1; $contador <= ($NrZeros - $Size); $contador++) {
                $Zeros = "0$Zeros";
            }
            $CaseNum = "$Zeros$CaseNum";
            return $CaseNum;
        }

        function Lock($Status)
        {
            global $connect;
            $Date = date("Y-m-d H:i");
            $SQL = "update ";
            $SQL .= " casequeue ";
            $SQL .= " set ";
            if ($Status == 0) {
                $SQL .= " LockedById = 0, ";
                $SQL .= " LockedBysamaccountname = '', ";
                $SQL .= " Lockeddatetime = '', ";
            }
            if ($Status == 3) {
                $SQL .= " LockedById = $this->UserId,";
                $SQL .= " LockedBysamaccountname = '$this->samaccountname',";
                $SQL .= " Lockeddatetime = '$Date', ";
            }
            $SQL .= " AdHoc = $Status ";
            $SQL .= " where ";
            $SQL .= " ProcId = $this->ProcId ";
            $SQL .= " and ";
            $SQL .= " StepId = $this->StepId";
            $SQL .= " and ";
            $SQL .= " CaseId = $this->CaseNum";
            mysqli_query($connect, $SQL);
        }

        function PegaPrioridade()
        {
            $SQL = "select priority from casequeue where CaseId = $this->CaseNum and StepId = $this->StepId and ProcId = $this->ProcId";
            $query = @mysqli_query($this->connect, $SQL);
            $Retorno = @mysqli_fetch_array($query);
            if (!empty($Retorno["priority"])) {
                return $Retorno["priority"];
            } else {
                return 0;
            }
        }

        function AtualizaPrioridade()
        {
            $SQL = " update ";
            $SQL .= " casequeue ";
            $SQL .= " set ";
            $SQL .= " priority = $this->Prioridade ";
            $SQL .= " where ";
            $SQL .= " ProcId = $this->ProcId ";
            $SQL .= " and ";
            $SQL .= " CaseId = $this->CaseNum ";
            $SQL .= " and ";
            $SQL .= " StepId = $this->StepId";
            $Query = mysqli_query($this->connect, $SQL);
            if (!$Query) {
                error_log("Falha Atualiza Prioridade: SQL: $SQL");
            }
            return $Query;
        }

        function SetPrioridade($Prioridade = 0)
        {
            if (empty($Prioridade)) {
                $Prioridade = $this->PegaPrioridade();
            }
            $this->Prioridade = $Prioridade;
            return $this->AtualizaPrioridade();
        }

        function pegaOrigem()
        {
            $SQL = "select Origem from exportkeys where CaseNum = $this->CaseNum ";
            $Query = mysqli_query($this->connect, $SQL);
            $result = mysqli_fetch_array($Query);
            return $result["Origem"];
        }

        /**
         * 
         */
        function RemoverdaFila()
        {
            $this->EventId = 618;
            $this->Atualizaaudittrail();
            $this->RemoveQueue($this->StepId);
            $this->RemoveCriadosDead($this->StepId);
        }

        function RemoverdaFilaViaMacro($StepId)
        {
            $this->EventId = 8011;
            $StepName = $this->PegaNomePasso($StepId);
            if ($this->CasoEstaNaFila($StepId)) {
                $this->EventId = 8010;
                $this->RemoveQueue($StepId);
                $this->RemoveCriadosDead($StepId);
            }
            $this->Atualizaaudittrail(0, 0, $StepName);
        }

        /**
         * Seta o tipo de acao
         * 
         *  0 / processar / commit - Processar
         *  apagar - Apagar Caso
         *  1 / salvar / save -  Salvar o Caso
         *  2 / salvarlockar - Salvar e travar o caso
         *  3 / deadsoft
         *  4 / deadhard
         *  5 / deadhardest
         *  6 / alterfieldvalue
         *  7 / executamacro
         *  8 / saveadmin / salvaradmin
         *  99 / abrir - Abrir o caso
         *  adhoc - Executar adhoc
         *  ingonorar 
         *  putstepadmin
         *  putstep
         * @param type $Action
         */
        function SetAction($Action)
        {
            //error_log("Acao Caso: $this->CaseNum");
            $this->Action = strtolower($Action);
            $this->Date = date("d/m/Y H:i");
            $this->SDate = date("Y-m-d H:i");
            if (!$this->SetPrioridade($this->Prioridade)) {
                error_log("Erro SetPrioridade - Action = $Action");
            }
            //mssql_query("BEGIN TRANSACTION", $this->connect);
            switch ($this->Action) {
                case "0":
                case "commit":
                case "processar":
                case "processarCT":
                    $this->EventId = 2;
                    $this->Atualizaaudittrail();
                    $this->Fields->AtualizaCampos($this->connect, $this->ProcId, $this->CaseNum);
                    if (!$this->NovoCaso) {
                        $this->Origem = $this->PegaOrigem();
                    }
                    $this->ExecutaCondicoes('C');
                    $this->CarregaCamposDaOrigem();
                    $this->AtualizaDeadTimes(0);
                    $this->EncerraCaso();
                    break;

                case "apagar":
                    $this->EventId = 2;
                    $this->Atualizaaudittrail();
                    $this->Fields->SetFieldValue($this->connect, 6, 0);
                    $this->Fields->AtualizaCampos($this->connect, $this->ProcId, $this->CaseNum);
                    $this->AtualizaDeadTimes($this->StepId);
                    break;

                case "1":
                case "salvar":
                case "save":
                    $this->SalvarCaso();
                    break;

                case "99":
                case "abrir":
                case "open":
                    $this->EventId = 20;
                    $this->Atualizaaudittrail();
                    $this->InicializaCamposInternos();
                    $this->AlteraCampo(0, "O");
                    break;

                case "2":
                case "salvarlockar":
                    $this->EventId = 9;
                    $this->Atualizaaudittrail();
                    $this->Fields->AtualizaCampos($this->connect, $this->ProcId, $this->CaseNum);
                    if ($this->LogarAlteracoesFormulario) {
                        $this->EventId = 503;
                        $this->GravaAlteracoesCampo();
                    }
                    $this->CarregaCamposDaOrigem();
                    $this->Lock(3);
                    break;

                case "adhoc":
                    $this->EventId = 3;
                    $this->Atualizaaudittrail();
                    $this->Fields->AtualizaCampos($this->connect, $this->ProcId, $this->CaseNum);
                    break;

                case "ignorar":
                    $this->EventId = 100;
                    $this->Atualizaaudittrail();
                    break;

                case "putstepadmin";
                    if (!CasoCasoEstaNaFila($this->ProcId, $this->StartStepId, $this->CaseNum)) {
                        $this->EventId = 620;
                        $this->InsertDate = $this->PegaInsertDate();
                        $this->Prioridade = 0;
                        $this->StepId = $this->StartStepId;
                        $this->IniciaStep($this->StartStepId, $this->InsertDate, $this->WaitForEvent);
                        $this->Atualizaaudittrail(0, $this->StartStepId);
                    }
                    break;

                case "putstep":
                    $this->EventId = 600;
                    $this->InsertDate = $this->PegaInsertDate();
                    $this->Prioridade = 0;
                    $this->IniciaStep($this->StartStepId, $this->InsertDate, $this->WaitForEvent);
                    break;

                case "3":
                case "deadsoft":
                    $this->ExecutaCondicoes('S');
                    $this->IniciarStep(1, "S", 6);
                    $this->AlteraDeadCond('DeadSoftExec');
                    break;

                case "4":
                case "deadhard":
                    $this->ExecutaCondicoes('H');
                    $this->IniciarStep(1, "H", 6);
                    $this->AlteraDeadCond('DeadHardExec');
                    break;

                case "5":
                case "deadhardest":
                    $this->ExecutaCondicoes('D');
                    $this->IniciarStep(1, "D", 6);
                    $this->AlteraDeadCond('DeadHardestExec');
                    break;

                case "6":
                case "alterfieldvalue":
                    $this->Fields->AtualizaCampos($this->connect, $this->ProcId, $this->CaseNum);
                    $this->EventId = 502;
                    $this->GravaAlteracoesCampo();
                    $this->CarregaCamposDaOrigem();
                    $this->Lock(0);
                    break;

                case "7":
                case "executamacro":
                    $this->EventId = 2000;
                    $this->Atualizaaudittrail();
                    $this->ExecutaCondicoes('C');
                    $this->CarregaCamposDaOrigem();
                    $this->AtualizaDeadTimes(0);
                    $this->EncerraCaso();

                case "8":
                case "salvaradmin":
                case "saveadmin":
                    $this->EventId = 3;
                    $this->Atualizaaudittrail();
                    $this->Fields->AtualizaCampos($this->connect, $this->ProcId, $this->CaseNum);
                    if ($this->LogarAlteracoesFormulario || $this->StepId == 0) {
                        $this->EventId = 503;
                        $this->GravaAlteracoesCampo();
                    }

                    if ($this->MacroAdminEditExec > 0) {
                        $this->StepId = $this->MacroAdminEditExec;
                        $this->EventId = 2001;
                        $this->Atualizaaudittrail();
                        $this->ExecutaCondicoes('C');
                    }

                    $this->CarregaCamposDaOrigem();
                    $this->Lock(0);
                    break;

                case "rotearcaso":
                case "9":
                    $this->RotearCaso();
                    break;
                default:
                    error_log("Sem Acao Definida Caso: $this->CaseNum");
                    break;
            }
            //mssql_query("COMMIT TRANSACTION", $this->connect);	
        }

        function SalvarCaso()
        {
            $this->EventId = 3;
            $this->Atualizaaudittrail();
            $this->Fields->AtualizaCampos($this->connect, $this->ProcId, $this->CaseNum);
            $this->CarregaCamposDaOrigem();
            $this->Lock(0);
        }

        function RotearCaso()
        {
            $PassoInicio = $this->Fields->Field[6]->FieldValue;
            $StatusPadraoInicio = PegaStatusProcessoDefault($this->ProcId, $this->StepId);

//            // Se o passo Padrão é Igual ao Passo Selecionado, apenas salva os dados
//            if ($StatusPadraoInicio == $PassoInicio) {
//                $this->SalvarCaso();
//                $this->GravaAlteracoesCampo();
//                return;
//            }
            $this->Fields->AtualizaCampos($this->connect, $this->ProcId, $this->CaseNum);
            $this->GravaAlteracoesCampo();
            $this->RemoverdaFila();
            $dadosPasso["StartStepId"] = $PassoInicio;
            $dadosPasso["rendezvous"] = false;
            $this->IniciaStepDireto(0, $dadosPasso, 899);
        }

        function TotalCasosAtivos($FieldId)
        {
            global $connect;
            $SQL = "select FieldSourceTable, FieldSourceField from procfieldsdef where ProcId = $this->ProcId and FieldId = $FieldId";
            $Query = mysqli_query($connect, $SQL);
            $Result = mysqli_fetch_array($Query);
            $ProcIdRef = str_replace("''", "'", $Result["FieldSourceTable"]);
            $FieldRef = $Result["FieldSourceField"];
            $SQL = "select count(*) as Ativos from casedata CD, casequeue CQ 
			where 
			CD.ProcId =  $ProcIdRef
			and 
			CQ.ProcId = CD.ProcId
			and 
			FieldId = $FieldRef 
			and 
			FieldValue like '$this->CaseNum'
			and
			CaseId = CaseNum and StepId > 0 ";
            $Query = mysqli_query($connect, $SQL);
            $Result = mysqli_fetch_array($Query);
            return $Result["Ativos"];
        }

        function FuncoesLogica($Func, $FieldId)
        {
            switch ($Func) {
                case 1: {
                        return $this->TotalCasosAtivos($FieldId);
                        break;
                    }
            }
            return false;
        }

        function NovoAvaliacondicoes($Condicoes)
        {
            $BoleanoValues["TRUE"] = "1";
            $BoleanoValues["FALSE"] = "0";
            $BoleanoValues["SIM"] = "1";
            $BoleanoValues["NãO"] = "0";
            $BoleanoValues["1"] = "1";
            $BoleanoValues["2"] = "0";
            $BoleanoValues["Sim"] = "1";
            $BoleanoValues["Não"] = "0";
            $BoleanoValues["NAO"] = "0";
            $Avalia = "";
            $Retorno = true;
            foreach ($Condicoes as $linha) {
                $OpenBracket = $linha["OpenBracket"];
                $CloseBracket = $linha["CloseBracket"];
                $Boleano = $linha["Concat"];
                $Boleano = str_replace("And", " & ", $Boleano);
                $Boleano = str_replace("Or", " | ", $Boleano);
                $Operator = $linha["Operator"];
                if ($Operator == "=") {
                    $Operator = "==";
                }

                $Field1Id = $linha["Field1Id"];
                if ($linha["Func1"] == 0) {
                    $Valor = $this->Fields->Field[$Field1Id]->FieldValueDB;
                } else {
                    $Valor = $this->FuncoesLogica($linha["Func1"], $Field1Id);
                }
                $Field2Id = $linha["Field2Id"];
                if ($Field2Id == 0) {
                    $Valor2 = trim($linha["OpValue"]);
                    if ($this->Fields->Field[$Field1Id]->FieldType == "BO" || $this->Fields->Field[$Field1Id]->FieldType == "RU") {
                        $Valor2 = $BoleanoValues[strtoupper($Valor2)];
                    }
                } else {
                    $Valor2 = $this->Fields->Field[$Field2Id]->FieldValueDB;
                }

                switch ($this->Fields->Field[$Field1Id]->FieldType) {
                    case "US":
                    case "GR":
                    case "NU":
                    case "BO";
                    case "CR":
                    case "RM":
                    case "FM":
                    case "SFR":
                        $Valor = trim($Valor);
                        $Valor2 = trim($Valor2);
                        if (!is_numeric($Valor)) {
                            $Valor = 0;
                        }
                        if (!is_numeric($Valor2)) {
                            $Valor2 = 0;
                        }
                        break;
                    case "LT":
                    case "TX":
                    case "HR":
                    case "DT":
                    case "MC":
                        $Valor = trim($Valor);
                        $Valor2 = trim($Valor2);
                        $Valor = str_replace("/*%/*", "'", $Valor);
                        $Valor = str_replace("\"", "'", $Valor);
                        $Valor2 = str_replace("\"", "'", $Valor2);
                        $Valor = "\"$Valor\"";
                        $Valor2 = "\"$Valor2\"";
                        break;
                }
                $Avalia .= "$Boleano $OpenBracket $Valor $Operator $Valor2 $CloseBracket ";
            }
            if (!eval('$Retorno = (' . $Avalia . "); return true;")) {
                error_log("Avaliacao  - Caso $this->CaseNum, Step $this->StepId Condição \"$this->ConditionName\" Falhou - \"$Avalia\"", 0);
            }
            return $Retorno;
        }

    }

}
    