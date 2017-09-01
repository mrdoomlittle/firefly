# ifndef __stop__watch__hpp
# define __stop__watch__hpp
# include <eint_t.hpp>
# include <chrono>
namespace mdl {
namespace firefly {
namespace system {
class stop_watch {
public:
std::chrono::high_resolution_clock::time_point begin_point;
std::chrono::high_resolution_clock::time_point end_point;

void begin() {
	begin_point = std::chrono::high_resolution_clock::now();
}

void stop() {
	this-> time_point();
}

void time_point() {
	end_point = std::chrono::high_resolution_clock::now();
}

template<typename __time_unit>
uint_t get_delta() {
	return std::chrono::duration_cast<__time_unit>(this-> end_point - this-> begin_point).count();
}

};
}
}
}

# endif /*__stop__watch__hpp*/
