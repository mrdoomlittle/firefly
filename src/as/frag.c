# include "frag.h"
# include "as.h"
# include "../stdio.h"
struct frag *fr_head = NULL;
struct frag *curfrag = NULL;

#define PAGE_SHIFT 4
#define PAGE_SIZE (1<<PAGE_SHIFT)

/*
	fragments from top to bottom

	AUTD:
		find fragment inbetween jump and label
*/
struct frag ***fr_tbl = NULL;
#define FR_DONE 0x02
#define FRT_PAGE_SHIFT 4
#define FRT_PAGE_SIZE (1<<FRT_PAGE_SHIFT)
/*
	MIGHTDO:
		store pointers of frags in array
*/

// fragment number
ff_uint_t fr_nr = 0;
ff_uint_t static frt_page_c = 0;
struct frag* ff_as_fnew(void) {
	if (curfrag != NULL) {
		if (!(curfrag->flags&FR_DONE)) {
			printf("warning previous fragment not done.\n");
		}
	}
	struct frag *f;
	f = (struct frag*)ff_as_al(sizeof(struct frag));	
	if (!fr_head)
		fr_head = f;
	if (curfrag != NULL)
		curfrag->next = f;
	curfrag = f;
	f->next = NULL;
	f->p = NULL;
	f->page_c = 0;
	f->size = 0;
	f->offset = 0;
	f->adr = 0;
	f->bs = 0;
	f->flags = 0x00;
	f->m = 0;

	struct frag ***page;
	ff_uint_t pg, pg_off;
	f->f = fr_nr++;
	pg = f->f>>PAGE_SHIFT;
	pg_off = f->f-(pg*PAGE_SIZE);

	if (!fr_tbl) {
		fr_tbl = (struct frag***)ff_as_al(sizeof(struct frag**));
		frt_page_c++;
	} else {
		if (pg>= frt_page_c) {
			fr_tbl = (struct frag***)ff_as_ral(fr_tbl, (++frt_page_c)*sizeof(struct frag**));
		} else
			goto _sk;
	}

	*(fr_tbl+pg) = (struct frag**)ff_as_al(FRT_PAGE_SIZE*sizeof(struct frag*));
_sk:
	page = fr_tbl+pg;
	*((*page)+pg_off) = f;
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
		ff_u32_t pg_c;		
		pg_c = ((__size+(0xffffffffffffffff>>(64-PAGE_SHIFT)))>>PAGE_SHIFT);;
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

static ff_u32_t fr_adr = 0;

// we are done with this fragment and are moving onto a new one
void ff_as_fdone(struct frag *__f) {
	__f->adr = fr_adr;
	fr_adr+=__f->size;
	printf("frag done, %u, %u\n", __f->size, __f->adr);
	adr = fr_adr;
	__f->flags |= FR_DONE;
}

struct frag *ff_as_fbn(ff_uint_t __n) {
	if (__n>=fr_nr) {
		printf("what the fuck are you trying to do!\n");
		return NULL;
	}
	ff_uint_t pg, pg_off;

	pg = __n>>FRT_PAGE_SHIFT;
	pg_off = __n-(pg*FRT_PAGE_SIZE);
	return *((*(fr_tbl+pg))+pg_off);
}

# include "../hexdump.h"
# include "../chrdump.h"
extern void(*ff_as_fixins)(struct fix_s*);
void ff_as_foutput(void) {
	struct frag *f = fr_head;
	ff_u32_t page, pg_c;
	void *p;
	ff_u16_t left;
	lseek(out, offset, SEEK_SET);
	while(f != NULL) {
		printf("outputing frag-%u, size: %u\n", f->f, f->size);
	
		f->dst = offset;
		if (!f->size)
			goto _skf;

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
	_skf:
		if (f->bs>0) {
			printf("-============-. %u - %u - %u\n", f->f, f->bs, f->size);
			ffly_hexdump(f->data, f->bs);
			write(out, f->data, f->bs);
			offset+=f->bs;
		}
		f = f->next;
	}
}
