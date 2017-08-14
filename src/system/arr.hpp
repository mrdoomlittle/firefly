# ifndef __arr__hpp
# define __arr__hpp
# include <cstdlib>
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
namespace mdl {
namespace firefly {
namespace system {
template <typename __T, std::size_t __N>
struct arr {
	public:
	array() {this->p = static_cast<__T*>(__ffly_mem_alloc(__N*sizeof(__T)));}
	__T &operator[](std::size_t __off) {return this->p[__off];}
	~array() {__ffly_mem_free(this->p);}
	mdl::uint_t size() {return __N;}
	private:
	__T *p = nullptr;
};
}
}
}


# endif /*__arr__hpp*/
