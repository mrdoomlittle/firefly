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
# define FDIR 0
# define RDIR 1
int unsigned xlen = 24, ylen = 24;
int unsigned xadd = 640, xa = 0;
int unsigned yadd = 640, ya = 0;
bool dirx = FDIR, diry = FDIR;

void draw_somthing(mdl::uint_t __pix_count, boost::uint8_t * __pixs) {
	for (mdl::uint_t x = xa; x != xlen+xa; x ++) {
		for (mdl::uint_t y = ya; y != ylen+ya; y ++) {
			__pixs[(mdl::emu2d(x, y, WIN_XA, WIN_YA) * 4)] = 63;
			__pixs[(mdl::emu2d(x, y, WIN_XA, WIN_YA) * 4) + 1] = 121;
			__pixs[(mdl::emu2d(x, y, WIN_XA, WIN_YA) * 4) + 2] = 191;
			__pixs[(mdl::emu2d(x, y, WIN_XA, WIN_YA) * 4) + 3] = 1;
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

int main() {
	mdl::tcp_server tcp_server;
	mdl::udp_server udp_server;
	tcp_server.init(21298);
	if (udp_server.init(21299)) return 0;
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

	cl::Buffer membuf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, pix_count);
	//cl::Buffer xy_spacing(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, 2);
	cl::Kernel kernel(program, "reset_background", &is_error);
	kernel.setArg(0, membuf);

	cl::CommandQueue queue(context, device);
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(pix_count));

	int key_code = 0;
	while(true) {
		queue.enqueueReadBuffer(membuf, CL_TRUE, 0, pix_count, pixs);
		tcp_server.read(key_code);
		switch(key_code) {
			case 113:
				xlen ++;
				printf("press\n");
			break;
			case 114:
				if (xlen != 0) xlen --;
				printf("press\n");
			break;
		}
		if (key_code != 0x0) printf("%d\n", key_code);
		//printf("%d\n", key_code);

		draw_somthing(pix_count, pixs);

		udp_server.send(pixs, pix_count);
		key_code = 0x0;
	}
}
