# ifndef __ffly__as__frag__h
# define __ffly__as__frag__h
# include "../ffint.h"
# include "fix_struc.h"

# define FR_FIX 0x01

// grow fragment by X about or (current fragment size)+X
# define fgrowb(__f, __by) \
	ff_as_fgrow(__f, (__f)->size+__by)
/*
	get frag by number

	example

	frag 0
	frag 1 
	frag 2
	frag 3

	say we have frag 1 and want frag the fragments from 1 to ...

_next:
	(frag i) = ff_as_fbn((frag 1)->f+i);
	i++;
	goto _next;

	could do this with linked lists but i like to keep my distance from useing them
	and only use them when necessary or theres no other way.
*/
extern ff_uint_t fr_nr;
struct frag *ff_as_fbn(ff_uint_t);
struct frag {
	// next fragment in list
	/*
		mostly used when writing fragments and cleanup <- later
	*/
	struct frag *next;
	// fragment number
	ff_uint_t f;

	/*
		page pointers 

		 . where fragment data is to be stored
	*/
	void **p;
	// number of pages
	ff_u32_t page_c;
	/*
		size of fragment in (bytes)(used)
		does not included excess if not aligned to page size

		for real size: (page_c*PAGE_SIZE) = size+excess;
	*/
	ff_u32_t size;
	/*
		where next piece of data should
		be placed within fragment.

		offset from base 
	*/
	ff_u32_t offset;
	/*
		where fragment was placed in file
	*/
	ff_u32_t dst;
	/*
		virtual address - where within program memory
		fragment is to be stored.
	*/
	ff_u32_t adr;
	ff_u8_t flags;

	ff_int_t bs;

	struct fix_s *fx;
};

// current fragment
extern struct frag *curfrag, *fr_head;
// new fragment
struct frag* ff_as_fnew(void);
/*
	write to fragment at X offset
*/
void ff_as_fwrite(struct frag*, ff_u32_t, void*, ff_uint_t);
/*
	read from fragment at X offset
*/
void ff_as_fread(struct frag*, ff_u32_t, void*, ff_uint_t);
/*
	TODO:
		add shrink function ?????? not needed but might be or could be usefull later ?

	grow fragment to X size
*/
void ff_as_fgrow(struct frag*, ff_u32_t);
/*
	output fragments from head to tail
*/
void ff_as_foutput(void);
/*
	plant a piece of data at end of fragment

	NOTE:
		does not grow fragment only places
*/
void ff_as_plant(struct frag*, void*, ff_uint_t);

// fragment is no longer needed as we are moving on to a new fragment
void ff_as_fdone(struct frag*);
# define frag_offset(__f) ((__f)->offset)
# define frag_flags(__f) (__f)->flags
# define pf_offset curfrag->offset
# define pf_flags curfrag->flags
# endif /*__ffly__as__frag__h*/
