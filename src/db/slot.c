# include "../ffint.h"
# include "../ffly_def.h"
# include "../system/mutex.h"
/*
	should make this automatic
*/
#define NO_SLOTS 20
/*
	so we dont need to give the pointers to clients
*/
ff_mlock_t static lock = FFLY_MUTEX_INIT;

void static *slot[NO_SLOTS];
void static **fresh = slot;

ff_uint_t static vacant[NO_SLOTS];
ff_uint_t static *next = vacant;

ff_uint_t acquire_slot() {
	ffly_mutex_lock(&lock);
	ff_uint_t ret;
	if (next>vacant) {
		ret = *(--next);
		goto _end;
	}
	if (fresh>=slot+NO_SLOTS) {
		//err
		ret = 0;
		goto _end;
	}
	ret = (fresh++)-slot;
_end:
	ffly_mutex_unlock(&lock);
	return ret;
}

void scrap_slot(ff_uint_t __no) {
	ffly_mutex_lock(&lock);
	if (__no>=NO_SLOTS)
		goto _end;
	void **p = slot+__no;
	if (p+1 == fresh)
		fresh--;
	else
		*(next++) = __no;
_end:
	ffly_mutex_unlock(&lock);
}

void *slotget(ff_uint_t __no) {
	ffly_mutex_lock(&lock);
	void *ret;
	if (__no>=NO_SLOTS) {
		ret = NULL;
		goto _end;
	}
	ret = *(slot+__no);
_end:
	ffly_mutex_unlock(&lock);
	return ret;
}

void slotput(ff_uint_t __no, void *__p) {
	ffly_mutex_lock(&lock);
	if (__no>=NO_SLOTS)
		goto _end;
	*(slot+__no) = __p;
_end:
	ffly_mutex_unlock(&lock);
}
