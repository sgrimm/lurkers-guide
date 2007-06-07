/*
** Copyright (C) 1995, Enterprise Integration Technologies Corp.        
** All Rights Reserved.
** Kevin Hughes, kevinh@eit.com 
** 3/11/94
*/

#include "swish.h"
#include "file.h"

/* Is a file a directory?
*/

int isdirectory(path)
     char *path;
{
        struct stat stbuf;

        if (stat(path, &stbuf))
                return 0;
        return ((stbuf.st_mode & S_IFMT) == S_IFDIR) ? 1 : 0;
}

/* Is a file a regular file?
*/

int isfile(path)
     char *path;
{
        struct stat stbuf;

        if (stat(path, &stbuf))
                return 0;
        return ((stbuf.st_mode & S_IFMT) == S_IFREG) ? 1 : 0;
}

/* Is a file a link?
*/

int islink(path)
char *path;
{
        struct stat stbuf;

        if (lstat(path, &stbuf))
                return 0;
        return ((stbuf.st_mode & S_IFLNK) == S_IFLNK) ? 1 : 0;
}

/* Get the size, in bytes, of a file.
** Return -1 if there's a problem.
*/

int getsize(path)
     char *path;
{
        struct stat stbuf;

        if (stat(path, &stbuf))
                return -1;
        return stbuf.st_size;
}

/* Reads the configuration file and puts all the right options
** in the right variables and structures.
*/

void getdefaults(conffile, hasdir, hasindex, plimit, flimit, hasverbose)
     char *conffile;
     int *hasdir;
     int *hasindex;
     int *plimit;
     int *flimit;
     int hasverbose;
{
        int skiplen, gotdir, gotindex;
        char *c, line[MAXSTRLEN], value[MAXSTRLEN];
        FILE *fp;

	gotdir = gotindex = 0;

        if ((fp = fopen(conffile, "r")) == NULL) {
		sprintf(errorstr,
		"Couldn't open the configuration file \"%s\".", conffile);
                progerr(errorstr);
	}
        while (fgets(line, MAXSTRLEN, fp) != NULL) {
                if (line[0] == '#' || line[0] == '\n')
                        continue;
                if (c = (char *) lstrstr(line, "IndexDir")) {
                        c += strlen("IndexDir");
                        while (!*hasdir) {
                                strcpy(value, (char *) getword(c, &skiplen));
                                if (!skiplen | value[0] == '\0' ||
                                value[0] == '\n')
                                        break;
                                else {
                                        c += skiplen;
                                        dirlist = (struct swline *)
                                        addswline(dirlist, value);
					gotdir = 1;
                                }
                        }
                }
                else if (c = (char *) lstrstr(line, "IndexFile")) {
                        c += strlen("IndexFile");
                        while (!*hasindex) {
                                strcpy(value, (char *) getword(c, &skiplen));
                                if (!skiplen | value[0] == '\0' ||
                                value[0] == '\n')
                                        break;
                                else {
                                        c += skiplen;
                                        indexlist = (struct swline *)
                                        addswline(indexlist, value);
					gotindex = 1;
                                }
                        }
                }
/* IndexVerbose is supported for backwards compatibility */
		else if (c = (char *) lstrstr(line, "IndexVerbose")) {
                        c += strlen("IndexVerbose");
                        strcpy(value, (char *) getword(c, &skiplen));
			verbose = (lstrstr(value, "yes")) ? 3 : 0;
		}
		else if (c = (char *) lstrstr(line, "IndexReport")) {
                        c += strlen("IndexReport");
                        strcpy(value, (char *) getword(c, &skiplen));
                        if (!skiplen | value[0] == '\0' || value[0] == '\n')
                                continue;
                        else {
                                c += skiplen;
				if (!hasverbose)
					verbose = atoi(value);
                        }
		}
                else if (c = (char *) lstrstr(line, "IndexOnly")) {
                        c += strlen("IndexOnly");
                        while (1) {
                                strcpy(value, (char *) getword(c, &skiplen));
                                if (!skiplen | value[0] == '\0' ||
                                value[0] == '\n')
                                        break;
                                else {
                                        c += skiplen;
                                        suffixlist = (struct swline *)
                                        addswline(suffixlist, value);
                                }
                        }
                }
                else if (c = (char *) lstrstr(line, "NoContents")) {
                        c += strlen("NoContents");
                        while (1) {
                                strcpy(value, (char *) getword(c, &skiplen));
                                if (!skiplen | value[0] == '\0' ||
                                value[0] == '\n')
                                        break;
                                else {
                                        c += skiplen;
                                        nocontentslist = (struct swline *)
                                        addswline(nocontentslist, value);
                                }
                        }
                }
                else if ((c = (char *) lstrstr(line, "pathname contains")) &&
		(char *) lstrstr(line, "FileRules")) {
                        c += strlen("pathname contains");
                        while (1) {
                                strcpy(value, (char *) getword(c, &skiplen));
                                if (!skiplen | value[0] == '\0' ||
                                value[0] == '\n')
                                        break;
                                else {
                                        c += skiplen;
                                        pathconlist = (struct swline *)
                                        addswline(pathconlist, value);
                                }
                        }
                }
                else if ((c = (char *) lstrstr(line, "directory contains")) &&
		(char *) lstrstr(line, "FileRules")) {
                        c += strlen("directory contains");
                        while (1) {
                                strcpy(value, (char *) getword(c, &skiplen));
                                if (!skiplen | value[0] == '\0' ||
                                value[0] == '\n')
                                        break;
                                else {
                                        c += skiplen;
                                        dirconlist = (struct swline *)
                                        addswline(dirconlist, value);
                                }
                        }
                }
                else if ((c = (char *) lstrstr(line, "filename contains")) &&
		(char *) lstrstr(line, "FileRules")) {
                        c += strlen("filename contains");
                        while (1) {
                                strcpy(value, (char *) getword(c, &skiplen));
                                if (!skiplen | value[0] == '\0' ||
                                value[0] == '\n')
                                        break;
                                else {
                                        c += skiplen;
                                        fileconlist = (struct swline *)
                                        addswline(fileconlist, value);
                                }
                        }
                }
                else if ((c = (char *) lstrstr(line, "title contains")) &&
		(char *) lstrstr(line, "FileRules")) {
                        c += strlen("title contains");
                        while (1) {
                                strcpy(value, (char *) getword(c, &skiplen));
                                if (!skiplen | value[0] == '\0' ||
                                value[0] == '\n')
                                        break;
                                else {
                                        c += skiplen;
                                        titconlist = (struct swline *)
                                        addswline(titconlist, value);
                                }
                        }
                }
                else if ((c = (char *) lstrstr(line, "filename is")) &&
		(char *) lstrstr(line, "FileRules")) {
                        c += strlen("filename is");
                        while (1) {
                                strcpy(value, (char *) getword(c, &skiplen));
                                if (!skiplen | value[0] == '\0' ||
                                value[0] == '\n')
                                        break;
                                else {
                                        c += skiplen;
                                        fileislist = (struct swline *)
                                        addswline(fileislist, value);
                                }
                        }
                }
                else if (c = (char *) lstrstr(line, "IgnoreWords")) {
                        c += strlen("IgnoreWords");
                        while (1) {
                                strcpy(value, (char *) getword(c, &skiplen));
                                if (!skiplen | value[0] == '\0' ||
                                value[0] == '\n')
                                        break;
                                else {
                                        c += skiplen;
					if (lstrstr(value, "SwishDefault"))
						readdefaultstopwords();
					else
						addstophash(value);
                                }
                        }
                }
                else if ((c = (char *) lstrstr(line, "IgnoreLimit"))) {
                        c += strlen("IgnoreLimit");
                        strcpy(value, (char *) getword(c, &skiplen));
                        if (!skiplen | value[0] == '\0' || value[0] == '\n')
                                continue;
                        else {
                                c += skiplen;
				*plimit = atoi(value);
                        }
                        strcpy(value, (char *) getword(c, &skiplen));
                        if (!skiplen | value[0] == '\0' || value[0] == '\n')
                                continue;
                        else {
                                c += skiplen;
				*flimit = atoi(value);
                        }
                }
                else if (c = (char *) lstrstr(line, "ReplaceRules")) {
                        c += strlen("ReplaceRules");
                        while (1) {
                                strcpy(value, (char *) getword(c, &skiplen));
                                if (!skiplen | value[0] == '\0' ||
                                value[0] == '\n')
                                        break;
                                else {
                                        c += skiplen;
                                        replacelist = (struct swline *)
                                        addswline(replacelist, value);
                                }
                        }
                }
                else if ((char *) getconfvalue(line, "FollowSymLinks",
		value) != NULL)
                        followsymlinks = (lstrstr(value, "yes")) ? 1 : 0;
                else if ((char *) getconfvalue(line, "IndexName",
		value) != NULL)
                        strcpy(indexn, value);
                else if ((char *) getconfvalue(line, "IndexDescription",
		value) != NULL)
                        strcpy(indexd, value);
                else if ((char *) getconfvalue(line, "IndexPointer",
		value) != NULL)
                        strcpy(indexp, value);
                else if ((char *) getconfvalue(line, "IndexAdmin",
		value) != NULL)
                        strcpy(indexa, value);
        }
        fclose(fp);

	if (gotdir && !(*hasdir))
		*hasdir = 1;
	if (gotindex && !(*hasindex))
		*hasindex = 1;
}
