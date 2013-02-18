<?php


class role
{
	public	$page=NULL;

	public	function open()
	{
		if ( $this->page )
		{
			$this->page->open();
		}
	}

	public	function close()
	{
		if ( $this->page )
		{
			$this->page->close();
		}
	}

};

require_once ("loginrole.php" );
require_once ("adminrole.php" );
require_once ("userrole.php" );
require_once ("providerrole.php" );
require_once ("operatorrole.php" );

?>
