/*
** SWISH Default Configuration File
**
** Kevin Hughes, kevinh@eit.com 
** 3/11/94
**
** The following are user-definable options that you can change
** to fine-tune SWISH's default options.
*/

/* #define NEXTSTEP */

/* You may need to define this if compiling on a NeXTstep machine.
*/

#define INDEXPERMS 0644

/* After SWISH generates an index file, it changes the permissions
** of the file to this mode. Change to the mode you like
** (note that it must be an octal number). If you don't want
** permissions to be changed for you, comment out this line.
*/

#define PLIMIT 80
#define FLIMIT 256

/* SWISH uses these parameters to automatically mark words as
** being too common while indexing. For instance, if I defined PLIMIT
** as 80 and FLIMIT as 256, SWISH would define a common word as
** a word that occurs in over 80% of all indexed files and over
** 256 files. Making these numbers lower will most likely make your
** index files smaller. Making PLIMIT and FLIMIT small will also
** ensure that searching consumes only so much CPU resources.
*/

#define VERBOSE 0

/* You can define VERBOSE to be a number from 0 to 3. 0 is totally
** silent operation; 3 is very verbose.
*/

#define MAXHITS 40

/* MAXHITS is the maximum number of results to return from a search.
*/

#define DEFAULT_RULE AND_RULE

/* If a list of search words is specified without booleans,
** SWISH will assume they are connected by a default rule.
** This can be AND_RULE or OR_RULE.
*/

#define TITLETOPLINES 12

/* This is how many lines deep SWISH will look into an HTML file to
** attempt to find a <TITLE> tag.
*/

#define EMPHASIZECOMMENTS 0

/* Normally, words within HTML comments are not assigned a higher
** relevance rank. If you're including keywords in comments
** define this as 1 so matching results will rise to the top
** of search results.
*/

#define MINWORDLIMIT 3

/* This is the minimum length of a word. Anything shorter will not
** be indexed.
*/

#define MAXWORDLIMIT 30

/* This is the maximum length of a word. Anything longer will not
** be indexed.
*/

#define ASCIIENTITIES 1

/* If defined as 1, all entities in search words and indexed
** words will be converted to an ASCII equivalent. For instance,
** with this feature you can index the word "resum&eacute;" or
** "resum&#233;" and it will be indexed as the word "resume".
** If defined as 0, only numerical entities will be converted
** to named entities, if they exist.
*/

#define IGNOREALLV 1
#define IGNOREALLC 1
#define IGNOREALLN 1

/* If IGNOREALLV is 1, words containing all vowels won't be indexed.
** If IGNOREALLC is 1, words containing all consonants won't be indexed.
** If IGNOREALLN is 1, words containing all digits won't be indexed.
** Define as 0 to allow words with consistent characters.
** Vowels are defined as "aeiou", digits are "0123456789".
*/

#define IGNOREROWV 4
#define IGNOREROWC 5
#define IGNOREROWN 4

/* IGNOREROWV is the maximum number of consecutive vowels a word can have.
** IGNOREROWC is the maximum number of consecutive consonants a word can have.
** IGNOREROWN is the maximum number of consecutive digits a word can have.
** Vowels are defined as "aeiou", digits are "0123456789".
*/

#define IGNORESAME 3

/* IGNORESAME is the maximum times a character can repeat in a word.
*/

#define WORDCHARS "abcdefghijklmnopqrstuvwxyz&#;0123456789'-_"

/* ADDED: ' for G'Kar etc, - and _ for filenames */

/* WORDCHARS is a string of characters which SWISH permits to
** be in words. Any strings which do not include these characters
** will not be indexed. You can choose from any character in
** the following string:
**
** abcdefghijklmnopqrstuvwxyz&#;0123456789_\|/-+=?!@$%^'\"`~,.[]{}()
**
** Note that if you omit "0123456789&#;" you will not be able to
** index HTML entities. DO NOT use the asterisk (*), lesser than
** and greater than signs (<), (>), or colon (:).
**
** Including any of these four characters may cause funny things to happen.
** If you have a pressing need to index 8-bit characters, please contact
** me for possible user testing in the future.
**
** Also note that if you specify the backslash character (\) or
** double quote (") you need to type a backslash before them to
** make the compiler understand them.
*/

#define BEGINCHARS "abcdefghijklmnopqrstuvwxyz&"

/* Of the characters that you decide can go into words, this is
** a list of characters that words can begin with. It should be
** a subset of (or equal to) WORDCHARS.
*/

#define ENDCHARS "abcdefghijklmnopqrstuvwxyz;"

/* This is the same as BEGINCHARS, except you're testing for
** valid characters at the ends of words.
*/

/* Note that if you really want to edit the default stopwords, (words
** that are deemed too common to be indexed) then you can do so in the
** file "swish.h". They don't have to be in alphabetical order.
*/

#define INDEXTAGS 0

/* Normally, all data in tags in HTML files (except for words in
** comments) is ignored. If you want to index HTML files with the
** text within tags and all, define this to be 1 and not 0.
*/
