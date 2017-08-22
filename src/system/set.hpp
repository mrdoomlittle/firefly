# ifndef __set__hpp
# define __set__hpp
# include "vec.h"
# include "../types/bool_t.h"
namespace mdl {
namespace firefly {
namespace system {
template<typename _T>
struct set {
	typedef _T* iterator;
	set() : raw(VEC_AUTO_RESIZE) {};
	_T* begin(){return this->raw.begin();}
	_T* end(){return this->raw.end();}
	types::bool_t empty(){return this->raw.empty();}
	void insert(_T __elem) {
		this->raw.push_back(__elem);
	}

	vec<_T> raw;
};
}
}
}
# endif /*__set__hpp*/
