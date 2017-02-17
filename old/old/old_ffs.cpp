# include "tcp_server.hpp"
# include "udp_server.hpp"
# define CL_USE_DEPRECATED_OPENCL_1_2_APIS
# include <CL/cl.hpp>
# include <vector>
# include <iostream>
# include <fstream>
# include <string>
# include <cstdio>
# include <emu2d.hpp>
# define WIN_XA 640 
# define WIN_YA 640
# include <png.h>
int img_width, img_height;
png_byte color_type;
png_byte bit_depth;
boost::uint8_t *pix_data;


boost::uint8_t read_png_file(char const * __filename) {
	FILE *file = fopen(__filename, "rb");

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) return 1;

	png_infop info = png_create_info_struct(png);
	if (!info) return 1;

	if(setjmp(png_jmpbuf(png))) return 1;

	png_init_io(png, file);
	png_read_info(png, info);

	img_width = png_get_image_width(png, info);
	img_height = png_get_image_height(png, info);
	color_type = png_get_color_type(png, info);
	bit_depth = png_get_bit_depth(png, info);

	pix_data = static_cast<boost::uint8_t *>(malloc((img_width * img_height) * 4));
	memset(pix_data, 0x0, ((img_width * img_height) * 4));
	for (std::size_t y = 0; y != img_height; y ++)
		png_read_row(png, pix_data + (y * (img_width * 4)), NULL);

	fclose(file);

	return 0;
}

# include <math.h>
# define PI 3.14159265359

double long round_no(double long __no) {
	return roundf(__no);
}

void rotate_point(double long& __xaxis, double long& __yaxis, mdl::uint_t __angle) {
	double long angle = (PI / 180.0) * (double long)__angle;
	double long sn = sinf(angle), cs = cosf(angle);

	double long n_xaxis = __xaxis * cs - __yaxis * sn;
	double long n_yaxis = __yaxis * cs + __xaxis * sn;

	__xaxis = round_no(n_xaxis);
	__yaxis = round_no(n_yaxis);
}

mdl::uint_t angle_of_rotation = 0;

mdl::uint_t xlen = 188, ylen = 50;
mdl::uint_t xoffset = 266, yoffset = 266;
mdl::uint_t buff_xlen = (xoffset + xlen), buff_ylen = (yoffset + ylen);

void draw_somthing(mdl::uint_t __pix_count, boost::uint8_t * __pixs) {
	for (mdl::uint_t y = (yoffset + ylen) - 1; y != yoffset - 1; y --) {
		for (mdl::uint_t x = (xoffset + xlen) - 1; x != xoffset - 1; x --) {
			double long n_xaxis = (double long)(x - xoffset), n_yaxis = (double long)(y - yoffset);

			rotate_point(n_xaxis, n_yaxis, angle_of_rotation);

			if (n_xaxis < 0.0) {
				n_xaxis -= n_xaxis * 2;
				n_xaxis = (double long)xoffset - n_xaxis;
			} else {
				n_xaxis = (double long)xoffset + n_xaxis;
			}

			if (n_yaxis < 0.0) {
				n_yaxis -= n_yaxis * 2;
				n_yaxis = (double long)yoffset - n_yaxis;
			} else {
				n_yaxis = (double long)yoffset + n_yaxis;
			}

			mdl::uint_t sx = (int unsigned)n_xaxis, sy = (int unsigned)n_yaxis;

			__pixs[(mdl::emu2d(sx, sy, WIN_XA, WIN_YA) * 4)] = 57;
			__pixs[(mdl::emu2d(sx, sy, WIN_XA, WIN_YA) * 4) + 1] = 120;
			__pixs[(mdl::emu2d(sx, sy, WIN_XA, WIN_YA) * 4) + 2] = 156;
			__pixs[(mdl::emu2d(sx, sy, WIN_XA, WIN_YA) * 4) + 3] = 255;
		}
	}
}

/*
# include <math.h>
double round_(double __no) {
	return round(__no);
}

# define PI 3.141592653589793
void rotate_point(double& __xaxis, double& __yaxis, mdl::uint_t __angle) {
	double angle = (PI / 180.0) * (double)__angle;
	double sn = sinf(angle), cs = cosf(angle);

	double n_xaxis = __xaxis * cs - __yaxis * sn;
	double n_yaxis = __xaxis * sn + __yaxis * cs;

	__xaxis = round_(n_xaxis);
	__yaxis = round_(n_yaxis);
}

int unsigned rotation = 0;
mdl::uint_t xlen = 188, ylen = 188;
mdl::uint_t xoffset = 266, yoffset = 266;
void draw_somthing(mdl::uint_t __pix_count, boost::uint8_t * __pixs) {
	for (mdl::uint_t y = (yoffset + ylen) - 1; y != yoffset - 1; y --) {

		for (mdl::uint_t x = (xoffset + xlen) - 1; x != xoffset - 1; x --) {
			// this is ok.
			mdl::uint_t _x = x - xoffset, _y = y - yoffset;

			double xaxis = (double)_x, yaxis = (double)_y;

			mdl::uint_t sx = 0, sy = 0;

			rotate_point(xaxis, yaxis, rotation);

			if (xaxis < 0.0) {
				xaxis -= xaxis * 2;
				xaxis = ((double)xoffset) - xaxis;
			} else {
				xaxis = ((double)xoffset) + xaxis;
			}

			if (yaxis < 0.0) {
				yaxis -= yaxis * 2;
				yaxis = ((double)yoffset) - yaxis;
			} else {
				yaxis = ((double)yoffset) + yaxis;
			}

			sx = (int)xaxis;
			sy = (int)yaxis;

			//printf("X: %lf, Y: %lf\n", xaxis, yaxis);
			//printf("X: %d, Y: %d : oX: %d, oY: %d\n", (int)xaxis, (int)yaxis, x - xoffset, y - yoffset);
			if (sx >= 630 || sx < 0 || sy >= 630 || sy < 0) { 
				printf("fatil placment error.\n");
				continue;
			}

			if (sx > 30) {
				__pixs[(mdl::emu2d(sx, sy, WIN_XA, WIN_YA) * 4)] = 57;
				__pixs[(mdl::emu2d(sx, sy, WIN_XA, WIN_YA) * 4) + 1] = 120;
				__pixs[(mdl::emu2d(sx, sy, WIN_XA, WIN_YA) * 4) + 2] = 156;
				__pixs[(mdl::emu2d(sx, sy, WIN_XA, WIN_YA) * 4) + 3] = 255;
			} else {
				__pixs[(mdl::emu2d(sx, sy, WIN_XA, WIN_YA) * 4)] = 156;
				__pixs[(mdl::emu2d(sx, sy, WIN_XA, WIN_YA) * 4) + 1] = 0;
				__pixs[(mdl::emu2d(sx, sy, WIN_XA, WIN_YA) * 4) + 2] = 0;
				__pixs[(mdl::emu2d(sx, sy, WIN_XA, WIN_YA) * 4) + 3] = 255;
			}
		}

	}
*/
/*
	if (dirx == FDIR) {
		if (xa == (xadd - xlen) - 1)
			dirx = RDIR;
		else xa ++;
	}

	if (dirx == RDIR) {
		if (xa == 0)
			dirx = FDIR;
		else xa --;
	}

	if (diry == FDIR) {
		if (ya == (yadd - ylen) - 1)
			diry = RDIR;
		else ya ++;
	}

	if (diry == RDIR) {
		if (ya == 0)
			diry = FDIR;
		else ya --;
	}
*/


# include "worker_info_t.hpp"
int main() {

	mdl::tcp_server tcp_server;
	mdl::udp_server udp_server;
	if (tcp_server.init(21299) == -1) return -1;
	if (udp_server.init(10198) == -1) return -1;

	mdl::uint_t pix_count = (WIN_XA * WIN_YA) * 4;
	boost::uint8_t * pixs = new boost::uint8_t[pix_count];
	memset(pixs, 100, pix_count);

	cl_int is_error = false;
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
 
	auto platform = platforms.front();

	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

	auto device = devices.front();

	std::ifstream amd("amd.cl");

	std::string src(std::istreambuf_iterator<char>(amd), (std::istreambuf_iterator<char>()));

	cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));
	cl::Context context(device);
	cl::Program program(context, sources);

	is_error = program.build("-cl-std=CL1.2");
	if (is_error != 0) { 
		printf("opencl file failed to compile.\n");
		return 1;
	}

	if (read_png_file("background.png")) {
		std::free(pixs);
		return 1;
	}

	printf("init.\n");

	cl::Buffer membuf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, pix_count);
	cl::Buffer background(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, pix_count, pix_data);
	cl::Kernel kernel(program, "reset_background", &is_error);
	kernel.setArg(0, membuf);
	kernel.setArg(1, background);

	cl::CommandQueue queue(context, device);
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(pix_count));
/*
	mdl::uint_t pix_count = (worker_info.chunk_xlen * worker_info.chunk_ylen) * 4;
	boost::uint8_t * pixs = new boost::uint8_t[pix_count];
	memset(pixs, 100, pix_count);
*/
	bool cl = false, conn_client = false;
	int key_code = 0, sock;
	
	while(true) {
		boost::int8_t sock_state;
		if (!conn_client) {
			printf("waiting for player/s to connect.\n");
			tcp_server.accept(sock);
			printf("player has connected to the server.\n");
			conn_client = true;
		}

		queue.enqueueReadBuffer(membuf, CL_TRUE, 0, pix_count, pixs);
		sock_state = tcp_server.recv(sock, key_code);
		if (sock_state == -1 || sock_state == 0) {
			printf("error sending tcp packet.\n");
			tcp_server.close(sock);
			conn_client = false;
		}

		switch(key_code) {
			case 113:
				if (angle_of_rotation < 360 ) {angle_of_rotation += 45; cl = true;} else {angle_of_rotation = 0; }
				printf("-------------> press rt %d\n", angle_of_rotation);
			break;
			case 114:
				if (angle_of_rotation > 0) { angle_of_rotation -= 45; cl = true;} else { angle_of_rotation = 360; }
				printf("-------------> press rt %d\n", angle_of_rotation);
			break;
			case 9:
				std::free(pixs);
				std::free(pix_data);
				return 1;
			break;
			default:
			cl = false;
			break;
		}
		if (key_code != 0x0) printf("%d\n", key_code);
		//printf("%d\n", key_code);

		draw_somthing(pix_count, pixs);

		sock_state = udp_server.send(pixs, pix_count);
		if (sock_state == -1 || sock_state == 0) {
			printf("error sending udp packets.\n");
		}

		key_code = 0x0;
	}
}
