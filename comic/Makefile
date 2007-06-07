FTP=/home/woodstock/ftp/pub/Babylon-5/Guide/Comic
WWW=/home/woodstock/hyperion/docs/lurk/comic
all: \
	$(FTP)/001.In-Darkness-Find-Me \
	$(FTP)/002.Treason \
	$(FTP)/003.In-Harms-Way \
	$(FTP)/004.The-Price-of-Peace \
	$(FTP)/005.With-Friends-Like-These \
	$(FTP)/006.Against-the-Odds \
	$(FTP)/007.Survival-the-Hard-Way \
	$(FTP)/008.Silent-Enemies \
	$(FTP)/009.Duet... \
	$(FTP)/010.Coda... \
	$(FTP)/011.The-Psi-Corps-and-You \
	/dev/null

$(FTP)/001.In-Darkness-Find-Me: $(WWW)/001.html
	echo "In Darkness Find Me" > $@
	/opt/local/bin/lynx -dump file://localhost$(WWW)/001.html | uniq | grep -v "Episode List" >> $@

$(FTP)/002.Treason: $(WWW)/002.html
	echo "Treason" > $@
	/opt/local/bin/lynx -dump file://localhost$(WWW)/002.html | uniq | grep -v "Episode List" >> $@

$(FTP)/003.In-Harms-Way: $(WWW)/003.html
	echo "In Harms Way" > $@
	/opt/local/bin/lynx -dump file://localhost$(WWW)/003.html | uniq | grep -v "Episode List" >> $@

$(FTP)/004.The-Price-of-Peace: $(WWW)/004.html
	echo "The Price of Peace" > $@
	/opt/local/bin/lynx -dump file://localhost$(WWW)/004.html | uniq | grep -v "Episode List" >> $@

$(FTP)/005.With-Friends-Like-These: $(WWW)/005.html
	echo "With Friends Like These" > $@
	/opt/local/bin/lynx -dump file://localhost$(WWW)/005.html | uniq | grep -v "Episode List" >> $@

$(FTP)/006.Against-the-Odds: $(WWW)/006.html
	echo "Against the Odds" > $@
	/opt/local/bin/lynx -dump file://localhost$(WWW)/006.html | uniq | grep -v "Episode List" >> $@

$(FTP)/007.Survival-the-Hard-Way: $(WWW)/007.html
	echo "Survival the Hard Way" > $@
	/opt/local/bin/lynx -dump file://localhost$(WWW)/007.html | uniq | grep -v "Episode List" >> $@

$(FTP)/008.Silent-Enemies: $(WWW)/008.html
	echo "Silent Enemies" > $@
	/opt/local/bin/lynx -dump file://localhost$(WWW)/008.html | uniq | grep -v "Episode List" >> $@

$(FTP)/009.Duet...: $(WWW)/009.html
	echo "Duet..." > $@
	/opt/local/bin/lynx -dump file://localhost$(WWW)/009.html | uniq | grep -v "Episode List" >> $@

$(FTP)/010.Coda...: $(WWW)/010.html
	echo "Coda..." > $@
	/opt/local/bin/lynx -dump file://localhost$(WWW)/010.html | uniq | grep -v "Episode List" >> $@

$(FTP)/011.The-Psi-Corps-and-You: $(WWW)/011.html
	echo "The Psi Corps and You" > $@
	/opt/local/bin/lynx -dump file://localhost$(WWW)/011.html | uniq | grep -v "Episode List" >> $@

