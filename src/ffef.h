# ifndef __ffly__ffef__h
# define __ffly__ffef__h
// resf
# include "ffint.h"

#define FF_EF_IL 16
#define FF_EF_MAG0 'f'
#define FF_EF_MAG1 'f'
#define FF_EF_MAG2 'e'
#define FF_EF_MAG3 'f'

#define FF_EF_NULL (~(ff_u64_t)0)

#define FF_SG_NULL 0x0
#define FF_SG_STACK 0x1
#define FF_SG_PROG 0x2

#define ffef_hdr_size sizeof(struct ffef_hdr)

#define ffef_sysz sizeof(struct ffef_sy)
#define ffef_reg_hdrsz sizeof(struct ffef_reg_hdr)
#define ffef_seg_hdrsz sizeof(struct ffef_seg_hdr)
#define ffef_relsz sizeof(struct ffef_rel)
#define ffef_hoksz sizeof(struct ffef_hok)

#define FF_RG_NULL 0x0
#define FF_RG_PROG 0x1
#define FF_RG_SS 0x2
#define FF_RG_STT 0x3
#define FF_RG_SYT 0x4

#define FF_SY_NULL 0x0
#define FF_SY_IND 0x1
#define FF_SY_GBL 0x2
#define FF_SY_LCA 0x3
typedef struct ffef_sy {
	ff_u16_t name;
	ff_u8_t type;
	ff_u8_t l;
	ff_u64_t loc;
	ff_u16_t reg;
} *ffef_syp;

typedef struct ffef_reg_hdr {
	ff_u64_t name;
	ff_u8_t l;
	ff_u8_t type;
	ff_u64_t beg, end;
	ff_u64_t adr;
} *ffef_reg_hdrp;

typedef struct ffef_seg_hdr {
	ff_u8_t type;
	ff_u64_t offset;
	ff_u64_t adr;
	ff_uint_t sz;
} *ffef_seg_hdrp;

typedef struct ffef_rel {
	ff_u64_t offset;
	ff_u8_t l;
	ff_u16_t sy;
	ff_u16_t addto;
} *ffef_relp;

typedef struct ffef_hok {
	ff_u64_t offset;
	ff_uint_t l;
	ff_u16_t to;
} *ffef_hokp;

typedef struct ffef_hdr {
	char ident[FF_EF_IL];
	ff_u8_t format;
	ff_u64_t routine;
	ff_u64_t sttr;

	ff_u16_t nsg, nrg, nrl, nhk;
	ff_u64_t sg, rg, rl, hk;
	ff_u32_t adr;
} *ffef_hdrp;

# endif /*__ffly__ffef__h*/
