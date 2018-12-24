# ifndef __set__hpp
# define __set__hpp
# include "vec.h"
# include "bin_tree.h"
# include "../types/bool_t.h"
# include "../types/off_t.h"
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
			ffly_vec_itr(&this->v_p->raw, (void**)&p, VEC_ITR_FD, __a);
			return (iterator){v_p:this->v_p, p:p};
		}

		_iterator& operator++(int) {
			ffly_vec_itr(&this->v_p->raw, (void**)&this->p, VEC_ITR_BK, 1);
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
		_T *p = this->v.last();
		this->tree.insert(this->v.off(p), (u64_t)__elem);
	}
	void erase(iterator __itr) {
		this->v.del(__itr.p);
		this->tree.erase(static_cast<uint_t>(*__itr.p));
	}
	iterator find(_T __elem, types::err_t& __err) {
		_T *p;
		types::off_t off;
		if (_err(__err = this->tree.find(off, (u64_t)__elem))) {
			return (iterator){};
		}
		p = &this->v.at_off(off);
		return (iterator){v_p:&this->v, p:p};
	}

	vec<_T> v;
	bin_tree<uint_t> tree;
};
}
}
}
# endif /*__set__hpp*/
