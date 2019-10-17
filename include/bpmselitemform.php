
<!-- BPMSelItemForm -->

<div id="DivMasterLista" style="position:Absolute;width:514px;height:304px;Display:none;top:0px;border: 1px #CCCCCC">
<iframe style="position:Absolute;top:1px;left:0px;width:503px;height:304px;z-index:999" src="blank.html"></iframe>
<div style="position:Absolute;z-index:1000;width:100%;height:100%;background-color: #CCCCCC;padding:1px">
<div class="DivTopPopUp">
    <div style="float:left">Seleção de Valor em Lista</div>
  <input id="BotaoWindowClose" type="button" value="" onClick="EscondeDivSelLista()"></div>
<div id="DivSelLista">
	<div id="DivConteudoLista" style="height:250px;overflow:auto;">	
	</div>
    <div class="DivPanel">       
    <table width="100%" border="0">
      <tr> 
        <td width="45%"><div align="center"> 
            <input name="AtualizaSelUser" type="button" class="buttonOk" value="Limpar" onclick="MudaItemLista('','')">
          </div></td>
        <td width="55%"><div align="center"> 
            <input name="CancelaSelUser" type="button" class="buttonCancel" value="Cancelar" onClick="EscondeDivSelLista()">
          </div></td>
      </tr>
    </table>
    </div>
</div>
</div>
</div>

<!-- BPMSelItemForm -->
