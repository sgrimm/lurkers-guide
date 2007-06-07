/*
** Copyright (C) 1995, Enterprise Integration Technologies Corp.        
** All Rights Reserved.
** Kevin Hughes, kevinh@eit.com 
** 3/11/94
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef NEXTSTEP
#include <sys/dir.h>
#else
#include <dirent.h>
#endif
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <setjmp.h>

#define VERSION "1.1.1"
#define INDEXFILE "index.swish"
#define INDEXHEADER "# SWISH format 1.1"

#define MAXFILELEN 1000
#define MAXSTRLEN 1000
#define MAXWORDLEN 1000
#define MAXTITLELEN 200
#define MAXSUFFIXLEN 10
#define MAXENTLEN 10
#define HASHSIZE 101
#define BIGHASHSIZE 1009

#define TI_OPEN 1
#define TI_CLOSE 2
#define TI_FOUND 4
#define NOWORD "thisisnotaword"
#define SECSPERMIN 60

#define NO_RULE 0
#define AND_RULE 1
#define OR_RULE 2
#define NOT_RULE 3

#define IN_FILE 1
#define IN_TITLE 2
#define IN_HEAD 4
#define IN_BODY 8
#define IN_COMMENTS 16
#define IN_HEADER 32
#define IN_EMPHASIZED 64
#define IN_ALL 127

#define MAXLONGLEN 16
#define MAXCHARS 68
#define MAXHEADCHARS MAXLONGLEN * MAXCHARS
#define STOPWORDPOS MAXCHARS - 3
#define FILELISTPOS MAXCHARS - 2
#define FILEOFFSETPOS MAXCHARS - 1

#include "config.h"

struct sortresult {
        char *fileinfo;
        int rank;
        struct sortresult *left;
        struct sortresult *right;
};

struct result {
        int filenum;
        int rank;
	int structure;
        struct result *next;
};

struct file {
        char *filename;
        char *title;
        int size;
        struct file *next;
};

struct filenum {
	int fileshort;
	long filelong;
	struct filenum *next;
};

struct location {
        int filenum;
        int frequency;
        int emphasized;
	int structure;
        struct location *next;
};

struct entry {
        char *word;
        int tfrequency;
        struct location *locationlist;
        struct entry *left;
        struct entry *right;
};

struct sortentry {
        char *filename;
        char *title;
        struct sortentry *left;
        struct sortentry *right;
};

struct swline {
        char *line;
        struct swline *next;
};

struct fwordtotal {
        int filenum;
        int totalwords;
        struct fwordtotal *next;
};

#ifndef MAIN_FILE
#define VAR extern
#else
#define VAR
#endif

VAR struct file *filelist;
VAR struct entry *entrylist;

VAR struct swline *pathconlist;
VAR struct swline *dirconlist;
VAR struct swline *fileconlist;
VAR struct swline *titconlist;
VAR struct swline *fileislist;
VAR struct swline *suffixlist;
VAR struct swline *nocontentslist;
VAR struct swline *replacelist;
VAR struct swline *searchwordlist;
VAR struct swline *dirlist;
VAR struct swline *indexlist;
VAR struct swline *hashstoplist[HASHSIZE];

VAR struct result *resulthashlist[HASHSIZE];
VAR struct fwordtotal *fwordtotals[BIGHASHSIZE];
VAR struct filenum *filehashlist[BIGHASHSIZE];

VAR long offsets[MAXCHARS];
VAR int verbose;
VAR int bigrank;
VAR int maxhits;
VAR int totalwords;
VAR int followsymlinks;
VAR int commonerror;

VAR char indexn[MAXSTRLEN];
VAR char indexd[MAXSTRLEN];
VAR char indexp[MAXSTRLEN];
VAR char indexa[MAXSTRLEN];
VAR char errorstr[MAXSTRLEN];

#ifdef MAIN_FILE

char *indexchars = "abcdefghijklmnopqrstuvwxyz&#;0123456789_\\|/-+=?!@$%^'\"`~,.<>[]{}";

char *defaultstopwords[] = {
"a", "about", "above", "according", "across", "actually", "adj", "after", 
"afterwards", "again", "against", "all", "almost", "alone", "along", 
"already", "also", "although", "always", "among", "amongst", "an", "and", 
"another", "any", "anyhow", "anyone", "anything", "anywhere", "are", "aren", 
"aren't", "around", "as", "at", "be", "became", "because", "become", "becomes", 
"becoming", "been", "before", "beforehand", "begin", "beginning", "behind", 
"being", "below", "beside", "besides", "between", "beyond", "billion", "both", 
"but", "by", "can", "can't", "cannot", "caption", "co", "could", "couldn",
"couldn't", "did", "didn", "didn't", "do", "does", "doesn", "doesn't", "don",
"don't", "down", "during", "each", "eg", "eight", "eighty", "either", "else",
"elsewhere", "end", "ending", "enough", "etc", "even", "ever", "every",
"everyone", "everything", "everywhere", "except", "few", "fifty", "first",
"five", "for", "former", "formerly", "forty", "found", "four", "from",
"further", "had", "has", "hasn", "hasn't", "have", "haven", "haven't",
"he", "hence", "her", "here", "hereafter", "hereby", "herein", "hereupon", 
"hers", "herself", "him", "himself", "his", "how", "however", "hundred", 
"ie", "i.e.", "if", "in", "inc", "inc.", "indeed", "instead", "into", "is",
"isn", "isn't", "it", "its", "itself", "last", "later", "latter", "latterly",
"least", "less", "let", "like", "likely", "ll", "ltd", "made", "make",
"makes", "many", "maybe", "me", "meantime", "meanwhile", "might", "million",
"miss", "more", "moreover", "most", "mostly", "mr", "mrs", "much", "must",
"my", "myself", "namely", "neither", "never", "nevertheless", "next", "nine",
"ninety", "no", "nobody", "none", "nonetheless", "noone", "nor", "not",
"nothing", "now", "nowhere", "of", "off", "often", "on", "once", "one",
"only", "onto", "or", "other", "others", "otherwise", "our", "ours",
"ourselves", "out", "over", "overall", "own", "per", "perhaps", "rather",
"re", "recent", "recently", "same", "seem", "seemed", "seeming", "seems",
"seven", "seventy", "several", "she", "should", "shouldn", "shouldn't",
"since", "six", "sixty", "so", "some", "somehow", "someone", "something",
"sometime", "sometimes", "somewhere", "still", "stop", "such", "taking",
"ten", "than", "that", "the", "their", "them", "themselves", "then",
"thence", "there", "thereafter", "thereby", "therefore", "therein",
"thereupon", "these", "they", "thirty", "this", "those", "though",
"thousand", "three", "through", "throughout", "thru", "thus", "to",
"together", "too", "toward", "towards", "trillion", "twenty", "two", "under",
"unless", "unlike", "unlikely", "until", "up", "upon", "us", "used", "using",
"ve", "very", "via", "was", "wasn", "we", "we", "well", "were", "weren",
"weren't", "what", "whatever", "when", "whence", "whenever", "where", 
"whereafter", "whereas", "whereby", "wherein", "whereupon", "wherever", 
"whether", "which", "while", "whither", "who", "whoever", "whole", "whom", 
"whomever", "whose", "why", "will", "with", "within", "without", "won", 
"would", "wouldn", "wouldn't", "yes", "yet", "you", "your", "yours",
"yourself", "yourselves", NULL };

char *entities[] = {
"", "&#32", " ", "", "&#33", "!", "&quot", "&#34", "\"", "", "&#35", "#",
"", "&#36", "$", "", "&#37", "%", "&amp", "&#38", "&", "", "&#39", "'",
"", "&#43", "+", "", "&#44", ",", "", "&#45", "-", "", "&#46", ".",
"", "&#47", "/", "", "&#48", "0", "", "&#49", "1", "", "&#50", "2",
"", "&#51", "3", "", "&#52", "4", "", "&#53", "5", "", "&#54", "6",
"", "&#55", "7", "", "&#56", "8", "", "&#57", "9", "", "&#58", "",
"", "&#59", ";", "&lt", "&#60", "<", "", "&#61", "=", "&gt", "&#62", ">",
"", "&#63", "?", "", "&#64", "@", "", "&#65", "A", "", "&#66", "B",
"", "&#67", "C", "", "&#68", "D", "", "&#69", "E", "", "&#70", "F",
"", "&#71", "G", "", "&#72", "H", "", "&#73", "I", "", "&#74", "J",
"", "&#75", "K", "", "&#76", "L", "", "&#77", "M", "", "&#78", "N",
"", "&#79", "O", "", "&#80", "P", "", "&#81", "Q", "", "&#82", "R",
"", "&#83", "S", "", "&#84", "T", "", "&#85", "U", "", "&#86", "V",
"", "&#87", "W", "", "&#88", "X", "", "&#89", "Y", "", "&#90", "Z",
"", "&#91", "[", "", "&#92", "\\", "", "&#93", "]", "", "&#94", "^",
"", "&#95", "-", "", "&#96", "`", "", "&#97", "a", "", "&#98", "b",
"", "&#99", "c", "", "&#100", "d", "", "&#101", "e", "", "&#102", "f",
"", "&#103", "g", "", "&#104", "h", "", "&#105", "i", "", "&#106", "j",
"", "&#107", "k", "", "&#108", "l", "", "&#109", "m", "", "&#110", "n",
"", "&#111", "o", "", "&#112", "p", "", "&#113", "q", "", "&#114", "r",
"", "&#115", "s", "", "&#116", "t", "", "&#117", "u", "", "&#118", "v",
"", "&#119", "w", "", "&#120", "x", "", "&#121", "y", "", "&#122", "z",
"", "&#123", "{", "", "&#124", "|", "", "&#125", "}", "", "&#126", "~",
"&nbsp", "&#160", " ", "&iexcl", "&#161", "", "&cent", "&#162", "",
"&pound", "&#163", "", "&curren", "&#164", "", "&yen", "&#165", "",
"&brvbar", "&#166", "", "&sect", "&#167", "", "&die", "&#168", "",
"&copy", "&#169", "", "&ordf", "&#170", "", "&laquo", "&#171", "",
"&not", "&#172", "", "&shy", "&#173", "", "&reg", "&#174", "",
"&macron", "&#175", "", "&degree", "&#176", "", "&plusmn", "&#177", "",
"&sup2", "&#178", "", "&sup3", "&#179", "", "&acute", "&#180", "",
"&micro", "&#181", "", "&mu", "&#182", "", "&middot", "&#183", "",
"&Cedilla", "&#184", "", "&sup1", "&#185", "", "&ordm", "&#186", "",
"&raquo", "&#187", "", "&frac14", "&#188", "", "&frac12", "&#189", "",
"&frac34", "&#190", "", "&iquest", "&#191", "", "&Agrave", "&#192", "A",
"&Aacute", "&#193", "A", "&Acirc", "&#194", "A", "&Atilde", "&#195", "A",
"&Auml", "&#196", "A", "&Aring", "&#197", "A", "&AElig", "&#198", "A",
"&Ccedil", "&#199", "C", "&Egrave", "&#200", "E", "&Eacute", "&#201", "E",
"&Ecirc", "&#202", "E", "&Euml", "&#203", "E", "&Igrave", "&#204", "E",
"&Iacute", "&#205", "I", "&Icirc", "&#206", "I", "&Iuml", "&#207", "I",
"&ETH", "&#208", "", "&Ntilde", "&#209", "N", "&Ograve", "&#210", "O",
"&Oacute", "&#211", "O", "&Ocirc", "&#212", "O", "&Otilde", "&#213", "O",
"&Ouml", "&#214", "O", "&times", "&#215", "", "&Oslash", "&#216", "O",
"&Ugrave", "&#217", "U", "&Uacute", "&#218", "U", "&Ucirc", "&#219", "U",
"&Uuml", "&#220", "U", "&Yacute", "&#221", "Y", "&THORN", "&#222", "",
"&szlig", "&#223", "", "&agrave", "&#224", "a", "&aacute", "&#225", "a",
"&acirc", "&#226", "a", "&atilde", "&#227", "a", "&auml", "&#228", "a",
"&aring", "&#229", "a", "&aelig", "&#230", "a", "&ccedil", "&#231", "c",
"&egrave", "&#232", "e", "&eacute", "&#233", "e", "&ecirc", "&#234", "e",
"&euml", "&#235", "e", "&igrave", "&#236", "i", "&iacute", "&#237", "i",
"&icirc", "&#238", "i", "&iuml", "&#239", "i", "&eth", "&#240", "",
"&ntilde", "&#241", "n", "&ograve", "&#242", "o", "&oacute", "&#243", "o",
"&ocirc", "&#244", "o", "&otilde", "&#245", "o", "&ouml", "&#246", "o",
"&divide", "&#247", "", "&oslash", "&#248", "o", "&ugrave", "&#249", "u",
"&uacute", "&#250", "u", "&ucirc", "&#251", "u", "&uuml", "&#252", "u",
"&yacute", "&#253", "y", "&thorn", "&#254", "", "&yuml", "&#255", "y",
NULL };

#else
extern char *indexchars;
extern char *defaultstopwords[];
extern char *entities[];
#endif

#ifdef MAIN_FILE
int main();
void usage();
void printversion();
void printrunning();
long getthetime();
#endif
