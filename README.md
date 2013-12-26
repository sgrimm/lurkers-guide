The Lurker's Guide to the Lurker's Guide
========================================

There are three main components to the Guide.  I'll refer to them by their
local URLs on www.midwinter.com.

* [/lurk](lurk) - the main top-level directory.
* [/lurk/ftp](lurk/ftp) - the former contents of the B5 directory on
  midwinter.com's anonymous FTP service; this is linked to from several
  places in the Guide.
* Some miscellaneous utilities

/lurk
-----

The first section, which is the bulk of the site, is a directory tree
of static HTML files with a few server-parsed HTML files (for
server-side includes) thrown in, plus one short PHP script.

Inside that directory are a bunch of subdirectories, mostly
corresponding to the major sections of the site.  I'll get to them
individually in a moment, but first a word on some common things you'll
see in most of them.

## gen.py

The biggest thing to know about is a Python script called "gen.py",
a version of which lives in each of the directories that contains
per-episode HTML files.  The script is responsible for assembling a
static HTML file out of a header section (generated for each episode;
it includes the episode's title among other things), a body file, and
a footer section.

The body files contain the meat of the pages.  They have the same
filenames as the HTML files, minus the .html extension.  In most cases
that's just the 3-digit episode number, e.g.  /lurk/guide/056.html
contains the body file "056" from the guide subdirectory.  When I want
to add a new comment to the page for episode 33, for example, I cd into
the guide directory, edit the file "033", then run "gen.py 033".  The
result is a new version of 033.html, with the old version moved aside
to #033.html as a precaution.

One other thing some of the gen.py versions do is add timestamps to items.
The idea is that when I modify a page, I want to highlight the change so
people who've already read the previous version can quickly scroll to the
new stuff.  To that end, gen.py looks for a special token "@@@" (which
must by followed by whitespace or end-of-line) in the body file.  When
it finds the token, it replaces it with "@@@nnn" where nnn is the current
timestamp in UNIX time\_t format.  This change is made to the body file
itself.  Then, when gen.py generates the HTML file from a body file, it
inserts the date in bold wherever it sees a recent timestamp.  Once a day
a script regenerates all the HTML files so that the date markers go away
after a while (keeping them in makes the pages look ugly and makes it
hard to quickly locate new items).

When you see a file "skel", it's a prototype body file.  I copy that to
the appropriate filename when a new episode/novel/etc. comes out.

In many of the subdirectories you'll see a "genall.csh" script.  This
is just a shortcut that runs "gen.py" for each episode.  I only use
this when I make a change to the structure of the page headers/footers
and want to apply it everywhere.

Also, in the top-level directory there's a "gen" script.  This runs
gen.py in each of the subdirectories.  I use that when, for example, I
add a picture for an episode and want it to be displayed on all the
pages for that episode.

What's under /lurk
------------------

### background

"The story so far" for each episode of seasons 1-4.
Season 5 still needs to be written.  The user-accessible files here are
all .shtml files.  You'll notice there aren't any body files here.
Instead, background/gen.py uses a few other source files.  There are
several different introductory summaries, which are in the sum-\*
files.  For example, sum-66 summarizes the story up to and including
episode 66, and is included as the first part of the backgrounds for
episodes 67 and later.  After the appropriate sum-\* file is included,
gen.py looks in story.html.  That file contains per-episode paragraphs
that are included in the "More recently..." section of the background
pages, under the introductions.  Only the paragraphs for episodes
between the summary and the current episode are included, e.g. for
episode 75, gen.py includes sum-66 and the paragraphs for episodes
67-74.

### chars

Bios of some of the characters.  These are linked to from the
"Universe" section.  Simple flat HTML files.

### comic

Guide pages for the comic series.  <a href="#gen.py">Body files and gen.py</a>.

### countries

The only HTML file directly under here is index.html, which is a list of
all the countries for which I have schedule information.  Each country
has a subdirectory under here (the directory name is generally the
country's 2-letter country code).  One pseudo-country of note is "master",
which contains the master episode list with the episodes in the correct
order (the original US airing order was slightly wrong storywise).

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

index.html is a link to eplist.html in the country directories.
There is a "mkcountry.sh" script to set up a new country directory.  There
are also skeleton episode lists for all 5 seasons (skel, skel2, skel3, etc.)

When I edit a country's schedule I use the "print-weeks.py" script, which
is described later, so I don't have to hand-type dates.

### credits

Episode credits.

### episodes.php

A short PHP script that does a simple lookup of the user's domain name to
figure out which country's episode list to show. Perhaps less relevant now
that the show is pretty much off the air, but when different seasons were
showing in different countries simultaneously this was handy for users.

### eplist.html

A symlink to the US episode list, so that any links to "../eplist.html" in
the the non-country-based guide directories will go somewhere useful. (See the
discussion of the "countries" directory above.)

### footer

The standard page footer used throughout the site.  Not used programmatically,
but I load this into my text editor when I create a new page.

### gen

Script that runs gen.py for an episode in each of the episode-related
subdirectories.

### gif

Pretty self-explanatory.  All the non-navigation-related images live here.
(Some of them are JPEGs despite the directory name.)

### guide

The episode guide pages.

### help.html

The site FAQ.  It's referred to by the name "help" instead to avoid confusion
with the Usenet B5 FAQ.

### index.html

Symlink to lurker.html.

### internal

Data files used by gen.py and CGIs:

* comicnames - Titles of comics, one title per line.
* dom-error.html - Error message returned when the episode-list selection script
  can't find an episode list to show.
* epnames - Titles of episodes, one title per line, starting with episode 0.
  Note that there are a lot of dummy lines because of gaps in episode numbering.
* lu-\* - Headers and footers used by gen.py.
* novelnames - Titles of novels, one title per line, used by novels/gen.py.
* otros.py - Library with common functions used by various gen.py versions.
* picnames - Names of images to display in episode guide page headers.  Each of
  these names is relative to /lurk/gif/XXX where XXX is the episode number.  Note
  that there are a lot of dummy lines because of gaps in episode numbering.

### lastmod.html

List of episode guide and synopsis pages, ordered by modification time.
Generated by the "lastupdate.py" utility.

### lurker-nobg.html

Home page without the black background, since some people find custom
backgrounds annoying.  This is automatically generated from lurker.html
via a cron job that runs "make" in the top-level directory.

### lurker.html

The home page.

### Makefile

Once an hour this Makefile is used to rebuild lurker-nobg.html, and once
a day it's used to make sure I haven't forgotten to run gen.py on something.
It also updates the b5tvlist.txt file in the FTP archive.

### making

The "Making of B5" section of the site.  Flat HTML files.

### maps

Imagemaps for navbar.

### misc

Miscellaneous documents that don't fit elsewhere, e.g. a blurb about
why Claudia Christian left the show.

### nav

Navigational images.  A secondary function is that its index.html is the
"preload your image cache here" page, which speeds up the site for folks
by letting them suffer all the pain of nav image downloading in one shot.

### novels

Novel guide pages.  This section is incomplete -- there aren't pages yet
for the latest round of novels.

### p5

Results of the Poll 5 viewer survey for each episode.

### reference

The "Reference" section of the site.  Mostly flat HTML files, but a couple
of the pages have server-side includes that bring in text files from the
FTP area.

### renew.html

Not referenced any more, but perhaps of historical amusement.

### resources

The "Other Resources" section of the site.  Flat HTML files.

### src

Contains some random support code. More on this later.

### synops

Episode synopses.

### toc.html

Site map.

### universe

The "Universe and Characters" section of the site.  Flat HTML files.  Season
5 still needs to be written up.

### volunteers.html

An exercise in naivete; I ended up doing all this myself!

### vqt.html

JMS message about B5 being ranked #1 by Viewers for Quality Television.

### welcome.html

Introduction to the site after the switch to the current look and feel.

### whatsnew.html

The What's New page.  When this gets to be big, I put all the old
items in archive what's-new pages and link to them at the bottom of this
page.

/lurk/ftp
---------

midwinter.com used to host an FTP archive with text files and images; these
are now all accessible via the Lurker's Guide and are stored in the "ftp"
subdirectory.

See the FTP index file (available in both
<a href="ftp/Index">text</a>
and
<a href="ftp/index.html">HTML</a>)
for descriptions of the various files in the FTP archive.  The Lurker's
Guide points to various files here -- including some server-side include
references.

Miscellaneous utilities
-----------------------

There are a few utilities whose source you'll find in /lurk/src unless
otherwise noted.  Some have been discussed above, but here's a list:

### gen-guide.sh

This is run once a day from a cron job.  It regenerates all the guide pages.
If a regenerated page doesn't differ from the old version, the old version
is moved back into place to preserve its modification date.  The purpose
here is to expire the timestamps on new items in the guide pages (see the
discussion of the gen.py script above).

### lastupdate.py

Run once an hour from cron; this updates /lurk/lastmod.html and is pretty
self-explanatory.
