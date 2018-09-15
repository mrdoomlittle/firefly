# include "tile.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "graphics/draw.h"
# include "ffly_def.h"
# include "dep/mem_set.h"
# include "system/io.h"
# include "system/sched.h"
# include "system/mutex.h"
# include "graphics/job.h"
# include "graphics/pipe.h"
static struct ffly_tile *top = NULL;
ff_mlock_t static lock = FFLY_MUTEX_INIT;

# define lk \
	ffly_mutex_lock(&lock)
# define ul \
	ffly_mutex_unlock(&lock)
void ffly_tile_draw(ffly_tilep __tile,
	ff_u16_t __width, ff_u16_t __height,
	ff_u16_t __x, ff_u16_t __y)
{
	ffly_grp_inject(&__ffly_grp__, ffly_grj_tdraw(__tile, __width, __height, __x, __y));

//	ffly_pixdraw(__x+__tile->xdis, __y+__tile->ydis, __dst, __width, __tile->p, 1<<__tile->size, 1<<__tile->size);
}

ffly_tilep ffly_tile_creat(ff_u8_t __size) {
	ffly_printf("new tile.\n");
	ffly_tilep tile;
	
	tile = (ffly_tilep)__ffly_mem_alloc(sizeof(struct ffly_tile));
	tile->size = __size;
	tile->m = ffly_mo_new();

	ff_uint_t sb;
	sb = (1<<__size)*(1<<__size)*4;
	ffly_mo_map(tile->m, sb);


	tile->bits = TILE_BLANK;
	tile->xdis = 0;
	tile->ydis = 0;
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
	ffly_mo_unmap(__tile->m);
	ffly_mo_destroy(__tile->m);
	__ffly_mem_free(__tile);
}

ff_u32_t static sched_id;
ff_i8_t static
update(void) {
	lk;
	ffly_tilep cur = top;
	while(cur != NULL) {

		cur = cur->next;
	}
	ul;
	return -1;
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
