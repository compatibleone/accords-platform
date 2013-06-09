<?php

class user_role extends role
{
	public	$status=NULL;
	public	$invoice=NULL;
	public	$service=NULL;

	public	function user_role($p)
	{
		$this->status = "";
		$this->page = $p;
		$this->invoice = new invoice_operations;
		$this->service = new service_operations;
	}

	/*	---------------------------------	*/
	/*	     d i s p l a y _ t a b s 		*/
	/*	---------------------------------	*/
	function display_tabs( $t )
	{
		$this->page->tabpage = $t;

		$tabimage1 = "images/tabsla.png";
		$tabimage2 = "images/tabprovisioning.png";
		$tabimage3 = "images/tabfinancial.png";
		$tabimage4 = "images/tabscript.png";
		$tabimage5 = "images/tabmenu.png";

		switch ( $this->page->tabpage )
		{
		case	5 :
			$tabimage5 = "images/actmenu.png";
			break;
		case	4 :
			$tabimage4 = "images/actscript.png";
			break;
		case	3 :
			$tabimage3 = "images/actfinancial.png";
			break;
		case	2 :
			$tabimage2 = "images/actprovisioning.png";
			break;
		case	1 :
			$tabimage1 = "images/actsla.png";
			break;
		}

		/* ------------------- */
		/* Page Header Section */
		/* ------------------- */
		print "<input type=hidden value='".$this->page->tabpage."' readonly>\n";
		print "<tr><th colspan=6><table border=0><tr>\n";
		print "<th class='button'><a href='dashboard.php?action=page1'><img src='".$tabimage1."'></a></th>\n";
		print "<th class='button'><a href='dashboard.php?action=page2'><img src='".$tabimage2."'></a></th>\n";
		print "<th class='button'><a href='dashboard.php?action=page3'><img src='".$tabimage3."'></a></th>\n";
		print "<th class='button'><a href='dashboard.php?action=page4'><img src='".$tabimage4."'></a></th>\n";
		print "<th class='button'><a href='dashboard.php?action=page5'><img src='".$tabimage5."'></a></th>\n";
		print "</tr></table></th>\n";
	}

	public	function action()
	{

		/* ---------------------- */
		/* Page Action Processing */
		/* ---------------------- */
		$result = "";
		$this->page->tabpage=0;
		require( "dashboard.inc" );

		if ( $_REQUEST['action'] != '' )
		{
			switch ( $_REQUEST['action'] )
			{

			case	"page1"	:
				$this->page->tabpage="1";
				$this->display_tabs( $this->page->tabpage );
				break;
			case	"page2"	:
				$this->page->tabpage="2";
				$this->display_tabs( $this->page->tabpage );
				break;
			case	"page3"	:
				$this->page->tabpage="3";
				$this->display_tabs( $this->page->tabpage );
				break;
			case	"page4"	:
				$this->page->tabpage="4";
				$this->display_tabs( $this->page->tabpage );
				break;
			case	"page5"	:
				$this->page->tabpage="5";
				$this->display_tabs( $this->page->tabpage );
				$this->main_menu();
				break;


			/* script launched operations */
			/* -------------------------- */


			case	"delete log"		:
				$this->page->tabpage="2";
				$this->display_tabs( $this->page->tabpage );
				$this->page->delete_log( $this->page );
				break;
			case	"analyse log"		:
				$this->page->tabpage="2";
				$this->display_tabs( $this->page->tabpage );
				$this->page->analyse_log( $this->page );
				break;

			case	"create provider"	:

				$args = $this->page->user;
				compile_cordscript( "csp-createprovider.txt", $this->page,"HTML", $args );
				break;

			case	"create user account"	:

				$args = $this->page->user;
				$args = $args." name=".$_REQUEST['name']." type=".$_REQUEST['type']." number=".$_REQUEST['number']." script=".$_REQUEST['script'];
				$args = $args." user=".$_REQUEST['username']." pass=".$_REQUEST['password']." mail=".$_REQUEST['mail'];
				$args = $args." os=".$_REQUEST['openstack']." on=".$_REQUEST['opennebula']." az=".$_REQUEST['windowsazure'];
				compile_cordscript( "csp-createaccount.txt", $this->page, "HTML", $args );
				break;

			case	"add account user"		:
				$args = $this->page->user;
				$args = $args." name=".$_REQUEST['name'];
				$args = $args." user=".$_REQUEST['username'];
				$args = $args." pass=".$_REQUEST['password'];
				$args = $args." mail=".$_REQUEST['mail'];
				$args = $args." role=".$_REQUEST['role'];
				compile_cordscript( "csp-adduser.txt", $this->page,"HTML", $args );
				break;

			case	"add federation"		:
				$args = $this->page->user;
				$args = $args." name=".$_REQUEST['name'];
				$args = $args." user=".$_REQUEST['user'];
				$args = $args." password=".$_REQUEST['password'];
				$args = $args." host=".$_REQUEST['host'];
				$args = $args." security=".$_REQUEST['security'];
				compile_cordscript( "csp-updfederation.txt", $this->page,"HTML", $args );
				break;
			case	"update user"		:
				$args = $this->page->user;
				$args = $args." id=".$_REQUEST['id'];
				$args = $args." user=".$_REQUEST['username'];
				$args = $args." pass=".$_REQUEST['password'];
				$args = $args." mail=".$_REQUEST['mail'];
				$args = $args." role=".$_REQUEST['role'];
				compile_cordscript( "csp-upduser.txt", $this->page,"HTML", $args );
				break;

			case	"create sla"		:

				$args = $this->page->user;
				$args = $args." name=".$_REQUEST['name'];
				$args = $args." desc='".$_REQUEST['desc']."'";
				$args = $args." init=".$_REQUEST['init'];
				$args = $args." resp=".$_REQUEST['resp'];
				$args = $args." from=".$_REQUEST['from'];
				$args = $args." expire=".$_REQUEST['expire'];
				$args = $args." template=".$_REQUEST['template'];
				$args = $args." manifest=".$_REQUEST['manifest'];
				compile_cordscript( "csp-createsla.txt", $this->page,"HTML", $args );
				break;

			case	"add os subscription"	:
				$args = $this->page->user;
				$args = $args." name=".$_REQUEST['name'];
				$args = $args." host=".$_REQUEST['host'];
				$args = $args." tenent=".$_REQUEST['tenent'];
				$args = $args." user=".$_REQUEST['user'];
				$args = $args." password=".$_REQUEST['password'];
				compile_cordscript( "csp-updos.txt", $this->page,"HTML", $args );
				break;
			case	"add ez subscription"	:
				$args = $this->page->user;
				$args = $args." name=".$_REQUEST['name'];
				$args = $args." host=".$_REQUEST['host'];
				$args = $args." tenent=".$_REQUEST['tenent'];
				$args = $args." user=".$_REQUEST['user'];
				$args = $args." password=".$_REQUEST['password'];
				$args = $args." iaas=".$_REQUEST['iaas'];
				compile_cordscript( "csp-updez.txt", $this->page,"HTML", $args );
				break;
			case	"add on subscription"	:
				$args = $this->page->user;
				$args = $args." name=".$_REQUEST['name'];
				$args = $args." host=".$_REQUEST['host'];
				$args = $args." tenent=".$_REQUEST['tenent'];
				$args = $args." user=".$_REQUEST['user'];
				$args = $args." password=".$_REQUEST['password'];
				compile_cordscript( "csp-updon.txt", $this->page,"HTML", $args );
				break;
			case	"add az subscription"	:
				$args = $this->page->user;
				$args = $args." name=".$_REQUEST['name'];
				$args = $args." host=".$_REQUEST['host'];
				$args = $args." tenent=".$_REQUEST['tenent'];
				$args = $args." user=".$_REQUEST['user'];
				$args = $args." password=".$_REQUEST['password'];
				compile_cordscript( "csp-updaz.txt", $this->page,"HTML", $args );
				break;
			case	"add pp subscription"	:
				$args = $this->page->user;
				$args = $args." name=".$_REQUEST['name'];
				$args = $args." host=".$_REQUEST['host'];
				$args = $args." tenent=".$_REQUEST['tenent'];
				$args = $args." user=".$_REQUEST['user'];
				$args = $args." password=".$_REQUEST['password'];
				compile_cordscript( "csp-updpp.txt", $this->page,"HTML", $args );
				break;
			case	"add pa subscription"	:
				$args = $this->page->user;
				$args = $args." name=".$_REQUEST['name'];
				$args = $args." host=".$_REQUEST['host'];
				$args = $args." tenent=".$_REQUEST['tenent'];
				$args = $args." user=".$_REQUEST['user'];
				$args = $args." password=".$_REQUEST['password'];
				compile_cordscript( "csp-updpa.txt", $this->page,"HTML", $args );
				break;
			case	"add ec subscription"	:
				$args = $this->page->user;
				$args = $args." name=".$_REQUEST['name'];
				$args = $args." host=".$_REQUEST['host'];
				$args = $args." tenent=".$_REQUEST['tenent'];
				$args = $args." user=".$_REQUEST['user'];
				$args = $args." password=".$_REQUEST['password'];
				compile_cordscript( "csp-updec.txt", $this->page,"HTML", $args );
				break;
			case	"add dc subscription"	:
				$args = $this->page->user;
				$args = $args." name=".$_REQUEST['name'];
				$args = $args." host=".$_REQUEST['host'];
				$args = $args." tenent=".$_REQUEST['tenent'];
				$args = $args." user=".$_REQUEST['user'];
				$args = $args." password=".$_REQUEST['password'];
				compile_cordscript( "csp-upddc.txt", $this->page,"HTML", $args );
				break;
			case	"add cn subscription"	:
				$args = $this->page->user;
				$args = $args." name=".$_REQUEST['name'];
				$args = $args." host=".$_REQUEST['host'];
				$args = $args." tenent=".$_REQUEST['tenent'];
				$args = $args." key=".$_REQUEST['key'];
				$args = $args." secret=".$_REQUEST['secret'];
				compile_cordscript( "csp-updcn.txt", $this->page,"HTML", $args );
				break;

			case	"add condition"		:

				$args = $this->page->user;
				$args = $args." id=".$_REQUEST['id'];
				$args = $args." algorithm=".$_REQUEST['algo'];
				$args = $args." security=".$_REQUEST['security'];
				$args = $args." energy=".$_REQUEST['energy'];
				$args = $args." provider=".$_REQUEST['provider'];
				$args = $args." zone=".$_REQUEST['zone'];
				$args = $args." rating=".$_REQUEST['rating'];
				compile_cordscript( "csp-addcondition.txt", $this->page,"HTML", $args );
				break;

			case	"add application"		:
				print "add application<br>\n";
				$args = $this->page->user;
				$args = $args." vendor=".$_REQUEST['vendor'];
				$args = $args." appname=".$_REQUEST['appname'];
				$args = $args." desc='".$_REQUEST['desc']."'";
				$args = $args." manifest=".$_REQUEST['manifest'];
				$args = $args." sla=".$_REQUEST['sla'];
				print "args: ".args."<br>\n";
				compile_cordscript( "csp-addapplication.txt", $this->page,"HTML", $args );
				break;

			case	"add guarantee"		:

				$args = $this->page->user;
				$args = $args." id=".$_REQUEST['id'];
				$args = $args." gm=".$_REQUEST["gm"];
				$args = $args." gc=".$_REQUEST["gc"];
				$args = $args." gv=".$_REQUEST["gv"];
				$args = $args." ob=".$_REQUEST["oblig"];
				$args = $args." gbv=".$_REQUEST["gbv"];
				$args = $args." gbx='".$_REQUEST["gbx"]."'";
				compile_cordscript( "csp-addguarantee.txt", $this->page,"HTML", $args );
				break;



			/* ------------------ */
			/* parsing operations */
			/* ------------------ */
			case	"upload and parse" :
				$this->display_tabs("0");
				$result = upload_manifest($this->page);
				break;
			/* ------------------ */
			/* parsing operations */
			/* ------------------ */
			case	"upload and compile" :
				$this->display_tabs("0");
				$result = upload_cordscript($this->page);
				break;
			case	"inspect script"	:
				$this->display_tabs("0");
				$result = inspect_script($this->page);
				break;
			case	"compile" :
				$this->display_tabs("5");
				$result = compile_and_run($this->page);
				break;
			case	"generate sla then parse" :
				$this->display_tabs("0");
				$result = generate_sla($this->page);
				break;

			/* ------------------ */
			/* service operations */
			/* ------------------ */
			case	"inspect plan" :
				$this->display_tabs("0");
				$result = $this->service->inspect_plan($this->page);
				break;
			case	"createservice"	:
			case	"create service" :
				$this->display_tabs( $this->page->tabpage );
				$result = $this->service->create($this->page);
				break;
			case	"startservice"	:
			case	"start service" :
				$this->display_tabs("0");
				$result = $this->service->start($this->page);
				break;
			case	"inspect service" :
				$this->display_tabs("0");
				$result = $this->page->file_inspect($_REQUEST['service'],$this->page);
				break;
			case	"restart service" :
				$this->display_tabs("0");
				$result = $this->service->restart($this->page);
				break;
			case	"save service" :
				$this->display_tabs("0");
				$result = $this->service->save($this->page);
				break;
			case	"snapshot service" :
				$this->display_tabs("0");
				$result = $this->service->snapshot($this->page);
				break;
			case	"stopservice"	:
			case	"stop service" :
				$this->display_tabs("0");
				$result = $this->service->stop($this->page);
				break;
			case	"delete service" :
				$this->display_tabs("0");
				$result = $this->service->delete($this->page);
				break;

			/* ------------------ */
			/* invoice operations */
			/* ------------------ */
			case	"create invoice" :
				$this->display_tabs( $this->page->tabpage );
				$result = $this->invoice->create($this->page);
				break;
			case	"inspect invoice" :
				$this->display_tabs( $this->page->tabpage );
				$result = $this->invoice->inspect($this->page);
				break;

			default	:
				$this->page->tabpage="5";
				$this->display_tabs( $this->page->tabpage );
				$this->main_menu();
				break;
			}
		}
	}

	public	function main_menu()
	{
		$_REQUEST['output']="HTML";
		$_REQUEST['script']="csp-welcome.txt";
		$result = compile_and_run($this->page);
	}

	public	function service_menu()
	{
		$_REQUEST['output']="HTML";
		$_REQUEST['script']="csp-services.txt";
		$result = compile_and_run($this->page);
	}

	public	function sla_list( $mode )
	{
		if ( $this->page->account != "" )
		{
			$a = array();
			exec("grep manifest -m1 plan_sla-*.xml | cut -f 1 -d :",&$a);
			foreach ($a as $l )
			{
				if ( $mode == "all" )
				{
					print "<option value='".$l."'>".$l."</option>\n";
				}
				else
				{
					$b = array();
					exec("grep ".$this->page->account." ".$l,&$b);
					foreach ($b as $m )
					{
						print "<option value='".$l."'>".$l."</option>\n";
					}
				}
			}
		}
	}

	public	function display()
	{
		switch ( $this->page->tabpage )
		{
		case	"4"	:
			$this->page->comment("This page gives access to the Accords Platform Script Upload and Execution Operations");
			print "<tr><th class=category><a href=\"dashboard.php?action=parser\"><img width='128' height='158' src='images/parser.png'></a></th>\n";
			print "<th class=description>Use this option to select a local cordscript document for upload, compile and execution";
			print "<th colspan=4>\n";
			require "compile.php";

			break;
		case	"1"	:
			$this->page->comment("This page gives access to the Accords Platform Manifest and Service Level Agreement Parsing Operations");
			$this->page->separator();
			print "<tr><th class=category><a href=\"dashboard.php?action=parser\"><img width='128' height='158' src='images/parser.png'></a></th>\n";
			print "<th class=description>Use this option to select a local manifest document for upload and parsing into the Accords Platform OCCI database";
			print "<th colspan=4>\n";
			require "upload.php";
			print "<tr><th class=category><a href=\"dashboard.php?action=manifest\"><img width='128' height='158' src='images/agreement.png'></a></th>\n";
			print "<th class=description>Use this option to create a service level agreement for upload and parsing into the Accords Platform OCCI database";
			print "<th colspan=4>\n";
			require "sla.php";
			$this->page->separator();
			break;
		case	"2"	:
			$this->page->comment("This page gives access to Accords Platform Service Management Operations");
			$this->page->separator();
			print "<tr><th class=category><a href=\"dashboard.php?action=broker\"><img width='128' height='158' src='images/broker.png'></a></th>\n";
			print "<th class=description>Use this option to broker and launch a service instance from a selected provisioning plan.";
			print "<th colspan=4>";
			print "<form action='dashboard.php' method='POST'>\n";
			print "<div class=whiteframe align=center>\n";
			print "<table width='100%'>";
			print "<tr><td>Plan<td><select style='width: 50mm;' name='upload'>\n";
			$this->sla_list("all");
			print "</select>\n";

			print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='create service'></div></tr>\n";
			print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='inspect plan'></div></tr>\n";
			print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='analyse log'></div></tr>\n";
			print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='delete log'></div></tr>\n";
			print "</table></div>\n";
			print "</form></tr>";
			print "</tr>\n";

			$this->page->separator();
	print "<tr><th class=category><a href=\"dashboard.php?action=stop\"><img width='128' height='158' src='images/stop.png'></a></th>\n";
	print "<th class=description>Use this option to terminate an active service instance and release all deployed resources. The service may be restarted using the appropriate service maintenance operation.";
	print "<th colspan=4>";
	print "<form action='dashboard.php' method='POST'>\n";
	print "<p><div class=whiteframe align=center>\n";
	print "<table width='100%'>\n";
	print "<tr><td>Service<td><select style='width: 50mm;' name='service'>\n";
	$a = array();
	exec("ls -t service/*",&$a);
	foreach ($a as $l )
	{
		print "<option value='".$l."'>".$l."</option>\n";
	}
	print "</select>\n";
	print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='inspect service'></div></tr>\n";
	print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='stop service'></div></tr>\n";
	print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='delete service'></div></tr>\n";
	print "</table></div>\n";
	print "</form></tr>";
			$this->page->separator();
	print "<tr><th class=category><a href=\"dashboard.php?action=start\"><img width='128' height='158' src='images/start.png'></a></th>\n";
	print "<th class=description>Use this option to restart a terminated service instance and deploy all provisioned resources.";
	print "<th colspan=4>\n";
	print "<form action='dashboard.php' method='POST'>\n";
	print "<p><div class=whiteframe align=center>\n";
	print "<table width='100%'>\n";
	print "<tr><td>Service<td><select style='width: 50mm;' name='service'>\n";
	$a = array();
	exec("ls -t service/*",&$a);
	foreach ($a as $l )
	{
		print "<option value='".$l."'>".$l."</option>\n";
	}
	print "</select>\n";
	print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='start service'></div></tr>\n";
	print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='restart service'></div></tr>\n";
	print "</table></div></tr>\n";
	print "</form><p>\n";
			$this->page->separator();

	print "<tr><th class=category><a href=\"dashboard.php?action=save\"><img width='128' height='158' src='images/check2.png'></a></th>\n";
	print "<th class=description>Use this option to snapshot or save the state of the machine images of the contracts of a service.";
	print "<th colspan=4>\n";
	print "<form action='dashboard.php' method='POST'>\n";
	print "<p><div class=whiteframe align=center>\n";
	print "<table width='100%'>\n";
	print "<tr><td>Service<td><select style='width: 50mm;' name='service'>\n";
	$a = array();
	exec("ls -t service/*",&$a);
	foreach ($a as $l )
	{
		print "<option value='".$l."'>".$l."</option>\n";
	}
	print "</select>\n";
	print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='save service'></div></tr>\n";
	print "<tr><td>Action<td><div align=center><input class='action' type='submit' name='action' value='snapshot service'></div></tr>\n";
	print "</table></div></tr>\n";
			$this->page->separator();
			break;
			break;
		case	"3"	:
			$this->page->comment("This page gives access to the Accords Platform Account Management Operations");
			$this->page->separator();
			print "<tr><th class=category><a href=\"dashboard.php?action=broker\"><img width='128' height='158' src='images/invoice.png'></a></th>\n";
			print "<th class=description>Use this option to collect the transactions for a specifed account and generate the invoice.";
			print "<p>This option also permits the selection of a particular invoice for consultation, update processing or closure.";
			print "<th colspan=4>\n";
			print "<form action='dashboard.php' method='POST'>\n";
			print "<div class=whiteframe align=center>\n";
			print "<table width='100%'>";
			print "<tr><td>Account<td><select style='width: 50mm;' name='account'>\n";
			$a = array();
			exec("grep name cords_account.xml | cut -f 4 -d = | cut -f 2 -d '\"' ",&$a);
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
			exec("grep document cords_invoice.xml | cut -f 12 -d = | cut -f 2 -d '\"' ",&$a);
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
			$this->page->separator();

		}

	}

};

?>
