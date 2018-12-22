# include "fs.h"
#ifdef __fflib
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
#else
# include <fcntl.h>
# include <unistd.h>
#endif
ff_s32_t static
_open(char const *__path, ff_u32_t __flags, ff_u32_t __mode) {
	return open(__path, __flags, __mode);
}

ff_s32_t static
_close(ff_u32_t __fd) {
	return close(__fd);	
}

ff_s32_t static
_lseek(ff_u32_t __fd, ff_u64_t __off, ff_u32_t __whence) {
	return lseek(__fd, __off, __whence);
}

ff_s32_t static 
_read(ff_u32_t __fd, void *__buf, ff_u64_t __size) {
	return read(__fd, __buf, __size);
}

ff_s32_t static
_write(ff_u32_t __fd, void *__buf, ff_u64_t __size) {
	return write(__fd, __buf, __size);
}

ff_s32_t static
_pwrite(ff_u32_t __fd, void *__buf, ff_u64_t __size, ff_u64_t __offset) {
	return pwrite(__fd, __buf, __size, __offset);
}

ff_s32_t static
_pread(ff_u32_t __fd, void *__buf, ff_u64_t __size, ff_u64_t __offset) {
	return pread(__fd, __buf, __size, __offset);
}

ff_s32_t static
_access(char const *__path, ff_u32_t __mode) {
	return access(__path, __mode);
}

ff_s32_t static
_creat(char const *__path, ff_u32_t __mode) {
	return creat(__path, __mode);
}

static struct ffly_fsop _op = {
	_open,
	_close,
	_lseek,
	_read,
	_write,
	_pwrite,
	_pread,
	_access,
	_creat
};

void posix_fsop(struct ffly_fsop *__op) {
	*__op = _op;
}
