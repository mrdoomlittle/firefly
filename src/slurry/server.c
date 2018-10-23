# include "slurry.h"
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <signal.h>
/*
	TODO:
		lock address within stack that a pointer may be stored or plate them like /memory/plate
		to avoid giving the user the pointer or dont.... as i dont realy care.
*/
static struct sockaddr_in adr;
int static sock;
void sse_open(void) {
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("error failed to create socket.\n");
		return;
	}
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = htons(INADDR_ANY);
	adr.sin_port = htons(*(ff_uint_t*)s_ov[OV_PORTN]);
	int val = 1;		
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));

	if (bind(sock, (struct sockaddr*)&adr, sizeof(struct sockaddr_in)) == -1) {
		printf("failed to bind socket to address.");
		return;
	}	
}


ff_u8_t static *p;
ff_i8_t static run;

static struct s_event evbuf[EVBS];
struct {int sock;} client;


void static sig(int __sig) {
	run = -1;
	shutdown(sock, SHUT_RDWR);
}

ff_u8_t static stack[512];
# define stackat(__ad) \
	(stack+(__ad))
void static
window_new(void) {
	printf("new window, %u, %u\n", *(ff_u16_t*)p, *(ff_u16_t*)(p+2));
	ff_u16_t dst;
	dst = *(ff_u16_t*)p;
	*(struct s_window**)stackat(dst) = __s_window_new(*(Display**)(stackat(*(ff_u16_t*)(p+2))));
}

void static
_read(void) {
	printf("read.\n");
	ff_u16_t src, n;
	src = *(ff_u16_t*)p;
	n = *(ff_u16_t*)(p+2);
	send(client.sock, stackat(src), n, 0);
}

void static
_write(void) {
	printf("write.\n");
	ff_u16_t dst, n;
	dst = *(ff_u16_t*)p;
	n = *(ff_u16_t*)(p+2);
	recv(client.sock, stackat(dst), n, 0);
}

void static
window_init(void) {
	printf("window init.\n");
	ff_u16_t wd;
	ff_u16_t width, height;
	ff_u16_t title;

	wd = *(ff_u16_t*)p;
	width = *(ff_u16_t*)(p+2);
	height = *(ff_u16_t*)(p+4);
	title = *(ff_u16_t*)(p+6);

	struct s_window *w;
	w = *(struct s_window**)stackat(wd);
	__s_window_init(w, width, height, (char const*)stackat(title));
}

void static
window_display(void) {
	printf("window display.\n");
	struct s_window *wd;

	wd = *(struct s_window**)stackat(*(ff_u16_t*)p);
	__s_window_display(wd);
}

void static
window_frame(void) {
	printf("window frame.\n");
	struct s_window *wd;

	wd = *(struct s_window**)stackat(*(ff_u16_t*)p);
	
	ff_u8_t *f;
	f = wd->frame_buff;

	s_recv(f, wd->width*wd->height*4, client.sock);
}

void static
window_destroy(void) {
	printf("window destroy.\n");
	ff_u16_t wd;
	wd = *(ff_u16_t*)p;

	struct s_window *w;
	w = *(struct s_window**)stackat(wd);
	__s_window_destroy(w);
}

void static
display_open(void) {
	printf("display open, %u\n", *(ff_u16_t*)p);
	*(Display**)stackat(*(ff_u16_t*)p) = XOpenDisplay(NULL);
}

void static
display_close(void) {
	printf("display close.\n");
	Display *d = *(Display**)stackat(*(ff_u16_t*)p);
	XSetCloseDownMode(d, DestroyAll);
	XCloseDisplay(d);
}

void static
event(void) {
	printf("event.\n");
	Display *d = *(Display**)stackat(*(ff_u16_t*)p);
	XEvent event;
	ff_u8_t *t;
	s_eventp ev;
	ev = evbuf;
_again:
	t = &ev->type;
	if (XPending(d) > 0 && ev<(evbuf+EVBS)) {
		XNextEvent(d, &event);
		switch(event.type) {
			case ClientMessage:
				*t = _s_event_msg;
			break;
			case ButtonPress: case ButtonRelease:
				*t = (event.type == ButtonPress?_s_button_press:_s_button_release);
				
				ev->button.x = event.xbutton.x;
				ev->button.y = event.xbutton.y;
			break;
			default:
				*t = _s_event_unknown;
		}
		ev++;
		goto _again;
	}
	
	ff_uint_t n;
	n = ev-evbuf;
	printf("number: %u\n", n);
	send(client.sock, &n, sizeof(ff_uint_t), 0);
	if (n>0)
		send(client.sock, evbuf, n*sizeof(struct s_event), 0);
}

void static
pending(void) {
	printf("pending.\n");
	Display *d = *(Display**)stackat(*(ff_u16_t*)p);
	ff_i8_t res;
	res = -1;
	if (XPending(d)>0) {
		res = 0;
	}
	send(client.sock, &res, 1, 0);
}

ff_i8_t static dc;
void disconnect();
__asm__("disconnect:\n\t"
		"movb $0, dc(%rip)\n\t"
		"ret");

static void(*op[])(void) = {
	window_new,
	_read,
	_write,
	window_init,
	window_destroy,
	disconnect,
	window_display,
	window_frame,
	display_open,
	display_close,
	event,
	pending
};

void ox(void *__op) {
	((void(*)(void))__op)();
}

ff_u8_t static osz[] = {
	4,		//window_new
	4,		//write
	4,		//read
	8,		//window_init
	2,		//window_destroy
	0,		//disconnect
	2,		//window_display
	2,		//window_frame
	2,		//display_open
	2,		//display_close
	2,		//event
	2		//pending
};

void texec(s_tapep __t) {
	p = (ff_u8_t*)__t->text;
	ff_u8_t *end;
	end = p+__t->len;
	ff_u8_t on;
_again:
	if (p>=end)
		return;
	on = *(p++);
	printf("operation : %u : %u\n", on, osz[on]);
	ox(op[on]);
	p+=osz[on];
	goto _again;
}

void sse_run(void) {
	struct sigaction sa;
	bzero(&sa, sizeof(struct sigaction));
	sa.sa_handler = sig;
	sigaction(SIGINT, &sa, NULL);
	struct sockaddr_in cli;
	run = 0;
	ff_uint_t len;
	s_tapep t;
	while(!run) {
		dc = -1;
		if (listen(sock, 1)<0) {
			printf("failed to listen.\n");
			break;
		}
		len = sizeof(struct sockaddr_in);
		if ((client.sock = accept(sock, (struct sockaddr*)&cli, &len)) <0) {
			printf("failed to accept.\n");
			break;
		}
		printf("client connected.\n");
		ff_uint_t tsz;
	_again:
		if (run == -1) break;
		if (recv(client.sock, &tsz, sizeof(ff_uint_t), 0)<=0) {
			printf("failed to recv.\n");
			break;
		}
		printf("got size: %u\n", tsz);
		printf("recved tape size.\n");
		t = s_tape_new(tsz);
		printf("waiting for tape text.\n");

		if (recv(client.sock, t->text, tsz, 0)<=0) {
			printf("failed to recv.\n");
			break;
		}
		printf("exec.\n");
		texec(t);
		s_tape_destroy(t);
		printf("\n\n");

		if (!dc) {
			printf("client disconnected.\n");
			close(client.sock);
		} else
			goto _again;
	}
	printf("\ngoodbye, sad to see you go :(\n");
}

void sse_close(void) {
	close(sock);
}
