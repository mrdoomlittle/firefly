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

# define ffef_sysz sizeof(struct ffef_sy)
# define ffef_reg_hdrsz sizeof(struct ffef_reg_hdr)
# define ffef_seg_hdrsz sizeof(struct ffef_seg_hdr)
# define ffef_relsz sizeof(struct ffef_rel)
# define ffef_hoksz sizeof(struct ffef_hook)
# define FF_RG_NULL 0x0
# define FF_RG_PROG 0x1
# define FF_RG_STT 0x2
# define FF_RG_SYT 0x3
typedef struct ffef_sy {
	mdl_u16_t name;
	mdl_uint_t l;
	mdl_u64_t loc;
	mdl_u64_t hooks;
} *ffef_syp;

typedef struct ffef_reg_hdr {
	mdl_u64_t name;
	mdl_u8_t type;
	mdl_uint_t l;
	mdl_u64_t offset;
	mdl_u64_t beg, end;
} *ffef_reg_hdrp;

typedef struct ffef_seg_hdr {
	mdl_u8_t type;
	mdl_u64_t offset;
	mdl_u64_t adr;
	mdl_uint_t sz;
} *ffef_seg_hdrp;

typedef struct ffef_rel {
	mdl_u64_t offset;
	mdl_u8_t l;
} *ffef_relp;

typedef struct ffef_hok {
	mdl_u64_t offset;
	mdl_uint_t l;
} *ffef_hokp;

typedef struct ffef_hdr {
	char ident[FF_EF_IL];
	mdl_u8_t format;
	mdl_u64_t routine;
	mdl_u64_t sttr;

	mdl_uint_t nsg, nrg, nrl;
	mdl_u64_t sg, rg, rl;
} *ffef_hdrp;

# endif /*__ffly__ffef__h*/
