/*
** Copyright (C) 1995, Enterprise Integration Technologies Corp.        
** All Rights Reserved.
** Kevin Hughes, kevinh@eit.com 
** 3/11/94
*/

#include "swish.h"
#include "merge.h"

/* The main merge functions - it accepts three file names.
** This is a bit hairy. It basically acts as a zipper,
** zipping up both index files into one.
*/

void readmerge(file1, file2, outfile)
     char *file1;
     char *file2;
     char *outfile;
{
	int i, j, indexfilenum1, indexfilenum2, result, totalfiles,
		skipwords, skipfiles;
	long limit1, limit2, fileinfo1, fileinfo2, offsetstart;
	char line[MAXSTRLEN];
	struct indexentry *ip1, *ip2, *ip3;
	struct indexentry *buffer1, *buffer2;
	FILE *fp1, *fp2, *fp3;

	initindexfilehashlist();

	if ((fp1 = fopen(file1, "r")) == NULL) {
		sprintf(errorstr, "Couldn't read the index file \"%s\".",
		file1);
		progerr(errorstr);
	}
	if (!isokindexheader(fp1)) {
		sprintf(errorstr, "\"%s\" has an unknown format.",
		file1);
		progerr(errorstr);
	}
	if ((fp2 = fopen(file2, "r")) == NULL) {
		sprintf(errorstr, "Couldn't read the index file \"%s\".",
		file2);
		progerr(errorstr);
	}
	if (!isokindexheader(fp2)) {
		sprintf(errorstr, "\"%s\" has an unknown format.",
		file2);
		progerr(errorstr);
	}

	ip1 = ip2 = ip3 = NULL;
	buffer1 = buffer2 = NULL;
	if (verbose)
		printf("Counting files... ");
	indexfilenum1 = getindexfilenum(fp1);
	indexfilenum2 = getindexfilenum(fp2);
	totalfiles = indexfilenum1 + indexfilenum2;
	if (verbose) {
		printf("%d files.\n", indexfilenum1 + indexfilenum2);
		printf("Reading stopwords...");
	}
	readoffsets(fp1);
	readstopwords(fp1);
	limit1 = offsets[STOPWORDPOS];
	fileinfo1 = offsets[FILELISTPOS];
	readoffsets(fp2);
	readstopwords(fp2);
	limit2 = offsets[STOPWORDPOS];
	fileinfo2 = offsets[FILELISTPOS];

	if (verbose)
		printf("\nReading file info...");
	fseek(fp1, fileinfo1, 0);
	for (i = 1; i <= indexfilenum1; i++) {
		fgets(line, MAXSTRLEN, fp1);
		addindexfilelist(i, line, &totalfiles);
	}
	fseek(fp2, fileinfo2, 0);
	for (i = 1; i <= indexfilenum2; i++) {
		fgets(line, MAXSTRLEN, fp2);
		addindexfilelist(i + indexfilenum1, line, &totalfiles);
	}

	if ((fp3 = fopen(outfile, "w")) == NULL) {
		sprintf(errorstr,
		"Couldn't write the merged index file \"%s\".",
		outfile);
		progerr(errorstr);
	}

	if (verbose)
		printf("\nMerging words... ");

	printheader(fp3, outfile, 0, totalfiles);

        offsetstart = ftell(fp3);
        for (i = 0; i < MAXCHARS; i++)
                fprintf(fp3, "%016li", offsets[i]);
        fputc('\n', fp3);

	readoffsets(fp1);
	readoffsets(fp2);

	for (i = 0; i < MAXCHARS; i++)
		offsets[i] = 0;

	skipwords = 0;
	while (1) {
		if (buffer1 == NULL) {
			ip1 = (struct indexentry *) readindexline(fp1, limit1);
			if (ip1 == NULL)
				break;
			buffer1 = ip1;
		}
		if (buffer2 == NULL) {
			ip2 = (struct indexentry *) readindexline(fp2, limit2);
			if (ip2 == NULL)
				break;
			addfilenums(ip2, indexfilenum1);
			buffer2 = ip2;
		}
		result = wordcompare(ip1->word, ip2->word);
		if (!result) {
			ip3 = (struct indexentry *) mergeindexentries(ip1, ip2);
			printindexentry(ip3, fp3);
			freeindexentry(ip1);
			freeindexentry(ip2);
			freeindexentry(ip3);
			buffer1 = buffer2 = NULL;
			skipwords++;
		}
		else if (result < 0) {
			printindexentry(ip1, fp3);
			freeindexentry(ip1);
			buffer1 = NULL;
		}
		else {
			printindexentry(ip2, fp3);
			freeindexentry(ip2);
			buffer2 = NULL;
		}
	}

	if (verbose) {
		if (skipwords)
			printf("%d redundant word%s.", skipwords,
			(skipwords == 1) ? "" : "s");
		else
			printf("no redundant words.");
	}

        printstopwords(fp3);

	if (verbose)
		printf("\nMerging file info... ");

	offsets[FILELISTPOS] = ftell(fp3);
	for (i = j = 1; i <= indexfilenum1 + indexfilenum2; i++)
		if (getmap(i) == j) {
			addtofilehashlist(j++ - 1, ftell(fp3));
			fprintf(fp3, "%s", lookupindexfilenum(i));
		}

	skipfiles = (indexfilenum1 + indexfilenum2) - totalfiles;
	if (verbose) {
		if (skipfiles)
			printf("%d redundant file%s.", skipfiles,
			(skipfiles == 1) ? "" : "s");
		else
			printf("no redundant files.");
	}

	printfileoffsets(fp3);

	fseek(fp3, offsetstart, 0);
        for (i = 0; i < MAXCHARS; i++)
                fprintf(fp3, "%016li", offsets[i]);
	fclose(fp3);

	fclose(fp1);
	fclose(fp2);

	if (verbose)
		printf("\nDone.\n");
}

/* Gets the number of files in an index file.
*/

int getindexfilenum(fp)
     FILE *fp;
{
	int i;
	char line[MAXSTRLEN];

	readoffsets(fp);
	fseek(fp, offsets[FILELISTPOS], 0);

	i = 0;
	while(ftell(fp) < offsets[FILEOFFSETPOS]) {
		fgets(line, MAXSTRLEN, fp);
		i++;
	}

	if (ftell(fp) != offsets[FILEOFFSETPOS])
		fprintf(stderr, "FILEOFFSETPOS is incorrect; corrupt index\n");

	return i;
}

/* This adds an offset to the file numbers in a particular
** result list. For instance, file 1 has file numbers going from
** 1 to 10, but so does file 2, so I have to add 10 to all the
** file numbers in file 2 before merging.
*/

void addfilenums(ip, num)
     struct indexentry *ip;
     int num;
{
	struct result *rp;

	rp = ip->result;
	while (rp != NULL) {
		rp->filenum =
		encodefilenum(getmap(decodefilenum(rp->filenum) + num));
		rp = rp->next;
	}
}

/* This reads the next line in the index file and puts the results
** in a result structure.
*/

struct indexentry *readindexline(fp, limit)
     FILE *fp;
     long limit;
{
        int i, c, x, countnum, rank, filenum, structure;
        char fileword[MAXWORDLEN];
        struct result *rp;
	struct indexentry *ip;

        rp = NULL;

	if (limit == ftell(fp))
		return NULL;
        for (i = 0; (c = fgetc(fp)) != 0; ) {
                if (c == ':') {
                        fileword[i] = '\0';
                        break;
                }
                else
                        fileword[i++] = c;
        }

        countnum = 1;

        ungetc(c, fp);
        while ((c = fgetc(fp)) != 0) {
                x = 0;
                do {
                        c = fgetc(fp);
                        if (c == 0)
                                break;
                        x *= 128;
                        x += c & 127;
                } while (c & 128);
		if (c == 0)
			break;
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
                                addtoresultlist(rp, filenum,
				rank, structure);
				countnum = 1;
                        }
                }
        }

	ip = (struct indexentry *) emalloc(sizeof(struct indexentry));
	ip->word = (char *) mystrdup(fileword);
	ip->result = rp;

        return ip;
}

/* This puts all the file info into a hash table so that it can
** be looked up by its pathname and filenumber. This is how
** we find redundant file information.
*/

void addindexfilelist(num, info, totalfiles)
     int num;
     char *info;
     int *totalfiles;
{
	int i;
	static int j;
	unsigned hashval;
	char tmpstr[MAXSTRLEN], path[MAXSTRLEN];
	struct indexfileinfo *ip1, *ip2;

	strcpy(path, extractpath(info));

	i = lookupindexfilepath(path);
	if (i != -1) {
		*totalfiles = *totalfiles - 1;
		remap(num, i);
		return;
	}

	remap(num, j + 1);
	j++;

	ip1 = (struct indexfileinfo *) emalloc(sizeof(struct indexfileinfo));
	ip1->filenum = num;
	ip1->fileinfo = (char *) mystrdup(info);
	ip1->path = (char *) mystrdup(path);

	sprintf(tmpstr, "%d", num);
	hashval = bighash(tmpstr);
	ip1->next = indexfilehashlist[hashval];
	indexfilehashlist[hashval] = ip1;

	ip2 = (struct indexfileinfo *) emalloc(sizeof(struct indexfileinfo));
	ip2->filenum = num;
	ip2->fileinfo = (char *) mystrdup(info);
	ip2->path = (char *) mystrdup(path);

	hashval = bighash(path);
	ip2->next = indexfilehashlist[hashval];
	indexfilehashlist[hashval] = ip2;
}

/* This extracts the pathname information from the file information
** line as stored in the index file.
*/

char *extractpath(s)
     char *s;
{
	int i;
	static char path[MAXSTRLEN];

	for (i = 0; s[i] && s[i] != '\"'; i++)
		path[i] = s[i];
	path[i - 1] = '\0';
	path[i] = '\0';

	return path;
}

/* This returns the file information corresponding to a file number.
*/

char *lookupindexfilenum(num)
     int num;
{
	unsigned hashval;
	char tmpstr[MAXSTRLEN];
	struct indexfileinfo *ip;

	sprintf(tmpstr, "%d", num);
	hashval = bighash(tmpstr);
	ip = indexfilehashlist[hashval];

	while (ip != NULL) {
		if (ip->filenum == num)
			return ip->fileinfo;
		ip = ip->next;
	}
	return NULL;
}

/* This returns the file number corresponding to a pathname.
*/

int lookupindexfilepath(path)
     char *path;
{
	unsigned hashval;
	struct indexfileinfo *ip;

	hashval = bighash(path);
	ip = indexfilehashlist[hashval];

	while (ip != NULL) {
		if (!strcmp(ip->path, path))
			return ip->filenum;
		ip = ip->next;
	}
	return -1;
}

/* This simply concatenates two information lists that correspond
** to a word found in both index files.
*/

struct indexentry *mergeindexentries(ip1, ip2)
     struct indexentry *ip1;
     struct indexentry *ip2;
{
	struct result *newrp, *rp1, *rp2;
	struct indexentry *ep;

	rp1 = ip1->result;
	rp2 = ip2->result;
	newrp = NULL;

	while (rp1 != NULL) {
		newrp = (struct result *) addtoresultlist(newrp,
		rp1->filenum, rp1->rank, rp1->structure);
		rp1 = rp1->next;
	}
	while (rp2 != NULL) {
		newrp = (struct result *) addtoresultlist(newrp,
		rp2->filenum, rp2->rank, rp2->structure);
		rp2 = rp2->next;
	}

	ep = (struct indexentry *) emalloc(sizeof(struct indexentry));
	ep->word = (char *) mystrdup(ip1->word);
	ep->result = newrp;

	return ep;
}

/* This prints a new word entry into the merged index file,
** removing redundant file information as it goes along.
*/

void printindexentry(ip, fp)
     struct indexentry *ip;
     FILE *fp;
{
	int i, num;
	struct result *rp;

	for (i = 0; indexchars[i] != '\0'; i++)
		if ((ip->word)[0] == indexchars[i] && !offsets[i])
			offsets[i] = ftell(fp);
	fprintf(fp, "%s:", ip->word);
	initmarkentrylist();
	rp = ip->result;
	while (rp != NULL) {
		num = rp->filenum;
		if (!ismarked(num)) {
			marknum(num);
			compress(num, fp);
			compress(rp->rank, fp);
			compress(rp->structure, fp);
		}
		rp = rp->next;
	}
	fputc(0, fp);
}

/* This associates a number with a new number.
** This function is used to remap file numbers from index
** files to a new merged index file.
*/

void remap(oldnum, newnum)
     int oldnum;
     int newnum;
{
	unsigned hashval;
	char tmpstr[MAXSTRLEN];
	struct mapentry *mp;

	mp = (struct mapentry *) emalloc(sizeof(struct mapentry));
	mp->oldnum = oldnum;
	mp->newnum = newnum;

	sprintf(tmpstr, "%d", oldnum);
	hashval = bighash(tmpstr);
	mp->next = mapentrylist[hashval];
	mapentrylist[hashval] = mp;
}

/* This retrieves the number associated with another.
*/

int getmap(num)
     int num;
{
	unsigned hashval;
	char tmpstr[MAXSTRLEN];
	struct mapentry *mp;

	sprintf(tmpstr, "%d", num);
	hashval = bighash(tmpstr);
	mp = mapentrylist[hashval];

	while (mp != NULL) {
		if (mp->oldnum == num)
			return mp->newnum;
		mp = mp->next;
	}
	return num;
}

/* This marks a number as having been printed.
*/

void marknum(num)
     int num;
{
	unsigned hashval;
	char tmpstr[MAXSTRLEN];
	struct markentry *mp;

	mp = (struct markentry *) emalloc(sizeof(struct markentry));
	mp->num = num;

	sprintf(tmpstr, "%d", num);
	hashval = bighash(tmpstr);
	mp->next = markentrylist[hashval];
	markentrylist[hashval] = mp;
}

/* Has a number been printed?
*/

int ismarked(num)
     int num;
{
	unsigned hashval;
	char tmpstr[MAXSTRLEN];
	struct markentry *mp;

	sprintf(tmpstr, "%d", num);
	hashval = bighash(tmpstr);
	mp = markentrylist[hashval];

	while (mp != NULL) {
		if (mp->num == num)
			return 1;
		mp = mp->next;
	}
	return 0;
}

/* Initialize the marking list.
*/

void initmarkentrylist()
{
	int i;
	struct markentry *mp;

	for (i = 0; i < BIGHASHSIZE; i++) {
		mp = markentrylist[i];
		if (mp != NULL)
			free(mp);
		markentrylist[i] = NULL;
	}
}

/* Initialize the main file list.
*/

void initindexfilehashlist()
{
	int i;
	struct indexfileinfo *ip;

	for (i = 0; i < BIGHASHSIZE; i++) {
		ip = indexfilehashlist[i];
		if (ip != NULL)
			free(ip);
		indexfilehashlist[i] = NULL;
	}
}

/* Frees up used index entries, my best attempt at memory management...
** I still have bytes leaking elsewhere...
*/

void freeindexentry(ip)
     struct indexentry *ip;
{
	struct result *rp, *oldp;

	free(ip->word);
	rp = ip->result;
	while (rp != NULL) {
		oldp = rp;
		rp = rp->next;
		free(oldp);
	}
	free(ip);
}

/* Translates a file number into something that can be compressed.
*/

int encodefilenum(num)
     int num;
{
	int i, j;

	for (i = j = 0; i != num; i++) {
		j++;
		if (!(j % 128))
			j++;
	}
	return j;
}

/* Translates a compressed file number into a correct file number.
*/

int decodefilenum(num)
     int num;
{
        int i, extra;

        for (i = 1, extra = 0; i < num; i++)
                if (!(i % 128)) {
                        extra++;
                        i++;
                }
        num -= extra;

	return num;
}
