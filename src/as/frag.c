# include "frag.h"
# include "as.h"
# include "../stdio.h"
static struct frag *head = NULL;
struct frag *curfrag = NULL;

# define PAGE_SHIFT 4
# define PAGE_SIZE (1<<PAGE_SHIFT)
/*
	MIGHTDO:
		store pointers of frags in array
*/

struct frag* ff_as_fnew(void) {
	struct frag *f = (struct frag*)ff_as_al(sizeof(struct frag));	
	if (!head)
		head = f;
	if (curfrag != NULL)
		curfrag->next = f;
	curfrag = f;
	f->next = NULL;
	f->p = NULL;
	f->page_c = 0;
	f->size = 0;
	f->offset = 0;
	f->adr = 0;
	return f;
}

# include "../string.h"
void ff_as_fwrite(struct frag *__f, ff_u32_t __offset, void *__buf, ff_uint_t __size) {
	ff_u32_t page, pg_left;
	ff_u16_t pg_off;
	ff_u8_t *dst;

	ff_u8_t *p = (ff_u8_t*)__buf;
	ff_u8_t *e = p+__size;
	ff_u32_t offset = __offset;

	ff_uint_t shred, left;
	while(p != e) {
		page = offset>>PAGE_SHIFT;
		printf("writing to page: %u\n", page);
		pg_off = offset-(page*PAGE_SIZE);
		dst = ((ff_u8_t*)*(__f->p+page))+pg_off;
		pg_left = PAGE_SIZE-pg_off;

		left = __size-(p-(ff_u8_t*)__buf);

		shred = left>pg_left?pg_left:left;
		memcpy(dst, p, shred);
		offset+=shred;
		p+=shred;
	}
}

void ff_as_plant(struct frag *__f, void *__buf, ff_uint_t __size) {
	ff_as_fwrite(__f, __f->offset, __buf, __size);
	__f->offset+=__size;
}

void ff_as_fread(struct frag *__f, ff_u32_t __offset, void *__buf, ff_uint_t __size) {
	ff_u32_t page, pg_left;
	ff_u16_t pg_off;
	ff_u8_t *src;

	ff_u8_t *p = (ff_u8_t*)__buf;
	ff_u8_t *e = p+__size;
	ff_u32_t offset = __offset;

	ff_uint_t shred, left;
	while(p != e) {
		page = offset>>PAGE_SHIFT;
		printf("reading from page: %u\n", page);
		pg_off = offset-(page*PAGE_SIZE);
		src = ((ff_u8_t*)*(__f->p+page))+pg_off;
		pg_left = PAGE_SIZE-pg_off;
        
		left = __size-(p-(ff_u8_t*)__buf);

		shred = left>pg_left?pg_left:left;
		memcpy(p, src, shred);
		offset+=shred;
		p+=shred;
	}
}

void ff_as_fgrow(struct frag *__f, ff_u32_t __size) {
	__f->size = __size;
	if (__f->page_c*PAGE_SIZE<__size) {
		printf("new frag pages.\n");
		ff_u32_t pg_c = ((__size>>PAGE_SHIFT)+((__size-((__size>>PAGE_SHIFT)*PAGE_SIZE))>0));
		if (!__f->p) {
			__f->p = (void**)ff_as_al(pg_c*sizeof(void*));
		} else
			__f->p = (void**)ff_as_ral(__f->p, pg_c*sizeof(void*));
		
		// allocate pages
		void **p = __f->p+__f->page_c;
		void **e = p+(pg_c-__f->page_c);
		__f->page_c = pg_c;
		while(p != e)
			*(p++) = ff_as_al(PAGE_SIZE);
	}
}

# include "../hexdump.h"
# include "../chrdump.h"

void ff_as_foutput(void) {
	struct frag *f = head;
	ff_u32_t page, pg_c;
	void *p;
	ff_u16_t left;
	adr = 0;
	lseek(out, offset, SEEK_SET);
	while(f != NULL) {
		printf("outputing frag, size: %u\n", f->size);
		f->dst = offset;
		page = 0;
		pg_c = f->size>>PAGE_SHIFT;
		while(page != pg_c) {
			printf("page %u out.\n", page);
			p = *(f->p+page);
			ffly_hexdump(p, PAGE_SIZE);
			ffly_chrdump(p, PAGE_SIZE);
			write(out, p, PAGE_SIZE);
			page++;
		}

		left = f->size-(pg_c*PAGE_SIZE);
		ffly_printf("--> %d\n", left);
		if (left>0) {
			printf("leftover: %u\n", left);
			if (!(p = *(f->p+page)) || page>=f->page_c) {

			} else {
				ffly_hexdump(p, left);
				ffly_chrdump(p, left);
				write(out, p, left);
			}
		}

		offset+=f->size;
		f->adr = adr;
		adr+=f->size;
		f = f->next;
	}
}
