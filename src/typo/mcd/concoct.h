# ifndef __ffly__typo__mcd__concoct__h
# define __ffly__typo__mcd__concoct__h
# include "../../ffint.h"
# include "struc.h"

# define seg_off(__p, __s) \
    (((ff_u8_t*)(__p))-((ff_u8_t*)(__s)->p))
# define ffly_mcd_cc_face_size(__width, __height) \
	ffly_mcd_cc_face_width = __width;\
	ffly_mcd_cc_face_height = __height

ff_uint_t extern ffly_mcd_cc_face_width;
ff_uint_t extern ffly_mcd_cc_face_height;

extern void(*ffly_mcd_cc_write)(void*, ff_uint_t, ff_u32_t);

struct cc_segment {
    struct segment text;
    ff_uint_t sz;
    void *p;
};

struct cc_glyph {
    struct glyph text;
    struct ffly_tape *tape;
};

struct cc_segment* ffly_mcd_cc_seg_new(void);
struct cc_glyph* ffly_mcd_cc_glyph_new(void);
void* ffly_mcd_cc_seg_alloc(struct cc_segment*, ff_uint_t);
void ffly_mcd_cc_prime(void);

// concoct it / output to write / make final product
void ffly_mcd_cc(void);

# endif /*__ffly__typo__mcd__concoct__h*/
