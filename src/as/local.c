# include "as.h"
# include "alloca.h"
ff_u16_t static n = 0;

long long static *array;
long long* as_local_new(ff_u16_t __n) {
	if (!array) {
		n = __n+1;
		array = (long long*)ff_as_al(n*sizeof(long long));
	} else {
		if (__n>=n) {
			n = __n+1;
			array = (long long*)ff_as_ral(array, n*sizeof(long long));
		}
	}
	return array+__n;
}

long long* as_local_get(ff_u16_t __n) {
	return array+__n;
}
