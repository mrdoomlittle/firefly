# ifndef __ffly__brick__h
# define __ffly__brick__h
# include "ffint.h"
# define bricksz(__sz) (1<<__sz)
enum {
	_ff_brick_8 = 3,
	_ff_brick_16,
	_ff_brick_32,
	_ff_brick_64,
	_ff_brick_128,
	_ff_brick_256
};

# define BRICK_OPEN 0x1
typedef struct ffly_brick {
	struct ffly_brick *next, **pn, *fd, **bk;
	void(*read)(long, void*, ff_u8_t);
	void(*write)(long, void*, ff_u8_t);
	void(*del)(long);
	void *p;
	long arg;
	ff_u8_t sz;
	ff_i8_t inuse;
	ff_u32_t id;
	ff_u8_t flags;
} *ffly_brickp;

void ffly_bricks_show(void);
ff_u32_t ffly_brick_new(ff_u8_t, void(*)(long, void*, ff_u8_t), void(*)(long, void*, ff_u8_t), void(*)(long), long);
void ffly_brick_rid(ff_u32_t);
void ffly_brick_cleanup(void);
void ffly_brick_open(ff_u32_t);
void ffly_brick_close(ff_u32_t);
void* ffly_brick_get(ff_u32_t);
# endif /*__ffly__brick__h*/
