/*
 Criação: Marcelo Mosczynski <mmoscz@gmail.com>
 Data Criação 26/07/2019
 Sistema: Process_XAMPP
 */

function js_ext_tipo_assinatura_click(FIELD_CODE)
{
    stringBusca = '[aria-type-assing="' + FIELD_CODE + '"]';
    itensCheck = $(stringBusca);    
    tiposSelecinados = [];
    itensCheck.each((indice, item) => {
        if ($(item).is(":checked"))
        {
            tiposSelecinados.push($(item).val());
        }
    });
    json = JSON.stringify(tiposSelecinados);
    $('[aria-field-code="' + FIELD_CODE + '"]').val(json);
}