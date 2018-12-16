# include <stdio.h>
# include <sys/stat.h>
# include <unistd.h>
# include <fcntl.h>
# include "m.h"
int main() {
	int fd;
	fd = open("test.bin", O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR);
	struct mfs_tract t;
	t.crnc = 0;
	t.crsc = 0;
	t.root = 0;
	t.cr = 0;
	t.sc = 0;
	t.slabs = 0;
	t.off = (sizeof(struct mfs_tract)+(MFS_SLAB_SIZE-1))>>MFS_SLAB_SHIFT;
	write(fd, &t, sizeof(struct mfs_tract));
	close(fd);
}
