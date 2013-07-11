<?php

class	invoice_operations
{
	public	$status=NULL;

/*	---------------------------------	*/
/*	   i n v o i c e _ o u t p u t		*/
/*	---------------------------------	*/
public function invoice_output( $t, $a, $q, $p )
{
	$p->separator();
	print "<tr><th colspan=6>\n";
	print "<table border=1 width='100%' style='overflow:scroll'>\n";
	print "<tr><th><div align=center class='title'>".$t." for ".$q."</div>\n";
	print "<tr><td style='overflow:scroll'>\n";
	foreach ($a as $l )
	{
		$m = explode( ":", $l );
		if ( $m[0] == 'invoice' )
		{
			$f = file( ltrim($m[1]) );
			foreach ($f as $n )
			{
				print $n;
			}
		}
	}
	print "</table>\n";
	print "</th></tr>\n";
	$p->separator();
}

/*	---------------------------------	*/
/*	   i n v o i c e _ v i e w 		*/
/*	---------------------------------	*/
public function invoice_view( $i, $p )
{
	$p->separator();
	print "<tr><th colspan=6>\n";
	print "<table border=1 width='100%' style='overflow:scroll'>\n";
	print "<tr><th><div align=center class='title'>Invoice: ".$i."</div>\n";
	print "<tr><td style='overflow:scroll'>\n";
	$f = file( ltrim( $i ) );
	foreach ($f as $n )
	{
		print $n;
	}
	print "</table>\n";
	print "</th></tr>\n";
	$p->separator();
}

/*	---------------------------------	*/
/*	  i n s p e c t _ i n v o ic e 		*/
/*	---------------------------------	*/
public function inspect($p)
{
	if ( $_REQUEST['invoice'] != "" )
	{
		$this->invoice_view( $_REQUEST['invoice'], $p );
		return( "invoice" );
	}
	else	return( "" );
}

/*	---------------------------------	*/
/*	  p r o c e s s _ i n v o ic e 		*/
/*	---------------------------------	*/
public function process($p)
{
	if ( $_REQUEST['invoice'] != "" )
	{
		return( "process invoice" );
	}
	else	return( "" );
}

/*	---------------------------------	*/
/*	   c r e a t e _ i n v o ic e 		*/
/*	---------------------------------	*/
public function create($p)
{
	if ( $_REQUEST['account'] != "" )
	{
		$a = array();
		$result = exec("bash ./dashboard-invoice ".$_REQUEST['account'],$a);
		$this->invoice_output( "Invoice", $a, $_REQUEST['account'],$p);
		return( "invoice" );
	}
	else	return( "" );
}

/*	---------------------------------	*/
/*	    c l o s e _ i n v o ic e 		*/
/*	---------------------------------	*/
public function close($p)
{
	if ( $_REQUEST['invoice'] != "" )
	{
		return( "close_invoice" );
	}
	else	return( "" );
}
};

?>
