# include "as.h"
# include "../malloc.h"
/*
	for this later, make it so 
	after x blocks it starts to free the overflow
*/

typedef struct hdr {
	struct hdr *next;
} *hdrp;


hdrp static top = NULL;
hdrp static last = NULL;

# define hdrsize sizeof(struct hdr)

void _cleanup() {
	if (!top) return;
	hdrp bk = NULL;
	hdrp cur = top;
	while(cur != NULL) {
		if (bk != NULL)
			free(bk);


		bk = cur;
		cur = cur->next;
	}

	if (top != NULL)
		free(top);
}

void* _alloca(mdl_uint_t __size) {
	mdl_u8_t *p = (mdl_u8_t*)malloc(hdrsize+__size);
	hdrp hdr = (hdrp)p;

	if (top != NULL)
		top = hdr;

	hdr->next = last;
	last = hdr;
	return p+hdrsize;
}
