#!/bin/csh

@ x=0
@ y=0
@ r=0
@ h=100
@ w=80
@ pal=32
@ maxwid=800

while ($y < 400)
	@ x = 0
	@ c = 0

	convert -crop 9999x${h}+0+${y} $1 r${r}.miff

	while ($x < ${maxwid})
		convert +dither -colors ${pal} -crop ${w}x9999+${x}+0 r${r}.miff r${r}c${c}.gif
		echo $r $x
		@ x += $w
		@ c++
	end

	rm r${r}.miff

	@ y += $h
	@ r++
end
