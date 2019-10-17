<?php
if (key_exists("REQUEST_URI", $_SERVER)) {
    $redirectUrl = $_SERVER["REQUEST_URI"];
} else {
    $redirectUrl = $_SERVER["REDIRECT_SCRIPT_URI"];
}
$servidor = $_SERVER["HTTP_HOST"];

echo "---------------- $redirectUrl<br />";
echo "---------------- $servidor<br />";
echo "---------------- " . __DIR__;
?>
<h1>DB</h1>
<?php
echo "BPMUSSER $BPMUSER <br />";
echo "BPMPWD $BPMPWD <br />";
echo "EXTERNALUSER $EXTERNALUSER <br />";
echo "EXTERNALPWD $EXTERNALPWD <br />";
echo "EXTERNAL_USERNAME $EXTERNAL_USERNAME <br />";
echo "BPM $BPMDB <br />";
?>