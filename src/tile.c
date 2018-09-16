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

void static
pt_get(ff_u8_t __what, long long __dst, void *__tile) {
	ffly_tilep t;

	t = (ffly_tilep)__tile;
	switch(__what) {
		case 0x00: // get bits
			*(ff_u8_t*)__dst = t->bits;	
		break;
		case 0x01: // get memory map
			*(void**)__dst = t->m->map;
		break;
		case 0x02: // get size by shift index
			*(ff_u8_t*)__dst = t->size;
		break;
	}
}

# include "context.h"
ff_u16_t ffly_g_ptile_new(void(*__get)(ff_u8_t, long long, void*), void *__tile) {
	ff_u16_t pt;

	struct ff_context *ctx;
	ctx = G_CONTEXT;
	pt = ctx->stack;
	ctx->stack+=2;
	ctx->driver.ptile_new(pt, __get, __tile);
	return pt;
}

void ffly_g_ptile_destroy(ff_u16_t __pt) {
	G_CONTEXT->driver.ptile_destroy(__pt);
}

void ffly_tile_draw(ffly_tilep __tile, ff_u32_t __x, ff_u32_t __y) {
	if (!(__tile->bits&TILE_PH)) {
		G_CONTEXT->driver.sget(&__tile->pt, 2, ffly_g_ptile_new(pt_get, __tile));
		__tile->bits |= TILE_PH;
		return;
	}



	G_CONTEXT->driver.tdraw(__tile->pt, __x, __y);	
}

ffly_tilep ffly_tile_creat(ff_u8_t __size) {
	ffly_printf("new tile.\n");
	ffly_tilep tile;
	
	tile = (ffly_tilep)__ffly_mem_alloc(sizeof(struct ffly_tile));
	tile->size = __size;
	tile->m = ffly_mo_new();
	tile->bits = 0x00;

	ff_uint_t sb;
	sb = (1<<__size)*(1<<__size)*4;
	ffly_mo_map(tile->m, sb);


	tile->bits = TILE_BLANK;
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
	if ((__tile->bits&TILE_PH)>0) {
		ffly_g_ptile_destroy(__tile->pt);
	}
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
