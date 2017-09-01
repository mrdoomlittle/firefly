# ifndef __arr__hpp
# define __arr__hpp
# include <cstdlib>
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
namespace mdl {
namespace firefly {
namespace system {
template <typename _T, std::size_t _N>
struct arr {
	public:
	arr() {this->p = static_cast<_T*>(__ffly_mem_alloc(_N*sizeof(_T)));}
	_T &operator[](std::size_t __off) {return this->p[__off];}
	~arr() {__ffly_mem_free(this->p);}
	mdl::uint_t size() {return _N;}
	_T *get_ptr() {return this->p;}
	private:
	_T *p = nullptr;
};
}
}
}


# endif /*__arr__hpp*/
