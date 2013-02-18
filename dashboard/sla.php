<?php

	print "<form method=\"POST\" action=\"dashboard.php\">\n";

	print "<p><div class=whiteframe align=center>\n";
	print "<table width='100%'>\n";
	print "<tr><td colspan=4><div align=left><b>Service Level Agreement</b></div></th></tr>\n";

	print "<tr><td>Name</td>\n";
		print "<td><input type=text name=slaname size=64></td>\n";
		print "</tr>\n";
	print "<tr><td>Description</td>\n";
		print "<td><input type=text name=sladesc size=64></td>\n";
		print "</tr>\n";

	print "<tr><td>Account</td>";
	print "<td><select style='width: 50mm;' name=slaaccount>\n";
	$a = array();
	exec("grep name cords_account.xml | cut -f 4 -d = | cut -f 2 -d '\"' ",&$a);
	foreach ($a as $l )
	{
		if ( $l != "" )
		{
			print "<option value='$l'>$l</option>\n";
		}
	}
	print "</select></td>\n";

	print "<tr><td>Manifest</td><td><select style='width: 50mm;' name=slaservice>\n";
	$a = array();
	exec("grep name cords_manifest.xml | cut -f 3 -d = | cut -f 2 -d '\"' ",&$a);
	foreach ($a as $l )
	{
		if ( $l != "" )
		{
			print "<option value='$l'>$l</option>\n";
		}
	}
	print "</select></td>\n";
	print "<tr><td colspan=4><div align=left><b>Service Conditions</b></div></th></tr>\n";
	print "<tr><td>Algorithm</td>";
	print "<td><select style='width: 50mm;' name=slalgo>\n";
	print "<option value=default>default</option>\n";
	print "<option value=scripted>scripted</option>\n";
	print "<option value=zone>zone</option>\n";
	print "<option value=security>security</option>\n";
	print "<option value=energy>energy</option>\n";
	print "<option value=opinion>reputation</option>\n";
	print "</select></td>\n";

	print "<tr><td>Scripted</td><td><input type=text name=slascript value='fairpack'>\n";
	print "<tr><td>Provider</td><td>\n";
	print "<select style='width: 50mm;' name=slaprovider>\n";
	print "<option value=any>any</option>\n";
	$a = array();
	exec("grep name cords_provider.xml | cut -f 5 -d = | cut -f 2 -d '\"' ",&$a);
	foreach ($a as $l )
	{
		if ( $l != "" )
		{
			print "<option value='$l'>$l</option>\n";
		}
	}
	print "</select></td></tr>\n";

	print "<tr><td>Zone<td><select style='width: 50mm;' name=slazone>\n";
	print "<option value=any>any</option>\n";
	print "<option value=westeurope>west europe</option>\n";
	print "<option value=easteurope>east europe</option>\n";
	print "<option value=northasia>north asia</option>\n";
	print "<option value=southasia>south asia</option>\n";
	print "<option value=northamerica>north america</option>\n";
	print "<option value=southameric>south america</option>\n";
	print "<option value=southafrica>south africa</option>\n";
	print "<option value=northafrica>north africa</option>\n";
	print "</select></td></tr>\n";

	print "<tr><td>Rating<td><select style='width: 50mm;' name=slarating>\n";
	print "<option value=any>any</option>\n";
	for ( $i=1; $i <= 10; $i += 1 )
	{
		print "<option value='".$i."'>".$i."</option>\n";
	}
	print "</select></td></tr>\n";

	print "<tr><td>Security<td><select style='width: 50mm;' name=slasecurity>\n";
	print "<option value=any>any</option>\n";
	print "<option value=trusted>trusted</option>\n";
	print "<option value=untrusted>untrusted</option>\n";
	print "</select></td></tr>\n";

	print "<tr><td>Energy<td><select style='width: 50mm;' name=slaenergy>\n";
	print "<option value=any>any</option>\n";
	print "<option value=high>high</option>\n";
	print "<option value=normal>normal</option>\n";
	print "<option value=low>low</option>\n";
	print "</select></td></tr>\n";

	print "<tr><td colspan=4><div align=left><b>Service Guarantees</b></div></th></tr>\n";
	
	print "<tr><td>Property</td><td >\n";
	print "<select style='width: 50mm;' name=gp1>\n";
	print "<option value=none>none</option>\n";
	$a = array();
	exec("grep name cords_metric.xml | cut -f 3 -d = | cut -f 2 -d '\"' ",&$a);
	foreach ($a as $l )
	{
		if ( $l != "" )
		{
			print "<option value='$l'>$l</option>\n";
		}
	}
	print "</select></td></tr>\n";

	print "<tr><td>Compare</td><td >\n";
	print "<select style='width: 50mm;' name=gc1>\n";
	print "<option value=eq>equal</option>\n";
	print "<option value=ne>not equal</option>\n";
	print "<option value=gr>greater</option>\n";
	print "<option value=le>not greater</option>\n";
	print "<option value=ls>less</option>\n";
	print "<option value=ge>not less</option>\n";
	print "</select></td></tr>\n";

	print "<tr><td>Value</td><td><input type=text name=gv1 size=64></td>\n";

	print "<tr><td>Business Value</td><td >\n";
	print "<select style='width: 50mm;' name=gb1>\n";
	print "<option value=none>none</option>\n";
	print "<option value=penalty>penalty</option>\n";
	print "<option value=reward>reward</option>\n";
	print "</select></td></tr>\n";
	print "<tr><td>Expression</td><td><input type=text name=gx1 size=64></td>\n";
	print "<tr><td colspan=2><hr></td></tr>\n";

	print "<tr><td>Property</td><td >\n";
	print "<select style='width: 50mm;' name=gp2>\n";
	print "<option value=none>none</option>\n";
	$a = array();
	exec("grep name cords_metric.xml | cut -f 3 -d = | cut -f 2 -d '\"' ",&$a);
	foreach ($a as $l )
	{
		if ( $l != "" )
		{
			print "<option value='$l'>$l</option>\n";
		}
	}
	print "</select></td></tr>\n";

	print "<tr><td>Compare</td><td >\n";
	print "<select style='width: 50mm;' name=gc2>\n";
	print "<option value=eq>equal</option>\n";
	print "<option value=ne>not equal</option>\n";
	print "<option value=gr>greater</option>\n";
	print "<option value=le>not greater</option>\n";
	print "<option value=ls>less</option>\n";
	print "<option value=ge>not less</option>\n";
	print "</select></td></tr>\n";

	print "<tr><td>Value</td><td><input type=text name=gv2 size=64></td>\n";

	print "<tr><td>Business Value</td><td >\n";
	print "<select style='width: 50mm;' name=gb2>\n";
	print "<option value=none>none</option>\n";
	print "<option value=penalty>penalty</option>\n";
	print "<option value=reward>reward</option>\n";
	print "</select></td></tr>\n";
	print "<tr><td>Expression</td><td><input type=text name=gx2 size=64></td>\n";
	print "<tr><td colspan=2><hr></td></tr>\n";

	print "<tr><td>Property</td><td >\n";
	print "<select style='width: 50mm;' name=gp3>\n";
	print "<option value=none>none</option>\n";
	$a = array();
	exec("grep name cords_metric.xml | cut -f 3 -d = | cut -f 2 -d '\"' ",&$a);
	foreach ($a as $l )
	{
		if ( $l != "" )
		{
			print "<option value='$l'>$l</option>\n";
		}
	}
	print "</select></td></tr>\n";

	print "<tr><td>Compare</td><td >\n";
	print "<select style='width: 50mm;' name=gc3>\n";
	print "<option value=eq>equal</option>\n";
	print "<option value=ne>not equal</option>\n";
	print "<option value=gr>greater</option>\n";
	print "<option value=le>not greater</option>\n";
	print "<option value=ls>less</option>\n";
	print "<option value=ge>not less</option>\n";
	print "</select></td></tr>\n";

	print "<tr><td>Value</td><td><input type=text name=gv3 size=64></td>\n";

	print "<tr><td>Business Value</td><td >\n";
	print "<select style='width: 50mm;' name=gb3>\n";
	print "<option value=none>none</option>\n";
	print "<option value=penalty>penalty</option>\n";
	print "<option value=reward>reward</option>\n";
	print "</select></td></tr>\n";
	print "<tr><td>Expression</td><td><input type=text name=gx3 size=64></td>\n";
	print "<tr><td colspan=2><hr></td></tr>\n";

	print "<tr><td>Property</td><td >\n";
	print "<select style='width: 50mm;' name=gp4>\n";
	print "<option value=none>none</option>\n";
	$a = array();
	exec("grep name cords_metric.xml | cut -f 3 -d = | cut -f 2 -d '\"' ",&$a);
	foreach ($a as $l )
	{
		if ( $l != "" )
		{
			print "<option value='$l'>$l</option>\n";
		}
	}
	print "</select></td></tr>\n";

	print "<tr><td>Compare</td><td >\n";
	print "<select style='width: 50mm;' name=gc4>\n";
	print "<option value=eq>equal</option>\n";
	print "<option value=ne>not equal</option>\n";
	print "<option value=gr>greater</option>\n";
	print "<option value=le>not greater</option>\n";
	print "<option value=ls>less</option>\n";
	print "<option value=ge>not less</option>\n";
	print "</select></td></tr>\n";

	print "<tr><td>Value</td><td><input type=text name=gv4 size=64></td>\n";

	print "<tr><td>Business Value</td><td >\n";
	print "<select style='width: 50mm;' name=gb4>\n";
	print "<option value=none>none</option>\n";
	print "<option value=penalty>penalty</option>\n";
	print "<option value=reward>reward</option>\n";
	print "</select></td></tr>\n";
	print "<tr><td>Expression</td><td><input type=text name=gx4 size=64></td>\n";
	print "<tr><td colspan=2><hr></td></tr>\n";

	print "<tr><td>Submit<td><div align=center><input class='action' type=submit name=action value='generate sla then parse'></div>\n";

	print "</table></div>\n";
	print "</form>\n";
?>

