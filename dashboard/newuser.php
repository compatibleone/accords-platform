<?php

	print "<form method=\"POST\" action=\"dashboard.php\">\n";

	print "<div class=whiteframe align=center>\n";
	print "<table width='100%'>\n";
	print "<tr><td colspan=4><div align=left><b>Step 2: Add a New Accords Platform User</b></td></tr>\n";

	print "<tr><td>Name</td><td><input name=\"name\" type=text size=48>\n";
	print "<tr><td>Password</td><td><input name=\"password\" type=text size=48>\n";
	print "<tr><td>Email</td><td><input name=\"email\" type=text size=48>\n";

	print "<tr><td>Role</td><td><select name=\"role\">\n";
	print "<option value=\"user\">user</option>\n";
	print "<option value=\"user\">admin</option>\n";
	print "<option value=\"user\">agent</option>\n";
	print "<option value=\"user\">provider</option>\n";
	print "<option value=\"user\">operator</option>\n";
	print "</select>\n";

	print "<tr><td>Account</td><td><input name=\"account\" readonly type=text value=\"".$_REQUEST['account']."\" size=48>\n";

	print "<tr><td>Authorization</td><td><select name=\"auth\">\n";
	print "<option value=\"crud\">all</option>\n";
	print "<option value=\"crux\">no delete</option>\n";
	print "<option value=\"crxx\">no update</option>\n";
	print "<option value=\"xrxx\">no create</option>\n";
	print "<option value=\"xxxx\">no access</option>\n";
	print "</select>\n";

	print "<tr><td>Submit<td><div align=center><input class='action' type=submit name=action value='new user'></div>\n";

	print "</table></div>\n";
	print "</form>\n";

?>

