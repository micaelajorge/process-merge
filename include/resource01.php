<?php

use setasign\Fpdi\Fpdi;

// Correcao 1
// 
// Data: 19/12/2005
// Problema: N�o estava mostrando selecionados campos com acentuacao
// Solucao: Comparado valor do campo com o Valor da Lista antes da Conversao em HTML 
//      Definiçõeo de Funções
// 		CampoReadOnly($Valor, $StiloObjeto)
//		MontaBoleano($Valor, $ReadOnly, $StiloObjeto)
//		MontaCampoTabela($ReadOnly, $StiloObjeto, $Valor, $SourceField, $DisplayField, $TableSource)
//		PegaParametroCSS($ParametrosCSS,$Parametro)
//		PegaColorCSS($ParametrosCSS)
//		MontaEstilo($ParametrosCSS,$campo,$Tipo)
//		PegaNomeProc($ProcId)
//		PegaNomeStep($ProcId, $StepId)
// 		formataCaseNum($CaseNum,$NrZeros)
// 		ConvDate($Data)
// Returns true if $string is valid UTF-8 and false otherwise.
// <editor-fold defaultstate="collapsed" desc="pegaDadosCaso">
function pegaDadosCaso($procId, $caseNum, $camposSelecionados = "")
{
    global $connect;

    if (!is_numeric($procId)) {
        $procId = PegaProcIdByCode($procId);
    }

    $SQL = "select "
            . "fieldCod, "
            . "fieldValue, "
            . "valueId "
            . "from "
            . "casedata, "
            . "procfieldsdef "
            . "where "
            . "caseNum = $caseNum "
            . "and "
            . "casedata.procid = $procId "
            . "and "
            . "procfieldsdef.procid = casedata.procid "
            . "and "
            . "procfieldsdef.fieldid = casedata.fieldid "
            . "and "
            . "FieldSpecial = 0";
    if (is_array($camposSelecionados)) {
        $SQL .= " and fieldcod in ( ";
        $separador = '';
        foreach ($camposSelecionados as $campo) {
            $SQL .= $separador . "'" . $campo . "'";
            $separador = ',';
        }
        $SQL .= ") ";
    }
    $SQL .= " order by fieldCod, valueId ";

    $query = mysqli_query($connect, $SQL);
    $valoresCampos = mysqli_fetch_all($query, MYSQLI_ASSOC);
    $dadosRetorno = array();
    foreach ($valoresCampos as $dadosCampo) {
        $campoCodigo = $dadosCampo["fieldCod"];
        if (key_exists($campoCodigo, $dadosRetorno)) {
            if (!is_array($dadosRetorno[$campoCodigo])) {
                $valorAnterior = $dadosRetorno[$campoCodigo];
                $dadosRetorno[$campoCodigo] = array();
                $dadosRetorno[$campoCodigo][] = $valorAnterior;
            }
            $dadosRetorno[$campoCodigo][] = $dadosCampo["fieldValue"];
        } else {
            $campoValor = $dadosCampo["fieldValue"];
            $dadosRetorno[$campoCodigo] = $campoValor;
        }
    }
    return $dadosRetorno;
}

// </editor-fold>

/**
 * wrap interno para a chamada da função strlen em regras do sistema
 * 
 * @param type $stringRecebida
 * @return type
 */
function w_strlen($stringRecebida)
{
    $retorno = mb_strlen($stringRecebida, 'UTF8');
    return $retorno;
}

/**
 * 
 * @param string $uf
 * @param string $cidade
 * @return string
 */
function buscaComarcaCidade($uf, $cidade)
{
    $curl = curl_init();

    curl_setopt_array($curl, array(
        CURLOPT_URL => "https://registros.certdox.com.br/api/v1/apicaselist",
        CURLOPT_RETURNTRANSFER => true,
        CURLOPT_ENCODING => "",
        CURLOPT_MAXREDIRS => 10,
        CURLOPT_TIMEOUT => 0,
        CURLOPT_FOLLOWLOCATION => true,
        CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
        CURLOPT_CUSTOMREQUEST => "POST",
        CURLOPT_POSTFIELDS => "{\n    \"procCode\": \"CARTORIOS_GATEWAY_REGISTROS\",\n    \"stepCode\": \"START\",\n        \"filters\": [\n        {\n            \"fieldCode\": \"AREA_ABRANGENCIA\",\n            \"fieldValue\": \"$cidade\",\n            \"fieldType\": \"TX\"\n        },\n        {\n            \"fieldCode\": \"ATTRIBUICAO\",\n            \"fieldValue\": \"Protesto\",\n            \"fieldType\": \"TX\"\n        },\n        {\n            \"fieldCode\": \"UF\",\n            \"fieldValue\": \"$uf\",\n            \"fieldType\": \"TX\"\n        }\t\n        ]\n}",
        CURLOPT_HTTPHEADER => array(
            "token: eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiI0MC4xMTQuNDYuODlcL2FsaWNyZWRcL2FwaVwvbWFuYWdlclwvc2FsdmFyZGFkb3N1c3VhcmlvIiwibmFtZSI6ImF1dG9tYXRvIiwiZW1haWwiOiJNTU9TQ1pAR01BSUwuQ09NIn0=.3+sf0o69WCAsIjhId9N7gyF2hjKPvyUfJHNLImttB/E=",
            "Content-Type: application/json",
            "Authorization: eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJsb2NhbGhvc3RcL3Byb2Nlc3NcL2FwaVwvbWFuYWdlclwvc2FsdmFyZGFkb3N1c3VhcmlvIiwibmFtZSI6bnVsbCwiZW1haWwiOiIifQ==.8+fihEnzr3j+kKCQqKf7RY6ghYdE4q77V9BIF2zubIg="
        ),
    ));
    $json_response = curl_exec($curl);
    curl_close($curl);

    $response = json_decode($json_response, true);

    if (json_last_error()) {
        return "";
    }

    $caseNum = $response[0]["CaseNum"];

    $curl = curl_init();

    curl_setopt_array($curl, array(
        CURLOPT_URL => "https://registros.certdox.com.br/api/apigetcasedata/",
        CURLOPT_RETURNTRANSFER => true,
        CURLOPT_ENCODING => "",
        CURLOPT_MAXREDIRS => 10,
        CURLOPT_TIMEOUT => 0,
        CURLOPT_FOLLOWLOCATION => true,
        CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
        CURLOPT_CUSTOMREQUEST => "POST",
        CURLOPT_POSTFIELDS => "{\n\t\"caseNum\": \"$caseNum\",\n\t\"fieldCode\": \"COMARCA\"\n}",
        CURLOPT_HTTPHEADER => array(
            "Content-Type: application/json",
            "token: eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiI0MC4xMTQuNDYuODlcL2FsaWNyZWRcL2FwaVwvbWFuYWdlclwvc2FsdmFyZGFkb3N1c3VhcmlvIiwibmFtZSI6ImF1dG9tYXRvIiwiZW1haWwiOiJNTU9TQ1pAR01BSUwuQ09NIn0=.3+sf0o69WCAsIjhId9N7gyF2hjKPvyUfJHNLImttB/E="
        ),
    ));

    $json_response = curl_exec($curl);
    curl_close($curl);

    $response = json_decode($json_response, true);

    if (json_last_error()) {
        return "";
    }

    curl_close($curl);
    $retorno = $response["Fields"][0]["fieldvalue"];
    return $retorno;
}

function validaData($data)
{
    $aData = explode("-", $data);
    if (!is_array($aData)) {
        return false;
    }

    if (mb_strlen($aData, 'UTF8') < 10) {
        return false;
    }

    if (count($aData) != 3) {
        return false;
    }

    $retorno = checkdate($aData[1], $aData[2], $aData[0]);
    return $retorno;
}

function ibge_busca_cidade($codigoCidade, $uf)
{
    $json_ibge = file_get_contents(FILES_ROOT . "/assets/config/municipios_ibge.min.json");
    $municipios_ibge = json_decode($json_ibge, true);

    if ($municipios_ibge[$codigoCidade]["uf"] != $uf) {
        return false;
    }
    return true;

//    if ($uf === null) {
//        return false;
//    }
//    return $uf;
}

function ibge_busca_uf($codigoCidade)
{
    $json_ibge = file_get_contents(FILES_ROOT . "/assets/config/municipios_ibge.min.json");
    $municipios_ibge = json_decode($json_ibge, true);

    $uf = $municipios_ibge[$codigoCidade]["uf"];

    if ($uf === null) {
        return false;
    }
    return $uf;
}

function cria_dominio_api($novoDominio)
{
    global $connect;
    $sql = "select count(*) as total from origem_dominio where origem_user = '$novoDominio'";
    $query = mysqli_query($connect, $sql);
    $retorno = mysqli_fetch_all($query);

    // Se já existe o dominio sai
    if ($retorno["total"] > 0) {
        return;
    }

    $json_dominiosMaster = file_get_contents(FILES_ROOT . "/assets/config/origem_dominio_automatico.json");
    $dominiosMaster = json_decode($json_dominiosMaster, true);

    // Verifica se existe o servidor
    if (!is_array($dominiosMaster[SRCACCESS])) {
        return;
    }

    // Verifica se está em um alias de servidor
    if (ALIAS_SERVIDOR != "") {
        // Verifica se existe a instância
        if (!is_array($dominiosMaster[SRCACCESS][ALIAS_SERVIDOR]["dominiosMaster"])) {
            return;
        }
        $dominiosMaster = $dominiosMaster[SRCACCESS][ALIAS_SERVIDOR]["dominiosMaster"];
    } else {
        $dominiosMaster = $dominiosMaster[SRCACCESS]["dominiosMaster"];
    }


    // Insere o novo dominio
    $sql = "insert into origem_dominio (origem_user, origem_doc) values ('$novoDominio', '$novoDominio')";
    mysqli_query($connect, $sql);

    // Insere o novo dominio sobre os dominios master
    foreach ($dominiosMaster as $dominioUsuarios) {
        $sql = "insert into origem_dominio (origem_user, origem_doc) values ('$dominioUsuarios', '$novoDominio')";
        mysqli_query($connect, $sql);
    }
}

/**
 * 
 * @global type $connect
 * @return type
 */
function pegaDadosConfig()
{
    global $connect;

    $SQL = "select * from config";
    $Query = mysqli_query($connect, $SQL);

    $retornoConfigs = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $configs = array();
    foreach ($retornoConfigs as $parametro) {
        $configs[$parametro['Funcao']] = $parametro["Valor"];
    }

    return $configs;
}

function validaTokenEnvio($token)
{
    $_password = "ui39%r3##73&DBOA";
}

function tokenUpload($usuario, $email)
{
    $_password = "ui39%r3##73&DBOA";

    $servidor = $_SERVER["HTTP_HOST"];

    if (key_exists("REQUEST_URI", $_SERVER)) {
        $redirectUrl = $_SERVER["REQUEST_URI"];
    } else {
        $redirectUrl = $_SERVER["REDIRECT_SCRIPT_URI"];
    }

    $header = [
        'alg' => 'HS256',
        'typ' => 'JWT'
    ];

    $header = json_encode($header);
    $header = base64_encode($header);

    $payload = [
        'iss' => $servidor . $redirectUrl,
        'name' => $usuario,
        'email' => $email,
        'data_inicio' => date("Y-m-d H:i")
    ];
    $payload = json_encode($payload);
    $payload = base64_encode($payload);

    $signature = hash_hmac('sha256', "$header.$payload", "$_password", true);
    $signature = base64_encode($signature);
}

function calculaToken($usuario, $email, $todosDados = true, $recuparacaoSenha = false)
{
    $_password = "ui39%r3##73&DBOA";

    $servidor = $_SERVER["HTTP_HOST"];

    if (key_exists("REQUEST_URI", $_SERVER)) {
        $redirectUrl = $_SERVER["REQUEST_URI"];
    } else {
        $redirectUrl = $_SERVER["REDIRECT_SCRIPT_URI"];
    }

    $header = [
        'alg' => 'HS256',
        'typ' => 'JWT'
    ];



    $header = json_encode($header);
    $header = base64_encode($header);

    $payload = [
        'iss' => $servidor . $redirectUrl,
        'name' => $usuario,
        'email' => $email,
        'data_inicio' => date("Y-m-d H:i")
    ];

    if ($recuparacaoSenha) {
        $payload["recuparacao_senha"] = true;
    }

    $payload = json_encode($payload);
    $payload = base64_encode($payload);

    $signature = hash_hmac('sha256', "$header.$payload", "$_password", true);
    $signature = base64_encode($signature);

    if ($todosDados) {
        return "$header.$payload.$signature";
    } else {
        return $signature;
    }
}

function validaSessaoUsuario()
{
    global $connect;

    $SessionId = md5(session_id());


    $SQL = "select * from userlogins where sessionid = '$SessionId'";
    $query = mysqli_query($connect, $SQL);

    return $query->num_rows == 0;
}

function validaUsuarioToken()
{
    global $connect, $userdef;
    $headers = getallheaders();
    if (key_exists("token", $headers) || key_exists("Authorization", $headers)) {
        $validadoToken = false;
        $token = (key_exists("token", $headers)) ? $headers["token"] : $headers["Authorization"];
        if ($token != "") {
            $userdef = new userdef;
            $userdef->Create('token', $connect, $token);
            $validadoToken = $userdef->Validado;
            $_SESSION["userdef"] = $userdef;
        }
    } else {
        $validadoToken = false;
    }
    return $validadoToken;
}

function buscaValorCampoUnique($procId, $fields, $fieldUnique)
{
    $valorCampoUnique = false;
    foreach ($fields as $campo) {
        $fieldCode = $campo["fieldCode"];
        $fieldId = PegaFieldIdByCode($procId, $fieldCode);
        if ($fieldId == $fieldUnique) {
            $valorCampoUnique = $campo["fieldValue"];
            break;
        }
    }
    return $valorCampoUnique;
}

function get_guid()
{
    $data = PHP_MAJOR_VERSION < 7 ? openssl_random_pseudo_bytes(16) : random_bytes(16);
    $data[6] = chr(ord($data[6]) & 0x0f | 0x40);    // Set version to 0100
    $data[8] = chr(ord($data[8]) & 0x3f | 0x80);    // Set bits 6-7 to 10
    return vsprintf('%s%s-%s-%s-%s-%s%s%s', str_split(bin2hex($data), 4));
}

function valida_CPF_CNPJ($cpfCNPJ)
{
    $cpf_valido = valida_CPF($cpfCNPJ);
    $cnpj_valido = valida_CNPJ($cpfCNPJ);

    $retorno = $cpf_valido | $cnpj_valido;
    return $retorno;
}

function valida_CNPJ($cnpj = null)
{

    // Verifica se um número foi informado
    if (empty($cnpj)) {
        return false;
    }

    // Elimina possivel mascara
    $cnpj = preg_replace("/[^0-9]/", "", $cnpj);
    $cnpj = str_pad($cnpj, 14, '0', STR_PAD_LEFT);

    // Verifica se o numero de digitos informados é igual a 11 
    if (strlen($cnpj) != 14) {
        return false;
    }

    // Verifica se nenhuma das sequências invalidas abaixo 
    // foi digitada. Caso afirmativo, retorna falso
    else if ($cnpj == '00000000000000' ||
            $cnpj == '11111111111111' ||
            $cnpj == '22222222222222' ||
            $cnpj == '33333333333333' ||
            $cnpj == '44444444444444' ||
            $cnpj == '55555555555555' ||
            $cnpj == '66666666666666' ||
            $cnpj == '77777777777777' ||
            $cnpj == '88888888888888' ||
            $cnpj == '99999999999999') {
        return false;

        // Calcula os digitos verificadores para verificar se o
        // CPF é válido
    } else {

        $j = 5;
        $k = 6;
        $soma1 = "";
        $soma2 = "";

        for ($i = 0; $i < 13; $i++) {

            $j = $j == 1 ? 9 : $j;
            $k = $k == 1 ? 9 : $k;

            $soma2 += ($cnpj{$i} * $k);

            if ($i < 12) {
                $soma1 += ($cnpj{$i} * $j);
            }

            $k--;
            $j--;
        }

        $digito1 = $soma1 % 11 < 2 ? 0 : 11 - $soma1 % 11;
        $digito2 = $soma2 % 11 < 2 ? 0 : 11 - $soma2 % 11;

        return (($cnpj{12} == $digito1) and ( $cnpj{13} == $digito2));
    }
}

function valida_CPF($cpf)
{

// Extrai somente os números
    $cpf = preg_replace('/[^0-9]/is', '', $cpf);

// Verifica se foi informado todos os digitos corretamente
    if (strlen($cpf) != 11) {
        return false;
    }
// Verifica se foi informada uma sequência de digitos repetidos. Ex: 111.111.111-11
    if (preg_match('/(\d)\1{10}/', $cpf)) {
        return false;
    }
// Faz o calculo para validar o CPF
    for ($t = 9; $t < 11; $t++) {
        for ($d = 0, $c = 0; $c < $t; $c++) {
            $d += $cpf{$c} * (($t + 1) - $c);
        }
        $d = ((10 * $d) % 11) % 10;
        if ($cpf{$c} != $d) {
            return false;
        }
    }
    return true;
}

function ContaArquivosDiretorio($diretorio)
{
    if (!is_dir($diretorio)) {
        return 0;
    }
    $files = scandir($diretorio);
    return count($files) - 2;
}

function TestaValidacaoUsuario($AuthUser)
{
    global $connect, $OrigemLogon;
    $userdef = new userdef;
    $userdef->Create($OrigemLogon, $connect, $AuthUser);
    return $userdef;
}

function PassoDefaultView($ProcId, $StepId)
{
    $procDef = $_SESSION['procdef'];
    $listaSteps = $procDef->Steps;
    $objStep = $listaSteps[$StepId];
    return $objStep['DefaultView'];
}

function PegaCamposRepositorio($ProcId)
{
    global $connect;
    $SQL = "SELECT ";
    $SQL .= " procfieldsdef.fieldid, ";
    $SQL .= " fieldname, ";
    $SQL .= " fielddesc, ";
    $SQL .= " fieldtype, ";
    $SQL .= " fieldlength, ";
    $SQL .= " 0 as readonly, ";
    $SQL .= " 1 as optional, ";
    $SQL .= " fieldspecial, ";
    $SQL .= " 0 as OrderId, ";
    $SQL .= " 0 as NewLine, ";
    $SQL .= " FieldSourceTable, ";
    $SQL .= " FieldSourceField, ";
    $SQL .= " FieldDisplayField, ";
    $SQL .= " DefaultValue, ";
    $SQL .= " GroupSource, ";
    $SQL .= " '' as ScriptValida, ";
    $SQL .= " ExtendProp, ";
    $SQL .= " FieldCod, ";
    $SQL .= " '' as CSS, ";
    $SQL .= " 0 as FieldHelp, ";
    $SQL .= " FieldChange,";
    $SQL .= " FieldKeyMaster, ";
    $SQL .= " FieldMaster, ";
    $SQL .= " FieldMask ";
    $SQL .= " from ";
    $SQL .= " procfieldsdef ";
    $SQL .= " where ";
    $SQL .= " procfieldsdef.procid = $ProcId "
            . "and "
            . "fieldspecial <> 1 and "
            . "fieldorigem <> 1 and "
            . "fieldtype in ('TX', 'NU', 'DT')";
    $Query = mysqli_query($connect, $SQL);
    $retorno = mysqli_fetch_all($Query, MYSQLI_ASSOC);
    return $retorno;
}

function PegaListaStatusRotear($ProcId, $StepId)
{
    global $connect;
    $SQL = "select extendprop from procfieldsdef where procid = $ProcId and FieldRazao = $StepId";
    $Query = mysqli_query($connect, $SQL);

    $dadosCampo = mysqli_fetch_array($Query);
    $passosLista = $dadosCampo['extendprop'];

    $SQL = "select stepname, stepid from stepdef where procid = $ProcId and stepid in ($passosLista) order by stepname";
    $Query = mysqli_query($connect, $SQL);
    $retorno = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    return $retorno;
}

function PegaStatusProcessoDefault($ProcId, $StepId)
{
    global $connect;
    $SQL = "select orderdesc from StepDef where ProcId = $ProcId and StepId = $StepId";
    $Query = mysqli_query($connect, $SQL);
    $linha = mysqli_fetch_all($Query, MYSQLI_ASSOC);
    $StatusInicial = $linha[0]['orderdesc'];
    return $StatusInicial;
}

function pegaTotalDocumentos($procId, $caseNum)
{
    global $connect;
    $procIdDocs = pegaRepositorio($procId);
    $campoDocs = 7;
    $SQL = " select "
            . "stepid, "
            . "count(*) as total_documentos "
            . "from "
            . "casequeue "
            . "where "
            . "procid = $procIdDocs and "
            . "stepid > 0 and "
            . "caseid in ( select "
            . "FieldValue "
            . "from "
            . "casedata "
            . "where "
            . "procid = $procId and "
            . "fieldid = $campoDocs and "
            . "casenum = $caseNum ) "
            . "group by stepid ";
    $query = mysqli_query($connect, $SQL);
    return mysqli_fetch_all($query, MYSQLI_ASSOC);
}

function pegaRepositorio($procId)
{
    global $connect;
    $SQL = "select repositorio from procdef where ProcId = $procId ";
    $Query = mysqli_query($connect, $SQL);
    $linhas = mysqli_fetch_all($Query, MYSQLI_ASSOC);
    $retorno = $linhas[0]['repositorio'];
    if (!is_numeric($retorno)) {
        $retorno = PegaProcIdByCode($retorno);
    }
    return $retorno;
}

function pegaPassoDocumentoDefault($ProcId)
{
    global $connect;
    $repositorio = pegaRepositorio($ProcId);

    $SQL = "select 1 as optional, stepdef.* from stepdef where procid = $repositorio and defaultview = 1";
    $Query = mysqli_query($connect, $SQL);
    $docDefaultView = mysqli_fetch_all($Query, MYSQLI_ASSOC);
    return $docDefaultView;
}

function pegaDocumentosProcesso($ProcId, $StepId, $defaultDoc = false)
{
    global $connect;
    $procRepositorio = pegaRepositorio($ProcId);
    $procIdRepositorio = PegaProcIdByCode($procRepositorio);

    $SQL = "select procdocuments.optional, stepdef.* from procdocuments, stepdef where procdocuments.ProcId = $ProcId and procdocuments.StepId = $StepId and stepdef.procid = $procIdRepositorio and stepdef.stepid = stepproc and defaultview = 0 ";
    $SQL .= "order by stepdef.defaultview desc, procdocuments.optional,  stepname";
    $Query = mysqli_query($connect, $SQL);
    $listaDocumentos = mysqli_fetch_all($Query, MYSQLI_ASSOC);
    return $listaDocumentos;
}

function EnviaArquivoBrowser($nomeArquivo, $descricaoArquivo, $dadosArquivo, $tipoArquivo)
{
//   $dadosArquivo = iconv("UTF-8", "ISO-8859-1", $dadosArquivo);
    $dadosArquivo = utf8_decode($dadosArquivo);
    if ($tipoArquivo == "excel") {
//        header('Content-Type: text/html; charset=utf-8');
        header('Content-Type: text/html; charset=iso-8859-1');
        header('Expires: Mon, 26 Jul 1997 05:00:00 GMT');
        header('Last-Modified: ' . gmdate("D,d M YH:i:s") . ' GMT');
        header('Cache-Control: no-cache, must-revalidate');
        header('Pragma: no-cache');
        header('Content-type: application/x-msexcel');
        header('Content-Disposition: attachment; filename="' . $nomeArquivo);
        header("Content-Description: $descricaoArquivo");
    } else {
//header('Content-Type: text/plain; charset=utf-8');
        header('Content-Type: text/plain; charset=iso-8859-1');
        header('Expires: Mon, 26 Jul 1997 05:00:00 GMT');
        header('Last-Modified: ' . gmdate("D,d M YH:i:s") . ' GMT');
        header('Cache-Control: no-cache, must-revalidate');
        header('Pragma: no-cache');
        header('Content-Disposition: attachment; filename="' . $nomeArquivo);
        header("Content-Description: $descricaoArquivo");
    }
    echo $dadosArquivo;
}

function PegaProcFields($ProcId)
{
    global $connect;
    $SQL = "select * from procfieldsdef where ProcId = $ProcId ";
    $Query = mysqli_query($connect, $SQL);
    $Fields = array();
    $Fields = mysqli_fetch_all($Query, MYSQLI_ASSOC);
    $Itens['Itens'] = $Fields;
    $Itens['NumCount'] = count($Fields);
    return $Itens;
}

function PegaTipoPasso($ProcId, $StepId)
{
    global $connect;
    $SQL = "select steptype from stepdef where ProcId = $ProcId and StepId = $StepId";
    $Query = mysqli_query($connect, $SQL);
    $dadosPasso = mysqli_fetch_all($Query, MYSQLI_ASSOC);
    $tipoPasso = $dadosPasso[0]["steptype"];
    return $tipoPasso;
}

function PegaTipoProc($ProcId)
{
    global $connect;
    $SQL = "select tipoproc from procdef where ProcId = $ProcId ";
    $Query = mysqli_query($connect, $SQL);
    $dadosProc = mysqli_fetch_all($Query, MYSQLI_ASSOC);
    $tipoProc = $dadosProc[0]["tipoproc"];
    return $tipoProc;
}

/**
 * 
 * @param type $dadosArquivoRecebido
 * @return type
 */
function PegaConteudoArquivoRecebido($dadosArquivoRecebido)
{
    $aDadosArquivo = explode(",", $dadosArquivoRecebido);
    if (count($aDadosArquivo) > 1) {
        $dadosArquivoBase64 = $aDadosArquivo[1];
    } else {
        $dadosArquivoBase64 = $dadosArquivoRecebido;
    }

// Descodifica o arquivo
    $dadosArquivo = base64_decode($dadosArquivoBase64);
    return $dadosArquivo;
}

function pdfCountPages($fileName)
{
    $ret = array();

    $comando = "pdfinfo $fileName";
    exec($comando, $resultado, $ret);

    if (0 <> $ret) {
        $dados = 'Error ' . $ret;
    } else {
        foreach ($resultado as $item) {
            if (strpos($item, "Pages") !== false) {
                $dadosPage = $item;
                break;
            }
        }
    }

    $retorno = preg_replace('/[^0-9]/', '', $dadosPage);
    return $retorno;
}

/**
 * 
 * @param type $filename
 * @param type $end_directory
 */
function _pdfCountPages($filename)
{
    require_once(FILES_ROOT . '/vendor/lib/fpdf/fpdf.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/FpdiException.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/PdfParserException.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/CrossReference/CrossReferenceException.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfType.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfIndirectObject.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfNull.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfNumeric.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfDictionary.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfToken.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfName.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfIndirectObjectReference.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfArray.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfHexString.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfStream.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfString.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfBoolean.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/StreamReader.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/FpdfTplTrait.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/FpdfTpl.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/FpdiTrait.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfReader/PdfReader.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfReader/PageBoundaries.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfReader/DataStructure/Rectangle.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfReader/Page.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/PdfParser.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Tokenizer.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/CrossReference/CrossReference.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/CrossReference/AbstractReader.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/CrossReference/ReaderInterface.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/CrossReference/FixedReader.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/Fpdi.php');

    $pdf = new FPDI();
    $pagecount = $pdf->setSourceFile($filename); // How many pages?
    return $pagecount;
}

/**
 * 
 * @param type $filename
 * @param type $end_directory
 */
function split_pdf($filename, $end_directory = false)
{
    require_once(FILES_ROOT . '/vendor/lib/fpdf/fpdf.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/FpdiException.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/PdfParserException.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/CrossReference/CrossReferenceException.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfType.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfIndirectObject.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfNull.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfNumeric.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfDictionary.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfToken.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfName.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfIndirectObjectReference.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfArray.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfHexString.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfStream.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfString.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Type/PdfBoolean.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/StreamReader.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/FpdfTplTrait.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/FpdfTpl.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/FpdiTrait.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfReader/PdfReader.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfReader/PageBoundaries.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfReader/DataStructure/Rectangle.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfReader/Page.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/PdfParser.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/Tokenizer.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/CrossReference/CrossReference.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/CrossReference/AbstractReader.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/CrossReference/ReaderInterface.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/PdfParser/CrossReference/FixedReader.php');
    require_once(FILES_ROOT . '/vendor/lib/setasign/Fpdi/Fpdi.php');


    $end_directory != false ? $end_directory : FILES_UPLOAD;
//    $new_path = preg_replace('/[\/]+/', '/', $end_directory . '/' . substr($filename, 0, strrpos($filename, '/')));
// Quarda os nomes dos arquivos por página
    $nomesArquivos = array();

//    if (!is_dir($new_path)) {
//        // Will make directories under end directory that don't exist
//        // Provided that end directory exists and has the right permissions
//        mkdir($new_path, 0777, true);
//    }

    $pdf = new FPDI();
    $pagecount = $pdf->setSourceFile($end_directory . "/" . $filename); // How many pages?
// Split each page into a new PDF
    for ($i = 1; $i <= $pagecount; $i++) {
        $new_pdf = new FPDI();
        $new_pdf->setSourceFile($end_directory . "/" . $filename);
        $templateId = $new_pdf->importPage($i);
        $size = $new_pdf->getTemplateSize($templateId);
        $new_pdf->AddPage($size["orientation"], $size);
        $new_pdf->useTemplate($templateId);
        try {
            $new_filename = $end_directory . str_replace('.pdf', '', $filename) . '_' . $i . ".pdf";
            $nomesArquivos[] = $new_filename;
            $new_pdf->Output($new_filename, "F");
            error_log("Page " . $i . " split into " . $new_filename . "<br />\n");
        } catch (Exception $e) {
            error_log('Caught exception: ', $e->getMessage(), "\n");
            $nomesArquivos = false;
            break;
        }
    }
    if ($nomesArquivos) {
        unlink($end_directory . "/" . $filename);
    }
    return $nomesArquivos;
}

/**
 * 
 * @param type $procId
 * @param type $arquivo
 * @param type $dirName
 * @param type $numeroCaso
 * @param type $nr_Imagem
 * @param type $fieldIdImagem
 * @return boolean
 */
function trataArquivoRecebidoJson($procId, $arquivo, $dirName, $numeroCaso, $nr_Imagem, $fieldIdImagem)
{
// Descodifica o arquivo
    $dadosArquivo = PegaConteudoArquivoRecebido($arquivo["fileData"]);

    $arquivoNomeImagem_JSON = $arquivo["fileName"];

// Extrai a extenção do Arquivo
    $extencaoArquivo = pathinfo($arquivoNomeImagem_JSON, PATHINFO_EXTENSION);

    $arquivoNomeImagem_STORAGE = criaNomeArquivoUpload($numeroCaso, $nr_Imagem, $fieldIdImagem, $extencaoArquivo);
    $arquivoNomeImagem_DISPLAY = $arquivoNomeImagem_JSON;
    if ($arquivoNomeImagem_JSON == "") {
        $arquivoNomeImagem_DISPLAY = $arquivoNomeImagem_STORAGE;
    }
    $extendData = criaExtendDataCampoArquivo($arquivoNomeImagem_STORAGE, $arquivoNomeImagem_DISPLAY, '', 0);

//error_log("NomeArquivoDestino: $nomeDestionImagem");
    atualizaTOCdoCaso($dirName, $procId, $numeroCaso, $fieldIdImagem, $extendData);

// Grava o Arquivo no Diretorio
    $nomeDestinoImagem = $dirName . "/" . $arquivoNomeImagem_STORAGE;
    if (file_put_contents($nomeDestinoImagem, $dadosArquivo, FILE_BINARY) === FALSE) {
        return false;
    }
    return $extendData;
}

function trataArquivosRecebidoPost()
{
    $keys = array_keys($_FILES);
//    $arquivo = $_FILES["arquivoUpload"];
    $arquivo = $_FILES[$keys[0]];
    $arquivoEnviado["filename"] = $arquivo["name"];
    $arquivoEnviado["filetempname"] = $arquivo["tmp_name"];
    $arquivos[] = $arquivoEnviado;
    return $arquivos;
}

/**
 * 
 * @param type $procId
 * @param type $arquivo
 * @param type $dirName
 * @param type $numeroCaso
 * @param type $nr_Imagem
 * @param type $fieldIdImagem
 * @return boolean
 */
function trataArquivoRecebidoPost($procId, $arquivo, $dirName, $numeroCaso, $nr_Imagem, $fieldIdImagem)
{

    if (isset($arquivo["filename"])) {
        $fileName = (!is_array($arquivo["filename"])) ? $arquivo["filename"] : $arquivo["filename"][0];
    } else {
        $DirOrigem = pathinfo($arquivo["name"][0], PATHINFO_DIRNAME);
        $nomeArquivoOrigem = pathinfo($arquivo["name"][0], PATHINFO_BASENAME);

//error_log("Path: $DirOrigem, $nomeArquivoOrigem " );
        $fileName = $nomeArquivoOrigem;
    }
    $arquivoNomeImagem_JSON = $fileName;

// Extrai a extenção do Arquivo
    $extencaoArquivo = pathinfo($arquivoNomeImagem_JSON, PATHINFO_EXTENSION);

    $arquivoNomeImagem_STORAGE = criaNomeArquivoUpload($numeroCaso, $nr_Imagem, $fieldIdImagem, $extencaoArquivo);
    $arquivoNomeImagem_DISPLAY = $arquivoNomeImagem_JSON;
    if ($arquivoNomeImagem_JSON == "") {
        $arquivoNomeImagem_DISPLAY = $arquivoNomeImagem_STORAGE;
    }
    $extendData = criaExtendDataCampoArquivo($arquivoNomeImagem_STORAGE, $arquivoNomeImagem_DISPLAY, '', 0);

//error_log("NomeArquivoDestino: $nomeDestionImagem");
    atualizaTOCdoCaso($dirName, $procId, $numeroCaso, $fieldIdImagem, $extendData);


    if (isset($arquivo["tmp_name"]) | isset($arquivo["filetempname"])) {
        $nomeDestinoImagem = $dirName . "/" . $arquivoNomeImagem_STORAGE;

        if (isset($arquivo["tmp_name"])) {
            $nomeKeyTemp = "tmp_name";
        } else {
            $nomeKeyTemp = "filetempname";
        }


        $fileNameTemp = (!is_array($arquivo[$nomeKeyTemp])) ? $arquivo[$nomeKeyTemp] : $arquivo[$nomeKeyTemp][0];
        $resultado = move_uploaded_file($fileNameTemp, $nomeDestinoImagem);
    } else {

// $nameOrigem = $DirOrigem . "/" . $nomeArquivoOrigem;
        $nameOrigem = $DirOrigem . "/" . $fileName;
        $nameDestino = $dirName . "/" . $arquivoNomeImagem_STORAGE;
        $resultado = copy($nameOrigem, $nameDestino);
    }
    if (!$resultado) {
        error_log("Falha '$nameOrigem' '$nameDestino' ");
        error_log(var_export(get_defined_vars(), true));
    }

    return $extendData;
}

function trataPdfRecebido($nomeArquivoPdf, $conteudoPdf, $dirName)
{
    $listaPaginasPdf = array();
    $dadosArquivo = PegaConteudoArquivoRecebido($conteudoPdf);
    if (file_put_contents($dirName . "/" . $nomeArquivoPdf, $dadosArquivo) === FALSE) {
        return false;
    }
    try {
        $paginasDoPdf = split_pdf($nomeArquivoPdf, $dirName);
    } catch (Exception $ex) {
        header('HTTP/1.0 500 splitting read PDF');
        error_log("Falha WebService " . var_export($ex, true));
        die();
    }
    foreach ($paginasDoPdf as $paginaDoPdf) {
        $pagina["fileName"] = basename($paginaDoPdf);
        $pagina["fileData"] = base64_encode(file_get_contents($paginaDoPdf));
        unlink($paginaDoPdf);
        $listaPaginasPdf[] = $pagina;
    }
    return $listaPaginasPdf;
}

/**
 * 
 * @param type $arquivosRecebidos
 * @param type $dirName
 * @return type
 */
function quebraPaginasArquivosPdf($arquivosRecebidos, $dirName)
{
//return $arquivosRecebidos;    
    $listaArquivosFinal = array();
    $nrPdf = 1;


    foreach ($arquivosRecebidos as $arquivo) {
        $arquivoNomeImagem_JSON = strtolower($arquivo["fileName"]);
// Extrai a extenção do Arquivo
        $extencaoArquivo = pathinfo($arquivoNomeImagem_JSON, PATHINFO_EXTENSION);
        if ($extencaoArquivo == "pdf") {
            $listaPaginasPdf = trataPdfRecebido($nrPdf . "_" . $arquivoNomeImagem_JSON, $arquivo["fileData"], $dirName);
            $listaArquivosFinal = $listaPaginasPdf;
        } else {
            $listaArquivosFinal[] = $arquivo;
        }
    }
    return $listaArquivosFinal;
}

/**
 * 
 * @param type $arquivos
 * @param type $procId
 * @param type $numeroCaso
 * @return boolean|array
 */
function criarArquivosEmCampoFolder($arquivosRecebidos, $procId, $numeroCaso, $codigoCampo, $quebrarPDF = false)
{
    $fieldIdImagem = PegaFieldIdByCode($procId, $codigoCampo);
    $nr_Imagem = contaNumeroArquivos($procId, $numeroCaso, $fieldIdImagem) + 1;
    $retorno = array();
    $aImagens = array();

// Cria o diretorio de armazenamento
    $dirName = criaDiretorioCampoArquivo($procId, $numeroCaso, $fieldIdImagem);

// Verifica os Arquivos Recebidos e trata arquivos PDF com multiplas páginas
    if ($quebrarPDF) {
        $arquivosRecebidos = quebraPaginasArquivosPdf($arquivosRecebidos, $dirName);
    }

    foreach ($arquivosRecebidos as $arquivo) {
        if (isset($arquivo["fileData"])) {
            $aImagens[] = trataArquivoRecebidoJson($procId, $arquivo, $dirName, $numeroCaso, $nr_Imagem, $fieldIdImagem);
        } else {
            $aImagens[] = trataArquivoRecebidoPost($procId, $arquivo, $dirName, $numeroCaso, $nr_Imagem, $fieldIdImagem);
        }
        $nr_Imagem++;
    }
    $retorno[0]["fieldCode"] = $codigoCampo;
    $retorno[0]["fieldValue"] = $aImagens;

//error_log("Arquivo Recebido: " . var_export($retorno, true));
    return $retorno;
}

/**
 * 
 * @param type $arquivos
 * @param type $procId
 * @param type $numeroCaso
 * @return boolean|array
 */
function criarArquivosEmCampo($procId, $numeroCaso, $campos)
{
    $retorno = array();
    $aImagens = array();
    foreach ($campos as $campo) {


        $fieldIdImagem = PegaFieldIdByCode($procId, $campo["fieldCode"]);

        $nr_Imagem = contaNumeroArquivos($procId, $numeroCaso, $fieldIdImagem) + 1;

// Cria o diretorio de armazenamento
        $dirName = criaDiretorioCampoArquivo($procId, $numeroCaso, $fieldIdImagem);

        $aImagens = array();
        foreach ($campo["fieldValue"] as $arquivo) {
            if (isset($arquivo["fileData"])) {
                $aImagens[] = trataArquivoRecebidoJson($procId, $arquivo, $dirName, $numeroCaso, $nr_Imagem, $fieldIdImagem);
            } else {
                $aImagens[] = trataArquivoRecebidoPost($procId, $arquivo, $dirName, $numeroCaso, $nr_Imagem, $fieldIdImagem);
            }
            $nr_Imagem++;
        }
        $dadosRetorno["fieldCode"] = $campo["fieldCode"];
        $dadosRetorno["fieldValue"] = $aImagens;
        $retorno[] = $dadosRetorno;
    }
    return $retorno;
}

/**
 * 
 * @global type $connect
 * @param type $procId
 * @param type $numeroCaso
 * @param type $fieldId
 */
function contaNumeroArquivos($procId, $numeroCaso, $fieldId)
{
    global $connect;
    $SQL = "select count(*) as totalRegistros from casedata where ProcId = $procId and casenum = $numeroCaso and FieldId = $fieldId";
    $Query = mysqli_query($connect, $SQL);
    $linha = mysqli_fetch_array($Query);
    return $linha["totalRegistros"];
}

/**
 * 
 * @param type $dados
 * @param string $nomeArray
 * @return type
 */
function trataCamposEmArray($dados, $nomeArray = "")
{
    $retorno = array();
    if ($nomeArray != "") {
        $nomeArray = $nomeArray . ".";
    }
    foreach ($dados as $nomeCampo => $valorCampo) {
        if (is_array($valorCampo)) {
            $dadosArray = trataCamposEmArray($valorCampo, $nomeCampo);
            $retorno = array_merge($retorno, $dadosArray);
        } else {
            $campo["fieldCode"] = $nomeArray . $nomeCampo;
            $campo["fieldValue"] = $valorCampo;
            array_push($retorno, $campo);
        }
    }
    return $retorno;
}

function PegaCampoUniqueClose($procId)
{
    global $connect;
    $SQL = "select "
            . "FieldCaseClosed, "
            . "FieldUnique, "
            . "FieldGrouping "
            . "from "
            . "procdef "
            . "where "
            . "ProcId = $procId";
    $Query = mysqli_query($connect, $SQL);
    $linha = mysqli_fetch_array($Query);
    return $linha;
}

function PegaExtendsPropProc($ProcId)
{
    global $connect;
    $SQL = "select extendPropsProc from procdef where procid = $ProcId";
    $Query = mysqli_query($connect, $SQL);
    $linha = msyqli_fetch_array($Query);
    $retorno = array();
    if ($linha != "") {
        $retorno = json_decode($linha["extedPropsProc"], true);
    }
    return $retorno;
}

/**
 * 
 * @global type $connect
 * @param type $procId
 * @param type $valorCampo
 * @param type $fieldUnique
 * @param type $fieldCaseClosed
 * @return int
 */
function pegaNumerCasoUnicoAberto($procId, $valorCampo, $fieldUnique = "", $fieldCaseClosed = "", $failDuplicate = false)
{
    global $connect;
// Pesquisa o NR do Case UNIQUE

    if (!is_numeric($procId)) {
        $procId = PegaProcIdByCode($procId);
    }

    if ($fieldUnique == "") {
        $retorno = PegaCampoUniqueClose($procId);
        $fieldCaseClosed = $retorno["FieldCaseClosed"];
        $fieldUnique = $retorno["FieldUnique"];
    }

    $camposProcesso = PegaFieldIdsByCode($procId);

    if ($fieldCaseClosed != "") {
// Pega Id Campo CaseClosed
        if (!is_numeric($fieldCaseClosed)) {
            $indiceFieldCaseClosed = array_search($fieldCaseClosed, array_column($camposProcesso, 'FieldCod'));
            $idFieldCaseClosed = $camposProcesso[$indiceFieldCaseClosed]["FieldId"];
        } else {
            $idFieldCaseClosed = $fieldCaseClosed;
        }
    } else {
        $idFieldCaseClosed = 0;
    }

    // Pega Id Campo CaseUnique
    if (!is_numeric($fieldUnique)) {
        $indiceFieldUnique = array_search($fieldUnique, array_column($camposProcesso, 'FieldCod'));
        $idFieldUnique = $camposProcesso[$indiceFieldUnique]["FieldId"];
    } else {
        $idFieldUnique = $fieldUnique;
    }


// Se não tem campo para caso fechado
    if ($idFieldCaseClosed != 0) {
        $SQL = "select "
                . " CaseNum"
                . " from "
                . " exportkeys "
                . " where "
                . " ProcId = $procId "
                . " and "
                . " ( "
                . " Campo$idFieldCaseClosed = '0' "
                . " or "
                . " Campo$idFieldCaseClosed = '' "
                . " or "
                . " Campo$idFieldCaseClosed is null "
                . " )"
                . " and "
                . " Campo$idFieldUnique = '$valorCampo'";
    } else {
        $SQL = "select "
                . " CaseNum"
                . " from "
                . " exportkeys "
                . " where "
                . " ProcId = $procId "
                . " and "
                . " Campo$idFieldUnique = '$valorCampo'";
    }
    $Query = mysqli_query($connect, $SQL);

    $totalEncontrados = mysqli_num_rows($Query);
    if ($totalEncontrados > 0) {
        // Retorna -1 se houver mais de um caso
        if ($failDuplicate & $totalEncontrados > 1) {
            $numeroCaso = -1;
        }
        $Linha = mysqli_fetch_array($Query);
        $numeroCaso = $Linha["CaseNum"];
    } else {
        $numeroCaso = 0;
    }
    return $numeroCaso;
}

/**
 * 
 * @global type $connect
 * @param type $procCode
 * @return type
 */
function pegaCampoCasoFechado($procCode)
{
    global $connect;
    if (!is_int($procCode)) {
        $procId = PegaProcessoByCod($procCode);
    } else {
        $procId = $procCode;
    }
    $SQL = "select "
            . "FieldCaseClosed "
            . " from procdef"
            . " where "
            . " ProcId = $procId";
    $Query = mysqli_query($connect, $SQL);
    $linha = mysqli_fetch_array($Query);
    $fieldCaseClosed = $linha["FieldCaseClosed"];
    return $fieldCaseClosed;
}

/**
 * 
 * @param type $urlWebservice
 * @param type $dadosEnvio - json
 * @param type $metodo
 * @param type $adicionalHeaders - formato key: valor Ex 'X-Signature: ASSSINATURA'
 * @param type $retornoJson = true / false
 * @return type
 */
function executaWebServiceCURL($urlWebservice, $dadosEnvio, $metodo, $adicionalHeaders = array(), $retonoJson = true, $headerJson = true)
{
    $headerPadrao = array();
    if ($headerJson) {
        $headerPadrao = array(
            'Content-Type: application/json',
            'Content-Length: ' . strlen($dadosEnvio)
        );
    }
    if (is_array($adicionalHeaders)) {
        $headerEnvio = array_merge($headerPadrao, $adicionalHeaders);
    }

    $ch = curl_init($urlWebservice);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, $metodo);

    (LOG_DATA) ? error_log("WebService Dados Envio:" . var_export($dadosEnvio, true)) : null;

    if ($dadosEnvio != null) {
        curl_setopt($ch, CURLOPT_POSTFIELDS, $dadosEnvio);
    }
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_HTTPHEADER, $headerEnvio);
    curl_setopt($ch, CURLOPT_TIMEOUT, 10);
    $resultadoChamada = curl_exec($ch);
    $lastErrorCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);

// Se o resultado da chamada for 200 então está OK.
    if ($lastErrorCode == 200) {
        $retornoOk["return"] = "ok";
// Retorna o Valor como um JSON para a função
        if ($retonoJson) {
            $resultado = json_encode($retornoOk);
        } else {
            $resultado = $resultadoChamada;
        }
    } else {
        $retornoErro["erroChamada"] = curl_error($ch) . " - " . $lastErrorCode;
        error_log("Erro Chamada " . curl_error($ch) . " - " . $lastErrorCode);
        error_log("Erro Chamada $urlWebservice " . var_export($resultadoChamada, true));
        $resultado = $retornoErro;
    }
    return $resultado;
}

function HumanSize($Bytes)
{
    $Type = array("", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB");
    $Index = 0;
    while ($Bytes >= 1024) {
        $Bytes /= 1024;
        $Index++;
    }
    $Bytes = round($Bytes, 2);
    return("" . $Bytes . " " . $Type[$Index]);
}

function diretorioArquivoImagem($procId, $caseNum, $fieldId)
{
    if (!is_numeric($procId)) {
        $procId = PegaProcIdByCode($procId);
    }

    if (!is_numeric($fieldId)) {
        $fieldId = PegaFieldIdByCode($procId, $fieldId);
    }

    return formataCaseNum($procId, 6) . "/" . formataCaseNum($caseNum, 10) . "/" . formataCaseNum($fieldId, 6) . "/";
}

function PegaFieldCode($ProcId, $FieldId)
{
    global $connect;
    $SQL = "select fieldcod from procfieldsdef where procid = $ProcId and fieldId = $FieldId";
    $Query = mysqli_query($connect, $SQL);
    $resultado = mysqli_fetch_array($Query);
    return $resultado["fieldcod"];
}

function PegaStepCode($ProcId, $StepId)
{
    global $connect;
    $SQL = "select StepCod from stepdef where procid = $ProcId and StepId = $StepId";
    $Query = mysqli_query($connect, $SQL);
    $resultado = mysqli_fetch_array($Query);
    return $resultado["StepCod"];
}

function PegaProcCode($ProcId)
{
    global $connect;
    $SQL = "select proccod from procdef where procid = $ProcId";
    $Query = mysqli_query($connect, $SQL);
    $resultado = mysqli_fetch_array($Query);
    return $resultado["proccod"];
}

/**
 * 
 * @param type $diretorio
 * @param type $ProcId
 * @param type $CaseNum
 * @param type $FieldId
 * @param type $dadosArquivo
 */
function atualizaTOCdoCaso($diretorio, $ProcId, $CaseNum, $FieldId, $extendData)
{
    $nomeArquivoTOC = $diretorio . "/toc.json";
    $ProcCode = PegaProcCode($ProcId);
    $fieldCod = PegaFieldCode($ProcId, $FieldId);
    if (file_exists($nomeArquivoTOC)) {
        $jsonArquivoTOC = file_get_contents($nomeArquivoTOC);
        $aArquivoTOC = json_decode($jsonArquivoTOC, true);
    } else {
        $aArquivoTOC = array();
        $aArquivoTOC["proccode"] = $ProcCode;
        $aArquivoTOC["casenum"] = $CaseNum;
    }
    if (!key_exists($fieldCod, $aArquivoTOC)) {
        $aArquivoTOC[$fieldCod] = array();
    }

    if (is_array($extendData)) {
        $dadosArquivo = $extendData;
    } else {
        $dadosArquivo = json_decode($extendData);
    }
    array_push($aArquivoTOC[$fieldCod], $dadosArquivo);
    $jsonArquivoTOC = json_encode($aArquivoTOC);
    file_put_contents($nomeArquivoTOC, $jsonArquivoTOC);
}

function adicionaCampoArray($_PARAMETROS)
{
    include_once(FILES_ROOT . "/pages/editcase_campos.inc");

    $fieldId = "";
    $fieldType = "";
    while (list($_NCampo, $_NValor) = each($_PARAMETROS)) {
        $$_NCampo = $_NValor;
    }

    $retorno = montaCampoTX($fieldId, "", "", "", true);
    return $retorno;
}

if (!function_exists('session_register')) {

    function session_register($name)
    {
        global $$name;
        if (!isset($_SESSION[$name])) {
            $_SESSION[$name] = $$name;
        }
        $$name = &$_SESSION[$name];
    }

}

function PegaProcIdCaso($caseNum)
{
    global $connect;
    $SQL = "select ProcId from casequeue where StepId = 0 and CaseId = '$caseNum'";
    $Query = mysqli_query($connect, $SQL);
    $Linha = mysqli_fetch_array($Query);
    $retorno = $Linha["ProcId"];
    return $retorno;
}

if (!function_exists('session_unregister')) {

    function session_unregister($session)
    {
        unset($_SESSION[$session]);
    }

}

function PegaPassoDefaultView($ProcId)
{
    global $connect;
    $SQL = "select StepId from stepdef where ProcId = $ProcId and DefaultView = 1";
    $Query = mysqli_query($connect, $SQL);
    $Linha = mysqli_fetch_array($Query);
    return $Linha["StepId"];
}

function mssql_query($connect, $SQL)
{
    $retorno = mysqli_query($connect, $SQL);
    return $retorno;
}

function mssql_fetch_array($Query)
{
    return mysqli_fetch_array($Query);
}

function mssql_num_rows($Query)
{
    return mysqli_num_rows($Query);
}

function ArquivoPermitido($filename)
{
    return true;
    $ExtensoesProibidas = array('EXE', 'COM', 'BAT', 'JS', 'ASP', 'PHP', 'INC');
    if (in_array(findexts($filename), $ExtensoesProibidas)) {
        return false;
    }
}

function findexts($filename)
{
    $filename = strtolower($filename);
    $exts = explode("[/\\.]", $filename);
    $n = count($exts) - 1;
    $exts = $exts[$n];
    return $exts;
}

function PegaPassosCasoAdmin($ProcId, $CaseNum)
{
    global $connect;
    $SQL = "select StepId from casequeue where ProcId = $ProcId and CaseId = $CaseNum order by StepName ";
    $Query = mysqli_query($connect, $SQL);
    $Passos = array();
    while ($Linha = mysqli_fetch_array($Query)) {
        array_push($Passos, $Linha["StepId"]);
    }
    return $Passos;
}

function AlteraOrigemCaso($Origem, $CaseNum, $Anterior)
{
    global $ProcId, $userdef, $connect;

// Altera ExportKey	
    $Origem = trim($Origem);
    $SQL = "update exportkeys set Origem = '$Origem' where CaseNum = $CaseNum";
    mysqli_query($connect, $SQL);
    Enteraudittrail($ProcId, $CaseNum, 0, 619, "Origem anterior $Anterior, nova origem $Origem");
// Altera casedata
    $SQL = "select FieldId, FieldKey from procfieldsdef where ProcId = $ProcId and FieldOrigem = 1";
    $Query = mysqli_query($connect, $SQL);
    $Result = mysqli_fetch_array($Query);
    $FieldId = $Result["FieldId"];
    $FieldKey = $Result["FieldKey"];
    AtualizaExportKey($CaseNum, $FieldId, $Origem, $Origem);

    $valores["Fields"][0]["FieldId"] = $FieldId;
    $valores["Fields"][0]["Value"] = $Origem;

    $Caso = new STEPDOC;
    $Caso->SetConnection($connect);
    $Caso->SetProc($ProcId);
    $Caso->SetStep(0);
    $Caso->open();
    $Caso->UserId = $userdef->UserId;
    $Caso->UserName = $userdef->UserName;
    $Caso->UserDesc = $userdef->UserDesc;
    $Caso->samaccountname = $this->UserName;
    $Caso->SetCaseNum($CaseNum);
    $Caso->PegaDadosDeArray($valores);
    $Caso->SetAction(6);
}

function PegaMacrosProcesso($ProcId)
{
    global $connect;
    $SQL = "select StepId, StepName, StepCod from stepdef where ProcId = $ProcId and StepType = 'M'";
    $Macros = array();
    $Query = mysqli_query($connect, $SQL);
    while ($Result = mysqli_fetch_array($Query)) {
        array_push($Macros, $Result);
    }
    return $Macros;
}

function PegaCamposPasso($ProcId, $StepId)
{
    global $connect;
    $Campos = array();
    $SQL = "select FieldId from stepfieldsdef where ProcId = $ProcId and StepId = $StepId";
    $Query = mysqli_query($connect, $SQL);
    while ($Linha = mysqli_fetch_array($Query)) {
        array_push($Campos, $Linha["FieldId"]);
    }
    return $Campos;
}

function move_file($Arquivo, $DirOrigem, $ArquivoDestino, $DirDestino)
{
    $ArquivoOrigem = $DirOrigem . $Arquivo;
    $ArquivoDestino = $DirDestino . $ArquivoDestino;
    if (file_exists($DirOrigem . $Arquivo)) {
        copy($ArquivoOrigem, $ArquivoDestino);
    }
    unlink($ArquivoOrigem);
}

function PegaNumeroColunasForm($ProcId, $StepId)
{
    global $connect;
    $SQL = "select NumCols from stepdef where StepId = $StepId and ProcId = $ProcId";
    $Query = mysqli_query($connect, $SQL);
    $Result = mysqli_fetch_array($Query);
    return $Result["NumCols"];
}

/**
 * 
 * @global type $connect
 * @param type $ProcId
 * @param type $CaseNum
 * @param type $Complex
 * @param type $StepId
 * @return string
 */
function PegaListaNomeUsuariosDoCaso($ProcId, $CaseNum, $Complex = false, $StepId = '')
{
    global $connect;
    $SQL = "select GroupId, GrpFld from casequeue, stepaddresses where stepaddresses.ProcId = $ProcId and stepaddresses.StepId = casequeue.StepId and casequeue.CaseId = $CaseNum ";
    if (empty($StepId)) {
        $SQL .= " and casequeue.StepId > 0";
    } else {
        $SQL .= " and casequeue.StepId = $StepId";
    }

    $Query = mysqli_query($connect, $SQL);
    $Groups = array();
    $Users = array();
    while ($Result = mysqli_fetch_array($Query)) {
        $GrpFld = $Result["GrpFld"];
        $GroupId = $Result["GroupId"];
        if ($GrpFld == 'F') {
            /* @var $GroupId type int */
            /* @var $CaseNum type int */
            /* @var $ProcId type int */
            $Valor = PegaValorCampo($ProcId, $CaseNum, $GroupId);
            if (is_numeric($Valor)) {
                $GrpFld = PegaTipoCampo($ProcId, $GroupId);
                $GroupId = $Valor;
            } else {
                continue;
            }
        }
        if ($GrpFld == 'US' || $GrpFld == 'U') {
            array_push($Users, $GroupId);
        }
        if ($GrpFld == 'GR' || $GrpFld == 'G') {
            array_push($Groups, $GroupId);
        }
    }
    $Origem = PegaOrigemCaso($CaseNum);
    $Dominio = PegaListaDominio($Origem);

    if (count($Users) > 0 || count($Groups) > 0) {
        return PegaListaUsuarios($Groups, $Users, $Dominio, $Complex);
    } else {
        return "";
    }
}

// <editor-fold defaultstate="collapsed" desc="Lista de Usu�rios">
function PegaListaUsuarios($Grupos, $Usuarios, $Dominio, $Complex = false)
{
    global $OrigemLogon, $connect, $EXTERNAL_USERFULLNAME, $EXTERNAL_USERID, $EXTERNAL_userdef, $EXTERNAL_USERNAME;
    $Grupos = implode(",", $Grupos);
    $Usuarios = implode(",", $Usuarios);
    if (count($Dominio) == 0) {
        return "N�o usu�rios com acesso a Origem";
    }
    for ($i = 0; $i < count($Dominio); $i++) {
        $Dominio[$i] = "'" . $Dominio[$i] . "'";
    }
    $Dominio = implode(", ", $Dominio);
    $Concat = "";
    switch ($OrigemLogon) {
        case "External":
            $SQL = "select $EXTERNAL_USERFULLNAME, $EXTERNAL_USERNAME from $EXTERNAL_userdef where (";
            if (!empty($Grupos)) {
                $SQL .= " $EXTERNAL_USERID in (select $EXTERNAL_USERID 
							from UsersGroups where GroupId in ($Grupos))";
                $Concat = " or ";
            }
            if (!empty($Usuarios)) {
                $SQL .= $Concat;
                $SQL .= " UserId in ($Usuarios)";
            }
            $AcessoDB = AtivaDBExterno(true);
            break;
        case "Process":
            $SQL = "select $EXTERNAL_USERFULLNAME, $EXTERNAL_USERNAME from $EXTERNAL_userdef where (";
            if (!empty($Grupos)) {
                $SQL .= " $EXTERNAL_USERID in (select $EXTERNAL_USERID
							 from UsersGroups where GroupId in ($Grupos))";
                $Concat = " or ";
            }
            if (!empty($Usuarios)) {
                $SQL .= $Concat;
                $SQL .= " UserId in ($Usuarios)";
            }
            $AcessoDB = $connect;
            break;
    }
    $SQL .= " ) and Origem in ($Dominio)";
    $SQL .= " order by $EXTERNAL_USERFULLNAME";
    $Nomes = array();
    $Query = mysqli_query($AcessoDB, $SQL);
    while ($Result = mysqli_fetch_array($Query)) {
        if ($Complex) {
            $Usuario["UserName"] = $Result[$EXTERNAL_USERNAME];
            $Usuario["UserFullName"] = $Result[$EXTERNAL_USERFULLNAME];
        } else {
            $Usuario = $Result[$EXTERNAL_USERNAME] . " (" . $Result[$EXTERNAL_USERFULLNAME] . ")";
        }
        array_push($Nomes, $Usuario);
    }
    return $Nomes;
}

// </editor-fold>
function PegaDisplayCampoRef($ProcId, $CaseNum, $FieldRef)
{
    global $connect;
    if (!is_numeric($FieldRef)) {
        $FieldRef = PegaFieldIdByCode($ProcId, $FieldRef);
    }
    $SQL = "select CampoDisplay$FieldRef from exportkeysdisplay where ProcId = $ProcId and CaseNum = $CaseNum";
    $Query = mysqli_query($connect, $SQL);
    $Result = mysqli_fetch_array($Query);
    return $Result["CampoDisplay$FieldRef"];
}

function FormataMascaraTX($Valor, $Mascara)
{
    if ($Mascara == "CPF/CNPJ") {
        $ValorFormatado = formatarCPF_CNPJ($Valor);
        return $ValorFormatado;
    }
}

function _VALIDACPF($cpf)
{
    /*
     */
    $nulos = array("12345678909", "11111111111", "22222222222", "33333333333",
        "44444444444", "55555555555", "66666666666", "77777777777",
        "88888888888", "99999999999", "00000000000");
    /* Retira todos os caracteres que nao sejam 0-9 */

    $limpesa = "";
    $cpf = preg_match("[^0-9]", $cpf, $limpesa);

    /* Retorna falso se houver letras no cpf */
    if (!(preg_match("[0-9]", $cpf))) {
        return false;
    }

    /* Retorna falso se o cpf for nulo */
    if (in_array($cpf, $nulos)) {
        return false;
    }

    /* Calcula o pen�ltimo d�gito verificador */
    $acum = 0;
    for ($i = 0; $i < 9; $i++) {
        $acum += $cpf[$i] * (10 - $i);
    }

    $x = $acum % 11;
    $acum = ($x > 1) ? (11 - $x) : 0;
    /* Retorna falso se o digito calculado eh diferente do passado na string */
    if ($acum != $cpf[9]) {
        return false;
    }
    /* Calcula o �ltimo d�gito verificador */
    $acum = 0;
    for ($i = 0; $i < 10; $i++) {
        $acum += $cpf[$i] * (11 - $i);
    }

    $x = $acum % 11;
    $acum = ($x > 1) ? (11 - $x) : 0;
    /* Retorna falso se o digito calculado eh diferente do passado na string */
    if ($acum != $cpf[10]) {
        return false;
    }
    /* Retorna verdadeiro se o cpf eh valido */
    return true;
}

function FormataMascaraNU($Valor, $Mascara)
{
    if (empty($Mascara)) {
        return $Valor;
    }

    $Valor = trim($Valor);
    if ($Valor == "") {
        return "";
    }

    if ($Mascara == "CPF/CNPJ") {
        $ValorFormatado = formatarCPF_CNPJ($Valor);
        return $ValorFormatado;
    }

    $ColocarPontoMilhar = false;
    $ColocarZeroDecimal = false;

    switch ($Mascara) {
        case "#.##0,00":
            $ColocarPontoMilhar = true;
            $ColocarZeroDecimal = true;
            break;
        case "0,00":
            $Valor = trim($Valor);
            if ($Valor == "") {
                $Valor = 0;
            }
            $ColocarZeroDecimal = true;
            break;
        case "#.###":
            $ColocarPontoMilhar = true;
            break;
        default:
            return $Valor;
    }
    $AValor = explode(".", $Valor);
    $Decimal = "";
    if (key_exists(1, $AValor)) {
        $Decimal = $AValor[1];
    }
    if ($ColocarZeroDecimal) {
        $Parte = $Decimal;
        $Decimal = "00";
        for ($i = 0; $i < strlen($Parte); $i++) {
            $Decimal[$i] = $Parte[$i];
        }
        $Decimal = "," . $Decimal;
    }
    $Parte = $AValor[0];
    $j = 0;
    $Inteiro = "";
    for ($i = strlen($Parte) - 1; $i >= 0; $i--) {
        $Inteiro = $Parte[$i] . $Inteiro;
        $j++;
        if ($j == 3 && $ColocarPontoMilhar && is_numeric($Parte[$i - 1])) {
            $Inteiro = "." . $Inteiro;
            $j = 0;
        }
    }
    $ValorFormatado = $Inteiro . $Decimal;
    return $ValorFormatado;
}

function formatarCPF_CNPJ($campo, $formatado = true)
{
//retira formato
    $codigoLimpo = ereg_replace("[' '-./ t]", '', $campo);
// pega o tamanho da string menos os digitos verificadores
    $tamanho = (strlen($codigoLimpo) - 2);
//verifica se o tamanho do c�digo informado � v�lido
    if ($tamanho < 9) {
        return $campo;
    }

    if (!VALIDACPF($campo)) {
        $codigoLimpo = ZerosaEsquera($codigoLimpo, 14);
    }

    if ($formatado) {
// seleciona a m�scara para cpf ou cnpj
        $mascara = ($tamanho == 9) ? '###.###.###-##' : '##.###.###/####-##';

        $indice = -1;
        for ($i = 0; $i < strlen($mascara); $i++) {
            if ($mascara[$i] == '#')
                $mascara[$i] = $codigoLimpo[++$indice];
        }
//retorna o campo formatado
        $retorno = $mascara;
    }else {
//se n�o quer formatado, retorna o campo limpo
        $retorno = $codigoLimpo;
    }

    return $retorno;
}

function filtraSQLInjection($texto)
{
    $badchar[1] = "drop";
    $badchar[2] = "select";
    $badchar[3] = "delete";
    $badchar[4] = "update";
    $badchar[5] = "insert";
    $badchar[6] = "alter";
    $badchar[7] = "destroy";
    $badchar[8] = "table";
    $badchar[9] = "database";
    $badchar[10] = "drop";
    $badchar[11] = "union";
    $badchar[12] = "TABLE_NAME";
    $badchar[13] = "1=1";
    $badchar[14] = 'or 1';
    $badchar[15] = 'exec';
    $badchar[16] = 'INFORMATION_SCHEMA';
    $badchar[17] = 'TABLE_NAME';
    $badchar[18] = 'like';
    $badchar[19] = 'COLUMNS';
    $badchar[20] = 'into';
    $badchar[21] = 'VALUES';

    $y = 1;
    $x = sizeof($badchar);

    while ($y <= $x) {
        $pos = strpos(strtolower($texto), strtolower($badchar[$y]));
        if ($pos !== false) {
            $texto = str_replace(strtolower($badchar[$y]), "", strtolower($texto));
        }
        $y++;
    }

    return $texto;
}

function PegaBotoes($ProcId, $StepId)
{
    global $connect;
    $MostrarBotoes["OK"] = 1;
    $MostrarBotoes["SALVAR"] = 1;
    $MostrarBotoes["BLOQUEAR"] = 1;
    $MostrarBotoes["CANCELAR"] = 1;
    $MostrarBotoes["HISTORICO"] = 1;
    $SQL = "select 
	ShowButtonCancelar, ShowButtonOk, ShowButtonSalvar, ShowButtonBloquear, ShowButtonHistorico
	from 
	stepdef 
	where 
	ProcId = $ProcId 
	and
	StepId = $StepId
	";
    $Query = mysqli_query($connect, $SQL);
    $Result = mysqli_fetch_array($Query);
    if (key_exists("ShowButtonCancelar", $Result)) {
        $MostrarBotoes["CANCELAR"] = $Result["ShowButtonCancelar"];
    }

    if (key_exists("ShowButtonSalvar", $Result)) {
        $MostrarBotoes["SALVAR"] = $Result["ShowButtonSalvar"];
    }

    if (key_exists("ShowButtonHistorico", $Result)) {
        $MostrarBotoes["HISTORICO"] = $Result["ShowButtonHistorico"];
    }

    if (key_exists("ShowButtonBloquear", $Result)) {
        $MostrarBotoes["BLOQUEAR"] = $Result["ShowButtonBloquear"];
    }

    if (key_exists("ShowButtonOk", $Result)) {
        $MostrarBotoes["OK"] = $Result["ShowButtonOk"];
    }
    return $MostrarBotoes;
}

function CasoCasoEstaNaFila($ProcId, $StepId, $CaseNum)
{
    global $connect;
    $SQL = "select StepId from casequeue where ProcId = $ProcId and StepId = $StepId and CaseId = $CaseNum ";
    $Query = mysqli_query($connect, $SQL);
    return mysqli_num_rows($Query) > 0;
}

function PegaPassosDisponiveis($ProcId)
{
    global $connect;
    $SQL = "select 
		stepdef.StepId, 
		StepName,
		count(*) as TotalCasos
		from
		stepdef,
		casequeue
		where 
		stepdef.ProcId = $ProcId 
		and 
		stepdef.StepId = casequeue.StepId 
		and
		casequeue.ProcId = stepdef.ProcId
		group by  stepdef.StepId, StepName, DefaultView
		having count(*) > 0	order by DefaultView desc, StepName	
		";
    $Steps = array();
    $Query = mysqli_query($connect, $SQL);
    while ($Result = mysqli_fetch_array($Query)) {
        array_push($Steps, $Result);
    }
    return $Steps;
}

function is_utf8($string)
{

    return preg_match('%^(?:
	          [\x09\x0A\x0D\x20-\x7E]            # ASCII
	        | [\xC2-\xDF][\x80-\xBF]             # non-overlong 2-byte
	        |  \xE0[\xA0-\xBF][\x80-\xBF]        # excluding overlongs
	        | [\xE1-\xEC\xEE\xEF][\x80-\xBF]{2}  # straight 3-byte
	        |  \xED[\x80-\x9F][\x80-\xBF]        # excluding surrogates
	        |  \xF0[\x90-\xBF][\x80-\xBF]{2}     # planes 1-3
	        | [\xF1-\xF3][\x80-\xBF]{3}          # planes 4-15
	        |  \xF4[\x80-\x8F][\x80-\xBF]{2}     # plane 16
	    )*$%xs', $string);
}

// function is_utf8

function ScriptValidacaoCampo_old($Field)
{
    $FieldName = $Field["fieldname"];
    $FieldId = $Field["fieldid"];
    $ReadOnly = $Field["readonly"];
    $optional = $Field["optional"];
    $ScriptValidaCampo = trim($Field["ScriptValida"]);
    if ($optional == "") {
        $optional = 0;
    }
    $FieldType = $Field["fieldtype"];
    if ($ReadOnly == 1 or ( $ReadOnly == 2)) {
        return;
    }
    if (empty($ScriptValidaCampo)) {
        switch ($FieldType) {
            case "NU":
                $Retorno .= "   ValidaNU(document.textos.t$FieldId, \"$FieldName\", 't$FieldId', $optional)\n";
                break;
            case "DT":
                $Retorno .= "   ValidaDT(document.textos.t$FieldId,\"$FieldName\", 't$FieldId', $optional)\n";
                break;
            case "HR":
            case "TX":
            case "TL":
            case "TM":
                $Retorno .= "   ValidaTX(document.textos.t$FieldId, \"$FieldName\", 't$FieldId', $optional)\n";
                break;
            case "US":
                $Retorno .= "   ValidaUS(document.textos.t$FieldId, \"$FieldName\", 't$FieldId', $optional)\n";
                break;
            case "AR":
                $Retorno .= "   ValidaAR(document.textos.I$FieldId, \"$FieldName\", 't$FieldId', $optional)\n";
                break;
            case "GR":
            case "BO":
            case "TB":
            case "LT":
            case "SFR":
                $Retorno .= "   ValidaLista(document.textos.t$FieldId, \"$FieldName\", 't$FieldId', $optional)\n";
                break;
            case "ATB":
                $Retorno .= "   ValidaListaArray('textos', 't$FieldId" . "[]', '\"$FieldName\", 't$FieldId', $optional)\n";
                break;
            case "RU":
                $Retorno .= " retorno = retorno && ValidaCampoRU()\n";
                break;
            case "ATX":
                $Retorno .= " retorno = retorno && ValidaATX('textos', 't$FieldId" . "[]', \"$FieldName\",'t$FieldId', $optional)\n";
                break;
            case "ANU":
                $Retorno .= " retorno = retorno && ValidaANU('textos', 't$FieldId" . "[]', \"$FieldName\", 't$FieldId',$optional)\n";
                break;
            case "FD":
                $Retorno .= " retorno = retorno && ValidaFD(document.textos.r$FieldId, \"$FieldName\", 'r$FieldId', $optional)\n";
                break;
        }
    } else {
        $Retorno .= "   ValidaCampo$FieldId(document.textos.t$FieldId, \"$FieldName\", 't$FieldId',   $optional)\n";
        $ValidacaoCuston = $ValidacaoCuston . $ScriptValidaCampo . "\n";
    }
    return $Retorno;
}

function ScriptValidacaoCampo($Field, $FormPai = "")
{
    $FieldName = $Field["fieldname"];
    $FieldId = $Field["fieldid"];
    $ReadOnly = $Field["readonly"];
    $optional = $Field["optional"];
    $ScriptValidaCampo = trim($Field["ScriptValida"]);
    $Executar = false;
    if ($optional == "") {
        $optional = 0;
    }
    $FieldType = $Field["fieldtype"];
    if (($ReadOnly == 1 || ( $ReadOnly == 2)) && $FieldType != "SFR") {
        return;
    }
    if (empty($ScriptValidaCampo)) {
        switch ($FieldType) {
            case "DC":
                $Retorno = " ValidaDC($(\"#t$FieldId\")[0], '$FieldName', 't$FieldId', $optional)";
                break;

            case "NU":
                $Retorno = " ValidaNU($(\"#t$FieldId\")[0], '$FieldName', 't$FieldId', $optional)";
                break;
            case "DT":
                $Retorno = " ValidaDT($(\"#t$FieldId\")[0],'$FieldName', 't$FieldId', $optional)";
                break;
            case "HR":
            case "TX":
            case "TL":
            case "TM":
                $Retorno = " ValidaTX($(\"#t$FieldId\")[0], '$FieldName', 't$FieldId', $optional)";
                break;
            case "US":
                $Retorno = " ValidaUS($(\"#t$FieldId\")[0], '$FieldName', 't$FieldId', $optional)";
                break;
            case "AR":
                $Retorno = " ValidaAR($(\"#t$FieldId\")[0], '$FieldName', 't$FieldId', $optional)";
                break;
            case "IM":
                $Retorno = " ValidaIM($(\"#t$FieldId\")[0], '$FieldName', 't$FieldId', $optional)";
                break;
            case "GR":
            case "BO":
            case "TB":
            case "LT":
                $Retorno .= " ValidaLista($(\"#t$FieldId\")[0], '$FieldName', 't$FieldId', $optional)";
                break;
            case "SFR":
                $Retorno = " new Object()";
                $Executar = true;
                break;
            case "ATB":
                $Retorno = " ValidaListaArray('$FormPai', 't$FieldId" . "[]', ''$FieldName', 't$FieldId', $optional)";
                break;
            case "RU":
                $Retorno = " ValidaCampoRU()";
                break;
            case "ATX":
                $Retorno = " ValidaATX('$FormPai', 't$FieldId" . "[]', '$FieldName','t$FieldId', $optional)";
                break;
            case "ANU":
                $Retorno = " ValidaANU('$FormPai', 't$FieldId" . "[]', '$FieldName', 't$FieldId',$optional)";
                break;
            case "FD":
                $Retorno = " ValidaFD($(\"#r$FieldId\")[0], '$FieldName', 'r$FieldId', $optional)";
                break;
            case "IFD":
                $Retorno = " ValidaIFD($(\"#t$FieldId\")[0], '$FieldName', 'r$FieldId', $optional)";
                break;
            case "EXT":
                $Retorno = "";
                return $Retorno;
                break;
        }
    } else {
        $Retorno = "ValidaCampo$FieldId($(\"#t$FieldId\")[0], '$FieldName', 't$FieldId',   $optional)";
        $ValidacaoCuston = $ValidacaoCuston . $ScriptValidaCampo . "\n";
    }
    $ArvoreForms = "";

    if (is_array($FormPai)) {
        foreach ($FormPai as $Endereco) {
            $ArvoreForms .= "[\"$Endereco\"]";
        }
    }
    $ArvoreForms .= "[\"t$FieldId\"]";
    if (empty($Retorno)) {
        $Executar = false;
        $Retorno = "true";
    }

    if ($Executar) {
        $Retorno = "Validacao$ArvoreForms =  $Retorno \n";
    } else {
        $Retorno = "Validacao$ArvoreForms =  \"" . addslashes($Retorno) . "\" \n";
    }

    return $Retorno;
}

function _ScriptValidacaoCampo($Field, $FormPai = "")
{
    $FieldName = $Field["fieldname"];
    $FieldId = $Field["fieldid"];
    $ReadOnly = $Field["readonly"];
    $optional = $Field["optional"];
    $ScriptValidaCampo = trim($Field["ScriptValida"]);
    $Executar = false;
    if ($optional == "") {
        $optional = 0;
    }
    $FieldType = $Field["fieldtype"];
    if (($ReadOnly == 1 || ( $ReadOnly == 2)) && $FieldType != "SFR") {
        return;
    }
    if (empty($ScriptValidaCampo)) {
        switch ($FieldType) {
            case "DC":
                $Retorno = " ValidaDC(document.textos.t$FieldId, '$FieldName', 't$FieldId', $optional)";
                break;

            case "NU":
                $Retorno = " ValidaNU(document.textos.t$FieldId, '$FieldName', 't$FieldId', $optional)";
                break;
            case "DT":
                $Retorno = " ValidaDT(document.textos.t$FieldId,'$FieldName', 't$FieldId', $optional)";
                break;
            case "HR":
            case "TX":
            case "TL":
            case "TM":
                $Retorno = " ValidaTX(document.textos.t$FieldId, '$FieldName', 't$FieldId', $optional)";
                break;
            case "US":
                $Retorno = " ValidaUS(document.textos.t$FieldId, '$FieldName', 't$FieldId', $optional)";
                break;
            case "AR":
                $Retorno = " ValidaAR(document.textos.I$FieldId, '$FieldName', 't$FieldId', $optional)";
                break;
            case "IM":
                $Retorno = " ValidaIM(document.textos.t$FieldId, '$FieldName', 't$FieldId', $optional)";
                break;
            case "GR":
            case "BO":
            case "TB":
            case "LT":
                $Retorno .= " ValidaLista(document.textos.t$FieldId, '$FieldName', 't$FieldId', $optional)";
                break;
            case "SFR":
                $Retorno = " new Object()";
                $Executar = true;
                break;
            case "ATB":
                $Retorno = " ValidaListaArray('textos', 't$FieldId" . "[]', ''$FieldName', 't$FieldId', $optional)";
                break;
            case "RU":
                $Retorno = " ValidaCampoRU()";
                break;
            case "ATX":
                $Retorno = " ValidaATX('textos', 't$FieldId" . "[]', '$FieldName','t$FieldId', $optional)";
                break;
            case "ANU":
                $Retorno = " ValidaANU('textos', 't$FieldId" . "[]', '$FieldName', 't$FieldId',$optional)";
                break;
            case "FD":
                $Retorno = " ValidaFD(document.textos.r$FieldId, '$FieldName', 'r$FieldId', $optional)";
                break;
            case "IFD":
                $Retorno = " ValidaIFD(document.textos.r$FieldId, '$FieldName', 'r$FieldId', $optional)";
                break;
        }
    } else {
        $Retorno = "ValidaCampo$FieldId(document.textos.t$FieldId, '$FieldName', 't$FieldId',   $optional)";
        $ValidacaoCuston = $ValidacaoCuston . $ScriptValidaCampo . "\n";
    }
    $ArvoreForms = "";

    if (is_array($FormPai)) {
        foreach ($FormPai as $Endereco) {
            $ArvoreForms .= "[\"$Endereco\"]";
        }
    }
    $ArvoreForms .= "[\"t$FieldId\"]";
    if (empty($Retorno)) {
        $Executar = false;
        $Retorno = "true";
    }

    if ($Executar) {
        $Retorno = "Validacao$ArvoreForms =  $Retorno \n";
    } else {
        $Retorno = "Validacao$ArvoreForms =  \"" . addslashes($Retorno) . "\" \n";
    }

    return $Retorno;
}

function ScriptValidacao($ScriptCampos, $Action, $Completo = true)
{
    global $connect;
    $ValidacaoCuston = "";

// Abre a funcao Java Script
    if ($Completo) {
        $Retorno = "var Validacao = new Object()\n";
        $Retorno .= "function jsValidacaoFormulario()\n";
        $Retorno .= "	{\n";
        $Retorno .= "   return ValidacaoArray(Validacao) \n";
        $Retorno .= "	}\n";
        $Retorno .= "\n";
    }

    if ($Action == "Edit") {
        $Retorno .= $ScriptCampos;
    }

    return $Retorno;
}

function ScriptValidacao_old($ProcId, $StepId, $Action, $Completo = true)
{
    global $connect;
    $ValidacaoCuston = "";
    $SQL = "SELECT ";
    $SQL .= " procfieldsdef.fieldid, ";
    $SQL .= " fieldname, ";
    $SQL .= " fielddesc, ";
    $SQL .= " fieldtype, ";
    $SQL .= " fieldlength, ";
    $SQL .= " readonly, ";
    $SQL .= " optional, ";
    $SQL .= " fieldspecial, ";
    $SQL .= " OrderId, ";
    $SQL .= " NewLine, ";
    $SQL .= " FieldSourceTable, ";
    $SQL .= " FieldSourceField, ";
    $SQL .= " FieldDisplayField, ";
    $SQL .= " DefaultValue, ";
    $SQL .= " GroupSource, ";
    $SQL .= " ScriptValida, ";
    $SQL .= " ExtendProp, ";
    $SQL .= " CSS ";
    $SQL .= " from ";
    $SQL .= " procfieldsdef, ";
    $SQL .= " stepfieldsdef ";
    $SQL .= " where ";
    $SQL .= " procfieldsdef.procid = $ProcId ";
    $SQL .= " and ";
    $SQL .= " stepfieldsdef.procid = procfieldsdef.procid ";
    $SQL .= " and ";
    $SQL .= " stepfieldsdef.stepid = $StepId ";
    $SQL .= " and ";
    $SQL .= " stepfieldsdef.fieldid = procfieldsdef.fieldid ";
    $SQL .= " order by ";
    $SQL .= " OrderId ";
// Developer
    $QUERY = mysqli_query($connect, $SQL);

// Abre a funcao Java Script
    if ($Completo) {
        $Retorno = "var RumOb = 0; \n var RumOp = 0;\n";
        $Retorno = "var Validacoes = new Array()\n";
        $Retorno .= "function jsValidacaoFormulario()\n";
        $Retorno .= "	{\n";
        $Retorno .= "  var retorno = true \n";
        $Retorno .= "  for (i=0;i < Validacoes.length; i++) \n";
        $Retorno .= "  { \n ";
        $Retorno .= " 		//alert(i)\n";
        $Retorno .= " 		retorno = retorno && Validacoes[i](); \n";
        $Retorno .= "  } \n ";
        $Retorno .= " //alert('Validacao interna')\n";
    } else {
        $Retorno = "var retorno = true \n";
    }
    if (($Action == "Edit")) {
        while ($linha = mysqli_fetch_array($QUERY)) {
            $nome = $linha["fieldname"];
            $campo = $linha["fieldid"];
            $ReadOnly = $linha["readonly"];
            $optional = $linha["optional"];
            $ScriptValidaCampo = trim($linha["ScriptValida"]);
            if ($optional == "") {
                $optional = 0;
            }
            $FieldType = $linha["fieldtype"];
            if ($ReadOnly == 1 or ( $ReadOnly == 2)) {
                continue;
            }
            if (empty($ScriptValidaCampo)) {
                switch ($FieldType) {
                    case "NU":
                        $Retorno .= "   ValidaNU(document.textos.t$campo, \"$nome\", 't$campo', $optional)\n";
                        break;
                    case "DT":
                        $Retorno .= "   ValidaDT(document.textos.t$campo,\"$nome\", 't$campo', $optional)\n";
                        break;
                    case "HR":
                    case "TX":
                    case "TL":
                    case "TM":
                        $Retorno .= "   ValidaTX(document.textos.t$campo, \"$nome\", 't$campo', $optional)\n";
                        break;
                    case "US":
                        $Retorno .= "   ValidaUS(document.textos.t$campo, \"$nome\", 't$campo', $optional)\n";
                        break;
                    case "AR":
                        $Retorno .= "   ValidaAR(document.textos.I$campo, \"$nome\", 't$campo', $optional)\n";
                        break;
                    case "GR":
                    case "BO":
                    case "TB":
                    case "LT":
                    case "SFR":
                        $Retorno .= "   ValidaLista(document.textos.t$campo, \"$nome\", 't$campo', $optional)\n";
                        break;
                    case "ATB":
                        $Retorno .= "   ValidaListaArray('textos', 't$campo" . "[]', '\"$nome\", 't$campo', $optional)\n";
                        break;
                    case "RU":
                        $Retorno .= " retorno = retorno && ValidaCampoRU()\n";
                        break;
                    case "ATX":
                        $Retorno .= " retorno = retorno && ValidaATX('textos', 't$campo" . "[]', \"$nome\",'t$campo', $optional)\n";
                        break;
                    case "ANU":
                        $Retorno .= " retorno = retorno && ValidaANU('textos', 't$campo" . "[]', \"$nome\", 't$campo',$optional)\n";
                        break;
                    case "FD":
                        $Retorno .= " retorno = retorno && ValidaFD(document.textos.r$campo, \"$nome\", 'r$campo', $optional)\n";
                        break;
                }
            } else {
                $Retorno .= "   ValidaCampo$campo(document.textos.t$campo, \"$nome\", 't$campo',   $optional)\n";
                $ValidacaoCuston = $ValidacaoCuston . $ScriptValidaCampo . "\n";
            }
        }
    }
// Fecha a Funcao
    $Retorno .= "	return retorno;\n";
//$Retorno .= "	return false;\n";
    if ($Completo) {
        $Retorno .= "	}\n";
        $Retorno .= "\n";
        $Retorno .= $ValidacaoCuston;
        $Retorno .= "var ValidaAntigo = valida\n";
    }
//return false;
    return $Retorno;
}

function AtualizaExportKey($CaseNum, $FieldId, $Valor, $ValorDisplay)
{
    global $connect;
    $Campo = "Campo" . $FieldId;

    $Valor = str_replace("'", "/*%/*", $Valor);
    $Valor = str_replace(",", "/*#/*", $Valor);
    $Valor = str_replace(",", "/*!/*", $Valor);
    $SQL = "UPDATE exportkeys set $Campo ='$Valor' where CaseNum = $CaseNum ";
    IF (!@mysqli_query($connect, $SQL)) {
        $SQL2 = "alter table exportkeys add $Campo varchar(255)";
        @mysqli_query($connect, $SQL2);
        @mysqli_query($connect, $SQL);
    }

    $ValorDisplay = str_replace("'", "/*%/*", $ValorDisplay);
    $ValorDisplay = str_replace(",", "/*#/*", $ValorDisplay);
    $ValorDisplay = str_replace(",", "/*!/*", $ValorDisplay);
    $Campo = "CampoDisplay" . $FieldId;
    $SQL = "UPDATE exportkeysdisplay set $Campo ='$ValorDisplay' where CaseNum = $CaseNum ";
    IF (!@mysqli_query($connect, $SQL)) {
        $SQL2 = "alter table exportkeysdisplay add $Campo varchar(255)";
        @mysqli_query($connect, $SQL2);
        @mysqli_query($connect, $SQL);
    }
    mysqli_query($connect, $SQL);
}

function PegaOrigemUserById($UserId)
{
    global $connect, $OrigemLogon;
    if (!is_numeric($UserId)) {
        $UserId = PegaIdPeloNome($UserId);
    }
    if ($UserId == 0) {
        return "";
    }
    $EXTERNAL_userdef = "";
    $EXTERNAL_USERID = "";
    include("../include/config.config.php");
    switch ($OrigemLogon) {
        case "ProcessLogon":
            $SQL = "select Origem from userdef where UserId = $UserId";
            $Query = mysqli_query($connect, $SQL);
            $result = mysqli_fetch_array($Query);
            $retorno = $result["Origem"];
            break;
        case "External":
            $connectDBExterno = AtivaDBExterno();
            $SQL = "select Origem from $EXTERNAL_userdef where $EXTERNAL_USERID = $UserId";
            $Query = mysqli_query($connectDBExterno, $SQL);
            $result = mysqli_fetch_array($Query);
            $retorno = $result["Origem"];
            break;
    }
    AtivaDBProcess();
    return $retorno;
}

/**
 * 
 * @global type $connect
 * @global type $CaseNum
 * @global type $ProcId
 * @global type $StepId
 * @global type $userdef
 * @param type $EventId
 * @param type $Desc
 */
function audittrail($EventId = 20, $Desc = "")
{
    global $connect, $CaseNum, $ProcId, $StepId, $userdef;
    if ($StepId == 0) {
        $StepName = "Administradores";
    } else {
        $StepName = PegaStepName($StepId, $ProcId);
    }
    $SQL = "insert into audittrail (ProcId, CaseNum, StepId, UserId, EventId, EventDateTime, StepName, UserName, ActionDesc) values ($ProcId, $CaseNum, $StepId, $userdef->UserId, $EventId, now(), '$StepName', '$userdef->UserDesc','$Desc')";
    mysqli_query($connect, $SQL);
}

/**
 * 
 * @global type $connect
 * @param type $procId
 * @param type $caseNum
 * @param type $stepId
 * @param type $userId
 * @param type $userName
 * @param type $eventId
 * @param type $mensagem
 */
function insereEntradaAuditTrail($procId, $caseNum, $stepId, $userId, $userName, $eventId, $mensagem)
{
    global $connect;

    $mensagemTratada = addslashes($mensagem);
    $StepName = PegaStepName($stepId, $procId);
    $SQL = "insert into "
            . "audittrail "
            . "( "
            . "ProcId, "
            . "CaseNum, "
            . "StepId, "
            . "UserId, "
            . "EventId, "
            . "EventDateTime, "
            . "StepName, "
            . "UserName, "
            . "ActionDesc"
            . ") "
            . "values "
            . "("
            . "$procId, "
            . "$caseNum, "
            . "$stepId, "
            . "$userId, "
            . "$eventId, "
            . "now(), "
            . "'$StepName', "
            . "'$userName',"
            . "'$mensagemTratada')";
    $query = mysqli_query($connect, $SQL);
    if (mysqli_errno($connect)) {
        error_log("Falha " . mysqli_error($connect));
    }
}

function PegaFieldIdByCode($ProcId, $FieldCode)
{
    global $connect;
    if (is_numeric($FieldCode)) {
        return $FieldCode;
    }

    if (!is_numeric($ProcId)) {
        $ProcId = PegaProcIdByCode($ProcId);
    }

    $SQL = "select FieldId, FieldCod from procfieldsdef where ProcId = $ProcId and FieldCod = '$FieldCode'";
    $Query = mysqli_query($connect, $SQL);
    if (mysqli_num_rows($Query) > 0) {
        $Result = mysqli_fetch_array($Query);
        $FieldId = $Result["FieldId"];
    } else {
        $FieldId = 0;
    }
    return $FieldId;
}

/**
 * 
 * @param type $caseNum
 * @param type $valueId
 * @param type $fieldId
 * @param type $extensaoArquivo
 * @return string
 */
function criaNomeArquivoUpload($caseNum, $valueId, $fieldId, $extensaoArquivo)
{
    $dirNameCaseNum = formataCaseNum($caseNum, 10);
    $valueIdFileName = formataCaseNum($valueId, 2);
    $fieldIdFileName = formataCaseNum($fieldId, 4);
    $nomeArquivo = strtolower($dirNameCaseNum . $fieldIdFileName . $valueIdFileName . "." . $extensaoArquivo);
    return $nomeArquivo;
}

/**
 * 
 * @param type $procId
 * @param type $caseNum
 * @param type $fieldId
 * @return string
 */
function criaDiretorioCampoArquivo($procId, $caseNum, $fieldId)
{
    $dirNameProc = formataCaseNum($procId, 6);
    $dirNameCaseNum = formataCaseNum($caseNum, 10);
    $dirNameFolderId = formataCaseNum($fieldId, 6);
    if (!is_dir(FILES_UPLOAD . "/" . $dirNameProc)) {
        mkdir(FILES_UPLOAD . "/" . $dirNameProc);
    }
    $dirToCaseNum = FILES_UPLOAD . "/$dirNameProc/$dirNameCaseNum";
    if (!is_dir($dirToCaseNum)) {
        mkdir($dirToCaseNum);
    }
    $dirName = FILES_UPLOAD . "/$dirNameProc/$dirNameCaseNum/$dirNameFolderId";
    if ($dirName) {
        mkdir($dirName);
    }
// error_log("diretorio destino: $dirName");
    return $dirName;
}

/**
 * 
 * @global type $connect
 * @param type $ProcId
 * @return type
 */
function PegaFieldIdsByCode($ProcId)
{
    global $connect;

    $SQL = "select FieldId, FieldCod from procfieldsdef where ProcId = $ProcId";
    $Query = mysqli_query($connect, $SQL);
    /*
      while ($Result = mysqli_fetch_array($Query)) {
      $FieldsCodes[trim($Result["FieldCod"])] = $Result["FieldId"];
      }
     * 
     */
    $FieldsCodes = mysqli_fetch_all($Query, MYSQLI_ASSOC);
    return $FieldsCodes;
}

/**
 * 
 * @global type $connect
 * @param type $ProcCode
 * @return int
 */
function PegaProcIdByCode($ProcCode)
{
    global $connect;
    if (is_numeric($ProcCode)) {
        return $ProcCode;
    }
    $ProcCodeSearch = trim($ProcCode);
    $SQL = "select ProcId from procdef where ProcCod = '$ProcCodeSearch' and Active = 1";
    $Query = mysqli_query($connect, $SQL);
    $Result = mysqli_fetch_array($Query);
    $ProcId = $Result["ProcId"];
    if (!is_numeric($ProcId)) {
        $ProcId = 0;
    }
    return $ProcId;
}

/**
 * 
 * @param type $fileNameStorage
 * @param type $fileName
 * @param type $UserName
 * @param type $UserId
 * @param type $returnArray
 * @return type
 */
function criaExtendDataCampoArquivo($fileNameStorage, $fileName, $UserName, $UserId, $returnArray = false)
{
    $extendData = array();
    $extendData["fileNameStorage"] = $fileNameStorage;
    $extendData["fileName"] = $fileName;
    $extendData["descricao"] = "";
    $extendData["versao"] = "";
    $extendData["username"] = $UserName;
    $extendData["userid"] = $UserId;
    $extendData["data"] = Date('Y-m-d H:i:s');
    if ($returnArray) {
        return $extendData;
    }
    $jsonExtendData = json_encode($extendData);
    return $jsonExtendData;
}

/**
 * 
 * @global type $connect
 * @param type $ProcId
 * @param type $StepCode
 * @return int
 */
function PegaStepIdByCode($ProcId, $StepCode)
{
    global $connect;
    if (is_numeric($StepCode)) {
        return $StepCode;
    }
    $SQL = "select StepId from stepdef where ProcId = $ProcId and StepCod = '$StepCode' and Active = 1";
    $Query = mysqli_query($connect, $SQL);
    $Result = mysqli_fetch_array($Query);
    $StepId = $Result["StepId"];
    if (!is_numeric($StepId)) {
        $StepId = 0;
    }
    return $StepId;
}

/**
 * 
 * @param type $ProcId
 * @param type $FieldId
 * @param type $valor
 * @return string
 */
function PegaDisplayCampoLista($ProcId, $FieldId, $valor)
{
    $Listavalores = PegaDadosCampoLista($ProcId, $FieldId, $valor);
    foreach ($Listavalores as $ItemLista) {
        if ($ItemLista["selected"]) {
            return $ItemLista["Display"];
        }
    }
    return "";
}

function montaFiltros($post)
{
    global $ProcId;
    $Retornar = false;
    $filtros = array();
    while (list($key, $valor) = each($post)) {
        if (strpos($key, 'FCampo') !== 0) {
            continue;
        }
        if ($valor != "") {
            $key = str_replace('FCampo', '', $key);
            $Filtro["Campo"] = $key;
//$Filtro["Valor"] = utf8_decode($valor);
            $Filtro["Valor"] = $valor;
            $Filtro["Tipo"] = PegaTipoCampoReferencia($key);
            $Filtro["Nome"] = PegaNomeCampoReferencia($key);
            $Filtro["Mascara"] = PegaMascaraCampoReferencia($key);
            switch ($Filtro["Tipo"]) {
                case "US":
                    $Filtro["ValorDisplay"] = PegaNomeUsuario($valor);
                    break;
                case "BO":
                    $Filtro["ValorDisplay"] = "Não";
                    if ($valor == 1) {
                        $Filtro["ValorDisplay"] = "Sim";
                    }
                    break;
                case "LT":
                case "SFR":
                    $Filtro["ValorDisplay"] = PegaDisplayCampoLista($ProcId, $key, $valor);
                    break;
                case "NU":
                    if ($Filtro["Mascara"] == "CPF/CNPJ") {
                        $valor = formatarCPF_CNPJ($valor, false);
                        $Filtro["Valor"] = $valor;
                    }
                    $Filtro["ValorDisplay"] = FormataMascaraNU($valor, $Filtro["Mascara"]);
                    break;
                default:
//$Filtro["ValorDisplay"] = utf8_decode($valor);
                    $Filtro["ValorDisplay"] = $valor;
                    break;
            }
            array_push($filtros, $Filtro);
            $Retornar = true;
        }
    }
    if ($Retornar) {
        return $filtros;
    } else {
        return "";
    }
}

function IncluiCalendario()
{
    include("/javascript/DycalendarCallBack.html");
}

function IncluiCalendarioXML($Campo)
{
    include("/javascript/DycalendarCallBackXML.html");
}

/**
 * Formata data dd/mm/YYYY para YYYY-mm-dd
 * 
 * @param string $Data
 * @return string
 */
function FormataDataDB($Data)
{
    if (strrpos($Data, "/")) {
        $Data = substr($Data, 6, 4) . "-" . substr($Data, 3, 2) . "-" . substr($Data, 0, 2) . substr($Data, 10, 5);
    }
    return $Data;
}

/**
 * 
 * Formata data dd/mm/YYYY para YYYY-mm-dd
 * 
 * @param type $Data
 * @return string
 */
function FormataData($Data)
{
    $DataS = $Data;
    if (strpos($Data, "/") == 1) {
        $Data = '0' . $Data;
    }
    if (strrpos($Data, "/") == 4) {
        $Data = substr($Data, 0, 2) . '/0' . substr($Data, 3, 1) . "/" . substr($Data, 5, 4) . " " . substr($Data, 10, 5);
    }
    if (strrpos($Data, "/")) {
        $DataS = substr($Data, 6, 4) . "-" . substr($Data, 3, 2) . "-" . substr($Data, 0, 2);
    }
    if (strrpos($Data, ":")) {
        $DataS .= " " . substr($Data, 11, 6);
    }
    return $DataS;
}

function isdate($Data)
{
    if (strpos($Data, "-") == 4) {
        return true;
    } else {
        return false;
    }
}

/**
 * 
 * @global type $connect
 * @param type $procId
 * @return type
 */
function PegaCamposProcesso($procId)
{
    global $connect;
    $SQL = "select fieldcod, fieldid from procfieldsdef where procid = $procId";
    $Query = mysqli_query($connect, $SQL);
    $retorno = mysqli_fetch_all($Query, MYSQLI_ASSOC);
    return $retorno;
}

function MostraReferenciasQueue($ProcId, $CaseNum)
{
    $Referencias = PegaReferencias($ProcId, $CaseNum, 1, 1);
    if (count($Referencias) > 0) {
        for ($contador = 0; $contador < count($Referencias); $contador++) {
            echo "<td>&nbsp;" . htmlentities(trim($Referencias[$contador]["FieldValue"])) . "</td>\n";
        }
    }
}

function MostraReferencias($ProcId, $CaseNum)
{
    $Referencias = PegaReferencias($ProcId, $CaseNum);
    echo "<td>$CaseNum</td>\n";
    if (count($Referencias) > 0) {
        for ($contador = 0; $contador < count($Referencias); $contador++) {
            $Referencias[$contador] = htmlentities($Referencias[$contador]);
            echo "<td>&nbsp;" . AcessoCaso($CaseNum, $ProcId) . $Referencias[$contador] . "</td>\n";
        }
    }
    return;
}

function PegaPageEdit($StepId, $ProcId = 0)
{
    global $procdef;
    if ($procdef != null) {
        $PageAction = trim($procdef->Steps[$StepId]["PageAction"]);
    }
    if (empty($PageAction)) {
//$PageAction = "BPMEditCase.php";
        $PageAction = "editcase";
    }
    return $PageAction;
}

function CabecalhoReferencias($ProcId, $Campo = '', $Ordem = '', $Action = '')
{
    $CamposRef = PegaCamposRef($ProcId);
    for ($i = 0; $i < count($CamposRef); $i++) {
        if ($CamposRef[$i]["Campo"] == $Campo)
            if ($Ordem == "Desc") {
                $ImagemRef = "ordem_desc.png";
                $Ordem = '';
            } else {
                $ImagemRef = "ordem_cresc.png";
                $Ordem = 'Desc';
            } else
            $ImagemRef = "ordem_inativa.png";
        ?>
        <td><table width="100%"><td class="LinhaTitulo">
        <?= $CamposRef[$i]["Nome"] ?>
                </td><td align="right">
                    <a href="<?= $Action ?>Campo=<?= $CamposRef[$i]["Campo"] ?>&Ordem=<?= $Ordem ?>"><img src="images/<?= $ImagemRef ?>" border="0" ></a></td></table></td>	
        <?php
    }
}

function CabecalhoReferenciasSimples($ProcId, $paraTemplate = false, $numRef = 0)
{
    AtivaDBProcess();
    $CamposRef = PegaCamposRef($ProcId);

    if ($$numRef > 0) { // Mantem apenas o numero de referencias em $numRef
        $CamposRef = array_slice($CamposRef, 0, $numRef);
    }

    /** Retorna apenas o array ou cria as celulas
     * 
     */
    if ($paraTemplate) {
        return $CamposRef;
    } else {
        ?>
        <td class="Referencia">
            Número
        </td>		
        <?php
        for ($i = 0; $i < count($CamposRef); $i++) {
            ?>
            <td class="Referencia">
            <?= htmlentities($CamposRef[$i]["Nome"]) ?>
            </td>	
                <?php
            }
        }
        return;
    }

    function PegaListaDominio($Origem)
    {
        global $connect;
        $SQL = "select Origem_User from origemdominio where Origem_DOC = '$Origem'";
        $Query = mysqli_query($connect, $SQL);
        $Dominio = array();
        while ($Result = mysqli_fetch_array($Query)) {
            array_push($Dominio, $Result["Origem_User"]);
        }
        return $Dominio;
    }

    function PegaOrigemCaso($CaseNum)
    {
        global $connect, $userdef;
        $SQL = "select Origem from exportkeys where CaseNum = $CaseNum";
        $query = mysqli_query($connect, $SQL);
        $Result = mysqli_fetch_array($query);
        $Origem = $Result["Origem"];
        if (empty($Origem)) {
            $Origem = $userdef->Origem;
        }
        return $Origem;
    }

    function PegaOrigem()
    {
        global $connect, $userdef;
        $SQL = "select Origem from origemgrupos where Grupo in ($userdef->usergroups)";
        $Query = mysqli_query($connect, $SQL);
        $Result = mysqli_fetch_array($Query);
        return $Result["Origem"];
    }

    function Enteraudittrail($ProcId, $CaseNum, $StepId = 0, $Codigo = 0, $ActionDesc = '')
    {
        global $connect, $userdef;
        $StepName = PegaNomeStep($ProcId, $StepId);
        $SQL = "insert into ";
        $SQL .= " audittrail ";
        $SQL .= " ( ";
        $SQL .= " ProcId, ";
        $SQL .= " CaseNum, ";
        $SQL .= " StepId, ";
        $SQL .= " EventId, ";
        $SQL .= " EventDatetime, ";
        $SQL .= " UserId, ";
        $SQL .= " UserName, ";
        $SQL .= " StepName, ";
        $SQL .= " ActionDesc";
        $SQL .= " ) ";
        $SQL .= " values  ";
        $SQL .= " ( ";
        $SQL .= " $ProcId,  ";
        $SQL .= " $CaseNum,  ";
        $SQL .= " $StepId,  ";
        $SQL .= " $Codigo,  ";
        $SQL .= " now(),  ";
        $SQL .= " $userdef->UserId, ";
        $SQL .= " '$userdef->UserDesc', ";
        $SQL .= " '$StepName', ";
        $SQL .= " '$ActionDesc'";
        $SQL .= " ) ";
        mysqli_query($connect, $SQL);
    }

    function unhtmlentities($string)
    {
        $trans_tbl = get_html_translation_table(HTML_ENTITIES);
        $trans_tbl = array_flip($trans_tbl);
        return strtr($string, $trans_tbl);
    }

    function Carregaconfig()
    {
        global $connect;
        $config = array();
        $SQL = "select * from config";
        $Query = mysqli_query($connect, $SQL);
        while ($Result = mysqli_fetch_array($Query)) {
            $Chave = trim($Result["Funcao"]);
            $config[$Chave] = trim($Result["Valor"]);
        }
        return $config;
    }

    function PegaNomeInstancia($ProcId)
    {
        global $connect;
        $SQL = " select InstanceName from procdef where ProcId = $ProcId";
        $Query = mysqli_query($connect, $SQL);
        $result = mysqli_fetch_array($Query);
        mysqli_free_result($Query);
        $Valor = $result["InstanceName"];
        if (empty($Valor)) {
            $Valor = "Caso";
        }
        return $Valor;
    }

    function e_ordenado($ProcId, $campo)
    {
        global $connect;
        $SQL = " select ";
        $SQL .= "  ExtendProp ";
        $SQL .= "  from ";
        $SQL .= "  procfieldsdef ";
        $SQL .= "  where ";
        $SQL .= "  ProcId = $ProcId";
        $SQL .= "  and ";
        $SQL .= "  FieldId = $campo ";

        $Query = mysqli_query($connect, $SQL);
        $result = mysqli_fetch_array($Query);
        mysqli_free_result($Query);
        $valores = $result["ExtendProp"];
        $pos = strpos($valores, "Sorted");
        if (is_numeric($pos)) {
            return true;
        } else {
            return false;
        }
    }

    function Prioridade($Prioridade)
    {
        $contador = 1;
        $APrioridade[0] = 'Sem Horário marcado';
        for ($contador1 = 6; $contador1 < 22; $contador1++) {
            for ($contador2 = 0; $contador2 < 4; $contador2++) {
                $minuto = $contador2 * 15;
                if ($minuto == 0)
                    $minuto = '00';
                $hora = $contador1;
                if ($hora < 10)
                    $hora = '0' . $hora;
                $APrioridade[$contador] = $hora . ':' . $minuto;
                $contador++;
            }
        }
        $APrioridade[$contador] = '22:00';
        return $APrioridade[$Prioridade];
    }

    function PegaAcao($ProcId, $StepId, $CaseNum = 0)
    {
        global $connect, $userdef;
        AtivaDBProcess();
        $SQL = "(select action from stepaddresses where ProcId = $ProcId and StepId = $StepId  and ((GroupId in ($userdef->usergroups) and GrpFld = 'G') or (GroupId = $userdef->UserId and GrpFld = 'U')))
	union 
	(select Action from stepaddresses SA, fieldaddresses FA, procfieldsdef PF 
	where 
	SA.ProcId = $ProcId and
	GrpFld = 'F' and 
	FA.FieldId = GroupId 
	and 
	FieldValue = '$userdef->UserId' 
	and 
	CaseNum = $CaseNum 
	and 
	SA.ProcId = FA.ProcId 
	and 
	StepId = $StepId 
	and
	PF.ProcId = SA.ProcID and PF.FieldId = GroupId
	and
	FieldType = 'US'
	)
	union
	(select Action from stepaddresses SA, fieldaddresses FA, procfieldsdef PF 
	where 
	SA.ProcId = $ProcId and
	GrpFld = 'F' and 
	FA.FieldId = GroupId 
	and 
	FieldValue in ($userdef->usergroups) 
	and 
	CaseNum = $CaseNum 
	and 
	SA.ProcId = FA.ProcId 
	and 
	StepId = $StepId 
	and
	PF.ProcId = SA.ProcID and PF.FieldId = GroupId
	and
	FieldType = 'GR'
	) ";

        $Q = mysqli_query($connect, $SQL);
        while ($linha = mysqli_fetch_array($Q)) {
            if ($linha["action"] == 1) {
                return "Edit";
            }
        }
        return "View";
    }

    function PegaInstanceName($ProcId)
    {
        global $connect;
        $SQL = " Select ";
        $SQL .= " InstanceName ";
        $SQL .= " from ";
        $SQL .= " procdef ";
        $SQL .= " where ";
        $SQL .= " ProcId = $ProcId ";
        $Query = mysqli_query($connect, $SQL);
        $result = mysqli_fetch_array($Query);
        return $result["InstanceName"];
    }

    function PegaStepDoc($ProcId, $CaseNum)
    {
        global $connect;
        $SQL = " Select ";
        $SQL .= " StepId ";
        $SQL .= " from ";
        $SQL .= " casequeue ";
        $SQL .= " where ";
        $SQL .= " ProcId = $ProcId ";
        $SQL .= " and ";
        $SQL .= " CaseId = $CaseNum ";
        $SQL .= " order by StepId desc ";
        $Query = mysqli_query($connect, $SQL);
        $result = mysqli_fetch_array($Query);
        return $result["StepId"];
    }

    function PegaStepName($StepId, $ProcId)
    {
        return PegaNomeStep($ProcId, $StepId);
    }

    function PegaProcName($ProcId)
    {
        return PegaNomeProc($ProcId);
    }

    function Descompacta($dir, $Source)
    {
        if (!empty($Source)) {
            exec("tar -xzf $dir$Source");
        }
    }

    function DateDiff($to, $from, $Tipo = "m")
    {
        return date_diff_i($to, $from, $Tipo);
    }

    function PegaDadosCampoTabela($ProcId, $Campo)
    {
        global $connect;
        $SQL = " select  ";
        $SQL .= "  FieldSourceTable,  ";
        $SQL .= "  FieldSourceField,  ";
        $SQL .= "  FieldDisplayField,  ";
        $SQL .= "  FieldUserId  ";
        $SQL .= "  from  ";
        $SQL .= "  procfieldsdef  ";
        $SQL .= "  where  ";
        $SQL .= "  ProcId = $ProcId  ";
        $SQL .= "  and  ";
        $SQL .= "  FieldId = $Campo ";
        $Query = mysqli_query($connect, $SQL);
        $result = mysqli_fetch_array($Query);
        $SourceData[0] = str_replace("''", "'", $result["FieldSourceTable"]);
        $SourceData[1] = $result["FieldSourceField"];
        $SourceData[2] = $result["FieldDisplayField"];
        $SourceData[3] = $result["FieldUserId"];
        return $SourceData;
    }

    function ExtendMes($mes)
    {
        $meses[1] = "Janeiro";
        $meses[2] = "Fevereiro";
        $meses[3] = "Mar�o";
        $meses[4] = "Abril";
        $meses[5] = "Maio";
        $meses[6] = "Junho";
        $meses[7] = "Julho";
        $meses[8] = "Agosto";
        $meses[9] = "Setembro";
        $meses[10] = "Outubro";
        $meses[11] = "Novembro";
        $meses[12] = "Dezembro";
        return $meses[$mes];
    }

    function date_diff_i($from, $to, $periodo = "m")
    {
// Separa Data da Hora
        list($Date_from, $Time_from) = explode(" ", $from);
        list($Date_to, $Time_to) = explode(" ", $to);

//Separa Dia mes e ano
        list($from_year, $from_month, $from_day) = explode("-", $Date_from);
        list($to_year, $to_month, $to_day) = explode("-", $Date_to);
// list($from_year, $from_month, $from_day, $Time_from) = explode(" ", $from); 
// list($to_year, $to_month, $to_day, $Time_to) = explode(" ", $to);          	
// $from_month = $meses["$from_month"];
// $to_month = $meses["$to_month"];
// Separa Minuto e segundo
        list($from_hora, $from_minutos, $from_segundos) = explode(":", $Time_from);
        list($to_hora, $to_minutos, $to_segundos) = explode(":", $Time_to);
        if (empty($to_segundos)) {
            $to_segundos = 0;
        }

        if (empty($from_segundos)) {
            $from_segundos = 0;
        }

        if ($from_year < 1970) {
            $from_year = 1971;
        }
        if ($to_year < 1970) {
            $to_year = 1971;
        }
        $from_date = mktime($from_hora, $from_minutos, $from_segundos, $from_month, $from_day, $from_year);
        $to_date = mktime($to_hora, $to_minutos, $to_segundos, $to_month, $to_day, $to_year);
        $diff = ($to_date - $from_date);
        switch ($periodo) {
            case "d":
                $days = $diff / 86400;
                break;
            case "h":
                $horas = $diff / 3600;
                $days = $horas;
                break;
            case "m":
                $days = $diff / 60;
                break;
            case "s":
                $days = $diff;
                break;
        }
        return $days;
    }

    function ConvDate($dataRecebida, $SupressZero = false, $dataOnly = false)
    {
// Convers�o de formato DB data para pt/BR
        $DataValor = "";
        if (!empty($dataRecebida)) {
            if (strpos($dataRecebida, ":") == 0) {
                if (strpos($dataRecebida, "/") == 0) {
                    $DataConv = explode(" ", $dataRecebida);
                    $Data = explode("-", $DataConv[0]);
                    $DataValor = $Data[2] . "/" . $Data[1] . "/" . $Data[0];
                } else {
                    $DataValor = $dataRecebida;
                }
            } else {
                if (strpos($dataRecebida, "/")) {
                    if ($SupressZero && strpos($Data, " ")) {
                        $DataConv = explode(" ", $dataRecebida);
                        return $DataConv[0];
                    }
                    return $dataRecebida;
                }
                $DataConv = explode(" ", $dataRecebida);
                if (empty($DataConv[1])) {
                    $DataValor = $DataConv[0];
                } else {
                    $DataValor = explode("-", $DataConv[0]);
                }
                if (empty($DataConv[1]) && $SupressZero) {
                    $DataValor = $DataValor[2] . "/" . $DataValor[1] . "/" . $DataValor[0];
                } else {
                    $DataValor = $DataValor[2] . "/" . $DataValor[1] . "/" . $DataValor[0] . " " . substr($DataConv[1], 0, 5);
                }
            }
        }
        return $DataValor;
    }

    function PegaDefaultStep($ProcId)
    {
        global $connect;
        $SQL = "select ";
        $SQL .= "  StepId ";
        $SQL .= "  from ";
        $SQL .= "  stepdef ";
        $SQL .= "  where ";
        $SQL .= "  (DefaultView = '1' or DefaultView = 'T')";
        $SQL .= "  and ";
        $SQL .= "  ProcId = $ProcId ";
        $QUERY = mysqli_query($connect, $SQL);
        $result = mysqli_fetch_array($QUERY);
        mysqli_free_result($QUERY);
        return $result['StepId'];
    }

    function PegaCamposRef($ProcId)
    {
        global $connect;
        $SQL = " select ";
        $SQL .= "  FieldName, FieldId ";
        $SQL .= "  from ";
        $SQL .= "  procfieldsdef ";
        $SQL .= "  where ";
        $SQL .= "  FieldRef <> 0 ";
        $SQL .= "  and ";
        $SQL .= "  ProcId = $ProcId ";
        $SQL .= "  order by ";
        $SQL .= "  FieldRefOrder ";
        $QUERY1 = mysqli_query($connect, $SQL);
        $Campos = array();
        while ($Result = mysqli_fetch_array($QUERY1)) {
            $Campo["Campo"] = "Campo" . $Result["FieldId"];
            $Campo["Nome"] = $Result["FieldName"];
            array_push($Campos, $Campo);
        }
        return $Campos;
    }

    function FormataCampo($Valor, $Tipo, $campo)
    {
        global $S_procdef;
        $Processo = $S_procdef["ProcId"];
//	AtivaDBProcess();	
        switch ($Tipo) {
            case "DC":
                $SourceData = PegaDadosCampoTabela($Processo, $campo);
                $Valor = PegaValorCampo($SourceData[0], $Valor, $SourceData[2], "TXT", 1);
                break;
            case "LT":
//$Valor = PegaValorCampoLista($Processo, $campo, $Valor); // Como usa Campo Display o valor j� est� transformado
                break;
            case "TB":
                $SourceData = PegaDadosCampoTabela($Processo, $campo);
                $Valor = PegaValorCampoTabela($Valor, $SourceData[0], $SourceData[1], $SourceData[2], $SourceData[3]);
                break;
            case "US":
                $Valor = PegaNomeUsuario($Valor);
                break;
            case "GR":
                $Valor = PegaValorCampoTabela($Valor, "groupdef", "GroupId", "GroupName");
                break;
            case "BO":
                if ($Valor == 1) {
                    $Valor = "Sim";
                } else {
                    $Valor = "N�o";
                }
                break;
            case "DT":
                $Valor = ConvDate($Valor, true, true);
                break;
            case "TM";
// N�o muda
                break;
        }
        return $Valor;
    }

    function ValorCampoQueue($Valor, $Tipo, $campo, $CaseNum, $Code)
    {
        global $S_procdef;
        $Processo = $S_procdef["ProcId"];
//	AtivaDBProcess();
        switch ($Tipo) {
            case "DC":
                $Valor = "PegaValorCampo('$Code$CaseNum', $Processo, $CaseNum, $campo, 'DC', 0)";
                break;
            case "LT":
            case "TX":
            case "NU":
                if (empty($Valor)) {
                    $Valor = "PegaValorCampo('$Code$CaseNum', $Processo, $CaseNum, $campo, '$Tipo', 1);";
                }
                break;
            case "TB":
                $Valor = "PegaValorCampo('$Code$CaseNum', $Processo, $CaseNum, $campo, '$Tipo', 0);";
                break;
            case "US":
                if (empty($Valor)) {
                    $Valor = "PegaValorCampo('$Code$CaseNum', $Processo, $CaseNum, $campo, 'US', 1);";
                }
                break;
            case "GR":
                $Valor = "PegaValorCampo('$Code$CaseNum', $Processo, $CaseNum, $campo, 'GR', 0);";
                break;
            case "BO":
                if ($Valor == 1 || $Valor == "Sim") {
                    $Valor = "Sim";
                } else {
                    $Valor = "N�o";
                }
                break;

            case "DT":
                if (empty($Valor)) {
                    $Valor = "PegaValorCampo('$Code$CaseNum', $Processo, $CaseNum, $campo, 'DT', 1);";
                }
                break;
        }
        return $Valor;
    }

    function PegaMascaraCampoReferencia($Campo)
    {
        global $S_procdef;
        for ($i = 0; $i < count($S_procdef["exportkeys"]); $i++) {
            if ($S_procdef["exportkeys"][$i]["FieldId"] == $Campo) {
                return $S_procdef["exportkeys"][$i]["FieldMask"];
            }
        }
        return "";
    }

    function PegaTipoCampoReferencia($Campo)
    {
        global $S_procdef;
        for ($i = 0; $i < count($S_procdef["exportkeys"]); $i++) {
            if ($S_procdef["exportkeys"][$i]["FieldId"] == $Campo) {
                return $S_procdef["exportkeys"][$i]["FieldType"];
            }
        }
        return "TX";
    }

    function PegaNomeCampoReferencia($Campo)
    {
        global $S_procdef;
        if ($Campo == "CaseNum") {
            return "N�mero do Caso";
        }
        for ($i = 0; $i < count($S_procdef["exportkeys"]); $i++) {
            if ($S_procdef["exportkeys"][$i]["FieldId"] == $Campo) {
                return $S_procdef["exportkeys"][$i]["FieldName"];
            }
        }
        return "";
    }

    function PegaCamposReferenciaProcesso($ProcId)
    {
        global $connect, $S_procdef;

        if (!is_numeric($ProcId)) {
            $ProcId = PegaProcIdByCode($ProcId);
        }

        if ($S_procdef["ProcId"] == $ProcId) {
            if (is_array($S_procdef["Referencias"])) {
                if (count($S_procdef["Referencias"]) > 0) {
                    return;
                }
            }
        }
        $_SESSION["S_procdef"] = $S_procdef;
        $S_procdef["ProcId"] = $ProcId;
        $S_procdef["Referencias"] = array();
        $S_procdef["exportkeys"] = array();
        $SQL = " select ";
        $SQL .= "  FieldName, FieldId, FieldType, FieldRef, FieldSourceTable, FieldSourceField, FieldDisplayField, FieldCod, FieldMask  ";
        $SQL .= "  from ";
        $SQL .= "  procfieldsdef ";
        $SQL .= "  where ";
        $SQL .= "  (FieldKey <> 0 or FieldRef <> 0)";
        $SQL .= "  and ";
        $SQL .= "  ProcId = $ProcId ";
        $SQL .= "  and ";
        $SQL .= "  Active = 1";
        $SQL .= "  order by ";
        $SQL .= "  FieldRef desc, ";
        $SQL .= "  FieldRefOrder, ";
        $SQL .= "  FieldName ";
        $QUERY1 = mysqli_query($connect, $SQL);
        while ($Result = mysqli_fetch_array($QUERY1)) {
            if ($Result["FieldRef"] == 1) {
                array_push($S_procdef["Referencias"], $Result);
            }
            array_push($S_procdef["exportkeys"], $Result);
        }
    }

    function PegaReferencias($ProcId, $CaseNum, $inArray = 0, $ExportKey = 0, $StepCode = 0, $numRef = 0)
    {
        global $connect, $S_procdef;

        PegaCamposReferenciaProcesso($ProcId);
        $Key = 0;
        $CamposSQL = array();
        foreach ($S_procdef["Referencias"] as $Ref) {
            array_push($CamposSQL, "CampoDisplay" . $Ref["FieldId"]);
        }

        if (count($CamposSQL) > 0) {
            $CamposSQL = implode(",", $CamposSQL);
            $SQL = "select $CamposSQL from exportkeysdisplay where CaseNum = $CaseNum and ProcId = $ProcId";
            $Qryvalores = mysqli_query($connect, $SQL);
        }

        if (!$Qryvalores) {
            $SQL = "select * from exportkeysdisplay where CaseNum = $CaseNum and ProcId = $ProcId";
            $Qryvalores = mysqli_query($connect, $SQL);
        }
        $valores = mysqli_fetch_array($Qryvalores);
        for ($i = 0; $i < count($S_procdef["Referencias"]); $i++) {
            if ($numRef != 0 & $i > $numRef - 1) { // Tras apenas o N�mero de Referencias selecionado
                break;
            }
            $LINHA1 = $S_procdef["Referencias"][$i];
            $Valor = trim($valores["CampoDisplay" . $LINHA1['FieldId']]);
            $Valor = str_replace("/*%/*", "'", $Valor);
            $Valor = str_replace("/*#/*", ";", $Valor);
            $Valor = str_replace("/*!/*", ",", $Valor);
            $Valor = str_replace("&", "&amp;", $Valor);
            ;
            $Valor = str_replace("<", "&lt;", $Valor);
            ;
            $Valor = str_replace(">", "&gt;", $Valor);
            ;
            if (empty($Valor)) {
                $Retorno[$Key]['Empty'] = true;
            }
            if ($inArray == 0) {
//			$Retorno[$contador] = "&nbsp;<span class=\"texto1blackBold\" >$Campo:</span><span class=\"texto1black\"> $Valor </span>";
                $Retorno[$Key] = FormataCampo($Valor, $LINHA1["FieldType"], $LINHA1['FieldId']);
            } else {
                if ($ExportKey == 1) {
                    $Retorno[$Key]['FieldName'] = "Campo" . $LINHA1["FieldId"];
                } else {
                    $Retorno[$Key]['FieldName'] = $LINHA1["FieldName"];
                }
                $Retorno[$Key]['FieldCod'] = $LINHA1["FieldCod"];
                $Retorno[$Key]['FieldValue'] = ValorCampoQueue($Valor, $LINHA1["FieldType"], $LINHA1['FieldId'], $CaseNum, $LINHA1["FieldCod"] . $StepCode);
            }
            $Key++;
        }
        return $Retorno;
    }

    function PegaArrayReferencias($ProcId, $ACaseNum, $retornarComoArray = 0, $ExportKey = 0, $StepCode = 0, $retornarFuncaoValorCampo = true)
    {
        global $connect, $S_procdef, $AReferencias;

        if (!is_numeric($ProcId)) {
            $ProcId = PegaProcIdByCode($ProcId);
        }

        if (count($ACaseNum) == 0) {
            return;
        }
        $SCaseNum = implode(",", $ACaseNum);
        if (empty($SCaseNum)) {
            return;
        }

        PegaCamposReferenciaProcesso($ProcId);
        $CamposSQL = array();
        array_push($CamposSQL, "CaseNum");
        foreach ($S_procdef["Referencias"] as $Ref) {
            array_push($CamposSQL, "CampoDisplay" . $Ref["FieldId"]);
        }

        $Qryvalores = false;
        if (count($CamposSQL) > 0) {
            $CamposSQL = implode(",", $CamposSQL);
            $SQL = "select $CamposSQL, CaseNum from exportkeysdisplay where ProcId = $ProcId and CaseNum in ($SCaseNum)";
            $Qryvalores = mysqli_query($connect, $SQL);
        }


        if (!$Qryvalores | $Qryvalores->numrows == 0) {
            $SQL = "select * from exportkeysdisplay where CaseNum in ($SCaseNum) and ProcId = $ProcId";
            $Qryvalores = mysqli_query($connect, $SQL);
        }

        if (!$Qryvalores) {
            error_log("Falha ao pegar campos referência: $SQL");
            return;
        }
        $Total_Referencias = count($S_procdef["Referencias"]);
        while ($valores = mysqli_fetch_array($Qryvalores)) {
            $Key = 0;
            $Retorno = array();
            for ($i = 0; $i < $Total_Referencias; $i++) {
                $Retorno[$Key]['Empty'] = false;
                $LINHA1 = $S_procdef["Referencias"][$i];
                $campoDisplay = "CampoDisplay" . $LINHA1['FieldId'];
                $Valor = $valores[$campoDisplay];
                $Valor = trim($Valor);
                $Valor = str_replace("/*%/*", "'", $Valor);
                $Valor = str_replace("/*#/*", ";", $Valor);
                $Valor = str_replace("/*!/*", ",", $Valor);
                $Valor = str_replace("&", "&amp;", $Valor);
                $Valor = str_replace("<", "&lt;", $Valor);
                $Valor = str_replace(">", "&gt;", $Valor);

                $fieldEmpty = empty($Valor);
                $fieldValue = $Valor;
                $fieldName = $LINHA1["FieldName"];
                if ($retornarComoArray == 0 | !$retornarFuncaoValorCampo) {
                    if (!$retornarFuncaoValorCampo) {
                        if ($Retorno[$Key]['Empty']) {
                            $fieldValue = FormataCampo($Valor, $LINHA1["FieldType"], $LINHA1['FieldId']);
                        }
                    } else {
                        if ($Retorno[$Key]['Empty']) {
//$Retorno[$Key] = FormataCampo($Valor, $LINHA1["FieldType"], $LINHA1['FieldId']);
                            $fieldValue = FormataCampo($Valor, $LINHA1["FieldType"], $LINHA1['FieldId']);
                        }
                    }
                } else {
                    if ($ExportKey == 1) {
                        $FieldName = "Campo" . $LINHA1["FieldId"];
                        $fieldName = $FieldName;
                    }
                    if ($fieldEmpty) {
                        $fieldValue = ValorCampoQueue($Valor, $LINHA1["FieldType"], $LINHA1['FieldId'], $valores["CaseNum"], $LINHA1["FieldCod"] . $StepCode);
                    }
                }
                $Retorno[$Key]['FieldName'] = $fieldName;
                $Retorno[$Key]['FieldId'] = $LINHA1["FieldId"];
                $Retorno[$Key]['FieldCod'] = $LINHA1["FieldCod"];
                $Retorno[$Key]['FieldValue'] = $fieldValue;
                $Retorno[$Key]['Empty'] = $fieldEmpty;
                $Key++;
            }
            $AReferencias[$valores["CaseNum"]] = $Retorno;
        }
        return $AReferencias;
    }

    function FormataCampoRef($ProcId, $Campo, $Valor, $Tipo = "TX", $Mascara = 1)
    {
        if ($Mascara == 1) {
            switch ($Tipo) {
                case "DC":
                    $SourceData = PegaDadosCampoTabela($ProcId, $Campo);
                    $Valor = PegaValorCampo($SourceData[0], $Valor, $SourceData[2], "TXT", 1);
                    break;
                case "LT":
                    $Valor = PegaValorCampoLista($ProcId, $Campo, $Valor);
                    break;
                case "TB":
                    $SourceData = PegaDadosCampoTabela($ProcId, $Campo);
                    $Valor = PegaValorCampoTabela($Valor, $SourceData[0], $SourceData[1], $SourceData[2], $SourceData[3]);
                    break;
                case "US":
                    $Valor = PegaNomeUsuario($Valor);
                    break;
                case "GR":
                    $Valor = PegaValorCampoTabela($Valor, "groupdef", "GroupId", "GroupName");
                    break;
                case "BO":
                    if ($Valor == 1) {
                        $Valor = "Sim";
                    } else {
                        $Valor = "N�o";
                    }
                    break;
                case "DT":
                    $Valor = ConvDate($Valor, false);
                    break;
                case "TM";
// N�o muda
                    break;
                case "SFR":
                    $Valor = PegaNomeStep($ProcId, $Valor);
                    break;
            }
        }
        return $Valor;
    }


    function PegaUltimoValorCampoMultiLinhas($ProcId, $CaseNum, $Campo)
    {
        global $connect;
        $SQL = "select FieldValue from casedata where ProcId = $ProcId and CaseNum = $CaseNum and FieldId = $Campo order by ValueId desc";
        $Query = mysqli_query($connect, $SQL);
        $Result = mysqli_fetch_array($Query);
        if ($Result) {
            return ExplodeValueTickler($Result["FieldValue"], 0);
        }
        return false;
    }

    function PegaValorConstant($ProcId, $Codigo)
    {
        global $connect;
        $SQL = "select ConstValue from internalconstants where ProcId = $ProcId and ConstCode = '$Codigo'";
        $Query = mysqli_query($connect, $SQL);
        $Result = mysqli_fetch_array($Query);
        return $Result["ConstValue"];
    }

    function ParsevaloresCampos($ProcId, $CaseNum, $contents)
    {
        if (strpos($contents, '!%') === false && strpos($contents, '!$') === false && strpos($contents, '!{') === false || ((substr_count($contents, '!$') % 2) != 0 && (substr_count($contents, '!%') % 2) != 0 && strpos($contents, '!{') === false )) {
            return $contents;
        }

        if (substr_count($contents, '!{') != substr_count($contents, '}!')) {
            return $contents;
        }

        while (strpos($contents, '!{') !== false && (substr_count($texto, '!{') == substr_count($texto, '}!'))) {
            $inicio = strpos($contents, '!{');
            $fim = strpos($contents, '}!', $inicio + 2);
            $CodigoCampo = substr($contents, $inicio + 2, $fim - $inicio - 2);
            $Valor = PegaValorConstant($ProcId, $CodigoCampo);
            $contents = str_replace("!{" . $CodigoCampo . "}!", $Valor, $contents);
        }

        while (strpos($contents, '!$') !== false && (substr_count($texto, '!$') % 2) == 0) {
            $inicio = strpos($contents, '!$');
            $fim = strpos($contents, '!$', $inicio + 2);
            $CodigoCampo = substr($contents, $inicio + 2, $fim - $inicio - 2);
            $Valor = ParseValorCampo($ProcId, $CaseNum, $CodigoCampo, '', 0);
            $contents = str_replace("!$" . $CodigoCampo . "!$", $Valor, $contents);
        }

        while (strpos($contents, '!%') !== false && (substr_count($texto, '!%') % 2) == 0) {
            $inicio = strpos($contents, '!%');
            $fim = strpos($contents, '!%', $inicio + 2);
            $CodigoCampo = substr($contents, $inicio + 2, $fim - $inicio - 2);
            $Valor = ParseValorCampo($ProcId, $CaseNum, $CodigoCampo, '', 1);
            $contents = str_replace("!%" . $CodigoCampo . "!%", $Valor, $contents);
        }
        return $contents;
    }

    function ParseValorCampo($Processo, $Caso, $campo, $Tipo, $Mascara = 0)
    {
        if ($campo == "SYSTEMDATETIME" | $campo == "DATA") {
            return date("d/m/Y H:i");
        }

        if ($campo == "SYSTEMDATE" | $campo == "DATA") {
            return date("d/m/Y");
        }

        if ($campo == "SYSTEMTIME" | $campo == "DATA") {
            return date("H:i");
        }
        return PegaValorCampo($Processo, $Caso, $campo, $Tipo, $Mascara);
    }

    function PegaTipoCampo($Processo, $campo)
    {
        global $connect;
        if (!is_numeric($Processo)) {
            $Processo = PegaProcIdByCode($Processo);
        }

        if (!is_numeric($campo)) {
            $campo = PegaFieldIdByCode($Processo, $campo);
        }

        $SQL = "select FieldType from procfieldsdef where ProcId = $Processo and FieldId = $campo";
        $Query = mysqli_query($connect, $SQL);
        $Result = mysqli_fetch_array($Query);
        return $Result["FieldType"];
    }

    /**
     * 
     * @global type $connect
     * @param type $Processo
     * @param type $Caso
     * @param type $campo
     * @param type $TipoCampo
     * @param type $Mascara
     * @param type $HTML
     * @param type $valores
     * @param type $DefaultValue
     * @return type
     */
    function PegaValorCampo($Processo, $Caso, $campo, $TipoCampo = "ND", $Mascara = 0, $HTML = 1, $valores = "", $DefaultValue = "")
    {
        global $connect;
        AtivaDBProcess();

        if (!is_numeric($Processo)) {
            $Processo = PegaProcIdByCode($Processo);
        }

        if (!is_numeric($campo)) {
            $campo = PegaFieldIdByCode($Processo, $campo);
        }

        if ($TipoCampo == '' && $Mascara == 1) {
            $TipoCampo = PegaTipoCampo($Processo, $campo);
        }

        if ($TipoCampo == 'ND' && $Mascara == 0) {
            $TipoCampo = 'TX';
        }

        if (!is_array($valores) || in_array($TipoCampo, array('IM', 'AR', 'TM', 'TK', 'IF', 'IT', 'STS', 'PRC', 'EXT'))) {
            $SQL = " SELECT ";
            $SQL .= " FieldValue, ExtendData, ValueId";
            $SQL .= " FROM";
            $SQL .= " casedata";
            $SQL .= " where";
            $SQL .= " fieldid = $campo";
            $SQL .= " and ";
            $SQL .= " procid = $Processo ";
            $SQL .= " and ";
            $SQL .= " casenum = $Caso ";
            $QUERY2 = mysqli_query($connect, $SQL);
            if (!$QUERY2) {
                error_log("Falha ao capturar valor de campo \"$campo\" Processo \"$Processo\" Caso \"$Caso\" Script: " . $_SERVER['SCRIPT_NAME']);
            }
            if (in_array($TipoCampo, array("IF", "IT"))) {
                $Valor = mysqli_fetch_all($QUERY2, MYSQLI_ASSOC);
            } else {
                $linha = mysqli_fetch_array($QUERY2, MYSQLI_ASSOC);
                $Valor = $linha["FieldValue"];
            }
            if (in_array($TipoCampo, array("AR", "IM", "EXT"))) {
                $extendData = $linha["ExtendData"];
                if ($extendData != "") {
                    $Valor = $extendData;
                }
            }
            mysqli_free_result($QUERY2);
        } else {
            $Valor = $valores[$campo];
        }

        if (!is_array($Valor)) {
            $Valor = trim($Valor);
            $Valor = str_replace("/*%/*", "'", $Valor);
            $Valor = str_replace("/*#/*", ";", $Valor);
        }

        if ($Valor <> "") {
            if ($Mascara == 1) {
                switch ($TipoCampo) {
                    case "DC":
                        $SourceData = PegaDadosCampoTabela($Processo, $campo);
                        $Valor = PegaValorCampo($SourceData[0], $Valor, $SourceData[2], "TXT", 1);
                        break;
                    case "LT":
                        $Valor = PegaValorCampoLista($Processo, $campo, $Valor);
                        break;
                    case "TB":
                        $SourceData = PegaDadosCampoTabela($Processo, $campo);
                        $Valor = PegaValorCampoTabela($Valor, $SourceData[0], $SourceData[1], $SourceData[2], $SourceData[3]);
                        break;
                    case "US":
                        $Valor = PegaNomeUsuario($Valor);
                        break;
                    case "GR":
                        $Valor = PegaValorCampoTabela($Valor, "groupdef", "GroupId", "GroupName");
                        break;
                    case "DT":
                        $Valor = ConvDate($Valor, false);
                        break;
                    case "BO":
                        if ($Valor == 1) {
                            $Valor = "Sim";
                        } else {
                            $Valor = "Não";
                        }
                        break;
                    case "DT":
                        $Valor = ConvDate($linha["fieldvalue"], false);
                        break;
                    case "TM";
// Não muda
                        break;
                }
            }
        } else {
            if ($TipoCampo == "BO") {
                $Valor = PegaDefaultValue($Processo, $campo, $TipoCampo, $DefaultValue);
                if ($Mascara == 1) {
                    if ($Valor == 1) {
                        $Valor = "Sim";
                    } else {
                        $Valor = "Não";
                    }
                }
            } else {
                $Valor = PegaDefaultValue($Processo, $campo, $TipoCampo, $DefaultValue);
            }
        }


        if ($Tipo == STS) {
            $Valor = PegaStatusProcessoDefault($Processo);
        }

        if (!in_array($Tipo, array("AR", "IM"))) {
            if ($HTML == 1) {
                $Valor = $Valor;
            } else {

                $Valor = addslashes($Valor);
            }
        }


        if (!is_array($Valor)) {
            $Valor = trim($Valor);
        }
        return $Valor;
    }

    function DateDB($Data)
    {
        if (!empty($Data)) {
            $Data = substr($Data, 6, 4) . "-" . substr($Data, 3, 2) . "-" . substr($Data, 0, 2);
        }
        return $Data;
    }

    function TrataMetaSubstituicao($DefaultValue)
    {
        $MetaDado = array();
        $retorno = $DefaultValue;
        $encontrados = preg_match_all('/!{.*?}/m', $DefaultValue, $MetaDados);
        if ($encontrados == 0) {
            return $retorno;
        }
        foreach ($MetaDados[0] as $MetaDado) {
            switch ($MetaDado) {
                case "!{LONGDATE}":
                    setlocale(LC_TIME, 'pt_BR', 'pt_BR.utf-8', 'pt_BR.utf-8', 'portuguese');
                    date_default_timezone_set('America/Sao_Paulo');
                    $substituicao = strftime('%d de %B de %Y %H:%M', strtotime('now'));
                    break;
                case "!{DATE}":
                    $substituicao = strftime('%d/%m/%Y', strtotime('now'));
                    break;
                case "!{TIME}":
                    $substituicao = strftime('%H:%M', strtotime('now'));
                    break;
                case "!{DATETIME}":
                    $substituicao = strftime('%d/%m/%Y %H:%M', strtotime('now'));
                    break;
                case "!{CASENUM}":
                    global $CaseNum;
                    $substituicao = $CaseNum;
                    break;
                default:
                    $substituicao = "";
            }
            $retorno = str_replace($MetaDado, $substituicao, $retorno);
        }
        return $retorno;
    }

    function PegaDefaultValue($ProcId, $campo, $TipoCampo, $DefaultValue)
    {
        global $connect;
        $DefaultValue = trim($DefaultValue);
        if ($DefaultValue != "") {
            if ((strpos(strtolower($DefaultValue), 'date("y-m-d")') !== false)) {
                $valor = '';
                $Eval = ' $valor = date("Y-m-d")' . ";";
                eval($Eval);
                return $valor;
            }

            if ((strpos(strtolower($DefaultValue), "date(\"h:i\")") !== false)) {
                $valor = '';
                $Eval = ' $valor = date("H:i")' . ";";
                eval($Eval);
                return $valor;
            }
            if ($TipoCampo == 'DT') {
                $Eval = '$valor = ' . $DefaultValue . ';';
                if (@!eval($Eval)) {
                    $valor = $DefaultValue;
                }
            } else {
                $valor = TrataMetaSubstituicao($DefaultValue);
            }
        } else {
            $valor = '';
        }
        return $valor;
    }

    function pegaTotalMensagens($UserId)
    {
        global $connect;
        $SQL = "Select ";
        $SQL .= "  count(*) as Total";
        $SQL .= "  from ";
        $SQL .= "  procdef, ";
        $SQL .= "  casedata ";
        $SQL .= "  where ";
        $SQL .= "  procdef.TipoProc = 'WG' ";
        $SQL .= "  and ";
        $SQL .= "  casedata.FieldId = 6 ";
        $SQL .= "  and ";
        $SQL .= "  FieldValue = $UserId ";
        $SQL .= "  and ";
        $SQL .= "  casedata.ProcId = procdef.ProcId ";
        $SQL .= "  and ";
        $SQL .= "  casedata.ProcId = procdef.ProcId ";
        $QUERY = mysqli_query($connect, $SQL);
        $result = mysqli_fetch_array($QUERY);
        $Total = $result['Total'];
        mysqli_free_result($QUERY);
        if ($Total == 0) {
            return 'Nenhuma Mensagem';
        } else {
            if ($Total == 1) {
                return '1 Mensagem';
            } else {
                return $Total . ' Mensagems';
            }
        }
    }

    function Lido($ProcId, $StepId, $CaseNum)
    {
        global $lidos;
        $naolido = 'style="border: 1px dotted #FF0000;"';
        echo 'id="$ProcId$StepId$CaseNum"';
        if (is_array($lidos)) {
            if (!in_array("$ProcId-$StepId-$CaseNum", $lidos)) {
                echo $naolido;
            }
        } else {
            echo $naolido;
        }
    }

    function CorLinha($cor)
    {
        if ($cor == "linha2") {
            $cor = "linha1";
        } else {
            $cor = "linha2";
        }
        echo "class='$cor'";
        return $cor;
    }

    function TotalCasosGuest($ProcId)
    {
        global $connect;
        $SQL .= " select  distinct ";
        $SQL .= "  count(*) as Total  ";
        $SQL .= " from  ";
        $SQL .= " casequeue,  ";
        $SQL .= " procdef,  ";
        $SQL .= " stepdef  ";
        $SQL .= " where  ";
        $SQL .= " procdef.ProcId = $ProcId ";
        $SQL .= " and ";
        $SQL .= " casequeue.ProcId = procdef.ProcId ";
        $SQL .= " and ";
        $SQL .= " casequeue.StepId > 0 ";
        $SQL .= " and ";
        $SQL .= " stepdef.ProcId = procdef.ProcId ";
        $SQL .= " and ";
        $SQL .= " stepdef.StepId = casequeue.StepId ";
        $QUERY = mysqli_query($connect, $SQL);
        $linha = mysqli_fetch_array($QUERY);
        return $linha["Total"];
    }

    function trataFiltroCampoDataIntervalo($dataSelecionada, $Campo)
    {
        $filtros = array();
        $data = $dataSelecionada[0];
        $dataFormatada = FormataData($data);
        $filtros[] = "Campo$Campo >= '$dataFormatada 00:00'";

        $data = $dataSelecionada[1];
        $dataFormatada = FormataData($data);
        $filtros[] = "Campo$Campo <= '$dataFormatada 23:59'";
        return $filtros;
    }

    function trataFiltroCampoSimples($dataSelecionada, $Campo)
    {
        $dataFormatada = FormataData($dataSelecionada);
        if (strrpos($dataFormatada, ":")) {
            if (substr($dataFormatada, strrpos($dataFormatada, ":") + 1, 2) == '00') {
                $data = substr($dataFormatada, 0, strrpos($dataFormatada, ":"));
                $Filtro = "convert(datetime,Campo$Campo) >= '$data:00'";
                $Filtro .= " and ";
                $Filtro .= "convert(datetime,Campo$Campo) <= '$data:59'";
            } else {
                $Filtro = "convert(datetime, Campo$Campo) = 'Valor'";
            }
        } else {
            $Filtro = " Campo$Campo >= '$dataFormatada 00:00" . "'";
            $Filtro .= " and ";
            $Filtro .= "Campo$Campo <= '$dataFormatada 23:59" . "'";
        }
        return $Filtro;
    }

    /**
     * 
     * @param type $dataSelecionada
     */
    function trataFiltroCampoData($dataSelecionada, $Campo)
    {
        if (is_array($dataSelecionada)) {
            $filtro = trataFiltroCampoDataIntervalo($dataSelecionada, $Campo);
        } else {
            $filtro = trataFiltroCampoSimples($dataSelecionada, $Campo);
        }
        return $filtro;
    }

    function TrataFiltros($Filtros, $ProcId = 0)
    {
        $retorno = array();
        foreach ($Filtros as $filtro) {
            if (key_exists("filter", $filtro)) {
                $filtro = $filtro["filter"];
            }
            if (key_exists("filtro", $filtro)) {
                $filtro = $filtro["filtro"];
            }
            $Valor = $filtro["valor"];
            //$Valor = trim($filtro["valor"]);


            $Campo = $filtro["campo"];
            if (key_exists("tipoCampo", $filtro)) {
                $tipoCampo = $filtro["tipoCampo"];
            }
            if (key_exists("tipo", $filtro)) {
                $tipoCampo = $filtro["tipo"];
            }

            if (!is_numeric($Campo)) {
                $Campo = PegaFieldIdByCode($ProcId, $Campo);
            }

            /**
             * Cria Filtro pelo CaseNum        
             */
            if ($Campo == "CaseNum") {
                $retorno[] = "exportkeys.CaseNum = '$Valor'";
                continue;
            }

            /**
             * Trata Filtro outros campos
             */
            switch ($tipoCampo) {
                case "DT":
                    $Filtro = trataFiltroCampoData($Valor, $Campo);
                    break;
                case "TX":
                    $Filtro = "Campo$Campo like '%$Valor%'";
                    break;
                default:
                    $Filtro = "Campo$Campo = '$Valor'";
            }

            /**
             * Verifica se o filtro é um array e faz o merge
             */
            if (is_array($Filtro)) {
                $retorno = array_merge($retorno, $Filtro);
            } else {
                $retorno[] = $Filtro;
            }
        }

        return $retorno;
    }

    function TrataCampoOrdem($Campo)
    {
        global $S_procdef;
        $Campo = trim($Campo);
        if (empty($Campo)) {
            return '';
        }
        $key = str_replace("Campo", "", $Campo);
        if (!is_array($S_procdef["Referencias"])) {
            $S_procdef["Referencias"] = [];
        }
        for ($i = 0; $i < count($S_procdef["Referencias"]); $i++) {
            if ($S_procdef["Referencias"][$i]["FieldId"] == $key) {
                if ($S_procdef["Referencias"][$i]["FieldType"] == 'DT') {
                    $Campo = "convert(datetime, $Campo)";
                }
                if ($S_procdef["Referencias"][$i]["FieldType"] == 'NU') {
                    $Campo = "convert(float, $Campo)";
                }
            }
        }
        return $Campo;
    }

    function NovosCasosUsuario($ProcCode = "", $Origem = "")
    {
        global $connect, $userdef;
        require_once('sqlqueue.php');
        $Filtros[0] = "insertdate >= '" . $userdef->lastnotification . "'";

        if (!is_numeric($ProcCode)) {
            $ProcId = PegaProcIdByCode($ProcCode);
        }

        $SQLUnion = ")";
        $SQLUnion .= "union";
        $SQLUnion .= "( \n";
        $SQL = "(";
        $Campos[0] = " count(*) as total ";

        $StepId = "";
        $HideQueue = false;
        $Campo = "";
        $ViewQueue = "1";

        $SQL .= SQLCasosGrupos($ProcId, $Campos, $StepId, $HideQueue, $Campo, $Origem, $Filtros, $ViewQueue);
        $SQL .= $SQLUnion;
        $SQL .= SQLCasosUsuario($ProcId, $Campos, $StepId, $HideQueue, $Campo, $Origem, $Filtros, $ViewQueue);

        $SQL1 = $SQLUnion;
        $SQL1 .= SQLCasosCampoUsuario($ProcId, $Campos, $StepId, $HideQueue, $Campo, $Origem, $Filtros, $ViewQueue);
        $SQL1 .= $SQLUnion;

        $SQL2 = SQLCasosCampoGrupo($ProcId, $Campos, $StepId, $HideQueue, $Campo, $Origem, $Filtros, $ViewQueue);
        $SQL2 = $SQL2;
//$SQL3 = $SQLUnion . SQLCasosAdHoc($ProcId, $Campos, $StepId, $HideQueue, $Campo, $Origem, $Filtros, $ViewQueue);
        $SQL3 = "";
        $SQL4 = $SQL . $SQL1 . $SQL2 . $SQL3;

        $SQL4 .= ")";

//$SQL_DADOS = "select * from casequeue";
//$SQL_DADOS = " select sum(total) as total from ($SQL4) as notificacoes ";
        $SQL_DADOS = " select sum(total) as total from ($SQL4) as notificacoes ";
//echo $SQL_DADOS;

        $retorno = mysqli_query($connect, $SQL_DADOS);
        return $retorno;
    }

    function PermissoesDoCasoNoPasso($procId, $stepId, $caseNum)
    {
        global $connect;
        $filtros = array();
        $filtro = array();
// Coloca o Filtro para Pegar as Acoes permitidas no Caso
        $filtro["Campo"] = "CaseNum";
        $filtro["Valor"] = $caseNum;
        array_push($filtros, $filtro);

        $CamposSelect = array("Action");

        $SQL = MontaSQLCasosNaFila($procId, $stepId, false, "", "", "", $filtros, 1, $CamposSelect);
        $Query = mysqli_query($connect, $SQL);
        $resultado = false;
        while ($Linha = mysqli_fetch_array($Query)) {
            $resultado = $resultado | ($Linha["Action"] == 1);
        }
        $retorno = ($resultado) ? 1 : 0;
        return $retorno;
    }

    function MontaSQLCasosNaFila($ProcId, $StepId, $HideQueue, $CampoOrdem, $Ordem, $Origem, $Filtros, $ViewQueue, $CamposSQLQueue)
    {
        $SQLUnion = ")";
        $SQLUnion .= " union ";
        $SQLUnion .= "( \n";

        $SQL = "(";

        $FiltrosTratados = "";
        if (is_array($Filtros)) {
            $FiltrosTratados = TrataFiltros($Filtros);
        }

        $Campos = $CamposSQLQueue;


        $SQL .= SQLCasosGrupos($ProcId, $Campos, $StepId, $HideQueue, $CampoOrdem, $Origem, $FiltrosTratados, $ViewQueue);

        $SQL1 = $SQLUnion;
        $SQL1 .= SQLCasosCampoUsuario($ProcId, $Campos, $StepId, $HideQueue, $CampoOrdem, $Origem, $FiltrosTratados, $ViewQueue);
        $SQL1 .= $SQLUnion;

        $SQL2 = SQLCasosCampoGrupo($ProcId, $Campos, $StepId, $HideQueue, $CampoOrdem, $Origem, $FiltrosTratados, $ViewQueue);
        $SQL2 = $SQL2;
//$SQL3 = $SQLUnion . SQLCasosAdHoc($ProcId, $Campos, $StepId, $HideQueue, $CampoOrdem, $Origem, $FiltrosTratados, $ViewQueue);

        $SQL4 = $SQL . $SQL1 . $SQL2 . $SQL3;
        $SQL4 .= ")";


        /*
          $SQL .= SQLCasosGrupos($ProcId, $Campos, $StepId, $HideQueue, $CampoOrdem, $Origem, $FiltrosTratados, $ViewQueue);
          $SQL .= $SQLUnion;
          $SQL .= SQLCasosUsuario($ProcId, $Campos, $StepId, $HideQueue, $CampoOrdem, $Origem, $FiltrosTratados, $ViewQueue);

          $SQL1 = $SQLUnion;
          $SQL1 .= SQLCasosCampoUsuario($ProcId, $Campos, $StepId, $HideQueue, $CampoOrdem, $Origem, $FiltrosTratados, $ViewQueue);
          $SQL1 .= $SQLUnion;

          $SQL2 = SQLCasosCampoGrupo($ProcId, $Campos, $StepId, $HideQueue, $CampoOrdem, $Origem, $FiltrosTratados, $ViewQueue);
          $SQL2 = $SQL2 . $SQLUnion;
          $SQL3 = SQLCasosAdHoc($ProcId, $Campos, $StepId, $HideQueue, $CampoOrdem, $Origem, $FiltrosTratados, $ViewQueue);

          $SQL4 = $SQL . $SQL1 . $SQL2 . $SQL3;
          $SQL4 .= ")";
         */

//    $SQL4 = SQLCasosGrupos($ProcId, $Campos, $StepId, $HideQueue, $CampoOrdem, $Origem, $FiltrosTratados, $ViewQueue);
        $SQLOrdem = " ";
        if (!empty($CampoOrdem)) {
            $posPonto = strpos($CampoOrdem, ".");
            if ($posPonto > - 1) {
                $CampoOrdem = substr($CampoOrdem, $posPonto);
            }
            $SQLOrdem .= " order by $CampoOrdem $Ordem ";
        }
        $SQL4 .= $SQLOrdem;
        return $SQL4;
    }

    /**
     * 
     * @global type $connect
     * @param type $ProcId
     * @param type $StepId
     * @param type $HideQueue
     * @param type $CampoOrdenacao
     * @param type $Ordem
     * @param type $Origem
     * @param type $Filtros
     * @param type $ViewQueue
     * @param type $RetornarCampos
     * @return type
     */
    function pegaListaCasosNaFila($ProcId, $StepId = '-1', $HideQueue = 1, $CampoOrdenacao = '', $Ordem = '', $Origem = '', $Filtros = '', $ViewQueue = 1, $RetornarCampos = true)
    {
        global $connect;
        require_once('sqlqueue.php');

        if (!is_numeric($ProcId)) {
            $ProcId = PegaProcIdByCode($ProcId);
        }

        if (!is_numeric($StepId) & $StepId != '') {
            $StepId = PegaStepIdByCode($ProcId, $StepId);
        }

        $CamposSQL = CamposSQLQueue($CampoOrdenacao, $RetornarCampos);
        $CampoOrdem = TrataCampoOrdem($CampoOrdenacao);
        $SQL = MontaSQLCasosNaFila($ProcId, $StepId, $HideQueue, $CampoOrdem, $Ordem, $Origem, $Filtros, $ViewQueue, $CamposSQL);
        $retorno = mysqli_query($connect, $SQL);
        if (mysqli_error($connect)) {
            $error = "Casos na Fila " . mysqli_error($connect);
            error_log($error);
            error_log("SQL casos na fila: $SQL");
        }
        return $retorno;
    }

    function PassosDoUsuario($ProcId, $Origem, $Filtros = "")
    {
        global $connect, $procdef;

        if ($procdef == "") {
            $procdef = new procdef();
            $procdef->create($ProcId, $connect);
        }
        require_once('sqlqueue.php');
        if (is_array($Filtros)) {
            $Filtros = TrataFiltros($Filtros, $ProcId);
        }
        $Campos[0] = " stepaddresses.StepId ";
        $SQLUnion = ")";
        $SQLUnion .= " union ";
        $SQLUnion .= "( \n";
        $SQL .= '(';
        $SQL .= SQLCasosGrupos($ProcId, $Campos, '', 1, '', $Origem, $Filtros);
        $SQL .= " group by stepaddresses.StepId";
        $SQL .= $SQLUnion;
        $SQL .= SQLCasosUsuario($ProcId, $Campos, '', 1, '', $Origem, $Filtros);
        $SQL .= " group by stepaddresses.StepId";
        $SQL .= $SQLUnion;
        $SQL .= SQLCasosCampoUsuario($ProcId, $Campos, '', 1, '', $Origem, $Filtros);
        $SQL .= " group by stepaddresses.StepId";
        $SQL .= $SQLUnion;
        $SQL .= SQLCasosCampoGrupo($ProcId, $Campos, '', 1, '', $Origem, $Filtros);
//    $SQL .= $SQLUnion;
//    $SQL .= SQLCasosAdHoc($ProcId, $Campos, '', 1, '', $Origem, $Filtros);
        $SQL .= " group by stepaddresses.StepId";
        $SQL .= ")";
        $QUERY = mysqli_query($connect, $SQL);
        $Steps = array();
        while ($Result = mysqli_fetch_array($QUERY)) {
            $StepId = $Result["StepId"];
            if ($StepId == 0) {
                $StepId = PegaDefaultStep($ProcId);
            }
            $StepCode = $procdef->Steps[$StepId]["StepCod"];
            $StepName = $procdef->Steps[$StepId]["StepName"];
            $Result["StepCode"] = $StepCode;
            $Result["StepName"] = $StepName;
            array_push($Steps, $Result);
        }
        return $Steps;
    }

    function NovoProcessosUsuario($ViewQueue = 1)
    {
        global $connect, $userdef;
        $SQL = " select ProcName, PageAction, SA.ProcId, count(CQ.StepId) as NumCount, CQ.StepId from procdef, stepaddresses SA left join casequeue CQ on SA.ProcId = CQ.ProcId and SA.StepId = CQ.StepId where (SA.GRPFLD = 'G' and GroupId  
in ($userdef->usergroups) or GrpFld = 'U' and GroupId = $userdef->UserId_Process ) and SA.ViewQueue = 1 and SA.ProcId = procdef.ProcId
group by ProcName, SA.ProcId, CQ.StepId, PageAction order by NumCount desc ";
        $Query = mysqli_query($connect, $SQL);
        $Processos = array();
        while ($Result = mysqli_fetch_array($Query)) {
            array_push($Processos, $Result);
        }
        return $Processos;
    }

    function TotalCasosPorCampo($ProcId, $StepId = '', $FieldId = "")
    {
        global $connect;
        require_once('sqlqueue.php');
        $SQLUnion = ")";
        $SQLUnion .= " union ";
        $SQLUnion .= "( \n";
        $SQL = '(';
        $Campos[0] = "campo$FieldId as status, exportkeys.casenum";
        $SQL .= SQLCasosGrupos($ProcId, $Campos, $StepId);
        $SQL .= $SQLUnion;
        $SQL .= SQLCasosUsuario($ProcId, $Campos, $StepId);
        $SQL .= $SQLUnion;
        $SQL .= SQLCasosCampoUsuario($ProcId, $Campos, $StepId);
        $SQL .= $SQLUnion;
        $SQL .= SQLCasosCampoGrupo($ProcId, $Campos, $StepId);
//    $SQL .= $SQLUnion;
//    $Campos[0] = "CaseId";
//    $SQL .= SQLCasosAdHoc($ProcId, $Campos, $StepId);
        $SQL .= ")";
        $SQL_AGRUPAMENTO = " select status , count(*) as total from ($SQL) as totais group by status";
        $QUERY = mysqli_query($connect, $SQL_AGRUPAMENTO);
        error_log("TotalCasosPorCampo $SQL_AGRUPAMENTO");
        return mysqli_fetch_all($QUERY, MYSQLI_ASSOC);
    }

    function TotalCasosNaFila($ProcId, $StepId = '')
    {
        global $connect;
        require_once('sqlqueue.php');
        $SQLUnion = ")";
        $SQLUnion .= " union all";
        $SQLUnion .= "( \n";
        $SQL = '(';
        $Campos[0] = "CaseId";
        $SQL .= SQLCasosGrupos($ProcId, $Campos, $StepId);
        $SQL .= $SQLUnion;
        $Campos[0] = "CaseId";
        $SQL .= SQLCasosUsuario($ProcId, $Campos, $StepId);
        $SQL .= $SQLUnion;
        $Campos[0] = "CaseId";
        $SQL .= SQLCasosCampoUsuario($ProcId, $Campos, $StepId);
        $SQL .= $SQLUnion;
        $SQL .= SQLCasosCampoGrupo($ProcId, $Campos, $StepId);
//    $SQL .= $SQLUnion;
        $Campos[0] = "CaseId";
//    $SQL .= SQLCasosAdHoc($ProcId, $Campos, $StepId);
        $SQL .= ")";
        $QUERY = mysqli_query($connect, $SQL);
        return mysqli_num_rows($QUERY);
    }

    function PegaProximoCasoFila($ProcId, $StepId = "")
    {
        global $connect;

        $procDef = $_SESSION["procdef"];

        $filtro = $procDef->filterQueue;

        require_once('sqlqueue.php');
        $SQLUnion = ")";
        $SQLUnion .= " union ";
        $SQLUnion .= "( \n";

        $Campos[0] = "CaseId";
        $Campos[1] = "casequeue.StepId";

        $FiltrosTratados = array();
        if (is_array($filtro)) {
            $FiltrosTratados = TrataFiltros($filtro, $ProcId);
        }

        $FiltrosTratados[] = "lockedbyid = 0";

        $SQL = '(';
        $SQL .= SQLCasosGrupos($ProcId, $Campos, $StepId, '', '', "", $FiltrosTratados);
        $SQL .= $SQLUnion;
        $SQL .= SQLCasosUsuario($ProcId, $Campos, $StepId, "", "", "", $FiltrosTratados);
        $SQL .= $SQLUnion;
        $SQL .= SQLCasosCampoUsuario($ProcId, $Campos, $StepId, "", "", "", $FiltrosTratados);
        $SQL .= $SQLUnion;
        $SQL .= SQLCasosCampoGrupo($ProcId, $Campos, $StepId, "", "", "", $FiltrosTratados);
//    $SQL .= $SQLUnion;
//    $SQL .= SQLCasosAdHoc($ProcId, $Campos, $StepId, "", "", "", $filtro);
        $SQL .= ")";

        $SqlFinal = "select CaseId, StepId from ( $SQL ) as dados limit 1";
        $QUERY = mysqli_query($connect, $SqlFinal);
        $retorno = mysqli_fetch_array($QUERY, MYSQLI_ASSOC);
        return $retorno;
    }

    function ZerosaEsquera($Numero, $NrZeros)
    {
        $Size = strlen($Numero);
        $Zeros = "";
        for ($contador = 1; $contador <= ($NrZeros - $Size); $contador++) {
            $Zeros = "0$Zeros";
        }
        return $Zeros . $Numero;
    }

    function formataCaseNum($Numero, $NrZeros)
    {
        return ZerosaEsquera($Numero, $NrZeros);
    }

    function StepFim($ProcId, $StepId)
    {
        global $connect;
        $SQL = " select ";
        $SQL .= "  EndStep ";
        $SQL .= "  from ";
        $SQL .= "  stepdef ";
        $SQL .= "  where ";
        $SQL .= "  ProcId = $ProcId ";
        $SQL .= "  and ";
        $SQL .= "  StepId = $StepId ";
        $QUERY = mysqli_query($connect, $SQL);
        $result = mysqli_fetch_array($QUERY);
        mysqli_free_result($QUERY);
        return $result["EndStep"];
    }

    /**
     * 
     * @global type $connect
     * @global type $procdef
     * @param type $ProcId
     * @param type $StepId
     * @param type $IncluirDescricao
     * @return string
     */
    function PegaNomeStep($ProcId, $StepId, $IncluirDescricao = 0)
    {
        global $connect, $procdef;
        if ($StepId == 0) {
            return "Administradores";
        }
        if (is_object($procdef)) {
            if ($procdef->ProcId == $ProcId & is_array($procdef->Steps)) {
                foreach ($procdef->Steps as $result) {
                    if ($result["StepId"] == $StepId) {
                        break;
                    }
                }
            }
        } else {
            $SQL = " select ";
            $SQL .= "  StepName, ";
            $SQL .= "  StepDesc ";
            $SQL .= "  from ";
            $SQL .= "  stepdef ";
            $SQL .= "  where ";
            $SQL .= "  ProcId = $ProcId ";
            $SQL .= "  and ";
            $SQL .= "  StepId = $StepId ";
            $QUERY = mysqli_query($connect, $SQL);
            $result = mysqli_fetch_array($QUERY);
            mysqli_free_result($QUERY);
        }
        if ($IncluirDescricao == 1) {
            $StepName = $result["StepName"] . " - " . $result["StepDesc"];
        } else {
            $StepName = $result["StepName"];
        }
        return $StepName;
    }

    function PegaNomeCondition($ProcId, $StepId, $ConditionId)
    {
        global $connect;
        $SQL = " select ";
        $SQL .= "  ConditionName ";
        $SQL .= "  from ";
        $SQL .= "  stepconditiondef ";
        $SQL .= "  where ";
        $SQL .= "  ProcId = $ProcId ";
        $SQL .= "  and ";
        $SQL .= "  StepId = $StepId ";
        $SQL .= "  and ";
        $SQL .= "  ConditionId = $ConditionId ";
        $QUERY = mysqli_query($connect, $SQL);
        if (!$QUERY) {
            error_log("Erro " . $SQL);
        }
        $result = mysqli_fetch_array($QUERY);
        mysqli_free_result($QUERY);
        $Condition = $result["ConditionName"];
        return $Condition;
    }

    function PegaNomeProc($ProcId)
    {
        global $connect;
        $SQL = " select ";
        $SQL .= "  ProcName ";
        $SQL .= "  from ";
        $SQL .= "  procdef ";
        $SQL .= "  where ";
        $SQL .= "  ProcId = $ProcId ";
        $QUERY = mysqli_query($connect, $SQL);

        $result = mysqli_fetch_array($QUERY);
        mysqli_free_result($QUERY);
        return $result["ProcName"];
    }

    function CampoReadOnly($Valor, $StiloObjeto, $id = "", $MaiorComprimento = 0, $Class = "form-control input-sm", $icon = "")
    {
        if ($MaiorComprimento > 0) {
            $Tamanho = $MaiorComprimento + 10;
        } else {
            $Tamanho = strlen($Valor) + 20;
        }
        if ($Valor == "Não") {
            $StiloObjeto1 = "style='color:red'";
        }
        $Campo = "\t\t\t\t<div class=\"input-group\"><input type=\"text\" class=\"$Class\" value=\"$Valor\" readonly=\"True\"  id=\"r$id\" name=\"r$id\">\n";

// Adiciona o Icone se existir
        if (!empty($icon)) {
            $Campo .= "<span class=\"input-group-addon\"><i class=\"fa $icon\"></i></span>";
        }
        $Campo .= "</div>";
        return $Campo;
    }

    /*
      function MontaBoleano($Valor, $ReadOnly, $StiloObjeto)
      {
      Global $campo;
      if ($ReadOnly == 1)
      {
      if  ($Valor==0)
      {
      $Valor="N�o";
      }
      else
      {
      $Valor="Sim";
      }
      $Campo = CampoReadOnly($Valor, $StiloObjeto);
      $pagina=$pagina."$Campo";
      }
      else
      {
      $pagina=$pagina."\t\t\t\t<input type=\"radio\" name=\"t$campo\" ";
      if ($Valor==1)
      {
      $pagina=$pagina." checked ";
      }
      $pagina=$pagina." value = \"1\"><span class=\"Bolean\">Sim</span>\n";
      $pagina=$pagina."\t\t\t\t<input type=\"radio\" name=\"t$campo\" ";
      if ($Valor==0)
      {
      $pagina=$pagina." checked ";
      }
      $pagina=$pagina." value = \"0\"><span class=\"Bolean\">N�o</span>\n";
      }
      return $pagina;
      }
     */

    function PegaValorCampoTabela($Valor, $TableSource, $SourceField, $DisplayField, $UserField = "")
    {
        global $connectDBExterno, $UserId;
        AtivaDBExterno();
        $UserField = trim($UserField);
        $Valor = trim($Valor);
        if (empty($DisplayField) | empty($Valor)) {
            return "";
        }
        if (!strpos($TableSource, 'select')) {
            $SQL = "select * from ";
        }
        $SQL .= $TableSource;
        $SQL .= "  where ";
        $SQL .= "  $SourceField = $Valor ";
        if (!empty($UserField)) {
            $SQL .= " and ";
            $SQL .= " $UserField = $UserId ";
        }
        $QUERY = mysqli_query($connectDBExterno, $SQL);
        $linha = mysqli_fetch_array($QUERY);
        mysqli_free_result($QUERY);
        return $linha["$DisplayField"];
    }

    function ReplaceFieldsValues($String)
    {
        global $userdef, $ProcId, $CaseNum;

// Substitui Origem do Usu�rio
        $String = str_replace('!%OrigemUser!%', $userdef->Origem, $String);

// Substitui Nome do Usuario
        $String = str_replace('!%UserName!%', $userdef->UserName, $String);

// Substitui Id do Usuario
        $String = str_replace('!%UserId!%', $userdef->UserId, $String);

// Substitui ProcId
        $String = str_replace('!%ProcId!%', $ProcId, $String);

// Substitui CaseId
        $String = str_replace('!%CaseId!%', $CaseNum, $String);

// Substitui ProcId
        $String = str_replace('!%CaseNum!%', $CaseNum, $String);

        $String = ParsevaloresCampos($ProcId, $CaseNum, $String);


        return $String;
    }

    function PegaValorCampos($CaseNum)
    {
        global $connect;
        $SQL = "select "
                . "FieldValue, "
                . "casedata.FieldId, "
                . "FieldType, "
                . "ExtendData, "
                . "ValueId "
                . "from "
                . "casedata, "
                . "procfieldsdef "
                . "where "
                . "CaseNum = $CaseNum "
                . "and "
                . "procfieldsdef.ProcId = casedata.ProcId "
                . "and "
                . "procfieldsdef.FieldId = casedata.FieldId "
                . "and "
                . "not fieldtype in ('AR', 'TM', 'TK') "
                . "order by ValueId";
        $Query = mysqli_query($connect, $SQL);
        while ($Linha = mysqli_fetch_array($Query)) {
            if ($Linha["FieldType"] != "AR") {
                $valores[$Linha["FieldId"]] = $Linha["FieldValue"];
            } else {
                $valores[$Linha["FieldId"]] = $Linha["ExtendData"];
            }
        }
        return $valores;
    }

    function PegaValorCampolista($ProcId, $Campo, $ValorCampo)
    {
        global $connect, $_ExtendProps;
        if (!isset($_ExtendProps)) {
            $SQL = "select ExtendProp from procfieldsdef where procid = $ProcId and FieldId = $Campo ";
            $Query = mysqli_query($connect, $SQL);
            $lista = mysqli_fetch_all($Query, MYSQLI_ASSOC);
            $_ExtendProps = $lista[0]["ExtendProp"];
        }

        if (!is_array($_ExtendProps)) {
            $_ExtendProps = array();
            session_register("_ExtendProps");
        }
        if ($ValorCampo == -1 || $ValorCampo == "") {
            return '';
        } else {
            if (!is_array($_ExtendProps[$Campo])) {
                $valores = explode(".", PegavaloresLista($ProcId, $Campo));
                $_ExtendProps[$Campo] = $valores;
            }
            reset($_ExtendProps[$Campo]);
            foreach ($_ExtendProps[$Campo] as $Valor) {
                $AValor = explode("|", $Valor);
                $ValorSel = $AValor[0];
                if (count($AValor) > 1) {
                    $ValorSel = $AValor[1];
                }
                if ($ValorSel == $ValorCampo) {
                    $retorno = $AValor[0];
                    break;
                }
            }
            return $retorno;
        }
    }

    function PegaValoresCampoLista_array($ProcId, $Campo)
    {
        $valores = explode(".", PegavaloresLista($ProcId, $Campo));
        $retorno = array();
        foreach ($valores as $Valor) {
            $AValor = explode("|", $Valor);
            $itemCampo["display"] = $AValor[0];
            $itemCampo["valor"] = $AValor[1];
            $retorno[] = $itemCampo;
        }
        return $retorno;
    }

    function PegavaloresLista($ProcId, $campo)
    {
        global $connect;
        $SQL = " select ";
        $SQL .= "  ExtendProp as OptValues ";
        $SQL .= "  from ";
        $SQL .= "  procfieldsdef ";
        $SQL .= "  where ";
        $SQL .= "  ProcId = $ProcId";
        $SQL .= "  and ";
        $SQL .= "  FieldId = $campo ";
        $Query = mysqli_query($connect, $SQL);
        $result = mysqli_fetch_array($Query);
        mysqli_free_result($Query);
        return $result["OptValues"];
    }

    function Totalvalores($ProcId, $FieldId, $CaseNum)
    {
        global $connect;
        $SQL = "select count(*) as Total from casedata where ProcID = $ProcId and CaseNum = $CaseNum and FieldId = $FieldId";
        $Query = mysqli_query($connect, $SQL);
        $Result = mysqli_fetch_array($Query);
        return $Result["Total"];
    }

    function campoMT_Totalvalores($ProcId, $CaseNum, $FieldId)
    {
        global $connect;
        $SQL = "select count(*) as Total from casedata where ProcID = $ProcId and CaseNum = $CaseNum and FieldId = $FieldId";
        $Query = mysqli_query($connect, $SQL);
        $Result = mysqli_fetch_array($Query);
        return $Result["Total"];
    }

    function PegavaloresListaUsuario($UserId, $Tabela, $CampoKey, $CampoDisp)
    {
        global $connect;
        $SQL = "select $CampoDisp from $Tabela where $CampoKey = $UserId";
        $Query = mysqli_query($connect, $SQL);
        $result = mysqli_fetch_array($Query);
        mysqli_free_result($Query);
        $valores = $result[$CampoDisp];
        return explode(";", $valores);
    }

    function CamposFormulario($ProcId, $StepId, $FieldId = 0)
    {
        global $connect;
        $SQL = "SELECT ";
        $SQL .= " procfieldsdef.fieldid, ";
        $SQL .= " fieldname, ";
        $SQL .= " fielddesc, ";
        $SQL .= " fieldtype, ";
        $SQL .= " fieldlength, ";
        $SQL .= " readonly, ";
        $SQL .= " optional, ";
        $SQL .= " fieldspecial, ";
        $SQL .= " OrderId, ";
        $SQL .= " NewLine, ";
        $SQL .= " FieldSourceTable, ";
        $SQL .= " FieldSourceField, ";
        $SQL .= " FieldDisplayField, ";
        $SQL .= " DefaultValue, ";
        $SQL .= " GroupSource, ";
        $SQL .= " ScriptValida, ";
        $SQL .= " ExtendProp, ";
        $SQL .= " FieldCod, ";
        $SQL .= " CSS, ";
        $SQL .= " FieldHelp, ";
        $SQL .= " FieldChange,";
        $SQL .= " FieldKeyMaster, ";
        $SQL .= " FieldMaster, ";
        $SQL .= " FieldMask, ";
        $SQL .= " footertext, ";
        $SQL .= " FixarValor ";
        $SQL .= " from ";
        $SQL .= " procfieldsdef, ";
        $SQL .= " stepfieldsdef ";
        $SQL .= " where ";
        $SQL .= " procfieldsdef.procid = $ProcId ";
        $SQL .= " and ";
        $SQL .= " stepfieldsdef.procid = procfieldsdef.procid ";
        $SQL .= " and ";
        $SQL .= " stepfieldsdef.stepid = $StepId ";
        $SQL .= " and ";
        $SQL .= " stepfieldsdef.fieldid = procfieldsdef.fieldid ";
        $SQL .= " and ";
        $SQL .= " procfieldsdef.Active = 1 ";
        if ($FieldId != 0) {
            $SQL .= " and ";
            $SQL .= " stepfieldsdef.fieldid = $FieldId ";
        }
        $SQL .= " order by ";
        $SQL .= " OrderId ";
        $Query = mysqli_query($connect, $SQL);
        $retorno = mysqli_fetch_all($Query, MYSQLI_ASSOC);
        return $retorno;
    }

    function PegaDadosCampoLista($ProcId, $FieldId, $Valor, $ExtendProps = '')
    {
        $Valor = html_entity_decode($Valor);
        if (empty($ExtendProps)) {
            $ExtendProps = PegavaloresLista($ProcId, $FieldId);
        }
        $ExtendProps = str_replace("\"", "", $ExtendProps);
        $ExtendProps = explode(";", $ExtendProps);
        if (is_array($ExtendProps)) {
            $ExtendProps = $ExtendProps[0];
        }
        $Listavalores = explode(".", $ExtendProps);
        $NovaLista = array();
        foreach ($Listavalores as $ItemValor) {
            $Item["Display"] = $ItemValor;
            $Item["Valor"] = $ItemValor;
            $A_Item = explode("|", $ItemValor);
            if (count($A_Item) > 1) {
                $Item["Display"] = $A_Item[0];
                $Item["Valor"] = $A_Item[1];
            }
            $Item["selected"] = false;
            if ($Item["Valor"] == $Valor) {
                $Item["selected"] = true;
            }
            array_push($NovaLista, $Item);
        }
        return $NovaLista;
    }

    function PegaParametroCSS($ParametrosCSS, $Parametro, $Default = "")
    {
        $CSS = explode(";", $ParametrosCSS);
        $Contador = 0;
        while ($Contador < count($CSS)) {
            $CSSParse = explode(":", $CSS[$Contador]);
            if ($CSSParse[0] == $Parametro) {
                return $CSSParse[1];
            }
            $Contador = $Contador + 1;
        }
        return $Default;
    }

    function PegaDescUsuario($UserId)
    {
        global $connect;
        $OrigemLogon = "";
        $EXTERNAL_USERFULLNAME = "";
        $EXTERNAL_userdef = "";
        $EXTERNAL_USERID = "";
        include("../include/config.config.php");
        require_once("connection.php");
        if (empty($UserId) | $UserId == 0) {
            return "";
        }
        if ($OrigemLogon == "External") {
            $connectDBExterno = AtivaDbExterno();
            $SQL = " Select $EXTERNAL_USERFULLNAME as UserFullName from $EXTERNAL_userdef where $EXTERNAL_USERID = $UserId ";
            $QUERY1 = mysqli_query($connectDBExterno, $SQL);
            $linha = mysqli_fetch_array($QUERY1);
            if (mysqli_num_rows($QUERY1) > 0) {
                $retorno = $linha["UserFullName"];
            } else {
                $retorno = "Usu�rio Inativo";
            }
            mysqli_free_result($QUERY1);
            AtivaDBProcess();
            return $retorno;
        } else {
            $SQL = " Select UserDesc from userdef where UserId = $UserId ";
            $QUERY1 = mysqli_query($connect, $SQL);
            $linha = mysqli_fetch_array($QUERY1);
            if (mysqli_num_rows($QUERY1) > 0) {
                $retorno = $linha["UserDesc"];
            } else {
                $retorno = "Usu�rio Inativo";
            }
            mysqli_free_result($QUERY1);
            return $retorno;
        }
    }

    function PegaNomeUsuario($UserId)
    {
        global $connect, $connectDBExterno, $OrigemLogon;
        $EXTERNAL_USERNAME = "";
        $EXTERNAL_userdef = "";
        $EXTERNAL_USERID = "";
        include(FILES_ROOT . "/include/config.config.php");
        require_once("connection.php");
        if (empty($UserId) | $UserId == 0) {
            return "";
        }
        if ($OrigemLogon == "External") {
            $connectDBExterno = AtivaDbExterno();
            $SQL = " Select $EXTERNAL_USERNAME as UserName from $EXTERNAL_userdef where $EXTERNAL_USERID = $UserId ";
            $QUERY1 = mysqli_query($connectDBExterno, $SQL);
            $linha = mysqli_fetch_array($QUERY1);
            mysqli_free_result($QUERY1);
            AtivaDBProcess();
            return $linha["UserName"];
        } else {
            $SQL = " Select UserDesc as UserName from userdef where UserId = $UserId ";
            $QUERY1 = mysqli_query($connect, $SQL);
            $linha = mysqli_fetch_array($QUERY1);
            mysqli_free_result($QUERY1);
            return $linha["UserName"];
        }
    }

    function PegaIdPeloNome($UserName)
    {
        global $connect, $connectDBExterno, $OrigemLogon;
        $EXTERNAL_USERID = "";
        $EXTERNAL_userdef = "";
        $EXTERNAL_USERNAME = "";
        include("../include/config.config.php");
        require_once("connection.php");
        if (empty($UserName)) {
            return "0";
        }
        if ($OrigemLogon == "External") {
            $connectDBExterno = AtivaDbExterno();
            $SQL = " Select $EXTERNAL_USERID as UserId from $EXTERNAL_userdef where $EXTERNAL_USERNAME = '$UserName' ";
            $QUERY1 = mysqli_query($connectDBExterno, $SQL);
            if (mysqli_num_rows($QUERY1) == 0) {
                return 0;
            }
            $linha = mysqli_fetch_array($QUERY1);
            mysqli_free_result($QUERY1);
            AtivaDBProcess();
            return $linha["UserId"];
        } else {
            $SQL = " Select UserId from userdef where UserName = $UserName ";
            $QUERY1 = mysqli_query($connect, $SQL);
            if (mysqli_num_rows($QUERY1) == 0) {
                return 0;
            }
            $linha = mysqli_fetch_array($QUERY1);
            mysqli_free_result($QUERY1);
            return $linha["UserId"];
        }
    }

    function MaiorComprimento($valores)
    {
        $tamanho = 0;
        for ($contador = 0; $contador < count($valores); $contador++) {
            if ($tamanho < strlen($valores[$contador])) {
                $tamanho = strlen($valores[$contador]);
            }
        }
        return $tamanho;
    }

    function PegaDescricaoUsuario($UserId)
    {
        global $connect, $OrigemLogon;
        $EXTERNAL_USERFULLNAME = "";
        $EXTERNAL_userdef = "";
        $EXTERNAL_USERID = "";
        include("../include/config.config.php");
        if (empty($UserId) || $UserId == 0) {
            return "Autom�tico";
        }
        if ($OrigemLogon == "External") {
            $connectDBExterno = AtivaDbExterno();
            $SQL = " Select $EXTERNAL_USERFULLNAME as UserFullName from $EXTERNAL_userdef where $EXTERNAL_USERID = $UserId ";
            $QUERY1 = mysqli_query($connectDBExterno, $SQL);
            $linha = mysqli_fetch_array($QUERY1);
            mysqli_free_result($QUERY1);
            AtivaDBProcess();
            return $linha[$EXTERNAL_USERFULLNAME];
        } else {
            $SQL = " Select UserDesc from userdef where UserId = $UserId ";
            $QUERY1 = mysqli_query($connect, $SQL);
            $linha = mysqli_fetch_array($QUERY1);
            mysqli_free_result($QUERY1);
            return $linha["UserDesc"];
        }
    }

    function MontaEstilo($ParametrosCSS, $campo, $Tipo, $ReadOnly, $optional)
    {
        $CSS = " $ParametrosCSS ";
        $CSSColor = PegaParametroCSS($ParametrosCSS, "Color");
        if (empty($CSSColor)) {
            if ($ReadOnly == 1) {
                $CSSColor = "Color:White";
            } else {
                if ($optional == 1) {
                    $CSSColor = "Color:blue";
                } else {
                    $CSSColor = "Color:red";
                }
            }
        } else {
            $CSSColor = ";Color:$CSSColor;";
        }
        $BackgroudDiv = "";
        if ($Tipo <> "Normal") {
            $BackgroudDiv = "Background:silver;";
        }
        $CSSDiv = " div.campo$campo { $CSS;$CSSColor;Cursor:hand;$BackgroudDiv } ";
        $CSSTD = " td.campo$campo { $CSSColor } ";
        return "\t$CSSDiv\n\t$CSSTD";
    }

    function Filhos($comentarios_ord, $comentarios, $Pai)
    {
        for ($contador = 1; $contador < count($comentarios) + 1; $contador++) {
            if (empty($comentarios[$contador][3])) {
                continue;
            }
            if ($comentarios[$contador][3] == $Pai) {
                $comentarios_ord[count($comentarios_ord) + 1] = $comentarios[$contador];
                $comentarios_ord = Filhos($comentarios_ord, $comentarios, $comentarios[$contador][4]);
            }
        }
        return $comentarios_ord;
    }

    function OrdenaComentarios($comentarios)
    {
        if (count($comentarios) <= 1) {
            return $comentarios;
        }
        $comentarios_ord[1] = $comentarios[1];
        $comentarios_ord = Filhos($comentarios_ord, $comentarios, $comentarios[0][4]);
        for ($contador = 2; $contador < count($comentarios) + 1; $contador++) {
            if (empty($comentarios[$contador][3])) {
                $comentarios_ord[count($comentarios_ord) + 1] = $comentarios[$contador];
                $comentarios_ord = Filhos($comentarios_ord, $comentarios, $comentarios[$contador][4]);
            } else {
                $comentarios_ord[count($comentarios_ord) + 1] = $comentarios[$contador];
                $comentarios_ord = Filhos($comentarios_ord, $comentarios, $comentarios[$contador][4]);
            }
        }
        return $comentarios_ord;
    }

    function ExplodeValueTickler($ValorCampo, $ValueId)
    {
        $ValorCampo = str_replace("/*%/*", "'", str_replace("/*;/*", "/*#/*", $ValorCampo));
        $ValorCampo = explode(";", $ValorCampo);

        $Valor = $ValorCampo[0];
        $Valor = substr($Valor, 8, 2) . "/" . substr($Valor, 5, 2) . "/" . substr($Valor, 0, 4) . " " . substr($Valor, 11, 5);
        $ValorCampo[0] = $Valor;

        $ValorCampo[2] = str_replace("/*#/*", ";", $ValorCampo[2]);
        $ValorCampo[5] = $ValorCampo[5];
        /*
          if (!empty($comentario[$contador][4]) & $AdHoc)
          {
          //			$comentario[$contador][1] = "<a href=\"BPMAdHoc.php?ProcId=$ProcId&StepId=$StepId&FieldId=$FieldTickler&CaseNum=$CaseNum&Tickler=1&NovaLinha=$TotalComentarios&Edit=0&Tickler=1&Valor=" .  $comentario[$contador][4] . "\" onmouseMOVE=\"window.status = ===='Enviar ADHOC para " . $comentario[$contador][1] . "'\" onmouseOUT=\"window.status = ====''\">"  . $comentario[$contador][1] . "</a>";
          }
         */
        $ValorCampo[4] = $ValueId;
        return $ValorCampo;
    }

    function Tickler($ProcId, $FieldId, $CaseNum)
    {
        global $connect;
        $SQL = "select FieldValue, ValueId from casedata where ProcId = $ProcId and FieldId = $FieldId and CaseNum = $CaseNum order by ValueId desc";
        $Query = mysqli_query($connect, $SQL);
        $contador = 0;
        $comentario = array();
        while ($result = mysqli_fetch_array($Query)) {
            $contador++;
            $comentario[$contador] = ExplodeValueTickler($result["FieldValue"], $result["ValueId"]);
        }
        return OrdenaComentarios($comentario);
    }

    function TipoProc($ProcId)
    {
        global $connect;
        $SQL = "select TipoProc from procdef where ProcId = $ProcId";
        $Query = mysqli_query($connect, $SQL);
        $Linha = mysqli_fetch_array($Query);
        return $Linha["TipoProc"];
    }

    function CasosNosProcessos()
    {
        global $connect, $S_procdef, $userdef, $Filtros;
        require_once('sqlqueue.php');
        $Campos[0] = 'exportkeys.CaseNum ';
        $SQLUnion = ")";
        $SQLUnion .= " union ";
        $SQLUnion .= "( \n";
        if (is_array($Filtros)) {
            $Filtros = TrataFiltros($Filtros);
        }
        $Campo = TrataCampoOrdem($Campo);
        $SQL = "select ProcId, Count(*) as totais from casequeue where CaseId in (";
        $SQL .= '(';
        $SQL .= SQLCasosGrupos('', $Campos, -1, 1, '', $userdef->Origem);
        $SQL .= $SQLUnion;
        $SQL .= SQLCasosUsuario('', $Campos, -1, 1, '', $userdef->Origem);
//	$SQL .= $SQLUnion;
//	$SQL .= SQLCasosCampo('', $Campos, -1, 1, '', $userdef->Origem);
//    $SQL .= $SQLUnion;
//    $SQL .= SQLCasosAdHoc('', $Campos, -1, 1, '', $userdef->Origem);
        $SQL .= ")";
        $SQL .= ")";
        $SQL .= " group by ProcId ";
        $QUERY = mysqli_query($connect, $SQL);
        return $QUERY;
    }

    function TotaisProcessosUsuario()
    {
        global $connect, $S_procdef, $S_Processos;
        require_once('sqlqueue.php');
        $Campos = CamposSQLProcessos();
        $SQLUnion = ")";
        $SQLUnion .= " union ";
        $SQLUnion .= "( \n";
        $SQL = '(';
        $SQL .= SQLCasosGrupos($ProcId, $Campos, $StepId, $HideQueue, $Campo, $Origem, $Filtros, 1, " group by casequeue.ProcId");
        $SQL .= $SQLUnion;
        $SQL .= SQLCasosUsuario($ProcId, $Campos, $StepId, $HideQueue, $Campo, $Origem, $Filtros, 1, " group by casequeue.ProcId");
//	$SQL .= $SQLUnion;
//	$SQL .= SQLCasosCampoUsuario($ProcId, $Campos, $StepId, $HideQueue, $Campo, $Origem, $Filtros, 1, " group by casequeue.ProcId");
//	$SQL .= $SQLUnion;
//	$SQL .= SQLCasosCampoGrupo($ProcId, $Campos, $StepId, $HideQueue, $Campo, $Origem, $Filtros, 1, " group by casequeue.ProcId");
//	$SQL .= $SQLUnion;
//	$SQL .= SQLCasosAdHoc($ProcId, $Campos, $StepId, $HideQueue, $Campo, $Origem, $Filtros, 1, " group by casequeue.ProcId");
        $SQL .= ")";
        $SQL .= " order by casequeue.ProcId ";
        $Query = @mysqli_query($connect, $SQL);
        while ($Result = mysqli_fetch_array($Query)) {
            $ProcId = $Result["ProcId"];
            if ($ProcId <> $ProcIdOld) {
                $ProcName = PegaNomeProc($ProcId);
                $ProcIdOld = $ProcId;
            }
            $Total = $Result["TotalCasos"];
            $Processos[$ProcName]["ProcId"] = $ProcId;
            $Processos[$ProcName]["TotalCasos"] += $Total;
            $Processos[$ProcName]["ProcName"] = $ProcName;
        }

        $Total = 0;
        ksort($Processos);
        $ProcessosZero = array();
        foreach ($S_Processos as $Processo) {
            $ProcName = $Processo["ProcName"];
            $ProcessosZero[$ProcName]["ProcName"] = $ProcName;
            $ProcessosZero[$ProcName]["ProcId"] = $Processo["ProcId"];
            $ProcessosZero[$ProcName]["TotalCasos"] += $Total;
        }
        ksort($ProcessosZero);
        $Processos = $Processos + $ProcessosZero;
        return $Processos;
    }

    function generateCallTrace()
    {
        $e = new Exception();
        $trace = explode("\n", $e->getTraceAsString());
// reverse array to make steps line up chronologically
        $trace = array_reverse($trace);
        array_shift($trace); // remove {main}
        array_pop($trace); // remove call to this method
        $length = count($trace);
        $result = array();

        for ($i = 0; $i < $length; $i++) {
            $result[] = ($i + 1) . ')' . substr($trace[$i], strpos($trace[$i], ' ')); // replace '#someNum' with '$i)', set the right ordering
        }
        error_log("\t" . implode("\n\t", $result));
    }
    ?>