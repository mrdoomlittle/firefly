# include "opt.h"
# include "system/err.h"
# include "system/map.h"
# include "dep/str_len.h"
# include "dep/str_dup.h"
# include "system/io.h"
# include "memory/mem_free.h"
# include "depart.h"
static struct ffly_map opt;
void ffoe_prep() {
	ffly_map_init(&opt, _ffly_map_127);
}

void ffoe_end() {
	void const *cur = ffly_map_beg(&opt);
	void const *end = ffly_map_end(&opt);
	while(cur != end) {
		ffoptp p = (ffoptp)ffly_map_getp(cur);
//		__ffly_mem_free((void*)p->val);
		ffly_depart((void*)p->val);
		__ffly_mem_free(p);
		ffly_map_fd(&opt, &cur);
	}

	ffly_map_de_init(&opt);
}

void static
putopt(char const *__name, ffoptp __opt) {
	ffly_map_put(&opt, (ff_u8_t const*)__name, ffly_str_len(__name), __opt);
}

ffoptp static
getopt(char const *__name) {
	ff_err_t err;
	ffoptp ret = (ffoptp)ffly_map_get(&opt, (ff_u8_t const*)__name, ffly_str_len(__name), &err);
	if (_err(err))
		return NULL;
	return ret;
}

// might not work, not tested
char const *ffoe_pcl(void *__arg_p) {
	ffpclp pcl = (ffpclp)__arg_p;
	if (pcl->p >= pcl->end)
		return NULL;

	char *p = __arg_p;
	char *bufp = pcl->buf;
	while(*p != ' ' && p < pcl->end)
		*(bufp++) = *(p++);
	*bufp = '\0';
	p++; // skspace
	return (char const*)pcl->buf;
}

char const* ffoe_pcll(void *__arg_p) {
	ffpcllp pcl = (ffpcllp)__arg_p;
	return pcl->cur>=pcl->end?NULL:*(pcl->cur++);
}

# include "memory/mem_alloc.h"
void ffoe(char const*(*__pcl)(void*), void *__arg_p) {
	char const *p;
	char buf[128];
	char *bufp;
	while((p = __pcl(__arg_p)) != NULL) {
		ffly_printf("optname: '%s'\n", p);
		while(*p != '\0') {
			if (*(p++) == '-') {
				bufp = buf;
				while(*p != '\0')
					*(bufp++) = *(p++);
				*bufp = '\0';
				// next
				if (!(p = __pcl(__arg_p))) {
					ffly_printf("error");
					return;
				}

				// should use alloca
				ffoptp opt = (ffoptp)__ffly_mem_alloc(sizeof(struct ffopt));
				opt->val = ffly_str_dup(p);

				putopt(buf, opt);
				break;
			}
		}
	}
}

ffoptp ffoe_get(char const *__name) {
	return getopt(__name);
}

// go thru the map and remove everything 
void ffoe_clr() {

}
