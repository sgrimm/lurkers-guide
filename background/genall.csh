#!/bin/csh
@ ep = 1
@ toteps = `sed '/XXX/,$d' < ../internal/epnames | wc -l`

set list = ""

while ($ep < $toteps)
	set list = "$list $ep"
	@ ep++
end

gen.py $list
