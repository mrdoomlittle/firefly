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
	typedef _T val_type;
	bool empty() {return this-> _size == 0? true : false;}

	vector() {
		this-> first = false;
		this-> _size = 0;
	}

	types::err_t resize(uint_t __size, u8_t __dir = 0) {
		if (__dir != 0) {
			if (__dir == -1) {
				__size = this-> size() - __size;
			}

			if (__dir == 1) {
				__size = this-> size() + __size;
			}
		}

		if (__size == 0) {
			this-> _size = 0;
			memory::mem_free(this-> data);
			return FFLY_SUCCESS;
		}

		if (this-> _size == 0 || !this-> first)
			this-> data = (val_type *)memory::mem_alloc(__size * sizeof(val_type));
		else
			this-> data = (val_type *)memory::mem_realloc(this-> data, __size * sizeof(val_type));

		if (this-> data == NULL)
			return FFLY_FAILURE;

		this-> _size = __size;
		this-> first = true;
		return FFLY_SUCCESS;
	}

	void swp(val_type *__apoint, uint_t __bpoint) {
		swp(__bpoint, __apoint);
	}

	void swp(uint_t __apoint, val_type *__bpoint) {
		val_type tmp = *__bpoint;
		*__bpoint = this-> data[__apoint];
		this-> data[__apoint] = tmp;
	}

	void swp(uint_t __apoint, uint_t __bpoint) {
		val_type tmp = this-> data[__bpoint];
		this-> data[__bpoint] = this-> data[__apoint];
		this-> data[__apoint] = tmp;
	}

	val_type& push_back() {
		this-> resize(this-> size() + 1);
		return *this-> begin();
	}

	void push_back(val_type __val) {
		this-> push_back() = __val;
	}

	val_type& operator[](uint_t __point) {return this-> data[(this-> _size - 1) - __point];}

	val_type* begin() {return this-> data + (this-> _size - 1);}

	val_type* end() {return this-> data;}
	val_type *iterator;

	uint_t size() {return this-> _size;}
	private:
	bool first = false;
	uint_t _size = 0;
	val_type *data = nullptr;
};

}
}
}

# endif /*__ffly_vector__hpp*/
