# include "m.h"
# include "../../memory/mem_alloc.h"
# include "../../memory/mem_free.h"
# include "../../memory/mem_realloc.h"
# include "../../system/io.h"
# include "../../dep/str_cpy.h"
# include "../../dep/mem_set.h"
# include "../../dep/mem_cpy.h"
# include "../../hexdump.h"
#define is_flag(__flags, __flag) \
	(((__flags)&(__flag)) == (__flag))
struct ffly_mfs *mfs;
struct mfs_cauldron mfs_cr = {.n = 0};
static ff_i8_t cr_init = -1;
ff_u32_t static
cr_alloc(ff_uint_t __size) {
	mfs_cr.n+=__size;
	if (cr_init == -1) {
		mfs_cr.s = mfs_alloc(__size);	
		cr_init = 0;
		goto _sk;
	}
	mfs_resize(mfs_cr.s, mfs_cr.n);
_sk:
	return mfs_cr.n-__size;
}

void static commit_node(struct mfs_node*);
struct mfs_node **nodes = NULL;
static ff_u32_t nc = 0;

void loadin_node(struct mfs_node*, struct mfs_engrave*);
struct mfs_node *mfs_getnode(ff_u32_t __n) {
	if ((__n+1)>nc) {
		ff_uint_t prior;
		prior = nc;
		nc = __n+1;

		/*
			TODO use pages
		*/

		if (!nodes)
			nodes = (struct mfs_node**)__ffly_mem_alloc(nc*sizeof(struct mfs_node*));
		else
			nodes = (struct mfs_node**)__ffly_mem_realloc(nodes, nc*sizeof(struct mfs_node*));

		ff_uint_t n;
		n = prior;
		while(n != nc)
			*(nodes+(n++)) = NULL;
	}

	struct mfs_node *n, **np;
	np = nodes+__n;
	if (!(n = *np)) {
		n = (*np = (struct mfs_node*)__ffly_mem_alloc(sizeof(struct mfs_node)));
		ffly_mem_set(n, 0, sizeof(struct mfs_node));
		n->nn = __n;

		/*
			if node has not been loaded yet then say that is should be
		*/
		n->flags = MFS_NNL;

	}

	return n;
}
# define DEBUG
struct mfs_node static* mfs_nget(struct mfs_scope*, ff_u32_t);
struct mfs_scope* mfs_build(ff_u32_t*, ff_uint_t);
void mfs_dmscope(struct mfs_scope*);

void mfs_swrite(struct mfs_scope *__sc, void *__buf,
	ff_uint_t __size, ff_u64_t __offset)
{
	ffly_printf("mds_write{size: %u, offset: %u}\n", __size, __offset);
	ff_uint_t ss, offset;
	ss = (__offset>>MFS_SLAB_SHIFT);
	struct mfs_slab **s, *sb;
	s = __sc->slabs+ss;
	offset = __offset-(ss*MFS_SLAB_SIZE);

	struct mfs_slab **end;
	end = s+(__size>>MFS_SLAB_SHIFT);
	ff_u8_t *p;

	p = (ff_u8_t*)__buf;
	if (offset>0 && __size>=MFS_SLAB_SIZE) {
		ff_uint_t sz;
		mfs->write(p, sz = (MFS_SLAB_SIZE-offset), ((*s)->off*MFS_SLAB_SIZE)+offset);
		s++;
		p+=sz;
		offset = 0;
	}

	if (offset+__size>MFS_SLAB_SIZE && __size<MFS_SLAB_SIZE) {
		ff_uint_t a0, a1;
		a0 = MFS_SLAB_SIZE-offset;
		a1 = __size-a0;
		mfs->write(p, a0, ((*s)->off*MFS_SLAB_SIZE)+offset);
		mfs->write(p+a0, a1, (*(s+1))->off*MFS_SLAB_SIZE);
		return;
	}

	while(s<end) {
		sb = *(s++);
		mfs->write(p, MFS_SLAB_SIZE, sb->off*MFS_SLAB_SIZE);
		p+=MFS_SLAB_SIZE;
	}

	ff_uint_t left;
	left = __size-(p-(ff_u8_t*)__buf);
	if (left>0) {
		mfs->write(p, left, ((*s)->off*MFS_SLAB_SIZE)+offset);
	}
}

void mfs_sread(struct mfs_scope *__sc, void *__buf,
	ff_uint_t __size, ff_u64_t __offset)
{
	ffly_printf("mfs_read{size: %u, offset: %u}\n", __size, __offset);
	ff_uint_t ss, offset;
	ss = (__offset>>MFS_SLAB_SHIFT);
	struct mfs_slab **s, *sb;
	s = __sc->slabs+ss;
	offset = __offset-(ss*MFS_SLAB_SIZE);

	struct mfs_slab **end;
	end = s+(__size>>MFS_SLAB_SHIFT);
	ff_u8_t *p;

//	ffly_printf("%u, %u, %u, %u\n", offset, ss, __offset, __size);
	p = (ff_u8_t*)__buf;
	if (offset>0 && __size>=MFS_SLAB_SIZE) {
		ff_uint_t sz;
		mfs->read(p, sz = (MFS_SLAB_SIZE-offset), ((*s)->off*MFS_SLAB_SIZE)+offset);
		s++;
		p+=sz;
		offset = 0;
	}

	if (offset+__size>MFS_SLAB_SIZE && __size<MFS_SLAB_SIZE) {
		ff_uint_t a0, a1;
		a0 = MFS_SLAB_SIZE-offset;
		a1 = __size-a0;
		mfs->read(p, a0, ((*s)->off*MFS_SLAB_SIZE)+offset);
		mfs->read(p+a0, a1, (*(s+1))->off*MFS_SLAB_SIZE);
		return;
	}

	while(s<end) {
		sb = *(s++);
		mfs->read(p, MFS_SLAB_SIZE, sb->off*MFS_SLAB_SIZE);
		p+=MFS_SLAB_SIZE;
	}

	ff_uint_t left;
	left = __size-(p-(ff_u8_t*)__buf);
	if (left>0) {
		mfs->read(p, left, ((*s)->off*MFS_SLAB_SIZE)+offset);
	}
}

struct mfs_scope* mfs_alloc(ff_uint_t);
void mfs_free(struct mfs_scope*);

struct mfs_node* mfs_node_new(ff_u32_t __nn) {
	struct mfs_node *n;
	n = (struct mfs_node*)mfs_getnode(__nn);
	n->flags |= MFS_NNA|MFS_NDNA;
	n->cbs = NULL;
	n->cbc = 0;
	ffly_mem_set(n->name, 0, sizeof(n->name));
	return n;
}

void static
unload_node(struct mfs_node *__n) {
	if (is_flag(__n->flags, MFS_PCP)) {
		ff_u8_t *buf, *d;
		buf = (ff_u8_t*)__ffly_mem_alloc(__n->pcs);
		d = buf;
		struct mfs_cb_struc *_cb, *_b;
		_cb = (struct mfs_cb_struc*)__ffly_mem_alloc(__n->cbc*sizeof(struct mfs_cb_struc));
		struct mfs_cb **cb, *b;
		cb = __n->cbs;
		ff_uint_t i;
		i = 0;
		for(;i != __n->cbc;i++,cb++) {
			_b = _cb+i;
			b = *cb;
			_b->off = b->off;
			_b->size = b->size;
			ffly_mem_cpy(d, b->p, b->size);	
			d+=b->size;
		}

		__n->_cbs = mfs_valloc(__n->cbc*sizeof(struct mfs_cb_struc))<<MFS_SLAB_SHIFT;
		__n->pc = mfs_valloc(__n->pcs)<<MFS_SLAB_SHIFT;
		ffly_printf("unloadcode: %u, %u\n", __n->pcs, __n->pc);
		ffly_hexdump(buf, __n->pcs);
		mfs->write(buf, __n->pcs, __n->pc);
		mfs->write(_cb, __n->cbc*sizeof(struct mfs_cb_struc), __n->_cbs);
		__ffly_mem_free(buf);
		commit_node(__n);
	}

	*(nodes+__n->nn) = NULL;
	__ffly_mem_free(__n);
}

ff_u16_t static
mfs_cbinsert(struct mfs_node *__n, ff_u8_t *__code, ff_uint_t __nn) {
	__n->cbc++;
	if (!__n->cbs) {
		__n->cbs = (struct mfs_cb**)__ffly_mem_alloc(__n->cbc*sizeof(struct mfs_cb*));
	} else {
		__n->cbs = (struct mfs_cb**)__ffly_mem_realloc(__n->cbs, __n->cbc*sizeof(struct mfs_cb*));
	}

	struct mfs_cb **cbp, *b;
	cbp = __n->cbs+(__n->cbc-1);
	b = *cbp = (struct mfs_cb*)__ffly_mem_alloc(sizeof(struct mfs_cb));
	b->p = __ffly_mem_alloc(__nn);
	ffly_mem_cpy(b->p, __code, __nn);
	b->size = __nn;
	ffly_printf("new code block-%u; size: %u\n", __n->cbc-1, __nn);
	b->off = __n->pcs;
	__n->pcs+=__nn;
	return __n->cbc-1;
}

void static mfs_node_destroy(struct mfs_node *__n) {
	__ffly_mem_free(__n);
}

struct mfs_node* mfs_lookup(char const *__path) {
	char const *p;
	char buf[128];
	char *bufp;
	ff_u32_t _n;
	struct mfs_node *n;
	_n = (n = mfs->root)->n;
	p = __path;
	ff_i8_t fnd;
_again:
	bufp = buf;
	p++;
	while(*p != '/' && *p != '\0')
		*(bufp++) = *(p++);
	*bufp = '\0';
	_n = mfs_hash_get(n->h, buf, bufp-buf, &fnd);
	n = mfs_getnode(_n);

	if (fnd == -1) {
		ffly_printf("dead end at %s\n", buf);
		return NULL;
	}
	if (*p == '\0')
		goto _end;
	goto _again;
_end:
	return n;
}

void static __mfs_write(struct mfs_node *__n, void *__buf, ff_uint_t __size, ff_u64_t __offset) {
	if (__offset+__size >= __n->size) {
		ff_u32_t apex;

		apex = __offset+__size;
		if (is_flag(__n->flags, MFS_NDNA)) {
			__n->s = mfs_alloc(apex);
			__n->flags ^= MFS_NDNA;
			goto _sk;		
		}

		mfs_resize(__n->s, apex);
	}
_sk:
	mfs_swrite(__n->s, __buf, __size, __offset);
}

void static __mfs_read(struct mfs_node *__n, void *__buf, ff_uint_t __size, ff_u64_t __offset) {
	mfs_sread(__n->s, __buf, __size, __offset);
}


void static mfs_write(ff_u32_t __f, void *__buf, ff_uint_t __size, ff_u64_t __offset) {
	__mfs_write(mfs_file_get(__f)->n, __buf, __size, __offset);
}

void static mfs_read(ff_u32_t __f, void *__buf, ff_uint_t __size, ff_u64_t __offset) {
	__mfs_read(mfs_file_get(__f)->n, __buf, __size, __offset);
}

// 0 = root
ff_u32_t static nn = 1;
void static
mfs_new(struct mfs_node *__dir, struct mfs_dent *__de) {
	ff_i8_t fnd;
	struct mfs_node *n;
	ff_u32_t _n;
	_n = mfs_hash_get(__dir->h, __de->name, __de->nlen, &fnd);
	if (fnd == -1) {
		mfs_hash_put(__dir->h, __de->name, __de->nlen, nn++);
		n = mfs_node_new(nn-1);
	} else {
		ffly_printf("'%s' already exists.\n", __de->name);
		return;
	}
 
	ffly_mem_cpy(n->name, __de->name, __de->nlen);
	__de->n = n;
}

void static
insert_node(struct mfs_node *__dir, struct mfs_node *__n) {
	ff_uint_t siz;
	__dir->n+=sizeof(ff_u32_t);

	siz = __dir->n;
	if (is_flag(__dir->flags, MFS_NDNA)) {
		__dir->s = mfs_alloc(siz);
		__dir->flags ^= MFS_NDNA;
		goto _sk;		
	}

	mfs_resize(__dir->s, siz);
_sk:
	{
		ff_u8_t code[18];
		*code = 0x00;
		*(ff_u32_t*)(code+1) = __n->nn;
		*(ff_u32_t*)(code+5) = __n->ca;
		*(code+9) = 0x01;
		*(ff_u32_t*)(code+10) = __dir->nn;
		*(ff_u32_t*)(code+14) = __n->nn;

		mfs_cbinsert(__dir, code, 18);
		__dir->flags |= MFS_PCP;
	}
	ffly_printf("insert_node, %u, %u, %u, %u\n", siz-sizeof(ff_u32_t), __dir->s->slab_c, __dir->s->slab_c*MFS_SLAB_SIZE, __n->ca);
	mfs_swrite(__dir->s, &__n->ca, sizeof(ff_u32_t), siz-sizeof(ff_u32_t));
	commit_node(__dir);
	return;
}

ff_u32_t mfs_balloc(void);
void
loadin_node(struct mfs_node *__n, struct mfs_engrave *__eg) {	
	__n->n = __eg->n;
	__n->h = __eg->h;
	__n->cbc = __eg->cbc;
	__n->_cbs = __eg->cbs;
	__n->mode = __eg->mode;
	__n->flags = __eg->flags;
	__n->pc = __eg->pc;
	__n->pcs = __eg->pcs;
	ffly_mem_cpy(__n->name, __eg->name, sizeof(__eg->name));
	if (!is_flag(__eg->flags, MFS_NDNA)) {
		ff_uint_t slab_c;
		slab_c = (__eg->n+(MFS_SLAB_SIZE-1))>>MFS_SLAB_SHIFT;
		ff_u32_t *slabs;
		slabs = (ff_u32_t*)__ffly_mem_alloc((__eg->bale_c*MFS_BALE_SIZE)*sizeof(ff_u32_t));
		ffly_mem_set(slabs, 0xff, (__eg->bale_c*MFS_BALE_SIZE)*sizeof(ff_u32_t));
		ff_uint_t i, ii;
		ff_u32_t bs;
		bs = __eg->start;
#ifdef DEBUG
		ffly_printf("bale_count: %u, start: %u\n", __eg->bale_c, __eg->start);
#endif
		struct mfs_bale b;
		i = 0;
		for(;i != __eg->bale_c;i++) {
#ifdef DEBUG
			ffly_printf("load_bale: %u\n", bs*MFS_SLAB_SIZE);
#endif
			if (bs == MFS_BALE_NULL) {
				ffly_printf("error.\n");
				break;
			}
				
			mfs->read(&b, sizeof(struct mfs_bale), bs*MFS_SLAB_SIZE);
			ii = 0;
			for(;ii != MFS_BALE_SIZE;ii++) {
				if ((i*MFS_BALE_SIZE)+ii >= slab_c)
					break;
				slabs[((__eg->bale_c*MFS_BALE_SIZE)-((i+1)*MFS_BALE_SIZE))+ii] = b.slabs[ii];
#ifdef DEBUG
				ffly_printf("load_slab: %u\n", b.slabs[ii]);
#endif
			}

			bs = b.next;
		}
#ifdef DEBUG
		ffly_hexdump(slabs, (__eg->bale_c*MFS_BALE_SIZE)*sizeof(ff_u32_t));
#endif
		__n->s = mfs_build(slabs, slab_c);
	}
	//run

	if (is_flag(__eg->flags, MFS_PCP)) {
		ff_u8_t *buf;
		buf = (ff_u8_t*)__ffly_mem_alloc(__eg->pcs);
		ffly_printf("PC %u, %u\n", __eg->pcs, __eg->pc);
		mfs->read(buf, __eg->pcs, __eg->pc);
		mfs_exec(buf, __eg->pcs);
	
		struct mfs_cb *b;
		struct mfs_cb_struc *cbs, *_b;
		cbs = (struct mfs_cb_struc*)__ffly_mem_alloc(__n->cbc*sizeof(struct mfs_cb_struc));
		mfs->read(cbs, __n->cbc*sizeof(struct mfs_cb_struc), __eg->cbs);
		__n->cbs = (struct mfs_cb**)__ffly_mem_alloc(__n->cbc*sizeof(struct mfs_cb*));
		ff_uint_t i;
		i = 0;
		for(;i != __n->cbc;i++) {
			_b = cbs+i;
			b = *(__n->cbs+i) = __ffly_mem_alloc(sizeof(struct mfs_cb));
			b->off = _b->off;
			b->size = _b->size;
			b->p = __ffly_mem_alloc(_b->size);
			ffly_mem_cpy(b->p, buf+_b->off, _b->size);
		}
		__ffly_mem_free(buf);
	}
}

void
map_node(struct mfs_node *__n) {
	if (is_flag(__n->flags, MFS_NNA)) {
		__n->ca = cr_alloc(sizeof(struct mfs_engrave));
		__n->flags ^= MFS_NNA;
	}
}

void
commit_node(struct mfs_node *__n) {
	struct mfs_engrave eg;
	eg.nn = __n->nn;
	eg.n = __n->n;
	eg.h = __n->h;
	eg.pc = __n->pc;
	eg.pcs = __n->pcs;
	eg.cbc = __n->cbc;
	eg.cbs = __n->_cbs;
	eg.mode = __n->mode;
	ffly_printf("commit_node; n: %u\n", eg.n);
	ffly_mem_cpy(eg.name, __n->name, sizeof(__n->name));
	if (!is_flag(__n->flags, MFS_NDNA)) {
		ff_uint_t bale_c;
		struct mfs_scope *s;
		s = __n->s;

		bale_c = (s->slab_c+(MFS_BALE_SIZE-1))>>MFS_BALE_SHIFT;
		ff_u32_t bs, bk;
		bk = MFS_BALE_NULL;
		struct mfs_bale b;
		ff_uint_t i, ii;
		i = 0;
		
		for(;i != bale_c;i++) {
			// bale space
			bs = mfs_balloc();	
#ifdef DEBUG
			ffly_printf("store_bale: %u\n", bs);
#endif
			ii = 0;
			ffly_mem_set(b.slabs, 255, MFS_BALE_SIZE*sizeof(ff_u32_t));
			for(;ii != MFS_BALE_SIZE;ii++) {
				if ((i*MFS_BALE_SIZE)+ii >= s->slab_c)
					break;

				b.slabs[ii] = (*(s->slabs+(i*MFS_BALE_SIZE)+ii))->in;
#ifdef DEBUG
				ffly_printf("store_slab: %u\n", b.slabs[ii]);
#endif
			}
#ifdef DEBUG
			ffly_hexdump(b.slabs, MFS_BALE_SIZE*sizeof(ff_u32_t));
#endif
			b.next = bk;
			bk = bs;
			mfs->write(&b, sizeof(struct mfs_bale), bs*MFS_SLAB_SIZE);
		}

		eg.start = bs;
		eg.bale_c = bale_c;
	}

	eg.flags = __n->flags;

	ffly_printf("node ca: %u\n", __n->ca);
	cr_write(&eg, sizeof(struct mfs_engrave), __n->ca);
}

void static
mfs_creat(struct mfs_node *__dir, struct mfs_dent *__de) {
	mfs_new(__dir, __de);
	struct mfs_node *n;
	n = __de->n;
	n->mode = MFS_FREG;
	n->n = 0;
	map_node(n);
	commit_node(n);
	insert_node(__dir, n);
}

void static
__mfs_mkdir(struct mfs_node *__dir, struct mfs_dent *__de) {
	mfs_new(__dir, __de);
	struct mfs_node *n;
	n = __de->n;
	n->h = mfs_hash_new();
	n->mode = MFS_FDIR;
	n->n = 0;
	map_node(n);
	commit_node(n);
	insert_node(__dir, n);
}

void static
mfs_mkdir(char const *__path) {
	struct mfs_node *d, *n;
	ff_i8_t fnd;
	ff_u32_t _n;
	char dir[1024];
	char buf[128];
	char *bp;
	char *dp = dir;
	char *pp = __path;
_again:
		pp++;
		bp = buf;
		while(*pp != '/' && *pp != '\0') {
			*(bp++) = *(pp++);
		}

		if (*pp == '\0') {
			*dp = '\0';	
			*bp = '\0';
			goto _esc;
		}

		*dp = '/';
		ffly_mem_cpy(dp+1, buf, bp-buf);
		dp+=(bp-buf)+1;
	goto _again;
_esc:
	ffly_printf("dir: %s\n", dir);

	d = dir == dp?mfs->root:mfs_lookup(dir);

	if (!d) {
		// error dir not found
		return NULL;
	}

	_n = mfs_hash_get(d->h, buf, bp-buf, &fnd);
	ffly_printf("---------> node: %u\n", _n);
	n = mfs_getnode(_n);

	if (fnd == -1) {
		struct mfs_dent de;
		de.name = buf;
		de.nlen = bp-buf;
		__mfs_mkdir(d, &de);	
	}
}

struct mfs_fo file_op = {
	.write = mfs_write,
	.read = mfs_read
};

ff_u32_t mfs_open(char const *__path, ff_u8_t __flags) {
	struct mfs_file *f;
	f = mfs_file_new();
	f->op = &file_op;
	struct mfs_node *n, *fn;
	ff_u32_t _n;
	ff_i8_t fnd;
	char dir[1024];
	char buf[128];
	char *bp;
	char *dp = dir;
	char *pp = __path;
_again:
		pp++;
		bp = buf;
		while(*pp != '/' && *pp != '\0') {
			*(bp++) = *(pp++);
		}

		if (*pp == '\0') {
			*dp = '\0';	
			*bp = '\0';
			goto _esc;
		}

		*dp = '/';
		ffly_mem_cpy(dp+1, buf, bp-buf);
		dp+=(bp-buf)+1;
	goto _again;
_esc:
	ffly_printf("dir: %s\n", dir);

	n = dir == dp?mfs->root:mfs_lookup(dir);

	if (!n) {
		// error dir not found
		return NULL;
	}

	_n = mfs_hash_get(n->h, buf, bp-buf, &fnd);
	fn = mfs_getnode(_n);

	if (fnd == -1 && is_flag(__flags, MFS_CREAT)) {
		struct mfs_dent de;
		de.name = buf;
		de.nlen = bp-buf;
		mfs_creat(n, &de);	
		f->n = de.n;
	} else if (!fnd) {	
		f->n = fn;
	} else 
		return 0;
	return f->i;
}

struct mfs_node* mfs_nget(struct mfs_scope *__s, ff_u32_t __n) {
	struct mfs_node *n;

	ff_u32_t ca;
	ff_u32_t offset;
	struct mfs_engrave eg;
	offset = __n*sizeof(ff_u32_t);

	mfs_sread(__s, &ca, sizeof(ff_u32_t), offset);
	cr_read(&eg, sizeof(struct mfs_engrave), ca);

	n = mfs_node_new(eg.nn);
	if (is_flag(n->flags, MFS_NNL)) {
		n->ca = ca;
#ifdef DEBUG
		ffly_printf("nget{ca: %u, offset: %u}\n", ca, offset);
#endif
		loadin_node(n, &eg);
		n->flags ^= MFS_NNL;
	}
	return n;
}

void static mfs_close(ff_u32_t __f) {


}

void mfs_slabs_save(ff_uint_t*, ff_u32_t);
void mfs_slabs_load(ff_uint_t, ff_u32_t);
void ffly_mfs_init(void) {
	struct mfs_tract t;
	mfs->read(&t, sizeof(struct mfs_tract), 0);
	mfs->off = t.off;
	ffly_printf("mfs_init{cr: %u, crnc: %u, root: %u, crsc: %u, sc: %u, off: %u-slabs}\n", t.cr, t.crnc, t.root, t.crsc, t.sc, t.off);
	struct mfs_node *root;
	root = mfs_node_new(0);

	mfs->root = root;
	root->h = mfs_hash_new();
	root->ntf |= MFS_HHT;
	if (t.sc>0) {
		mfs_slabs_load(t.sc, t.slabs);
	}
	
	if (t.crsc>0 && t.crnc>0) {
		cr_init = 0;
		ff_u32_t *sr_slabs;
		sr_slabs = (ff_u32_t*)__ffly_mem_alloc(t.crsc*sizeof(ff_u32_t));

		mfs->read(sr_slabs, t.crsc*sizeof(ff_u32_t), t.cr);	
		ff_uint_t i;
		for(i = 0;i != t.crsc;i++)
			ffly_printf("crsb: %u\n", sr_slabs[i]);
		mfs_cr.s = mfs_build(sr_slabs, t.crsc);
		mfs_cr.n = t.crnc;
		
		struct mfs_engrave eg;
		cr_read(&eg, sizeof(struct mfs_engrave), t.root);

		ffly_printf("root: h: %u, pc: %u, pcs: %u, mode: %u, flags: %u, nn: %u, n: %u, start: %u, bale_c: %u\n",
			eg.h, eg.pc, eg.pcs, eg.mode, eg.flags, eg.nn, eg.n, eg.start, eg.bale_c);
		loadin_node(root, &eg);
		__ffly_mem_free(sr_slabs);
		return;
	}

	map_node(root);
	commit_node(root);
}

void ffly_mfs_de_init(void) {
	struct mfs_tract t;
	ff_uint_t i;

	t.root = mfs->root->ca;
	struct mfs_node *n;
	i = 0;
	while(i != nc) {
		n = mfs_getnode(i++);
		if (n != NULL)
			unload_node(n);
	}

	t.cr = mfs->off<<MFS_SLAB_SHIFT;
	t.crnc = mfs_cr.n;
	t.off = mfs->off;
	ff_u32_t *cr_slabs;
	ff_uint_t sc;

	sc = mfs_cr.s->slab_c;

	t.slabs = t.cr+(sc*sizeof(ff_u32_t));
	t.crsc = sc;

	ffly_printf("mfs_de_init{cr: %u, crnc: %u, root: %u, crsc: %u}\n", t.cr, t.crnc, t.root, t.crsc);
	cr_slabs = (ff_u32_t*)__ffly_mem_alloc(sc*sizeof(ff_u32_t));

	i = 0;
	for(;i != sc;i++) {
		cr_slabs[i] = (*(mfs_cr.s->slabs+i))->in;
		ffly_printf("crsb: %u\n", cr_slabs[i]);
	}

	mfs->write(cr_slabs, sc*sizeof(ff_u32_t), t.cr);
	__ffly_mem_free(cr_slabs);

	mfs_slabs_save(&t.sc, t.slabs);
	mfs->write(&t, sizeof(struct mfs_tract), 0);
}

#define PAD (pad+PADSP)
#define PADSP 2
void
mfs_tree(struct mfs_node *__n) {
	char static pad[64];
	char static *pd = pad;
	ff_uint_t i;

	i = 0;
	for(;i != PADSP;i++)
		*(pd++) = ' ';

	*pd = '\0';
	ff_uint_t n;
	n = __n->n/sizeof(ff_u32_t);

	ffly_printf("%u or %u\n", n, __n->n);
	struct mfs_node *nn;
	i = 0;
	for(;i != n;i++) {
		nn = mfs_nget(__n->s, i);
		if (nn != NULL) {
		if (nn->mode == MFS_FDIR) {
#ifdef DEBUG
			ffly_printf("-----> N: %u, H: %u, SZ: %u, CA: %u\n", nn->n, nn->h, nn->size, nn->ca);
#endif
			ffly_printf("%s\e[34m%s\e[0m\n", PAD, nn->name);
			mfs_tree(nn);
		} else {
			ffly_printf("%s.%s\n", PAD, nn->name);
		}
		}
	}

	pd-=PADSP;
	*pd = '\0';
}

# include "../../fs.h"

static struct ff_fso mfs_op = {
	.open = mfs_open,
	.close = mfs_close,
	.write = mfs_write,
	.read = mfs_read,
	.mkdir = mfs_mkdir
};

void ffly_mfs(void) {
	ffly_mfs_init();
	__ff_fso__ = &mfs_op;
}
