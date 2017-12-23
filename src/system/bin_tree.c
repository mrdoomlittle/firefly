# include "bin_tree.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "errno.h"
# include "io.h"
# include "err.h"
struct node {
	mdl_u8_t inuse;
	mdl_uint_t key;
	void *p;
	struct node *lhs, *rhs, *back;
};

ffly_err_t ffly_bin_tree_init(struct ffly_bin_tree *__bin_tree) {
	__bin_tree->root = NULL;
	return FFLY_SUCCESS;
}

ffly_err_t _ffly_bin_tree_find(struct ffly_bin_tree *__bin_tree, mdl_uint_t __key, struct node **__node) {
	struct node *branch = __bin_tree->root;
	_again:
	if (!branch) {
		*__node = NULL;
		ffly_fprintf(ffly_err, "root branch hasen't been allocated.\n");
		return FFLY_FAILURE;
	}

	if (branch->key == __key) {
		if (!branch->inuse)
			*__node = NULL;
		else
			*__node = branch;
		goto _end;
	}
	if (branch->key < __key)
		branch = branch->lhs;
	else if (branch->key > __key)
		branch = branch->rhs;
	goto _again;
	_end:
	return FFLY_SUCCESS;
}

ffly_err_t ffly_bin_tree_find(struct ffly_bin_tree *__bin_tree, mdl_uint_t __key, void **__p) {
	struct node *branch;
	ffly_err_t err;
	if (_err(err = _ffly_bin_tree_find(__bin_tree, __key, &branch))) {
		return err;
	}
	if (!branch) {
		*__p = NULL;
		ffly_fprintf(ffly_err, "failed to find branch, key: %u\n", __key);
		return FFLY_FAILURE;
	} else
		*__p = branch->p;
	return FFLY_SUCCESS;
}

ffly_err_t static insert(struct node *__root, mdl_uint_t __key, void *__p) {
	struct node *branch = __root;
	_again:
	if (branch->key < __key) {
		if (branch->lhs != NULL) {
			if (!branch->lhs->inuse && branch->lhs->key == __key) {
				branch->lhs->p = __p;
				branch->inuse = 1;
				goto _end;
			}
			branch = branch->lhs;
			goto _again;
		}
		if ((branch->lhs = (struct node*)__ffly_mem_alloc(sizeof(struct node))) == NULL) {
			return FFLY_FAILURE;
		}
		*branch->lhs = (struct node) {
			.inuse = 1, .key = __key, .p = __p,
			.lhs = NULL, .rhs = NULL, .back = branch
		};
	} else if (branch->key > __key) {
		if (branch->rhs != NULL) {
			if (!branch->rhs->inuse && branch->rhs->key == __key) {
				branch->rhs->p = __p;
				branch->rhs->inuse = 1;
				goto _end;
			}
			branch = branch->rhs;
			goto _again;
		}
		if ((branch->rhs = (struct node*)__ffly_mem_alloc(sizeof(struct node))) == NULL) {
			return FFLY_FAILURE;
		}
		*branch->rhs = (struct node) {
			.inuse = 1, .key = __key, .p = __p,
			.lhs = NULL, .rhs = NULL, .back = branch
		};
	}
	_end:
	return FFLY_SUCCESS;
}

void static remap_branch(struct node *__dst, struct node *__src) {
	if (!__src) return;
	insert(__dst, __src->key, __src->p);
	if (__src->lhs != NULL) {
		remap_branch(__dst, __src->lhs);
		__ffly_mem_free(__src->lhs);
	}
	if (__src->rhs != NULL) {
		remap_branch(__dst, __src->rhs);
		__ffly_mem_free(__src->rhs);
	}
}

ffly_err_t ffly_bin_tree_del(struct ffly_bin_tree *__bin_tree, mdl_uint_t __key) {
	struct node *branch;
	ffly_err_t err;
	if (_err(err = _ffly_bin_tree_find(__bin_tree, __key, &branch))) {
		return err;
	}
	if (branch == __bin_tree->root) {
		__ffly_mem_free(branch);
		__bin_tree->root = NULL;
	} else {
		struct node *re;
		if (branch->lhs != NULL) {
			re = branch->lhs;
			remap_branch(re, branch->rhs);
			__ffly_mem_free(branch->rhs);
		}
		else if (branch->rhs != NULL) {
			re = branch->rhs;
			remap_branch(re, branch->lhs);
			__ffly_mem_free(branch->lhs);
		}
		if (branch == branch->back->lhs)
			branch->back->lhs = re;
		else if (branch == branch->back->rhs)
			branch->back->rhs = re;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_bin_tree_erase(struct ffly_bin_tree *__bin_tree, mdl_uint_t __key) {
	struct node *branch;
	ffly_err_t err;
	if (_err(err = _ffly_bin_tree_find(__bin_tree, __key, &branch))) {
		return err;
	}
	if (branch != NULL) {
		branch->p = NULL;
		branch->inuse = 0;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_bin_tree_insert(struct ffly_bin_tree *__bin_tree, mdl_uint_t __key, void *__p) {
	if (!__bin_tree->root) {
		if ((__bin_tree->root = __ffly_mem_alloc(sizeof(struct node))) == NULL) {
			return FFLY_FAILURE;
		}
		*(struct node*)__bin_tree->root = (struct node) {
			.inuse = 1, .key = __key, .p = __p,
			.lhs = NULL, .rhs = NULL, .back = NULL
		};
	} else {
		ffly_err_t err;
		if (_err(err = insert(__bin_tree->root, __key, __p))) {
			return err;
		}
	}
	return FFLY_SUCCESS;
}

void static free_tree(struct node *__branch) {
	if (!__branch) return;
	if (__branch->lhs != NULL) free_tree(__branch->lhs);
	if (__branch->rhs != NULL) free_tree(__branch->rhs);
}

ffly_err_t ffly_bin_tree_de_init(struct ffly_bin_tree *__bin_tree) {
	free_tree(__bin_tree->root);
	return FFLY_SUCCESS;
}
