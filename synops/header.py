#!/usr/local/bin/python

import sys,string
sys.path.append('../internal')
import otros

curep = string.atoi(sys.argv[1])

output = otros.pageheader(curep, 'synopsis')

output = output + """<pre>

</pre>"""

print output
