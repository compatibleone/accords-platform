<?php
		print "<form action='dashboard.php' method='POST'  enctype=\"multipart/form-data\">\n";
		print "<div class=whiteframe>\n";
		print "<table width='100%'>";
		print "<tr><td colspan=2><div align=left><b>Upload and Run Script</b></div>\n";
		print "<tr><td>Script<td><div align=center><input class='upload' type='file' name='script'></div></tr>\n";
		print "<tr><td>Output<td><div align=center><select class='output' name='output'><option value='html'>HTML</option><option value='text'>TEXT</option></select></div></tr>\n";
		print "<tr><td>Option<td><div align=center><input class='edit' type='text' name='option'></div></td></tr>\n";
		print "<tr><td>Submit<td><div align=center><input class='action' type='submit' name='action' value='upload and compile'></div></tr>\n";
		print "</table></div></form>\n";

		print "<div class=whiteframe>\n";
		print "<table width='100%'>";
		print "<tr><td colspan=2><div align=left><b>Run Script</b></div>\n";
		print "<tr><td colspan=2><ul>\n";
		$a = array();
		exec("ls csp-*",&$a);
		foreach ( $a as $l )
		{
			print "<li><a href='/accords/dashboard.php?action=compile&output=HTML&script=".$l."'>".$l."</a>\n";
		}
		print "</ul></td></tr>\n";
		print "</table></div><p>\n";

		print "<form action='dashboard.php' method='POST'  enctype=\"multipart/form-data\">\n";
		print "<div class=whiteframe>\n";
		print "<table width='100%'>";
		print "<tr><td colspan=2><div align=left><b>Run Script with Options</b></div>\n";
		print "<tr><td>Script<td><div align=center><select class='output' name='script'>\n";
		$a = array();
		exec("ls csp-*",&$a);
		foreach ( $a as $l )
		{
			print "<option value='".$l."'>".$l."</option>\n";
		}
		print "</select></div></tr>\n";
		print "<tr><td>Output<td><div align=center><select class='output' name='output'><option value='html'>HTML</option><option value='text'>TEXT</option></select></div></tr>\n";
		print "<tr><td>Option<td><div align=center><input class='edit' type='text' name='option'></div></td></tr>\n";
		print "<tr><td>Submit<td><div align=center><input class='action' type='submit' name='action' value='compile'></div></tr>\n";
		print "</table></div></form>\n";

		print "<form action='dashboard.php' method='POST'  enctype=\"multipart/form-data\">\n";
		print "<div class=whiteframe>\n";
		print "<table width='100%'>";
		print "<tr><td colspan=2><div align=left><b>Inspect Script</b></div>\n";
		print "<tr><td>Script<td><div align=center><select class='output' name='script'>\n";
		$a = array();
		exec("ls csp-*",&$a);
		foreach ( $a as $l )
		{
			print "<option value='".$l."'>".$l."</option>\n";
		}
		print "</select></div></tr>\n";
		print "<tr><td>Submit<td><div align=center><input class='action' type='submit' name='action' value='inspect script'></div></tr>\n";
		print "</table></div></form>\n";

?>
