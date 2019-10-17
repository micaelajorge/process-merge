<?php

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

$t_body = new Template(FILES_ROOT . "assets/templates/t_selusergrouplist.html");
// </editor-fold>


$Total = 0;
$Filtrar = filter_input(INPUT_GET, "Filtrar");
if ($Filtrar <> "") {
    //require_once("common.php");
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
            $Users = PegaUsuariosProcess($Filtrar, $Grupo, $Origem, $TipoFiltro);
            break;
        case "External":
            $Users = PegaUsuariosExterno($Filtrar, $Grupo, $Origem, $TipoFiltro);
            break;
    }

    $Total = count($Users);
}
if ($Total >= 0) {
    for ($i = 0; $i < $Total; $i++) {
        $NomeLista = $Users[$i]["UserName"];
        $NomeLista = str_replace("'", "", $NomeLista);
        $accountLista = $Users[$i]["samaccountname"];
        $Origem = $Users[$i]["Origem"];
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
        $t_body->ACCONTLISTA = $accountLista;
        $t_body->NOME = $NomeLista;
        $t_body->VALOR = $valor;
        $t_body->ORIGEM = $Origem;
        $t_body->NRLINHA = $i;
        
        $t_body->block("BLOCK_USUARIO");
    }
    $t_body->block("BLOCK_LISTA_USUARIOS");
}
$t_body->show();
?>