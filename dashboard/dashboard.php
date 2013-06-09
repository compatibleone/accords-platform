<?php

require_once( "dashboard.inc"	);
require_once( "cedexis.inc"  	);
require_once( "platform.php"  	);
require_once( "invoice.php"   	);
require_once( "services.php"    );
require_once( "pagemanager.php" );
require_once( "stdform.php"  	);
require_once( "roles.php"    	);

$pageman = new page_manager;

/*	---------------------------------	*/
/*	   p a r s e _ m a n i f e s t 		*/
/*	---------------------------------	*/
function parse_manifest($manifest,$p)
{
	$p->file_inspect( $manifest, $p );
	$a = array();
	$result = exec("bash ./dashboard-parser --verbose ".$manifest,&$a);
	$p->command_output( "Manifest Parsing", $a );
	return( "parse" );
}

/*	---------------------------------	*/
/*	   p a r s e _ cordscript		*/
/*	---------------------------------	*/
function compile_cordscript($script,$p,$n,$o)
{
	$a = array();
	$result = exec("bash ./dashboard-command run ".$script." \"".$o."\"",&$a);
	if ( $n == 'text' )
	{	
	$p->command_output( "Script Compilation and Execution", $a );
	}
	else
	{	
	$p->html_output( "Script Compilation and Execution", $a );
	}	
	return( "compile" );
}

/*	---------------------------------	*/
/*	s l a _ g u a r a n t e e 		*/
/*	---------------------------------	*/
function sla_guarantee( $h, $nb, $gp, $gc, $gv, $gb, $gx )
{
	if (( $gp !='none' ) && ( $gb !='none' ))
	{ 
		fprintf($h,"<term name=\":gt%s\" >\n",$nb);
		fprintf($h,"<guarantee name=\":g%s\" obligated=\"provider\" importance=\"normal\" scope=\"default\">\n",$nb);
		fprintf($h,"<variable name=\":gv%s\" property=\"%s\" condition=\"%s\" value=\"%s\"/>\n", $nb, $gp, $gc, $gv );
		fprintf($h,"<business name=\":bv%s\" nature=\"%s\" expression=\"%s\" />\n", $nb, $gb, $gx );
		fprintf($h,"</guarantee>\n");
		fprintf($h,"</term>\n");
	}
}

/*	---------------------------------	*/
/*	 g e n e r a t e _ a c c o u n t	*/
/*	---------------------------------	*/
function generate_account()
{
	$n = $_REQUEST['account'];
	$m = "account-".$n.".xml";
	$h = fopen( $m ,"w" );
	if ( $h )
	{
		fprintf($h,"<?xml version=\"1.0\" encoding=\"UTF8\"?>\n");
		fprintf($h,"<import name=\"account:%s\" xmlns=\"http://www.compatibleone.fr/schemes/cords.xsd\">\n",$n);
		fprintf($h,"<account name=\"%s\">\n",$n);
		fprintf($h,"<user \n");
		fprintf($h," name=\"%s\"\n",$_REQUEST['name']);
		fprintf($h," password=\"%s\"\n",$_REQUEST['password']);
		fprintf($h," email=\"%s\"\n",$_REQUEST['email']);
		fprintf($h," role=\"%s\"\n",$_REQUEST['role']);
		fprintf($h," authorisation=\"%s\"/>\n",$_REQUEST['auth']);
		fprintf($h,"</account>\n");
		fprintf($h,"</import>\n");
		fclose($h);
		return( $m );
	}
	else	return( "" );
}

/*	---------------------------------	*/
/*	   g e n e r a t e _ s l a 		*/
/*	---------------------------------	*/
function generate_sla($p)
{
	$n = $_REQUEST['slaaccount']."-".$_REQUEST['slaname'];
	$a = explode(" ",$n);
	$n = implode("_",$a);
	$h = fopen( "sla-".$n.".xml","w" );
	if ( $h )
	{
		fprintf($h,"<?xml version=\"1.0\" encoding=\"UTF8\"?>\n");
		fprintf($h,"<agreement name=\"%s\" xmlns=\"http://www.compatibleone.fr/schemes/slam.xsd\"\n",$n);
		fprintf($h,"initiator=\"%s\" responder=\"accords\" serviceprovider=\"responder\"",$_REQUEST['slaaccount']);
		fprintf($h," description=\"%s\" initiation=\"now\" expiration=\"never\" template=\"none\">\n",$_REQUEST['sladesc']);
		fprintf($h,"<terms name=\":s\" type=\"services\">\n");
		fprintf($h,"<term name=\":s\" >\n");
		fprintf($h,"<manifest name=\"%s\"/>\n",$_REQUEST['slaservice']);
		fprintf($h,"</term>\n");
		fprintf($h,"</terms>\n");
		fprintf($h,"<terms name=\":c\" type=\"conditions\">\n");
		if ( $_REQUEST['slaprovider'] != "any" )
		{
			fprintf($h,"<term name=\":ca\" >\n");
			if ( $_REQUEST['slalgo'] == "scripted" )
			{	$slalgo = $_REQUEST['slascript'];	}
			else
			{	$slalgo = "quota:".$_REQUEST['slalgo'];		}
			fprintf($h,"<variable name=\":va\" property=\"occi.placement.algorithm\" condition=\"eq\" value=\"%s\"/>\n",
				$slalgo);
			fprintf($h,"</term>\n");
		}
		if ( $_REQUEST['slaprovider'] != "any" )
		{
			fprintf($h,"<term name=\":cp\" >\n");
			fprintf($h,"<variable name=\":vp\" property=\"occi.placement.provider\" condition=\"eq\" value=\"%s\"/>\n",
				$_REQUEST['slaprovider']);
			fprintf($h,"</term>\n");
		}
		if ( $_REQUEST['slasecurity'] != "any" )
		{
			fprintf($h,"<term name=\":cs\" >\n");
			fprintf($h,"<variable name=\":vs\" property=\"occi.placement.security\" condition=\"eq\" value=\"%s\"/>\n",
				$_REQUEST['slasecurity']);
			fprintf($h,"</term>\n");
		}
		if ( $_REQUEST['slazone'] != "any" )
		{
			fprintf($h,"<term name=\":cz\" >\n");
			fprintf($h,"<variable name=\":vz\" property=\"occi.placement.zone\" condition=\"eq\" value=\"%s\"/>\n",
				$_REQUEST['slazone']);
			fprintf($h,"</term>\n");
		}
		if ( $_REQUEST['slarating'] != "any" )
		{
			fprintf($h,"<term name=\":cr\" >\n");
			fprintf($h,"<variable name=\":vr\" property=\"occi.placement.opinion\" condition=\"eq\" value=\"%s\"/>\n",
				$_REQUEST['slarating']);
			fprintf($h,"</term>\n");
		}
		if ( $_REQUEST['slaenergy'] != "any" )
		{
			fprintf($h,"<term name=\":ce\" >\n");
			fprintf($h,"<variable name=\":ve\" property=\"occi.placement.energy\" condition=\"eq\" value=\"%s\"/>\n",
				$_REQUEST['slaenergy']);
			fprintf($h,"</term>\n");
		}
		if ( $_REQUEST['ejf'] > '0' )
		{
			fprintf($h,"<term name=\":ef\" >\n");
			fprintf($h,"<variable name=\":ef\" property=\"occi.elastic.floor\" condition=\"eq\" value=\"%s\"/>\n",
				$_REQUEST['ejf']);
			fprintf($h,"</term>\n");
			if ( $_REQUEST['ejc'] > '0' )
			{
				fprintf($h,"<term name=\":ec\" >\n");
				fprintf($h,"<variable name=\":ec\" property=\"occi.elastic.ceiling\" condition=\"eq\" value=\"%s\"/>\n",
					$_REQUEST['ejc']);
				fprintf($h,"</term>\n");
			}
			fprintf($h,"<term name=\":es\" >\n");
			fprintf($h,"<variable name=\":es\" property=\"occi.elastic.strategy\" condition=\"eq\" value=\"%s\"/>\n",
				$_REQUEST['ejs']);
			fprintf($h,"</term>\n");
		}

		fprintf($h,"</terms>\n");
		fprintf($h,"<terms name=\":g\" type=\"guarantees\">\n");
		sla_guarantee( $h,1,$_REQUEST['gp1'],$_REQUEST['gc1'],$_REQUEST['gv1'],$_REQUEST['gb1'],$_REQUEST['gx1']);
		sla_guarantee( $h,2,$_REQUEST['gp2'],$_REQUEST['gc2'],$_REQUEST['gv2'],$_REQUEST['gb2'],$_REQUEST['gx2']);
		sla_guarantee( $h,3,$_REQUEST['gp3'],$_REQUEST['gc3'],$_REQUEST['gv3'],$_REQUEST['gb3'],$_REQUEST['gx3']);
		sla_guarantee( $h,4,$_REQUEST['gp4'],$_REQUEST['gc4'],$_REQUEST['gv4'],$_REQUEST['gb4'],$_REQUEST['gx4']);
		fprintf($h,"</terms>\n");
		fprintf($h,"</agreement>\n");
		fclose( $h );
		parse_manifest( "sla-".$n.".xml", $p );
		return( "sla" );
	}
	else
	{
		return("");
	}
}


/*	---------------------------------	*/
/*	   u p l o a d _ m an i f e s t		*/
/*	---------------------------------	*/
function upload_manifest($p)
{
	copy( $_FILES['upload']['tmp_name'], $_FILES['upload']['name'] );
	parse_manifest( $_FILES['upload']['name'], $p );
	return( "upload" );
}

/*	---------------------------------	*/
/*	   u p l o a d _ cordscript		*/
/*	---------------------------------	*/
function upload_cordscript($p)
{
	copy( $_FILES['script']['tmp_name'], "csp-".$_FILES['script']['name'] );
	$p->file_inspect( "csp-".$_FILES['script']['name'], $p );
	compile_cordscript( "csp-".$_FILES['script']['name'], $p,$_REQUEST['output'],$p->user." ".$_REQUEST['option'] );
	return( "compile" );
}

/*	---------------------------------	*/
/*	   compile and run			*/
/*	---------------------------------	*/
function compile_and_run($p)
{
	compile_cordscript( $_REQUEST['script'], $p,$_REQUEST['output'], $p->user." ".$_REQUEST['option'] );
	return( "compile" );
}

/*	---------------------------------	*/
/*	   inspect script			*/
/*	---------------------------------	*/
function inspect_script($p)
{
	$p->file_inspect( $_REQUEST['script'], $p );
	return( "inspect" );
}

/*	---------------------------------	*/
/*	   c r e a t e _ a c c o u n t 		*/
/*	---------------------------------	*/
function create_account()
{
	if ( $_REQUEST['accountname'] != "" )
	{
		return( "account" );
	}
	else	return( "" );
}

/*	---------------------------------	*/
/*	   c r e a t e _ u s e r 		*/
/*	---------------------------------	*/
function create_user()
{
	if ( $_REQUEST['name'] != "" )
	{
		return( "user" );
	}
	else	return( "" );
}

/*	---------------------------------	*/
/*	     u s e r _ l o g i n		*/
/*	---------------------------------	*/
function user_login( $n, $p )
{
	if ( $n == "" )
	{	return( "user login required" );	}
	else if ( $p == "" )
	{	return( "user password required" );	}

	$qn = "\"".$n."\"";
	$qp = "\"".$p."\"";
	$a = array();
	exec("grep name cords_user.xml",&$a);
	foreach ($a as $l )
	{
		$un = "";
		$up = "";
		$ur = "";
		$u = explode(" ",$l);
		foreach ( $u as $f )
		{
			$i = explode("=",$f);
			if ( $i[0] == "name" )
				$un = $i[1];
			else if ( $i[0] == "password" )
				$up = $i[1];
			else if ( $i[0] == "role" )
				$ur = $i[1];
		}
		if (( $un == $qn ) && ( $up == $qp ))
		{
			$r = explode("\"",$ur);
			$a = array();
			$result = exec("bash ./dashboard-command run csp-login.txt ".$un." ".$up,&$a);
			return( $r[1] );
		}
	}

	/* ------------------- */
	/* test standard users */
	/* ------------------- */
	if (( $n == "administrator" ) && ( $p == "administrator" ))
	{	return( $n );		}
	else if (( $n == "user" ) && ( $p == "user" ))
	{	return( $n );		}
	else if (( $n == "operator" ) && ( $p == "operator" ))
	{	return( $n );		}
	else if (( $n == "provider" ) && ( $p == "provider" ))
	{	return( $n );		}
	else
	{	return( "incorrect user login or password" );	}
}

/*	---------------------------------	*/
/*		n e w _ u s e r			*/
/*	---------------------------------	*/
function new_user()
{
	$manifest = generate_account();
	if ( $manifest != "" )
	{
		$a = array();
		$result = exec("bash ./dashboard-parser --verbose ".$manifest,&$a);
	}
}

/*	---------------------------------	*/
/*	     p a g e _ a c c e s s 		*/
/*	---------------------------------	*/
function page_access($p)
{
	$role= "none";
	$now = time();
	if ( $_COOKIE['session'] == "" )
	{
		if ( $_REQUEST['action'] == "user login" )
		{
			$role = user_login($_REQUEST['username'],$_REQUEST['password'] );
			switch ( $role )
			{
			case	"user"	:
			case	"administrator" :
			case	"operator"	:
			case	"provider"	:
				$p->thistab = "1";
				setcookie("session",$_REQUEST['username'].":".$_REQUEST['password'].":".$now.":".$role,0);
				$p->set_user($_REQUEST['username']);
				return( $role );
			default			:
			
				setcookie("session","",0);
				return( $role );
			}
		}
		else if ( $_REQUEST['action'] == "create account" )
		{
			$role = "newaccount";
			setcookie("session","",0);
			return( $role );
		}
		else if ( $_REQUEST['action'] == "new account" )
		{
			$role = "newuser";
			setcookie("session","",0);
			return( $role );
		}
		else if ( $_REQUEST['action'] == "new user" )
		{
			new_user();
			$role = "user account created";
			setcookie("session","",0);
			return( "" );
		}
		else
		{
			return( "" );
		}
	}
	else if ( $_REQUEST['action'] == "logout" )
	{
		setcookie("session","",0);
		return( "" );
	}
	else
	{
		$a = explode(":",$_COOKIE['session'] );

		if ( ($a[2]+900) < ($now+0) )
		{	return( "incorrect session" );	}

		$role = user_login( $a[0], $a[1] );
		switch ( $role )
		{
			case	"user"	:
			case	"administrator":
			case	"operator":
			case	"provider":
				$p->set_user( $a[0] );
				setcookie("session",$a[0].":".$a[1].":".$now.":".$role,0);
				return( $a[3] );
			default		:
				setcookie("session","",0);
				return( $role );
		}
	}
}

/*	-----------------------------------	*/
/*	     d e f i n e _ a c c o u n t 	*/
/*	-----------------------------------	*/
function define_account()
{
	page_display("100");
}

/*	-----------------------------------	*/
/*	g e n e r a t e _ d a s h b o a r d	*/ 
/*	-----------------------------------	*/
function generate_dashboard($pageman)
{
	$pageman->role = page_access($pageman);
	$extra = "";
	switch ( $pageman->role )
	{
	case	"administrator"	:
		$role = new admin_role( $pageman );
		break;
	case	"user"		:
		$role = new user_role( $pageman );
		$extra = "csp-welcome.txt";
		break;
	case	"operator"	:
		$role = new user_role( $pageman );
		break;
	case	"provider"	:
		$role = new user_role( $pageman );
		break;
	case	"vendor"	:
		$role = new user_role( $pageman );
		break;
	default			:
		$pageman->reset();
		$role = new login_role( $pageman );
		break;
	}

	$role->open();
	$role->action();
	$role->display();
	if ( strlen($extra) > 0)
	{
		compile_cordscript( $extra, "HTML", $p->user );
	}
	$role->close();

}


$pageman->thistab=0;

generate_dashboard($pageman);

?>


