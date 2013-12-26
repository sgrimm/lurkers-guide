#!/usr/local/bin/python

import sys,string

curep = string.atoi(sys.argv[1])
cureplong = '%03d' % (curep)
prevep = '%03d' % (curep - 1)
nextep = '%03d' % (curep + 1)

curep = `curep`

output = """
<p align=center nowrap><nobr><img
	align=middle border=0 usemap="#titlebar"
	src="/lurk/nav/titlebar.gif" width=500 height=32
	alt="[Home]">

<br><a href="/lurk/gif/comic/""" + cureplong + """.jpg"><img
align=middle width=78 height=120 border=0
	src="/lurk/gif/comic/""" + cureplong + """.gif" alt=""></a><img
align=middle width=422 height=120
	src="/lurk/gif/titles/c""" + curep + """.gif"
	alt="">

<br><img
	align=middle border=0
	src="/lurk/nav/lit/gu-guide.gif" width=100 height=24
	alt=" ### GUIDE ### "><img
align=middle border=0
	src="/lurk/nav/dark/ep-blank.gif" width=100 height=24
	alt=""><img
align=middle border=0
	src="/lurk/nav/dark/ep-blank.gif" width=100 height=24
	alt=""><img
align=middle border=0
	src="/lurk/nav/dark/ep-blank.gif" width=100 height=24
	alt=""><a
href="index.html"><img
	align=middle border=0
	src="/lurk/nav/dark/gu-index.gif" width=34 height=24
	alt="[Index] "></a><a
href=""" + '"' + prevep + """.html"><img
	align=middle border=0
	src="/lurk/nav/dark/gu-prev.gif" width=33 height=24
	alt="[Previous] "></a><a
href=""" + '"' + nextep + """.html"><img
	align=middle border=0
	src="/lurk/nav/dark/gu-next.gif" width=33 height=24
	alt="[Next] "></a>
</nobr>
"""

if len(sys.argv) != 3:
	output = output + """
<p>
<b>Contents:</b>
<a href="#OV">Overview</a> -
<a href="#SY">Synopsis</a> -
<a href="#BP">Backplot</a> -
<a href="#UQ">Questions</a> -
<a href="#AN">Analysis</a> -
<a href="#NO">Notes</a> -
<a href="#JS">JMS</a>

<p>
<hr>
"""

print output
