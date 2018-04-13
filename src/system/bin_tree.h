# ifndef __ffly__bin_tree__h
# define __ffly__bin_tree__h
# include "../ffint.h"
# include "../types.h"
struct ffly_bin_tree {
	void *root;
};

# ifdef __cplusplus
extern "C" {
# endif
ff_err_t ffly_bin_tree_init(struct ffly_bin_tree*);
ff_err_t ffly_bin_tree_insert(struct ffly_bin_tree*, ff_u64_t, void*);
ff_err_t ffly_bin_tree_find(struct ffly_bin_tree*, ff_u64_t, void**);
ff_err_t ffly_bin_tree_del(struct ffly_bin_tree*, ff_u64_t);
ff_err_t ffly_bin_tree_erase(struct ffly_bin_tree*, ff_u64_t);
ff_err_t ffly_bin_tree_de_init(struct ffly_bin_tree*);
# ifdef __cplusplus
}
# include "../memory/mem_alloc.h"
# include "err.h"
namespace mdl {
namespace firefly {
namespace system {
template<typename _T>
struct bin_tree {
	bin_tree() {this->init();}
	~bin_tree() {this->de_init();}

	types::err_t init() {
		return ffly_bin_tree_init(&this->raw);
	}

	types::err_t de_init() {
		return ffly_bin_tree_de_init(&this->raw);
	}

	types::err_t insert(_T __v, u64_t __key) {
		_T *p;
		*(p = static_cast<_T*>(memory::mem_alloc(sizeof(_T)))) = __v;
		return ffly_bin_tree_insert(&this->raw, __key, p);
	}

	types::err_t find(_T& __v, u64_t __key) {
		_T *p;
		types::err_t err;
		if (_err(err = ffly_bin_tree_find(&this->raw, __key, reinterpret_cast<void**>(&p)))) {
			system::io::fprintf(ffly_err, "bin_tree: failed to find.\n");
			return FFLY_FAILURE;
		}
		if (p != nullptr)
			__v = *p;
		return FFLY_SUCCESS;
	}
	types::err_t del(u64_t __key) {
		return ffly_bin_tree_del(&this->raw, __key);
	}

	types::err_t erase(u64_t __key) {
		return ffly_bin_tree_erase(&this->raw, __key);
	}
	struct ffly_bin_tree raw;
};
}
}
}
# endif
# endif /*__ffly__bin_tree__h*/
