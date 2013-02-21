<?php
require_once("dashboard.inc");
global $configdir;

class	page_manager
{

	public	$tabpage=1;
	public	$copyright=NULL;
	public	$title=NULL;
	public	$timer;
	public  $role="";
	public	$user="";
	public	$account="";

	/*	--------------------------------	*/
	/*	    p a g e _ m a n a g e r		*/
	/*	--------------------------------	*/
	public	function page_manager($u="",$r="")
	{
		$this->copyright = array( 
			"Copyright &copy; 2012 Iain James Marshall &amp; Jean-Pierre Laisn&eacute;",
			"All images are licensed by the Library of the Faculty of Law and Labour of the University of Seville under CC BY 2.0" 
			);
		$this->tabpage=1;
		$this->timer=60;
		$this->title = "Accords Platform Dashboard";
		$this->role = $r;
		$this->user = $u;
	}

	/* --------------- */
	/* s e t _ u s e r */
	/* --------------- */
	public	function set_user( $u )
	{
		global $configdir;
		$this->user = $u;
		$a = array();
		exec( "grep name $configdir/cords_account.xml | grep jamie | cut -f 2 -d = | cut -f 2 -d '\"'",&$a);
		$this->account = $a[0];
	}

	/*	---------------------------	*/
	/*	p e r i o d i c R e l o a d 	*/
	/*	---------------------------	*/
	function periodicReload()
	{
		$nbsec = $this->timer;
		print "<script language=\"javascript\"";
		print " type=\"text/javascript\">\n";
		print "function reloadPage(newpage)\n";
		print "{\n";
		print "window.location = newpage;\n";
		print "return(false);\n";
		print "}\n";
		if ( $nbsec > 0 )
		{
			print "var prTimer = setTimeout('reloadPage(window.location)',";
			print $nbsec*1000;
		}
		print ");\n";
		print "</script>\n";
	}

	/*	--------------------------------	*/
	/*	       s e p a r a t o r 		*/
	/*	--------------------------------	*/
	public function separator()
	{
		print "<tr><th class='separator' colspan=6>&nbsp;</th></tr>\n";
	}

	/*	--------------------------------	*/
	/*	       c o m m e n t			*/
	/*	--------------------------------	*/
	public function comment($message)
	{
		print "<tr><th class='comment' colspan=6>".$message."<br></th></tr>\n";
	}

	/*	--------------------------------	*/
	/*	       w a r n i n g			*/
	/*	--------------------------------	*/
	public function warning($message)
	{
		print "<tr><th class='warning' colspan=6>".$message."<br></th></tr>\n";
	}

	/*	---------------------------------	*/
	/*	     	r e s e t 			*/
	/*	---------------------------------	*/
	public	function reset()
	{
		setcookie("session","",0);
	}

	/*	---------------------------------	*/
	/*	      f i l e _ o u t p u t		*/
	/*	---------------------------------	*/
	public 	function file_inspect( $fn, $p )
	{
		if (!( $a = file( $fn ) ))
		{
			return;
		}
		$p->separator();
		print "<tr><th colspan=6>\n";
		print "<table border=1 width='100%' style='overflow:scroll'><tr><th>\n";
		print "<div align=center class='title'>File Inspection : ".$fn."</div>\n";
		print "<tr><td style='overflow:scroll'><pre>\n";
		foreach ($a as $l )
		{
			$r = str_replace("&","&amp;",$l);
			$l = str_replace(">","&gt;",$r);
			$r = str_replace("<","&lt;",$l);
			print $r;
		}
		print "</pre></td></tr></table>\n";
		print "</th></tr>\n";
		$p->separator();
	}

	/*	---------------------------------	*/
	/*	   c o m m a n d _ o u t p u t		*/
	/*	---------------------------------	*/
	public	function command_output( $t, $a )
	{
		$this->separator();
		print "<tr><th colspan=6>\n";
		print "<table border=1 width='100%' style='overflow:scroll'>\n";
		print "<tr><th><div align=center class='title'>".$t." Ouput</div>\n";
		print "<tr><td style='overflow:scroll'>\n";
		foreach ($a as $l )
		{
			$r = str_replace("&","&amp;",$l);
			$l = str_replace(">","&gt;",$r);
			$r = str_replace("<","&lt;",$l);
			print $r."<br>\n";
		}
		print "</table>\n";
		print "</th></tr>\n";
		$this->separator();
	}
	/*	---------------------------------	*/
	/*	   h t m l  _ o u t p u t		*/
	/*	---------------------------------	*/
	public	function html_output( $t, $a )
	{
		$this->separator();
		print "<tr><th colspan=6>\n";
		print "<table border=1 width='100%' style='overflow:scroll'>\n";
		/* print "<tr><th><div align=center class='title'>".$t." Ouput</div>\n"; */
		print "<tr><td style='overflow:scroll'>\n";
		foreach ($a as $l )
		{
			print $l."\n";
		}
		print "</table>\n";
		print "</th></tr>\n";
		$this->separator();
	}

	/*	--------------------------------	*/
	/*	       o p e n 				*/
	/*	--------------------------------	*/
	public	function open()
	{
		/* --------------------- */
		/* Start Page Production */
		/* --------------------- */
		print "<html><head><title>".$this->title."</title>\n";
		print "<link href='dashboard.css' rel='stylesheet' type='text/css' media='all'/></head>\n";
		print "<body>\n";
		print "<div align=center><div class=frame>\n";
		if ( $this->user != "" )
		{
			print "<div align=right><a href=\"dashboard.php?action=logout\">Logout ".$this->user."(".$this->account.")</a></div>\n";
		}
		else
		{
			print "<br>\n";
		}
		print "<table width='98%'><tr><th class=category><div align=center><p>\n";
		print "<table width='98%'>\n";
		print "<tr><th class=banner colspan=6><div align=left><img src='imgs/banner.png'></div>\n";
	}

	/*	---------------------------	*/
	/*	d i s p l a y _ n u m b e r	*/
	/*	---------------------------	*/
	private	function display_number($n)
	{
		$dashboard_number = "0";
		if ( $dashboard_number == "1" )
		{
			for ( $i=0; $i < strlen( $n ); $i += 1 )
			{
				$c = substr( $n,$i, 1);
				print "<img width='40' height='48' src='imgs/".$c."-icon.png'>\n";
			}
		}
		else	{	print $n;	}
	}

	/*	-------------------------	*/
	/*	w i n d o w e d _ h r e f 	*/
	/*	-------------------------	*/
	public	function windowed_href( $windowname, $url )
	{
		print "href=\"javascript:window.open('".$url."','".$windowname."','fullscreen=no,toolbar=yes,scrollbars=yes',false);\"";
	}

	/*	-------------------------	*/
	/*	w i n d o w e d _ l i n k	*/
	/*	-------------------------	*/
	public function windowed_link( $url )
	{
		print "<a ";
		$this->windowed_href( "accords",$url );
		print ">";
	}


	/*	--------------------------------	*/
	/*	 c o r d s _ s t a t i s t i c 		*/
	/*	--------------------------------	*/
	private function cords_statistic($n,$a)
	{
		print "<th class='category'>";
		$this->windowed_link( $a."/".$n."/" );
		print "<img width=128 height=158 src='imgs/".$n.".png'>";
		print "</a></th>";
		print "<td class='category'>";
		print "<table><tr><td class='category'>&nbsp;</td><tr>\n";
		print "<td class='value'>";
		$v = exec("./dashboard-stat.sh cords_".$n);
		$this->display_number($v);
		print "</td>\n";
		print "<tr><td class='category'>&nbsp;</td><tr></table></tD>\n";
	}

	/*	----------------------------------------	*/
	/*	f u l l _ c o r d s _ s t a t i s t i c		*/
	/*	----------------------------------------	*/
	private function full_cords_statistic($n,$a)
	{
		print "<th class='category'>";
		$this->windowed_link( $a."/".$n."/" );
		print "<img width=128 height=158 src='imgs/".$n.".png'>";
		print "</a></th><td class='value'>";
		$v = exec("./dashboard-stat.sh cords_".$n);
		$this->display_number($v);
		print "</td>\n";
	}

	/*	--------------------------------	*/
	/*	       s t a t i s t i c 		*/
	/*	--------------------------------	*/
	private function statistic($n,$a)
	{
		print "<th class='category'>";
		$this->windowed_link( $a."/".$n."/" );
		print "<img width=128 height=158 src='imgs/".$n.".png'>";
		print "</a></th>";
		print "<td class='category'>";
		print "<table><tr><td class='category'>&nbsp;</td><tr>\n";
		print "<td class='value'>";
		$v = exec("./dashboard-stat.sh ".$n);
		$this->display_number($v);
		print "</td>\n";
		print "<tr><td class='category'>&nbsp;</td><tr></table></tD>\n";
	}

	/*	---------------------------------	*/
	/*	a c c o r d s _ d a s h b o a r d	*/
	/*	---------------------------------	*/
	function functions()
	{
		print "<tr><th class=category><a href=\"dashboard.php?action=start\"><img width='128' height='158' src='imgs/start.png'></a></th>\n";
		print "<th class=description>Use this option to access the service management functions";
		print "<th class=category><a href=\"dashboard.php?action=check\"><img width='128' height='158' src='imgs/check.png'></a></th>\n";
		print "<th class=description>Use this option to verify the integrity of the complete collection of Accords Platform category managers.";
		print "<th class=category><a href=\"dashboard.php?action=stop\"><img width='128' height='158' src='imgs/stop.png'></a></th>\n";
		print "<th class=description>Use this option to access the service management functions";
		$this->separator();
		print "<tr><th class=category><a href=\"dashboard.php?action=parser\"><img width='128' height='158' src='imgs/parser.png'></a></th>\n";
		print "<th class=description>Use this option to select a manifest document for upload or create a service level agreement. <p>The resulting documents will then be parsed into the Accords Platform OCCI database";
		print "<th class=category><a href=\"dashboard.php?action=broker\"><img width='128' height='158' src='broker.png'></a></th>\n";
		print "<th class=description>Use this option to select a provisioning plan, resulting from the parsing of an uploaded manifest or service level agreement. <p>The provisioning plan will then be used to broker a service instance and deploy the corresponding cloud resources.";
		print "<th class=category><a href=\"dashboard.php?action=log\"><img width='128' height='158' src='inspect.png'></a></th>\n";
		print "<th class=description>Use this option to inspect the accords platform log file.";
	}

	/*	---------------------------------	*/
	/*	     d i s p l a y _ p a g e 		*/
	/*	---------------------------------	*/
	function display_page()
	{
		require("dashboard.inc");
		switch ( $this->tabpage )
		{
		case	"1"	:
			$this->comment("This page gives access to the Accords Platform Parsing and Brokering Tools");
			$this->separator();
			$this->functions();
			break;
		case	"2"	:
			$this->comment("This page gives access to the Accords Platform Service and Contract Provisioning Management Categories");
			$this->separator();
			print "<tr>";
			$this->cords_statistic("manifest",$manifest);
			$this->cords_statistic("node",$node);
			$this->cords_statistic("action",$action);
			print "</tr>";
			$this->separator();
			print "<tr>";
			$this->cords_statistic("service",$service);
			$this->cords_statistic("contract",$contract);
			$this->cords_statistic("instruction",$instruction);
			print "</tr>";
			break;
		case	"3"	:
			$this->comment("This page gives access to the Accords Platform Service Level Agreement Management Categories");
			$this->separator();
			print "<tr>";
			$this->cords_statistic("agreement",$agreement);
			$this->cords_statistic("penalty",$penalty);
			$this->cords_statistic("reward",$reward);
			print "</tr>";
			$this->separator();
			print "<tr>";
			$this->cords_statistic("provider",$provider);
			$this->cords_statistic("quota",$quota);
			$this->cords_statistic("placement",$placement);
			print "</tr>";
			break;
		case	"4"	:
			$this->comment("This page gives access to the Accords Platform Registered Provisioning Interface Categories");
			$this->separator();
			print "<tr>";
			$this->statistic("openstack",$openstack);
			$this->statistic("opennebula",$opennebula);
			$this->statistic("windowsazure",$windowsazure);
			print "</tr>";
			$this->separator();
			print "<tr>";
			$this->statistic("amazon",$amazon);
			$this->statistic("deltacloud",$deltacloud);
			$this->statistic("proactive",$proactive);
			print "</tr>";
			$this->separator();
			print "<tr>";
			$this->statistic("cloudfoundry",$cloudfoundry);
			$this->statistic("jpaas",$jpaas);
			print "</tr>";
			break;
		case	"5"	:
			$this->comment("This page gives access to the Accords Platform SLA Control and Monitoring Categories");
			$this->separator();
			print "<tr>";	
			$this->cords_statistic("event",$event);
			$this->cords_statistic("alert",$alert);
			$this->cords_statistic("probe",$probe);
			print "</tr>";
			$this->separator();
			print "<tr>";
			$this->cords_statistic("session",$session);
			$this->cords_statistic("monitor",$monitor);
			$this->cords_statistic("control",$control);
			print "</tr>";
			break;
		case	"6"	:
			$this->comment("This page gives access to the Accords Platform Invoice and Transaction Management Categories");
			$this->separator();
			print "<tr>";
			$this->cords_statistic("account",$account);
			$this->cords_statistic("user",$user);
			$this->cords_statistic("authorization",$authorization);
			print "</tr>";
			$this->separator();
			print "<tr>";
			$this->cords_statistic("price",$price);
			$this->cords_statistic("transaction",$transaction);
			$this->cords_statistic("invoice",$invoice);
			print "</tr>";
			break;
		case	"7"	:
			$this->comment("This page gives access to the Accords Platform Parsing Operations");
			$this->separator();
			print "<tr><th class=category><a href=\"dashboard.php?action=parser\"><img width='128' height='158' src='parser.png'></a></th>\n";
			print "<th class=description>Use this option to select a local manifest document for upload and parsing into the Accords Platform OCCI database";
			print "<th colspan=4>\n";
			require "upload.php";
			print "<tr><th class=category><a href=\"dashboard.php?action=manifest\"><img width='128' height='158' src='agreement.png'></a></th>\n";
			print "<th class=description>Use this option to create a service level agreement for upload and parsing into the Accords Platform OCCI database";
			print "<th colspan=4>\n";
			require "sla.php";
			$this->separator();
			break;
		case	"8"	:
			$this->comment("This page gives access to the Accords Platform Brokering and Invoice Operations");
			$this->separator();
			print "<tr><th class=category><a href=\"dashboard.php?action=broker\"><img width='128' height='158' src='broker.png'></a></th>\n";
			print "<th class=description>Use this option to broker and launch a service instance from a selected provisioning plan.";
			print "<th colspan=4>";
			print "<form action='dashboard.php' method='POST'>\n";
			print "<div class=whiteframe align=center>\n";
			print "<table width='100%'>";
			print "<tr><td>Plan<td><select style='width: 50mm;' name='upload'>\n";
			$a = array();
			exec("ls $configdir/plan_sla-*.xml",&$a);
			foreach ($a as $l )
			{
				print "<option value='".$l."'>".$l."</option>\n";
			}
			print "</select>\n";
			print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='create service'></div></tr>\n";
			print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='inspect plan'></div></tr>\n";
			print "</table></div>\n";
			print "</form></tr>";
			print "</tr>\n";

			$this->separator();
			print "<tr><th class=category><a href=\"dashboard.php?action=broker\"><img width='128' height='158' src='invoice.png'></a></th>\n";
			print "<th class=description>Use this option to collect the transactions for a specifed account and generate the invoice.";
			print "<p>This option also permits the selection of a particular invoice for consultation, update processing or closure.";
			print "<th colspan=4>\n";
			print "<form action='dashboard.php' method='POST'>\n";
			print "<div class=whiteframe align=center>\n";
			print "<table width='100%'>";
			print "<tr><td>Account<td><select style='width: 50mm;' name='account'>\n";
			$a = array();
			exec("grep name $configdir/cords_account.xml | cut -f 4 -d = | cut -f 2 -d '\"' ",&$a);
			foreach ($a as $l )
			{
				print "<option value='".$l."'>".$l."</option>\n";
			}
			print "</select>\n";
			print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='create invoice'></div></tr>\n";
			print "</table></div>\n";
			print "</form><p>\n";

			print "<form action='dashboard.php' method='POST'>\n";
			print "<div class=whiteframe align=center>\n";
			print "<table width='100%'>";
			print "<tr><td>Invoice<td><select style='width: 50mm;' name='invoice'>\n";
			$a = array();
			exec("grep document $configdir/cords_invoice.xml | cut -f 12 -d = | cut -f 2 -d '\"' ",&$a);
			foreach ($a as $l )
			{
				print "<option value='".$l."'>".$l."</option>\n";
			}
			print "</select>\n";
			print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='inspect invoice'></div></tr>\n";	
			print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='process invoice'></div></tr>\n";
			print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='close invoice'></div></tr>\n";
			print "</table></div></tr>\n";
			print "</form>\n";
			$this->separator();
			break;
		}

	}
	/*	--------------------------------	*/
	/*	       c l o s e 			*/
	/*	--------------------------------	*/
	public	function close()
	{
		/* ------------------- */
		/* Page Footer Section */
		/* ------------------- */
		$this->separator();
		print "<tr><th class='title' colspan=6><div class=small>".$this->copyright[0]."</div>\n";
	
		print "<div class=tiny>".$this->copyright[1]."</div>\n";
		print "</th></tr>\n";
		print "</table></div></table><p></div></div><p>\n";
		cedexis_radar("01-11403");
		print "</body></html>\n";
	}
	
};

?>
