#!/usr/local/bin/python3

import sys,string
sys.path.append('../internal')
import otros

curep = int(sys.argv[1])

if len(sys.argv) == 3:
	output = otros.pageheader(curep, 'other')
else:
	output = otros.pageheader(curep, 'guide')

if len(sys.argv) != 3:
	output = output + """
<p>
<b>Contents:</b>
<a href="#OV">Overview</a> -
<a href="#BP">Backplot</a> -
<a href="#UQ">Questions</a> -
<a href="#AN">Analysis</a> -
<a href="#NO">Notes</a> -
<a href="#JS">JMS</a>

<p>
<hr>
"""

print(output)
