/*
 * Select an episode list to show based on a control file.
 */

#define LISTFILE "/home/koreth/lurk/internal/domainlist"
char * BASEDIR = "/home/koreth/lurk/";
#define ERRORFILE "/home/koreth/lurk/internal/dom-error.html"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

char *getenv();

main()
{
	FILE		*fp;
	char		domain[100], filename[500], bestname[500];
	char		*hisdomain, *mapbuf;
	int		matchlen = 0, hislen, thislen, i, c, fd;
	struct stat	st;

	chdir(BASEDIR);

	printf("Content-type: text/html\n");

	/*
	 * Get the remote domain name and make it lower case.
	 */
	hisdomain = getenv("REMOTE_HOST");
	if (hisdomain == NULL)
		hisdomain = strdup("com");
	hislen = strlen(hisdomain);
	for (i = 0; i < hislen; i++)
		hisdomain[i] = tolower(hisdomain[i]);

	/*
	 * Now read list entries and find the filename with the longest
	 * matching domain name.
	 */
	fp = fopen(LISTFILE, "r");
	if (fp)
	{
		/* First entry is the default filename. */
		fscanf(fp, "%s %s", domain, bestname);

		while (fscanf(fp, "%s %s", domain, filename) > -1)
		{
			thislen = strlen(domain);
			if (thislen > matchlen && thislen <= hislen)
			{
				if (! strcmp(domain, hisdomain+hislen-thislen))
				{
					strcpy(bestname, filename);
					matchlen = thislen;
				}
			}
		}

		fclose(fp);
	}
	else
		strcpy(bestname, ERRORFILE);

	printf("Location: http://%s/lurk/%s\n\n", getenv("SERVER_NAME"),
		bestname);
}
