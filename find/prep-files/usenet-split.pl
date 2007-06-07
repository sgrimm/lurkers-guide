#! /usr/local/bin/perl
# Splits usenet archives by article

# pull in directories...

eval(`cat splitter.cf`) || die("Config file error!");


# NEARLY CONSTS


die("Please supply filename for splitter input\nUsage:  splitter  jms-95-01\nproduces files jms-95-01/nn.html") if ! $ARGV[0];

foreach(@ARGV) {
   $FIRST=0;
   $COUNT=0;
   $SKIPPING=0;
   $SKIPCOUNT=0;
   $DATE="";
   $OLDDATE="";
   $SUBJECT="";
   $OLDSUBJECT="";
   $BODY="";
   $QUESTION="";
   ($FNAME) = m!/([^/]+)$!;
   $FNAME= $_ if ! $FNAME;
   $NEWPATH= "$USENETDIR/$FNAME";
   mkdir($NEWPATH, oct("777"));

   open(INF, "< $_");
   while (<INF>) {
     # first skip to the first date

     s/\&/&amp;/g;
     s/\>/&gt;/g;
     s/\</&lt;/g;
     if (/^Date: ([0-9]+ [A-Za-z]+ [0-9]+)/) {
        $DATE= $1;
	do nextfile();
	next;
     }
     if (/^Subject: (.*)/) {
	$SUBJECT= $1;
	do nextfile();
	next;
     }

     next if ! $DATE;

     if ((/^From/)) {
	$FIRST=0;
	do nextfile();
	next;
     }
     if ($SUBJECT && $DATE && (/^ *$/)) {
	  if ($FIRST) {
	    print OUTFILE "<HTML>\n<HEAD>\n";
	    print OUTFILE "<TITLE>$SUBJECT ($DATE)</TITLE>\n";
	    print OUTFILE "</HEAD>\n<BODY>\n";
  	    print OUTFILE `cat $USENETBANNER`
	          if -r $USENETBANNER;
	  }
	  print OUTFILE "</PRE>\n" if (! $FIRST);
print OUTFILE "- <A HREF=\"$PREVFILE\">Previous</A> " if $FIRST && $PREVFILE;
print OUTFILE "- <A HREF=\"$NEXTFILE\">Next</A>\n<BR>\n" if $FIRST && $NEXTFILE;
	  print OUTFILE "<H2>$SUBJECT</H2>\n$DATE<PRE>";
	  print "$FNAME File number $COUNT $SUBJECT\n" if (! $SKIPPING);
	  $SUBJECT="";
	  $FIRST= 0;
	  next;
     }
     print OUTFILE $_;
  }
  close(OUTFILE);
  close(INF);
}
print "\r\nDone, skipped $SKIPCOUNT messages\r\n";


sub nextfile {
	if (! $FIRST) {
		if ($COUNT>0) {
			print OUTFILE "</PRE>\n</BODY>\n</HTML>\n";
			close OUTFILE;
		}
		$PREVFILE= "$COUNT.html" if $COUNT>0;
		$COUNT++;
		$NCOUNT= $COUNT + 1;
		$NEXTFILE= "$NCOUNT.html";
		$OLDDATE= $DATE;
		$OLDSUBJECT= $SUBJECT;
		$NEWFILENAME="$NEWPATH/$COUNT.html";
		if (-e $NEWFILENAME) {
			open(OUTFILE, ">/dev/null");
			$SKIPPING=1;
			$SKIPCOUNT++;
		}
		else {
			open(OUTFILE, ">$NEWPATH/$COUNT.html");
			$SKIPPING=0;
		}
		$FIRST=1;
	}
}
