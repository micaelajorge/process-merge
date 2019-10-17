<?php

function EncerraErros()
{
    global $Err;
    //Cria Variavel de Erro
    if ($Err[0]["ErrId"] == 0) {
        $Error['Count'] = 0;
    } else {
        $Error['Count'] = count($Err);
    }
    $Error['Itens'] = $Err;
    return $Error;
}

function GeraErro($CodigoErro, $ErroDetail = "")
{
    global $Err, $URL, $SERVER_NAME;
    $ErrorDef[0] = "sem erro";
    $ErrorDef[1] = "Caso não consta na $ErroDetail Fila ";
    $ErrorDef[2] = "Caso Bloqueado por outro Usuário";
    $ErrorDef[3] = "Caso Bloqueado em AdHoc";
    $ErrorDef[4] = "Falha de Logon";
    $ErrorDef[5] = "Falha ao acessar Banco de Dados";
    $ErrorDef[7] = "Step não permitido para NovoDoc - $ErroDetail";
    $ErrorDef[8] = "Erro em Passagem de Campos";
    $ErrorDef[9] = "Usuário não tem permisss�es suficientes para a operação";
    $ErrorDef[10] = "Caso não localizado";
    $ErrorDef[11] = "Caso não é do Processo";
    $ErrorDef[12] = "Campo não existe";
    $ErrorDef[13] = "Processo não encontrado ou Id Inválido- $ErroDetail";
    $ErrorDef[14] = "Passo não encontrado - $ErroDetail";
    $Err[0]["ErrId"] = 0;
    $Err[0]["ErrDesc"] = $ErrorDef[0];
    $i = count($Err);
    if (($i == 1 && $Err[0]["ErrId"] == 0)) {
        $i = 0;
    }
    $dadosChamada = debug_backtrace();        
    
    $funcaoChamada = $dadosChamada[1]["function"];
    $dadosErro = "Erro no WebService: $CodigoErro - " . $ErrorDef[$CodigoErro] . "Arquivo: " . $dadosChamada["file"] . " Função: " . $funcaoChamada . " End. Remoto: " . $_SERVER["REMOTE_ADDR"];
    //$dadosErro = "Erro no WebService: $CodigoErro - " . $ErrorDef[$CodigoErro] . " Função: " . $funcaoChamada . " End. Remoto: ";
    error_log("Dados Erro: $dadosErro");
    $ErrorDef[6] = "Campo $ErroDetail Inválido ";
    $Err[$i]["ErrId"] = $CodigoErro;
    $Err[$i]["ErrDesc"] = $ErrorDef[$CodigoErro] . " - $SERVER_NAME$URL";
    if (!empty($ErroDetail)) {
        $Err[$i]["ErrDesc"] .= $ErroDetail;
    }
}
