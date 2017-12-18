# ifndef __set__hpp
# define __set__hpp
# include "vec.h"
# include "../types/bool_t.h"
namespace mdl {
namespace firefly {
namespace system {
template<typename _T>
struct set {
	typedef struct _iterator {
		vec<_T> *v_p;
		_T *p;
		_T operator*() {
			return *p;
		}

		_iterator operator+(mdl_uint_t __a) {
			_T *p = this->p;
			ffly_vec_itr(&this->v_p->raw_vec, (void**)&p, VEC_ITR_FD, __a);
			return (iterator){v_p:this->v_p, p:p};
		}

		_iterator& operator++(int) {
			ffly_vec_itr(&this->v_p->raw_vec, (void**)&this->p, VEC_ITR_BK, 1);
		}

		types::bool_t operator!=(const _iterator& __val) {
			return (this->p != __val.p);
		}

		types::bool_t operator==(const _iterator& __val) {
			return (this->p == __val.p);
		}

		types::bool_t operator==(const void *__val) {
			return (this->p == __val);
		}

	} iterator;
	set() : v(VEC_AUTO_RESIZE|VEC_BLK_CHAIN|VEC_UUU_BLKS) {};

	iterator begin(){return (iterator){v_p:&this->v, p:this->v.first()};}
	iterator end(){return (iterator){v_p:&this->v, p:this->v.last()};}
	types::bool_t empty(){return this->v.empty();}
	void insert(_T __elem) {
		this->v.push_back(__elem);
	}
	void erase(iterator __itr) {
		this->v.del(__itr.p);
	}
	vec<_T> v;
};
}
}
}
# endif /*__set__hpp*/
