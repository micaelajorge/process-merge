<?php
require_once(FILES_ROOT . "include/resource01.php");
include(FILES_ROOT . "include/ldap.php");

function PegaDadosAddress($GroupId, $GrpFld) {
    global $AGrupos, $AUsuarios, $Fields, $ProcId;
    switch ($GrpFld) {
        case "F":
            $Retorno["TipoGrupo"] = "Campo do Processo";
            if (!is_array($Fields)) {
                $Fields = CarregaListaCamposProcessoExterno($ProcId, "", "FieldId, FieldName, FieldCod");
            }
            for ($i = 0; $i < count($Fields); $i++) {
                if ($Fields[$i]["FieldId"] == $GroupId) {
                    $Retorno["GroupName"] = $Fields[$i]["FieldName"] . " (" . $Fields[$i]["FieldCod"] . ")";
                    break;
                }
            }
            break;
        case "G":
            if (empty($AGrupos[$GroupId])) {
                $AGrupos[$GroupId] = PegaNomeGrupo($GroupId);
            }
            $Retorno["GroupName"] = $GroupId . " - " . $AGrupos[$GroupId];
            $Retorno["TipoGrupo"] = "Grupo";
            break;
        case "U":
            if (empty($AUsuarios[$GroupId])) {
                $AUsuarios[$GroupId] = PegaNomeUsuario($GroupId);
            }
            $Retorno["GroupName"] = $GroupId . " - " . $AUsuarios[$GroupId];
            $Retorno["TipoGrupo"] = "Usuário";
            break;
    }
    return $Retorno;
}

if (empty($procName)) {
    $ProcName = PegaNomeProc($procId);
}
?>
<html>
    <head>
        <link href="styles/report.css" rel="stylesheet" type="text/css">
    </head>
    <title>
        Lista de Permissões no Processo - <?= $ProcName ?>
    </title> 
    <body>
        <div class="ListaCampos">
            Emissão: <?= date("d/m/Y h:i") ?>
            <div class="TituloRelatorio">Lista de Permissões no Processo<br>
                Processo: <?= $ProcName ?>
            </div>
            <table cellpadding="0" cellspacing="2" class="ReportTable" border="0">
                </tr>
<?php
$SQL = "select GroupId, GrpFld, Action from geraladmins where ProcId = $procId order by GrpFld desc";
$Query = mysqli_query($connect, $SQL);
if (mysqli_num_rows($Query) > 0) {
    ?>
                    <tr> 
                        <td colspan="6" class="tdTituloReport"> 
                            Administradores Gerais
                        </td>
                    </tr>	
                    <tr> 
                        <td>&nbsp;</td>
                        <td class="tdTituloReport">Grupo/Usuário</td>
                        <td class="tdTituloReport">Tipo</td>
                        <td class="tdTituloReport" colspan="3">Editar</td>
                    </tr>  	
    <?php
    while ($Result = mysqli_fetch_array($Query)) {
        $DadosAddress = PegaDadosAddress($Result["GroupId"], $Result["GrpFld"]);
        $Editar = "NãO";
        if ($Result["Action"] == 1) {
            $Editar = "SIM";
        }
        ?>
                        <tr> 
                            <td>&nbsp;</td>
                            <td><?= $DadosAddress["GroupName"] ?></td>
                            <td><?= $DadosAddress["TipoGrupo"] ?></td>
                            <td colspan="3"><?= $Editar ?></td>
                        </tr>  		
                        <?php
                    }
                    ?>
                    <tr><td colspan="6">&nbsp;<td></tr>
                    <?php
                }
                $SQL = "select GroupId, GrpFld, Action from geraladmins where ProcId = $procId order by GrpFld desc";
                $Query = mysqli_query($connect, $SQL);
                if (mysqli_num_rows($Query) > 0) {
                    ?>
                    <tr> 
                        <td colspan="6" class="tdTituloReport"> 
                            Administradores de Processo
                        </td>
                    </tr>	
                    <tr> 
                        <td>&nbsp;</td>
                        <td class="tdTituloReport">Grupo/Usuário</td>
                        <td class="tdTituloReport">Tipo</td>
                        <td class="tdTituloReport" colspan="3">Editar</td>
                    </tr>  	
                    <?php
                    while ($Result = mysqli_fetch_array($Query)) {
                        $DadosAddress = PegaDadosAddress($Result["GroupId"], $Result["GrpFld"]);
                        $Editar = "não";
                        if ($Result["Action"] == 1) {
                            $Editar = "SIM";
                        }
                        ?>
                        <tr> 
                            <td>&nbsp;</td>
                            <td><?= $DadosAddress["GroupName"] ?></td>
                            <td><?= $DadosAddress["TipoGrupo"] ?></td>
                            <td colspan="3"><?= $Editar ?></td>
                        </tr>  	
                        <tr>
                        <?php
                    }
                    ?>
                        <td colspan="6">&nbsp;<td></tr>	
                    <?php
                }
                $SQL = "select * from stepadmins where ProcId = $procId order by StepId, GrpFld desc";
                $Query = mysqli_query($connect, $SQL);
                if (mysqli_num_rows($Query) > 0) {
                    ?>
                    <tr> 
                        <td colspan="6" class="tdTituloReport"> 
                            Administradores de Tarefa
                        </td>
                    </tr>	

                        <?php
                        $StepIdOld = '';
                        while ($Result = mysqli_fetch_array($Query)) {
                            if ($StepIdOld != $Result["StepId"]) {
                                $StepIdOld = $Result["StepId"];
                                $StepName = PegaNomeStep($procId, $StepIdOld, 0, 1);
                                ?>
                            <tr> 
                                <td colspan="6"> 
                                    Tarefa: <?= $StepName ?>
                                </td>
                            </tr>	
                            <tr> 
                                <td>&nbsp;</td>
                                <td class="tdTituloReport">Grupo/Usuário</td>
                                <td class="tdTituloReport">Tipo</td>
                                <td class="tdTituloReport" colspan="3">Editar</td>
                            </tr>  		
                            <?php
                        }
                        $DadosAddress = PegaDadosAddress($Result["GroupId"], $Result["GrpFld"]);
                        $Editar = "NÃO";
                        if ($Result["Action"] == 1) {
                            $Editar = "SIM";
                        }
                        ?>
                        <tr> 
                            <td>&nbsp;</td>
                            <td><?= $DadosAddress["GroupName"] ?></td>
                            <td><?= $DadosAddress["TipoGrupo"] ?></td>
                            <td colspan="3"><?= $Editar ?></td>
                        </tr>  	
                        <tr>
        <?php
    }
    ?>
                        <td colspan="6">&nbsp;<td></tr>	
                    <?php
                }
                $ShowFile = false;
                $StepIdOld = '';
                $SQL = "select SD.StepId, StepName, StepCod, GroupId, GrpFld, Action, viewQueue, NewDoc from stepaddresses SA, stepdef SD where SD.ProcId = $procId and SA.ProcId = SD.ProcID and SA.StepId = SD.StepId and SD.Active = 1 order by StepName, StepCod, GRPFLD desc";
                $Query = mysqli_query($connect, $SQL);
                while ($Result = mysqli_fetch_array($Query)) {
                    if ($StepIdOld != $Result["StepId"]) {
                        if ($StepIdOld != '') {
                            ?>
                            <tr><td colspan="6">&nbsp;<td></tr>
            <?php
        }
        ?>   
                        <tr>
                            <td colspan="6"> 
                                Tarefa: <?= $Result["StepId"] ?> - <?= $Result["StepName"] ?> (<?= $Result["StepCod"] ?>)
                            </td>
                        </tr>	
                        <tr> 
                            <td>&nbsp;</td>
                            <td class="tdTituloReport">Grupo/Usuário/Tarefa</td>
                            <td class="tdTituloReport">Tipo</td>
                            <td class="tdTituloReport">Editar</td>
                            <td class="tdTituloReport">Visualizar Fila</td>
                            <td class="tdTituloReport">Novo caso</td>          
                        </tr>  	
                        <?php
                        $StepIdOld = $Result["StepId"];
                    }

                    $Editar = "Não";
                    if ($Result["Action"] == 1) {
                        $Editar = "SIM";
                    }

                    $viewQueue = "Não";
                    if ($Result["viewQueue"]) {
                        $viewQueue = "SIM";
                    }

                    $NewDoc = "Não";
                    if ($Result["NewDoc"]) {
                        $NewDoc = "SIM";
                    }
                    $DadosAddress = PegaDadosAddress($Result["GroupId"], $Result["GrpFld"]);
                    ?>
                    <tr> 
                        <td>&nbsp;</td>
                        <td><?= $DadosAddress["GroupName"] ?></td>
                        <td><?= $DadosAddress["TipoGrupo"] ?>
                        </td>
                        <td><?= $Editar ?>
                        </td>
                        <td><?= $viewQueue ?>
                        </td>          
                        <td><?= $NewDoc ?>
                        </td>          
                    </tr>
                    <?php
                }
                ?>
            </table>
        </div>
    </body>
</html>