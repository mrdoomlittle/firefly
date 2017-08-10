# ifndef __queue__hpp
# define __queue__hpp
# include <cstdio>
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "../types/err_t.h"
# include "../errno.h"
# include <errno.h>
namespace mdl {
namespace firefly {
namespace system {
template<typename _T>
struct queue {
	typedef _T* itorator;

	uint_t const page_size = 20;
	uint_t const page_bc = page_size*sizeof(_T);

	size_t __inline__ get_ele_c() {return this-> ele_c;}

	void dump() {
	//	return;
		if (!this-> ele_c)return;
		printf("---> ");
		for (uint_t x{}; x != this-> page_c*this-> page_size; x++)
			printf("%d, ", this-> data[x]);
		printf("\n");
	}

	_T __inline__* begin() {return this->data;}
	_T __inline__* end() {return this-> data+(this->ele_c+this->epoint_fs);}
	_T __inline__ front() {return *(this->data+this->epoint_fs);}

	types::err_t push(_T __element) {
		if (!this-> data && !this-> ele_c) {
			if ((this-> data = (_T*)memory::mem_alloc(page_bc)) == NULL) {
				fprintf(stderr, "queue: failed to alloc memory, errno: %d, ffly_errno: %d\n", errno, ffly_errno);
				return FFLY_FAILURE;
			}
			this-> page_c++;
		} else {
			if (this-> epoint > this-> page_c*this-> page_size) {
				if ((this-> data = (_T*)memory::mem_realloc(this-> data, (++this-> page_c)*page_bc)) == NULL) {
					fprintf(stderr, "queue: failed to realloc memory, errno: %d, ffly_errno: %d\n", errno, ffly_errno);
					return FFLY_FAILURE;
				}
			}
		}

	//	printf("%d - %d\n", this-> epfs_point, this-> epoint_fs);

		if (!this-> epoint_fs || this-> epfs_point+1 > this-> epoint_fs) {
			this-> data[epoint] = __element;
			epoint++;
		} else {
	//		printf("hek\n");
			this-> data[epfs_point] = __element;

			//if (this-> epfs_point == this-> epoint_fs - 1)
			//	this-> epfs_point = 0;
			//else
			this-> epfs_point++;
		}

		ele_c++;
		return FFLY_SUCCESS;
	}

	_T pop(types::err_t& __any_err) noexcept {
		if (!this-> data && !ele_c) {__any_err = FFLY_FAILURE;throw;}
		_T to_return = this-> data[epoint_fs];

		if (!(ele_c-1)) {
			memory::mem_free(this-> data);
			this-> data = nullptr;
			this-> page_c--;
		} else {
			if (this-> page_c != 1 && epoint < (this-> page_c-1)*this-> page_size) {
				if ((this-> data = (_T*)memory::mem_realloc(this-> data, (--this-> page_c)*page_bc)) == NULL) {
					fprintf(stderr, "queue: failed to realloc memory, errno: %d, ffly_errno: %d\n", errno, ffly_errno);
					return FFLY_FAILURE;
				}
			}
		}

		if (this-> epoint_fs == this-> epoint - 1) {
			printf("ddhudh\n");
			this-> epoint_fs = 0;
			this-> epoint = 0;
			this-> epfs_point = 0;
		} else
			epoint_fs++;

		ele_c--;
		__any_err = FFLY_SUCCESS;
		return to_return;
	}

	void de_init() {
		memory::mem_free(this-> data);
		this-> data = nullptr;
		this-> epoint = this-> epoint_fs = this-> epfs_point = this-> ele_c = this-> page_c = 0;
	}

	uint_t epoint = 0, epoint_fs = 0;
	uint_t epfs_point = 0;
	uint_t ele_c = 0, page_c = 0;
	_T *data = nullptr;
};
}
}
}

# endif /*__queue__hpp*/
