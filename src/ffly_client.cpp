# include <X11/Xlib.h>
# include <GL/gl.h>
# include <GL/glx.h>
# include <GL/glu.h>
# include <cstdio>
# include <boost/cstdint.hpp>
# include <string.h>
# define WX_LEN 664
# define WY_LEN 664

float button_rcolour = 1.0;

void draw() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1., 1., -1., 1., 1., 20.);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);
    glBegin(GL_QUADS);
        glColor3f(1., 0., 0.); glVertex3f(-.75, -.75, 0.);
        glColor3f(0., 1., 0.); glVertex3f( .75, -.75, 0.);
        glColor3f(0., 0., 1.); glVertex3f( .75,  .75, 0.);
        glColor3f(1., 1., 0.); glVertex3f(-.75,  .75, 0.);
    glEnd();
}

void draw_button() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glBegin(GL_QUADS);
		glColor3f(button_rcolour, 0.2, 0.3);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f( 0.5f, -0.5f);
		glVertex2f( 0.5f,  0.5f);
		glVertex2f(-0.5f,  0.5f);
	glEnd();
}

bool is_inside(int w_xpos, int w_ypos, int m_xpos, int m_ypos) {
	if ((m_xpos > w_xpos && m_xpos < (w_xpos + WX_LEN)) && (m_ypos > w_ypos && m_ypos < (w_ypos + WY_LEN))) {
		int container_x = w_xpos + (WX_LEN/4), container_y = w_ypos + (WY_LEN/4);

		int container_xlen = (WX_LEN/2) + 1, container_ylen = (WY_LEN/2) + 1;

		if ((m_xpos > container_x && m_xpos < (container_x + container_xlen)) && (m_ypos > container_y && m_ypos < (container_y + container_ylen))) {
		
			button_rcolour = 0.1;
			return true;
		}


	}

	button_rcolour = 1.0;
	return false;
}

bool is_inside_window(int w_xpos, int w_ypos, int m_xpos, int m_ypos) {
	if ((m_xpos > w_xpos && m_xpos < (w_xpos + WX_LEN)) && (m_ypos > w_ypos && m_ypos < (w_ypos + WY_LEN))) return true;
	return false;
}
# include <boost/asio.hpp>
# define PACKET_SIZE 65500
# include <iostream>
# include <math.h>
# include <chrono>
int main() {

	boost::asio::io_service io_service;
    boost::asio::ip::udp::socket socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 10198));

    boost::asio::ip::udp::resolver resolver(io_service);
    boost::asio::ip::udp::endpoint endpoint = *resolver.resolve({boost::asio::ip::udp::v4(), "192.168.0.100", "21299"});


	boost::uint8_t * pixels = static_cast<boost::uint8_t *>(malloc((WX_LEN * WY_LEN) * 4));
	memset(pixels, 0x1, (WX_LEN * WY_LEN) * 4);

	boost::uint32_t pix_count = (WX_LEN * WY_LEN) * 4;

	std::size_t packet_count = ceil((float(pix_count)/PACKET_SIZE));

	printf("pass. packets %ld.\n", packet_count);

	XVisualInfo * _vis_info;

	GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};

	XSetWindowAttributes _win_att;

	Colormap _color_map;

	Display * _display;
	Window _window;
	XEvent _xevent;
	Window _rwindow;

	_display = XOpenDisplay(NULL);
	if (_display == NULL) {
		printf("display error.\n");
		return 1;
	}

	int _screen = DefaultScreen(_display);

	_rwindow = XRootWindow(_display, 0);

	_vis_info = glXChooseVisual(_display, 0, att);

	_color_map = XCreateColormap(_display, _rwindow, _vis_info-> visual, AllocNone);

	_win_att.colormap = _color_map;
	_win_att.event_mask = ExposureMask | KeyPressMask;

	_window = XCreateWindow(_display, _rwindow, 0, 0, WX_LEN, WY_LEN, 0, _vis_info-> depth, InputOutput, _vis_info-> visual, CWColormap | CWEventMask, &_win_att);

	XMapWindow(_display, _window);

	Atom WM_DELETE_WINDOW = XInternAtom(_display, "WM_DELETE_WINDOW", false);

	XSetWMProtocols(_display, _window, &WM_DELETE_WINDOW, 1);

	XStoreName(_display, _window, "FireFly Client");

	XSizeHints _size_hints;

	_size_hints.width = _size_hints.min_width = _size_hints.max_width = WX_LEN;
	_size_hints.height = _size_hints.min_height = _size_hints.max_height = WY_LEN;

	_size_hints.flags = PSize | PMinSize | PMaxSize | PPosition;

	XSetWMNormalHints(_display, _window, &_size_hints);

	GLXContext _glx_context = glXCreateContext(_display, _vis_info, NULL, GL_TRUE);
	glXMakeCurrent(_display, _window, _glx_context);

	int unsigned screen_count = XScreenCount(_display);

	Window * r_windows = static_cast<Window *>(malloc(screen_count * sizeof(Window)));

	for (std::size_t i = 0; i != screen_count; i ++) {
		r_windows[i] = XRootWindow(_display, i);
	}

	int w_xpos, w_ypos, m_xpos, m_ypos, r_mxpos, r_mypos;
	int unsigned w_width, w_height, w_border_width, w_depth, m_mask;
	Window child, root = XRootWindow(_display, 0);
	boost::uint8_t n = 0;
	int unsigned tick_count = 0;

    socket.set_option(boost::asio::socket_base::send_buffer_size(60 * PACKET_SIZE));
    socket.set_option(boost::asio::socket_base::receive_buffer_size(60 * pix_count));

	auto begin = std::chrono::high_resolution_clock::now();
	while(true) {
//		socket.send_to(boost::asio::buffer(&n, sizeof(boost::uint8_t)), endpoint);
		std::size_t amount_to_recv = pix_count;
		std::size_t o = 0;

		while (o != packet_count) {
			socket.send_to(boost::asio::buffer(&n, sizeof(boost::uint8_t)), endpoint);
			std::size_t bytes_to_recv = PACKET_SIZE;

			if (amount_to_recv < PACKET_SIZE)
				bytes_to_recv = amount_to_recv;

//			printf("recv packet of data. %ld\n", bytes_to_recv);
			socket.receive_from(boost::asio::buffer((pixels + (o * PACKET_SIZE)), bytes_to_recv * sizeof(boost::uint8_t)), endpoint);
			amount_to_recv -= PACKET_SIZE;
			o ++;
		}

		//XGetGeometry(_display, _window, &root, &w_xpos, &w_ypos, &w_width, &w_height, &w_border_width, &w_depth);
		XTranslateCoordinates(_display, _window, root, 0, 0, &w_xpos, &w_ypos, &child);
		std::size_t i = 0;
		while(i != screen_count) {
			bool result = XQueryPointer(_display, r_windows[i], &_rwindow, &child, &r_mxpos, &r_mypos, &m_xpos, &m_ypos, &m_mask);
			if (result) break;
			i ++;
		}

//		printf("window X: %d, window Y: %d, mouse X: %d, mouse Y: %d\n", w_xpos, w_ypos, m_xpos, m_ypos);

		while (XPending(_display) > 0) {
			XNextEvent(_display, &_xevent);
			if (_xevent.type == KeyPress || _xevent.type == ClientMessage) goto end;
		}

		glDrawPixels(WX_LEN, WY_LEN , GL_RGBA, GL_BYTE, pixels);

		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(now - begin);
		std::chrono::milliseconds time_span = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
		if (time_span.count() >= 1000) {
			std::cout << "FPS: " << tick_count << std::endl;
			tick_count = 0;
			begin = std::chrono::high_resolution_clock::now();
		} else {
			tick_count ++;
		}

		glXSwapBuffers(_display, _window);
	}

	end:

	glXMakeCurrent(_display, None, NULL);
	glXDestroyContext(_display, _glx_context);
	XDestroyWindow(_display, _window);
	XCloseDisplay(_display);
	std::free(r_windows);

	return 0;
}
