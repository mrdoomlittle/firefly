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
		to avoid giving the user the pointer
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
	adr.sin_port = htons(10198);
	int val = 1;		
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));

	if (bind(sock, (struct sockaddr*)&adr, sizeof(struct sockaddr_in)) == -1) {
		printf("failed to bind socket to address.");
		return;
	}	
}


ff_u8_t static *p;
ff_i8_t static run;

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
	printf("new window.\n");
	ff_u16_t dst;
	dst = *(ff_u16_t*)p;
	*(struct s_window**)stackat(dst) = s_window_new();
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

	struct s_window *sw;
	sw = *(struct s_window**)stackat(wd);
	ffly_wd_init(&sw->fw, width, height, (char const*)stackat(title));
}

void static
window_open(void) {
	printf("window open.\n");
	ff_u16_t wd;
	wd = *(ff_u16_t*)p;
	ffly_wd_open(&(*(struct s_window**)stackat(wd))->fw);
}

void static
window_close(void) {
	printf("window close.\n");
	ff_u16_t wd;
	wd = *(ff_u16_t*)p;
	ffly_wd_close(&(*(struct s_window**)stackat(wd))->fw);
}

void static
window_display(void) {
	printf("window display.\n");
	struct s_window *wd;

	wd = *(struct s_window**)stackat(*(ff_u16_t*)p);
	ffly_wd_display(&wd->fw);
}

void static
window_frame(void) {
	printf("window frame.\n");
	struct s_window *wd;

	wd = *(struct s_window**)stackat(*(ff_u16_t*)p);
	
	ff_u8_t *f;
	f = ffly_wd_frame_buff(&wd->fw);

	ff_uint_t width, height;
	mare_get(wd->fw.m, 0x01, &width);
	mare_get(wd->fw.m, 0x02, &height);

	printf("width: %u, height: %u\n", width, height);

	ff_uint_t n, c;

	n = width*height*4;
	c = n>>14;

	ff_uint_t i;
	ff_u8_t ack;
	i = 0;
	while(i != c) {
		recv(client.sock, f, 1<<14, 0);
		send(client.sock, &ack, 1, 0);
		f+=(1<<14);
		i++;
	}

	ff_uint_t left;
	left = n-(c*(1<<14));
	if (left>0) {
		recv(client.sock, f, left, 0);
		send(client.sock, &ack, 1, 0);
	}
}

void static
window_destroy(void) {
	printf("window destroy.\n");
	ff_u16_t wd;
	wd = *(ff_u16_t*)p;

	struct s_window *sw;
	sw = *(struct s_window**)stackat(wd);

	ffly_wd_cleanup(&sw->fw);
	s_window_destroy(sw);
}

ff_i8_t static dc;
void disconnect();
__asm__("disconnect:\n\t"
		"movb $0, dc(%rip)\n\t"
		"ret");

void(*op[])(void) = {
	window_new,
	_read,
	_write,
	window_init,
	window_open,
	window_close,
	window_destroy,
	disconnect,
	window_display,
	window_frame
};

void ox(void *__op) {
	((void(*)(void))__op)();
}

ff_u8_t static osz[] = {
	2,		//window_new
	4,		//write
	4,		//read
	8,		//window_init
	2,		//window_open
	2,		//window_close
	2,		//window_destroy
	0,		//disconnect
	2,		//window_display
	2		//window_frame
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
