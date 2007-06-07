/*
** Copyright (C) 1995, Enterprise Integration Technologies Corp.        
** All Rights Reserved.
** Kevin Hughes, kevinh@eit.com 
** 3/11/94
*/

#include "swish.h"
#include "hash.h"

/* Hashes a string.
*/

unsigned hash(s)
     char *s;
{
        unsigned hashval;

        for (hashval = 0; *s != '\0'; s++)
                hashval = *s + 31 * hashval;
        return hashval % HASHSIZE;
}

/* Hashes a string for a larger hash table.
*/

unsigned bighash(s)
     char *s;
{
        unsigned hashval;

        for (hashval = 0; *s != '\0'; s++)
                hashval = *s + 31 * hashval;
        return hashval % BIGHASHSIZE;
}

/* Reads the internal list of default stopwords.
*/

void readdefaultstopwords()
{
        int i;

        for (i = 0; defaultstopwords[i] != NULL; i++)
                addstophash(defaultstopwords[i]);
}

/* Adds a stop word to a hash table.
*/

void addstophash(word)
     char *word;
{
        unsigned hashval;
        struct swline *sp;

	if (isstopword(word))
		return;

        sp = (struct swline *) emalloc(sizeof(struct swline));
        sp->line = (char *) mystrdup(word);

        hashval = hash(word);
        sp->next = hashstoplist[hashval];
        hashstoplist[hashval] = sp;
}

/* Sees if a word is a stop word by looking it up in the hash table.
*/

int isstopword(word)
     char *word;
{
        unsigned hashval;
        struct swline *sp;

        hashval = hash(word);
        sp = hashstoplist[hashval];

        while (sp != NULL) {
                if (!strcmp(sp->line, word))
                        return 1;
                sp = sp->next;
        }
        return 0;
}

/* Adds a file number and its associated file location
** to a hash table.
*/

void addtofilehashlist(fileshort, filelong)
     int fileshort;
     long filelong;
{
        unsigned hashval;
        struct filenum *fp;
        char tmpstr[MAXSTRLEN];

        fp = (struct filenum *) emalloc(sizeof(struct filenum));
        fp->fileshort = fileshort;
        fp->filelong = filelong;

        sprintf(tmpstr, "%d", fileshort);
        hashval = bighash(tmpstr);
        fp->next = filehashlist[hashval];
        filehashlist[hashval] = fp;
}

/* Looks up a file number in the hash table and
** returns the file position of the associated file info.
*/

long getfilenum(filenum)
     int filenum;
{
        unsigned hashval;
        struct filenum *fp;
        char tmpstr[MAXSTRLEN];

        sprintf(tmpstr, "%d", filenum);
        hashval = bighash(tmpstr);
        fp = filehashlist[hashval];

        while (fp != NULL) {
                if (fp->fileshort == filenum)
                        return fp->filelong;
                fp = fp->next;
        }
        return 0;
}

/* Adds a file number and the number of indexed words
** to a hash table.
*/

void addtofwordtotals(filenum, ftotalwords)
     int filenum;
     int ftotalwords;
{
        unsigned hashval;
        struct fwordtotal *fp;
        char tmpstr[MAXSTRLEN];

        fp = (struct fwordtotal *) emalloc(sizeof(struct fwordtotal));
        fp->filenum = filenum;
        fp->totalwords = ftotalwords;
        fp->next = NULL;

        sprintf(tmpstr, "%d", filenum);
        hashval = bighash(tmpstr);
        fp->next = fwordtotals[hashval];
        fwordtotals[hashval] = fp;
}

/* Looks up a file number in the hash table and
** returns the total number of words indexed in it.
*/

int gettotalwords(filenum)
     int filenum;
{
        unsigned hashval;
        struct fwordtotal *fp;
        char tmpstr[MAXSTRLEN];

        sprintf(tmpstr, "%d", filenum);
        hashval = bighash(tmpstr);
        fp = fwordtotals[hashval];

        while (fp != NULL) {
                if (fp->filenum == filenum)
                        return fp->totalwords;
                fp = fp->next;
        }
        return 0;
}

/* Adds a file number to a hash table of results.
** If the entry's alrady there, add the ranks,
** else make a new entry.
*/

void mergeresulthashlist(filenum, rank, structure)
     int filenum;
     int rank;
     int structure;
{
        unsigned hashval;
        struct result *rp, *newrp;
        char word[MAXWORDLEN];

        sprintf(word, "%d", filenum);
	hashval = hash(word);

	rp = resulthashlist[hashval];
	while (rp != NULL) {
		if (rp->filenum == filenum) {
			rp->rank += rank;
			rp->structure |= structure;
			return;
		}
		rp = rp->next;
	}
	
        newrp = (struct result *) emalloc(sizeof(struct result));
        newrp->filenum = filenum;
        newrp->rank = rank;
	newrp->structure = structure;
        newrp->next = resulthashlist[hashval];
        resulthashlist[hashval] = newrp;
}

/* Initializes the result hash list.
*/

void initresulthashlist()
{
	int i;

	for (i = 0; i < HASHSIZE; i++)
		resulthashlist[i] = NULL;
}
