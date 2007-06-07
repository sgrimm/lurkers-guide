/*
** Copyright (C) 1995, Enterprise Integration Technologies Corp.        
** All Rights Reserved.
** Kevin Hughes, kevinh@eit.com 
** 3/11/94
*/

#include "swish.h"
#include "error.h"

/* Whoops, something bad happened...
*/

void progerr(errstring)
     char *errstring;
{
        fprintf(stderr, "swish: %s\n", errstring);
        exit(-1);
}
