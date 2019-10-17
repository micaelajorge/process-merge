<?php
/*
 * rota: /selgruposuser
 */
// <editor-fold defaultstate="collapsed" desc="Incializacao">


$gmtDate = gmdate("D, d M Y H:i:s");
header("Content-Type: text/html; charset=utf8", true);
header("Expires: {$gmtDate} GMT");
header("Last-Modified: {$gmtDate} GMT");
header("Cache-Control: no-cache, must-revalidate");
header("Pragma: no-cache");

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="Definição do Template">

use raelgc\view\Template;

$t_body = new Template(FILES_ROOT . "assets/templates/t_adm_permissoes_selusergroup_list.html");
// </editor-fold>

$Total = 0;
$Filtrar = filter_input(INPUT_GET, "Filtrar");
$procId = filter_input(INPUT_GET, "procId");
$Users = array();
$Groups = array();
$Campos = array();
if ($Filtrar <> "") {
    require_once("common.php");
    require_once("ldap.php");
    switch ($OrigemLogon) {
        case "AD":
            if ($Grupo <> 0) {
                $GrupoSam = PegaSamAccountNameGrupo($Grupo);
                $Users = PegaMembrosGrupo($GrupoSam, $Filtrar);
            } else {
                $Users = PegaUsuariosAD($Filtrar);
            }
            break;
        case "ProcessLogon":
            switch ($tipoPesquisa) {
                case "U":
                    $Users = PegaUsuariosProcessPermissoes($Filtrar);
                    break;
                case "G":
                    $Groups = PegaGruposProcessPermissoes($Filtrar);
                    break;
                case "UG":
                    $Groups = PegaGruposProcessPermissoes($Filtrar);
                    $Users = PegaUsuariosProcessPermissoes($Filtrar);
                    $Campos = PegaCamposProcessPermissoes($Filtrar, $procId);
                    break;
            }
            //$Users = array_merge($Users, $Groups, $Campos);
            $Users = array_merge($Users, $Groups, $Campos);
            break;
        case "External":
            $Users = PegaUsuariosExterno($Filtrar, $Grupo, $Origem, $TipoFiltro);
            break;
    }
    $Total = count($Users);
}


$listaEntidades = $_SESSION["listaEntidades"];
if ($Total >= 0) {
    for ($i = 0; $i < $Total; $i++) {
        $NomeLista = $Users[$i]["UserName"];
        $NomeLista = str_replace("'", "", $NomeLista);
        $accountLista = $Users[$i]["samaccountname"];
        $grpFld = $Users[$i]["grpFld"];
        $userId = $Users[$i]["UserId"];
//        if (in_array($userId, $listaEntidades)) {
//            $grpFld = "P";
//        }

        $USERDESC = $Users[$i]["UserDesc"];
        if (!empty($Users[$i]["UserId"])) {
            $valor = $Users[$i]["UserId"];
        } else {
            $valor = $Users[$i]["samaccountname"];
        }
        $classe = "LinhaUser";
        if ($Nome == $accountLista) {
            $classe = "LinhaUserSel";
            $ItemSel = $i;
        }

        // Popula a lista de usuários
        $t_body->USERNAME = $NomeLista;
        $t_body->USER_ID = $valor;
        $t_body->USERDESC = $USERDESC;
        $t_body->NRLINHA = $i;
        $t_body->GRPFLD = $grpFld;

        switch ($grpFld) {
            case "U":
                $t_body->CLASS_GRUPOUSUARIO = "fa-user text-primary";
                $t_body->block("BLOCK_USUARIO");
                break;
            case "G":
                $t_body->CLASS_GRUPOUSUARIO = "fa-users text-warning";
                $t_body->block("BLOCK_USUARIO");
                break;
            case "P":
                $t_body->CLASS_GRUPOUSUARIO = "fa-ban";
                $t_body->block("BLOCK_USUARIO_DISABLE");
                break;
            case "F":
                $t_body->CLASS_GRUPOUSUARIO = "fa-user-secret text-green";
                $t_body->block("BLOCK_USUARIO");
                break;
        }
    }
    $t_body->block("BLOCK_LISTA_USUARIOS");
}
$t_body->show();