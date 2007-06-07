/*
** Copyright (C) 1995, Enterprise Integration Technologies Corp.        
** All Rights Reserved.
** Kevin Hughes, kevinh@eit.com 
** 3/11/94
*/

#include "swish.h"
#include "check.h"

/* Check if a file with a particular suffix should be indexed
** according to the settings in the configuration file.
*/

int isoksuffix(filename, rulelist)
     char *filename;
     struct swline *rulelist;
{
	int badfile;
	char *c, suffix[MAXSUFFIXLEN], checksuffix[MAXSUFFIXLEN];
	struct swline *tmplist;

	tmplist = rulelist;
        if (tmplist == NULL)
                return 1;
	if ((c = (char *) strrchr(filename, '.')) == NULL)
		return 0;

	badfile = 1;
	strcpy(checksuffix, c + 1);
        while (tmplist != NULL) {
		if ((c = (char *) strrchr(tmplist->line, '.')) == NULL)
			strcpy(suffix, tmplist->line);
		else
			strcpy(suffix, c + 1);
                if (lstrstr(suffix, checksuffix) && strlen(suffix) ==
		strlen(checksuffix))
                        badfile = 0;
                tmplist = tmplist->next;
        }
	return !(badfile);
}

/* Check if a particular title should be ignored
** according to the settings in the configuration file.
*/

int isoktitle(title)
     char *title;
{
        int badfile;
        struct swline *tmplist;

        badfile = 0;
        tmplist = titconlist;
        while (tmplist != NULL) {
                if (lstrstr(title, tmplist->line)) {
                        badfile = 1;
                        break;
                }
                tmplist = tmplist->next;
        }
        if (badfile)
                return 0;
        else
                return 1;
}

/* Should a word be indexed? Consults the stopword hash list
** and checks if the word is of a reasonable length...
** If you have any good rules that can work with most languages,
** please let me know...
*/

int isokword(word)
      char *word;
{
        int i, same, hasnumber, hasvowel, hascons,
		numberrow, vowelrow, consrow;
	char lastchar;

	if (word[0] == '\0')
		return 0;

        if (isstopword(word))
                return 0;
        if (strlen(word) < MINWORDLIMIT || strlen(word) > MAXWORDLIMIT)
                return 0;

	lastchar = ':';
	same = 0;
	hasnumber = hasvowel = hascons = 0;
	numberrow = vowelrow = consrow = 0;
	for (i = 0; word[i] != '\0'; i++) {
		if (word[i] == lastchar) {
			same++;
			if (same > IGNORESAME)
				return 0;
		}
		else
			same = 0;
		if (isdigit(word[i])) {
			hasnumber = 1;
			numberrow++;
			if (numberrow > IGNOREROWN)
				return 0;
			vowelrow = 0;
			consrow = 0;
		}
		else if (isvowel(word[i])) {
			hasvowel = 1;
			vowelrow++;
			if (vowelrow > IGNOREROWV)
				return 0;
			numberrow = 0;
			consrow = 0;
		}
		else if (!ispunct(word[i])) {
			hascons = 1;
			consrow++;
			if (consrow > IGNOREROWC)
				return 0;
			numberrow = 0;
			vowelrow = 0;
		}
		lastchar = word[i];
	}

	if (IGNOREALLV)
		if (hasvowel && !hascons)
			return 0;
	if (IGNOREALLC)
		if (hascons && !hasvowel)
			return 0;
	if (IGNOREALLN)
		if (hasnumber && !hasvowel && !hascons)
			return 0;

	return 1;
}

/* Does a word have valid characters?
*/

int hasokchars(word)
     char *word;
{
	int i, j;
	char c;

	c = word[strlen(word) - 1];
	for (i = j = 0; BEGINCHARS[i] != '\0'; i++)
		if (word[0] == BEGINCHARS[i])
			j++;
	if (!j)
		return 0;
	for (i = j = 0; ENDCHARS[i] != '\0'; i++)
		if (c == ENDCHARS[i])
			j++;
	if (!j)
		return 0;
	for (i = 0; word[i] != '\0'; i++)
		for (j = 0; WORDCHARS[j] != '\0'; j++)
			if (word[i] == WORDCHARS[j])
				return 1;
	return 0;
}

/* Is a letter a vowel?
*/

int isvowel(c)
     char c;
{
	if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
		return 1;
	return 0;
}

/* This checks is a filename has one of the following suffixes:
** "htm", "HTM", "html", "HTML", "shtml", "SHTML".
*/

int ishtml(filename)
     char *filename;
{
	char *c, suffix[MAXSUFFIXLEN];

	c = (char *) strrchr(filename, '.');

	if (c == NULL)
		return 0;
	strcpy(suffix, c + 1);
	if (suffix[0] == '\0')
		return 0;

	if (!strncmp(suffix, "htm", 3))
		return 1;
	else if (!strncmp(suffix, "HTM", 3))
		return 1;
	else if (!strncmp(suffix, "shtml", 5))
		return 1;
	else if (!strncmp(suffix, "SHTML", 5))
		return 1;
	return 0;
}
