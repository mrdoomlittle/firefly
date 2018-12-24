# ifndef __queue__hpp
# define __queue__hpp
# include <cstdio>
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "../types/err_t.h"
# include "../errno.h"
# define _QUEUE_PAGE_SIZE 32
namespace mdl {
namespace firefly {
namespace system {
template<typename _T>
struct queue {
	typedef _T* itorator;

	_T __inline__ front() {return (*(this->mem_ptr+(this->begin_off>>5)))[this->begin_off-((this->begin_off>>5)*_QUEUE_PAGE_SIZE)];}
	_T __inline__ end() {return (*(this->mem_ptr+(this->end_off>>5)))[this->end_off-((this->end_off>>5)*_QUEUE_PAGE_SIZE)];}

	mdl::uint_t size() {
		mdl::uint_t ret_val = 0;
		if (this->end_off < this->begin_off)
			ret_val = this->end_off+((this->page_c*_QUEUE_PAGE_SIZE)-this->begin_off);

		if (this->end_off > this->begin_off)
			ret_val = this->end_off-this->begin_off;
		return ret_val;
	}

	types::err_t push(_T __element) {
		if (this->end_off+1 == this->begin_off) {
			printf("queue: overflow.\n");
			return FFLY_NOP;
		}

		if (!this->mem_ptr) {
			if ((this->mem_ptr = (_T**)memory::mem_alloc((++this->page_c)*sizeof(_T*))) == NULL) {
				return FFLY_FAILURE;
			}
			goto _init_page;
		}

		if (this->end_off >> 5 == this->page_c) {
			if ((this->mem_ptr = (_T**)memory::mem_realloc(this->mem_ptr, (++this->page_c)*sizeof(_T*))) == NULL) {
				return FFLY_FAILURE;
			}
			goto _init_page;
		}

		goto _sk_init_page;
		_init_page:
		if ((*(this->mem_ptr+(this->page_c-1)) = (_T*)memory::mem_alloc(_QUEUE_PAGE_SIZE*sizeof(_T))) == NULL) {
			return FFLY_FAILURE;
		}
		_sk_init_page:

		if (this->begin_off >> 5 > 0 && this->end_off+1 == this->page_c*_QUEUE_PAGE_SIZE) this->end_off = 0;

		mdl::uint_t page = this->end_off >> 5;
		if (!*(this->mem_ptr+page)) {
			if ((*(this->mem_ptr+page) = (_T*)memory::mem_alloc(_QUEUE_PAGE_SIZE*sizeof(_T))) == NULL) {
				return FFLY_FAILURE;
			}
		}

		(*(this->mem_ptr+page))[(this->end_off++)-(page*_QUEUE_PAGE_SIZE)] = __element;
	}

	_T pop(types::err_t& __any_err) noexcept {
		if (this->begin_off+1 == this->end_off) {
			printf("queue: underflow.\n");
			__any_err = FFLY_NOP;
			throw;
		}

		if (this->end_off < this->begin_off && this->begin_off+1 == this->page_c*_QUEUE_PAGE_SIZE) {
			this->begin_off = 0;
		}

		mdl::uint_t page = this->begin_off >> 5;
		_T ret_val = (*(this->mem_ptr+page))[(this->begin_off++)-(page*_QUEUE_PAGE_SIZE)];
		if (this->begin_off >> 5 > page) {
			memory::mem_free(*(this->mem_ptr+page));
			*(this->mem_ptr+page) = nullptr;
		}

		return ret_val;
	}

	private:
	uint_t page_c = 0;
	uint_t begin_off = 0, end_off = 0;
	_T **mem_ptr = nullptr;
};
}
}
}


# endif /*__queue__hpp*/
