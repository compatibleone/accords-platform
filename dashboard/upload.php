<?php
		print "<form action='dashboard.php' method='POST'  enctype=\"multipart/form-data\">\n";
		print "<div class=whiteframe>\n";
		print "<table width='100%'>";
		print "<tr><td colspan=2><div align=left><b>Document Upload</b></div>\n";
		print "<tr><td>Document<td><div align=center><input class='upload' type='file' name='upload'></div></tr>\n";
		print "<tr><td>Submit<td><div align=center><input class='action' type='submit' name='action' value='upload and parse'></div></tr>\n";
		print "</table></div></form>\n";
?>
