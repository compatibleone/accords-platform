<?php

class login_role extends role
{
	public	$status=NULL;

	public	function login_role($p)
	{
		$this->status = "";
		$this->page = $p;
	}

	public	function action()
	{

		
	}

	public	function display()
	{
		$this->page->comment("Please specify valid authentication credentials to be allowed access to this site");
		$this->page->separator();
		$login = new standard_form( "Accords Platform User Login", "400px", "dashboard.php");
		$login->open();
		$login->input( "Name", "username", "", 64 );
		$login->password( "Password", "password", "", 64 );
		$login->action( "&nbsp;", "user login" );
		$login->action( "&nbsp;", "forgot password" );
		$login->action( "&nbsp;", "create account" );
		$login->close();
		$this->page->separator();
	}

};

?>
