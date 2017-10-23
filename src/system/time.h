# ifndef __time__h
# define __time__h
# include "../types/tpv_t.h"
# ifdef __cplusplus
extern "C" {
# endif
ffly_tpv_t ffly_ns_to_us(ffly_tpv_t);
ffly_tpv_t ffly_ns_to_ms(ffly_tpv_t);
ffly_tpv_t ffly_ns_to_sec(ffly_tpv_t);
ffly_tpv_t ffly_ns_to_min(ffly_tpv_t);

ffly_tpv_t ffly_get_ns_tp();
ffly_tpv_t ffly_get_us_tp();
ffly_tpv_t ffly_get_ms_tp();
ffly_tpv_t ffly_get_sec_tp();
ffly_tpv_t ffly_get_min_tp();
# ifdef __cplusplus
//# ifdef __INLINE_ALL
# include "../ffly_def.h"
//# endif
}
namespace mdl {
namespace firefly {
namespace system {
namespace time {
constexpr static ffly_tpv_t(*ns_to_us)(ffly_tpv_t) = &ffly_ns_to_us;
constexpr static ffly_tpv_t(*ns_to_ms)(ffly_tpv_t) = ffly_ns_to_ms;
constexpr static ffly_tpv_t(*ns_to_sec)(ffly_tpv_t) = &ffly_ns_to_sec;
constexpr static ffly_tpv_t(*ns_to_min)(ffly_tpv_t) = &ffly_ns_to_min;

types::tpv_t force_inline get_ns_tp() {return ffly_get_ns_tp();}
types::tpv_t force_inline get_us_tp() {return ffly_get_us_tp();}
types::tpv_t force_inline get_ms_tp() {return ffly_get_ms_tp();}
types::tpv_t force_inline get_sec_tp() {return ffly_get_sec_tp();}
types::tpv_t force_inline get_min_tp() {return ffly_get_min_tp();}
//# else

//# endif
template<types::tpv_t(*__e)()>
types::tpv_t get_tp() {return __e;}
template<types::tpv_t(*__e)(types::tpv_t)>
static types::tpv_t unit_cast(types::tpv_t __u) {
	if (!__e) return __u;
	return __e(__u);
}
}
}
}
}
# endif
# endif /*__time__h*/
