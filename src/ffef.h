# ifndef __ffly__ffef__h
# define __ffly__ffef__h
# include <mdlint.h>

# define FF_EF_IL 16
# define FF_EF_MAG0 'f'
# define FF_EF_MAG1 'f'
# define FF_EF_MAG2 'e'
# define FF_EF_MAG3 'f'

# define FF_EF_NULL (~(mdl_u64_t)0)

# define FF_SG_STACK 0x0

# define ffef_hdr_size sizeof(struct ffef_hdr)

# define ffef_sym_hdrsz sizeof(struct ffef_sym_hdr)
# define ffef_reg_hdrsz sizeof(struct ffef_reg_hdr)
# define ffef_seg_hdrsz sizeof(struct ffef_seg_hdr)

typedef struct ffef_sym_hdr {
	mdl_u64_t name;
	mdl_uint_t l;
	mdl_u64_t loc;
} *ffef_sym_hdrp;

typedef struct ffef_reg_hdr {
	mdl_u64_t fd;

} *ffef_reg_hdrp;

typedef struct ffef_seg_hdr {
	mdl_u8_t type;
	mdl_u64_t fd;
	mdl_u64_t offset;
	mdl_u64_t adr;
	mdl_uint_t memsz;
} *ffef_seg_hdrp;

typedef struct ffef_hdr {
	char ident[FF_EF_IL];
	mdl_u8_t format;
	mdl_u64_t routine, end;
	mdl_uint_t nsy;
	mdl_u64_t sg, rg, sy;
} *ffef_hdrp;

# endif /*__ffly__ffef__h*/
