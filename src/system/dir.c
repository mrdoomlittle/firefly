# include "dir.h"
# include <fcntl.h>
# include <unistd.h>
# include "io.h"
void ffly_mk_dir(char *__path, mode_t __mode) {
	if (mkdir(__path, __mode) < 0) {
		// error
	}
}

void ffly_rm_dir(char *__path) {
	if (rmdir(__path) < 0) {
		// error
	}
}


