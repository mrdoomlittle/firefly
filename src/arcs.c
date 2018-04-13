# include "arcs.h"
# include "ffly_def.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/dict.h"
struct ffly_arc __ffly_arcroot__;
ffly_arcp __ffly_arccur__ = &__ffly_arcroot__;

struct ffly_dict static dict;

ff_u64_t static deadno[200];
ff_u64_t static out = 0;

ff_u64_t static* fresh = deadno;

ff_u64_t ffly_arcs_alno() {
	if (fresh > deadno)
		return *(--fresh);	
	return out++;	
}

void ffly_arcs_frno(ff_u64_t __no) {
	if (out-1 == __no) out--;
	*(fresh++) = __no;
}

void ffly_arcs_init() {
	ffly_arc_prepare(&__ffly_arcroot__);
	ffly_dict_init(&dict);
}

void ffly_arcs_de_init() {
	void *p = ffly_dict_head(&dict);
	while(p != NULL) {
		__ffly_mem_free((void*)ffly_dict_getp(p));	
		ffly_dict_fd(&p);
	}

	ffly_dict_de_init(&dict);
	ffly_arc_free(&__ffly_arcroot__);
}

# include "memory/alloca.h"
# include "stdio.h"
# include "dep/str_cpy.h"
# include "system/util/base64.h"
ff_u64_t* ffly_arcs_alias(char const *__name) {
	ff_err_t err;
	char const *s;
	void *frame = NULL;
	if (&__ffly_arcroot__ != __ffly_arccur__) {
		frame = ffly_frame();
		char *buf = (char*)ffly_alloca(128, NULL);
		ffly_b64_encode(buf, &__ffly_arccur__->p->no, sizeof(ff_u64_t));	
		ffly_str_cpy(buf+ffly_b64_enc_len(sizeof(ff_u64_t)), __name);
		s = buf;
	} else
		s = __name;

	ff_u64_t *p = NULL;
	if (!(p = (ff_u64_t*)ffly_dict_get(&dict, s, &err)))
		// remove __ffly_mem_alloc for somthing else
		ffly_dict_put(&dict, s, p = (ff_u64_t*)__ffly_mem_alloc(sizeof(ff_u64_t)));
	if (frame != NULL)
		ffly_collapse(frame);
	if (!p) {
		ffly_printf("error.\n");
	}

	return p;
}

ffly_arc_recp ffly_arc_lookup(ffly_arcp __arc, ff_u64_t __no) {
	ffly_arc_recp cur = *(__arc->rr+(__no&0xff));
	while(cur != NULL) {
		if (cur->no == __no) return cur;
		cur = cur->fd;
	}
	return NULL;
}

void ffly_arc_prepare(ffly_arcp __arc) {
	__arc->rr = (ffly_arc_recp*)__ffly_mem_alloc(0x100*sizeof(ffly_arc_recp));
	ffly_arc_recp *p = __arc->rr;
	ffly_arc_recp *end = p+0x100;
	while(p != end)
		*(p++) = NULL;
	__arc->bk = NULL;
	__arc->p = NULL;
}

void ffly_arc_free(ffly_arcp __arc) {
	ffly_arc_recp *p = __arc->rr;
	ffly_arc_recp *end = p+0x100;
	ffly_arc_recp rec, tmp;
	while(p != end) {
		if (*p != NULL) {
			rec = *p;
			while(rec != NULL) {
				tmp = rec->fd;
				if (rec->sort == _ffly_rec_arc)
					ffly_arc_free((ffly_arcp)rec->p);
				//__ffly_mem_free(rec);
				ffly_arc_delrec(__arc, rec);
				rec = tmp;
			}
		}
		p++;
	}
	__ffly_mem_free(__arc->rr);
}

ffly_arc_recp ffly_arc_creatrec(ffly_arcp __arc, ff_u64_t __no,
	void *__p, ff_u8_t __sort, ff_flag_t __flags)
{
	ffly_arc_recp *p = __arc->rr+(__no&0xff); 
	ffly_arc_recp rec = (ffly_arc_recp)__ffly_mem_alloc(sizeof(struct ffly_arc_rec));
	rec->bk = NULL;
	rec->fd = *p;
	if (*p != NULL)
		(*p)->bk = rec;
	
	*p = rec;
	rec->no = __no;
	rec->p = __p;
	rec->sort = __sort;
	rec->flags = __flags;
	return rec;
}

void ffly_arc_delrec(ffly_arcp __arc, ffly_arc_recp __rec) {
	ffly_arc_recp *p = __arc->rr+(__rec->no&0xff);
	if (*p == __rec)
		*p = __rec->fd;
	else {
		__rec->bk->fd = __rec->fd;
		__rec->fd->bk = __rec->bk;
	}

	if (ffly_is_flag(__rec->flags, REC_FLG_TOFREE) && __rec->p != NULL)
		__ffly_mem_free(__rec->p);
	__ffly_mem_free(__rec);
}

ffly_arcp ffly_creatarc(ffly_arcp __arc, ff_u64_t __no) {
	ffly_arc_recp rec = ffly_arc_creatrec(__arc, __no, __ffly_mem_alloc(sizeof(struct ffly_arc)), _ffly_rec_arc, REC_FLG_TOFREE);
	
	ffly_arcp arc = (ffly_arcp)rec->p;
	ffly_arc_prepare(arc);
	arc->bk = __arc;
	arc->p = rec;
	return NULL;
}

void ffly_delarc(ffly_arcp __arc) {
	ffly_arc_free(__arc);
	ffly_arc_delrec(__arc->bk, __arc->p);
}

# include "dep/mem_cpy.h"
void ffly_arc_recw(ffly_arc_recp __rec, void *__buf,
	ff_uint_t __offset, ff_uint_t __n)
{
	ffly_mem_cpy((ff_u8_t*)__rec->p+__offset, __buf, __n);
}

void ffly_arc_recr(ffly_arc_recp __rec, void *__buf,
	ff_uint_t __offset, ff_uint_t __n)
{
	ffly_mem_cpy(__buf, (ff_u8_t*)__rec->p+__offset, __n);
}

# include "stdio.h"
void tree(ffly_arcp __root) {
    ffly_arc_recp *p = __root->rr;
    ffly_arc_recp *end = p+0x100;
    ffly_arc_recp rec;
	ff_uint_t static pad = 0;
    while(p != end) {
        if (*p != NULL) {
            rec = *p;
            while(rec != NULL) {
				ff_u8_t i = 0;
				while(i != pad) {
					ppad('~', pad<<1);
					putchar('+');
					i++;
				}

                if (rec->sort == _ffly_rec_arc) {
                    printf(" %u\n", rec->no);
					pad++;
                    tree((ffly_arcp)rec->p);
					pad--;
                } else {
                    printf(" %u\n", rec->no);
                }
                rec = rec->fd;
            }
        }
        p++;
    }
}
//#define __ffly_debug
# ifdef __ffly_debug
void pr();
# include "stdio.h"
# include "types/err_t.h"
# include "dep/mem_dup.h"
# include "dep/str_len.h"
# include "dep/str_cmp.h"
# include "dep/mem_cpy.h"
# include "dep/str_dup.h"
ff_err_t ffmain(int __argc, char const *__argv) {
	char buf[200];
	ffly_mem_cpy(buf, "Hello", 6);

	printf("%s\n", buf);
	return 0;
	char const *by;

	ffly_arcs_tun("info");
	ffly_arcs_recr("created-by", &by, 0, sizeof(char const*));
	ffly_arcs_bk();
	

	printf("info/created-by: '%s'\n", by);

	tree(&__ffly_arcroot__);
	return 0;
}
# endif
