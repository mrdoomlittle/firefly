# include "ffly_server.hpp"
boost::int8_t mdl::ffly_server::init() {
	this-> uni_particles = static_cast<firefly::types::uni_par_t *>(calloc(this-> uni_particle_count, sizeof(firefly::types::uni_par_t)));
	this-> uni_par_colours = static_cast<boost::uint8_t *>(malloc((this-> uni_particle_count * 4) * sizeof(boost::uint8_t)));
	memset(uni_par_colours, 255, (this-> uni_particle_count * 4) * sizeof(boost::uint8_t));
	uint_t curr_point = 0, offset = 4;

	printf("please wait this might take some time. :)\n");
	while (curr_point != this-> uni_particle_count) {
		uint_t begin_point = (curr_point * offset);

		this-> uni_particles[curr_point].colour.r = &uni_par_colours[begin_point];
		this-> uni_particles[curr_point].colour.g = &uni_par_colours[begin_point + 1];
		this-> uni_particles[curr_point].colour.b = &uni_par_colours[begin_point + 2];
		this-> uni_particles[curr_point].colour.a = &uni_par_colours[begin_point + 3];

		curr_point ++;
	}

	if (this-> opencl.init() == -1) return -1;
}

void mdl::ffly_server::uni_wmanager() {
	do {

	} while(true);
}

void mdl::ffly_server::client_handler(int __sock, uint_t __player_id) {
	firefly::types::player_info_t& player_info = this-> player_index[__player_id];

	printf("client thread has begin.\n");
	uint_t cam_xlen = 256, cam_ylen = 256;

	uint_t cam_pix_count = (cam_xlen * cam_ylen) * 4;

	boost::uint8_t *player_camera = static_cast<boost::uint8_t *>(malloc(cam_pix_count * sizeof(boost::uint8_t)));
	memset(player_camera, 100, cam_pix_count);
	serial serialize('\0');

	std::size_t ss = serialize.get_size(&player_info);
	serialize.init(ss);

	serialize | 'w';

	player_info.key_code = 0x0;

	player_info.xaxis = 0;
	player_info.yaxis = 0;
	player_info.zaxis = 0;

	cl_int any_error;
	cl_ulong offsets[2] = {0, 0};
	cl_ulong _uni_xlen[1] = {this-> uni_xlen};

	cl_mem offsets_buff = clCreateBuffer(this-> opencl.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 2 * sizeof(cl_ulong), offsets, &any_error);
	if (any_error != CL_SUCCESS) {
		return;
	}

	cl_mem uni_xlen_buff = clCreateBuffer(this-> opencl.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 1 * sizeof(cl_ulong), _uni_xlen, &any_error);
	if (any_error != CL_SUCCESS) {
		return;
	}

	cl_mem uni_buff = clCreateBuffer(this-> opencl.context, CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, (this-> uni_particle_count * 4) * sizeof(boost::uint8_t), this-> uni_par_colours, &any_error);
	if (any_error != CL_SUCCESS) {
		fprintf(stderr, "error unibuff.\n");
		return;
	}

	cl_mem cam_buff = clCreateBuffer(this-> opencl.context, CL_MEM_WRITE_ONLY, cam_pix_count * sizeof(boost::uint8_t), NULL, &any_error);
	if (any_error != CL_SUCCESS) {
		fprintf(stderr, "error cambuff, error code: %d\n", any_error);
		return;
	}

	cl_kernel cam_kernel = clCreateKernel(this-> opencl.program, "render_camera", &any_error);
	if (any_error != CL_SUCCESS) {
		fprintf(stderr, "failed to create kernel, error code: %d\n", any_error);
		return;
	}

	any_error = clSetKernelArg(cam_kernel, 0, sizeof(cl_mem), (void *)&uni_buff);
	any_error = clSetKernelArg(cam_kernel, 1, sizeof(cl_mem), (void *)&cam_buff);
	any_error = clSetKernelArg(cam_kernel, 2, sizeof(cl_mem), (void *)&offsets_buff);
	any_error = clSetKernelArg(cam_kernel, 3, sizeof(cl_mem), (void *)&uni_xlen_buff);

	if (any_error != CL_SUCCESS) {
		fprintf(stderr, "failed to set kernel args.\n");
		return;
	}

	std::size_t global_work_size[2] = {cam_xlen, cam_ylen};
	std::size_t local_work_size[2] = {4, 4};

//	any_error = clEnqueueNDRangeKernel(this-> opencl.command_queue, cam_kernel, 2, NULL, global_work_size, local_work_size, 0, NULL, NULL);
//	if (any_error != CL_SUCCESS) {
//		fprintf(stderr, "'clEnqueueNDRangeKernel' failed, error code: %d", any_error);
//		return;
//	}

	do {

for (std::size_t y = player_info.yaxis; y != player_info.yaxis + 128; y ++) {
for (std::size_t x = player_info.xaxis; x != player_info.xaxis + 128; x ++) {
int unsigned pos = (x + (y * this-> uni_xlen)) * 4;
this-> uni_par_colours[pos] = 255;
this-> uni_par_colours[pos + 1] = 0;
this-> uni_par_colours[pos + 2] = 0;
this-> uni_par_colours[pos + 3] = 255;
}
}


		offsets[0] = player_info.xaxis;
		offsets[1] = player_info.yaxis;

	any_error = clEnqueueWriteBuffer(this-> opencl.command_queue, uni_buff, CL_TRUE, 0, (this-> uni_particle_count * 4) * sizeof(boost::uint8_t), this-> uni_par_colours, 0, NULL, NULL);

	any_error = clEnqueueNDRangeKernel(this-> opencl.command_queue, cam_kernel, 2, NULL, global_work_size, local_work_size, 0, NULL, NULL);
	if (any_error != CL_SUCCESS) {
		fprintf(stderr, "'clEnqueueNDRangeKernel' failed, error code: %d", any_error);
		return;
	}


	any_error = clEnqueueTask(this-> opencl.command_queue, cam_kernel, 0, NULL, NULL);
	if (any_error != CL_SUCCESS) {
		fprintf(stderr, "cl task failed, error code: %d\n", any_error);
		return;
	}

	any_error = clEnqueueReadBuffer(this-> opencl.command_queue, cam_buff, CL_TRUE, 0, cam_pix_count * sizeof(boost::uint8_t), player_camera, 0, NULL, NULL);
	if (any_error != CL_SUCCESS) {
		fprintf(stderr, "failed to read cam buff, error code: %d\n", any_error);
		return;
	}
		boost::int8_t sock_result;

		sock_result = this-> cl_tcp_stream.recv(__sock, serialize.get_data(), ss);
		player_info.achieve(serialize);
		serialize.reset();

		if (sock_result == -1) break;

		sock_result = this-> cl_udp_stream.send(player_camera, cam_pix_count);
		if (sock_result == -1) break;

		printf("X: %d, Y: %d, Z: %d\n", player_info.xaxis, player_info.yaxis, player_info.zaxis);
		switch(player_info.key_code) {
			case X11_LT_W:
				if (player_info.yaxis != 0)
					player_info.yaxis --;
			break;
			case X11_LT_A:
				if (player_info.xaxis != 0)
					player_info.xaxis --;
			break;
			case X11_LT_S:
				player_info.yaxis ++;
			break;
			case X11_LT_D:
				player_info.xaxis ++;
			break;
		}

		printf("%d\n", player_info.key_code);

		player_info.key_code = 0x0;
		//usleep(1000000);
	} while(true);
	printf("client thread has closed.\n");
	close(__sock);
}

boost::int8_t mdl::ffly_server::begin() {
	boost::thread * dum = nullptr;
	dum = new boost::thread(boost::bind(&ffly_server::uni_wmanager, this));

	if (this-> cl_tcp_stream.init(21299) == -1) return -1;
	if (this-> cl_udp_stream.init(10198) == -1) return -1;

	boost::thread * cl = nullptr;

	this-> opencl.load_source("../src/render_camera.cl");
	this-> opencl.build_prog();

	do {
		int sock;
		printf("waiting for player/s to connect.\n");
		this-> cl_tcp_stream.accept(sock);
		uint_t player_id = this-> add_player();
		cl = new boost::thread(boost::bind(&ffly_server::client_handler, this, sock, player_id));
		do {

		} while(1);
		printf("player has connected to server.\n\n");
	} while(true);
}
