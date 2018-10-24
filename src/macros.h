# define __ffly_inna(__dst, __src) \
    if ((__dst) != NULL) *(__dst) = (__src);
# define __ffly_finn(__p) \
	if ((__p) != NULL) __ffly_mem_free((void*)(__p))
