# ifndef __ffly__as__frag__h
# define __ffly__as__frag__h
# include "../ffint.h"
# define fgrowb(__f, __by) \
	ff_as_fgrow(__f, __f->size+__by)
struct frag {
	/*
		next fragment
	*/
	struct frag *next;

	/*
		page pointers
	*/
	void **p;

	/*
		number of pages
	*/
	ff_u32_t page_c;

	/*
		size in (bytes)
	*/
	ff_u32_t size;

	/*
		where next piece of data should
		be placed within fragment.

		offset from base
	*/
	ff_u32_t offset;

	/*
		where fragment was put in file
	*/
	ff_u32_t dst;

	/*
		virtual address - where within program memory
		fragment is stored
	*/
	ff_u32_t adr;
};

// current fragment
extern struct frag *curfrag;

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
	grow fragment to X size
*/
void ff_as_fgrow(struct frag*, ff_u32_t);

/*
	output fragments from head to tail
*/
void ff_as_foutput(void);

/*
	plant a piece of data at end of fragment
*/
void ff_as_plant(struct frag*, void*, ff_uint_t);

# define frag_offset(__f) ((__f)->offset)
# endif /*__ffly__as__frag__h*/
