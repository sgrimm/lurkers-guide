/*
** Copyright (C) 1995, Enterprise Integration Technologies Corp.        
** All Rights Reserved.
** Kevin Hughes, kevinh@eit.com 
** 3/11/94
*/

#include "swish.h"
#include "index.h"

/* Recursively goes into a directory and calls the word-indexing
** functions for each file that's found.
*/

void indexadir(dir)
     char *dir;
{
        int badfile;
        DIR *dfd;
#ifdef NEXTSTEP
        struct direct *dp;
#else
        struct dirent *dp;
#endif
	static char s[MAXFILELEN], title[MAXSTRLEN];
        struct sortentry *sortfilelist, *sortdirlist;
        struct swline *tmplist;

        sortfilelist = sortdirlist = NULL;

	if (islink(dir) && !followsymlinks)
		return;

        if (dir[strlen(dir) - 1] == '/')
                dir[strlen(dir) - 1] = '\0';

        if ((dfd = opendir(dir)) == NULL)
                return;

        while ((dp = readdir(dfd)) != NULL && dirconlist != NULL) {
                badfile = 0;
                tmplist = dirconlist;
                while (tmplist != NULL) {
                        if (lstrstr(dp->d_name, tmplist->line)) {
                                badfile = 1;
                                break;
                        }
                        tmplist = tmplist->next;
                }
                if (badfile)
                        return;
	}
	closedir(dfd);
	dfd = opendir(dir);

        while ((dp = readdir(dfd)) != NULL) {

                if ((dp->d_name)[0] == '.')
                        continue;
		if (islink(dp->d_name) && !followsymlinks)
			continue;

                badfile = 0;
                tmplist = fileislist;
                while (tmplist != NULL) {
                        if (!strcmp(dp->d_name, tmplist->line)) {
                                badfile = 1;
                                break;
                        }
                        tmplist = tmplist->next;
                }
                if (badfile)
                        continue;

                badfile = 0;
                tmplist = fileconlist;
                while (tmplist != NULL) {
                        if (lstrstr(dp->d_name, tmplist->line)) {
                                badfile = 1;
                                break;
                        }
                        tmplist = tmplist->next;
                }
                if (badfile)
                        continue;

                sprintf(s, "%s%s%s", dir, dir[strlen(dir) - 1] == '/' ?
                "" : "/", dp->d_name);
		if (islink(s) && !followsymlinks)
			continue;

                badfile = 0;
                tmplist = pathconlist;
                while (tmplist != NULL) {
                        if (lstrstr(s, tmplist->line)) {
                                badfile = 1;
                                break;
                        }
                        tmplist = tmplist->next;
                }
                if (badfile)
                        continue;

                if (!isdirectory(s)) {

			if (!isoksuffix(dp->d_name, suffixlist))
				continue;

                        if (ishtml(s)) {
                                strcpy(title, (char *) parsetitle(s));
                                if (!isoktitle(title))
                                        continue;
                        }
                        else {
                                if (strrchr(s, '/') != NULL)
                                        strcpy(title, strrchr(s, '/') + 1);
                                else
                                        strcpy(title, s);
                        }
                        sortfilelist = (struct sortentry *)
                        addsortentry(sortfilelist, s, title);
                }
                else {
                        sortdirlist = (struct sortentry *)
                        addsortentry(sortdirlist, s, s);
                }
        }

        closedir(dfd);

        printfiles(sortfilelist);
        printdirs(sortdirlist);
}

/* Calls the word-indexing function for a single file.
*/

void indexafile(path)
     char *path;
{
        int badfile;
        char *t, title[MAXSTRLEN];
        struct sortentry *fileentry;
        struct swline *tmplist;

	if (islink(path) && !followsymlinks)
		return;

        if (path[strlen(path) - 1] == '/')
                path[strlen(path) - 1] = '\0';

        badfile = 0;
        tmplist = fileislist;
        while (tmplist != NULL) {
                if (!strcmp(path, tmplist->line)) {
                        badfile = 1;
                        break;
                }
                tmplist = tmplist->next;
        }
        if (badfile)
                return;

        badfile = 0;
        tmplist = fileconlist;
        while (tmplist != NULL) {
                if (lstrstr(path, tmplist->line)) {
                        badfile = 1;
                        break;
                }
                tmplist = tmplist->next;
        }
        if (badfile)
                return;

        badfile = 0;
        tmplist = pathconlist;
        while (tmplist != NULL) {
                if (lstrstr(path, tmplist->line)) {
                        badfile = 1;
                        break;
                }
                tmplist = tmplist->next;
        }
        if (badfile)
                return;

	if (!isoksuffix(path, suffixlist))
		return;

        if (ishtml(path)) {
                strcpy(title, (char *) parsetitle(path));
                if (!isoktitle(title))
                        return;
        }
        else {
                if ((t = strrchr(path, '/')) != NULL)
                        strcpy(title, t + 1);
                else
                        strcpy(title, path);
        }

        fileentry = (struct sortentry *) emalloc(sizeof(struct sortentry));
        fileentry->filename = (char *) mystrdup(path);
        fileentry->title = (char *) mystrdup(title);
        fileentry->left = fileentry->right = NULL;

        printfiles(fileentry);
}

/* Indexes the words in the tree of files in alphabetical order.
*/

void printfiles(e)
     struct sortentry *e;
{
        int wordcount;
	char *s;

        if (e != NULL) {
                printfiles(e->left);
                if (verbose == 3) {
			if ((s = (char *) strrchr(e->filename, '/')) == NULL)
                        	printf("  %s", e->filename);
			else
                        	printf("  %s", s + 1);
		}
                wordcount = countwords(e->filename, e->title);
                if (verbose == 3) {
                        if (wordcount)
                                printf(" (%d words)\n", wordcount);
                        else
                                printf(" (no words)\n");
                        fflush(stdout);
                }
		free(e->filename);
		free(e->title);
                printfiles(e->right);
		free(e);
        }
}

/* Prints out the directory names as things are getting indexed.
** Calls indexadir() so directories in the tree are indexed,
** in alphabetical order...
*/

void printdirs(e)
     struct sortentry *e;
{
        if (e != NULL) {
                printdirs(e->left);
                if (verbose == 3)
                        printf("\nIn dir \"%s\":\n", e->filename);
                else if (verbose == 2)
                        printf("Checking dir \"%s\"...\n", e->filename);
                indexadir(e->filename);
		free(e->filename);
		free(e->title);
                printdirs(e->right);
		free(e);
        }
}

/* Stores file names in alphabetical order so they can be
** indexed alphabetically. No big whoop.
*/

struct sortentry *addsortentry(e, filename, title)
     struct sortentry *e;
     char *filename;
     char *title;
{
        if (e == NULL) {
                e = (struct sortentry *) emalloc(sizeof(struct sortentry));
                e->filename = (char *) mystrdup(filename);
                e->title = (char *) mystrdup(title);
                e->left = e->right = NULL;
        }
        else {
                if (strcmp(e->filename, filename) > 0)
                        e->left = (struct sortentry *)
                        addsortentry(e->left, filename, title);
                else
                        e->right = (struct sortentry *)
                        addsortentry(e->right, filename, title);
        }

        return e;
}

/* Adds a word to the master index tree.
*/

struct entry *addentry(e, word, filenum, emphasized, structure)
     struct entry *e;
     char *word;
     int filenum;
     int emphasized;
     int structure;
{
        int isbigger;
        struct location *tp, *oldtp;

        if (e == NULL) {
                e = (struct entry *) emalloc(sizeof(struct entry));
                e->word = (char *) mystrdup(word);
                e->tfrequency = 1;
                e->locationlist = (struct location *)
                emalloc(sizeof(struct location));
                e->locationlist->filenum = filenum;
                e->locationlist->frequency = 1;
                e->locationlist->emphasized = emphasized;
                e->locationlist->structure = structure;
                e->locationlist->next = NULL;
                e->left = e->right = NULL;
		totalwords++;
        }
        else {
                isbigger = wordcompare(e->word, word);
                if (isbigger == 0) {
                        tp = e->locationlist;
                        while (tp != NULL && tp->filenum != filenum) {
                                oldtp = tp;
                                tp = tp->next;
                        }
                        if (tp == NULL) {
                                tp = (struct location *) emalloc(sizeof(struct
                                location));
                                tp->filenum = filenum;
                                tp->frequency = 1;
                                tp->emphasized = emphasized;
                                tp->structure = structure;
                                tp->next = NULL;
                                oldtp->next = tp;
                                if (!emphasized)
                                        e->tfrequency = e->tfrequency + 1;
                        }
                        else {
                                if (tp->filenum == filenum) {
                                        tp->frequency = tp->frequency + 1;
					if (emphasized)
						tp->emphasized =
						tp->emphasized + 1;
					tp->structure |= structure;
				}
                        }
                }
                else if (isbigger > 0)
                        e->left = (struct entry *)
                        addentry(e->left, word, filenum, emphasized,
			structure);
                else
                        e->right = (struct entry *)
                        addentry(e->right, word, filenum, emphasized,
			structure);
        }

        return e;
}

/* Adds a file to the master list of files and file numbers.
*/

struct file *addtofilelist(filep, filename, title, size)
     struct file *filep;
     char *filename;
     char *title;
     int size;
{
        struct file *newnode;
        static struct file *filelistp = NULL;

        newnode = (struct file *) emalloc(sizeof(struct file));
        newnode->filename = (char *) mystrdup(filename);
        newnode->title = (char *) mystrdup(title);
        newnode->size = size;
        newnode->next = NULL;

        if (filep == NULL)
                filep = newnode;
        else if (filelistp != NULL)
                filelistp->next = newnode;

        filelistp = newnode;

        return filep;
}

/* Just goes through the master list of files and
** counts 'em.
*/

int getfilecount(filep)
     struct file *filep;
{
        int i;

        for (i = 0; filep != NULL; filep = filep->next)
                i++;

        return i;
}

/* Returns the nicely formatted date.
*/

char *getthedate()
{
	static char date[MAXSTRLEN];
	time_t time;

	time = (time_t) getthetime();
	strftime(date, MAXSTRLEN, "%x %X", (struct tm *) localtime(&time));
	strftime(date, MAXSTRLEN, "%d/%m/%y %H:%M:%S %Z",
	(struct tm *) localtime(&time));

	return date;
}

/* Indexes all the words in a file and adds the appropriate information
** to the appropriate structures.
*/

int countwords(filename, title)
     char *filename;
     char *title;
{
        int c, i, j, inword, ftotalwords, emphasized, structure;
        static int filenum;
        char word[MAXWORDLEN], tag[MAXSTRLEN];
        FILE *fp;

        if ((fp = fopen(filename, "r")) == NULL)
                return 0;

        ftotalwords = 0;
        if (isoksuffix(filename, nocontentslist) && nocontentslist != NULL) {
                filelist = addtofilelist(filelist, filename,
                title, getsize(filename));
                fclose(fp);
                filenum++;
                if (!(filenum % 128))
                        filenum++;
                addtofwordtotals(filenum, 100);
                return (countwordstr(title, filenum, 0));
        }

        filelist = addtofilelist(filelist, filename, title, getsize(filename));
        filenum++;
        if (!(filenum % 128))
                filenum++;

	c = 1;
        i = j = 0;
        inword = 0;
        emphasized = 0;
	structure = 1;

        while (c != EOF && (c = fgetc(fp)) != EOF) {
                if (!inword) {
                        if (iswordchar(c)) {
                                i = 0;
                                word[i++] = c;
				if (i == MAXWORDLEN)
					i--;
                                inword = 1;
                        }
                }
                else if (inword) {
                        if (!iswordchar(c)) {
                                word[i++] = '\0';
				if (i == MAXWORDLEN)
					word[--i] = '\0';
                                for (i = 0; word[i]; i++)
                                        word[i] = tolower(word[i]);
				i = 0;
                                if (isokword(word))
					strcpy(word, (char *)
					convertentities(word));

/* Sorry, have to do isokword() twice to filter out converted strings! */

                                if (hasokchars(word) && isokword(word)) {
#ifdef DEBUG
printf("	%s %d\n", word, structure);
#endif
                                        entrylist = (struct entry *)
                                        addentry(entrylist, word,
                                        filenum, emphasized, structure);
                                        ftotalwords++;
                                }
                                inword = 0;
                        }
                        else {
                                word[i++] = c;
				if (i == MAXWORDLEN)
					i--;
			}
		}
                if (c == '<' && !INDEXTAGS) {
			j = 0;
			while ((c = fgetc(fp)) != EOF) {
				tag[j++] = c;
				if (j == MAXSTRLEN)
					j--;
                        	if (c == '>') {
					if (j)
						tag[--j] = '\0';
					else
						tag[j] = '\0';
#ifdef DEBUG
printf("t: %s\n", tag);
#endif
					structure = getstructure(tag,
					structure);
#ifdef DEBUG
printf("s: %d\n", structure);
#endif
					if (tag[0] == '!')
						ftotalwords +=
						parsecomment(tag,
						filenum, structure);
					if ((structure & IN_HEADER) ||
					(structure & IN_TITLE))
						emphasized = 5;
					else
						emphasized = 0;

					break;
				}
			}
                }
        }

        fclose(fp);

        addtofwordtotals(filenum, ftotalwords);
        return ftotalwords;
}

/* Indexes the words in a string, such as a file name or an
** HTML title.
*/

int countwordstr(s, filenum, emphasized)
     char *s;
     int filenum;
     int emphasized;
{
        int i, j, inword, wordcount;
        char c, word[MAXWORDLEN], tmpstr[MAXFILELEN];

        sprintf(tmpstr, "%s ", s);
        for (j = inword = wordcount = 0; (c = tmpstr[j]) != '\0'; j++) {
                if (!inword) {
                        if (iswordchar(c)) {
                                i = 0;
                                word[i++] = c;
				if (i == MAXWORDLEN)
					i--;
                                inword = 1;
                        }
                }
                else {
                        if (!iswordchar(c)) {
                                wordcount++;
                                word[i] = '\0';
                                for (i = 0; word[i]; i++)
                                        word[i] = tolower(word[i]);
                                if (isokword(word))
					strcpy(word, (char *)
					convertentities(word));
                                if (hasokchars(word) && isokword(word))
                                        entrylist = (struct entry *)
                                        addentry(entrylist, word,
                                        filenum, emphasized, IN_FILE);
                                inword = 0;
                        }
                        else {
                                word[i++] = c;
				if (i == MAXWORDLEN)
					i--;
			}
                }
        }

        return wordcount;
}

/* This returns the value corresponding to the HTML structures
** a word is in.
*/

int getstructure(tag, structure)
     char *tag;
     int structure;
{
	int len;

	len = strlen(tag);
	if (lstrstr(tag, "/title") && len == 6)
		structure &= ~IN_TITLE;
	else if (lstrstr(tag, "title") && len == 5)
		structure |= IN_TITLE;
	else if (lstrstr(tag, "/head") && len == 5)
		structure &= ~IN_HEAD;
	else if (lstrstr(tag, "head") && len == 4)
		structure |= IN_HEAD;
	else if (lstrstr(tag, "/body") && len == 5)
		structure &= ~IN_BODY;
	else if (lstrstr(tag, "body") && len == 4)
		structure |= IN_BODY;
	else if (tag[0] == '/' && tolower(tag[1]) == 'h' && isdigit(tag[2]))
		structure &= ~IN_HEADER;
	else if (tolower(tag[0]) == 'h' && isdigit(tag[1]))
		structure |= IN_HEADER;
	else if (lstrstr(tag, "/em") || lstrstr(tag, "/strong"))
		structure &= ~IN_EMPHASIZED;
	else if (lstrstr(tag, "em") || lstrstr(tag, "strong"))
		structure |= IN_EMPHASIZED;
	else if ((tolower(tag[0]) == 'b' || tolower(tag[0]) == 'i') &&
	len == 1)
		structure |= IN_EMPHASIZED;
	else if (tag[0] == '/' && tag[2] == '\0' && (tolower(tag[1]) == 'b' ||
	tolower(tag[1]) == 'i'))
		structure &= ~IN_EMPHASIZED;

	return structure;
}

/* Parses the words in a comment.
*/

int parsecomment(tag, filenum, structure)
     char *tag;
     int filenum;
     int structure;
{
	int i, j, inword, wordcount, emphasized;
	char c, word[MAXWORDLEN];

	if (EMPHASIZECOMMENTS)
		emphasized = 5;
	else
		emphasized = 0;
	structure |= IN_COMMENTS;
        for (j = 3, inword = wordcount = 0; (c = tag[j]) != '\0'; j++) {
                if (!inword) {
                        if (iswordchar(c)) {
                                i = 0;
                                word[i++] = c;
				if (i == MAXWORDLEN)
					i--;
                                inword = 1;
                        }
                }
                else {
                        if (!iswordchar(c)) {
                                wordcount++;
                                word[i] = '\0';
                                for (i = 0; word[i]; i++)
                                        word[i] = tolower(word[i]);
                                if (isokword(word))
					strcpy(word, (char *)
					convertentities(word));
                                if (hasokchars(word) && isokword(word))
                                        entrylist = (struct entry *)
                                        addentry(entrylist, word,
                                        filenum, emphasized, structure);
                                inword = 0;
                        }
                        else {
                                word[i++] = c;
				if (i == MAXWORDLEN)
					i--;
			}
                }
        }

        return wordcount;
}

/* Removes words that occur in over _plimit_ percent of the files and
** that occur in over _flimit_ files (marks them as stopwords, that is).
*/

int removestops(ep, totalfiles, plimit, flimit)
     struct entry *ep;
     int totalfiles;
     int plimit;
     int flimit;
{
	int percent, wordfilecount, stopwords;
        struct location *lp;

	stopwords = 0;
	if (ep != NULL) {
		stopwords += removestops(ep->left, totalfiles, plimit, flimit);
                lp = ep->locationlist;
		wordfilecount = 0;
                while (lp != NULL) {
			wordfilecount++;
                        lp = lp->next;
                }
		percent = ((float) wordfilecount / (float) totalfiles) * 100.0;
		if (percent >= plimit && wordfilecount >= flimit) {
			addstophash(ep->word);
			stopwords++;
		}
		stopwords += removestops(ep->right, totalfiles, plimit, flimit);
	}
	return stopwords;
}

/* This is somewhat similar to the rank calculation algorithm
** from WAIS (I think). Any suggestions for improvements?
** Note that ranks can't be smaller than 1, emphasized words
** (words in titles, headers) have ranks multiplied by at least 5
** (just a guess), and ranks divisible by 128 are bumped up by one
** (to make the compression scheme with with '\0' as a line delimiter
** work). Fudging with the ranks doesn't seem to make much difference.
*/

int getrank(freq, tfreq, words, emphasized)
     int freq;
     int tfreq;
     int words;
     int emphasized;
{
        float d, e, f;
        int tmprank;
        char rankstr[MAXSTRLEN];

        if (freq < 5)
                freq = 5;
        d = 1.0 / (double) tfreq;
        e = ((log((double) freq) + 10.0) * d) / words;
        f = e * 10000.0;

        sprintf(rankstr, "%f", f);
        tmprank = atoi(rankstr);
        if (tmprank <= 0)
                tmprank = 1;
        if (emphasized)
                tmprank *= emphasized;
        if (!(tmprank % 128))
                tmprank++;

        return tmprank;
}

/* Prints the index information at the head of index files.
*/

void printheader(fp, filename, totalwords, totalfiles)
     FILE *fp;
     char *filename;
     int totalwords;
     int totalfiles;
{
	char *c;

	c = (char *) strrchr(filename, '/');

	fprintf(fp, "%s\n", INDEXHEADER);
	fprintf(fp, "# Name: %s\n", (indexn[0] == '\0') ? "(no name)" :
	indexn);
	fprintf(fp, "# Saved as: %s\n", (c == NULL && c + 1 != '\0') ?
		filename : c + 1);
	fprintf(fp, "# Counts: ");
	if (totalwords)
		fprintf(fp, "%d words%s", totalwords, (totalfiles) ? ", " : "");
	if (totalfiles)
		fprintf(fp, "%d files", totalfiles);
	fprintf(fp, "\n");
	fprintf(fp, "# Indexed on: %s\n", getthedate());
	fprintf(fp, "# Description: %s\n", (indexd[0] == '\0') ?
	"(no description)" : indexd);
	fprintf(fp, "# Pointer: %s\n", (indexp[0] == '\0') ?
	"(no pointer)" : indexp);
	fprintf(fp, "# Maintained by: %s\n", (indexa[0] == '\0') ?
	"(no maintainer)" : indexa);
}

/* Print the index entries that hold the word, rank, and other information.
*/

void printindex(ep, fp)
     struct entry *ep;
     FILE *fp;
{
        int i, rank;
        struct location *lp;

        if (ep != NULL) {
                printindex(ep->left, fp);
		if (!isstopword(ep->word)) {

			for (i = 0; indexchars[i] != '\0'; i++)
				if ((ep->word)[0] == indexchars[i] &&
				!offsets[i])
					offsets[i] = ftell(fp);

	                fprintf(fp, "%s:", ep->word);
			lp = ep->locationlist;
			while (lp != NULL) {
				compress(lp->filenum, fp);
				rank = getrank(lp->frequency, ep->tfrequency,
				gettotalwords(lp->filenum), lp->emphasized);
				compress(rank, fp);
				compress(lp->structure, fp);
				lp = lp->next;
                	}
                	fputc(0, fp);

		}
                printindex(ep->right, fp);
        }
}

/* Prints the list of stopwords into the index file.
*/

void printstopwords(fp)
     FILE *fp;
{
	int hashval;
	struct swline *sp;

	offsets[STOPWORDPOS] = ftell(fp);
	for (hashval = 0; hashval < HASHSIZE; hashval++) {
        	sp = hashstoplist[hashval];
		while (sp != NULL) {
			fprintf(fp, "%s ", sp->line);
			sp = sp->next;
		}
	}
	fprintf(fp, "\n");
}

/* Prints the list of files, titles, and sizes into the index file.
*/

void printfilelist(filep, fp)
     struct file *filep;
     FILE *fp;
{
	int i;

	i = 0;
	offsets[FILELISTPOS] = ftell(fp);
        while (filep != NULL) {
		addtofilehashlist(i++, ftell(fp));
                fprintf(fp, "%s \"%s\" %d\n", ruleparse(filep->filename),
                filep->title, filep->size);
                filep = filep->next;
        }
}

/* Prints the list of file offsets into the index file.
*/

void printfileoffsets(fp)
     FILE *fp;
{
	int i;

	offsets[FILEOFFSETPOS] = ftell(fp);
	for (i = 0; getfilenum(i) != 0; i++)
		fprintf(fp, "%016li", getfilenum(i));
}

/* Takes a number and prints it to a file using the simple
** accordion scheme of storing numbers.
*/

void compress(num, fp)
     int num;
     FILE *fp;
{
        int i, r;
        static char s[8];

        i = 0;
        while (num) {
                r = num % 128;
                num /= 128;
                s[i++] = r;
        }
        while (i-- >= 0)
                fputc(s[i] | (i ? 128 : 0), fp);
}

/* Prints out the decompressed values in an index file.
*/

void decompress(fp)
     FILE *fp;
{
        int c, x, inword;
	long pos;
        char line[MAXSTRLEN], header[MAXHEADCHARS + 1];

	readoffsets(fp);
	fseek(fp, 0, 0);
        inword = 1;

        while (1) {
                c = fgetc(fp);
                ungetc(c, fp);
                if (c == '#') {
                        fgets(line, MAXSTRLEN, fp);
			printf("%s", line);
                        continue;
                }
                else {
                        fgets(header, MAXHEADCHARS + 1, fp);
			printf("%s", header);
                        break;
                }
        }

        while ((c = fgetc(fp)) != EOF) {
                if (c == ':' && inword) {
                        inword = 0;
                        putchar(c);
                }
                if (inword)
                        putchar(c);
                else {
                        x = 0;
                        do {
                                c = fgetc(fp);
				pos = ftell(fp);
				if (pos == offsets[STOPWORDPOS]) {
					putchar('\n');
					while (fgets(line, MAXSTRLEN, fp)
					!= NULL)
						printf("%s", line);
						return;
				}
                                if (c == 0) {
                                        putchar('\n');
                                        inword = 1;
                                        break;
                                }
                                x *= 128;
                                x += c & 127;
                        } while (c & 128);
                        if (x)
                                printf(" %d", x);
                }
        }
}

/* Parses lines according to the ReplaceRules directives.
*/

char *ruleparse(line)
     char *line;
{
        char rule[MAXSTRLEN];
        static char tmpline[MAXSTRLEN], newtmpline[MAXSTRLEN];
        static char line1[MAXSTRLEN], line2[MAXSTRLEN];
        struct swline *tmplist;

	if (replacelist == NULL)
		return line;

        tmplist = replacelist;
        strcpy(tmpline, line);
        while(1) {
		if (tmplist == NULL)
			return tmpline;
                strcpy(rule, tmplist->line);
		tmplist = tmplist->next;
		if (tmplist == NULL)
			return tmpline;
                if (rule == NULL) {
                        replacelist = tmplist;
                        return tmpline;
                }
                else {
                        if (lstrstr(rule, "replace")) {
                                strcpy(line1, tmplist->line);
				tmplist = tmplist->next;
                                strcpy(line2, tmplist->line);
				tmplist = tmplist->next;
                                strcpy(newtmpline, (char *) replace(tmpline,
                                line1, NOWORD));
                                strcpy(newtmpline, (char *) replace(newtmpline,
                                NOWORD, line2));
                        }
                        else if (lstrstr(rule, "append")) {
				sprintf(newtmpline, "%s%s", tmpline,
				tmplist->line);
				tmplist = tmplist->next;
			}
			else if (lstrstr(rule, "prepend")) {
				sprintf(newtmpline, "%s%s", tmplist->line,
				tmpline);
				tmplist = tmplist->next;
			}
			strcpy(tmpline, newtmpline);
                }
        }
}
