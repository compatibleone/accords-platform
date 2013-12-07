<?php

function AccountSelector( $sname )
{
	print "<select style='width: 50mm;' name=".$sname.">\n";
	$a = array();
	$r = exec("bash ./dashboard-command run csp-option-list.txt account",$a);
	foreach ($a as $l )
	{
		print $l."\n";
	}
	print "</select>";
}

function ManifestSelector( $sname )
{
	print "<select style='width: 50mm;' name=".$sname.">\n";
	$a = array();
	$r = exec("bash ./dashboard-command run csp-option-list.txt manifest",$a);
	foreach ($a as $l )
	{
		print $l."\n";
	}
	print "</select>";
}

function ProviderSelector( $sname )
{
	print "<select style='width: 50mm;' name=".$sname.">\n";
	$a = array();
	$r = exec("bash ./dashboard-command run csp-option-list.txt provider",$a);
	foreach ($a as $l )
	{
		print $l."\n";
	}
	print "</select>";
}

function MetricSelector( $sname, $holder )
{
	print "<select style='width: 50mm;' name=".$sname.">\n";
	if ( $holder == "" )
	{
		$a = array();
		$r = exec("bash ./dashboard-command run csp-option-list.txt metric",$a);
		$holder = $a;
	}
	else
	{
		$a = $holder;
	}
	foreach ($a as $l )
	{
		print $l."\n";
	}
	print "</select>";
	return( $holder );
}

?>
