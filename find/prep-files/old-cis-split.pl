#! /usr/local/bin/perl
# Splits usenet archives by article

# pull in directories...

eval(`cat splitter.cf`) || die("Config file error!");


# NEARLY CONSTS

$COUNT=0;
$FIRST=0;
$DATE="";
$OLDDATE="";
$SUBJECT="";
$OLDSUBJECT="";
$BODY="";
$QUESTION="";

die("Please supply filename for splitter input\nUsage:  splitter  jms-95-01\nproduces files jms-95-01/nn.html") if ! $ARGV[0];

foreach(@ARGV) {
   ($FNAME) = m!/([^/]+)$!;
   $FNAME= $_ if ! $FNAME;
   $NEWPATH= "$CISDIR/$FNAME";
   mkdir($NEWPATH, oct("777"))  || 
      print "Warning: Unable to create new directory $NEWPATH\n";

   open(INF, "< $_");
   while (<INF>) {
     # first skip to the first date

     s/\&/&amp;/g;
     s/\>/&gt;/g;
     s/\</&lt;/g;
     if (/^#:/) {
       $_ = <INF>;
       /^ *(.*)/;
       $DATE = $1;
       next;
     }

     if (/^Sb: #?[0-9]*-?#?<?(.*)>?/) {
       $SUBJECT = $1;
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
	  open(OUTFILE, ">$NEWPATH/$COUNT.html");
	  $FIRST=1;
       }
       next;
     }

     if (/^Subj: (.*) *Section: (.*)/) {
       $SUBJECT = $1;
       $SECTION = $2;
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
	  open(OUTFILE, ">$NEWPATH/$COUNT.html");
	  $FIRST=1;
	  $DATE="";
       }
       next;
     }
     if (/^  To: .*[, ]  *([^ ]*day, )?([A-Z].*[0-9] *[aApP][mM])/) {
	$DATE = $2;
	if (! $FIRST) {
	  # copied from ^From handling below - koreth
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
	  open(OUTFILE, ">$NEWPATH/$COUNT.html");
	  $FIRST=1;
	  $SUBJECT="";
	}
	next;
     }

     next if ! $SUBJECT;

     if (/^[~]*From:/) {
	  if ($FIRST) {
		  next;
	  }
     }
     if ($SUBJECT && $DATE && (/^ *$/)) {
	  if ($FIRST) {
	    print OUTFILE "<HTML>\n<HEAD>\n";
	    print OUTFILE "<TITLE>$SUBJECT ($DATE)</TITLE>\n";
	    print OUTFILE "</HEAD>\n<BODY>\n";
  	    print OUTFILE `cat $CISBANNER`
	          if -r $CISBANNER;
	print OUTFILE "- <A HREF=\"$PREVFILE\">Previous</A> " if $PREVFILE;
	print OUTFILE "- <A HREF=\"$NEXTFILE\">Next</A>\n<BR>\n" if $NEXTFILE;
	    print OUTFILE "<H2>$SUBJECT</H2>\n";
	    print OUTFILE "<h3>Section: $SECTION<br>Date: $DATE</h3>\n<pre>";
	    print "$FNAME File number $COUNT $SUBJECT\n";
	    $DATE="";
	    $FIRST= 0;
	  }
	  next;
     }
     print OUTFILE $_;
  }
  print OUTFILE "</PRE>\n</BODY>\n</HTML>\n" if $COUNT > 0;
  close(OUTFILE);
  close(INF);
}
print "\r\nDone\r\n";

