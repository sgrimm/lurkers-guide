#! /usr/local/bin/perl
# Splits GEnie archives by message...


# pull in directories...

eval(`cat splitter.cf`) || die("Config file error!");


# NEARLY CONSTS

$COUNT=0;
$CLOSED=1;
$INHEADER= 1;
$FIRST=0;
$DATE="";
$OLDTOPIC="";
$TOPIC="";
$SUBJECT="";
$BODY="";
$QUESTION="";

die("Please supply filenames for splitter\nUsage:  genie-split  ../jms-95-01 \nproduces files jms-95-01/nn.html\n") if ! $ARGV[0];


foreach(@ARGV) {
  ($FNAME) = m!/([^/]+)$!;
  $FNAME= $_ if ! $FNAME;
  $NEWPATH= "$GENIEDIR/$FNAME";
  mkdir($NEWPATH, oct("777"));

  open(INF, "< $_");
  while (<INF>) {
       #  ------------
       # SFRT II RoundTable
       # Category 18,  Topic 17
       # Message 525       Fri Jun 02, 1995
       # STRACZYNSKI [Joe]            at 23:55 EDT

       # first skip to the first message

       s/\&/&amp;/g;
       s/\>/&gt;/g;
       s/\</&lt;/g;
       if (/^Category \d+,  (Topic \d+)/) {
	   $TOPIC= $1;
	   next;
       }
       if (/^(Topic \d+) /) {
	   $TOPIC= $1;
	   next;
       }
       if (/^Message [0-9]+ +([A-Za-z ]+ [0-9]+, [0-9]+)/) {
	 $DATE= $1;
	 ($MM, $DD, $YY) = /^Message [0-9]+ +[A-Za-z]+ ([A-Za-z]+) ([0-9]+), ([0-9]+)/;
	 $MMYY= "$MM $DD $YY";
	 next;
       }
       next if /^SFRT II RoundTable/;
       next if /^STRACZYNSKI/;
       next if /^[A-Z ]+ \[/;
       if (/^ \*\*\*\*/) {
	 $INHEADER= 1;
       }
       if (/^ ----/ || /^ ____/) {
	 $INHEADER= 0;
       }
       next if $INHEADER;
   #   next if ! $DATE;

       if (/^ ----/ || /^ ____/) {
	    if ($COUNT>0) {
	       print OUTFILE "</PRE>\n</BODY>\n</HTML>\n";
	       close OUTFILE;
	    }
	    $CLOSED=1;
	    next;
      }
      if ($CLOSED) {
	    $CLOSED=0;
	    $PREVFILE= "$COUNT.html" if $COUNT>0;
	    $COUNT++;
	    $NCOUNT= $COUNT + 1;
	    $NEXTFILE= "$NCOUNT.html";
	    $OLDTOPIC= $TOPIC;
	    print "\r $FNAME File number $COUNT $TOPIC";     
	    open(OUTFILE, ">$NEWPATH/$COUNT.html");
	    print OUTFILE "<HEAD><TITLE>JMS Genie Message on $MMYY</TITLE></HEAD>\n";
	    print OUTFILE "<BODY>\n";
	    print OUTFILE `cat $GENIEBANNER`
		      if -r $GENIEBANNER;
	  print OUTFILE "- <A HREF=\"$PREVFILE\">Previous</A> " if $PREVFILE;
	  print OUTFILE "- <A HREF=\"$NEXTFILE\">Next</A>\n<BR>\n" if $NEXTFILE;
	    print OUTFILE "<PRE>\n";
	    next;
       }
       print OUTFILE $_;
  }
  close(OUTFILE);
  close(INF);
}
print "\r\nDone\r\n";

