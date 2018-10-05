# include "slurry.h"
# include "connection.h"
# include "o_sheet.h"
# ifndef __fflib
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# else
# include "../linux/unistd.h"
# include "../string.h"
# include "../linux/socket.h"
# include "../stdio.h"
# endif
# define tape(__conn, __code, __len) \
{	\
	ff_uint_t len; \
	len = __len; \
	send(__conn->sock, &len, sizeof(ff_uint_t), 0); \
	send(__conn->sock, __code, __len, 0);\
}

/*
	TODO:
		build tape then allow user to run it not like ....
*/

ff_u16_t static stack = 0;
ff_u16_t s_window_new(s_connp __conn, ff_u16_t __d) {
	ff_u16_t w;
	w = stack;
	stack+=8;

	ff_u8_t code[5];

	*code = _op_window_new;
	*(ff_u16_t*)(code+1) = w;
	*(ff_u16_t*)(code+3) = __d;
	tape(__conn, code, 5);
	return w;
}

void s_read(s_connp __conn, ff_u16_t __adr, ff_uint_t __n, void *__buf) {
	ff_u8_t code[5];
	*code = _op_read;
	*(ff_u16_t*)(code+1) = __adr;
	*(ff_u16_t*)(code+3) = __n;
	tape(__conn, code, 5);
	recv(__conn->sock, __buf, __n, 0);
}

void s_write(s_connp __conn, ff_u16_t __adr, ff_uint_t __n, void *__buf) {
	ff_u8_t code[5];
	*code = _op_write;
	*(ff_u16_t*)(code+1) = __adr;
	*(ff_u16_t*)(code+3) = __n;
	tape(__conn, code, 5);
	send(__conn->sock, __buf, __n, 0);
}

void s_window_init(s_connp __conn, ff_u16_t __wd, 
	ff_u16_t __width, ff_u16_t __height, ff_u16_t __title)
{
	ff_u8_t code[9];
	*code = _op_window_init;
	*(ff_u16_t*)(code+1) = __wd;
	*(ff_u16_t*)(code+3) = __width;
	*(ff_u16_t*)(code+5) = __height;
	*(ff_u16_t*)(code+7) = __title;
	tape(__conn, code, 9);
}

ff_u16_t s_display_open(s_connp __conn) {
	ff_u16_t d;
	d = stack;
	stack+=8;
	ff_u8_t code[3];
	*code = _op_display_open;
	*(ff_u16_t*)(code+1) = d;
	tape(__conn, code, 3);
	return d;
}

void s_display_close(s_connp __conn, ff_u16_t __d) {
	ff_u8_t code[3];
	*code = _op_display_close;
	*(ff_u16_t*)(code+1) = __d;
	tape(__conn, code, 3);
}

void s_window_destroy(s_connp __conn, ff_u16_t __wd) {
	ff_u8_t code[3];
	*code = _op_window_destroy;
	*(ff_u16_t*)(code+1) = __wd;
	tape(__conn, code, 3);
}

void s_window_display(s_connp __conn, ff_u16_t __wd) {
	ff_u8_t code[3];
	*code = _op_window_display;
	*(ff_u16_t*)(code+1) = __wd;
	tape(__conn, code, 3);
}

void s_window_frame(s_connp __conn, ff_u16_t __wd, ff_u8_t *__frame, ff_uint_t __width, ff_uint_t __height) {
	ff_u8_t code[3];
	*code = _op_window_frame;
	*(ff_u16_t*)(code+1) = __wd;
	tape(__conn, code, 3);
	/*
		using tcp for this is bad
		TODO:
			change
			keep extra udp socket open for this?????
	*/
	s_send(__frame, __width*__height*4, __conn->sock);
}

static struct s_event evbuf[EVBS];
static ff_uint_t ev = 0;
static ff_uint_t nev = 0;

void s_event(s_connp __conn, ff_u16_t __d, struct s_event **__dst) {
	if (ev<nev) {
	_up:
		*__dst = evbuf+(ev++);
	} else {
		printf("new events.\n");
		ff_u8_t code[3];
		*code = _op_event;
		*(ff_u16_t*)(code+1) = __d;
		tape(__conn, code, 3);

		ff_uint_t n;
		recv(__conn->sock, &n, sizeof(ff_uint_t), 0);
		nev = n;
		ev = 0;
		if (n>0)
			recv(__conn->sock, evbuf, n*sizeof(struct s_event), 0);
		else {
			*__dst = NULL;
			return;
		}
		goto _up;
	}
}

ff_i8_t s_pending(s_connp __conn, ff_u16_t __d) {
	ff_i8_t res;
	if (ev<nev) {
		res = 0;
	} else {
		ff_u8_t code[3];
		*code = _op_pending;
		*(ff_u16_t*)(code+1) = __d;
		tape(__conn, code, 3);
		ff_i8_t res;
		recv(__conn->sock, &res, 1, 0);
	}
	return res;
}

ff_u16_t s_rtn(ff_uint_t __sz) {
	ff_u16_t r;
	r = stack;
	stack+=__sz;
	return r;
}

void s_disconnect(s_connp __conn) {
	ff_u8_t code;
	code = _op_disconnect;
	tape(__conn, &code, 1);
}
# ifdef __fflib
# include "../system/nanosleep.h"
void s_test(void) {
	stack = 0;
	s_connp con;
	con = s_open();
	s_connect(con, 10198, "127.0.0.1");
	ff_u16_t d;
	d = s_display_open(con);
	ff_u16_t wd;
	wd = s_window_new(con, d);
	char const *tstr = "MEH";

	ff_u16_t title;
	title = s_rtn(strlen(tstr)+1);
	s_write(con, title, strlen(tstr)+1, tstr);
	s_window_init(con, wd, 600, 600, title);
	ffly_nanosleep(5, 0);	
	s_window_destroy(con, wd);
	s_display_close(con, d);
	s_disconnect(con);
	s_close(con);
}
# endif
