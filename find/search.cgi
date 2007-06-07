#!/usr/local/bin/perl
#
# Babylon 5 lurker's guide searching script.
#
# Config params
#
# Full path to the index search program
$SWISHBIN   = "/opt/local/bin/swish";

# Full path to where the index files are stored (no trailing slash)
$swishdbdir = "/home/woodstock/hyperion/docs/newlurk/find/indexes";

# URL to the print-hit-bold script used to bold search results.
# Note that a bunch of stuff is tacked onto this string, so no
# trailing spaces or slashes, ok?
$PRINTURL="http://www.midwinter.com/cgi-bin/print-hit-bold";

# These are for frobbing result urls on mirror sites - allows the mirror
# to rip out bits of search result URLs and replace them with something else
# (NB: these are Perl regexps, so mind your escaping of metachars like :

# $PREFIX="http\://some.other.site/mirror/lurk/";
# $REMOVE="http\://www.midwinter.com/lurk/";
$REMOVE= "thiswillnevermatchanythign";

# The directory where your swish binaries are residing
# Full path name should be used.


# Pulls in banner for search.
$BANNER= `cat /home/woodstock/hyperion/docs/newlurk/find/prep-files/sbanner`;


#
# Maximum number of documents output
#
$maxout = 100;

#
# End Config Params
#


#
# Get our parameters
#
$searchkey = $ENV{"QUERY_STRING"};
$swishdb = $ENV{"PATH_INFO"};
$server = $ENV{"SERVER_NAME"};
$serverport = $ENV{"SERVER_PORT"};
if ($serverport ne "80") {
  $server .= ":" . $serverport;
}
$scriptname = $ENV{"SCRIPT_NAME"};
$INFO{'GO'}="Internal error - no \"key\" field!";

if ( $searchkey ) {
	&form_at($searchkey, *INFO);
    $searchkey= $INFO{'KEY'};

    $printkey = $searchkey;
    $printkey =~ s'<'&lt;';
    $printkey =~ s'>'&gt;';
}

$INFO{"JMSANSWERS"} =1 if  $swishdb=~ /answers/;
$INFO{'GUIDE'}      =1 if  $swishdb=~ /guide/;
$INFO{'SYNOPS'}	    =1 if  $swishdb=~ /synops/;
$INFO{'USENET'}     =1 if  $swishdb=~ /usenet/;
$INFO{'GENIE'}      =1 if  $swishdb=~ /genie/;
$INFO{'COMPUSERVE'} =1 if  $swishdb=~ /compuserve/;

if ($INFO{'ALLJMS'}) {
	$INFO{'JMSANSWERS'} =1;
	$INFO{'USENET'} =1;
	$INFO{'GENIE'} =1;
	$INFO{'COMPUSERVE'} =1;
}
$swishdb = "";
$swishdb.= "/answers"  	 if $INFO{"JMSANSWERS"};
$swishdb.= "/guide"      if $INFO{"GUIDE"};
$swishdb.= "/synops"     if $INFO{"SYNOPS"};
$swishdb.= "/usenet"     if $INFO{"USENET"};
$swishdb.= "/genie"      if $INFO{"GENIE"};
$swishdb.= "/compuserve" if $INFO{"COMPUSERVE"};


$myurl = "http://$server$scriptname$swishdb";

#
# Process request
#
if ( ! $searchkey ) {
    &frontpage();
} else {
    &dosearch();
}

exit;

sub form_at {
	local($form, *data) = @_;
	local($key);
	$form =~ s/\r\n$//;
	$form =~ s/\r$//;
	$form =~ s/\n$//;
	foreach(split(/&/, $form)) {
		($key, $_) = split(/=/, $_, 2);
		$_    =~ s/\+/ /g;				 # + -> space
		$key  =~ s/\+/ /g;
		$_    =~ s/%([0-9A-Fa-f]{1,2})/pack(C,hex($1))/eig;  # undo %nn
		$key  =~ s/%([0-9A-Fa-f]{1,2})/pack(C,hex($1))/eig;  # escapes
		$key  =~ tr/a-z/A-Z/;				 # key in UPPER
		$data{$key} = $_;
	}
}



#
# Return standard search form
#
sub frontpage {
    print "Content-type: text/html\n\n";
	
    $JMSCHECKED 	= "CHECKED" if $swishdb=~ /answer/;
    $GUIDECHECKED	= "CHECKED" if $swishdb=~ /guide/;
    $SYNOPSCHECKED	= "CHECKED" if $swishdb=~ /synops/;
    $USENETCHECKED	= "CHECKED" if $swishdb=~ /usenet/;
    $GENIECHECKED	= "CHECKED" if $swishdb=~ /genie/;
    $COMPUCHECKED	= "CHECKED" if $swishdb=~ /compuserve/;

    print <<EOM;
<html><head>
<title>Search the Lurker\'s Guide to Babylon 5</title>
<base href="http://$server$scriptname">
</head>
<body>
$BANNER
<H1>Search the Lurker's Guide</H1>
<P>
<FORM ACTION="$myurl" METHOD=GET>
Enter words to search for: <INPUT NAME="KEY" SIZE=25>
<input type=submit name=go value="Search">
<P>
Select areas to search:
<DL>
<DL COMPACT>
<DT><INPUT NAME="ALLJMS" VALUE=1 TYPE=CHECKBOX>
<DD>All of the jms posting archives - or select individual archives below:

<DL COMPACT>
<DT><INPUT NAME="JMSANSWERS" VALUE=1 TYPE=CHECKBOX $JMSCHECKED>
<DD>The jms answers file - collected highlights from jms postings put together by Dan Wood.
<DT><INPUT NAME="USENET" VALUE=1 TYPE=CHECKBOX $USENETCHECKED>
<DD>The Usenet postings archive
<DT><INPUT NAME="GENIE" VALUE=1 TYPE=CHECKBOX $GENIECHECKED>
<DD>The GEnie messages archive
<DT><INPUT NAME="COMPUSERVE" VALUE=1 TYPE=CHECKBOX $COMPUCHECKED>
<DD>The CompuServe messages archive
</DL>
<DT><INPUT NAME="GUIDE" VALUE=1 TYPE=CHECKBOX $GUIDECHECKED>
<DD>The Episode Guide pages - fan written analysis of each episode
<DT>
<INPUT NAME="SYNOPS" VALUE=1 TYPE=CHECKBOX $SYNOPSCHECKED> 
<DD>The Episode Synopses- detailed fan written summaries of each episode<BR>
</DL>
<P>
<DD>Highlight keywords in documents?  
<INPUT NAME="BOLD" VALUE=1 TYPE=RADIO> Yes
<INPUT NAME="BOLD" VALUE=0 TYPE=RADIO CHECKED> No
<DD>Search only titles and headlines?
<INPUT NAME="THE" VALUE=1 TYPE=RADIO> Yes
<INPUT NAME="THE" VALUE=0 TYPE=RADIO CHECKED> No
</DL>
</FORM>
<P>
<HR>
<H2>Hints on searching</H2>
<P>
You may combine search words with <b>OR</b>, <b>AND</b> and <b>NOT</b>.
<b>AND</b>-expressions are evaluated before <b>OR</b>-expressions.  Words are
<b>AND</b>ed togeth by default.  Hence <kbd>sinclair duck</kbd> is equivalent 
to <kbd>sinclair and duck</kbd>.
<p>
For instance the search expression <kbd>londo AND g'kar OR delenn</kbd> will
find all documents containing both Londo and G'Kar, plus all
documents containing Delenn.
You may also write this as <kbd>( Londo AND g'kar ) OR Delenn</kbd>.
<p>
The search expression <kbd>Londo OR Sheridan AND NOT G'Kar</kbd> will find all\n";
documents containing Sheridan and that does not contain G'Kar, plus all
documents containing Londo.
You may also write this as <kbd>Londo OR ( Sheridan AND (NOT G'Kar) )</kbd>.
<p>
You can search for partial words by truncating them with *, for
instance <kbd>Vorl*</kbd> will match all words beginning with Vorl.
Note that this is also useful for getting plurals and posessives of
words, so <kbd>sinclair</kbd> will not find <em>sinclair's duck</em> nor
will it uncover <em>the sinclairs have always been pilots</em>. But 
<kbd>sinclair*</kbd> will find both.
</body>
</html>
EOM
}

#
# Search and make a html doc of the results
#
sub dosearch {
    # Remove the leading slash from pathinfo
    $swishdb= substr($swishdb, 1);

    print "Content-type: text/html\n\n";
    print <<EOM;
<html><head>
<title>Search for "$printkey" in the Lurker\'s Guide</title>
<base href="http://$server$scriptname">
</head>
<body>
$BANNER
<h2>Search</h2>
<B>in  $swishdb</B>
<P>
You may search the selected databases with a new search expression.
<P>
<form method=GET action="$myurl"><b>Search for:</b>
<input type=text name=key size=40> <input type=submit name=go value="Search">
<BR>
Highlight keywords in documents?  
<INPUT NAME="BOLD" VALUE=1 TYPE=RADIO> Yes
<INPUT NAME="BOLD" VALUE=0 TYPE=RADIO CHECKED> No
<BR>
Search only titles and headlines?
<INPUT NAME="THE" VALUE=1 TYPE=RADIO> Yes
<INPUT NAME="THE" VALUE=0 TYPE=RADIO CHECKED> No
<P>
</form>
<HR>
<h2>Search Results for \"$printkey\"</H2>
EOM

    if (! $swishdb) {
	print "<B>Sorry - you must select at least one database to search</B>";
	return;
    }


    push(@dblist, "-f");
    foreach $db ( split(/\//, $swishdb ) ) {
	push(@dblist, "$swishdbdir/$db.swish");
    }

    # Search only in title, headers and emphasized text.
    push(@dblist, "-t", "the") if $INFO{'THE'};

    push(@dblist, "-m");
    push(@dblist, $maxout);

    # The search itself
    # opening the pipe this way prevents the args from being evaluated
    # by the shell - the args can't be hacked as easily.
    open(SWISHSEARCH, "-|") || 
	exec($SWISHBIN, @dblist, "-w", $searchkey);
    @docs = <SWISHSEARCH>;
    close SWISHSEARCH;

    # No docs found
    if ( $#docs <= 0 ) {
	print "<h2>No documents found. Try repeat the search with a\n";
	print "different search expression.</h2>\n";
    	print "</body>\n</html>\n";
	return;
    }

    # Print results
    print "<ul>\n";
    foreach (@docs) {
	/# Name: (.*)/ && ( $NAME= $1 );
	/# Description: (.*)/ && ( print "</UL>\n<H2>$NAME</H2>$1\n<UL>\n");
	/^([0-9]+) (.+) \"([^\"]*)\" ([0-9]+)/ && (&print_url($1, $2, $3, $4));
 	/^err\:(.*)/ && ( print "<LI> <b>$1</b><br>\n");
    }

    print "</ul>\n";
}

sub print_url {
  local($SCORE, $URL, $NAME, $SIZE) = @_;
  # URLify the searchkey
  $searchkey=~s/\*//g;
  $searchkey=~s/ /+/g;
  if (($INFO{'BOLD'}) && (($URL =~ /$REMOVE/))) {
	$URL=~s!$REMOVE!$PREFIX!;
	print "<LI> <A HREF=\"$PRINTURL$URL?$searchkey#first_hit\">$NAME</A><BR>\n";
  } else {
  	print "<LI> <A HREF=\"$URL\">$NAME</A><BR>\n";
  }
  print "Score: $SCORE  Size: $SIZE\n";
}


#
# Split a line with fieldnames and values into an associative array
#
sub get_fields {
    local ( $inl, *outarr ) = @_;

    @fields = split(/\|/, $inl);

    foreach $f ( @fields ) {
	$eq = index($f, "=");
	$fname = substr($f, 0, $eq);
	$fvalue = substr($f, $eq+1);
	$fvalue =~ s/\/BAR_CODE\//\|/g;

	$outarr{$fname} = $fvalue;
    }
}

#
# Unescape an URL
#
sub unescape {
    local ($in) = @_;

    # Any +'es -> space
    $in =~ s/\+/ /g;

    # Hex values
    for ($start = 0; $start>=0;) {
	$start = index($in, "%", $start);
	if ( $start >= 0 ) {
	    $ch = hex(substr($in, $start+1, 2));
	    substr($in, $start, 3) = sprintf("%c", $ch);
    	    $start++;
	}
    }

    return $in;
}	








