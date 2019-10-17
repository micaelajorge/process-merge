<?php

/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 31/07/2018
  Sistema: Process_XAMPP
 */

function wsPegacategorias()
{
    global $connect;
    $SQL = "select * from categorias";
    $Query = mysqli_query($connect, $SQL);
    $resultado = mysqli_fetch_all($Query, MYSQLI_ASSOC);
    return $resultado;
}

function wsPegaProcessosUsuario($AuthUser)
{
    global $connect, $userdef;
    if (!AuthenticateWs($AuthUser)) {
        GeraErro(4);
        return array();
    }

    $gruposUsuario = $userdef->usergroups;
    $idUsuario = $userdef->UserId;
    $SQL = "select 
proccod, 
stepcod, 
procname, 
stepname,
stepdesc,
procdesc,
fieldname,
fielddesc,
fieldtype,
fieldlength,
extendprop,
orderid,
optional
from 
procdef, stepaddresses, stepdef, procfieldsdef,
stepfieldsdef
where 
stepaddresses.ProcId = procdef.procid 
and 
(
(stepaddresses.GroupId = $idUsuario and GrpFld = 'U') 
or 
(stepaddresses.GroupId in ($gruposUsuario) and GrpFld = 'G')
) 
and 
mobile = 1 
and 
NewDoc = 1 
and 
stepdef.procId = procdef.procId 
and 
stepdef.stepid = stepaddresses.stepid 
and
stepfieldsdef.stepid = stepdef.stepid 
and
stepfieldsdef.procid = stepdef.procid 
and
procfieldsdef.procid = stepdef.procid 
and
procfieldsdef.fieldid = stepfieldsdef.fieldid 
and 
stepfieldsdef.readonly = 0 ";
    $Query = mysqli_query($connect, $SQL);

    $resultado = mysqli_fetch_all($Query, MYSQLI_ASSOC);

    $listaProcessosRetorno = array();

    $listacategorias = wsPegacategorias();

    $processoOld = $resultado[0]["proccod"];
    $stepOld = $resultado[0]["stepcod"];
    $processo["proccod"] = $resultado[0]["proccod"];
    $processo["procname"] = $resultado[0]["procname"];
    $processo["procdesc"] = $resultado[0]["procdesc"];
    $processo["stepcod"] = $resultado[0]["stepcod"];
    $processo["stepname"] = $resultado[0]["stepname"];
    $processo["stepdesc"] = $resultado[0]["stepdesc"];
    $codCategoria = $resultado[0]["cod_categoria"];
    $indiceCategoria = array_search($codCategoria, array_column($listacategorias, 'cod_categoria'));
    if ($indiceCategoria !== false) {
        $processo["categoria"] = $listacategorias[$indiceCategoria];
    }

    $processo["fields"] = array();
    $campo = array();

    foreach ($resultado as $processoAtual) {

        // Acrescenta se mudou o processo ou o passo
        if ($processo["proccod"] != $processoOld | $processo["stepcod"] != $stepOld) {
            array_push($listaProcessosRetorno, $processo);
            $processo["proccod"] = $processoAtual["proccod"];
            $processo["procdesc"] = $processoAtual["procdesc"];
            $processo["procdesc"] = $processoAtual["procdesc"];
            $processo["stepname"] = $processoAtual["stepname"];
            $processo["stepcod"] = $processoAtual["stepcod"];
            $processo["stepdesc"] = $processoAtual["stepdesc"];

            // Pega as categorias do Processo;
            $codCategoria = $resultado[0]["cod_categoria"];
            $indiceCategoria = array_search($codCategoria, array_column($listacategorias, 'cod_categoria'));
            if ($indiceCategoria !== false) {
                $processo["categoria"] = $listacategorias[$indiceCategoria];
            }


            $processo["fields"] = array();
            $campo = array();
        }
        $campo["fieldname"] = $processoAtual["fieldname"];
        $campo["fielddesc"] = $processoAtual["fielddesc"];
        $campo["fieldtype"] = $processoAtual["fieldtype"];
        $campo["fieldlenght"] = $processoAtual["fieldlength"];
        $campo["extenddata"] = $processoAtual["extendprop"];

        // Altera de Opcional para obrigatório
        $campo["obrigatory"] = ($processoAtual["optional"] == "1") ? 0 : 1;
        $campo["order"] = $processoAtual["orderid"];
        array_push($processo["fields"], $campo);
    }
    array_push($listaProcessosRetorno, $processo);

    $retorno = array();
    $retorno["processos"] = $listaProcessosRetorno;
    $retorno["erro"] = EncerraErros();
    return $retorno;
}
