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
# define blkd_size sizeof(struct ffdb_blkd)
# define PAGE_SHIFT 6
# define PAGE_SIZE (1<<PAGE_SHIFT)
char const* ff_db_errno_str(ff_db_errno __errno) {
	switch(__errno) {
		case _ff_err_null: return "no error";
		case _ff_err_lci: return "login credentials incorrect";
		case _ff_err_nsu: return "no such user";
		case _ff_err_ali: return "already logged in";
		case _ff_err_prd: return "permission denied";
	}
	return "unknown";
}

ffly_err_t ffdb_init(ffdbp __db) {
	__db->off = 0;
	__db->top = NULL;
	__db->next = __db->free;
	ffly_map_init(&__db->map, _ffly_map_127);
}

ffly_err_t ffdb_open(ffdbp __db, char const *__file) {
	ffly_err_t err;
	__db->file = ffly_fopen(__file, FF_O_RDWR|FF_O_TRUNC|FF_O_CREAT, FF_S_IRUSR|FF_S_IWUSR, &err);
}

ffly_err_t ff_db_snd_key(FF_SOCKET *__sock, mdl_u8_t *__key, mdl_u64_t __enckey) {
	ffly_err_t err;
	mdl_u8_t tmp[KEY_SIZE];
	ffly_mem_cpy(tmp, __key, KEY_SIZE);
	mdl_encrypt(tmp, KEY_SIZE, __enckey);
	ff_net_send(__sock, tmp, KEY_SIZE, &err);
	return err;
}

ffly_err_t ff_db_rcv_key(FF_SOCKET *__sock, mdl_u8_t *__key, mdl_u64_t __enckey) {
	ffly_err_t err; 
	ff_net_recv(__sock, __key, KEY_SIZE, &err);
	mdl_decrypt(__key, KEY_SIZE, __enckey);
	return err;
}

ffly_err_t ff_db_snd_err(FF_SOCKET *__sock, ffly_err_t __err) {
	ffly_err_t err;
	ff_net_send(__sock, &__err, sizeof(ffly_err_t), &err);
	return err;
}

ffly_err_t ff_db_rcv_err(FF_SOCKET *__sock, ffly_err_t *__err) {
	ffly_err_t err;
	ff_net_recv(__sock, __err, sizeof(ffly_err_t), &err);
	return err;
}

ffly_err_t ff_db_snd_errno(FF_SOCKET *__sock, ff_db_errno __err) {
	ffly_err_t err;
	ff_net_send(__sock, &__err, sizeof(ff_db_errno), &err);
	return err;
}

ffly_err_t ff_db_rcv_errno(FF_SOCKET *__sock, ff_db_errno *__err) {
	ffly_err_t err;
	ff_net_recv(__sock, __err, sizeof(ff_db_errno), &err);
	return err;
}

ffly_err_t ff_db_sndmsg(FF_SOCKET *__sock, ff_db_msgp __msg) {
	ffly_err_t err;
	mdl_uint_t sent;

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

ffly_err_t ff_db_rcvmsg(FF_SOCKET *__sock, ff_db_msgp __msg) {
	ffly_err_t err;
	mdl_uint_t recved;

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

ffly_err_t ffdb_close(ffdbp __db) {
	ffly_fclose(__db->file);
}

void prepare_pile(ffdb_pilep __p) {
	ffly_map_init(&__p->map, _ffly_map_127);
	__p->top = NULL;
}

void destroy_pile(ffdbp __db, ffdb_pilep __p) {
	ffdb_recordp rec = __p->top;
	while(rec != NULL) {
		ffdb_del_record(__db, __p, rec);
		rec = rec->next;
	} 

	ffly_map_de_init(&__p->map);
}

void free_pile(ffdbp __db, ffdb_pilep __p) {
	destroy_pile(__db, __p);
	__ffly_mem_free(__p);
}

ffly_err_t ffdb_cleanup(ffdbp __db) {
	ffly_map_de_init(&__db->map);
	ffdb_pilep cur = __db->top, prev;
	ffdb_pilep *p;
	if (!cur)
		goto _sk;
	prev = NULL;
	while(cur != NULL) {
		if (prev != NULL)
			free_pile(__db, prev);
		prev = cur;
		cur = cur->next;
	}
	free_pile(__db, prev);
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
}

ffdb_recordp ffdb_creat_record(ffdbp __db, ffdb_pilep __pile, mdl_uint_t __size) { 
	ffdb_recordp p = (ffdb_recordp)__ffly_mem_alloc(sizeof(struct ffdb_record));
	p->prev = NULL;
	p->next = NULL;
	if (!__pile->top)
		__pile->top = p;
	else {
		p->next = __pile->top;
		__pile->top->prev = p;
		__pile->top = p;
	}

	mdl_uint_t page_c = __size>>PAGE_SHIFT;
	page_c = page_c+((__size-(page_c*PAGE_SIZE))>0);
	p->p = ffdb_alloc(__db, page_c*PAGE_SIZE);
	return p;
}

ffdb_recordp ffdb_fetch_record(ffdbp __db, char const *__pile, char const *__name) {
	ffdb_recordp p;

	ffly_err_t err;
	p = (ffdb_recordp)ffly_map_get(&ffdb_fetch_pile(__db, __pile)->map, (mdl_u8_t const*)__name, ffly_str_len(__name), &err);	 
	return p;
}

void ffdb_del_record(ffdbp __db, ffdb_pilep __pile, ffdb_recordp __rec) {
	if (__pile->top == __rec) {
		__pile->top = __rec->next;
		if (__pile->top != NULL)
			__pile->top->prev = NULL;
	} else {
		if (__rec->prev != NULL)
			__rec->prev->next = __rec->next;
		if (__rec->next != NULL)
			__rec->next->prev = __rec->prev;
	}
 
	ffdb_free(__db, __rec->p); 
}

void write_blkd(ffdbp __db, ffdb_blkdp __p, ffly_off_t __off) {
	ffly_fseek(__db->file, __off, FF_SEEK_SET);
	ffly_fwrite(__db->file, __p, blkd_size);
}

void read_blkd(ffdbp __db, ffdb_blkdp __p, ffly_off_t __off) {
	ffly_fseek(__db->file, __off, FF_SEEK_SET);
	ffly_fread(__db->file, __p, blkd_size);
}

void ffdb_write(ffdbp __db, ffdb_pilep __pile, ffdb_recordp __rec, ffly_off_t __off, void *__buf, mdl_uint_t __size) {
	ffly_fseek(__db->file, __rec->p+__off, FF_SEEK_SET);
	ffly_fwrite(__db->file, __buf, __size);
}

void ffdb_read(ffdbp __db, ffdb_pilep __pile, ffdb_recordp __rec, ffly_off_t __off, void *__buf, mdl_uint_t __size) {
	ffly_fseek(__db->file, __rec->p+__off, FF_SEEK_SET);
	ffly_fread(__db->file, __buf, __size);
}

void ffdb_record_alias(ffdbp __db, ffdb_pilep __pile, char const *__name, ffdb_recordp __p) {
	ffly_map_put(&__pile->map, (mdl_u8_t const*)__name, ffly_str_len(__name), __p);
}

void ffdb_pile_alias(ffdbp __db, char const *__name, ffdb_pilep __p) {
	ffly_map_put(&__db->map, (mdl_u8_t const*)__name, ffly_str_len(__name), __p);
}

ffdb_pilep ffdb_creat_pile(ffdbp __db) {
	ffdb_pilep p;
	if (__db->next > __db->free)
		p = *(--__db->next);
	else
		p = __ffly_mem_alloc(sizeof(struct ffdb_pile));
	p->prev = NULL;
	p->next = NULL;
	if (!__db->top)
		__db->top = p; 
	else {
		p->next = __db->top;
		__db->top->prev = p;
		__db->top = p; 
	}

	prepare_pile(p);
	return p;
}

ffdb_pilep ffdb_fetch_pile(ffdbp __db, char const *__name) {
	ffdb_pilep p;

	ffly_err_t err;
	p = (ffdb_pilep)ffly_map_get(&__db->map, (mdl_u8_t const*)__name, ffly_str_len(__name), &err);
	return p;
}

void ffdb_del_pile(ffdbp __db, ffdb_pilep __pile) {
	if (__pile == __db->top) {
		__db->top = __pile->next;
		if (__db->top != NULL)
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


# define FFDB_NULL ((mdl_uint_t)~0)
mdl_uint_t static top = FFDB_NULL;
ffdb_blkdp static bin = NULL;
void ffdb_unlink(ffdb_blkdp);
mdl_uint_t ffdb_alloc(ffdbp __db, mdl_uint_t __bc) {
	if (!__bc) return FFDB_NULL;
	if (bin != NULL) {
		struct ffdb_blkd *p = bin;
		while(p != NULL) {
			ffly_printf("%u >= %u\n", p->size, __bc);
			if (p->size >= __bc) {
				mdl_uint_t leftover = p->size-__bc;
				if (leftover>blkd_size) {
					if (leftover-blkd_size >= PAGE_SIZE) {

					}
				}
				ffly_printf("found free space.\n");
				ffdb_unlink(p);
				return p->off+blkd_size; 
			}
			p = p->fd;
		}
	}

	ffly_off_t off = __db->off;
	struct ffdb_blkd blk = {
		.size = __bc,
		.off = off,
		.fd = NULL,
		.bk = NULL,
		.p = NULL,
		.prev = top,
		.next = FFDB_NULL, .inuse = 1
	};

	if (top != FFDB_NULL) {
		struct ffdb_blkd blk;
		read_blkd(__db, &blk, top);
		blk.next = off;
		write_blkd(__db, &blk, top);
	}

	top = off;
	__db->off+=__bc+blkd_size;
	blk.end =  __db->off;
	write_blkd(__db, &blk, off);
	return off+blkd_size;
}

void ffdb_reattach(ffdbp __db, struct ffdb_blkd *__blk) {
	struct ffdb_blkd prev, next;
	if (__blk->end == __db->off) {
		struct ffdb_blkd blk;
		read_blkd(__db, &blk, top);
		blk.next = __blk->off;
		write_blkd(__db, &blk, top);
		top = __blk->off;
		return;
	}  

	if (__blk->prev != FFDB_NULL) {
		read_blkd(__db, &prev, __blk->prev);
		prev.next = __blk->off;
		write_blkd(__db, &prev, __blk->prev); 
	}

	if (__blk->next != FFDB_NULL) {
		read_blkd(__db, &next, __blk->next);
		next.prev = __blk->off;
		write_blkd(__db, &next, __blk->next);
	}
}

void ffdb_detatch(ffdbp __db, struct ffdb_blkd *__blk) {
	struct ffdb_blkd prev, next;
	if (__blk->off == top) {
		top = __blk->prev;
		if (top != FFDB_NULL) {
			struct ffdb_blkd blk;
			read_blkd(__db, &blk, top);
			blk.next = FFDB_NULL;
			write_blkd(__db, &blk, top);
		}
		return;
	}

	if (__blk->prev != FFDB_NULL) {
		read_blkd(__db, &prev, __blk->prev);
		prev.next = __blk->next;
		write_blkd(__db, &prev, __blk->prev);
	}

	if (__blk->next != FFDB_NULL) {
		read_blkd(__db, &next, __blk->next);
		next.prev = __blk->prev;
		write_blkd(__db, &next, __blk->next);
	}
}

void ffdb_unlink(ffdb_blkdp __blk) {
	if (__blk->p == bin) {
		bin = __blk->fd;
		if (bin != NULL)
			bin->bk = NULL;
		goto _end;
	}

	if (__blk->fd != NULL)
		__blk->fd->bk = __blk->bk;
	if (__blk->bk != NULL)
		__blk->bk->fd = __blk->fd;
	_end:
	__ffly_mem_free(__blk);
}

# include "linux/unistd.h"
void ffdb_free(ffdbp __db, mdl_uint_t __p) {
	if (__p == FFDB_NULL) return;
	struct ffdb_blkd blk;
	ffly_off_t off = __p-blkd_size;
	read_blkd(__db, &blk, off);
	ffdb_detatch(__db, &blk);

	ffly_printf("blockd size: %u\n", blkd_size);
	ffly_printf("0; %ubytes.\n", blk.size);
	struct ffdb_blkd prev, next;
	if (blk.prev != FFDB_NULL) {
		read_blkd(__db, &prev, blk.prev);
		if (!prev.inuse) {
			ffly_printf("prev not inuse, %ubytes.\n", blk.size);
			ffdb_detatch(__db, &prev);
			ffdb_unlink(prev.p);
			blk.size += prev.size+blkd_size;
			blk.prev = prev.prev;
			blk.off = prev.off;
			off = blk.off;
		}
	}
	ffly_printf("1; %ubytes.\n", blk.size);
	if (blk.next != FFDB_NULL) {
		read_blkd(__db, &next, blk.next);
		if (!next.inuse) {
			ffly_printf("next not inuse, %ubytes.\n", blk.size);
			ffdb_detatch(__db, &next);
			ffdb_unlink(next.p);
			blk.next = next.next;
			blk.size += next.size+blkd_size;
			blk.end = next.end; 
		}
	}
	ffly_printf("2; %ubytes.\n", blk.size);
	if (blk.end == __db->off) {
		__db->off = off;
		ftruncate(ffly_fileno(__db->file), off);
		return;
	}

	ffdb_reattach(__db, &blk);
	blk.inuse = 0;
	blk.p = (ffdb_blkdp)__ffly_mem_alloc(sizeof(struct ffdb_blkd));
	*blk.p = blk;
	if (!bin)
		bin = blk.p;
	else {
		bin->bk = blk.p;
		blk.p->fd = bin;
		bin = blk.p;
	}
	ffly_printf("freed: %u\n", blk.size);
	write_blkd(__db, &blk, off);	
}

mdl_uint_t no = 0;
mdl_uint_t rand() {
	mdl_uint_t ret;
	ret = no;
	no = ((no<<1)|0x1)+7;
	no = no^((no>>8)&0xff);
	no = no&0xffff;
	return ret;
}

void ts0(ffdbp __db) {
	mdl_uint_t p0;
	mdl_u8_t i = 0;
	p0 = ffdb_alloc(__db, 100);
	ffdb_alloc(__db, 1);
	ffdb_free(__db, p0);

	while(i != 200) {
		p0 = ffdb_alloc(__db, rand()%100);
		ffdb_free(__db, p0);
		i++;
	}
}

void ts1(ffdbp __db, char const *__pile) {
	ffdb_pilep p = ffdb_fetch_pile(__db, __pile);
	mdl_u8_t i = 0;   
 
	ffdb_recordp rec0 = ffdb_creat_record(__db, p, 100);
	ffdb_recordp rec1 = ffdb_creat_record(__db, p, 1);
	ffdb_del_record(__db, p, rec0);

	while(i != 0xf) {
		ffdb_recordp rec = ffdb_creat_record(__db, p, rand()%100);
		ffdb_del_record(__db, p, rec); 
		i++;
	}
	ffdb_del_record(__db, p, rec1);
}

void ts2(ffdbp __db) {
	mdl_uint_t p0, p1, p2, p3, p4, p5;
	p0 = ffdb_alloc(__db, 1);
	p1 = ffdb_alloc(__db, 1);
	p2 = ffdb_alloc(__db, 1);
	p3 = ffdb_alloc(__db, 1);
	p4 = ffdb_alloc(__db, 1);
	p5 = ffdb_alloc(__db, 1);

	ffdb_free(__db, p0);
	ffdb_free(__db, p2);
	ffdb_free(__db, p4);

	ffdb_free(__db, p3);
	ffdb_free(__db, p1);
	ffdb_free(__db, p5); 
}

void ts3(ffdbp __db) {
	mdl_uint_t p0, p1, p2, p3, p4, p5;
	p0 = ffdb_alloc(__db, 1);
	p1 = ffdb_alloc(__db, 2);
	p2 = ffdb_alloc(__db, 3);
	p3 = ffdb_alloc(__db, 4);
	p4 = ffdb_alloc(__db, 5);
	p5 = ffdb_alloc(__db, 6);

	ffdb_free(__db, p0);
	ffdb_free(__db, p1);
	ffdb_free(__db, p2);
	ffdb_free(__db, p3);
	ffdb_free(__db, p4);
	ffdb_free(__db, p5);
}

void _pf() {
	struct ffdb_blkd *blk = bin;
	ffly_printf("---- free ----\n");
	while(blk != NULL) {
		ffly_printf("size: %u\n", blk->size);
		blk = blk->fd;
	}
}


void _pr(ffdbp __db) {
	mdl_uint_t off = top;
	struct ffdb_blkd blk;
	ffly_printf("---- all ----\n");
	while(off != FFDB_NULL) {
		read_blkd(__db, &blk, off);
		ffly_printf("size: %u\n", blk.size);
		off = blk.prev;
	}
}


void ts4(ffdbp __db) {
	ffdb_recordp rec = ffdb_fetch_record(__db, "users", "mrdoomlittle");
	ffdb_pilep p = ffdb_fetch_pile(__db, "users");
	char passwd[40];
	char c;
	mdl_uint_t off = 0;
	while(1) {
		ffdb_read(__db, p, rec, off, &c, 1);
		if (c == '\0') break;
		*(passwd+off) = c;
		off++;
	}
	*(passwd+off) = '\0';

	ffly_printf("passwd: %s\n", passwd);
}
/*
int main() {
	ffly_io_init();
	struct ffdb db;
	char const *passwd = "21299";
	mdl_uint_t l = ffly_str_len(passwd);
	ffdb_init(&db);
	ffdb_open(&db, "test.db");

	ffdb_pilep p = ffdb_creat_pile(&db);
	ffdb_pile_alias(&db, "users", p); 
	ffdb_recordp rec = ffdb_creat_record(&db, p, l+1);
	ffdb_write(&db, p, rec, 0, passwd, l+1);
	ffdb_record_alias(&db, p, "mrdoomlittle", rec);
//	  ts1(&db, "example-pile");
	ts4(&db);
	
	ffdb_del_pile(&db, p);

//	  ts2(&db);
	_pf();
	_pr(&db);
	ffdb_close(&db);
	ffdb_cleanup(&db);
	ffly_io_closeup();
}*/
