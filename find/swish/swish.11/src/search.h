/*
** Copyright (C) 1995, Enterprise Integration Technologies Corp.        
** All Rights Reserved.
** Kevin Hughes, kevinh@eit.com 
** 3/11/94
*/

void search();
struct swline *fixnot();
struct swline *expandstar();
char *getmatchword();
void getheader();
void readoffsets();
void readstopwords();
void readfileoffsets();

struct result *parseterm();
struct result *operate();
char *lookupfile();
struct result *getfileinfo();

int isrule();
int isbooleanrule();
int isunaryrule();
int getrulenum();

struct result *andresultlists();
struct result *orresultlists();
struct result *notresultlist();
struct result *addtoresultlist();

struct sortresult *addsortresult();
void printsortedresults();

void getrawindexline();
int isokindexheader();
