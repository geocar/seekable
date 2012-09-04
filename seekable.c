#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	FILE *x;
	int ch;

	if (lseek(0, 0, SEEK_SET) == 0) {
		/* stdin is ALREADY a seekable; just rewind it */
		execvp(argv[1],argv+1);
		exit(111);
	}

	rewind(stdin); clearerr(stdin);

	x = tmpfile();
	if (!x) exit(111);

	while ((ch = fgetc(stdin)) != EOF) fputc(ch, x);
	if (ferror(stdin) || ferror(x) || fflush(x)) exit(111);

	close(0);
	if (dup(fileno(x)) != 0) exit(111);
	if (fcntl(0, F_SETFD, 0) != 0) exit(111);
	if (lseek(0, 0, SEEK_SET) != 0) exit(111);
	execvp(argv[1],argv+1);
	exit(111);
}
