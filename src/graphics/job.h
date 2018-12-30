# ifndef __ffly__graphics__job__h
# define __ffly__graphics__job__h
# include "../ffint.h"
# include "../types.h"
# include "colour.h"
# include "../pallet.h"
# include "../tile.h"
# include "struc.h"
#define _grj_pixfill	0
#define _grj_pixcopy	1
#define _grj_pixdraw	2
#define _grj_tdraw		3
#define _grj_tri2		4
struct ffly_grj {
	ff_u8_t kind;
	void *p;
	void *par[20];
	struct ffly_grj *prev, *next;
};

# ifdef __cplusplus
extern "C" {
# endif
struct ffly_grj* ffly_grj_tri2(struct ffly_tri2 *, ff_u16_t, ff_u32_t, ff_u32_t);
struct ffly_grj* ffly_grj_mk(ff_u8_t, void*);
struct ffly_grj* ffly_grj_pixfill(ff_u32_t, ffly_colour_t, ff_u32_t);
struct ffly_grj* ffly_grj_pixdraw(ff_u32_t, ff_u32_t, void*, ff_u32_t, ff_u32_t);
struct ffly_grj* ffly_grj_pixcopy(ff_u32_t, ff_u32_t, ff_u32_t, ff_u32_t);
struct ffly_grj* ffly_grj_tdraw(ffly_tilep, ff_u32_t, ff_u32_t);
ff_err_t ffly_grj_prosess(struct ffly_grj*);
void ffly_grj_cleanup();
# ifdef __cplusplus
}
# include "../memory/mem_alloc.h"
namespace mdl {
namespace firefly {
namespace graphics {
template<typename _A1>
static struct ffly_grj* mk_job(u8_t __kind, _A1 __a1) {
	void *par = memory::mem_alloc(sizeof(_A1));
	u8_t *p = static_cast<u8_t*>(par);
	*reinterpret_cast<_A1*>(p) = __a1;
	return ffly_grj_mk(__kind, par);
}

template<typename _A1, typename _A2>
static struct ffly_grj* mk_job(u8_t __kind, _A1 __a1, _A2 __a2) {
	void *par = memory::mem_alloc(sizeof(_A1)+sizeof(_A2));
	u8_t *p = static_cast<u8_t*>(par);
	*reinterpret_cast<_A1*>(p) = __a1;
	p+= sizeof(_A1);

	*reinterpret_cast<_A2*>(p) = __a2;
	return ffly_grj_mk(__kind, par);
}

template<typename _A1, typename _A2, typename _A3>
static struct ffly_grj* mk_job(u8_t __kind, _A1 __a1, _A2 __a2, _A3 __a3) {
    void *par = memory::mem_alloc(sizeof(_A1)+sizeof(_A2)+sizeof(_A3));
	u8_t *p = static_cast<u8_t*>(par);
	*reinterpret_cast<_A1*>(p) = __a1;
	p+= sizeof(_A1);

	*reinterpret_cast<_A2*>(p) = __a2;
	p+= sizeof(_A2);

	*reinterpret_cast<_A3*>(p) = __a3;
    return ffly_grj_mk(__kind, par);
}

template<typename _A1, typename _A2, typename _A3, typename _A4>
static struct ffly_grj* mk_job(u8_t __kind, _A1 __a1, _A2 __a2, _A3 __a3, _A4 __a4) {
    void *par = memory::mem_alloc(sizeof(_A1)+sizeof(_A2)+sizeof(_A3)+sizeof(_A4));
	u8_t *p = static_cast<u8_t*>(par);
	*reinterpret_cast<_A1*>(p) = __a1;
	p+= sizeof(_A1);

	*reinterpret_cast<_A2*>(p) = __a2;
	p+= sizeof(_A2);

	*reinterpret_cast<_A3*>(p) = __a3;
	p+= sizeof(_A3);

	*reinterpret_cast<_A4*>(p) = __a4;
    return ffly_grj_mk(__kind, par);
}

template<typename _A1, typename _A2, typename _A3, typename _A4, typename _A5>
static struct ffly_grj* mk_job(u8_t __kind, _A1 __a1, _A2 __a2, _A3 __a3, _A4 __a4, _A5 __a5) {
    void *par = memory::mem_alloc(sizeof(_A1)+sizeof(_A2)+sizeof(_A3)+sizeof(_A4)+sizeof(_A5));
	u8_t *p = static_cast<u8_t*>(par);
	*reinterpret_cast<_A1*>(p) = __a1;
	p+= sizeof(_A1);

	*reinterpret_cast<_A2*>(p) = __a2;
	p+= sizeof(_A2);

	*reinterpret_cast<_A3*>(p) = __a3;
	p+= sizeof(_A3);

	*reinterpret_cast<_A4*>(p) = __a4;
	p+= sizeof(_A4);

	*reinterpret_cast<_A5*>(p) = __a5;
	return ffly_grj_mk(__kind, par);
}

template<typename _A1, typename _A2, typename _A3, typename _A4, typename _A5, typename _A6>
static struct ffly_grj* mk_job(u8_t __kind, _A1 __a1, _A2 __a2, _A3 __a3, _A4 __a4, _A5 __a5, _A6 __a6) {
	void *par = memory::mem_alloc(sizeof(_A1)+sizeof(_A2)+sizeof(_A3)+sizeof(_A4)+sizeof(_A5)+sizeof(_A6));
	u8_t *p = static_cast<u8_t*>(par);
	*reinterpret_cast<_A1*>(p) = __a1;
	p+= sizeof(_A1);

	*reinterpret_cast<_A2*>(p) = __a2;
	p+= sizeof(_A2);

	*reinterpret_cast<_A3*>(p) = __a3;
	p+= sizeof(_A3);

	*reinterpret_cast<_A4*>(p) = __a4;
	p+= sizeof(_A4);

	*reinterpret_cast<_A5*>(p) = __a5;
	p+= sizeof(_A5);

	*reinterpret_cast<_A6*>(p) = __a6;
	return ffly_grj_mk(__kind, par);
}

template<typename _A1, typename _A2, typename _A3, typename _A4, typename _A5, typename _A6, typename _A7>
static struct ffly_grj* mk_job(u8_t __kind, _A1 __a1, _A2 __a2, _A3 __a3, _A4 __a4, _A5 __a5, _A6 __a6, _A7 __a7) {
	void *par = memory::mem_alloc(sizeof(_A1)+sizeof(_A2)+sizeof(_A3)+sizeof(_A4)+sizeof(_A5)+sizeof(_A6)+sizeof(_A7));
	u8_t *p = static_cast<u8_t*>(par);
	*reinterpret_cast<_A1*>(p) = __a1;
	p+= sizeof(_A1);

	*reinterpret_cast<_A2*>(p) = __a2;
	p+= sizeof(_A2);

	*reinterpret_cast<_A3*>(p) = __a3;
	p+= sizeof(_A3);

	*reinterpret_cast<_A4*>(p) = __a4;
	p+= sizeof(_A4);

	*reinterpret_cast<_A5*>(p) = __a5;
	p+= sizeof(_A5);

	*reinterpret_cast<_A6*>(p) = __a6;
	p+= sizeof(_A6);

	*reinterpret_cast<_A7*>(p) = __a7;
	return ffly_grj_mk(__kind, par);
}
}
}
}
# endif
# endif /*__ffly__graphics__job__h*/
