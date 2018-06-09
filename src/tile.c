# include "tile.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "graphics/draw.h"
# include "ffly_def.h"
# include "dep/mem_set.h"
# include "system/io.h"
# include "system/sched.h"
# include "system/mutex.h"
static struct ffly_tile *top = NULL;
ff_mlock_t static lock = FFLY_MUTEX_INIT;

# define lk \
	ffly_mutex_lock(&lock)
# define ul \
	ffly_mutex_unlock(&lock)
void ffly_tile_draw(ffly_tilep __tile, ff_u8_t *__dst,
	ff_u16_t __width, ff_u16_t __height, ff_u16_t __x, ff_u16_t __y)
{
	ffly_pixdraw(__x+__tile->xoff, __y+__tile->yoff, __dst, __width, __tile->p, 1<<__tile->size, 1<<__tile->size);
}

ffly_tilep ffly_tile_creat(ff_u8_t __size) {
	ffly_printf("new tile.\n");
	ffly_tilep tile = (ffly_tilep)__ffly_mem_alloc(sizeof(struct ffly_tile));
	tile->size = __size;
	tile->p = __ffly_mem_alloc((1<<__size)*(1<<__size)*4);
	ffly_mem_set(tile->p, 255, (1<<__size)*(1<<__size)*4);
	tile->blank = 0;
	tile->xoff = 0;
	tile->yoff = 0;
	tile->child = NULL;
	lk;
	if (top != NULL)
		top->bk = &tile->next;
	tile->next = top;
	tile->bk = &top;
	top = tile;
	ul;
	return tile;
}

void ffly_tile_del(ffly_tilep __tile) {
	lk;
	*__tile->bk = __tile->next;
	if (__tile->next != NULL)
		__tile->next->bk = __tile->bk;
	ul;
	__ffly_mem_free(__tile->p);
	__ffly_mem_free(__tile);
}

ff_u32_t static sched_id;
void static
update(void) {
	lk;
	ffly_tilep cur = top;
	while(cur != NULL) {

		cur = cur->next;
	}
	ul;
}

void ffly_tiles_sched(void) {
	sched_id = ffly_schedule(update, NULL, 2);
}

void ffly_tiles_usched(void) {
	ffly_sched_rm(sched_id);
}

void ffly_tile_cleanup(void) {
	ffly_tilep cur = top, bk;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		ffly_tile_del(bk);
	}
}
