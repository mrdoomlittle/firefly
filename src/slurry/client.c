# include "connection.h"
# include "o_sheet.h"
# include <unistd.h>
# include <string.h>
# define tape(__conn, __code, __len) \
{	\
	ff_uint_t len; \
	len = __len; \
	send(__conn->sock, &len, sizeof(ff_uint_t), 0); \
	send(__conn->sock, __code, __len, 0);\
}

ff_u16_t static stack;
ff_u16_t s_window_new(s_connp __conn) {
	ff_u16_t w;
	w = stack;
	stack+=8;

	ff_u8_t code[3];

	*code = _op_window_new;
	*(ff_u16_t*)(code+1) = w;
	tape(__conn, code, 3);
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

void s_window_open(s_connp __conn, ff_u16_t __wd) {
	ff_u8_t code[3];
	*code = _op_window_open;
	*(ff_u16_t*)(code+1) = __wd;
	tape(__conn, code, 3);
}

void s_window_close(s_connp __conn, ff_u16_t __wd) {
	ff_u8_t code[3];
	*code = _op_window_close;
	*(ff_u16_t*)(code+1) = __wd;
	tape(__conn, code, 3);
}

void s_window_destroy(s_connp __conn, ff_u16_t __wd) {
	ff_u8_t code[3];
	*code = _op_window_destroy;
	*(ff_u16_t*)(code+1) = __wd;
	tape(__conn, code, 3);
}

ff_u16_t rtn(ff_uint_t __sz) {
	ff_u16_t r;
	r = stack;
	stack+=__sz;
}

int main() {
	stack = 0;
	s_connp con;
	con = s_open();
	s_connect(con, 21299, "127.0.0.1");
	ff_u16_t wd;
	wd = s_window_new(con);
	char const *tstr = "MEH";

	ff_u16_t title;
	title = rtn(strlen(tstr)+1);
	s_write(con, title, strlen(tstr)+1, tstr);
	s_window_init(con, wd, 600, 600, title);
	s_window_open(con, wd);

	usleep(5000000);
	s_window_close(con, wd);
	s_window_destroy(con, wd);
	shutdown(con->sock, SHUT_RDWR);	
	s_close(con);
}
