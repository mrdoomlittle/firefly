# ifndef __ffly_vector__hpp
# define __ffly_vector__hpp
# include "errno.h"
# include <cstdio>
# include <eint_t.hpp>
# include "../types/err_t.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
namespace mdl {
namespace firefly {
namespace system {
template<typename _T>
struct vector {
	typedef _T value_type;
	bool empty() {return this-> size == 0? true : false;}

	types::err_t resize(uint_t __size) {
		if (__size == 0) {
			this-> _size = 0;
			memory::mem_free(this-> data);
			return FFLY_SUCCESS;
		}

		if (this-> _size == 0)
			this-> data = (value_type *)memory::mem_alloc(__size * sizeof(value_type));
		else
			this-> data = (value_type *)memory::mem_realloc(this-> data, __size * sizeof(value_type));

		if (this-> data == NULL)
			return FFLY_FAILURE;

		this-> _size = __size;

		return FFLY_SUCCESS;
	}

	value_type& push_back() {
		this-> resize(this-> size() + 1);
		return *this-> begin();
	}

	value_type& operator[](uint_t __point) {return this-> data[(this-> _size - 1) - __point];}

	value_type *begin() {return this-> data + (this-> _size - 1);}

	value_type *end() {return this-> data;}
	value_type *iterator;

	uint_t size() {return this-> _size;}
	private:
	uint_t _size = 0;
	value_type *data = nullptr;
};

}
}
}

# endif /*__ffly_vector__hpp*/
