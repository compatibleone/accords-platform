<?php

class admin_role extends role
{
	public	$status=NULL;
	public	$platform=NULL;
	public	$invoice=NULL;
	public	$service=NULL;

	public	function admin_role($p)
	{
		$this->status = "";
		$this->page = $p;
		$this->platform = new platform_operations;
		$this->invoice = new invoice_operations;
		$this->service = new service_operations;
	}
	private function services()
	{
	$this->page->separator();
	print "<tr><th class=category><a href=\"dashboard.php?action=stop\"><img width='128' height='158' src='stop.png'></a></th>\n";
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
	print "<tr><th class=category><a href=\"dashboard.php?action=start\"><img width='128' height='158' src='start.png'></a></th>\n";
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
	print "<tr><th class=category><a href=\"dashboard.php?action=save\"><img width='128' height='158' src='check2.png'></a></th>\n";
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
	print "</form>\n";
	$this->page->separator();
	}

	/*	---------------------------------	*/
	/*	     d i s p l a y _ t a b s 		*/
	/*	---------------------------------	*/
	function display_tabs( $t )
	{
		$this->page->tabpage = $t;
		$tabimage1 = "tabdashboard.png";
		$tabimage2 = "tabprovisioning.png";
		$tabimage3 = "tabsla.png";
		$tabimage4 = "tabproviders.png";
		$tabimage5 = "tabmonitoring.png";
		$tabimage6 = "tabfinancial.png";
		switch ( $this->page->tabpage )
		{
		case	1 :
			$tabimage1 = "actdashboard.png";
			break;
		case	2 :
			$tabimage2 = "actprovisioning.png";
			break;
		case	3 :
			$tabimage3 = "actsla.png";
			break;
		case	4 :
			$tabimage4 = "actproviders.png";
			break;	
		case	5 :
			$tabimage5 = "actmonitoring.png";
			break;
		case	6 :
			$tabimage6 = "actfinancial.png";
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
		print "<th class='button'><a href='dashboard.php?action=page6'><img src='".$tabimage6."'></a></th>\n";	
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

			/* ------------------ */
			/* service operations */
			/* ------------------ */
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
			case	"stop service" :
				$this->display_tabs("0");
				$result = $this->service->stop($this->page);
				break;
			case	"delete service" :
				$this->display_tabs("0");
				$result = $this->service->delete($this->page);
				break;

			case	"inspect plan" :
				$this->display_tabs("0");
				$result = $this->service->inspect_plan($this->page);
				break;
			/* ------------------- */
			/* platform operations */
			/* ------------------- */
			case	"start platform" :
				$this->display_tabs("0");
				$result = $this->platform->start($this->page);
				break;

			case	"stop platform" :
				$this->display_tabs("0");
				$result = $this->platform->stop($this->page);	
				break;

			case	"check" :
				$this->display_tabs("0");
				$result = $this->platform->check($this->page);
				break;

			case	"log" :
				$this->display_tabs("0");
				$result = $this->platform->log($this->page);
				break;
			case	"start" :
				$this->display_tabs("0");
				$this->page->tabpage="20";
				break;
			case	"stop" :
				$this->display_tabs("0");
				$this->page->tabpage="20";
				break;

			case	"upload and parse" :
				$this->display_tabs("0");
				$result = upload_manifest($this->page->tabpage);
				break;
			case	"generate sla then parse" :
				$this->display_tabs("0");
				$result = generate_sla($this->page->tabpage);
				break;

			case	"create service" :
				$this->display_tabs( $this->page->tabpage );
				$result = $this->service->create($this->page);
				break;

			case	"create account" :
				$this->display_tabs( $this->page->tabpage );
				$result = create_account();
				break;
			case	"create user" :
				$this->display_tabs( $this->page->tabpage );
				$result = create_user();
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
			case	"parser" :
				$this->display_tabs( $this->page->tabpage );	
				$this->page->tabpage = 7;
				break;
			case	"broker" :
				$this->display_tabs( $this->page->tabpage );
				$this->page->tabpage = 8;
				break;
			case	"manifest" :
				$this->display_tabs( $this->page->tabpage );
				$this->page->tabpage = 9;
				break;
			case	"user login" :
				$this->page->tabpage="1";
				$this->display_tabs( $this->page->tabpage );
				break;
			case	"page1"	:
				$this->page->tabpage="1";
				$this->display_tabs( $this->page->tabpage );
				break;
			case	"page2"	:
				$this->page->tabpage="2";
				$this->page->periodicReload();
				$this->display_tabs( $this->page->tabpage );
				break;
			case	"page3"	:
				$this->page->tabpage="3";
				$this->page->periodicReload();
				$this->display_tabs( $this->page->tabpage );
				break;
			case	"page4"	:
				$this->page->tabpage="4";
				$this->page->periodicReload();
				$this->display_tabs( $this->page->tabpage );
				break;
			case	"page5"	:
				$this->page->tabpage="5";
				$this->page->periodicReload();
				$this->display_tabs( $this->page->tabpage );
				break;
			case	"page6"	:
				$this->page->tabpage="6";
				$this->page->periodicReload();	
				$this->display_tabs( $this->page->tabpage );
				break;
			}
		}
		else
		{
			$this->page->tabpage = "1";
			$this->display_tabs( $this->page->tabpage );
			$result = "";
		}
		return( $this->page->tabpage  );

	}

	/*	---------------------------------	*/
	/*	       p a g e _ d i s p l a y 		*/
	/*	---------------------------------	*/
	public	function display()
	{
		$result = "";

		/* ---------------------- */
		/* Page Accords Dashboard */
		/* ---------------------- */
		switch ( $this->page->tabpage )
		{
		case	"20"	:
			$this->services();
			break;
		default		:
			$this->page->display_page();
		}
	}

};

?>
