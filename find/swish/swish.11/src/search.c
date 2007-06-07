/*
** Copyright (C) 1995, Enterprise Integration Technologies Corp.        
** All Rights Reserved.
** Kevin Hughes, kevinh@eit.com 
** 3/11/94
*/

#include "swish.h"
#include "search.h"

/* The main search function.
** Parentheses are stripped out, things made lowercase,
** extra blanks removed, etc.
*/

void search(words, indexlist, structure)
     char *words;
     struct swline *indexlist;
     int structure;
{
        int i, j;
        float num;
        char word[MAXWORDLEN];
	struct result *resultlist;
	struct sortresult *sortresultlist;
	struct swline *tmplist;
        FILE *fp;

	searchwordlist = NULL;

        for (i = j = 0; words[i] != '\0' && words[i] != '\n'; i++) {
		if (isspace(words[i]) || words[i] == '(' ||
		words[i] == ')') {
			if (j) {
				word[j] = '\0';
				searchwordlist = (struct swline *)
				addswline(searchwordlist, (char *)
				convertentities(word));
				j = 0;
			}
			if (words[i] == '(') {
				searchwordlist = (struct swline *)
				addswline(searchwordlist, "(");
			}
			if (words[i] == ')') {
				searchwordlist = (struct swline *)
				addswline(searchwordlist, ")");
			}
		}
		else
			word[j++] = tolower(words[i]);
	}
	if (j) {
		word[j] = '\0';
		searchwordlist = (struct swline *)
		addswline(searchwordlist, (char *) convertentities(word));
	}

        printf("%s\n", INDEXHEADER);
        if (words[0] == '\0') {
                printf("err: no search words specified\n.\n");
                exit(0);
        }

        printf("search words:");
	tmplist = searchwordlist;
	while (tmplist != NULL) {
		printf(" %s", tmplist->line);
		tmplist = tmplist->next;
	}
	putchar('\n');

	while (indexlist != NULL) {

		commonerror = bigrank = 0;

		if ((fp = fopen(indexlist->line, "r")) == NULL) {
			printf("# Name: unknown index\n");
			printf("err: could not open index file\n.\n");
			exit(0);
		}

		if (!isokindexheader(fp)) {
			printf("err: the index file format is unknown\n.\n");
			exit(0);
		}

		getheader(fp);

		if (!getindexfilenum(fp)) {
			printf("err: the index file is empty\n.\n");
			exit(0);
		}

		readoffsets(fp);
		readstopwords(fp);
		readfileoffsets(fp);

		resultlist = NULL;
		tmplist = searchwordlist;
		tmplist = (struct swline *) fixnot(tmplist, fp);
		searchwordlist = (struct swline *) expandstar(tmplist, fp);
		resultlist = (struct result *) parseterm(fp, 0);

		sortresultlist = NULL;
                while (resultlist != NULL) {
			if (resultlist->structure & structure)
                        	sortresultlist = (struct sortresult *)
                        	addsortresult(sortresultlist, resultlist->rank,
                        	lookupfile(resultlist->filenum, fp));
                        resultlist = resultlist->next;
                }

        	fclose(fp);

		if (sortresultlist == NULL) {
			if (commonerror)
				printf("err: a word is too common\n");
			else
		                printf("err: no results\n");
		}
		else {
			if (bigrank)
				num = 1000.0 / (float) bigrank;
			else
				num = 1000;
		        printsortedresults(sortresultlist, num);
		}

		searchwordlist = tmplist;
		indexlist = indexlist->next;

	}

        printf(".\n");
}

/* This puts parentheses in the right places around not structures
** so the parser can do its thing correctly.
*/

struct swline *fixnot(sp)
     struct swline *sp;
{
	int openparen, hasnot;
	char lastline[MAXWORDLEN];
	struct swline *tmpp, *newp;
#ifdef DEBUG
	struct swline *newp2;
#endif

	tmpp = sp;
	newp = NULL;

	openparen = 0;
	hasnot = 0;
	lastline[0] = '\0';
	while (tmpp != NULL) {
		if ((tmpp->line)[0] == '(')
			openparen++;
		if ((tmpp->line)[0] == ')')
			openparen--;
		if (!strcmp(tmpp->line, "not") && lastline[0] != '\0' &&
		lastline[0] != '(') {
			hasnot = 1;
			newp = (struct swline *) addswline(newp, "(");
		}
		else if (hasnot && !openparen) {
			hasnot = 0;
			newp = (struct swline *) addswline(newp, tmpp->line);
			newp = (struct swline *) addswline(newp, ")");
			strcpy(lastline, tmpp->line);
			tmpp = tmpp->next;
			continue;
		}
		newp = (struct swline *) addswline(newp, tmpp->line);
		strcpy(lastline, tmpp->line);
		tmpp = tmpp->next;
	}

#ifdef DEBUG
	newp2 = newp;
	while (newp2 != NULL) {
		printf("%s ", newp2->line);
		newp2 = newp2->next;
	}
	putchar('\n');
#endif

	return newp;
}

/* Expands words with asterisks as wildcards into a series of
** "or" searches. Terms like "quick\*" are expanded into
** "quicktime or quickly", etc.
*/

struct swline *expandstar(sp, fp)
     struct swline *sp;
     FILE *fp;
{
	int i, firsttime, gotstar;
	char foundword[MAXWORDLEN], searchword[MAXWORDLEN];
        struct swline *newp;

	newp = NULL;
	while (sp != NULL) {
		strcpy(searchword, sp->line);
		if (searchword[0] != '*' && strchr(searchword, '*')) {
			for (i = gotstar = 0; searchword[i]; i++)
				if (gotstar)
					searchword[i] = '\0';
				else if (searchword[i] == '*') {
					searchword[i] = '\0';
					gotstar = 1;
				}
			firsttime = 0;
			do {
				strcpy(foundword, getmatchword(searchword,
				fp, firsttime));
				if (strcmp(foundword, NOWORD)) {
					if (firsttime)
						newp = (struct swline *)
						addswline(newp, "or");
					newp = (struct swline *)
					addswline(newp, foundword);
				}
				else {
					if (!firsttime)
						newp = (struct swline *)
						addswline(newp, NOWORD);
					break;
				}
				firsttime++;
			} while (strcmp(foundword, NOWORD));
		}
		else {
			newp = (struct swline *) addswline(newp,
			searchword);
		}
		sp = sp->next;
	}
	return newp;
}

/* If firsttime is 1, returns the first match to a beginnng of a word.
** Else if it's 0, returns the next match, until nothing is found,
** in which case NULL is returned.
*/

char *getmatchword(word, fp, firsttime)
     char *word;
     FILE *fp;
     int firsttime;
{
	int i, c, found;
        char *d;
	static char fileword[MAXWORDLEN];

	if (!firsttime) {
		for (i = found = 0; indexchars[i] != '\0'; i++)
			if (word[0] == indexchars[i]) {
				fseek(fp, offsets[i], 0);
				found = 1;
			}
		if (!found)
			return NOWORD;
	}

	if (offsets[STOPWORDPOS] == ftell(fp))
		return NOWORD;
        for (i = 0; (c = fgetc(fp)) != 0; ) {
                if (c == ':') {
                        fileword[i] = '\0';
			i = 0;
			while ((c = fgetc(fp)) != 0)
				;
			if (fileword[0] != word[0])
				return NOWORD;
			d = (char *) strstr(fileword, word);
                        if (d != NULL && d == &fileword[0])
                                return fileword;
			else {
				if (offsets[STOPWORDPOS] == ftell(fp))
					return NOWORD;
			}
                }
		else
			fileword[i++] = c;
	}
	return NOWORD;
}

/* Reads and prints the header of an index file.
*/

void getheader(fp)
     FILE *fp;
{
	int c;
	char line[MAXSTRLEN];

	fgets(line, MAXSTRLEN, fp);
        while (1) {
                c = fgetc(fp);
                ungetc(c, fp);
                if (c == '#') {
                        fgets(line, MAXSTRLEN, fp);
                        printf("%s", line);
                        continue;
                }
		else
			break;
	}
	fseek(fp, 0, 0);
}

/* Reads the offsets in the index file so word lookup is faster.
*/

void readoffsets(fp)
     FILE *fp;
{
	int c, i, k;
	long j, num;

	for (i = 0; i < MAXCHARS; i++)
		offsets[i] = 0;

	fseek(fp, 0, 0);
	while (1) {
		c = fgetc(fp);
		if (c == '#') {
			do {
				c = fgetc(fp);
			} while (c && c != '\n');
			continue;
		}
		else
			break;
	}

        j = 0;
        while (c != EOF && c != '\n') {
		k = MAXLONGLEN;
                for (num = 0; c && isdigit(c) && k--; ) {
                        num = (num * 10) + (c - '0');
			c = fgetc(fp);
		}
                offsets[j++] = num;
        }
}

/* Reads the stopwords in the index file.
*/

void readstopwords(fp)
     FILE *fp;
{
	int i, c;
	char word[MAXWORDLEN];

	fseek(fp, offsets[STOPWORDPOS], 0);
	for (i = 0; (c = fgetc(fp)) != '\n' && c != EOF; )
		if (!isspace(c))
			word[i++] = c;
		else {
			word[i] = '\0';
			addstophash(word);
			i = 0;
		}
}

/* Reads the file offset table in the index file.
*/

void readfileoffsets(fp)
     FILE *fp;
{
	int j, k, c;
	long num;

        j = 0;
        fseek(fp, offsets[FILEOFFSETPOS], 0);
        c = fgetc(fp);
        while (c != EOF && c != '\n') {
                k = MAXLONGLEN;
                for (num = 0; c != EOF && isdigit(c) && k--; ) {
                        num = (num * 10) + (c - '0');
                        c = fgetc(fp);
                }
                addtofilehashlist(j++, num);
        }
}

/* The recursive parsing function.
** This was a headache to make but ended up being surprisingly easy. :)
** parseone tells the function to only operate on one word or term.
*/

struct result *parseterm(fp, parseone)
     FILE *fp;
     int parseone;
{
	int rulenum;
        char word[MAXWORDLEN];
	struct result *rp, *newrp;

	rp = NULL;

	rulenum = OR_RULE;
	while (searchwordlist != NULL) {
		strcpy(word, searchwordlist->line);

		if (rulenum == NO_RULE)
			rulenum = DEFAULT_RULE;
		if (isunaryrule(word)) {
			searchwordlist = searchwordlist->next;
			rp = (struct result *) parseterm(fp, 1);
			rp = (struct result *) notresultlist(rp, fp);
			continue;
		}
		else if (isbooleanrule(word)) {
			rulenum = getrulenum(word);
			searchwordlist = searchwordlist->next;
			continue;
		}

		if (word[0] == '(') {

			searchwordlist = searchwordlist->next;
			newrp = (struct result *) parseterm(fp, 0);

			if (rulenum == AND_RULE)
				rp = (struct result *)
				andresultlists(rp, newrp);
			else if (rulenum == OR_RULE)
				rp = (struct result *)
				orresultlists(rp, newrp);
			if (searchwordlist == NULL)
				break;

			continue;

		}
		else if (word[0] == ')') {
			searchwordlist = searchwordlist->next;
			break;
		}

		rp = (struct result *) operate(rp, rulenum, word, fp);

		if (parseone) {
			searchwordlist = searchwordlist->next;
			break;
		}
		rulenum = NO_RULE;

		searchwordlist = searchwordlist->next;
        }

	return rp;
}

/* Looks up a word in the index file -
** it calls getfileinfo(), which does the real searching.
*/

struct result *operate(rp, rulenum, word, fp)
     struct result *rp;
     int rulenum;
     char *word;
     FILE *fp;
{
        int i, found;
	struct result *newrp, *returnrp;

	if (isstopword(word) && !isrule(word)) {
		if (rulenum == OR_RULE && rp != NULL)
			return rp;
		else
			commonerror = 1;
	}

	for (i = found = 0; indexchars[i] != '\0'; i++)
		if (word[0] == indexchars[i]) {
			fseek(fp, offsets[i], 0);
			found = 1;
		}
	if (!found) {
		if (rulenum == AND_RULE)
			return NULL;
		else if (rulenum == OR_RULE)
			return rp;
	}

	newrp = (struct result *) getfileinfo(word, fp);
	if (rulenum == AND_RULE)
		returnrp = (struct result *) andresultlists(rp, newrp);
	else if (rulenum == OR_RULE)
		returnrp = (struct result *) orresultlists(rp, newrp);
	else if (rulenum == NOT_RULE)
		returnrp = (struct result *) notresultlist(newrp, fp);
	return returnrp;
}

/* Looks up a file name in the index file.
*/

char *lookupfile(filenum, fp)
     int filenum;
     FILE *fp;
{
        static char line[MAXSTRLEN];

        fseek(fp, getfilenum(decodefilenum(filenum) - 1), 0);
        fgets(line, MAXSTRLEN, fp);

	return line;
}

/* Finds a word and returns its corresponding file and rank information list.
** If not found, NULL is returned.
*/

struct result *getfileinfo(word, fp)
     char *word;
     FILE *fp;
{
	int i, c, x, countnum, rank, filenum, structure;
        char fileword[MAXWORDLEN];
        struct result *rp;

	rp = NULL;

        for (i = 0; (c = fgetc(fp)) != 0; ) {
                if (c == ':') {
                        fileword[i] = '\0';
			i = 0;
                        if (!strcmp(word, fileword))
                                break;
                        else {
				while ((c = fgetc(fp)) != 0)
					;
				if (offsets[STOPWORDPOS] == ftell(fp))
					return NULL;
				continue;
			}
                }
		else
			fileword[i++] = c;
	}
	if (c == 0)
		return NULL;

        countnum = 1;

	ungetc(c, fp);
	while ((c = fgetc(fp)) != 0) {
		x = 0;
		do {
			c = fgetc(fp);
			if (c == 0)
				return rp;
			x *= 128;
			x += c & 127;
		} while (c & 128);
		if (x) {
			if (countnum == 1) {
				filenum = x;
				countnum++;
			}
			else if (countnum == 2) {
				rank = x;
				countnum++;
			}
			else if (countnum == 3) {
				structure = x;
				rp = (struct result *)
				addtoresultlist(rp, filenum, rank,
				structure);
				countnum = 1;
			}
		}
	}

	return rp;
}

/* Is a word a rule?
*/

int isrule(word)
     char *word;
{
	if (!strcmp(word, "and") || !strcmp(word, "or") || !strcmp(word, "not"))
		return 1;
	else
		return 0;
}

/* Is a word a boolean rule?
*/

int isbooleanrule(word)
     char *word;
{
	if (!strcmp(word, "and") || !strcmp(word, "or"))
		return 1;
	else
		return 0;
}

/* Is a word a unary rule?
*/

int isunaryrule(word)
     char *word;
{
	if (!strcmp(word, "not"))
		return 1;
	else
		return 0;
}

/* Return the number for a rule.
*/

int getrulenum(word)
     char *word;
{
	if (!strcmp(word, "and"))
		return AND_RULE;
	else if (!strcmp(word, "or"))
		return OR_RULE;
	else if (!strcmp(word, "not"))
		return NOT_RULE;
	return NO_RULE;
}

/* Takes two lists of results from searches and ANDs them together.
*/

struct result *andresultlists(r1, r2)
     struct result *r1;
     struct result *r2;
{
        static struct result *tmpnode, *newnode;

        if (r1 == NULL || r2 == NULL)
                return NULL;

        newnode = NULL;

        while (r1 != NULL) {
                tmpnode = r2;
                while (tmpnode != NULL) {
                        if (r1->filenum == tmpnode->filenum)
                                newnode = (struct result *)
                                addtoresultlist(newnode, r1->filenum,
                                (r1->rank + tmpnode->rank) / 2,
				r1->structure & tmpnode->structure);
                        tmpnode = tmpnode->next;
                }
                r1 = r1->next;
        }

        return newnode;
}

/* Takes two lists of results from searches and ORs them together.
*/

struct result *orresultlists(r1, r2)
     struct result *r1;
     struct result *r2;
{
        int i;
        struct result *rp;
        static struct result *newnode;

	newnode = NULL;

        if (r1 == NULL)
                return r2;
        else if (r2 == NULL)
                return r1;

	initresulthashlist();
	while (r1 != NULL) {
		mergeresulthashlist(r1->filenum, r1->rank, r1->structure);
		r1 = r1->next;
	}
	while (r2 != NULL) {
		mergeresulthashlist(r2->filenum, r2->rank, r2->structure);
		r2 = r2->next;
	}
	for (i = 0; i < HASHSIZE; i++) {
		rp = resulthashlist[i];
		while (rp != NULL) {
			newnode = (struct result *) addtoresultlist(newnode,
			rp->filenum, rp->rank, rp->structure);
			rp = rp->next;
		}
	}

        return newnode;
}

/* This performs the NOT unary operation on a result list.
** NOTed files are marked with a default rank of 1000.
*/

struct result *notresultlist(rp, fp)
     struct result *rp;
     FILE *fp;
{
	int i, filenums;
        struct result *newp;

	newp = NULL;

	initmarkentrylist();
	while (rp != NULL) {
		marknum(rp->filenum);
		rp = rp->next;
	}

	filenums = getindexfilenum(fp);
	for (i = 1; !ismarked(i) && i < filenums; i++)
		newp = (struct result *) addtoresultlist(newp,
		i, 1000, IN_ALL);

	return newp;
}

/* Adds a file number and rank to a list of results.
*/

struct result *addtoresultlist(rp, filenum, rank, structure)
     struct result *rp;
     int filenum;
     int rank;
     int structure;
{
        struct result *newnode;
        static struct result *head;

        newnode = (struct result *) emalloc(sizeof(struct result));
        newnode->filenum = filenum;
        newnode->rank = rank;
        newnode->structure = structure;
        newnode->next = NULL;

        if (rp == NULL)
                rp = newnode;
        else
                head->next = newnode;

        head = newnode;

        return rp;
}

/* Adds the results of a search, sorts them by rank.
*/

struct sortresult *addsortresult(sp, rank, fileinfo)
     struct sortresult *sp;
     int rank;
     char *fileinfo;
{
        if (rank > bigrank)
                bigrank = rank;

        if (sp == NULL) {
                sp = (struct sortresult *) emalloc(sizeof(struct sortresult));
                sp->rank = rank;
                sp->fileinfo = (char *) mystrdup(fileinfo);
                sp->left = sp->right = NULL;
        }
        else {
                if (sp->rank < rank)
                        sp->left = (struct sortresult *)
                        addsortresult(sp->left, rank, fileinfo);
                else
                        sp->right = (struct sortresult *)
                        addsortresult(sp->right, rank, fileinfo);
        }

        return sp;
}

/* Prints the final results of a search.
*/

void printsortedresults(sp, num)
     struct sortresult *sp;
     float num;
{
        int rank;

        if (sp != NULL) {
                printsortedresults(sp->left, num);
                rank = (int) ((float) sp->rank * num);
                if (rank >= 999)
                        rank = 1000;
		if (maxhits) {
                	if (maxhits == -1)
				printf("%d %s", (rank <= 0) ? 1 :
				rank, sp->fileinfo);
			else if (maxhits > 0) {
				printf("%d %s", (rank <= 0) ? 1 :
				rank, sp->fileinfo);
				maxhits--;
			}
		}
                printsortedresults(sp->right, num);
        }
}

/* Reads a compressed line. This is just here for testing, etc.
*/

void getrawindexline(fp)
     FILE *fp;
{
        int c, inword;

        inword = 1;
        while ((c = fgetc(fp)) != EOF) {
                if (c == ':' && inword)
                        inword = 0;
                if (!inword) {
                        do {
                                c = fgetc(fp);
                                if (c == 0)
                                        return;
                        } while (c & 128);
                }
        }
}

/* Does an index file have a readable format?
*/

int isokindexheader(fp)
     FILE *fp;
{
	char line[MAXSTRLEN];

	fseek(fp, 0, 0);
	fgets(line, MAXSTRLEN, fp);
	if (line[strlen(line) - 1] == '\n')
		line[strlen(line) - 1] = '\0';
	if (strcmp(line, INDEXHEADER)) {
		fseek(fp, 0, 0);
		return 0;
	}
	fseek(fp, 0, 0);
	return 1;
}
