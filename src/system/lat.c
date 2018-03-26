# define __ffly_lat_internal
# include "lat.h"
# include "../system/io.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
podp static
alloc_pod() {
	podp ret;
	if (!(ret = (podp)__ffly_mem_alloc(sizeof(struct pod)))) {
		ffly_fprintf(ffly_err, "failed to allocate.\n");
		return NULL;
	}

	podp *p;
	if (!(p = (podp*)(ret->p = __ffly_mem_alloc(0x100*sizeof(podp))))) {
		ffly_fprintf(ffly_err, "failed to allocate.\n");
		return NULL;
	}

	while(p != (podp*)ret->p+0x100)
		*(p++) = NULL;
	return ret;
}

void* ffly_lat_head(ffly_latp __lat) {
	return (void*)__lat->head;
}

void ffly_lat_fd(void **__p) {
	*__p = (void*)((recordp)*__p)->fd;
}

void* ffly_lat_getp(void *__p) {
	return ((recordp)__p)->p;
}

void ffly_lat_prepare(ffly_latp __lat) {
	if (!(__lat->p = alloc_pod())) {
		ffly_fprintf(ffly_err, "failed to allocate.\n");
		return;
	}
	__lat->head = NULL;
}

void ffly_lat_put(ffly_latp __lat, mdl_u64_t __key, void *__p) {
	podp *p = (podp*)__lat->p->p;
	if (!*(p = (p+((__key&0xff)^(__key>>8&0xff))))) {
		if (!(*p = alloc_pod())) {
			ffly_fprintf(ffly_err, "pod 0 allocation failed.\n");
			return;
		} else
			ffly_fprintf(ffly_log, "pod 0 allocated.\n");
	} else
		ffly_fprintf(ffly_log, "pod 0 passthrough.\n");

	if (!*(p = (podp*)(*p)->p+((__key>>16&0xff)^(__key>>24&0xff)))) {
		if (!(*p = alloc_pod())) {
			ffly_fprintf(ffly_err, "pod 1 allocation failed.\n");
			return;
		} else
			ffly_fprintf(ffly_log, "pod 1 allocated.\n");
	} else
		ffly_fprintf(ffly_log, "pod 1 passthrough.\n");

	if (!*(p = (podp*)(*p)->p+((__key>>32&0xff)^(__key>>40&0xff)))) {
		if (!(*p = alloc_pod())) {
			ffly_fprintf(ffly_err, "pod 2 allocation failed.\n");
			return;
		} else
			ffly_fprintf(ffly_log, "pod 2 allocated.\n");
	} else
		ffly_fprintf(ffly_log, "pod 2 passthrough.\n");

	if (!*(p = (podp*)(*p)->p+((__key>>48&0xff)^(__key>>56&0xff)))) {
		if (!(*p = (podp)__ffly_mem_alloc(sizeof(struct pod)))) {
			ffly_fprintf(ffly_err, "pod 3 allocation failed.\n");
			return;
		} else
			ffly_fprintf(ffly_log, "pod 3 allocated.\n");
		(*p)->p = NULL;
	} else
		ffly_fprintf(ffly_log, "pod 3 passthrough.\n");

	recordp rec;
	if (!(rec = (recordp)__ffly_mem_alloc(sizeof(struct record)))) {
		ffly_fprintf(ffly_err, "failed to allocate.\n");
		return;
	}

	rec->next = (recordp)(*p)->p;
	(*p)->p = (void*)rec;
	rec->key = __key;
	rec->p = __p;
	rec->fd = __lat->head;
	__lat->head = rec;
}

void* ffly_lat_get(ffly_latp __lat, mdl_u64_t __key) {
	podp *p = (podp*)__lat->p->p;
	if (!*(p = (p+((__key&0xff)^(__key>>8&0xff))))) {
		ffly_fprintf(ffly_err, "pod 0 dead-end.\n");
		return NULL;
	}

	if (!*(p = (podp*)(*p)->p+((__key>>16&0xff)^(__key>>24&0xff)))) {
		ffly_fprintf(ffly_err, "pod 1 dead-end.\n");
		return NULL;
	}

	if (!*(p = (podp*)(*p)->p+((__key>>32&0xff)^(__key>>40&0xff)))) {
		ffly_fprintf(ffly_err, "pod 2 dead-end.\n");
		return NULL;
	}
  
	if (!*(p = (podp*)(*p)->p+((__key>>48&0xff)^(__key>>56&0xff)))) {
		ffly_fprintf(ffly_err, "pod 3 dead-end.\n");
		return NULL;
	}

	recordp rec = (recordp)(*p)->p;
	while(rec != NULL) {
		if (rec->key == __key) return rec->p;
		rec = rec->next;
	}
	
	ffly_fprintf(ffly_err, "not found.\n");
	return NULL;
}

void static
free_pod(podp __pod) {
	mdl_u8_t static depth = 0;
	if (depth <= 3) {
		depth++;
		podp *p = (podp*)__pod->p;
		while(p != (podp*)__pod->p+0x100) {
			if (*p != NULL) {
				free_pod(*p);
				__ffly_mem_free(*p);
			}
			p++;
		}
		depth--;
		if (__pod->p != NULL)
			__ffly_mem_free(__pod->p);
	} else {
		recordp rec = (recordp)__pod->p, bk;
		while(rec != NULL) {
			bk = rec;	
			rec = rec->next;
			__ffly_mem_free(bk);
		}
	}
}

void ffly_lat_free(ffly_latp __lat) {
	if (__lat->p != NULL) {
		free_pod(__lat->p);
		__ffly_mem_free(__lat->p);
	}
}
