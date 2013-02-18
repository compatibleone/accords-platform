<?php

	print "<form method=\"POST\" action=\"dashboard.php\">\n";

	print "<div align=center><table width='400px'><tr><th>\n";
	print "<p><div class=whiteframe align=center>\n";
	print "<table width='100%'>\n";
	print "<tr><td colspan=4><div align=left><b>Accords Platform User Login</b></div></th></tr>\n";

	print "<tr><td>Name</td>\n";
		print "<td><input type=text name=username size=64></td>\n";
		print "</tr>\n";
	print "<tr><td>Password</td>\n";
		print "<td><input type=password name=password size=64></td>\n";
		print "</tr>\n";

	print "<tr><td>&nbsp;<td><div align=left><input class='action' type=submit name=action value='user login'></div>\n";
	print "<tr><td>&nbsp;<td><div align=left><input class='action' type=submit name=action value='forgot password'></div>\n";
	print "<tr><td>&nbsp;<td><div align=left><input class='action' type=submit name=action value='create account'></div>\n";

	print "</table></div>\n";
	print "</form>\n";
	print "</table></div>\n";

?>

