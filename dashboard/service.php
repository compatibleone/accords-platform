<?php
	require_once("dashboard.inc");
	global $configdir;

	page_comment("This page gives allows access to the Accords Platform Service Maintenance operations.");
	separator();
	print "<tr><th class=category><a href=\"dashboard.php?action=stop\"><img width='128' height='158' src='images/stop.png'></a></th>\n";
	print "<th class=description>Use this option to terminate an active service instance and release all deployed resources. The service may be restarted using the appropriate service maintenance operation.";
	print "<th colspan=4>";
	print "<form action='dashboard.php' method='POST'>\n";
	print "<p><div class=whiteframe align=center>\n";
	print "<table width='100%'>\n";
	print "<tr><td>Service<td><select style='width: 50mm;' name='service'>\n";
	$a = array();
	exec("ls -t $configdir/service/*",&$a);
	foreach ($a as $l )
	{
		print "<option value='".$l."'>".$l."</option>\n";
	}
	print "</select>\n";
	print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='inspect service'></div></tr>\n";
	print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='stop service'></div></tr>\n";
	print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='delete service'></div></tr>\n";
	print "</table></div>\n";
	print "</form></tr>";

	separator();
	print "<tr><th class=category><a href=\"dashboard.php?action=start\"><img width='128' height='158' src='images/start.png'></a></th>\n";
	print "<th class=description>Use this option to restart a terminated service instance and deploy all provisioned resources.";
	print "<th colspan=4>\n";
	print "<form action='dashboard.php' method='POST'>\n";
	print "<p><div class=whiteframe align=center>\n";
	print "<table width='100%'>\n";
	print "<tr><td>Service<td><select style='width: 50mm;' name='service'>\n";
	$a = array();
	exec("ls -t $configdir/service/*",&$a);
	foreach ($a as $l )
	{
		print "<option value='".$l."'>".$l."</option>\n";
	}
	print "</select>\n";
	print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='start service'></div></tr>\n";
	print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='restart service'></div></tr>\n";
	print "</table></div></tr>\n";
	print "</form><p>\n";

	separator();
	print "<tr><th class=category><a href=\"dashboard.php?action=save\"><img width='128' height='158' src='images/check2.png'></a></th>\n";
	print "<th class=description>Use this option to snapshot or save the state of the machine images of the contracts of a service.";
	print "<th colspan=4>\n";
	print "<form action='dashboard.php' method='POST'>\n";
	print "<p><div class=whiteframe align=center>\n";
	print "<table width='100%'>\n";
	print "<tr><td>Service<td><select style='width: 50mm;' name='service'>\n";
	$a = array();
	exec("ls -t $configdir/service/*",&$a);
	foreach ($a as $l )
	{
		print "<option value='".$l."'>".$l."</option>\n";
	}
	print "</select>\n";
	print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='save service'></div></tr>\n";
	print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='snapshot service'></div></tr>\n";
	print "</table></div></tr>\n";
	print "</form>\n";
	separator();

?>
