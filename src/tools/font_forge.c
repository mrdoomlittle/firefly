# include <stdio.h>
# include <stdlib.h>
# include <malloc.h>
# include <X11/X.h>
# include <X11/Xlib.h>
# include <GL/gl.h>
# include <GL/glx.h>
# include <GL/glu.h>
# include "../ffint.h"
# include <string.h>
# include <unistd.h>
# include <fcntl.h>

#define DENT_SHIFT 6
#define DENT_SIZE (1<<DENT_SHIFT)
#define WIDTH DENT_SIZE*8
#define HEIGHT DENT_SIZE*8

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

struct dent {
	ff_u8_t bit;
	float r, g, b, a;
};

ff_uint_t nd;
struct dent *map;

# define dentat(__x, __y) \
	(map+(__x)+((__y)*(WIDTH>>DENT_SHIFT)))

void btn_press(ff_uint_t __x, ff_uint_t __y) {
	struct dent *d;
	d = dentat(__x>>DENT_SHIFT, __y>>DENT_SHIFT);
	if (d->r == 1.)
		d->r = 0;
	else
		d->r = 1.;


	d->bit ^= 0x01;
}

void gen(void) {
	struct dent *d, *e;
	d = map;
	e = d+nd;
	ff_u64_t out = 0;
	while(d != e) {
		out = out<<1|((ff_u64_t)d->bit);
		d++;
	}
	printf("out : %lx\n", out);
}

void clear(void) {
	struct dent *d, *e;
	d = map;
	e = d+nd;
	while(d != e) {
		d->r = 0;
		d->g = 0;
		d->b = 0;
		d->a = 1.;
		d->bit = 0x00;
		d++;
	}
}

void init(void) {
	map = (struct dent*)malloc((nd = (8*8))*sizeof(struct dent));
	clear();
}

void fi(void) {
	free(map);
}

void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, 0);
	struct dent *d;
	ff_uint_t x, y;
	y = 0;
	while(y != 8) {
		x = 0;
		while(x != 8) {
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
					gen();			
				break;
				case 24://q
					clear();
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
