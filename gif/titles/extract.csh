#!/bin/csh

@ chunksize=8
@ x=0
@ y=0
@ h=120
@ pal=4
@ wid=999
@ maxwid=5
@ chunkheight = $chunksize * $h

if ($#argv < 3) then
	echo Usage: $0 file prefix startrow
	exit 1
endif

@ r=$3
@ chunks = 0

echo -n determining image height...
set imheight="`identify comictitles.gif |& cut -d' ' -f2 | cut -dx -f2`"
echo $imheight

while ($y < $imheight)
	convert -crop ${wid}x${chunkheight}+0+${y} +compress $1 chunk${chunks}.gif
	echo chunk $chunks

	@ y += $chunkheight
	@ chunks++
end

@ chunkno = 0

while ($chunkno < $chunks)
	@ y = 0

	while ($y < $chunkheight)
		convert +dither -crop ${wid}x${h}+0+${y} -colors $pal chunk${chunkno}.gif $2${r}.gif
		echo $r

		@ y += $h
		@ r++
	end

	@ chunkno++
end
