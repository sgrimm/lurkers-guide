FTP=/home/woodstock/ftp/pub/Babylon-5/Guide/Novels
WWW=/home/woodstock/hyperion/docs/lurk/novels
all: \
	$(FTP)/001.Voices \
	$(FTP)/002.Accusations \
	$(FTP)/003.Blood-Oath \
	$(FTP)/004.Clarks-Law \
	/dev/null

$(FTP)/001.Voices: $(WWW)/001.html
	echo "Voices" > $@
	/opt/local/bin/lynx -dump file://localhost$(WWW)/001.html | uniq | grep -v "Episode List" >> $@

$(FTP)/002.Accusations: $(WWW)/002.html
	echo "Accusations" > $@
	/opt/local/bin/lynx -dump file://localhost$(WWW)/002.html | uniq | grep -v "Episode List" >> $@

$(FTP)/003.Blood-Oath: $(WWW)/003.html
	echo "Blood Oath" > $@
	/opt/local/bin/lynx -dump file://localhost$(WWW)/003.html | uniq | grep -v "Episode List" >> $@

$(FTP)/004.Clarks-Law: $(WWW)/004.html
	echo "Clarks Law" > $@
	/opt/local/bin/lynx -dump file://localhost$(WWW)/004.html | uniq | grep -v "Episode List" >> $@

