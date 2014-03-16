<?php

class	service_operations
{
	public	$status=NULL;

/*	---------------------------------	*/
/*	   i n s t a n c e _ p l a n 		*/
/*	---------------------------------	*/
public function create($p,$n)
{
	if ( $_REQUEST['upload'] != "" )
	{
		$a = array();
		if ( $n )
			$result = exec("bash ./dashboard-broker --no-deployment --verbose --name ".$n." ".$_REQUEST['upload'],$a);
		else	$result = exec("bash ./dashboard-broker --no-deployment --verbose ".$_REQUEST['upload'],$a);
		$p->command_output( "Instance Plan", $a );
		return( "instance" );
	}
	else	return( "" );
}
/*	---------------------------------	*/
/*	   s t o p _ s e r v i c e 		*/
/*	---------------------------------	*/
public function stop($p)
{
	$a = array();
	exec("bash ./dashboard-command --verbose stop ".$_REQUEST['service'],$a);
	$p->command_output( "Stop Service", $a );
	return( "stop" );
}

/*	---------------------------------	*/
/*	 r e c o v e r _ s e r v i c e 		*/
/*	---------------------------------	*/
public function recover($p)
{
	$a = array();
	exec("bash ./dashboard-command --verbose recover ".$_REQUEST['service'],$a);
	$p->command_output( "Recover Service", $a );
	return( "recover" );
}

/*	---------------------------------	*/
/*	 s c a l e u p _ s e r v i c e 		*/
/*	---------------------------------	*/
public function scaleup($p)
{
	$a = array();
	exec("bash ./dashboard-command --verbose scaleup ".$_REQUEST['service'],$a);
	$p->command_output( "Stop Service", $a );
	return( "stop" );
}

/*	---------------------------------	*/
/*	s c a l e d o w n _ s e r v i c e 	*/
/*	---------------------------------	*/
public function scaledown($p)
{
	$a = array();
	exec("bash ./dashboard-command --verbose scaledown ".$_REQUEST['service'],$a);
	$p->command_output( "Stop Service", $a );
	return( "stop" );
}

/*	---------------------------------	*/
/*	   s a v e _ s e r v i c e 		*/
/*	---------------------------------	*/
public function save($p)
{
	$a = array();
	exec("bash ./dashboard-command --verbose save ".$_REQUEST['service'],$a);
	$p->command_output( "Save Service", $a );
	return( "save service" );
}

/*	---------------------------------	*/
/*	d e l e t e_ s e r v i c e 		*/
/*	---------------------------------	*/
public function delete($p)
{
	$a = array();
	exec("bash ./dashboard-command --verbose delete ".$_REQUEST['service'],$a);
	$p->command_output( "Delete Service", $a );
	return( "delete service" );
}


/*	---------------------------------	*/
/*	s n a p s h o t _ s e r v i c e 	*/
/*	---------------------------------	*/
public function snapshot($p)
{
	$a = array();
	exec("bash ./dashboard-command --verbose snapshot ".$_REQUEST['service'],$a);
	$p->command_output( "Snapshot Service", $a );
	return( "restart service" );
}

/*	---------------------------------	*/
/*	  r e s t a r t _ s e r v i c e 	*/
/*	---------------------------------	*/
public function restart($p)
{
	$a = array();
	exec("bash ./dashboard-command --verbose restart ".$_REQUEST['service'],$a);
	$p->command_output( "Restart Service", $a );
	return( "restart service" );
}

/*	---------------------------------	*/
/*	   s t a r t _ s e r v i c e 		*/
/*	---------------------------------	*/
public function start($p)
{
	$a = array();
	exec("bash ./dashboard-command --verbose start ".$_REQUEST['service'],$a);
	$p->command_output( "Start Service", $a );
	return( "stop" );
}

/*	---------------------------------	*/
/*	   i n s p e c t _ p l a n 		*/
/*	---------------------------------	*/
public function inspect_plan($p)
{
	if ( $_REQUEST['upload'] != "" )
	{
		$p->file_inspect( $_REQUEST['upload'], $p );
		return( "inspection" );
	}
	else	return( "" );
}

};


?>
