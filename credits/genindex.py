#!/usr/local/bin/python3

import sys,string
sys.path.append('../internal')
import otros

pix = open('../internal/epnames', 'r')
epnames = pix.readlines()
pix.close()

seasons = [(1, 'Season One'), (23, 'Season Two'), (45, 'Season Three'),
	   (67, 'Season Four'), (89, 'Season Five')]
season = 0

output = open('index.html', 'w')

output.write("""<html>
<head>
<title>Babylon 5 Episode Credits</title>
<link rev=made href="mailto:genoa@mit.edu">
</head>

<body>
<h1>Babylon 5 Episode Credits</h1>

<ul>
""")

for i in range(0, 200):
	filename = "%03d" % i
	print(filename)
	try:
		handle = open(filename, 'r')
		handle.close()
	except:
		break

	if i == seasons[season][0]:
		output.write('</ul>\n\n<h2>' + seasons[season][1] +
				'</h2>\n<ul>\n')
		season = season + 1
	output.write('   <li> <a href="%03d.html">%s</a>\n' % (i,
			epnames[i][:-1]))

output.write("""
</ul>

<p>
<a href="/lurk/episodes.php">To the episode list</a>

</body>
</html>
""")

output.close()
