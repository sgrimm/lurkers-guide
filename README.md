<html>
<head>
<title>The Lurker's Guide to the Lurker's Guide</title>
</head>

<h1>The Lurker's Guide to the Lurker's Guide</h1>

<p>
There are three main components to the Guide.  I'll refer to them by their
local URLs on www.midwinter.com.

<ul>
<li> <a href="#lurk">/lurk</a> -- local path /home/woodstock/hyperion/docs/lurk<br>
<li> <a href="#ftp">/ftp</a> (also /b5 and /b5p) -- local path /home/woodstock/ftp<br>
<li> <a href="#cgi-bin">/cgi-bin</a> -- local paths /usr/www/cgi-bin and /usr/www/uncgi-bin
<li> <a href="#utils">Miscellaneous utilities</a>
</ul>

<h2><a name="lurk">/lurk</a></h2>

<p>
The first section, which is the bulk of the site, is a directory tree
of static HTML files with a few server-parsed HTML files (for
server-side includes) thrown in.  The home directory of the lurk-sf
account is the top level of that directory tree, which the Web server
knows as http://www.midwinter.com/lurk.

<p>
Inside that directory are a bunch of subdirectories, mostly
corresponding to the major sections of the site.  I'll get to them
individually in a moment, but first a word on some common things you'll
see in most of them.

<h3><a name="gen.py">gen.py</a></h3>
The biggest thing to know about is a script
called "gen.py" (it's written in Python), a version of which lives in
each of the directories that contains per-episode HTML files.  The
script is responsible for assembling a static HTML file out of a header
section (generated for each episode; it includes the episode's title
among other things), a body file, and a footer section.

<p>
The body files contain the meat of the pages.  They have the same
filenames as the HTML files, minus the .html extension.  In most cases
that's just the 3-digit episode number, e.g.  /lurk/guide/056.html
contains the body file "056" from the guide subdirectory.  When I want
to add a new comment to the page for episode 33, for example, I cd into
the guide directory, edit the file "033", then run "gen.py 033".  The
result is a new version of 033.html, with the old version moved aside
to #033.html as a precaution.

<p>
One other thing some of the gen.py versions do is add timestamps to items.
The idea is that when I modify a page, I want to highlight the change so
people who've already read the previous version can quickly scroll to the
new stuff.  To that end, gen.py looks for a special token "@@@" (which
must by followed by whitespace or end-of-line) in the body file.  When
it finds the token, it replaces it with "@@@nnn" where nnn is the current
timestamp in UNIX time_t format.  This change is made to the body file
itself.  Then, when gen.py generates the HTML file from a body file, it
inserts the date in bold wherever it sees a recent timestamp.  Once a day
a script regenerates all the HTML files so that the date markers go away
after a while (keeping them in makes the pages look ugly and makes it
hard to quickly locate new items).

<p>
When you see a file "skel", it's a prototype body file.  I copy that to
the appropriate filename when a new episode/novel/etc. comes out.

<p>
In many of the subdirectories you'll see a "genall.csh" script.  This
is just a shortcut that runs "gen.py" for each episode.  I only use
this when I make a change to the structure of the page headers/footers
and want to apply it everywhere.

<p>
Also, in the top-level directory there's a "gen" script.  This runs
gen.py in each of the subdirectories.  I use that when, for example, I
add a picture for an episode and want it to be displayed on all the
pages for that episode.

<h3>What's under /lurk</h3>

<p>
<dl>
<dt>
<a href="/lurk/background/">background</a>
<dd>
"The story so far" for each episode of seasons 1-4.
Season 5 still needs to be written.  The user-accessible files here are
all .shtml files.  You'll notice there aren't any body files here.
Instead, background/gen.py uses a few other source files.  There are
several different introductory summaries, which are in the sum-*
files.  For example, sum-66 summarizes the story up to and including
episode 66, and is included as the first part of the backgrounds for
episodes 67 and later.  After the appropriate sum-* file is included,
gen.py looks in story.html.  That file contains per-episode paragraphs
that are included in the "More recently..." section of the background
pages, under the introductions.  Only the paragraphs for episodes
between the summary and the current episode are included, e.g. for
episode 75, gen.py includes sum-66 and the paragraphs for episodes
67-74.

<p>
<dt>
<a href="/lurk/chars/">chars</a>
<dd>
Bios of some of the characters.  These are linked to from the
"Universe" section.  Simple flat HTML files.

<p>
<dt>
<a href="/lurk/comic/">comic</a>
<dd>
Guide pages for the comic series.  <a href="#gen.py">Body files and gen.py</a>.

<p>
<dt>
<a href="/lurk/countries/">countries</a>
<dd>
The only HTML file directly under here is index.html, which is a list of
all the countries for which I have schedule information.  Each country
has a subdirectory under here (the directory name is generally the
country's 2-letter country code).  One pseudo-country of note is "master",
which contains the master episode list with the episodes in the correct
order (the original US airing order was slightly wrong storywise).

<p>
Inside each country's directory is an eplist.html file with the schedule
for that country, plus symbolic links to the various episode-guide
subdirectories.  By using relative paths in my URLs, I let the browser
remember which country's schedule the user was looking at without resorting
to cookies (which didn't yet exist as a concept when this site first
started!)  For example, /lurk/countries/se/eplist.html has a link to
"guide/056.html", which is really the same file as /lurk/guide/056.html
since /lurk/countries/se/guide is a symlink to /lurk/guide.  But since the
browser doesn't know that, 056.html's link back to the episode page is
"../eplist.html" which goes back to the Swedish schedule.

<p>
index.html is a link to eplist.html in the country directories.
There is a "mkcountry.sh" script to set up a new country directory.  There
are also skeleton episode lists for all 5 seasons (skel, skel2, skel3, etc.)

<p>
When I edit a country's schedule I use the "print-weeks.py" script, which
is described later, so I don't have to hand-type dates.

<p>
<dt>
<a href="/lurk/credits/">credits</a>
<dd>
Episode credits.  <a href="#gen.py">Body files and gen.py</a>.

<p>
<dt>
<a href="/lurk/eplist.html">eplist.html</a>
<dd>
On the surface, just a symlink to /lurk/countries/us/eplist.html.  But
the Web server is configured to alias /lurk/eplist.html to
/cgi-bin/select-eplist, which does the per-country episode list selection.

<p>
<dt>
footer
<dd>
The standard page footer used throughout the site.  Not used programmatically,
but I load this into my text editor when I create a new page.

<p>
<dt>
gen
<dd>
Script that runs gen.py for an episode in each of the episode-related
subdirectories.

<p>
<dt>
gif
<dd>
Pretty self-explanatory.  All the non-navigation-related images live here.
(Some of them are JPEGs despite the directory name.)

<p>
<dt>
<a href="/lurk/guide/">guide</a>
<dd>
The episode guide pages.  <a href="#gen.py">Body files and gen.py</a>.

<p>
<dt>
<a href="/lurk/help.html">help.html</a>
<dd>
The site FAQ.  It's referred to by the name "help" instead to avoid confusion
with the Usenet B5 FAQ.

<p>
<dt>
index.html
<dd>
Symlink to lurker.html.

<p>
<dt>
internal
<dd>
Data files used by gen.py and CGIs.

<dl>

<p>
<dt>
comicnames
<dd>
Titles of comics, one title per line.

<p>
<dt>
dom-error.html
<dd>
Error message returned when the episode-list selection CGI program can't
find an episode list to show.

<p>
<dt>
domainlist
<dd>
A mapping between DNS top-level domains and country directories.  This
is used so that when someone clicks on the "Episodes" link on the home
page, they're taken to the schedule page for their country.  (It doesn't
suffice to just use the last part of the domain name as a country name
since there are UK ISPs that use .net addresses, for example.)  More on
this in the CGI section below.

<p>
<dt>
epnames
<dd>
Titles of episodes, one title per line, starting with episode 0.
Note that there are a lot of dummy lines because of gaps in episode numbering.

<p>
<dt>
lu-footer<br>
lu-footer-epnum<br>
lu-header<br>
lu-header-date<br>
lu-header-epnum<br>
lu-header-guide<br>
lu-header-synops
<dd>
Headers and footers used by gen.py.

<p>
<dt>
novelnames
<dd>
Titles of novels, one title per line, used by novels/gen.py.

<p>
<dt>
otros.py
<dd>
Library with common functions used by various gen.py versions.

<p>
<dt>
picnames
<dd>
Names of images to display in episode guide page headers.  Each of these
names is relative to /lurk/gif/XXX where XXX is the episode number.  Note
that there are a lot of dummy lines because of gaps in episode numbering.

</dl>

<p>
<dt>
lastmod.html
<dd>
List of episode guide and synopsis pages, ordered by modification time.
Generated by the "lastupdate.py" utility.

<p>
<dt>
lgchat.html
<dd>
Live voice-chat for LG users.  Beats the heck out of text-only conferencing.

<p>
<dt>
<a href="/lurk/lurker-nobg.html">lurker-nobg.html</a>
<dd>
Home page without the black background, since some people find custom
backgrounds annoying.  This is automatically generated from lurker.html
via a cron job that runs "make" in the top-level directory.

<p>
<dt>
<a href="/lurk/lurker.html">lurker.html</a>
<dd>
The home page.

<p>
<dt>
Makefile
<dd>
Once an hour this Makefile is used to rebuild lurker-nobg.html, and once
a day it's used to make sure I haven't forgotten to run gen.py on something.
It also updates the b5tvlist.txt file in the FTP archive.

<p>
<dt>
<a href="/lurk/making/">making</a>
<dd>
The "Making of B5" section of the site.  Flat HTML files.

<p>
<dt>
maps
<dd>
Imagemaps for navbar.

<p>
<dt>
misc
<dd>
Miscellaneous documents that don't fit elsewhere, e.g. a blurb about
why Claudia Christian left the show.

<p>
<dt>
<a href="/lurk/nav/">nav</a>
<dd>
Navigational images.  A secondary function is that its index.html is the
"preload your image cache here" page, which speeds up the site for folks
by letting them suffer all the pain of nav image downloading in one shot.

<p>
<dt>
<a href="/lurk/novels/">novels</a>
<dd>
Novel guide pages.  This section is incomplete -- there aren't pages yet
for the latest round of novels.  <a href="#gen.py">Body files and gen.py</a>.

<p>
<dt>
<a href="/lurk/p5/">p5</a>
<dd>
Results of the Poll 5 viewer survey for each episode.

<p>
<dt>
<a href="/lurk/reference/">reference</a>
<dd>
The "Reference" section of the site.  Mostly flat HTML files, but a couple
of the pages have server-side includes that bring in text files from the
FTP area.

<p>
<dt>
<a href="/lurk/renew.html">renew.html</a>
<dd>
Not referenced any more, but perhaps of historical amusement.

<p>
<dt>
<a href="/lurk/resources/">resources</a>
<dd>
The "Other Resources" section of the site.  Flat HTML files.

<p>
<dt>
src
<dd>
Contains scripts and CGI source code.  More on these later.

<p>
<dt>
<a href="/lurk/support/">support</a>
<dd>
Not used any more (but retained since there are links to it on other
sites).

<p>
<dt>
<a href="/lurk/survey.html">survey.html</a>
<dd>
A survey I ran a few times.

<p>
<dt>
<a href="/lurk/synops/">synops</a>
<dd>
Episode synopses.  <a href="#gen.py">Body files and gen.py</a>.

<p>
<dt>
<a href="/lurk/toc.html">toc.html</a>
<dd>
Site map.

<p>
<dt>
<a href="/lurk/universe/">universe</a>
<dd>
The "Universe and Characters" section of the site.  Flat HTML files.  Season
5 still needs to be written up.

<p>
<dt>
<a href="/lurk/volunteers.html">volunteers.html</a>
<dd>
An exercise in naivete; I ended up doing all this myself!

<p>
<dt>
<a href="/lurk/vqt.html">vqt.html</a>
<dd>
JMS message about B5 being ranked #1 by Viewers for Quality Television.

<p>
<dt>
<a href="/lurk/welcome.html">welcome.html</a>
<dd>
Introduction to the site after the switch to the current look and feel.

<p>
<dt>
<a href="/lurk/whatsnew.html">whatsnew.html</a>
<dd>
The What's New page.  When this gets to be big, I put all the old
items in archive what's-new pages and link to them at the bottom of this
page.

</dl>

<h2><a name="ftp">/ftp</a></h2>

<p>
The FTP archive at
<a href="ftp://ftp.midwinter.com/">ftp://ftp.midwinter.com/</a>
is also accessible via HTTP.  There are a few aliases pointing into the
FTP directory.

<p>
<dl>
<dt>
/ftp
<dd>
The top-level FTP directory.
<dt>
/b5
<dd>
The /pub/Babylon-5 FTP directory.
<dt>
/b5p
<dd>
The /pub/Babylon-5/Pictures FTP directory.
</dl>

<p>
See the FTP index file (available in both
<a href="/b5/Index">text</a>
and
<a href="/b5/index.html">HTML</a>)
for descriptions of the various files in the FTP archive.  The Lurker's
Guide points to various files here -- including some server-side include
references.

<h2><a name="cgi-bin">/cgi-bin</a></h2>

<p>
There are several Lurker's Guide-related CGI programs.  The source code
for all of them may be found in /lurk/src.  Some of them make use of my
<a href="http://www.midwinter.com/~koreth/uncgi.html">Un-CGI</a>
utility; you can distinguish those by the fact that their URLs look like
/cgi-bin/uncgi/programname.

<p>
<dl>

<dt>
/cgi-bin/uncgi/lgmail
<dd>
Displays the feedback form.  This is CGI because it needs to look at
the referring URL and pass that on to the mail-sending program via a
hidden form field.

<p>
<dt>
/cgi-bin/uncgi/lgsend
<dd>
Accepts input from the feedback form and sends me mail.

<p>
<dt>
/lurk/search.html
<dd>
The entry point to the site search system. This is just a Google custom
search box (there used to be a custom search system but Google is better
and faster.)

<p>
<dt>
/cgi-bin/printcurrent
<dd>
This isn't currently used, but when the show was originally airing in
the US, there was a "This week's episode" link on the home page that
pointed to this script.  It looked at a symbolic link in /lurk/guide
and redirected the user to the episode the link pointed to.  The link
was updated by the update-current.sh script.

<p>
<dt>
/cgi-bin/select-eplist
<dd>
Displays one of the episode lists based on which country the user is
coming from.  Uses /lurk/internal/domainlist to obtain the domain-to-country
mapping.

</dl>

<h2><a name="utils">Miscellaneous utilities</a></h2>

<p>
There are a few utilities whose source you'll find in /lurk/src unless
otherwise noted.  Some have been discussed above, but here's a list:

<p>
<dl>
<dt>
gen-guide.sh
<dd>
This is run once a day from a cron job.  It regenerates all the guide pages.
If a regenerated page doesn't differ from the old version, the old version
is moved back into place to preserve its modification date.  The purpose
here is to expire the timestamps on new items in the guide pages (see the
discussion of
<a href="#gen.py">the gen.py script</a>
above).

<p>
<dt>
lastupdate.py
<dd>
Run once an hour from cron; this updates /lurk/lastmod.html and is pretty
self-explanatory.

<p>
<dt>
update-current.sh
<dd>
Updates the symbolic link used by /cgi-bin/printcurrent.  Not currently
in use.

</dl>


<h5>Last update: June 7, 2007</h5>
</body>
</html>
