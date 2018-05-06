# include "as.h"
# include "../malloc.h"
typedef struct hdr {
	struct hdr *next;
} *hdrp;


hdrp static top = NULL;
# define hdrsize sizeof(struct hdr)

void _cleanup() {
	if (!top) return;
	hdrp cur = top, bk;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		free(bk);
	}
}

void* _alloca(ff_uint_t __size) {
	ff_u8_t *p = (ff_u8_t*)malloc(hdrsize+__size);
	hdrp hdr = (hdrp)p;

	hdr->next = top;
	top = hdr;
	return p+hdrsize;
}
