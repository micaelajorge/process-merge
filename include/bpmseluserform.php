
<!-- BPMSelUserForm -->

<div id="DivMasterUser" class="DivMasterUser" style="display:none">
	<iframe class="IframeSelUser" src="blank.html"></iframe>
	<div class="DivDisplayDadosUser">
		<div class="DivTopPopUp">
    		<div class="TituloPopUp">Seleção de Usuário</div>
  				<input id="BotaoWindowClose" type="button" value="" onClick="LimpaListaUsers()">
  		</div>
		<div id="DivSelUser">
	  		<div class="DivTitulos">
	  			Usuário - Nome (Origem)
	  		</div>
			<div id="DivConteudoUser" class="DivConteudoUser">
				<div class="DivListaUser">
				</div>
			</div>
		    <div class="DivPanelSelUser">       
			    <table width="100%" border="0">
			      <tr> 
			        <td colspan="2" align="center">
			          <input type="radio" name="TypePesq" id="TypePesqSelUser" value="0" checked><label>Usuário</label><input type="radio" name="TypePesq" value="1" id="TypePesqSelNome"><label>Nome</label><br />
			          <label>Procurar Usuário </label>
			          <input name="Filtrar" type="text" id="Filtrar" tabindex="0" size="20" maxlength="30" value="<?= $Filtrar ?>"> 
			          <input name="Submit3" type="button" value="Localizar" onClick="MostraListaUsers('<?= $Origem ?>')" class="buttonsSearch"> 
			        </td>
			      </tr>
			      <tr> 
			        <td width="45%"><div align="center"> 
			            <input name="AtualizaSelUser" type="button" class="buttonOk" value="Ok" onclick="AtualizaUser(uValor, dValor)">
			          </div></td>
			        <td width="55%"><div align="center"> 
			            <input name="CancelaSelUser" type="button" class="buttoncancel" value="Cancelar" onClick="LimpaListaUsers()">
			          </div></td>
			      </tr>
			    </table>
	    	</div>
		</div>
	</div>
</div>
<!-- BPMSelUserForm -->
