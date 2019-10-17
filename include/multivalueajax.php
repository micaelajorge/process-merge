<?php
require_once("include/resource01.php");
$MultiValue = Tickler($ProcId, $FieldId, $CaseNum);
$Acao = "BPMTickler.php?ProcId=$ProcId&FieldId=$FieldId&CaseNum=$CaseNum&Tickler=0&FieldName=$nome";
$Estilo = "style=\"visibility=hidden\"";
if (!empty($ReadOnly)) {
    $CampoReadOnly = "readonly=\"readonly\"";
}
?>
<div class="modal-header">
    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">×</button>
    <h4 class="modal-title " id="myModalLabel">Hist&oacute;rico do Campo</h4>
</div>
<div class="modal-body">
    <table>
        <?php
        $TotalComentarios = count($MultiValue);
        $comentarios = 0;
        if ($TotalComentarios > 0) {
            for ($contador = 1; $contador < (count($MultiValue) + 1); $contador++) {
                $coment = $MultiValue[$contador];
                $Editar = "NovaLinha=" . $coment[4] . "&ProcId=$ProcId&FieldId=$FieldId&CaseNum=$CaseNum&Id=" . $coment[4] . "&Tickler=$Tickler&Edit=1&FieldName=$FieldName";
                $Resposta = "NovaLinha=" . ($comentarios + 4) . "&ProcId=$ProcId&FieldId=$FieldId&CaseNum=$CaseNum&Id=" . $coment[4];
                if (empty($comentario[$contador][3])) {
                    $comentarios++;
                }
                ?>
                <tr> 
                    <td>
                        <p>
                            <?= nl2br(htmlentities($coment[2])); ?><br />
                            <span class="text-muted small">Usu&aacute;rio: <?= htmlentities($coment[1]); ?> Data:  <?= htmlentities($coment[0]); ?></span>
                        </p>
                    </td>
                </tr>
                <?php
            }
        }
        ?>
    </table>
</div>
<div class="modal-footer">    
</div>