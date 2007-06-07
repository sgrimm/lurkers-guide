#! /usr/local/bin/perl
# Splits the JMS answer file up


eval(`cat splitter.cf`) || die("Config file error!");

$BANNER= `cat $ANSWERBANNER`;
$COUNT=1;
$START=1;
$KEYS="";
$BODY="";
$QUESTION="";

while (<>) {
   $_=""    if /^=======/;
   if (($START==0) && (/^====/)) {
     $COUNT++;
	print "\r File number $COUNT                           ";
	print "\r File number $COUNT $QUESTION";
     do save_it($COUNT,$KEYS,$QUESTION,$BODY);
     $KEYS="";
     $BODY="";
     $QUESTION="";
   }
   $START=0;
   $KEYS=$1 if /^==== +(.*)/;
   $_=""    if /^====/;
   
   $QUESTION=$1 if (! $QUESTION) && (/^\?\?\?\? (.*)/);
   $_ = "<H2>$QUESTION</H2>\n" if (/^\?\?\?\? (.*)/);

   $_ = "" if $START;
   $_ = "<P><HR><P>\n" if /^---/;
   $_ = "<P>\n" if /^ *$/;

   $BODY.= $_;
}

print "\r\nDone\r\n";

sub save_it {
   local($COUNT, $KEYS, $QUESTION, $BODY) = @_;
     $KEYS =~ tr/\~/ /;
     $QUESTION= $KEYS if !$QUESTION;
     open(OUT, ">$ANSWERDIR/answ$COUNT.html");
     print OUT <<EOM;
<HTML>
<HEAD><TITLE>$QUESTION</TITLE>
</HEAD><BODY>
$BANNER
Keywords: <EM>$KEYS</EM>
$BODY
</BODY>
</HTML>
EOM
     close(OUT);
}