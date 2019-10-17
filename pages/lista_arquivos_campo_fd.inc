<?php
require_once(FILES_ROOT . "include/resource01.php");
require_once(FILES_ROOT . "pages/editcase_campos.inc");

use raelgc\view\Template;

echo ListaArquivosCampoImagem($ProcId, $CaseNum, $FieldId, $ReadOnly, $tipoFolderList, $MultipleFiles);
