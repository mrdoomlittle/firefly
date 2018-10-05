# ifndef __ffly__slurry__h
# define __ffly__slurry__h
# include "../ffint.h"
# include "proto.h"
# include "window.h"
# include "../graphics/window.h"
# include <X11/Xlib.h>
# include <GL/glx.h>
#define EVBS 20
struct s_window {
	struct s_window *next;
	struct ffly_wd fw;
	ff_u8_t *frame_buff;
	Display *d;
	ff_uint_t width, height;
	Window w;
	GLXContext glx_ct;
};

typedef struct s_tape {
	void *text;
	ff_uint_t len;
} *s_tapep;

typedef struct s_buff {
	void *data;
	ff_uint_t len;
} *s_buffp;

struct s_button {
	ff_int_t x, y;
};

typedef struct s_event {
	ff_u8_t type;
	struct s_button button;
} *s_eventp;

enum {
	_s_event_msg,
	_s_button_press,
	_s_button_release,
	_s_event_unknown
};

void(*s_io_send)(void*, ff_uint_t, long long);
void(*s_io_recv)(void*, ff_uint_t, long long);

s_tapep s_tape_new(ff_uint_t);
void s_tape_destroy(s_tapep);

void sse_open(void);
void sse_run(void);
void sse_close(void);
# endif /*__ffly__slurry__h*/
