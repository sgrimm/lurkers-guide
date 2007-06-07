#!/usr/bin/python
#
# Print a bunch of dates, one week apart, replacing dates at the beginning
# of standard input.
#

import string, sys
from time import gmtime,mktime

ndays = [ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 ]
step = 7
skipdays = [ 0, 0, 0, 0, 0, 0, 0 ]
countdays = [ 1, 1, 1, 1, 1, 1, 1 ]
europe = 0

while sys.argv[1][0] == '-':
	if sys.argv[1] == '-step':
		step = string.atoi(sys.argv[2])
		sys.argv = sys.argv[2:]

	if sys.argv[1] == '-e':
		europe = 1
		sys.argv = sys.argv[1:]

	if sys.argv[1] == '-daily':
		step = 1
		# Skip Saturday and Sunday
		skipdays[5] = 1
		skipdays[6] = 1
		sys.argv = sys.argv[1:]

	if sys.argv[1] == '-on':
		step = 1
		skipdays = [ 1, 1, 1, 1, 1, 1, 1 ]
		for num in string.splitfields(sys.argv[2], ','):
			skipdays[string.atoi(num)] = 0
		sys.argv = sys.argv[2:]

	if sys.argv[1] == '-repeat':
		for num in string.splitfields(sys.argv[2], ','):
			n = string.atoi(num)
			countdays[n] = countdays[n] + 1
		sys.argv = sys.argv[2:]

	if sys.argv[1] == '-skip':
		step = 1
		skipdays = [ 0, 0, 0, 0, 0, 0, 0 ]
		for num in string.splitfields(sys.argv[2], ','):
			skipdays[string.atoi(num)] = 1
		sys.argv = sys.argv[2:]

year = string.atoi(sys.argv[1])
month = string.atoi(sys.argv[2])
day = string.atoi(sys.argv[3])
if year < 93:
	year = year + 100

(x,x,x,x,x,x,weekday,x,x) = gmtime(mktime(year + 1900, month,
					day, 10, 0, 0, 0, 0, 0))
thisdaycount = 0

if len(sys.argv) > 4:
	step = string.atoi(sys.argv[4])

if (year & 3) == 0:
	ndays[2] = 29

line = sys.stdin.readline()
while line != '':
	if line[0:5] == '<pre>':
		head = '<pre>'
		line = line[5:]
	else:
		head = ''

	if not (line[0] in ['0','1','2','3','4','5','6','7','8','9','?']):
		sys.stdout.write(head + line)
		line = sys.stdin.readline()
		continue
	else:
		if europe == 1:
			sys.stdout.write('%s%02d/%02d/%02d%s' % (head,
					day, month, year % 100, line[8:]))
		else:
			sys.stdout.write('%s%02d/%02d/%02d%s' % (head,
					year % 100, month, day, line[8:]))

	line = sys.stdin.readline()

	thisdaycount = thisdaycount + 1
	if thisdaycount < countdays[weekday]:
		continue

	day = day + step
	(x,x,x,x,x,x,weekday,x,x) = gmtime(mktime(year + 1900, month,
						day, 10, 0, 0, 0, 0, 0))
	thisdaycount = 0

	while skipdays[weekday]:
		day = day + 1
		weekday = (weekday + 1) % 7

	if day > ndays[month]:
		day = day - ndays[month]
		month = month + 1

		if month > 12:
			month = 1

			year = year + 1

			if (year & 3) == 0:
				ndays[2] = 29
			else:
				ndays[2] = 28
