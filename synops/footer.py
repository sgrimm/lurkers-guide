#!/usr/local/bin/python

import sys,string
sys.path.append('../internal')
import otros

curep = string.atoi(sys.argv[1][:3])

output = """
<pre>

</pre>

<p nowrap>
<nobr><a href="/lurk/lurker.html"><img
	align=bottom border=0
	width=64 height=24 src="/lurk/nav/foot-station.gif"
	alt="[Home] "></a><a
href="#TOP"><img
	align=bottom border=0
	width=72 height=13 src="/lurk/nav/foot-pagetop.gif"
	alt="[Top] "></a><a
href="mailto:koreth@midwinter.com"><img
	align=bottom border=0
	width=80 height=13 src="/lurk/nav/foot-comments.gif"
	alt="[Comments] "></a><img
align=bottom
	width=184 height=13 src="/lurk/nav/foot-bare-short.gif"
	alt="">"""

output = output + otros.navbuttons(curep) + """

<h5>
Last update:
October 8, 1995

</h5>
</body>
</html>
"""

print output
