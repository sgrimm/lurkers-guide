#!/bin/csh

@ x=0
@ y=0
@ r=0
@ h=32
@ pal=80
@ wid=999
@ maxwid=5

while ($y < 272)
	@ x = 0

	convert +dither -crop ${wid}x${h}+0+${y} -colors $pal $1 r${r}.miff

	while ($x < ${maxwid})
		convert -crop 100x999+${x}00+0 r${r}.miff r${r}c${x}.gif
		echo $x $r
		@ x++
	end

	@ y += $h
	@ h = 24
	@ pal = 20
	@ r++
	if ($r == 15) then
		@ wid = 400
		@ maxwid = 4
	endif
end

#
# Two-row-high set of buttons; use the same palette on both rows.
#

convert -colors ${pal} +dither -crop 500x48+0+${y} $1 r11-12.miff

@ x = 0
@ r2 = $r + 1
while ($x < ${maxwid})
	convert -crop 100x${h}+${x}00+0 r11-12.miff r${r}c${x}.gif
	convert -crop 100x${h}+${x}00+${h} r11-12.miff r${r2}c${x}.gif
	echo $x $r - $r2
	@ x++
end

@ r += 2
@ y += $h + $h

while ($y < 392)
	@ x = 0

	convert +dither -crop ${wid}x${h}+0+${y} -colors $pal $1 r${r}.miff

	while ($x < ${maxwid})
		convert -crop 100x999+${x}00+0 r${r}.miff r${r}c${x}.gif
		echo $x $r
		@ x++
	end

	@ y += $h
	@ h = 24
	@ pal = 20
	@ r++
	if ($r == 15) then
		@ wid = 400
		@ maxwid = 4
	endif
end
