# include "m.h"
# include "../../ffly_def.h"
# include "../../system/io.h"
# include "../../dep/mem_cpy.h"
# include "../../dep/mem_set.h"
# include "../../linux/unistd.h"
# include "../../linux/fcntl.h"
# include "../../linux/stat.h"
# include "../../system/nanosleep.h"
#define SIZE 8192
ff_u8_t static data[SIZE];
void static __write(void *__buf, ff_uint_t __size, ff_u64_t __offset) {
	ffly_nanosleep(0, 1000000); // 10ms
	if (__offset+__size>=SIZE) {
		ffly_printf("r/w error, %u:%u\n", __offset, __size);
		return;
	}
	ffly_mem_cpy(data+__offset, __buf, __size);
}

void static __read(void *__buf, ff_uint_t __size, ff_u64_t __offset) {
	ffly_nanosleep(0, 10000000);
	if (__offset+__size>SIZE) {
		ffly_printf("r/w error, %u:%u\n", __offset, __size);
		return;
	}

	ffly_mem_cpy(__buf, data+__offset, __size);
}

# include "../../fs.h"
# include "../../hexdump.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	ffly_mem_set(data, '@', SIZE);
	struct stat st;
	int fd;
	fd = open("test.bin", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	fstat(fd, &st);
	read(fd, data, st.st_size);

	struct ffly_mfs _mfs = {
		.write = __write,
		.read = __read,
		.top = NULL, .bin = NULL,
		.off = 0
	};
	mfs = &_mfs;

	void(*fs_fin)(void);
	fs_fin = ffly_fs(_ff_mfs);

	ff_u32_t f;
	fs_mkdir("/test");
	f = fs_open("/test/test.txt", MFS_CREAT);
//	fs_close(f);

	mfs_tree(mfs->root);
//	fs_fin();
//	pwrite(fd, data, SIZE, 0);
	close(fd);
}
