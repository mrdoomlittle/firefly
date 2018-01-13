# ifndef __ffly_timer__hpp
# define __ffly_timer__hpp
# include <mdlint.h>
# include "time.h"

namespace mdl {
namespace firefly {
namespace system {
struct timer {
	types::tpv_t begin_point;
	types::tpv_t end_point;

	void begin() {this->begin_point = time::get_ns();}
	void end() {this->end_point = time::get_ns();}
	void reset() {this->begin();}

	template<types::tpv_t(*__e)(types::tpv_t)>
	types::tpv_t now() {
		return time::unit_cast<__e>(time::get_ns()-this->begin_point);}

	template<types::tpv_t(*__e)(types::tpv_t)>
	types::tpv_t delta() {
		return time::unit_cast<__e>(this->end_point-this->begin_point);}
};
}
}
}

# endif /*__ffly_timer__hpp*/
