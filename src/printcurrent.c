#include <stdio.h>
#include <fcntl.h>

char *getenv();

/*
 * Point the user to the current Lurker's Guide episode page.
 */
main()
{
	char	filename[100];

	readlink("/home/woodstock/www/docs/lurk/guide/current.html",
		filename, sizeof(filename));

	printf("Location: http://www.midwinter.com/lurk/guide/%s\n"
		"Content-Type: text/html\n"
		"\n"
		"<html><body>Try <a href=\"/lurk/guide/%s\">here.</a>"
		"</body></html>\n", filename, filename);
	fflush(stdout);
	close(1);

/*
	{
		int	fd;
		char	*c, *val;

		val = getenv("HTTP_USER_AGENT");
		for (c = val; *c; c++)
			if (*c == '/')
				*c = '#';
		
		chdir("/home/woodstock/koreth/browsers");
		fd = open(val, O_RDWR | O_APPEND | O_CREAT, 0644);
		write(fd, val, 1);
		close(fd);
	}
*/

	exit(0);
}
