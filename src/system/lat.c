# include "lat.h"
# include "../system/io.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
podp static
alloc_pod() {
	podp ret = __ffly_mem_alloc(sizeof(struct pod));
	podp *p = (podp*)(ret->p = __ffly_mem_alloc(0x100*sizeof(podp)));
	while(p != ret->p+0x100)
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
	__lat->p = alloc_pod();
	__lat->head = NULL;
}

void ffly_lat_put(ffly_latp __lat, mdl_u64_t __key, void *__p) {
	podp *p = __lat->p->p;
	if (!*(p = (p+((__key&0xff)^(__key>>8&0xff))))) {
		*p = alloc_pod();
	}

	if (!*(p = (podp*)(*p)->p+((__key>>16&0xff)^(__key>>24&0xff)))) {
		*p = alloc_pod();
	}

	if (!*(p = (podp*)(*p)->p+((__key>>32&0xff)^(__key>>40&0xff)))) {
		*p = alloc_pod();
	}

	if (!*(p = (podp*)(*p)->p+((__key>>48&0xff)^(__key>>56&0xff)))) {
		*p = (podp)__ffly_mem_alloc(sizeof(struct pod));
		(*p)->p = NULL;
	}

	recordp rec = __ffly_mem_alloc(sizeof(struct record));
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
		ffly_fprintf(ffly_err, "pod 0 failure.\n");
		return NULL;
	}

	if (!*(p = (podp*)(*p)->p+((__key>>16&0xff)^(__key>>24&0xff)))) {
		ffly_fprintf(ffly_err, "pod 2 failure.\n");
		return NULL;
	}

	if (!*(p = (podp*)(*p)->p+((__key>>32&0xff)^(__key>>40&0xff)))) {
		ffly_fprintf(ffly_err, "pod 4 failure.\n");
		return NULL;
	}
  
	if (!*(p = (podp*)(*p)->p+((__key>>48&0xff)^(__key>>56&0xff)))) {
		ffly_fprintf(ffly_err, "pod 6 failure.\n");
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
	if (depth < 3) {
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
	free_pod(__lat->p);
	__ffly_mem_free(__lat->p);
}
