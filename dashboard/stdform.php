<?php

class	standard_form
{
	public	$header=NULL;
	public	$target=NULL;
	public	$width=NULL;

	public function standard_form( $header, $width, $target )
	{
		$this->header = $header;
		$this->target = $target;
		$this->width = $width;
	}

	public	function	open()
	{
		print "<form method=\"POST\" action=\"".$this->target."\">\n";
		print "<div align=center><table width='".$this->width."'><tr><th>\n";
		print "<p><div class=whiteframe align=center>\n";
		print "<table width='100%'>\n";
		print "<tr><td colspan=4><div align=left><b>".$this->header."</b></div></th></tr>\n";
	}

	public	function	input( $l, $n, $v, $w )
	{
		print "<tr><td>".$l."</td>\n";
			print "<td><input type=text name=".$n." value=\"".$v."\" size=".$w."></td>\n";
			print "</tr>\n";
	}

	public	function	action( $l, $n )
	{
		print "<tr><td>".$l."<td><div align=left><input class='action' type=submit name=action value='".$n."'></div>\n";
	}

	public	function	password( $l, $n, $w )
	{
		print "<tr><td>".$l."</td>\n";
			print "<td><input type=password name=".$n." size=".$w."></td>\n";
			print "</tr>\n";
	}

	public function close()
	{
		print "</table></div>\n";
		print "</form>\n";
		print "</table></div>\n";
	}
};

?>
