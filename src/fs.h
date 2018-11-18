# ifndef __ffly__fs__h
# define __ffly__fs__h
# include "ffint.h"
#define fs(__func, ...)\
	__ff_fso__->__func(__VA_ARGS__)
#define fs_open(...)	fs(open, __VA_ARGS__)
#define fs_close(...)	fs(close, __VA_ARGS__)
#define fs_write(...)	fs(write, __VA_ARGS__)
#define fs_read(...)	fs(read, __VA_ARGS__)
#define fs_mkdir(...)	fs(mkdir, __VA_ARGS__)
struct ff_fso {
	ff_u32_t(*open)(char const*, ff_u8_t);
	void(*close)(fff_u32_t);
	void(*write)(ff_u32_t, void*, ff_uint_t, ff_u64_t);
	void(*read)(ff_u32_t, void*, ff_uint_t, ff_u64_t);
	void(*mkdir)(char const*);
};

enum {
	_ff_mfs
};

struct ff_fso extern *__ff_fso__;
void(*ffly_fs(ff_u8_t))(void);
# endif /*__ffly__fs__h*/
