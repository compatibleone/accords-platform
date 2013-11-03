<?php

class	platform_operations
{
	public	$status;

	/*	---------------------------------	*/
	/*		l o g _ m e t h o d 		*/
	/*	---------------------------------	*/
	private function log_method( $f, $colour, $p )
	{
		print "<font color=".$colour.">[".$f[1]."] ".$f[2]." ".$f[3]." ";
		if ( substr( $f[4], 0, 4) == "http" )
		{
			$p->windowed_link( $f[4] );
			print $f[4]."</a>";
		}
		else
		{
			print $f[4];
		}
		print "</font><br>\n";
	}

	/*	---------------------------------	*/
	/*	   	l o g _ o u t p u t		*/
	/*	---------------------------------	*/
	private function log_output( $a, $p )
	{
		$p->separator();
		print "<tr><th colspan=6>\n";
		print "<table border=1 width='100%' style='overflow:scroll'><tr><th><div align=center class='title'>Accords Platform LOG Inspection</div>\n";
		print "<tr><td style='overflow:scroll'>\n";
		$p->periodicReload();
		foreach ($a as $l )
		{
			$r = str_replace("&","&amp;",$l);
			$l = str_replace(">","&gt;",$r);
			$r = str_replace("<","&lt;",$l);
			$f = explode( " ", $r );
			switch ( $f[3] )
			{
			case	"HTTP/1.0"	:
				print "<font color=orange>[".$f[1]."] ".$f[2]." ".$f[3]." ".$f[4]." ".$f[5]." ".$f[6]." ".$f[7]." ".$f[8]."</tr>\n";
				break;
			case	"HTTP/1.1"	:
				print "<font color=orange>[".$f[1]."] ".$f[2]." ".$f[3]." ".$f[4]." ".$f[5]." ".$f[6]."</font><br>\n";
				break;
			case	"GET"	:
				$this->log_method( $f, "green",$p );
				break;
			case	"POST"	:
				$this->log_method( $f, "blue",$p  );
				break;
			case	"PUT"	:
				$this->log_method( $f, "navy",$p  );
				break;
			case	"DELETE":
				$this->log_method( $f, "red",$p  );
				break;
			default	:
				print "<font color=grey>".$r."</font><br>\n";	
				break;
			}
		}
		print "</table>\n";
		print "</th></tr>\n";
		$p->separator();
	}

	/*	---------------------------------	*/
	/*	   s t a r t _ p l a t f o r m		*/
	/*	---------------------------------	*/
	public function start($p)
	{
		$a = array();
		exec("bash ./dashboard-start",$a);
		$p->command_output( "Start Platform",$a );
		return( "start" );
	}

	/*	----------------------------------	*/
	/* 		a n a l y s e _ l o g 		*/
	/*	----------------------------------	*/
	public function analyse_log($p)
	{
		$a = array();
		exec("bash ./dashboard-analyse",$a);
		$p->command_output( "Log Analysis",$a );
		return( "analysis" );
	}

	/*	---------------------------------	*/
	/*	   c h e c k _ p l a t f o r m		*/
	/*	---------------------------------	*/
	public function check($p)
	{
		$a = array();
		$result = exec("bash ./dashboard-check",$a);
		$p->command_output( "Check Platform",$a );
		return( "check" );
	}

	/*	---------------------------------	*/
	/*	   s t o p _ p l a t f o r m		*/
	/*	---------------------------------	*/
	public function stop($p)
	{
		$a = array();
		exec("bash ./dashboard-stop",$a);
		$p->command_output( "Stop Platform", $a );
		return( "stop" );
	}

	/*	---------------------------------	*/
	/*	 l o g _ p l a t f o r m _ l o g 	*/
	/*	---------------------------------	*/
	public function log($p)
	{
		$a = array();
		$result = exec("bash dashboard-log",$a);
		$this->log_output( $a, $p );	
		return( "log" );
	}
};

?>
