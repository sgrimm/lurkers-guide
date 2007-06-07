/* This is a generic function that adds lines to a list -
** these lists hold file rules, index files, files to be
** indexed, etc.
*/

#include "swish.h"
#include "list.h"

struct swline *addswline(rp, line)
     struct swline *rp;
     char *line;
{
        struct swline *newnode;
        static struct swline *nodep;

        newnode = (struct swline *) emalloc(sizeof(struct swline));
        newnode->line = (char *) mystrdup(line);
        newnode->next = NULL;

        if (rp == NULL)
                rp = newnode;
        else
                nodep->next = newnode;

        nodep = newnode;

        return rp;
}
