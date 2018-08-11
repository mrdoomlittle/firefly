# include "mm.h"
# include "../malloc.h"
# include "../stdio.h"
ff_uint_t static used = 0;
/*
	should rename to area ???
*/
struct block {
	struct block **bk, *next;
	void *p;
	ff_uint_t size;
};

static struct block *top = NULL;

# define hdrsize sizeof(struct block)
/*
	temporary
*/
void *ff_dus_mmap(ff_uint_t __size) {
	ff_u8_t *p = malloc(hdrsize+__size);
	struct block *b = (struct block*)p;
	if (top != NULL)
		top->bk = &b->next;

	b->next = top;
	b->bk = &top;
	top = b;
	b->size = __size;
	used+=__size;
	return (b->p = (void*)(p+hdrsize));
}

void ff_dus_munmap(void *__p) {
	struct block *b = (struct block*)((ff_u8_t*)__p-hdrsize);
	if (b->next != NULL)
		b->next->bk = b->bk;
	*b->bk = b->next;
	used-=b->size;
	free(b);
}

void ff_dus_mm_cleanup(void) {
	struct block *cur = top, *bk;
	printf("memory used: %u\n", used);
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		ff_dus_munmap(bk->p);
	}
}
