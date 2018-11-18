# include "m.h"
# include "../../memory/mem_alloc.h"
# include "../../memory/mem_free.h"
# include "../../system/io.h"
# include "../../dep/str_cpy.h"
# include "../../dep/mem_set.h"
# include "../../dep/mem_cpy.h"
#define is_flag(__flags, __flag) \
	(((__flags)&(__flag)) == (__flag))
struct ffly_mfs *mfs;
static struct mfs_cauldron cr = {.n = 0};
static ff_i8_t cr_init = -1;
ff_u32_t static
cr_alloc(ff_uint_t __size) {
	cr.n+=__size;
	if (cr_init == -1) {
		cr.s = mfs_alloc(__size);	
		cr_init = 0;
		goto _sk;
	}
	mfs_resize(cr.s, cr.n);
_sk:
	return cr.n-__size;
}

struct mfs_scope* mfs_build(ff_u32_t*, ff_uint_t);
void mfs_dmscope(struct mfs_scope*);
#define cr_write(__buf, __size, __dst)\
	mfs_swrite(cr.s, __buf, __size, __dst)
#define cr_read(__buf, __size, __src)\
	mfs_sread(cr.s, __buf, __size, __src)

void mfs_swrite(struct mfs_scope *__sc, void *__buf,
	ff_uint_t __size, ff_u64_t __offset)
{
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



struct mfs_node static* mfs_node_new(void) {
	struct mfs_node *n;
	n = (struct mfs_node*)__ffly_mem_alloc(sizeof(struct mfs_node));
	ffly_mem_set(n, 0, sizeof(struct mfs_node));
	n->flags = MFS_NNA|MFS_NDNA;
	ffly_mem_set(n->name, 0, sizeof(n->name));
	return n;
}

void static mfs_node_destroy(struct mfs_node *__n) {
	__ffly_mem_free(__n);
}

struct mfs_node static* mfs_lookup(char const *__path) {
	char const *p;
	char buf[128];
	char *bufp;
	struct mfs_node *n;
	n = mfs->root;
	p = __path;
_again:
	bufp = buf;
	p++;
	while(*p != '/' && *p != '\0')
		*(bufp++) = *(p++);
	*bufp = '\0';
	n = (struct mfs_node*)mfs_hash_get(n->h, buf, bufp-buf);
	if (!n) {
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

void static
mfs_new(struct mfs_node *__dir, struct mfs_dent *__de) {
	struct mfs_node *n;
	n = (struct mfs_node*)mfs_hash_get(__dir->h, __de->name, __de->nlen);
	if (!n) {
		mfs_hash_put(__dir->h, __de->name, __de->nlen, n = mfs_node_new());
	}

	ffly_mem_cpy(n->name, __de->name, __de->nlen);
	__de->n = n;
}

void static commit_node(struct mfs_node*);

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
	ffly_printf("insert_node, %u, %u, %u, %u\n", siz-sizeof(ff_u32_t), __dir->s->slab_c, __dir->s->slab_c*MFS_SLAB_SIZE, __n->ca);
	mfs_swrite(__dir->s, &__n->ca, sizeof(ff_u32_t), siz-sizeof(ff_u32_t));
	commit_node(__dir);
	return;
}
# include "../../hexdump.h"
ff_u32_t mfs_balloc(void);
void
loadin_node(struct mfs_node *__n) {
	struct mfs_engrave eg;
	cr_read(&eg, sizeof(struct mfs_engrave), __n->ca);
	__n->n = eg.n;
	__n->h = eg.h;
	__n->mode = eg.mode;
	__n->flags = eg.flags;
	ffly_mem_cpy(__n->name, eg.name, sizeof(eg.name));
	if (!is_flag(eg.flags, MFS_NDNA)) {
		ff_uint_t slab_c;
		slab_c = (eg.n+(MFS_SLAB_SIZE-1))>>MFS_SLAB_SHIFT;
		ff_u32_t *slabs;
		slabs = (ff_u32_t*)__ffly_mem_alloc((eg.bale_c*MFS_BALE_SIZE)*sizeof(ff_u32_t));
		ffly_mem_set(slabs, 255, (eg.bale_c*MFS_BALE_SIZE)*sizeof(ff_u32_t));
		ff_uint_t i, ii;
		ff_u32_t bs;
		bs = eg.start;
#ifdef DEBUG
		ffly_printf("BLK_C: %u\n", eg.bale_c);
#endif
		struct mfs_bale b;
		i = 0;
		for(;i != eg.bale_c;i++) {
#ifdef DEBUG
			ffly_printf("LD-BS: %u\n", bs);
#endif
			if (bs == MFS_BALE_NULL) {
				ffly_printf("error.\n");
			}
				
			mfs->read(&b, sizeof(struct mfs_bale), bs*MFS_SLAB_SIZE);
			ii = 0;
			for(;ii != MFS_BALE_SIZE;ii++) {	
				slabs[((eg.bale_c*MFS_BALE_SIZE)-((i+1)*MFS_BALE_SIZE))+ii] = b.slabs[ii];
#ifdef DEBUG
				ffly_printf("LD-SLB: %u\n", b.slabs[ii]);
#endif
			}

			bs = b.next;
		}
#ifdef DEBUG
		ffly_hexdump(slabs, (eg.bale_c*MFS_BALE_SIZE)*sizeof(ff_u32_t));
#endif
		__n->s = mfs_build(slabs, slab_c);
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
	eg.n = __n->n;
	eg.h = __n->h;
	eg.mode = __n->mode;
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
			// block space
			bs = mfs_balloc();	
#ifdef DEBUG
			ffly_printf("ST-BS: %u\n", bs);
#endif
			ii = 0;
			ffly_mem_set(b.slabs, 255, MFS_BALE_SIZE*sizeof(ff_u32_t));
			for(;ii != MFS_BALE_SIZE;ii++) {
				if ((i*MFS_BALE_SIZE)+ii >= s->slab_c)
					break;

				b.slabs[ii] = (*(s->slabs+(i*MFS_BALE_SIZE)+ii))->in;
#ifdef DEBUG
				ffly_printf("ST-SLB: %u\n", b.slabs[ii]);
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

	ffly_printf("CA: %u\n", __n->ca);
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

	n = (struct mfs_node*)mfs_hash_get(d->h, buf, bp-buf);

	if (!n) {
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
	struct mfs_node *n;

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

	struct node *fn;
	fn = (struct mfs_node*)mfs_hash_get(n->h, buf, bp-buf);

	if (!fn && is_flag(__flags, MFS_CREAT)) {
		struct mfs_dent de;
		de.name = buf;
		de.nlen = bp-buf;
		mfs_creat(n, &de);	
		f->n = de.n;
	} else if (fn != NULL) {	
		f->n = fn;
	} else 
		return 0;
	return f->i;
}

struct mfs_node static* mfs_nget(struct mfs_scope *__s, ff_u32_t __n) {
	struct mfs_node *n;
	n = mfs_node_new();
	ff_u32_t ca;
	ff_u32_t offset;
	struct mfs_engrave eg;
	offset = __n*sizeof(ff_u32_t);

	mfs_sread(__s, &ca, sizeof(ff_u32_t), offset);
	n->ca = ca;
#ifdef DEBUG
	ffly_printf("CA:+ %u, %u\n", ca, offset);
#endif
	loadin_node(n);
	return n;
}

void static mfs_close(ff_u32_t __f) {


}

void ffly_mfs_init(void) {
	mfs->off+=sizeof(struct mfs_tract);
	struct mfs_tract t;
	mfs->read(&t, sizeof(struct mfs_tract), 0);
	struct mfs_node *root;
	root = mfs_node_new();

	mfs->root = root;
	root->h = mfs_hash_new();
	if (t.crnc>0) {
		cr_init = 0;
		ff_u32_t *sr_slabs;
		sr_slabs = (ff_u32_t*)__ffly_mem_alloc(t.crnc*sizeof(ff_u32_t));

		mfs->read(sr_slabs, t.crnc*sizeof(ff_u32_t), t.cr);	
		cr.s = mfs_build(sr_slabs, t.crnc);

		root->ca = t.root;
		loadin_node(root);
		__ffly_mem_free(sr_slabs);
		return;
	}

	map_node(root);
}

void ffly_mfs_de_init(void) {
	struct mfs_tract t;
	t.cr = mfs->off;
	t.crnc = cr.n;
	t.root = mfs->root->ca;
	ff_u32_t *cr_slabs;
	ff_uint_t sc;

	sc = cr.s->slab_c;

	cr_slabs = (ff_u32_t*)__ffly_mem_alloc(sc);
	ff_uint_t i;

	for(;i != sc;i++)
		cr_slabs[i] = (*(cr.s->slabs+i))->in;
	
	__ffly_mem_free(cr_slabs);
	mfs->write(cr_slabs, sc*sizeof(ff_u32_t), t.cr);
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

	struct mfs_node *nn;
	i = 0;
	for(;i != n;i++) {
		nn = mfs_nget(__n->s, i);
		if (nn->mode == MFS_FDIR) {
#ifdef DEBUG
			ffly_printf("-----> %u\n", nn->n);
#endif
			ffly_printf("%s\e[34m%s\e[0m\n", PAD, nn->name);
			mfs_tree(nn);
		} else {
			ffly_printf("%s.%s\n", PAD, nn->name);
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
