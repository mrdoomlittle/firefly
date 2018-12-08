# ifndef __ffly__remf__h
# define __ffly__remf__h
# include "ffint.h"

#define FF_REMF_IL		16
#define FF_REMF_MAG0	'r'
#define FF_REMF_MAG1	'e'
#define FF_REMF_MAG2	'm'
#define FF_REMF_MAG3	'f'

#define FF_REMF_NULL (~(ff_u64_t)0)

#define FF_SG_NULL	0x00
#define FF_SG_STACK	0x01
#define FF_SG_PROG	0x02

#define remf_hdrsz		sizeof(struct remf_hdr)

#define remf_sysz		sizeof(struct remf_sy)
#define remf_reghdrsz	sizeof(struct remf_reg_hdr)
#define remf_seghdrsz	sizeof(struct remf_seg_hdr)
#define remf_relsz		sizeof(struct remf_rel)
#define remf_hoksz		sizeof(struct remf_hok)

#define FF_RG_NULL	0x00
#define FF_RG_PROG	0x01
#define FF_RG_SS	0x02
#define FF_RG_STT	0x03
#define FF_RG_SYT	0x04

#define FF_SY_NULL	0x00
#define FF_SY_IND	0x01
#define FF_SY_GBL	0x02
#define FF_SY_LCA	0x03
typedef struct remf_sy {
	ff_u16_t name;
	ff_u8_t type;
	ff_u8_t l;
	ff_u64_t loc;
	ff_u16_t reg;
	ff_u16_t f;
} *remf_syp;

typedef struct remf_reg_hdr {
	ff_u64_t name;
	ff_u8_t l;
	ff_u8_t type;
	ff_u64_t offset;
	ff_u32_t size;
	ff_u64_t adr;

	/*
		put own struct e.g. freg for 'fragmentized region'
	*/
	ff_u16_t fs, nf;
} *remf_reg_hdrp;

typedef struct remf_seg_hdr {
	ff_u8_t type;
	ff_u64_t offset;
	ff_u64_t adr;
	ff_uint_t sz;
} *remf_seg_hdrp;

typedef struct remf_rel {
	ff_u64_t offset;
	ff_u8_t l;
	ff_u16_t sy;
	ff_u16_t adr;
	ff_u16_t addto;
	ff_u16_t f;
} *remf_relp;

typedef struct remf_hok {
	ff_u64_t offset;
	ff_uint_t l;
	ff_u16_t adr;
	ff_u16_t to;
	ff_u16_t f;
} *remf_hokp;

struct remf_frag {
/*
	fragments may or may not be in order from 0-...
*/
	ff_uint_t id;
	ff_u64_t src;
	ff_uint_t size;
};

// fragment table
typedef struct remf_ft {
	ff_uint_t n;
	ff_u64_t array;
} *remf_ftp;

typedef struct remf_hdr {
	char ident[FF_REMF_IL];
	ff_u8_t format; // <- remove

	/*
		start routine could just rename to entry .... COULD
	*/
	ff_u64_t routine;

	// string table region
	ff_u64_t sttr;

	/*
		number of segments, regions, relocs, hooks
	*/
	ff_u16_t nsg, nrg, nrl, nhk;

	/*
		where segments/regions... are located within file{offset to}
	*/
	ff_u64_t sg, rg, rl, hk;

	/*
		bottom of program data usage: bond -> where next file program data will be placed
	*/
	ff_u32_t adr;

	// frag table location
	ff_u64_t ft;
} *remf_hdrp;

# endif /*__ffly__remf__h*/
