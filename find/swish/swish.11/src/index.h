/*
** Copyright (C) 1995, Enterprise Integration Technologies Corp.        
** All Rights Reserved.
** Kevin Hughes, kevinh@eit.com 
** 3/11/94
*/

void indexadir();
void indexafile();
void printfiles();
void printdirs();
struct sortentry *addsortentry();
struct entry *addentry();
struct file *addtofilelist();
int getfilecount();
char *getthedate();
int countwords();
int countwordstr();
int getstructure();
int parsecomment();
int removestops();
int getrank();
void printheader();
void printindex();
void printfilelist();
void printstopwords();
void printfileoffsets();
void compress();
void decompress();
char *ruleparse();
