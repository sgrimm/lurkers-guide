/*
** Copyright (C) 1995, Enterprise Integration Technologies Corp.        
** All Rights Reserved.
** Kevin Hughes, kevinh@eit.com 
** 3/11/94
*/

struct indexentry {
        char *word;
        struct result *result;
};

struct indexfileinfo {
        int filenum;
        char *fileinfo;
	char *path;
        struct indexfileinfo *next;
};

struct mapentry {
	int oldnum;
	int newnum;
	struct mapentry *next;
};

struct markentry {
	int num;
	struct markentry *next;
};

struct indexfileinfo *indexfilehashlist[BIGHASHSIZE];
struct mapentry *mapentrylist[BIGHASHSIZE];
struct markentry *markentrylist[BIGHASHSIZE];

void readmerge();
int getindexfilenum();
void addfilenums();
struct indexentry *readindexline();
void addindexfilelist();
char *extractpath();
char *lookupindexfilenum();
int lookupindexfilepath();
struct indexentry *mergeindexentries();
void printindexentry();
void remap();
int getmap();
void marknum();
int ismarked();
void initmarkentrylist();
void initindexfilehashlist();
void freeindexentry();
int encodefilenum();
int decodefilenum();
