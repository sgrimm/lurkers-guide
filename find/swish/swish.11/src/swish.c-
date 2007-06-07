/*
** Copyright (C) 1995, Enterprise Integration Technologies Corp.        
** All Rights Reserved.
** Kevin Hughes, kevinh@eit.com 
** 3/11/94
*/

#define MAIN_FILE
#include "swish.h"

int main(argc, argv)
     int argc;
     char **argv;
{
        char c, word[MAXWORDLEN], wordlist[MAXSTRLEN],
		maxhitstr[MAXSTRLEN], structstr[MAXSTRLEN];
	char tmpindex1[MAXFILELEN], tmpindex2[MAXFILELEN],
		index1[MAXSTRLEN], index2[MAXSTRLEN],
		index3[MAXSTRLEN], index4[MAXSTRLEN];
        int i, j, hasindex, hasdir, hasconf, hasverbose, structure,
		totalfiles, stopwords, index, decode, merge;
	long offsetstart, starttime, stoptime, plimit, flimit;
        FILE *fp1, *fp2;
        struct file *filep;
        struct entry *entryp;
	struct swline *conflist, *tmplist;

        index = decode = merge = 0;
        hasindex = hasdir = hasconf = hasverbose = 0;
	followsymlinks = stopwords = 0;
        totalwords = stopwords = 0;
        filelist = NULL;
        entrylist = NULL;
	dirlist = indexlist = conflist = tmplist = NULL;
        suffixlist = nocontentslist = replacelist = NULL;
        pathconlist = dirconlist = fileconlist = NULL;
	titconlist = fileislist = NULL;
        maxhits = -1;
        verbose = VERBOSE;
	plimit = PLIMIT;
	flimit = FLIMIT;
	structure = 1;
        wordlist[0] = '\0';
	structstr[0] = '\0';
	indexn[0] = '\0';
	indexd[0] = '\0';
	indexp[0] = '\0';
	indexa[0] = '\0';

        if (argc == 1)
                usage();
        while (--argc > 0) {
                ++argv;
                if ((*argv)[0] != '-')
                        usage();
                c = (*argv)[1];
		if ((*argv)[2] != '\0' && isalpha((*argv)[2]))
			usage();
                if (c == 'i') {
                        index = 1;
                        while ((argv + 1)[0] != '\0' && *(argv + 1)[0] != '-') {
				dirlist = (struct swline *)
				addswline(dirlist, (++argv)[0]);
                                argc--;
                        }
                }
                else if (c == 'w') {
                        while ((argv + 1)[0] != '\0' && *(argv + 1)[0] != '-') {                                strcpy(word, (++argv)[0]);
                                argc--;
                                sprintf(wordlist, "%s%s%s", wordlist,
                                (wordlist[0] == '\0') ? "" : " ", word);
                        }
                }
                else if (c == 'f') {
                        while ((argv + 1)[0] != '\0' && *(argv + 1)[0] != '-') {
				indexlist = (struct swline *)
				addswline(indexlist, (++argv)[0]);
                                argc--;
                        }
                }
                else if (c == 'c') {
                        index = 1;
                        hasconf = 1;
                        while ((argv + 1)[0] != '\0' && *(argv + 1)[0] != '-') {
				conflist = (struct swline *)
				addswline(conflist, (++argv)[0]);
                                argc--;
                        }
                }
                else if (c == 'l') {
			followsymlinks = 1;
			argc--;
		}
                else if (c == 'm') {
			if ((argv + 1)[0] == '\0')
				maxhits = -1;
			else {
				strcpy(maxhitstr, (++argv)[0]);
				if (lstrstr(maxhitstr, "all"))
					maxhits = -1;
				else if (isdigit(maxhitstr[0]))
                        		maxhits = atoi(maxhitstr);
				else
					maxhits = -1;
                        	argc--;
			}
                }
                else if (c == 't') {
			if ((argv + 1)[0] == '\0')
				progerr("Specify tag fields (HBtheca).");
			else {
				structure = 0;
                        	strcpy(structstr, (++argv)[0]);
                        	argc--;
			}
                }
                else if (c == 'v') {
			hasverbose = 1;
			if ((argv + 1)[0] == '\0') {
				verbose = 3;
				break;
			}
			else if (!isdigit((argv + 1)[0][0]))
				verbose = 3;
			else
                        	verbose = atoi((++argv)[0]);
                        argc--;
                }
                else if (c == 'V')
                        printversion();
                else if (c == 'z' || c == 'h' || c == '?')
                        usage();
                else if (c == 'M') {
			merge = 1;
                        while ((argv + 1)[0] != '\0' && *(argv + 1)[0] != '-') {
				indexlist = (struct swline *)
				addswline(indexlist, (++argv)[0]);
                                argc--;
                        }
		}
                else if (c == 'D') {
			decode = 1;
                        while ((argv + 1)[0] != '\0' && *(argv + 1)[0] != '-') {
				indexlist = (struct swline *)
				addswline(indexlist, (++argv)[0]);
                                argc--;
                        }
                }
                else
                        usage();
                if (argc == 0)
                        break;
        }

	hasdir = (dirlist == NULL) ? 0 : 1;
	hasindex = (indexlist == NULL) ? 0 : 1;

	if (index && merge)
		index = 0;

	if (decode) {

		if (!hasindex)
			progerr("Specify the index file to decode.");

		while (indexlist != NULL) {

                	if ((fp1 = fopen(indexlist->line, "r")) == NULL) {
				sprintf(errorstr,
				"Couldn't open the index file \"%s\".",
				indexlist->line);
				progerr(errorstr);
			}
			if (!isokindexheader(fp1)) {
				sprintf(errorstr,
				"\"%s\" has an unknown format.",
				indexlist->line);
				progerr(errorstr);
			}

                	decompress(fp1);
			putchar('\n');
                	fclose(fp1);

			indexlist = indexlist->next;
		}
		exit(0);

	}
        else if (index) {

                if (hasconf)
			while (conflist != NULL) {
                        	getdefaults(conflist->line, &hasdir, &hasindex,
				&plimit, &flimit, hasverbose);
				conflist = conflist->next;
			}
		if (!hasindex)
			indexlist = (struct swline *)
			addswline(indexlist, INDEXFILE);
		if (!hasdir)
			progerr("Specify directories or files to index.");

		if (verbose < 0)
			verbose = 0;
		if (verbose > 3)
			verbose = 3;
		if (verbose)
			starttime = getthetime();

		while (dirlist != NULL) {
			if (isdirectory(dirlist->line)) {
				if (verbose >= 2)
					printf("\nChecking dir \"%s\"...\n",
					dirlist->line);
	                	indexadir(dirlist->line);
			}
			else if (isfile(dirlist->line)) {
				if (verbose >= 2)
					printf("\nChecking file \"%s\"...\n",
					dirlist->line);
	                	indexafile(dirlist->line);
			}
			dirlist = dirlist->next;
		}

                if ((fp1 = fopen(indexlist->line, "w")) == NULL) {
			sprintf(errorstr,
			"Couldn't write the index file \"%s\".",
			indexlist->line);
                        progerr(errorstr);
		}

		if (verbose > 1)
			putchar('\n');
		if (verbose)
			printf("Removing very common words... ");

                filep = filelist;
		totalfiles = getfilecount(filep);
		entryp = entrylist;
		stopwords = removestops(entryp, totalfiles, plimit, flimit);

		if (verbose) {
			if (stopwords)
				printf("%d word%s removed.\n",
				stopwords, (stopwords == 1) ? "" : "s");
			else
		                printf("no words removed.\n");
                        printf("Writing main index... ");
		}

		printheader(fp1, indexlist->line, totalwords, totalfiles);

		offsetstart = ftell(fp1);
                for (i = 0; i < MAXCHARS; i++)
                        fprintf(fp1, "%016li", offsets[i]);
                fputc('\n', fp1);

                printindex(entrylist, fp1);
		printstopwords(fp1);

		if (verbose) {
			if (totalwords)
				printf("%d unique word%s indexed.\n",
				totalwords, (totalwords == 1) ? "" : "s");
			else
		                printf("no unique words indexed.\n");
                        printf("Writing file index... ");
		}

                printfilelist(filelist, fp1);
		printfileoffsets(fp1);
                fclose(fp1);

                fp2 = fopen(indexlist->line, "r+");
		fseek(fp2, offsetstart, 0);
                for (i = 0; i < MAXCHARS; i++)
                        fprintf(fp2, "%016li", offsets[i]);
                fclose(fp2);

		if (verbose) {
			if (totalfiles)
				printf("%d file%s indexed.\n", totalfiles,
				(totalfiles == 1) ? "" : "s");
			else
				printf("no files indexed.\n");

			stoptime = getthetime();

			printrunning(starttime, stoptime);
                	printf("Indexing done!\n");
		}
#ifdef INDEXPERMS
                chmod(indexlist->line, INDEXPERMS);
#endif
		exit(0);

        }
        else if (merge) {

		if (indexlist == NULL)
			progerr("Specify index files and an output file.");
                if (hasconf)
			while (conflist != NULL) {
                        	getdefaults(conflist->line, &hasdir, &hasindex,
				&plimit, &flimit, hasverbose);
				conflist = conflist->next;
			}

		tmplist = indexlist;
		for (i = 0; tmplist != NULL; i++) {
			strcpy(index4, tmplist->line);
			tmplist = tmplist->next;
		}
		j = i - 2;
		if (i < 3)
			progerr("Specify index files and an output file.");

		sprintf(tmpindex1, tmpnam(NULL));
		sprintf(tmpindex2, tmpnam(NULL));

		i = 1;
		strcpy(index1, indexlist->line);
		indexlist = indexlist->next;
		while (i <= j) {
			strcpy(index2, indexlist->line);
			if (i % 2) {
				if (i != 1)
					strcpy(index1, tmpindex2);
				strcpy(index3, tmpindex1);
			}
			else {
				strcpy(index1, tmpindex1);
				strcpy(index3, tmpindex2);
			}
			if (i == j)
				strcpy(index3, index4);
			readmerge(index1, index2, index3);
			indexlist = indexlist->next;
			i++;
		}
#ifdef INDEXPERMS
                chmod(index3, INDEXPERMS);
#endif
		if (isfile(tmpindex1))
			remove(tmpindex1);
		if (isfile(tmpindex2))
			remove(tmpindex2);

	}
        else {

		for (i = 0; structstr[i] != '\0'; i++)
			switch (structstr[i]) {
			case 'H':
				structure |= IN_HEAD;
				break;
			case 'B':
				structure |= IN_BODY;
				break;
			case 't':
				structure |= IN_TITLE;
				break;
			case 'h':
				structure |= IN_HEADER;
				break;
			case 'e':
				structure |= IN_EMPHASIZED;
				break;
			case 'c':
				structure |= IN_COMMENTS;
				break;
			default:
				structure |= IN_FILE;
				break;
			}

		if (maxhits <= 0)
			maxhits = -1;
		if (!hasindex)
			indexlist = (struct swline *)
			addswline(indexlist, INDEXFILE);
		search(wordlist, indexlist, structure);

	}

        exit(0);
}

/* Gets the current time in seconds since the epoch.
*/

long getthetime()
{
        long thetime;
        time_t tp;

        thetime = (long) time(&tp);
        return thetime;
}

/* Prints the running time (the time it took for indexing).
*/

void printrunning(starttime, stoptime)
     long starttime;
     long stoptime;
{
        int minutes, seconds;

        minutes = (stoptime - starttime) / SECSPERMIN;
        seconds = (stoptime - starttime) % SECSPERMIN;
        printf("Running time: ");
        if (minutes)
                printf("%d minute%s", minutes, (minutes == 1) ? "" : "s");
        if (minutes && seconds)
                printf(", ");
        if (seconds)
                printf("%d second%s", seconds, (seconds == 1) ? "" : "s");
        if (!minutes && !seconds)
                printf("Less than a second");
        printf(".\n");
}

/* Prints the SWISH usage.
*/

void usage()
{
	printf("  usage: swish [-i dir file ... ] [-c file] [-f file] [-l] [-v (num)]\n");
	printf("         swish -w word1 word2 ... [-f file1 file2 ...] [-m num] [-t str]\n");
	printf("         swish -M index1 index2 ... outputfile\n");
	printf("         swish -D file\n");
	printf("         swish -V\n");
	putchar('\n');
        printf("options: defaults are in brackets\n");
        printf("         -i : create an index from the specified files\n");
        printf("         -w : search for words \"word1 word2 ...\"\n");
        printf("         -t : tags to search in - specify as a string\n");
        printf("              \"HBthec\" - in head, body, title, header,\n");
	printf("              emphasized, or comments\n");
        printf("         -f : index file to create or search from [%s]\n", INDEXFILE);
        printf("         -c : configuration file to use for indexing\n");
        printf("         -v : verbosity level (0 to 3) [%d]\n", VERBOSE);
        printf("         -l : follow symbolic links when indexing\n");
        printf("         -m : the maximum number of results to return [%d]\n", MAXHITS);
	printf("         -M : merges index files\n");
	printf("         -D : decodes an index file\n");
        printf("         -V : prints the current version\n\n");
        printf("version: %s\n", VERSION);
        printf("   docs: http://www.eit.com/software/swish/\n");
        exit(1);
}

void printversion()
{
        printf("SWISH %s\n", VERSION);
        exit(0);
}
