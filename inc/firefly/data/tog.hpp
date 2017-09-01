# ifndef __tog__hpp
# define __tog__hpp
# include "../types/bool_t"
namespace mdl {
namespace firefly {
namespace data {
void __inline__ tog(types::bool_t& __val) {__val = ~__val & 0x1;}
}
}
}
# endif /*__tog__hpp*/
