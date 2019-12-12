<?php

//require_once("include/common.php");
require_once("include/resource01.php");

//include("include/validacaoapi.php");

define("LIMITE_FORCA_SENHA", 40);

/**
 * ROTA: api/manager/salvasenharecuparacao
 * 
 * @global type $USUARIO_SENHA
 */
function salva_senha_recuperacao()
{
    global $connect;

    $dadosEntrada = apiTrataDadosEntrada();

    $forcaSenha = testaForcaSenha($dadosEntrada['USUARIO_SENHA']);
    if ($forcaSenha < LIMITE_FORCA_SENHA) {
        header('HTTP/1.0 403 ERRO');
        die;
    }

    $chaveBusca = filter_var($dadosEntrada['chaveRecuperacao'], FILTER_SANITIZE_STRING);

    $SQL = "select * from userdef where tokenEmail = '$chaveBusca'";
    $Query = mysqli_query($connect, $SQL);

    $linhas = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    if (count($linhas) == 0) {
        header('HTTP/1.0 403 ERRO');
        die;
    }

    $userId = $linhas[0]["UserId"];
    salvar_senha($userId, $dadosEntrada["USUARIO_SENHA"]);
}



/**
 * ROTA: enviorecuperarsenha
 * @global type $connect
 */
function EnvioEmailRecuperacao()
{
    global $connect;
    $dadosEntrada = apiTrataDadosEntrada();

    $emailBusca = filter_var($dadosEntrada["email"], FILTER_SANITIZE_EMAIL);

    $SQL = "select * from userdef where email = '$emailBusca'";
    $Query = mysqli_query($connect, $SQL);

    $linhas = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    if (count($linhas) == 0) {
        error_log("Usuário não encontrado - " . $emailBusca);
        header('Location: recuperarsenha/enviado');
        return;
    }


    if (!key_exists("tokenEmail", $linhas[0])) {
        $SQL = "alter table userdef add column tokenEmail varchar(255)";
        $Query = mysqli_query($connect, $SQL);
    }

    $tokenRecuperacao = uniqid();
    $SQL = "update userdef set tokenEmail = '$tokenRecuperacao' where userId = {$linhas[0]["UserId"]}";
    $Query = mysqli_query($connect, $SQL);

    require_once FILES_ROOT . "include/envio_email.inc";
    require_once FILES_ROOT . "pages/gerar_arquivo_template.inc";

    $dadosConfig = pegaDadosConfig();

    $emailFrom["address"] = $dadosConfig["emailNotificationUser"];
    $emailFrom["nome"] = $dadosConfig["emailNotificationDesc"];

    $emailAddress["address"] = $linhas[0]["Email"];
    $emailAddress["nome"] = $linhas[0]["UserDesc"];

    $emailSubject = "Troca de Senha";

    $variaveisTemplate["token"] = $tokenRecuperacao;
    $variaveisTemplate["URL_SERVER"] = URL_SERVER;

    $bodyMessage = criaHtmlTemplate('email_recuperacao_senha.html', $variaveisTemplate, 'assets/notificacoes/');

    EnviarEmail($emailFrom, $emailAddress, $emailSubject, $bodyMessage);

    header('Location: recuperarsenha/enviado');
}

/**
 * ROTA: api/v1/rp
 * @global type $connect
 */
function RecuperarSenha()
{
    global $connect, $chaveRecuperacao;

    $chaveBusca = filter_var($chaveRecuperacao, FILTER_SANITIZE_EMAIL);
    $SQL = "select * from userdef where tokenEmail = '$chaveBusca'";
    $Query = mysqli_query($connect, $SQL);

    $linhas = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    if (count($linhas) == 0) {
        $_SESSION = array();
        $erro_logon = true;
        $MsgLogon = 'token de autenticação inválido';
        include(FILES_ROOT . "pages/logon.inc");
        exit;
    }

    include(FILES_ROOT . "pages/user_reset_password.inc");
}

function testaForcaSenha($senhaUsuario)
{
    $forcaSenha = 0;
    if (strlen($senhaUsuario) < 8) {
        return 0;
    }
    $retorno = preg_match('/(\W+)/m', $senhaUsuario);
    if ($retorno !== false) {
        $forcaSenha += 10;
    }
    $retorno = preg_match('/a-z]+/m', $senhaUsuario);
    if ($retorno !== false) {
        $forcaSenha += 10;
    }
    $retorno = preg_match('/A-Z]+/m', $senhaUsuario);
    if ($retorno !== false) {
        $forcaSenha += 10;
    }
    $retorno = preg_match('/0-9]+/m', $senhaUsuario);
    if ($retorno !== false) {
        $forcaSenha += 10;
    }
    return $forcaSenha;
}

// ROTA: api/manager/savepasswordstart
function salva_senha_inicial()
{
    global $USUARIO_SENHA;
    salva_senha($USUARIO_SENHA, true);
}

/**
 * 
 * @global type $USUARIO_ID
 * @param type $USUARIO_SENHA
 */
function salva_senha($USUARIO_SENHA, $senhaStart = false)
{
    global $USUARIO_ID;
    salvar_senha($USUARIO_ID, $USUARIO_SENHA, $senhaStart);
}

function verificaSenhas($senhaUsuario, $historicoSenhas)
{
    foreach ($historicoSenhas as $senha) {
        if (password_verify($senhaUsuario, $senha)) {
            return true;
        }
    }
    return false;
}

function verificaHistoriocSenhas($USUARIO_ID, $SENHA, $senhaCript)
{
    global $connect;
    $SQL = "select lastpasswords from userdef where userId = $USUARIO_ID";
    $query = mysqli_query($connect, $SQL);

    $linhas = mysqli_fetch_all($query, MYSQLI_ASSOC);

    $json_historicoSenhas = $linhas[0]["lastpasswords"];

    if ($json_historicoSenhas == "" | $json_historicoSenhas == 'null' | $json_historicoSenhas == NULL) {
        $json_historicoSenhas = '[]';
    }
    $historicoSenhas = json_decode($json_historicoSenhas, true);

    if (verificaSenhas($SENHA, $historicoSenhas)) {
        return false;
    }

    array_push($historicoSenhas, $senhaCript);

    $json_AtualizarHistorico = json_encode($historicoSenhas);
    $SQL = "update userdef set lastpasswords = '$json_AtualizarHistorico' where userId = $USUARIO_ID";
    $query = mysqli_query($connect, $SQL);
    return true;
}

/**
 * 
 * @global type $connect
 * @param type $USUARIO_ID
 * @param type $USUARIO_SENHA
 */
function salvar_senha($USUARIO_ID, $USUARIO_SENHA, $senhaStart = false)
{
    global $connect;
    
    $senhaCript = password_hash($USUARIO_SENHA, PASSWORD_DEFAULT);

    if ($senhaStart) {
        $senhaValida = true;
        $tokenEmail = uniqid();
        $dataUltimaTroca = '1974-01-01';
    } else {
        $senhaValida = verificaHistoriocSenhas($USUARIO_ID, $USUARIO_SENHA, $senhaCript);
        $tokenEmail = "";        
        $dataUltimaTroca = date('Y-m-d');
    }

    // Se a senha já está no histórico retorna outro erro
    if (!$senhaValida) {
        header('HTTP/1.0 422 falha');
        return;
    }

    $SQL = "update userdef set UserPWD = '$senhaCript', tokenEmail='$tokenEmail', lastAlterPassword = '$dataUltimaTroca'  where userId = $USUARIO_ID";
    
    insereEntradaAuditTrail(0, 0, 0, $USUARIO_ID, '', 988, "Aleração de Senha ");
    
    // TODO: Inserir LOGg de troca de senha ou criação de usuário
    $resultado = mysqli_query($connect, $SQL);
    if ($resultado) {
        header('HTTP/1.0 200 ok');
    } else {
        header('HTTP/1.0 403 falha');
    }
}

/**
 * ROTA: api/manager/savepassword
 * 
 * @global type $USUARIO_SENHA
 */
function salva_senha_troca()
{
    global $USUARIO_SENHA, $connect, $USUARIO_SENHA_ATUAL;

    $forcaSenha = testaForcaSenha($USUARIO_SENHA);
    if ($forcaSenha < LIMITE_FORCA_SENHA) {
        header('HTTP/1.0 403 ERRO');
        die;
    }

    $userDef = $_SESSION["userdef"];

    $userTeste = new userdef;
    $resultadoSenha = $userTeste->ProcessLogon_BuscaUsuario($userDef->UserName, $USUARIO_SENHA_ATUAL, $connect);

    if (!$resultadoSenha) {
        header('HTTP/1.0 403 ERRO');
        die;
    }

    salva_senha($USUARIO_SENHA);
}

function removeEntidade()
{
    global $connect, $entidadeId, $grpFld, $usuarioGrupo;
    if ($grpFld == "U") {
        $sql = "delete from usergroup where userid = $entidadeId and groupId = $usuarioGrupo";
    } else {
        $sql = "delete from usergroup where groupid = $entidadeId and userId = $usuarioGrupo";
    }
    $resultado = mysqli_query($connect, $sql);
    return $resultado;
}

function adicionaentidade()
{
    global $connect;
    global $usuarioGrupo, $entidadeId, $grpFld;

    if ($grpFld == "U") {
        $UserId = $entidadeId;
        $GroupId = $usuarioGrupo;
    } else {
        $UserId = $usuarioGrupo;
        $GroupId = $entidadeId;
    }
    $SQL = "insert into usergroup (userid, groupid) values ($UserId, $GroupId)";
    $resultado = mysqli_query($connect, $SQL);
    return $resultado;
}

function PegaUsuariosLogados()
{
    $conect = AtivaDBExterno();
    $SQL = "select USERID from userlogins";
    $Query = mysqli_query($conect, $SQL);
    $Retorno = mysqli_fetch_all($Query, MYSQLI_ASSOC);
    return $Retorno;
}

function DesconectaUsuario($userId)
{
    $conect = AtivaDBExterno();
    $SQL = "delete from userlogins where UserId = $userId";
    $Query = mysqli_query($conect, $SQL);
}

function gravaDadosUsuario()
{
    global $connect;
    global $USUARIO_DESC, $USUARIO_EMAIL, $USUARIO_ATIVO, $USUARIO_ORIGEM, $USUARIO_ID, $USUARIO_NOME, $USUARIO_CUSTONGATE;
    $dadosEntrada = apiTrataDadosEntrada();


    $SQL = "select username from userdef where username = '$USUARIO_NOME' ";
    $Query = mysqli_query($connect, $SQL);

    if (mysqli_num_rows($Query) > 0) {
        header('HTTP/1.0 422 ERRO');
        echo "Usuário já existe";
        return;
    }

    echo "$header.$payload.$signature";
    $token = $dadosEntrada["token"];
    if (trim($token) == "") {
        $token = calculaToken($USUARIO_NOME, $USUARIO_EMAIL);
    }

    if ($dadosEntrada["USUARIO_ID"] > 0) {
        $SQL = "update "
                . "userdef "
                . "set "
                . "UserDesc = '$USUARIO_DESC',"
                . "Email =  '$USUARIO_EMAIL', "
                . "UActive = $USUARIO_ATIVO, "
                . "Origem = '$USUARIO_ORIGEM',"
                . "CustonGate = '$USUARIO_CUSTONGATE',"
                . "token = '$token' "
                . "where "
                . "UserId = $USUARIO_ID";
    } else {
        $SQL = "select max(UserId) as maximo from userdef";
        $query = mysqli_query($connect, $SQL);
        $Linha = mysqli_fetch_array($query);
        $USUARIO_ID = $Linha["maximo"] + 1;
        $SQL = "insert into userdef ("
                . "UserId, "
                . "UserName, "
                . "UserDesc,"
                . "Email,"
                . "UActive, "
                . "Origem,"
                . "token "
                . ") values ("
                . "$USUARIO_ID,"
                . "'$USUARIO_NOME',"
                . "'$USUARIO_DESC',"
                . "'$USUARIO_EMAIL',"
                . "$USUARIO_ATIVO, "
                . "'$USUARIO_ORIGEM',"
                . "'$token'"
                . ") ";
    }
    $resultado = mysqli_query($connect, $SQL);

    if ($resultado) {
        header('HTTP/1.0 200 ok');
    } else {
        error_log("erroNi: " . mysqli_errno($connect));
        error_log("erro: " . mysqli_error($connect));
        header('HTTP/1.0 403 ERRO');
    }
}

function gravaDadosGrupo()
{
    global $connect;
    global $GROUP_DESC, $GROUP_NAME, $GROUP_ID;
    $dadosEntrada = apiTrataDadosEntrada();

    if ($dadosEntrada["GROUP_ID"] > 0) {
        $SQL = "update "
                . "groupdef "
                . "set "
                . "GroupDesc = '$GROUP_DESC',"
                . "GroupName =  '$GROUP_NAME' "
                . "where "
                . "GroupId = $GROUP_ID";
    } else {
        $SQL = "select max(GroupId) as maximo from groupdef";
        $query = mysqli_query($connect, $SQL);
        $Linha = mysqli_fetch_array($query);
        $GROUP_ID = $Linha["maximo"] + 1;
        $SQL = "insert into groupdef ("
                . "groupid, "
                . "groupname, "
                . "groupdesc"
                . ") values ("
                . "$GROUP_ID,"
                . "'$GROUP_NAME',"
                . "'$GROUP_DESC'"
                . ") ";
    }
    $resultado = mysqli_query($connect, $SQL);

    if ($resultado) {
        header('HTTP/1.0 200 ok');
    } else {
        header('HTTP/1.0 403 ERRO');
    }
}

function ProcuraUsuario($UserId, $Lista)
{
    $retorno = "0";
    foreach ($Lista as $Usuario) {
        if ($Usuario["USERID"] == $UserId) {
            $retorno = "1";
            break;
        }
    }
    return $retorno;
}

function api_pegarUsuarios()
{
    global $connect;
    $dadosEntrada = apiTrataDadosEntrada();
    $filtros = apiTrataCampos($dadosEntrada);

    if (isset($_GET["DISCONECT"])) {
        DesconectaUsuario($_GET["USERID"]);
        unset($_GET["USERID"]);
    }

//$SQL = "select USERID, USERNAME, USERDESC, EMAIL, UACTIVE from userdef where UACTIVE = 1 order by UserId";
    $SQL = "select USERID, USERNAME, USERDESC, EMAIL, UACTIVE, LASTLOGON, ORIGEM from userdef ";

    $SQLOrder = "order by username";

    $where = " ";
    $sqlAnd = " ";
    $sqlCondicoes = " ";
    if ($filtros["search"] !== "") {
        $where = " where ";
        $sqlCondicoes = " ( "
                . "USERNAME like '{$filtros["search"]}%' "
                . "or "
                . "USERDESC like '{$filtros["search"]}%' "
                . " or "
                . "ORIGEM like '{$filtros["search"]}%' "
                . ")";
    }

    if (isset($_GET["USERID"])) {
        $where = " where ";
        $sqlUserId = " USERID = " . $_GET["USERID"];
        $SQLOrder = " ";
    } else {
        $Logados = PegaUsuariosLogados();
    }

    $SQL .= $where . $sqlCondicoes . $sqlAnd . $sqlUserId . $SQLOrder;
    $Query = mysqli_query($connect, $SQL);
    $registrosFiltrados = mysqli_num_rows($Query);
    mysqli_data_seek($Query, $dadosEntrada["start"]);
    $listaUsuarios = array();

    $totalRegistros = 0;
    while ($Linha = mysqli_fetch_assoc($Query)) {
        if ($totalRegistros >= $dadosEntrada["length"]) {
            break;
        }
        /* @var $Linha type */
        $LOGADOS["LOGADO"] = ProcuraUsuario($Linha["USERID"], $Logados);
        $Linha = array_merge($LOGADOS, $Linha);
        $listaUsuarios[] = $Linha;
        $totalRegistros++;
    }

    $TotalRegistros = registrosTabela('userdef');
    $totalTabela = $TotalRegistros;
    $dadosRetorno = apiTrataDadosSaida($dadosEntrada, $listaUsuarios, $totalTabela, $registrosFiltrados);
    $jDados = json_encode($dadosRetorno);

    header('Content-Type: application/json');
    echo $jDados;
}

function api_pegaGrupos()
{
    global $connect;
    $dadosEntrada = apiTrataDadosEntrada();
    $filtros = apiTrataCampos($dadosEntrada);

    if (isset($_GET["DISCONECT"])) {
        DesconectaUsuario($_GET["USERID"]);
        unset($_GET["USERID"]);
    }

//$SQL = "select USERID, USERNAME, USERDESC, EMAIL, UACTIVE from userdef where UACTIVE = 1 order by UserId";
    $SQL = "select groupid, groupname, groupdesc, gactive from groupdef ";

    $SQLOrder = "order by groupname";

    $where = " ";
    $sqlAnd = " ";
    $sqlCondicoes = " ";
    if (isset($dadosEntrada["search"]["value"])) {
        if ($dadosEntrada["search"]["value"] != "") {
            $where = " where ";
            $sqlCondicoes = " ( "
                    . "groupname like '{$dadosEntrada["search"]["value"]}%' "
                    . "or "
                    . "groupdesc like '{$dadosEntrada["search"]["value"]}%' "
                    . ")";
        }
    }

    $SQL .= $where . $sqlCondicoes . $sqlAnd . $sqlUserId . $SQLOrder;

    $Query = mysqli_query($connect, $SQL, MYSQLI_ASSOC);
    $listaUsuarios = array();
    while ($Linha = mysqli_fetch_assoc($Query)) {
        /* @var $Linha type */
        $listaUsuarios[] = $Linha;
    }

    $TotalRegistros = registrosTabela('groupdef');
    $totalTabela = $TotalRegistros;
    $registrosFiltrados = mysqli_num_rows($Query);
    $dadosRetorno = apiTrataDadosSaida($dadosEntrada, $listaUsuarios, $totalTabela, $registrosFiltrados);
    $jDados = json_encode($dadosRetorno);

    header('Content-Type: application/json');
    echo $jDados;
}
