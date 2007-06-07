#!/usr/local/bin/perl
#
# print_hit_bold.pl -- Display documents selected by a WAIS search interface.
#     This attempts to "BOLD" every string which matches one of the search
#     words. It skips over anything within <>'s so that it hopefully doesn't
#     screw up any html stuff. It also skips over "non-formatted" sections
#     (LISTING, XMP, PLAINTEXT) and over the TITLE section.
#     It presents text files within <PRE> ... </PRE> tags.
# Michael A. Grady, UIUC/CCSO June 1994 (m-grady@uiuc.edu)
# <A HREF="http://ewshp2.cso.uiuc.edu/">Mike Grady</A>
# modified on 7/12/94 to fix a bug with the "first_hit" anchor when the first
# term that gets bolded is the "text" for a link. Small change on 7/20/94 to
# exempt TITLE field from being bolded. Change on 7/21/94 to have file read
# work in line mode rather than paragraph mode -- paragraph mode appears to
# be "dangerous" with lots of the WWW type files (few blank lines in
# documents). Change on 8/15/94 to add BASE tag to front so relative URLs from
# this document DO NOT go back thru print_hit_bold.
# Change on 01/18/95 to fix occasional problem with placing of "first_hit"
# anchor. Added a "buffer" (variable $buffer_it) to hold the anchor/link text
# until we are sure we don't need to place the 'name="first_hit"' into it.

# expects a URL of the form: http:
# //your.www.server/cgibindir/print_hit_bold.pl/path/to/file?terms+to+hilite#first_hit
# where the "path/to/file" is relative to your document root directory

# Specify the url for this WWW server -- same as in "kidofwais.pl" script
#  (this is the same string you added when you waisindexed (using -t url))
$serverURL = "http://diglib.stanford.edu/";

# For DEBUG purposes, specify the fully-qualified name of a file in the
# logging directory for your server. Then if you turn on the $DEBUG flag,
# all the lines that this script sends to the HTTPD server will instead be
# sent into this logfile, except for the beginning "Content-type" lines and
# the pointer to the original document before it has been mucked with by this
# script. The match structure built by "prepare_matching_program()" is also
# written into the log (actually will precede the "real file").
$debugLOG = "/var/info/www/httpd/logs/cgi_log";
$DEBUG = 0;	# set to 1 to turn on debugging code

# Specify the "first hit anchor" to be used with hiliting,
# should match up with anchor value in "kidofwais.pl". See &hilite.
# Also need to add </A> to end of hit to end anchor.
$anchor = ' name="first_hit"';

# who maintains this service?
$maintainer = 'Christian Mogensen';

# you shouldn't have to edit anything below this line

sub send_file {
    print "Location: /$partial_name\n\n";
}


sub prepare_matching_program {
    # prepare the query word matching patterns (will use eval on this to
    # hopefully make it faster). Creates "$match_terms" and initializes
    # "$looking_for_first" to "on" (=1). This sets up the terms that we will
    # search and substitute for, adding BOLD tags around. The bold tags we
    # add are <B > ... </B >. <B > was chosen (with cap and space between B
    # and >) so as to be unlikely to be the same as any of the original BOLD
    # tags in the document. This enables us, on the "first match", to find
    # the position in the string of the "first match" and add the anchor tag
    # in front of that (so the document is positioned to the first hit when
    # presented to the user).
    local($searchterm);
    $looking_for_first = 1;
    $match_terms = "{study;\n";
    foreach $searchterm (@query) {
	# remove parens, quotes, words: and or not
	$searchterm =~ tr/\(\)\'\"//d;

	$searchterm =~ /^and$/i && next;
	$searchterm =~ /^or$/i  && next;
	$searchterm =~ /^not$/i && next;
	$searchterm =~ /^\s*$/  && next;

	$searchterm =~ /^.+\*$/ && $searchterm =~ tr/*//d &&
	  ($match_terms .=
	    "\$matched = 1 if s!\\b($searchterm)!<B >\$1</B >!ig;\n")
			&& next;
	#default case
	$searchterm =~ tr/*//d;
	$match_terms .=
	  "\$matched = 1 if s!\\b($searchterm)\\b!<B >\$1</B >!ig;\n";
    } #end foreach searchterm
    $match_terms .= "}";
    $DEBUG && print LOG "$match_terms\n";
    return;
}


sub hilite {
	# Search for the query words and place BOLD tags around each if found.
	# If the "first hit" in this document, put in the "first_hit" anchor.
	# Also need to add </A> to end of hit to end anchor.
	local($_) = @_;		# text to hilite
	local($position,$end_position);
	$matched = 0;
	eval ($match_terms);	# do the matching/replacing
	if ($looking_for_first && $matched) { # something matched for the 1st
		$looking_for_first = 0;	      # time, try to place an anchor
		$position = index( $_, "<B >" );
				# put anchor text into string if above found
		if ($position >= 0) {
		    # If text that has been bolded first is already within a
		    # <a> .., </a> construct, then we can't put another such
		    # construct around it. So instead stick the "name=" string
		    # into the <a ...> construct that is already there. If it
		    # already has a name= clause, not sure which will be
		    # recognized.
		    if ($buffer_it && $in_anchor) {
			$position = index( $buffer_it, ">" );
			substr($buffer_it, $position, 0) = $anchor;
		    } elsif (! $in_anchor) {
		    	substr($_, $position, 0) = "<A $anchor>";
		    	$end_position = index( $_, "</B >", $position);
		    	($end_position >= 0) &&
				(substr($_, $end_position + 5, 0) = "</A>");
		    }
		}
	}
	&print_it ( $_ );
	return;
}

sub print_it {	# This routine does the actual "printing" of the document
	# If we are in an anchor string, and have not yet found the
	# "first_hit", then save the text to be printed in a buffer. This will
	# allow us to put the 'name="first_hit"' into the anchor/link
	# construct itself if the first hit happens to be in the text that the
	# anchor/link is "around".

	local($to_print) = @_;
	if ($in_anchor && $looking_for_first) {
		$buffer_it .= $to_print;
	} else {
		print $buffer_it if $buffer_it;
		print $to_print;
		$buffer_it = '';
	}
}

sub process_file {

    # if 'PATH_INFO'/'PATH_TRANSLATED' has a non-null value, then 
    # 'PATH_TRANSLATED' should be the fully qualified path to the file to
    # to be hilited. If these environment variables are null, then someone
    # has constructed an incorrect url
    $partial_name = $ENV{'PATH_INFO'};
    $full_name    = $ENV{'PATH_TRANSLATED'};
    if ($partial_name eq "") {
    	print "Content-type: text/html\n\n";
	print "<HEAD><TITLE>Incorrect url specified</TITLE></HEAD>\n";
	print "<BODY><H2>Incorrect url specified</H2>\n";
	print "<p>";
	print "This script has been referenced by an incorrect url. Please ";
	print "contact $maintainer if you have any questions.<p>\n";
	print "<A HREF=\"$serverURL\">Main page for this server.</A></BODY>\n";
	return;
    }
    $partial_name = substr($partial_name,1); # remove beginning slash
    TYPE: for ($partial_name) {
	/.html$/	&& do { $type = 'html'; $ok = 1; last TYPE };
	/.txt$/		&& do { $type = 'text'; $ok = 1; last TYPE };
	/.TEXT$/	&& do { $type = 'text'; $ok = 1; last TYPE };
	$ok = 0;
    }

    do { &send_file; return; } unless defined @ARGV; # nothing to hilite
	# probably can't try to hilite without messing up file unless html/txt
    do { &send_file; return; } unless $ok;

    local(@query) = @ARGV;
    local($pquery) = join(" ", @query);
    # NCSA's HTTPD puts backslashes in front of "funny" or "dangerous"
    # characters in the input supplied thru argv. In the case of search terms
    # for WAIS, this can screw up the search (parens and "*" get backslashed
    # and then don't work correctly). So remove the backslashes, AND the
    # potentially "dangerous" characters ( ; ` ! ).
    $pquery =~ tr/!\;\`\\//d;           # just in case, get rid of ;`! and \
    @query = split(' ',$pquery);        # and recreate query word array

    # DEBUG code - write stuff into file
    $DEBUG && do { open (LOG, ">>$debugLOG") || die "can't open log";};

    &prepare_matching_program;  # create $match_terms string

    # just send file if there are no words left to hilite
    do { &send_file; return; } if ($match_terms eq "{study;\n}");

    # start the html document to "return"
    print "Content-type: text/html\n\n";
    print "<BASE HREF=\"$serverURL$partial_name\">\n";
    print "<A HREF=\"$serverURL$partial_name\">Get the original (un<b>BOLD</b>ed) document.</A><p>\n";

    $DEBUG && select (LOG); # write rest of stuff to file for DEBUG

    # try and open file for reading
    open (FP, "$full_name") || print "File $full_name can't be read. Please contact $maintainer." && return;

    if ($type eq 'text') {	# Start document for "text"
    	print "<HEAD><TITLE>File: $partial_name</TITLE></HEAD>\n";
	print "<BODY><p>\n<PRE>";
    }

    local($in_tag,$skip_till_endtag,$endtag,$line_left);
    $in_tag = 0;
    $skip_till_endtag = $in_anchor = 0;
    $endtag = $buffer_it = '';
    READ_LINE: while (<FP>) {
	$line_left = $_;
	PROCESS_LINE: for ($line_left) {

		($line_left eq "") && next READ_LINE;	# nothing left of this
							# paragraph
		$skip_till_endtag && $line_left =~ /$endtag/i && do {
			   $line_left = $';
			   &print_it ( "$`$endtag" );
			   $skip_till_endtag = 0;
			   $endtag = "";
			   redo PROCESS_LINE;
			};
		$skip_till_endtag && do {  # endtag must be in next paragraph
			   &print_it ( $line_left );
			   next READ_LINE;
			};
		! $in_tag && $line_left =~ /</  && do {
			   $line_left = $';
			   &hilite($`) if ($` ne "");
			   &print_it ( "<" );
			   if ($line_left =~
				 /^(PLAINTEXT|XMP|LISTING|TITLE)/i) {
				$line_left = $';
				$endtag = "</$1";
				&print_it ( $1 );
				$skip_till_endtag = 1;
			   } else {
				($line_left =~ /^\/A/i) && ($in_anchor = 0);
				($line_left =~ /^A\s+/i) && ($in_anchor = 1);
				$in_tag = 1;
			   }
			   redo PROCESS_LINE;
			};
		$in_tag   && $line_left =~ />/  && do {
			   $line_left = $';
			   &print_it ( "$`>" );
			   $in_tag = 0;
			   redo PROCESS_LINE;
			};
		$in_tag   && do {	# ending ">" must be in next paragraph
			   &print_it ( $line_left );
			   next READ_LINE;
			};
		&hilite($line_left);  # default case: hilite rest of paragraph
		next READ_LINE;
	} # end line_left foreach
    }
    print $buffer_it if $buffer_it; # print out "leftover stuff" if any
    close(FP);

    print "\n</PRE>\n</BODY>\n" if ($type eq 'text'); # end document for "text"
    return;
}

open (STDERR,"> /dev/null");
eval '&process_file';

