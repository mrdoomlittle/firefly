# ifndef __ffly__fix__struc__h
# define __ffly__fix__struc__h

struct fix_s {
	struct fix_s *next;
 	void *arg;

	ff_u8_t type;
	ff_u8_t flags;
	struct frag *f;
};

# endif /*__ffly__fix__struc__h*/
