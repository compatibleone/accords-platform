<?php

	print "<form action='dashboard.php' method='POST'>\n";
	print "<div align=center><table width='60%'><tr><th>\n";
	if ( $_REQUEST['account'] == "" )
	{
		require( "newaccount.php" );
	}
	else
	{
		require( "newuser.php" );
	}

	print "</table></div>\n";

?>
