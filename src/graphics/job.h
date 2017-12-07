# ifndef __ffly__graphics__job__h
# define __ffly__graphics__job__h
# include "../types/err_t.h"
# include <mdlint.h>

enum {
    _grj_pixfill,
    _grj_pixcopy
};

struct ffly_grj {
	mdl_u8_t kind;
	void *par;
};

# ifdef __cplusplus
extern "C" {
# endif
struct ffly_grj* ffly_grj_mk(mdl_u8_t, void*);
ffly_err_t ffly_grj_prosess(struct ffly_grj*);

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
}
}
}
# endif
# endif /*__ffly__graphics__job__h*/
