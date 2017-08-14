# ifndef __time__h
# define __time__h
# include "../types/tpv_t.h"
# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace system {
namespace time {

extern "C" {
types::tpv_t ns_to_us(types::tpv_t);
types::tpv_t ns_to_ms(types::tpv_t);
types::tpv_t ns_to_sec(types::tpv_t);
types::tpv_t ns_to_min(types::tpv_t);

types::tpv_t get_ns_tp();
types::tpv_t get_us_tp();
types::tpv_t get_ms_tp();
types::tpv_t get_sec_tp();
types::tpv_t get_min_tp();
}

template<types::tpv_t(*__e)()>
types::tpv_t get_tp() {return __e;}
template<types::tpv_t(*__e)(types::tpv_t)>
types::tpv_t unit_cast(types::tpv_t __u) {return __e(__u);}

}
}
}
}
# else

# endif


# endif /*__time__h*/
