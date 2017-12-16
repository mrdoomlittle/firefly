# include "dir.h"
# include <fcntl.h>
# include <unistd.h>
# include "io.h"
# include "errno.h"
# include <sys/stat.h>
ffly_err_t ffly_mkdir(char *__dir, mode_t __mode) {
	if (mkdir(__dir, __mode) < 0) {
		ffly_fprintf(ffly_err, "failed to make directory.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_rmdir(char *__dir) {
	if (rmdir(__dir) < 0) {
		ffly_fprintf(ffly_err, "failed to remove directory.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}


