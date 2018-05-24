# include "port.h"
# include "shm.h"
# include "../dep/bcopy.h"
# include "../linux/ipc.h"
# include "../linux/stat.h"
# include "error.h"
# include "mutex.h"
# include "io.h"
# define STRT 0x1
# define STOP 0x2
# define DUMP 0x4
# define OK 0x8

# define is_flag(__flags, __flag) \
	((__flags&__flag) == __flag)
# define clr_bit(__bits, __bit) \
	(*__bits) ^= *__bits&__bit;
# define set_bit(__bits, __bit)\
	(*__bits) |= __bit;
# define is_bit(__bits, __bit) \
	((*__bits&__bit) == __bit)
# define get_port(__id) (ports+__id)
# define MAX 5

typedef struct hdr {
	ff_u16_t size;
} *hdrp;

struct port {
	ff_uint_t shm_id;
	ff_u8_t *bits;
	hdrp h;
	void *buf, *p;
	ff_uint_t width;
	ff_u8_t flags;
	ff_mlock_t *tx_lock;
	ff_u32_t *band;
};

struct port static ports[MAX];
struct port static *next = ports;
struct port static *dead[MAX];
struct port static **reuse = dead;

# define ADRNULL 0xff
ff_uint_t ffly_port_get_shmid(ff_uint_t __id) {
	return get_port(__id)->shm_id;
}

void static
discard(struct port *__pi) {
	if (__pi == next-1)
		next--;
	else
		*(reuse++) = __pi;
}

ff_uint_t ffly_port(ff_uint_t __width, ff_u8_t __flags,
	ff_uint_t __id, ff_err_t *__err)
{
	*__err = FFLY_SUCCESS;
	ff_uint_t id;
	if (reuse>dead)
		id = *(--reuse)-ports;
	else
		id = (next++)-ports;
	struct port *pt = ports+id;
	pt->flags = __flags;
	int flags = 0;
	if (is_flag(__flags, FF_PORT_CREAT))
		flags |= IPC_CREAT;
	if (is_flag(__flags, FF_PORT_SHMM))
		pt->shm_id = __id;
	pt->p = ffly_shmget(&pt->shm_id, __width+sizeof(struct hdr)+1+sizeof(ff_mlock_t)+sizeof(ff_u32_t),
		flags|S_IRUSR|S_IWUSR, is_flag(__flags, FF_PORT_SHMM)?FF_SHM_MCI:0);
	if (!pt->p) {
		ffly_fprintf(ffly_out, "pipe failure.\n");
	}

	pt->width = __width;
	ff_u8_t *p = (ff_u8_t*)pt->p;
	pt->bits = p++;
	pt->h = (hdrp)p;
	p+=sizeof(struct hdr);
	pt->buf = (void*)p;
	p+=__width;
	pt->tx_lock = (ff_mlock_t*)p;
	p+=sizeof(ff_mlock_t);
	pt->band = (ff_u32_t*)p;

	if (is_flag(__flags, FF_PORT_CREAT)) {
		*pt->bits = 0x0;
		*pt->tx_lock = FFLY_MUTEX_INIT;
		*pt->band = ADRNULL;
	}
	return id;
}

void ffly_port_close(ff_uint_t __id) {
	struct port *pt = get_port(__id);
	ffly_shmdt(pt->shm_id);
	if (is_flag(pt->flags, FF_PORT_CREAT))
		ffly_shm_free(pt->shm_id);
	ffly_shm_cleanup(pt->shm_id);
	discard(pt);
}

void static
port_snd(ff_uint_t __port, void *__buf, ff_uint_t __size) {
	struct port *pt = get_port(__port);
	set_bit(pt->bits, STRT);
	while(is_bit(pt->bits, STRT));

	ff_u8_t *p = (ff_u8_t*)__buf;
	ff_u8_t *end = p+__size;
	while(p < end) {
		ff_uint_t left = __size-(p-(ff_u8_t*)__buf);
		ff_uint_t size = left>pt->width?pt->width:left;
		ffly_bcopy(pt->buf, p, size);
		p+=size;
		pt->h->size = size;
		set_bit(pt->bits, DUMP);
		while(is_bit(pt->bits, DUMP));
	}

	set_bit(pt->bits, STOP);
	while(!is_bit(pt->bits, OK));
	clr_bit(pt->bits, OK);
}

void static
port_rcv(ff_uint_t __port, void *__buf, ff_uint_t __size) {
	struct port *pt = get_port(__port);
	while(is_bit(pt->bits, OK));
	while(!is_bit(pt->bits, STRT));
	clr_bit(pt->bits, STRT);

	ff_u8_t *p = (ff_u8_t*)__buf;
	ff_u8_t *end = p+__size;
	while(!is_bit(pt->bits, STOP)) {
		if (is_bit(pt->bits, DUMP) && p < end) {
			ff_uint_t size = pt->h->size;
			ffly_bcopy(p, pt->buf, size);
			p+=size;
			clr_bit(pt->bits, DUMP);
		}
	}
	clr_bit(pt->bits, STOP);
	set_bit(pt->bits, OK);
	while(is_bit(pt->bits, OK));
}


ff_err_t
ffly_port_send(ff_u32_t __band, void *__buf, ff_uint_t __size, ff_uint_t __port) {
	struct port *pt = get_port(__port);
	ffly_mutex_lock(pt->tx_lock);
	*pt->band = __band;
	port_snd(__port, __buf, __size);
	ffly_mutex_unlock(pt->tx_lock);
	retok;
}

ff_i8_t ffly_port_band(ff_uint_t __port, ff_u32_t __band) {
	return ((ff_i8_t)(*get_port(__port)->band == __band))-1;
}

ff_err_t
ffly_port_recv(ff_u32_t __band, void *__buf, ff_uint_t __size, ff_uint_t __port) {
	struct port *pt = get_port(__port);
	while(*pt->band != __band);
	port_rcv(__port, __buf, __size);
	retok;
}
