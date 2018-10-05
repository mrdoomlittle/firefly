# ifndef __ffly__slurry__h
# define __ffly__slurry__h
# include "../ffint.h"
# include "proto.h"
# ifndef __slurry_client
# include "window.h"
# include <X11/Xlib.h>
# include <GL/glx.h>
# endif

#define EVBS 20
# include "event.h"

# ifndef __slurry_client
struct s_window {
	struct s_window *next;
	ff_u8_t *frame_buff;
	Display *d;
	ff_uint_t width, height;
	Window w;
	GLXContext glx_ct;
};
# endif

typedef struct s_tape {
	void *text;
	ff_uint_t len;
} *s_tapep;

typedef struct s_buff {
	void *data;
	ff_uint_t len;
} *s_buffp;

void(*s_io_send)(void*, ff_uint_t, long long);
void(*s_io_recv)(void*, ff_uint_t, long long);

s_tapep s_tape_new(ff_uint_t);
void s_tape_destroy(s_tapep);

# ifndef __slurry_client
void sse_open(void);
void sse_run(void);
void sse_close(void);

# endif
# endif /*__ffly__slurry__h*/
