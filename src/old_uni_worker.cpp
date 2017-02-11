# include "tcp_client.hpp"
# include "udp_client.hpp"
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
# define FDIR 0
# define RDIR 1
int unsigned xlen = 24, ylen = 24;
int unsigned xadd = 640, xa = 0;
int unsigned yadd = 640, ya = 0;
bool dirx = FDIR, diry = FDIR;
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

void draw_somthing(mdl::uint_t __pix_count, boost::uint8_t * __pixs) {
	for (mdl::uint_t x = xa; x != xlen+xa; x ++) {
		for (mdl::uint_t y = ya; y != ylen+ya; y ++) {
			__pixs[(mdl::emu2d(x, y, WIN_XA, WIN_YA) * 4)] = 57;
			__pixs[(mdl::emu2d(x, y, WIN_XA, WIN_YA) * 4) + 1] = 120;
			__pixs[(mdl::emu2d(x, y, WIN_XA, WIN_YA) * 4) + 2] = 156;
			__pixs[(mdl::emu2d(x, y, WIN_XA, WIN_YA) * 4) + 3] = 255;
		}
	}

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
}

# include "worker_info_t.hpp"
int main() {
/*
	mdl::tcp_client tcp_client;
	mdl::udp_client udp_client;
	if (tcp_client.init("192.168.0.100", 21299)) return 1;
	tcp_client.connect();

//	if (udp_client.init("192.168.0.100", 10198)) return 1;
	worker_info_t worker_info;
	serial archive('\0');

	std::size_t size = archive.get_size(&worker_info);
	archive | 'w';
	archive.init(size);

	tcp_client.recv(archive.get_data(), size);
	printf("size: %d\n", size);
	worker_info.arc(archive);

	printf("chunk xlen: %ld, chunk ylen: %ld\n", worker_info.chunk_xlen, worker_info.chunk_ylen);
return 0;
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
		printf("opencl file failed to compile.");
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
*/
	mdl::tcp_client tcp_client;

	mdl::worker_info_t worker_info;
	mdl::serial archiver('\0');

	std::size_t size = archive.get_size(&worker_info);

	archiver | 'w';
	archiver.init(size);

	tcp_client.recv(archive.get_data(), size);

	worker_info.arc(archiver);

	mdl::uint_t pix_count = (worker_info.chunk_xlen * worker_info.chunk_ylen) * 4;
	boost::uint8_t * pixs = new boost::uint8_t[pix_count];
	memset(pixs, 100, pix_count);

	do {



	} while(true);

/*
	int key_code = 0;
	while(true) {
		//queue.enqueueReadBuffer(membuf, CL_TRUE, 0, pix_count, pixs);
		//tcp_client.read(key_code);
		switch(key_code) {
			case 113:
				xlen ++;
				printf("press\n");
			break;
			case 114:
				if (xlen != 0) xlen --;
				printf("press\n");
			break;
			case 9:
				std::free(pixs);
				std::free(pix_data);
				return 1;
			break;
		}
		if (key_code != 0x0) printf("%d\n", key_code);
		//printf("%d\n", key_code);

		//draw_somthing(pix_count, pixs);

		//udp_client.send(pixs, pix_count);
		//key_code = 0x0;
	}
*/
}
