# include "db.h"
# include "system/io.h"
# include "dep/str_len.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# include "system/err.h"
# include "crypto.h"
# include "dep/mem_cpy.h"
# define msgsize sizeof(struct ff_db_msg)
# define FFDB_NNO 0xffff
# include "db/block.h"
/*
	TODO:
	 make thread safe
*/


/*
	not tested at all.
*/

// rivet table
static ffdb_rivetp rt[64] = {
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

ff_u16_t ffdb_ndiched[100];
ff_u16_t *ffdb_nnext = ffdb_ndiched;
ff_u16_t ffdb_nfresh = 0;

ff_u16_t
ffdb_obtain_no() {
	if (ffdb_nnext>ffdb_ndiched)
		return *(--ffdb_nnext);
	return ffdb_nfresh++;
}

void
ffdb_ditch_no(ff_u16_t __no) {
	if (ffdb_nfresh-1 == __no) {
		ffdb_nfresh--;
		return;
	}
	*(ffdb_nnext++) = __no;
}

ffdb_rivetp static
lookup(ff_u16_t __no) {
	ffdb_rivetp cur = rt[__no&0x3f];
	while(cur != NULL) {
		if (cur->no == __no) return cur;
		cur = cur->next;
	}
	return NULL;
}

ff_i8_t ffdb_exist(ff_u16_t __no) {
	return !lookup(__no)?-1:0;
}

void* ffdb_rivetto(ff_u16_t __no) {
	ffdb_rivetp rivet;
	if (!(rivet = lookup(__no)))
		return NULL;
	return rivet->to;
}

static struct ffdb_rivet *rtop = NULL;
void ffdb_rivet(ff_u16_t __no, void *__to) {
	ffdb_rivetp *p = rt+(__no&0x3f);
	ffdb_rivetp r = (ffdb_rivetp)__ffly_mem_alloc(sizeof(struct ffdb_rivet));
	r->next = *p;
	r->no = __no;
	r->to = __to;
	r->fd = rtop;
	r->bk = &rtop;
	if (rtop != NULL)
		rtop->bk = &r->fd;
	rtop = r;
	*p = r;
}

void static
delink_rivet(ffdb_rivetp __r) {
	*__r->bk = __r->fd;
	if (__r->fd != NULL)
		__r->fd->bk = __r->bk;
}

void ffdb_derivet(ff_u16_t __no) {
	ffdb_rivetp *p = rt+(__no&0x3f);
	ffdb_rivetp *i = p;
	ffdb_rivetp cur = *p;
	while(cur != NULL) {
		if (cur->no == __no) {
			*i = cur->next;
			delink_rivet(cur);
			__ffly_mem_free(cur);
			return;
		}
		i = &cur->next;
	}
}

ff_err_t ffdb_init(ffdbp __db) {
	__db->off = ffdb_hdrsize;
	__db->top = NULL;
	__db->next = __db->free;

	ff_err_t err;
	if (_err(err = ffly_map_init(&__db->map, _ffly_map_127))) { 
		ffly_printf("failed to init map.\n");
		_ret;
	}
	retok;
}

ff_err_t ffdb_open(ffdbp __db, char const *__file) {
	ff_err_t err;
	ff_i8_t load = -1;
	if (!access(__file, F_OK)) {
		ffly_printf("database file exists.\n");
		load = 0;
	}

	__db->file = ffly_fopen(__file, FF_O_RDWR|FF_O_CREAT, FF_S_IRUSR|FF_S_IWUSR, &err);
	ffly_fopt(__db->file, FF_NOBUFF);
	if (_err(err)) {
		ffly_printf("failed to open database file.\b");
		_ret;
	}

	if (!load)
		ffdb_load(__db);
	retok;
}

ff_err_t ff_db_snd_key(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey) {
	ff_err_t err;
	ff_u8_t tmp[KEY_SIZE];
	ffly_mem_cpy(tmp, __key, KEY_SIZE);
	ffly_encrypt(tmp, KEY_SIZE, __enckey);
	ff_net_send(__sock, tmp, KEY_SIZE, &err);
	return err;
}

ff_err_t ff_db_rcv_key(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey) {
	ff_err_t err; 
	ff_net_recv(__sock, __key, KEY_SIZE, &err);
	ffly_decrypt(__key, KEY_SIZE, __enckey);
	return err;
}

ff_err_t ff_db_snd_err(FF_SOCKET *__sock, ff_err_t __err) {
	ff_err_t err;
	ff_net_send(__sock, &__err, sizeof(ff_err_t), &err);
	return err;
}

ff_err_t ff_db_rcv_err(FF_SOCKET *__sock, ff_err_t *__err) {
	ff_err_t err;
	ff_net_recv(__sock, __err, sizeof(ff_err_t), &err);
	return err;
}

ff_err_t ff_db_snd_errno(FF_SOCKET *__sock, ff_db_err __err) {
	ff_err_t err;
	ff_net_send(__sock, &__err, sizeof(ff_db_err), &err);
	return err;
}

ff_err_t ff_db_rcv_errno(FF_SOCKET *__sock, ff_db_err *__err) {
	ff_err_t err;
	ff_net_recv(__sock, __err, sizeof(ff_db_err), &err);
	return err;
}

ff_err_t ff_db_sndmsg(FF_SOCKET *__sock, ff_db_msgp __msg) {
	ff_err_t err;
	ff_uint_t sent;

	sent = ff_net_send(__sock, __msg, msgsize, &err); 
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to send message.\n");
		return err;
	}

	if (sent != msgsize) {
		ffly_fprintf(ffly_err, "message was not fully sent.\n");
		reterr;
	}
	retok;
}

ff_err_t ff_db_rcvmsg(FF_SOCKET *__sock, ff_db_msgp __msg) {
	ff_err_t err;
	ff_uint_t recved;

	recved = ff_net_recv(__sock, __msg, msgsize, &err); 
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to recv message.\n");
		return err;
	}

	if (recved != msgsize) {
		ffly_fprintf(ffly_err, "message was not fully recved.\n");
		reterr;
	}
	retok;
}

ff_err_t ffdb_close(ffdbp __db) {
	ffly_fclose(__db->file);
}

void static
prepare_pile(ffdb_pilep __p) {
	ffly_map_init(&__p->map, _ffly_map_127);
	__p->top = NULL;
}

void static
destroy_pile(ffdbp __db, ffdb_pilep __p) {
	ffdb_recordp rec = __p->top, bk;
	while(rec != NULL) {
		bk = rec;
		rec = rec->next;
		ffdb_record_del(__db, __p, bk);
	} 

	ffly_map_de_init(&__p->map);
}

void static
free_pile(ffdbp __db, ffdb_pilep __p) {
	destroy_pile(__db, __p);
	__ffly_mem_free(__p);
}

// clear block bin
void static 
clear_bbin(void) {
	ffdb_blkdp b = ffdb_bbin, bk;
	while(b != NULL) {
		bk = b;
		b = b->fd;
		__ffly_mem_free(bk);
	}
}

// remove rivets and memory
void static
rivet_disposal(void) {
	ffdb_rivetp r = rtop, bk;
	while(r != NULL) {
		bk = r;
		r = r->fd;
		delink_rivet(bk);
		__ffly_mem_free(bk);
	}
}

ff_err_t ffdb_cleanup(ffdbp __db) {
	ffly_map_de_init(&__db->map);

	ffdb_pilep cur = __db->top, bk;
	ffdb_pilep *p;
	if (!cur)
		goto _sk;

	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		free_pile(__db, bk);
	}

	__db->top = NULL;
_sk:
	if (__db->next > __db->free) {
		p = __db->free;
		while(p != __db->next) {
			__ffly_mem_free(*p);
			p++;
		}
		__db->next = __db->free; 
	}

	clear_bbin();
	rivet_disposal();
}

/*
	record creat and del dont effect the space allocated on file
	if not freed the space will be forever lost in the abyss
*/

ffdb_recordp
ffdb_record_creat(ffdbp __db, ffdb_pilep __pile, ff_uint_t __size) { 
	ffdb_recordp p = (ffdb_recordp)__ffly_mem_alloc(sizeof(struct ffdb_record));
	p->prev = NULL;
	p->next = __pile->top;
	if (__pile->top != NULL)
		__pile->top->prev = p;
	__pile->top = p;
	p->size = __size;
	p->p = FFDB_NULL;
	return p;
}

void ffdb_record_alloc(ffdbp __db, ffdb_recordp __rec) {
	ff_uint_t page_c = __rec->size>>PAGE_SHIFT;
	page_c = page_c+((__rec->size-(page_c*PAGE_SIZE))>0);
	__rec->p = ffdb_balloc(__db, page_c*PAGE_SIZE);
}

void ffdb_record_free(ffdbp __db, ffdb_recordp __rec) {
	ffdb_bfree(__db, __rec->p);
}

// ffdb_record_fetch
ffdb_recordp
ffdb_record_fetch(ffdbp __db, char const *__pile, char const *__name) {
	ffdb_recordp p;

	ff_err_t err;
	p = (ffdb_recordp)ffly_map_get(&ffdb_pile_fetch(__db, __pile)->map,
		(ff_u8_t const*)__name, ffly_str_len(__name), &err);	 
	return p;
}

void ffdb_record_del(ffdbp __db, ffdb_pilep __pile, ffdb_recordp __rec) {
	if (__pile->top == __rec) {
		if ((__pile->top = __rec->next) != NULL)
			__pile->top->prev = NULL;
	} else {
		if (__rec->prev != NULL)
			__rec->prev->next = __rec->next;
		if (__rec->next != NULL)
			__rec->next->prev = __rec->prev;
	}
	__ffly_mem_free(__rec);
}

void ffdb_write(ffdbp __db, ffdb_pilep __pile, ffdb_recordp __rec,
	ff_off_t __off, void *__buf, ff_uint_t __size)	
{
	ffly_fseek(__db->file, __rec->p+__off, FF_SEEK_SET);
	ffly_fwrite(__db->file, __buf, __size);
}

void ffdb_read(ffdbp __db, ffdb_pilep __pile, ffdb_recordp __rec,
	ff_off_t __off, void *__buf, ff_uint_t __size)
{
	ffly_fseek(__db->file, __rec->p+__off, FF_SEEK_SET);
	ffly_fread(__db->file, __buf, __size);
}

void ffdb_record_stat(ffdbp __db, ffdb_recordp __rec, ffdb_recstatp __stat) {
	__stat->size = __rec->size;
}

void ffdb_record_alias(ffdbp __db, ffdb_pilep __pile, char const *__name, ffdb_recordp __p) {
	ffly_map_put(&__pile->map, (ff_u8_t const*)__name, ffly_str_len(__name), __p);
}

void ffdb_pile_alias(ffdbp __db, char const *__name, ffdb_pilep __p) {
	ffly_map_put(&__db->map, (ff_u8_t const*)__name, ffly_str_len(__name), __p);
}


// ffly_pile_...
ffdb_pilep ffdb_pile_creat(ffdbp __db) {
	ffdb_pilep p;
	if (__db->next > __db->free)
		p = *(--__db->next);
	else
		p = __ffly_mem_alloc(sizeof(struct ffdb_pile));

	p->next = __db->top;
	if (__db->top != NULL)
		__db->top->prev = p;
	__db->top = p;
	p->prev = NULL;
	prepare_pile(p);
	return p;
}

ffdb_pilep ffdb_pile_fetch(ffdbp __db, char const *__name) {
	ffdb_pilep p;

	ff_err_t err;
	p = (ffdb_pilep)ffly_map_get(&__db->map, (ff_u8_t const*)__name,
		ffly_str_len(__name), &err);
	return p;
}

void ffdb_pile_del(ffdbp __db, ffdb_pilep __pile) {
	if (__pile == __db->top) {
		if ((__db->top = __pile->next) != NULL)
			__db->top->prev = NULL;
		goto _sk;
	}

	if (__pile->prev != NULL)
		__pile->prev->next = __pile->next;
	if (__pile->next != NULL)
		__pile->next->prev = __pile->prev;
	__pile->prev = NULL;
	__pile->next = NULL;

_sk:
	if (__db->next-__db->free < 20) {
		destroy_pile(__db, __pile);
		*(__db->next++) = __pile;
		return;
	} 

	free_pile(__db, __pile);
}

ff_uint_t static no = 0;
ff_uint_t static rand() {
	ff_uint_t ret;
	ret = no;
	no = ((no<<1)|0x1)+7;
	no = no^((no>>8)&0xff);
	no = no&0xffff;
	return ret;
}

void static ts0(ffdbp __db) {
	ff_uint_t p0;
	ff_u8_t i = 0;
	p0 = ffdb_balloc(__db, 100);
	ffdb_balloc(__db, 1);
	ffdb_bfree(__db, p0);

	while(i != 200) {
		p0 = ffdb_balloc(__db, rand()%100);
		ffdb_bfree(__db, p0);
		i++;
	}
}

void static ts1(ffdbp __db, char const *__pile) {
	ffdb_pilep p = ffdb_pile_fetch(__db, __pile);
	ff_u8_t i = 0;   
 
	ffdb_recordp rec0 = ffdb_record_creat(__db, p, 100);
	ffdb_recordp rec1 = ffdb_record_creat(__db, p, 1);
	ffdb_record_del(__db, p, rec0);

	while(i != 0xf) {
		ffdb_recordp rec = ffdb_record_creat(__db, p, rand()%100);
		ffdb_record_del(__db, p, rec); 
		i++;
	}
	ffdb_record_del(__db, p, rec1);
}

void static ts2(ffdbp __db) {
	ff_uint_t p0, p1, p2, p3, p4, p5;
	p0 = ffdb_balloc(__db, 1);
	p1 = ffdb_balloc(__db, 1);
	p2 = ffdb_balloc(__db, 1);
	p3 = ffdb_balloc(__db, 1);
	p4 = ffdb_balloc(__db, 1);
	p5 = ffdb_balloc(__db, 1);

	ffdb_bfree(__db, p0);
	ffdb_bfree(__db, p2);
	ffdb_bfree(__db, p4);

	ffdb_bfree(__db, p3);
	ffdb_bfree(__db, p1);
	ffdb_bfree(__db, p5); 
}

void static ts3(ffdbp __db) {
	ff_uint_t p0, p1, p2, p3, p4, p5;
	p0 = ffdb_balloc(__db, 1);
	p1 = ffdb_balloc(__db, 2);
	p2 = ffdb_balloc(__db, 3);
	p3 = ffdb_balloc(__db, 4);
	p4 = ffdb_balloc(__db, 5);
	p5 = ffdb_balloc(__db, 6);

	ffdb_bfree(__db, p0);
	ffdb_bfree(__db, p1);
	ffdb_bfree(__db, p2);
	ffdb_bfree(__db, p3);
	ffdb_bfree(__db, p4);
	ffdb_bfree(__db, p5);
}

void _pf() {
	struct ffdb_blkd *blk = ffdb_bbin;
	ffly_printf("---- free ----\n");
	while(blk != NULL) {
		ffly_printf("size: %u\n", blk->size);
		blk = blk->fd;
	}
}


void _pr(ffdbp __db) {
	ff_uint_t off = ffdb_btop;
	struct ffdb_blkd blk;
	ffly_printf("---- all ----\n");
	while(off != FFDB_NULL) {
		ffdb_blkd_read(__db, &blk, off);
		ffly_printf("size: %u\n", blk.size);
		off = blk.prev;
	}
}

void static ts4(ffdbp __db) {
	ffdb_recordp rec = ffdb_record_fetch(__db, "users", "mrdoomlittle");
	ffdb_pilep p = ffdb_pile_fetch(__db, "users");
	char passwd[40];
	char c;
	ff_uint_t off = 0;
	while(1) {
		ffdb_read(__db, p, rec, off, &c, 1);
		if (c == '\0') break;
		*(passwd+off) = c;
		off++;
	}
	*(passwd+off) = '\0';

	ffly_printf("passwd: %s\n", passwd);
}
//# define __debug
# ifdef __debug
# include "dep/str_cpy.h"
# define PILE 0
# define REC 1
ff_err_t ffmain(int __argc, char const *__argv[]) {
	struct ffdb db;
	ffdb_init(&db);
	ffdb_open(&db, "test.db");

/*
	if (ffdb_exist(PILE) == -1) {
		ffly_printf("creating pile.\n");
		ffdb_pilep pile = ffdb_pile_creat(&db);
		ffdb_rivet(PILE, pile);
		ffdb_bind(pile, PILE);
	} else {
		ffly_printf("pile already exists.\n");
		ffdb_pilep pile;
		ffdb_recordp rec;
		pile = ffdb_rivetto(PILE);
		char buf[67];
		if (ffdb_exist(GITHUB_REC) == -1) {
			rec = ffdb_record_creat(&db, pile, 67);
			ffdb_record_alloc(&db, rec);
			ffdb_rivet(GITHUB_REC, rec);
			ffdb_bind(rec, GITHUB_REC);

			ffly_str_cpy(buf, "http://github.com/mrdoomlittle/");
			ffdb_write(&db, pile, rec, 0, buf, 67);
		} else {
			rec = ffdb_rivetto(GITHUB_REC);

			ffdb_read(&db, pile, rec, 0, buf, 67);
			ffly_printf("github: %s\n", buf);
			ffdb_record_free(&db, rec);
			ffdb_record_del(&db, pile, rec);
			ffdb_derivet(GITHUB_REC);
		}

		if (ffdb_exist(TWITTER_REC) == -1) {
			rec = ffdb_record_creat(&db, pile, 67);
			ffdb_record_alloc(&db, rec);
			ffdb_rivet(TWITTER_REC, rec);
			ffdb_bind(rec, TWITTER_REC);
			ffly_str_cpy(buf, "http://twitter.com/mrunoko/");
			ffdb_write(&db, pile, rec, 0, buf, 67);
		} else {
			rec = ffdb_rivetto(TWITTER_REC);
			ffdb_read(&db, pile, rec, 0, buf, 67);
			ffly_printf("twitter: %s\n", buf);
		}
	}
*/
	ffdb_recordp rec;
	ffdb_pilep pile;
	if (ffdb_exist(PILE) == -1) {
		ffly_printf("no pile.\n");
		pile = ffdb_pile_creat(&db);
		ffdb_rivet(PILE, pile);
		ffdb_bind(pile, PILE);

//		rec = ffdb_record_creat(&db, pile, 67);
//		ffdb_record_alloc(&db, rec);
//		ffdb_rivet(REC, rec);
//		ffdb_bind(rec, REC);
	} else {
		pile = ffdb_rivetto(PILE);
		rec = ffdb_rivetto(REC);
//		ffdb_record_free(&db, rec);

//		ffdb_record_del(&db, pile, rec);
//		ffdb_derivet(REC);

		ffdb_derivet(PILE);
		ffdb_pile_del(&db, pile);
	}
	_pr(&db);
	_pf();
	
/*
	ff_u32_t p0, p1;
	p0 = ffdb_balloc(&db, 100);
	p1 = ffdb_balloc(&db, 100);


	ffdb_bfree(&db, p0);
	p0 = ffdb_balloc(&db, 100);

	ffdb_bfree(&db, p1);
	ffdb_bfree(&db, p0);

	_pr(&db);
	_pf();
*/
	ffdb_settle(&db);
	ffdb_cleanup(&db);
	ffdb_close(&db);
}
# endif
