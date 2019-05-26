B5DIR=/home/woodstock/ftp/pub/Babylon-5

all: commit lurker-nobg.html subdirs

commit:
	@-/usr/bin/git diff-files --quiet || /usr/bin/git commit -a -q -m "Automatic commit of changes"

lurker-nobg.html: lurker.html
	sed -e 's/ BGSTART.*BGEND//' -e '/BGLINK/d' < lurker.html > $@

subdirs:
	@(cd guide; sh genall.csh)
	@(cd comic; sh genall.csh)
	@(cd synops; sh genall.csh)
	@(cd novels; sh genall.csh)

tvlist: $(B5DIR)/b5tvlist.txt

$(B5DIR)/b5tvlist.txt: reference/tvl.html
	/opt/local/bin/lynx -nolist -dump http://www.midwinter.com/lurk/reference/tvl.html > $@
