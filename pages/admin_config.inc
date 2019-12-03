<?php
/*
Criação: Marcelo Mosczynski <mmoscz@gmail.com>
Data Criação 19/09/2018
Sistema: Process_XAMPP
*/

include_once(FILES_ROOT . "include/common.php");
include_once(FILES_ROOT . "include/func_api.inc");
include_once(FILES_ROOT . "include/toolbar.inc");

use raelgc\view\Template;

$dadosEntrada = apiTrataDadosEntrada();
$filtros = apiTrataCampos($dadosEntrada);

$mobileMode = 0;
if (isset($_SESSION["mobileMode"]))
{
    $mobileMode = $_SESSION["mobileMode"];
}

$t_body = new Template(FILES_ROOT . "assets/templates/master/t_master.html");
$t_body->addFile("PAGEHEADER", FILES_ROOT . "assets/templates/master/t_header.html");
$t_body->addFile("PAGEMENULEFT", FILES_ROOT . "assets/templates/master/t_menu.html");

// Adiciona o template de DATATABLE
$t_body->addFile("PAGECONTENT", FILES_ROOT . "assets/templates/t_admin_config.html");

$t_body->block("BLOCK_CONTENT");

$t_body->SIDE_BAR_SIZE = "sidebar-mini sidebar-collapse";

$t_body->addFile("PAGEFOOTER", FILES_ROOT . "assets/templates/master/t_footer.html");

if ($mobileMode)
{
    $t_body->addFile("TOOLBAR", FILES_ROOT . "assets/templates/master/t_toolbar.html");
} else {
    $t_body->addFile("TOOLBAR_LEFT", FILES_ROOT . "assets/templates/master/t_toolbar_left.html");
}

//$t_body->addFile("FILTROS", FILES_ROOT . "assets/templates/t_auditoria.html");

$t_body->MAIN_FOOTER_HIDEN = "hidden";

$t_body->SITE_LOCATION = SITE_LOCATION;
$t_body->TITULO_PAGE = "Segurança | Process";
$t_body->HEADER_TEXT = "Segurança";
$t_body->HEADER_SMALL_TEXT = "Configuração de Segurança";
$t_body->HEADER_ICO = "fa fa-lock";
$t_body->HEADER_COLOR = "#3c8dbc";

$t_body->NAV_HEADER_TEXT = "Segurança";
$t_body->NAV_HEADER_ICO = "fa fa-lock";
$t_body->NAV_HEADER_CLASS = "text-primary";
$t_body->NAV_HEADER_COLOR = "";

$t_body->NAGEVAGAO_LINK = "javascript:";
$t_body->block("BLOCO_NAVEGACAO"); // Inclui na navegação do Site
$t_body->block("BLOCO_MOSTRAR_NAVEGACAO");

$t_body->PROCID = $ProcId;
$t_body->PROCNAME = $procdef->ProcName;

// Definições do Usuário
$t_body->USER_DESC = $userdef->UserDesc;
$t_body->USER_NAME = $userdef->UserName;
$t_body->USER_ORIGEM = $userdef->Origem;
$t_body->USER_EMAIL = $userdef->Email;

$SQL = "select * from config";
$Query = mysqli_query($connect, $SQL);

$configs = mysqli_fetch_all($Query, MYSQLI_ASSOC);

foreach ($configs as $config)
{
    $nomeConfig = $config["Funcao"];
    $$nomeConfig = $config["Valor"];
}

$t_body->MAX_FALHA_LOGON = $maxFalhaLogon;
$t_body->MESES_INATIVIDADE = $mesesInatividade;
$t_body->EMAIL_NOTIFICATION_PASSWORD = $emailNotificationPassword;
$t_body->EMAIL_NOTIFICATION_NAME = $emailNotificationUser;
$t_body->EMAIL_NOTIFICATION_SERVER = $emailNotificationServer;
$t_body->EMAIL_NOTIFICATION_PORT = $emailNotificationPort;

$t_body->ARQUIVO_JS = "admin_config.js"; // Incicializacao DataTables
$t_body->block("BLOCO_JAVASCRIPT");

// <editor-fold defaultstate="collapsed" desc="Popula toolbar / Side Bar">
$Menu = "config_seguranca";
populaToolBarTemplate($t_body, $Menu);
//$t_body->block("BLOCK_SIDEBAR");
// </editor-fold>
/// <editor-fold defaultstate="collapsed" desc="Envia dados para o Browser">
$t_body->show();
// </editor-fold>



