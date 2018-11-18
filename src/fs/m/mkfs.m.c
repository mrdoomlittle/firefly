# include <stdio.h>
# include <sys/stat.h>
# include <unistd.h>
# include <fcntl.h>
# include "m.h"
int main() {
	int fd;
	fd = open("test.bin", O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR);



	close(fd);
}
