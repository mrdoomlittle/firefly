# include "brick.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# include "system/io.h"
# define PAGE_SHIFT 2
# define PAGE_SIZE (1<<PAGE_SHIFT)

/*
	later on we need to download files for shit to work e.g. config files
	but if the client cuts out downloading the file over again is wastful
	soo once the client is back online next try will start off at the brick it left on.
*/

/*
	TODO:
		free memory after x amount of time after close
*/

/*
	needs testing
*/

static ffly_brickp *bricks = NULL;
static ff_uint_t page_c = 0;
static ff_u64_t off = 0;
static ffly_brickp bin = NULL;

static ffly_brickp top = NULL;

# define get_brick(__id) \
	((*(bricks+(__id&0xfff)))+((__id>>12)&0xfffff))
void static delink(ffly_brickp);

ff_u32_t
ffly_brick_new(ff_u8_t __sz,
	void(*__read)(long, void*, ff_u8_t), void(*__write)(long, void*, ff_u8_t),
	void(*__del)(long), long __arg)
{
	ffly_brickp b;
	if (bin != NULL) {
		b = bin;
		if ((bin = bin->fd) != NULL)
			bin->bk = &bin;
		
		b->inuse = 0;
		return b->id;
	}
	ff_uint_t page = off>>PAGE_SHIFT;
	ff_u32_t pg_off;
	if (!bricks) {
		bricks = (ffly_brickp*)__ffly_mem_alloc(sizeof(ffly_brickp));
		page_c++;
	} else {
		if (page>page_c-1)
			bricks = (ffly_brickp*)__ffly_mem_realloc(bricks, (++page_c)*sizeof(ffly_brickp));
		else
			goto _sk;
	}

	*(bricks+page) = (ffly_brickp)__ffly_mem_alloc(PAGE_SIZE*sizeof(struct ffly_brick));
_sk:
	pg_off = (off++)-(page*PAGE_SIZE);
	b = (*(bricks+page))+pg_off;

	b->arg = __arg;
	b->read = __read;
	b->write = __write;
	b->del = __del;
	b->p = NULL;
	b->flags = 0x0;
	b->inuse = 0;
	b->next = top;
	if (top != NULL)
		top->pn = &b->next;
	b->pn = &top;
	b->sz = __sz;
	top = b;
	return (b->id = (page&0xfff)|((pg_off&0xfffff)<<12));
}

void ffly_brick_open(ff_u32_t __id) {
	ffly_brickp b = get_brick(__id);
	b->flags |= BRICK_OPEN;}

void ffly_brick_close(ff_u32_t __id) {
	ffly_brickp b = get_brick(__id);
	b->flags ^= BRICK_OPEN;
	if (b->p != NULL) {
		if (b->write != NULL)
			b->write(b->arg, b->p, b->sz);
	}
}

void* ffly_brick_get(ff_u32_t __id) {
	ffly_brickp b = get_brick(__id);
	if (!b->p) {
		b->p = __ffly_mem_alloc(1<<b->sz);
		b->read(b->arg, b->p, b->sz);
	}
	return b->p;
}

void ffly_brick_cleanup(void) {
	ffly_brickp cur = top;
	while(cur != NULL) {
		if (cur->p != NULL)
			__ffly_mem_free(cur->p);
		cur = cur->next;
	}

	ff_uint_t page = 0;
	ffly_printf("brick.c; %u pages to be freed.\n", page_c);
	while(page != page_c) {
		__ffly_mem_free(*(bricks+page));
		page++;
	}

	if (bricks != NULL)
		__ffly_mem_free(bricks);
}

void static
deattach(ffly_brickp __b) {
	*__b->pn = __b->next;
	if (__b->next != NULL)
		__b->next->pn = __b->pn;
}

void
delink(ffly_brickp __b) {
	*__b->bk = __b->fd;
	if (__b->fd != NULL)
		__b->fd->bk = __b->bk;
}

void ffly_bricks_show(void) {
	ffly_brickp cur = top;
	ff_uint_t i = 0;
	while(cur != NULL) {
		ffly_printf("%u: %s, id: %u, pg: %u\n", i++, !cur->inuse?"inuse":"not inuse", cur->id, cur->id&0xfff);
		cur = cur->next;
	}
}

// get rid of brick
void ffly_brick_rid(ff_u32_t __id) {
	ffly_brickp b = get_brick(__id);
	if (b->del != NULL)
		b->del(b->arg);
	ff_u64_t bo = ((__id&0xfff)*PAGE_SIZE)+(__id>>12&0xfffff);
	if (bo == off-1 && page_c>1) {
		ffly_printf(".... next: %p\n", b->next);
		ffly_brickp cur = b->next;
		off--;
		deattach(b);
		while(cur != NULL) {
			b = cur;
			cur = cur->next;
			if (!b->inuse) {
				ffly_printf("brick found to be inuse, breaking\n");
				break;
			}
			if (!(off-1>>PAGE_SHIFT))
				break;
			if (b->p != NULL)
				__ffly_mem_free(b->p);
			ffly_printf("found more dead bricks, id: %u\n", b->id);
			delink(b);
			deattach(b);
			off--;
		}

		ff_uint_t old_pgc = page_c;
		ff_uint_t page = off>>PAGE_SHIFT;
		while(page < page_c-1) {
			page_c--;
			ffly_printf("removing page: %u\n", page_c);
			__ffly_mem_free(*(bricks+page_c));
		}
	
		if (page_c<old_pgc)
			bricks = (ffly_brickp*)__ffly_mem_realloc(bricks, page_c*sizeof(ffly_brickp));
		return;
	}

	if (b->p != NULL) {
		__ffly_mem_free(b->p);
		b->p = NULL;
	}
	b->fd = bin;
	b->inuse = -1;
	b->bk = &bin;
	if (bin != NULL)
		bin->bk = &b->fd;
	bin = b;
	ffly_printf("added brick to bin.\n");
}
