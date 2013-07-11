<?php
require_once("dashboard.inc");
global $configdir;
	print "<form method=\"POST\" action=\"dashboard.php\">\n";
	print "<table>\n";
	print "<tr><th>Name<td><select name=nodetype>\n";
	print "<option value=public>none</option>\n";
	$a = array();
	exec("grep name $configdir/cords_node.xml | cut -f 3 -d = | cut -f 2 -d '\"' ",$a);
	foreach ($a as $l )
	{
		print "<option value='$l'>$l</option>\n";
	}
	print "</select>\n";
	print "<tr><th>Name<td><input type=text name=nodename value=' ' size=32>\n";
	print "<tr><th>Access<td><select name=nodeaccess>\n";
	print "<option value=public>public</option>\n";
	print "<option value=public>private</option>\n";
	print "</select>\n";
	print "<tr><th>Scope<td><select name=nodescope>\n";
	print "<option value=public>normal</option>\n";
	print "<option value=public>common</option>\n";
	print "</select>\n";
	print "<tr><th>Type<td><select name=nodetype>\n";
	print "<option value=public>simple</option>\n";
	$a = array();
	exec("grep name $configdir/cords_manifest.xml | cut -f 3 -d = | cut -f 2 -d '\"' ",$a);
	foreach ($a as $l )
	{
		print "<option value='$l'>$l</option>\n";
	}
	print "</select>\n";
	print "<tr><th>Category<td><select name=nodescope>\n";
	print "<option value=public>manifest</option>\n";
	print "<option value=public>paas_manifest</option>\n";
	print "</select>\n";
	print "</table>\n";
	print "</form>\n";
?>

