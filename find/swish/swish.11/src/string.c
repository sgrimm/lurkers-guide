/*
** Copyright (C) 1995, Enterprise Integration Technologies Corp.        
** All Rights Reserved.
** Kevin Hughes, kevinh@eit.com 
** 3/11/94
*/

#include "swish.h"
#include "string.h"

/* My own case-insensitive strstr().
*/

char *lstrstr(s, t)
     char *s;
     char *t;
{

	int i, j, k, l;

	for (i = 0; s[i]; i++) {
		for (j = 0, l = k = i; s[k] && t[j] &&
		tolower(s[k]) == tolower(t[j]); j++, k++)
			;
		if (t[j] == '\0')
			return s + l;
	}
	return NULL;
}

/* Gets the next word in a line. If the word's in quotes,
** include blank spaces in the word or phrase.
*/

char *getword(line, skiplen)
     char *line;
     int *skiplen;
{
        int i, inquotes;
        char *start;
        static char word[MAXSTRLEN];

        start = line;
        if (!(*line))
                return "\0";
        while (isspace(*line))
                line++;
        if (!(*line))
                return "\0";
        if (*line == '\"') {
                inquotes = 1;
                line++;
        }
        else
                inquotes = 0;
        for (i = 0; *line && i < MAXSTRLEN &&
        ((inquotes) ? (*line != '\"') : (!isspace(*line))); line++)
                word[i++] = *line;
        word[i] = '\0';
        if (!(*line))
                return "\0";
        if (*line == '\"')
                line++;

        *skiplen = line - start;

        return word;
}

/* Gets the value of a variable in a line of the configuration file.
** Basically, anything in quotes or an argument to a variable.
*/

char *getconfvalue(line, var, value)
     char *line;
     char *var;
     char *value;
{
        int i;
        char *c;
        static char tmpvalue[MAXSTRLEN];

        if ((c = (char *) lstrstr(line, var)) != NULL) {
                if (c != line)
                        return NULL;
                c += strlen(var);
                while (isspace(*c) || *c == '\"')
                        c++;
                if (*c == '\0')
                        return NULL;
                for (i = 0; *c != '\0' && *c != '\"' && *c != '\n' &&
                i < MAXSTRLEN; c++)
                        tmpvalue[i++] = *c;
                tmpvalue[i] = '\0';
                strcpy(value, tmpvalue);
                return tmpvalue;
        }
        else
                return NULL;
}

/* Extracts anything in <title> tags from an HTML file and returns it.
** Otherwise, only the file name without its path is returned.
*/

char *parsetitle(filename)
     char *filename;
{
        register int c, d;
	register char *p;
	char *tag;
        static char title[MAXTITLELEN], shorttitle[MAXTITLELEN];
        int i, j, lines, status, tagbuflen, totaltaglen;
        FILE *fp;

	tag = (char *) emalloc(1);
	tag[0] = '\0';
        lines = status = 0;
        p = title;
        *p = '\0';

        if (strrchr(filename, '/'))
                strcpy(shorttitle, strrchr(filename, '/') + 1);
	else
                strcpy(shorttitle, filename);

        fp = fopen(filename, "r");
        if (fp == NULL) {
		free(tag);
                return shorttitle;
	}
        for (; lines < TITLETOPLINES ; ) {
                c = getc(fp);
                if (c == '\n')
                        lines++;
                if (feof(fp)) {
                        fclose(fp);
			free(tag);
                        return shorttitle;
                }
                switch(c) {
                case '<':

			free(tag);
			tag = (char *) emalloc(MAXSTRLEN);
			tag[0] = '\0';
			tagbuflen = totaltaglen = 0;

			tag[totaltaglen++] = '<';
			tagbuflen++;
                        status = TI_OPEN;

			while (1) {
                                d = getc(fp);
                                if (d == EOF) {
					fclose(fp);
					free(tag);
					return shorttitle;
				}
				else if (d == '>') {
					tag[totaltaglen] = '\0';
					tag = (char *) erealloc(tag,
					totaltaglen + MAXSTRLEN);
					sprintf(tag, "%s%c", tag, '>');
					break;
				}

				tag[totaltaglen++] = d;
				tagbuflen++;
				if (tagbuflen >= MAXSTRLEN) {
					tag = (char *) erealloc(tag,
					totaltaglen + MAXSTRLEN);
					tagbuflen = 0;
				}
			}

                        if (lstrstr(tag, "</title>")) {
                                status = TI_CLOSE;
                                *p = '\0';
                                fclose(fp);
                                for (i = 0; title[i]; i++)
                                        if (title[i] == '\n')
                                                title[i] = ' ';
/*				for (i = 0; isspace(title[i]) 
				     || title[i] == '\"' 
				     ; i++)
					;*/
				for (j = 0; title[i]; j++)
					title[j] = title[i++];
				for (j = strlen(title) - 1;
				(j && isspace(title[j]))
				|| title[j] == '\0' 
				     /* || title[j] == '\"' */
				     ; j--)
					title[j] = '\0';
				for (j = 0; title[j]; j++)
					if (title[j] == '\"')
						title[j] = '\'';
				free(tag);
                                return title;
                        }
                        else {
                                if (lstrstr(tag, "<title>"))
                                        status = TI_FOUND;
                        }
                        break;
                default:
                        if (status == TI_FOUND) {
                                *p = c;
                                p++;
                        }
                        else {
                                if (status == TI_CLOSE) {
                                        fclose(fp);
					free(tag);
                                        return shorttitle;
                                }
                        }
                }
        }
        fclose(fp);
	free(tag);
        return shorttitle;
}

/* Malloc()'s a string, returns it.
*/

char *mystrdup(s)
     char *s;
{
        char *p;

        p = (char *) emalloc(strlen(s) + 1);
        strcpy(p, s);
        return p;
}

/* Is a character a valid word character?
*/

int iswordchar(c)
     char c;
{
	int d, i;

	d = tolower(c);
	for (i = 0; WORDCHARS[i] != '\0'; i++)
		if (d == WORDCHARS[i])
			return 1;
        return 0;
}

/* In a string, replaces all occurrences of "oldpiece" with "newpiece".
** This is not really bulletproof yet.
*/

char *replace(string, oldpiece, newpiece)
     char *string;
     char *oldpiece;
     char *newpiece;
{
        int i, j, limit;
        char *c;
        char beforestring[MAXSTRLEN], afterstring[MAXSTRLEN];
        static char newstring[MAXSTRLEN];

        if ((c = (char *) strstr(string, oldpiece)) == NULL)
                return string;
        limit = c - string;

        for (i = 0; i < limit; i++)
                beforestring[i] = string[i];
        beforestring[i] = '\0';

        i += strlen(oldpiece);

        for (j = 0; string[i] != '\0'; i++)
                afterstring[j++] = string[i];
        afterstring[j] = '\0';

        sprintf(newstring, "%s%s%s", beforestring, newpiece, afterstring);

        while (strstr(newstring, oldpiece))
                strcpy(newstring, replace(newstring, oldpiece, newpiece));

        return newstring;
}

/* Like strcmp(), but the order of sorting the first char is
** determined by the order of the characters in the wordchars array.
*/

int wordcompare(s1, s2)
     char *s1;
     char *s2;
{
	register int i, j;

	if (s1[0] != s2[0]) {
		for (i = 0; WORDCHARS[i] != '\0'; i++)
			if (s1[0] == WORDCHARS[i])
				break;
		for (j = 0; WORDCHARS[j] != '\0'; j++)
			if (s2[0] == WORDCHARS[j])
				break;
		if (i < j)
			return -1;
		else
			return 1;
	}
	else
		return strcmp(s1, s2);
}

/* This converts HTML numbered entities (such as &#169;)
** to strings (like &copy;). Much is this function is
** simply adding semicolons in the right places.
** This and the functions it calls are not very fast
** and could be made faster.
*/

char *convertentities(s)
     char *s;
{
	int i, skip;
	char ent[MAXENTLEN];
	static char newword[MAXWORDLEN];

	if ((char *) strchr(s, '&') == NULL)
		return s;
	if (strlen(s) > MAXWORDLIMIT)
		return s;

	for (i = 0; i < MAXWORDLEN; i++)
		newword[i] = '\0';
	for (; *s != '\0'; s++) {
		if (*s == '&') {
			strcpy(ent, getent(s, &skip));
			if (ent[0] == '\0') {
				sprintf(newword, "%s&", newword);
				continue;
			}
			s += skip;
			if (*s == ';')
				s++;
			sprintf(newword, "%s%s;", newword, ent);
			s--;
		}
		else
			sprintf(newword, "%s%c", newword, *s);
	}

	strcpy(newword, (char *) converttonamed(newword));
	if (ASCIIENTITIES)
		strcpy(newword, (char *) converttoascii(newword));

	return newword;
}

/* Returns a matching entity that matches the beginning of a string, if any.
*/

char *getent(s, skip)
     char *s;
     int *skip;
{
	int i;
	static char ent[MAXENTLEN], testent[MAXENTLEN];

	*skip = 0;
	strncpy(ent, s, MAXENTLEN);
	if (ent[1] == '#') {
		if (isdigit(ent[5]))
			return "\0";
		for (i = 2; ent[i] != '\0' && isdigit(ent[i]); i++)
			;
		while (ent[i] != '\0' && !isdigit(ent[i]))
			ent[i++] = '\0';
		*skip = strlen(ent);
		return ent;
	}
	else {
		for (i = 0; entities[i] != NULL; i += 3) {
			strcpy(testent, entities[i]);
			if (testent[0] != '\0') {
				if (!strncmp(testent, ent, strlen(testent))) {
					strcpy(ent, testent);
					*skip = strlen(ent);
					return ent;
				}
			}
		}
	}

	return "\0";
}

/* This is the real function called by convertentities() that
** changes numbered to named entities.
*/

char *converttonamed(s)
     char *s;
{
	int i;
	char testent[MAXENTLEN], newent[MAXENTLEN];
	static char newword[MAXWORDLEN];

	strcpy(newword, s);
	for (i = 0; entities[i] != NULL; i += 3) {
		sprintf(testent, "%s;", entities[i + 1]);
		if (strstr(newword, testent) != NULL &&
		(entities[i])[0] != '\0') {
			sprintf(newent, "%s;", entities[i]);
			strcpy(newword, (char *) replace(newword,
			testent, newent));
		}
	}

	while (hasnumbered(newword))
		strcpy(newword, (char *) converttonamed(newword));

	return newword;
}

/* Does a string still contain numbered entities that can be converted?
*/

int hasnumbered(s)
     char *s;
{
	int i;
	char testent[MAXENTLEN];

	for (i = 0; entities[i] != NULL; i += 3) {
		sprintf(testent, "%s;", entities[i + 1]);
		if (strstr(s, testent) != NULL && (entities[i])[0] != '\0')
			return 1;
	}
	return 0;
}

/* This function converts all convertable named and numbered
** entities to their ASCII equivalents, if they exist.
*/

char *converttoascii(s)
     char *s;
{
	int i;
	char *c, *d, wrdent[MAXENTLEN], nument[MAXENTLEN];
	static char newword[MAXWORDLEN];

	strcpy(newword, s);
	for (i = 0; entities[i] != NULL; i += 3) {

		sprintf(wrdent, "%s;", entities[i]);
		sprintf(nument, "%s;", entities[i + 1]);

		c = d = NULL;
		if ((entities[i])[0] != '\0')
			c = (char *) strstr(newword, wrdent);
		if ((entities[i + 1])[0] != '\0')
			d = (char *) strstr(newword, nument);
		if ((entities[i + 2])[0] != '\0') {
			if (c != NULL)
				strcpy(newword, (char *) replace(newword,
				wrdent, entities[i + 2]));
			if (d != NULL)
				strcpy(newword, (char *) replace(newword,
				nument, entities[i + 2]));
		}
	}

	while (hasnonascii(newword))
		strcpy(newword, (char *) converttoascii(newword));

	return newword;
}

/* Does a string still contain numbered or named entities that can
** be converted to ascii equivalents?
*/

int hasnonascii(s)
     char *s;
{
	int i;
	char *c, *d;

	for (i = 0; entities[i] != NULL; i += 3) {
		c = d = NULL;
		if ((entities[i])[0] != '\0')
			c = (char *) strstr(s, entities[i]);
		if ((entities[i + 1])[0] != '\0')
			d = (char *) strstr(s, entities[i + 1]);
		if ((entities[i + 2])[0] != '\0')
			if (c != NULL || d != NULL)
				return 1;
	}

	return 0;
}
