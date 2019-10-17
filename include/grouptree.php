<?php
function getTopTree()
{
	global $connect;
	$SQL = "select * from groupdef where GroupId in (select GroupDad from grouptree) and GroupId not in (select GroupId from grouptree) ";
	$Query = mysqli_query($connect, $SQL);
	$Groups = array();
	while ($result = mysqli_fetch_array($Query))
	{
		array_push($Groups, $result);
	}
	return $Groups;
}

function TopTree()
{
	echo "<ul class=\"ulgrouptree\">";
	$Groups = getTopTree();
	foreach ($Groups as $Group) 
	{
		$GroupName = $Group["GroupName"];
		$GroupId = $Group["GroupId"];
		echo "<li><a href=\"javascript:void(0);SelectGroups($GroupId, '$GroupName')\">$GroupName</a></li>";
	}
	echo "</ul>";
}

function GroupSams($GroupId, $Chains)
{
	echo "<ul class=\"ulgrouptree\">";
	$Groups = GetGroupSamns($GroupId, $Chains);
	foreach ($Groups as $Group) 
	{
		$GroupName = $Group["GroupName"];
		$GroupId = $Group["GroupId"];
		echo "<li><a href=\"javascript:void(0);SelectGroups($GroupId, '$Chains >> $GroupName')\">$GroupName</a></li>";
	}
	echo "</ul>";	
}

function GetGroupSamns($GroupId, $Chains)
{
	global $connect;
	$SQL = "select * from groupdef where GroupId in
(select GroupId from grouptree where GroupDad = $GroupId)";
	$Query = mysqli_query($connect, $SQL);
	$Groups = array();
	while ($result = mysqli_fetch_array($Query))
	{
		array_push($Groups, $result);
	}
	return $Groups;	
}
?>