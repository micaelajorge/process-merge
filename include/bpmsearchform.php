<!-- // //Versao 1.0.0 /Versao -->
<div id="DivMasterSearch" style="display:none"> 
    <div class="DivTopPopUp">      
        <div style="float:left" id="DivTituloAdHoc">Filtrar</div>
        <input id="BotaoWindowClose" type="button" value="" onClick="EscondeSearchForm()">
    </div>
    <div id="DivSearch"> 
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <form name="SearchForm" method="post" action="">
                <tr>             
                    <td colspan="3">
                        <table width="100%" border="0">
                            <?php
                            $CamposRef = $S_procdef["Referencias"];
                            for ($i = 0; $i < count($CamposRef); $i++) {
                                $Campo = 'FCampo' . $CamposRef[$i]["FieldId"];
                                ?>
                                <tr> 
                                    <td height="24" align="right"> 
                                        <?= $CamposRef[$i]["FieldName"] ?>
                                        :</td>
                                    <td>
                                        <?php
                                        if ($CamposRef[$i]["FieldType"] == "US") {
                                            $campo = "FCampo" . $CamposRef[$i]["FieldId"];
                                            $Destinatario = MontaCampoUsuario($campo, '', '', '', '', '', '');
                                            ?>
                                            <?= $Destinatario ?>                
                                            <?php
                                        } else {
                                            ?>	                	
                                            <input type="text" name="FCampo<?= $CamposRef[$i]["FieldId"] ?>" value="<?= $$Campo ?>" id="FCampo<?= $CamposRef[$i]["FieldId"] ?>" /> 
                                            <?php
                                            if ($CamposRef[$i]["FieldType"] == "DT") {
                                                IncluiCalendarioXML("FCampo" . $CamposRef[$i]["FieldId"]);
                                            }
                                        }
                                        ?>
                                    </td>
                                </tr>           
                                        <?php
                                    }
                                    ?>			
                        </table> 
                    </td>
                </tr>
                <tr> 
                    <td width="28%" height="43" align="center"> 
                        <input name="Button" type="button" class="btntoolbar" value="Aplicar" onClick="document.SearchForm.LimparFiltros.value = ''; SubmitSearchForm('<?= $URLSearch ?>');" ></td>
                    <td width="44%" align="center">
                        <input name="Submit2" type="button" class="btntoolbar" value="Remover" onClick="LimpaPesquisa(); document.SearchForm.LimparFiltros.value = '1'; SubmitSearchForm('<?= $URLSearch ?>');
                    " >
                    </td>
                    <td width="28%" align="center">
                        <input name="Submit22" type="button" class="btntoolbar" value="Cancelar" onClick="EscondeSearchForm()"></td>
                </tr>
                <input name="LimparFiltros" type="hidden" id="LimparFiltros">
                <input name="CampoOrdem" type="hidden" id="CampoOrdem" value="<?= $CampoOrdem ?>">
                <input name="Ordem" type="hidden" id="Ordem" value="<?= $Ordem ?>">
                <input name="ProcId" type="hidden" id="ProcId" value="<?= $ProcId ?>">
                <input name="Pagina" type="hidden" id="Pagina" value="1">
                <input name="Linhas" type="hidden" id="Linhas" value="<?= $Linhas ?>">
                <input name="Selecionado" type="hidden" id="Selecionado" value="<?= $Selecionado ?>">
                <input name="Ac" type="hidden" id="Ac" value="<?= $Ac ?>">					
            </form>
        </table>
    </div>
</div>