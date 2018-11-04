# include <stdio.h>
# include <stdlib.h>
# include <malloc.h>
# include <X11/X.h>
# include <X11/Xlib.h>
# include <GL/gl.h>
# include <GL/glx.h>
# include <GL/glu.h>
# include "../../ffint.h"
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# define WIDTH 512
# define HEIGHT 512
Display *dpy;
Window	root;
GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
XVisualInfo *vis;
Colormap colour_map;
XSetWindowAttributes swa;
Window win;
GLXContext ctx;
XWindowAttributes gwa;
XEvent xev;

# define DENT_SHIFT 1
# define DENT_SIZE (1<<DENT_SHIFT)

struct dent {
	float r, g, b, a;
};

ff_uint_t nd;
struct dent *map;

# define dentat(__x, __y) \
	(map+(__x)+((__y)*(WIDTH>>DENT_SHIFT)))


void clear(void) {
	struct dent *d, *e;
	d = map;
	e = d+nd;
	while(d != e) {
		d->r = 0;
		d->g = 0;
		d->b = 0;
		d->a = 1.;
		d++;
	}
}

struct point {
	ff_int_t x, y;
};

# define MAX 86
struct array {
	struct point points[MAX];
	ff_uint_t i;

	struct array *link;
};

struct array *arr_head = NULL;

void output(void) {
	char ib[64];
	ff_uint_t len;
	printf("enter name: ");
	fflush(stdout);
	len = read(fileno(stdin), ib, 64);
	*(ib+len-1) = '\0';
	printf("name: %s\n", ib);
	printf("output.\n");
	int fd;

	char fb[64];
	len = sprintf(fb, "%s.h", ib);

	fd = open(fb, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	if (fd == -1) {
		printf("error.\n");
		return;
	}
	char buf[1024];

	len = sprintf(buf, "struct point %s_points[] = {\n", ib);
	write(fd, buf, len);

	struct array *arr;

	arr = arr_head;
	ff_uint_t i, n;
	n = 0;

	while(arr != NULL) {
		i = 0;
		struct point *p;
		while(i != arr->i) {
			p = arr->points+(i++);
			len = sprintf(buf, "\t{%u, %u},\n", p->x, p->y);
			write(fd, buf, len);
		}
		// loop back
		p = arr->points;
		len = sprintf(buf, "\t{%u, %u},\n", p->x, p->y);
		write(fd, buf, len);

		write(fd, "\n", 1);
		n+=arr->i+1;
		arr = arr->link;
	}
	len = sprintf(buf, "};//%u", n);
	write(fd, buf, len);
	close(fd);
}

void init(void) {
	map = (struct dent*)malloc((nd = ((WIDTH>>DENT_SHIFT)*(HEIGHT>>DENT_SHIFT)))*sizeof(struct dent));
	clear();
	printf("dent %u, %u\n", WIDTH>>DENT_SHIFT, HEIGHT>>DENT_SHIFT);
}

void fi(void) {
	free(map);
}

void btn_press(ff_uint_t __x, ff_uint_t __y) {
	struct dent *d;
	d = dentat(__x>>DENT_SHIFT, __y>>DENT_SHIFT);
	d->r = 1.;
	struct point *p;
	if (!arr_head) {
		printf("no array.\n");
		return;
	}
	p = arr_head->points+(arr_head->i++);
	p->x = __x>>DENT_SHIFT;
	p->y = __y>>DENT_SHIFT;
}

float in(ff_int_t __x, ff_int_t __y) {
	float r;
	r = 0.;
	ff_uint_t i;

	i = 0;
	ff_int_t t0, t1, t2, t3;
	t0 = 0;
	t1 = 0;
	t2 = 0;
	t3 = 0;
	struct point *p0, *p1;
	while(i != arr_head->i-1) {
		p0 = arr_head->points+(i++);
		p1 = arr_head->points+i;

		ff_int_t x0, y0;
		ff_int_t x, y;

		x0 = p1->x-p0->x;
		y0 = p1->y-p0->y;

		x = __x-p0->x;
		y = __y-p0->y;

		ff_int_t k0, k1;
		k0 = x*y0;
		k1 = x0*y;

		ff_int_t s;
		s = k0-k1;

		ff_int_t b0, b1, b2, b3;
		b0 = __x-p0->x;
		b1 = __x-p1->x;
		b2 = __y-p0->y;
		b3 = __y-p1->y;

		if ((b2>0 && b3<0) || (b2<0 && b3>0)) {
			t0+=s>0;
			t1+=s<0;
		}

		if ((b0>0 && b1<0) || (b0<0 && b1>0)) {
			t2+=s>0;
			t3+=s<0;
		}

	}

	if (t0-t1 > 0 && t2-t3 > 0) {
		return 1.; 
	}   
	if (t0-t1 < 0 && t2-t3 < 0) {
		return 1.; 
	}

	if (!r)
		return -1.;
	return r;
}

void drawpoints(void) {
	printf("draw points.\n");
	if (!arr_head) {
		printf("no array.\n");
		return;
	}
	if (!arr_head->i) {
		printf("no points.\n");
		return;
	}
	ff_uint_t x, y;
	struct dent *d;
	float v;
	y = 0;
	while(y != (HEIGHT>>DENT_SHIFT)) {
		x = 0;
		while(x != (WIDTH>>DENT_SHIFT)) {
			if ((v = in(x, y))>0) {
				d = dentat(x, y);
				d->b = v;
			}
			x++;
		}
		y++;
	}
}

void new(void) {
	struct array *arr;

	arr = (struct array*)malloc(sizeof(struct array));
	arr->i = 0;
	arr->link = arr_head;
	arr_head = arr;
}

void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, 0);
	struct dent *d;
	ff_uint_t x, y;
	y = 0;
	while(y != (HEIGHT>>DENT_SHIFT)) {
		x = 0;
		while(x != (WIDTH>>DENT_SHIFT)) {
			d = dentat(x, y);
			ff_uint_t dx, dy;
			dx = x*DENT_SIZE;
			dy = y*DENT_SIZE;
			glBegin(GL_QUADS);
			glColor4f(d->r, d->g, d->b, d->a);
			glVertex2i(dx, dy);
			glVertex2i(dx+DENT_SIZE, dy);
			glVertex2i(dx+DENT_SIZE, dy+DENT_SIZE);
			glVertex2i(dx, dy+DENT_SIZE);
			glEnd();
			x++;
		}
		y++;
	}
}

int main() {
	init();
	dpy = XOpenDisplay(NULL);
	root = DefaultRootWindow(dpy);

	vis = glXChooseVisual(dpy, 0, att);
 	colour_map = XCreateColormap(dpy, root, vis->visual, AllocNone);

 	swa.colormap = colour_map;
 	swa.event_mask = ExposureMask|ButtonPressMask|KeyPressMask; 
 	win = XCreateWindow(dpy, root, 0, 0, WIDTH, HEIGHT, 0, vis->depth, InputOutput, vis->visual, CWColormap|CWEventMask, &swa);

	XStoreName(dpy, win, "font");
	XMapWindow(dpy, win);

 	ctx = glXCreateContext(dpy, vis, NULL, GL_TRUE);
 	glXMakeCurrent(dpy, win, ctx);

	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WIDTH, HEIGHT, 0, -10, 10);

	while(1) {
		XNextEvent(dpy, &xev);
		if (xev.type == ButtonPress) {
			btn_press(xev.xbutton.x, xev.xbutton.y);
			printf("button pressed, %d, %d\n", xev.xbutton.x, xev.xbutton.y);
		} else if (xev.type == KeyPress) {
			ff_uint_t kc;
			kc = xev.xkey.keycode;
			printf("key: %u\n", kc);
			switch(kc) {
				case 65://space
					drawpoints();
				break;
				case 24://q
					clear();
				break;
				case 42://g
					output();
				break;
				case 57://n
					new();
				break;

			}
		} else if (xev.type == ClientMessage) {
			break;
		}

		draw();
		glXSwapBuffers(dpy, win);
	}

	glXMakeCurrent(dpy, None, NULL);
 	glXDestroyContext(dpy, ctx);
	XDestroyWindow(dpy, win);
 	XCloseDisplay(dpy);
 	fi();
}
