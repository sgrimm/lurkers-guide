#!/bin/csh

@ x=0
@ y=0
@ r=0
@ h=24
@ pal=32
@ wid=999
@ maxwid=5

while ($y < 408)
	@ x = 0

	convert +dither -crop ${wid}x${h}+0+${y} -colors $pal $1 r${r}.miff

	while ($x < ${maxwid})
		convert -crop 100x999+${x}00+0 r${r}.miff r${r}c${x}.gif
		echo $x $r
		@ x++
	end

	@ y += $h
	@ r++
end
