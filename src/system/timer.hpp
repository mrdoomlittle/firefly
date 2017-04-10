# ifndef __ffly_timer__hpp
# define __ffly_timer__hpp
# include <eint_t.hpp>
# include <chrono>
// this will replace stop_watch
namespace mdl {
namespace firefly {
namespace system {
struct timer {
	std::chrono::high_resolution_clock::time_point begin_point;
	std::chrono::high_resolution_clock::time_point end_point;

	void begin() {begin_point = std::chrono::high_resolution_clock::now();}
	void end() {end_point = std::chrono::high_resolution_clock::now();}
	template<typename __time_unit>
	uint_t now() {
		return std::chrono::duration_cast<__time_unit>(std::chrono::high_resolution_clock::now() - this-> begin_point).count();
	}

	template<typename __time_unit>
	uint_t delta() {
		return std::chrono::duration_cast<__time_unit>(this-> end_point - this-> begin_point).count();
	}
};
}
}
}

# endif /*__ffly_timer__hpp*/
