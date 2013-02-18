<?php
function cedexis_radar($account)
{
print "<script type=\"text/javascript\">\n";
print "(function(w, d) { var a = function() { var a = d.createElement('script'); a.type = 'text/javascript';\n";
print "a.async = 'async'; a.src = '//' + ((w.location.protocol === 'https:') ? 's3.amazonaws.com/cdx-radar/' :\n";
print "'radar.cedexis.com/') + '".$account."-radar10.min.js'; d.body.appendChild(a); };\n";
print "if (w.addEventListener) { w.addEventListener('load', a, false); }\n";
print "else if (w.attachEvent) { w.attachEvent('onload', a); }\n";
print "}(window, document));\n";
print "</script>\n";
}
?>
