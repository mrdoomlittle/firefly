# include "arcs.h"
# include "ffly_def.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
struct ffly_arc __ffly_arcroot__;
ffly_arcp __ffly_arccur__ = &__ffly_arcroot__;

ffly_arc_recp ffly_arc_lookup(ffly_arcp __arc, mdl_u64_t __no) {
	ffly_arc_recp cur = *(__arc->rr+(__no&0xff));
	while(cur != NULL) {
		if (cur->no == __no) return cur;
		cur = cur->fd;
	}
	return NULL;
}

void ffly_arc_prepare(ffly_arcp __arc) {
	__arc->rr = (ffly_arc_recp*)__ffly_mem_alloc(0xff*sizeof(ffly_arc_recp));
	ffly_arc_recp *p = __arc->rr;
	ffly_arc_recp *end = p+0xff;
	while(p != end)
		*(p++) = NULL;
}

void ffly_arc_free(ffly_arcp __arc) {
	ffly_arc_recp *p = __arc->rr;
	ffly_arc_recp *end = p+0xff;
	ffly_arc_recp rec, tmp;
	while(p != end) {
		if (*p != NULL) {
			rec = *p;
			while(rec != NULL) {
				tmp = rec->fd;
				if (rec->sort == _ffly_rec_arc)
					ffly_arc_free((ffly_arcp)rec->p);
				__ffly_mem_free(rec);
				rec = tmp;
			}
		}
		p++;
	}
}

ffly_arc_recp ffly_arc_creatrec(ffly_arcp __arc, mdl_u64_t __no,
	void *__p, mdl_u8_t __sort, ffly_flag_t __flags)
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
}

ffly_arcp ffly_creatarc(ffly_arcp __arc, mdl_u64_t __no) {
	ffly_arc_recp rec = ffly_arc_creatrec(__arc, __no, __ffly_mem_alloc(sizeof(struct ffly_arc)), _ffly_rec_arc, REC_FLG_TOFREE);
	
	ffly_arcp arc = (ffly_arcp)rec->p;
	ffly_arc_prepare(arc);
	arc->bk = __arc;
	arc->p = rec;
	return arc;
}

void ffly_delarc(ffly_arcp __arc) {
	ffly_arc_free(__arc);
	ffly_arc_delrec(__arc->bk, __arc->p);
}

# include "data/mem_cpy.h"
void ffly_arc_recw(ffly_arc_recp __rec, void *__buf,
	mdl_uint_t __offset, mdl_uint_t __n)
{
	ffly_mem_cpy((mdl_u8_t*)__rec->p+__offset, __buf, __n);
}

void ffly_arc_recr(ffly_arc_recp __rec, void *__buf,
	mdl_uint_t __offset, mdl_uint_t __n)
{
	ffly_mem_cpy(__buf, (mdl_u8_t*)__rec->p+__offset, __n);
}
# include "stdio.h"
void tree(ffly_arcp __root) {
    ffly_arc_recp *p = __root->rr;
    ffly_arc_recp *end = p+0xff;
    ffly_arc_recp rec;
    while(p != end) {
        if (*p != NULL) {
            rec = *p;
            while(rec != NULL) { 
                if (rec->sort == _ffly_rec_arc) {
                    printf("--> arc: recno{%u}\n", rec->no);
                    tree((ffly_arcp)rec->p);
                } else {
                    printf("--> recno{%u}\n", rec->no);
                }
                rec = rec->fd;
            }
        }
        p++;
    }
}

# include "types/err_t.h"
ffly_err_t ffmain(int __argc, char const *__argv) {
	ffly_arc_prepare(__ffly_arccur__);
	ffly_arcp arc = ffly_creatarc(__ffly_arccur__, 0);

	ffly_arc_recp r0 = ffly_arc_creatrec(arc, 0, NULL, _ffly_rec_def, 0);
	tree(&__ffly_arcroot__);	

	ffly_arc_delrec(arc, r0);
	ffly_arc_free(__ffly_arccur__);
}

