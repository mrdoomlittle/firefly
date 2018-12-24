# ifndef __matrix__hpp
# define __matrix__hpp
# include "vec.h"
# include <string.h>
namespace mdl {
namespace firefly {
namespace system {
template<typename _T>
struct matrix {
	matrix(uint_t __xa_len, uint_t __ya_len) {this->_vec.resize(__xa_len*__ya_len);}
	_T& operator()(uint_t __xa, uint_t __ya) {return this->_vec.at(__xa*__ya);}
	void resize(uint_t __xa_len, uint_t __ya_len) {this->_vec.resize(__xa_len*__ya_len);}
	void clear() {memset(this->_vec.begin(), 0, this->_vec.size());}
	vec<_T> _vec;
};
}
}
}

# endif /*__matrix__hpp*/
