# ifndef __id__tracker__hpp
# define __id__tracker__hpp
# include <eint_t.hpp>
# include <errno.h>
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "../system/errno.h"
# include <cstdio>

// has not been tested havent had time
namespace mdl {
namespace firefly {
namespace data {
class id_tracker {
	boost::int8_t add_id(uint_t *__id) {
		uint_t change = 0;
		if (this-> id_count == 0) {
			this-> id_indx = (uint_t **)memory::mem_alloc(__id * sizeof(uint_t *));
			if (this-> id_indx == NULL) {
				fprintf(stderr, "id_tracker: failed to alloc memory for 'id_indx', errno: %d\n", errno);
				return FFLY_FAILURE;
			}
		} else if (this-> id_count != 0 && this-> highest < *__id) {
			uint_t **id_indx = (uint_t **)memory::mem_realloc(this-> id_indx, (*__id) * sizeof(uint_t *));
			if (id_indx == NULL) {
				fprintf(stderr, "id_tracker: failed to alloc memory for 'id_indx', errno: %d\n", errno);
				return FFLY_FAILURE;
			} else
				this-> id_indx = id_indx;
		}

		*(this-> id_indx + (*__id)) = __id;

		return FFLY_SUCCESS;
	}

	void update_id(uint_t *__old_id, uint_t *__id) {
		if (this-> highest < *__id || this-> highest < *__old_id) return FFLY_FAILURE;

		*(this-> id_indx + (*__old_id)) = nullptr;
		*(this-> id_indx + (*__id)) = __id;
 	}

	bool is_id(uint_t *__id) {
		if (this-> highest < *__id) return FFLY_FAILURE;

		return *(this-> id_indx + (*__id)) == *__id? true : false;
	}

	private:
	uint_t highest = 0;
	uint_t id_count = 0;
	uint_t **id_indx;
};
}
}
}
# endif /*__id__tracker__hpp*/
