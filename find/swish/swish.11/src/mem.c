/*
** Copyright (C) 1995, Enterprise Integration Technologies Corp.        
** All Rights Reserved.
** Kevin Hughes, kevinh@eit.com 
** 3/11/94
*/

#include "swish.h"
#include "mem.h"

/* Error-checking malloc()...
*/

void *emalloc(i)
     int i;
{
        void *p;
 
        if ((p = (void *) malloc(i)) == NULL)
                progerr("Ran out of memory (could not allocate enough)!");
        return p;
}

void *erealloc(ptr, i)
     void *ptr;
     int i;
{
        void *p;
 
        if ((p = (void *) realloc(ptr, i)) == NULL)
                progerr("Ran out of memory (could not reallocate enough)!");
        return p;
}
