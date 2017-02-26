# include "ffly_server.hpp"
# include "graphics/draw_pixmap.hpp"
boost::int8_t mdl::ffly_server::init() {
	this-> uni_particles = static_cast<firefly::types::uni_par_t *>(calloc(this-> uni_particle_count, sizeof(firefly::types::uni_par_t)));
	this-> uni_par_colours = static_cast<boost::uint8_t *>(malloc((this-> uni_particle_count * 4) * sizeof(boost::uint8_t)));
	memset(uni_par_colours, 40, (this-> uni_particle_count * 4) * sizeof(boost::uint8_t));
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

// this will be used later
void mdl::ffly_server::uni_wmanager() {
	do {

	} while(true);
}

void mdl::ffly_server::player_handler(int __sock, uint_t __player_id) {
	firefly::types::player_info_t& player_info = this-> player_index[__player_id];
	printf("player with the id of %d. thread began.\n");

	serializer serialize('\0');

	std::size_t ss = serialize.get_size(&player_info);
	serialize.init(ss);

	serialize | 'w';

	player_info.key_code = 0x0;
	player_info.xaxis = 0;
	player_info.yaxis = 0;
	player_info.zaxis = 0;

	cl_int any_error = CL_SUCCESS;

	uint_t cam_offsets[3] = {
		player_info.xaxis,
		player_info.yaxis,
		player_info.zaxis
	};

	uint_t camera_xlen = 256, camera_ylen = 256;

	// amount of bytes in the pixmap for the camera
	uint_t cam_pixmap_size = (camera_xlen * camera_ylen) * 4;

	boost::uint8_t *camera_pixmap = this-> create_pixmap(camera_xlen, camera_ylen, 4);

	cl_mem uni_buff, cam_buff, cam_offsets_buff, uni_xlen_buff;

	uint_t const real_uni_xlen = this-> uni_xlen * FFLY_UNI_PAR_XLEN;

	try {
		uni_buff = clCreateBuffer(this-> opencl.context, CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, (this-> uni_particle_count * 4) * sizeof(boost::uint8_t), this-> uni_par_colours, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create 'uni_buff', error code: %d\n", any_error);
			throw;
		}

		cam_buff = clCreateBuffer(this-> opencl.context, CL_MEM_WRITE_ONLY, cam_pixmap_size * sizeof(boost::uint8_t), NULL, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create 'cam_buff', error code: %d\n", any_error);
			throw;
		}


		cam_offsets_buff = clCreateBuffer(this-> opencl.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 3 * sizeof(uint_t), cam_offsets, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create 'cam_offsets_buff', error code: %d\n", any_error);
			throw;
		}

		uni_xlen_buff = clCreateBuffer(this-> opencl.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint_t), const_cast<uint_t *>(&real_uni_xlen), &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create 'uni_xlen_buff', error code: %d\n", any_error);
			throw;
		}
	}
	catch(...) {
		std::free(camera_pixmap);
		return;
	}

	std::size_t cam_gwork_size[2] = {camera_xlen, camera_ylen};
	std::size_t cam_lwork_size[2] = {4, 4};

	uint_t camera_coords[3] = {0, 0, 0};

	cl_kernel cam_kernel = clCreateKernel(this-> opencl.program, "render_camera", &any_error);
	if (any_error != CL_SUCCESS) {
		fprintf(stderr, "opencl: failed to create camera kernel, error code: %d\n", any_error);
		goto end;
	}

	any_error = clSetKernelArg(cam_kernel, 0, sizeof(cl_mem), (void *)&uni_buff);
	if (any_error != CL_SUCCESS) {
		fprintf(stderr, "opencl: failed to set 'uni_buff' arg, error code: %d\n", any_error);
		goto end;
	}

	any_error = clSetKernelArg(cam_kernel, 1, sizeof(cl_mem), (void *)&cam_buff);
	if (any_error != CL_SUCCESS) {
		fprintf(stderr, "opencl: failed to set 'cam_buff' arg, error code: %d\n", any_error);
		goto end;
	}

	any_error = clSetKernelArg(cam_kernel, 2, sizeof(cl_mem), (void *)&cam_offsets_buff);
	if (any_error != CL_SUCCESS) {
		fprintf(stderr, "opencl: failed to set 'cam_offsets_buff' arg, error code: %d\n", any_error);
		goto end;
	}

	any_error = clSetKernelArg(cam_kernel, 3, sizeof(cl_mem), (void *)&uni_xlen_buff);
	if (any_error != CL_SUCCESS) {
		fprintf(stderr, "opencl: failed to set 'uni_xlen' arg, error code: %d\n", any_error);
		goto end;
	}

	do {
		for (std::size_t y = player_info.yaxis; y != player_info.yaxis + 16; y ++) {
			if ((player_info.yaxis + 17) >= (this-> uni_ylen * FFLY_UNI_PAR_XLEN)) break;
			for (std::size_t x = player_info.xaxis; x != player_info.xaxis + 16; x ++) {
				uint_t point = (x + (y * (this-> uni_xlen * FFLY_UNI_PAR_XLEN))) * 4;
				this-> uni_par_colours[point] = 255;
				this-> uni_par_colours[point + 1] = 0;
				this-> uni_par_colours[point + 2] = 0;
				this-> uni_par_colours[point + 3] = 180;
			}
		}

		if (player_info.xaxis <= round(camera_xlen/2)) {
			camera_coords[0] = 0;
		} else {
			if ((player_info.xaxis + round(camera_xlen/2)) < (this-> uni_xlen * FFLY_UNI_PAR_XLEN))
				camera_coords[0] = player_info.xaxis - round(camera_xlen/2);
		}

		if (player_info.yaxis <= round(camera_ylen/2)) {
			camera_coords[1] = 0;
		} else {
			if ((player_info.yaxis + round(camera_ylen/2)) < (this-> uni_ylen * FFLY_UNI_PAR_YLEN))
				camera_coords[1] = player_info.yaxis - round(camera_ylen/2);
		}

		if (cam_offsets[0] != camera_coords[0] || cam_offsets[1] != camera_coords[1] || cam_offsets[2] != camera_coords[2]) {
			cam_offsets[0] = camera_coords[0];
			cam_offsets[1] = camera_coords[1];
			cam_offsets[2] = camera_coords[2];

			any_error = clEnqueueWriteBuffer(this-> opencl.command_queue, cam_offsets_buff, CL_TRUE, 0, 3 * sizeof(uint_t), &cam_offsets, 0, NULL, NULL);
			if (any_error != CL_SUCCESS) {
				fprintf(stderr, "opencl: failed to write 'cam_offsets_buff', error code: %d\n", any_error);
				break;
			}
		}

		any_error = clEnqueueWriteBuffer(this-> opencl.command_queue, uni_buff, CL_TRUE, 0, (this-> uni_particle_count * 4) * sizeof(boost::uint8_t), this-> uni_par_colours, 0, NULL, NULL);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to write 'uni_buff', error code: %d\n", any_error);
			break;
		}

		any_error = clEnqueueNDRangeKernel(this-> opencl.command_queue, cam_kernel, 2, NULL, cam_gwork_size, cam_lwork_size, 0, NULL, NULL);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to set nd range, error code: %d\n", any_error);
			break;
		}

		any_error = clEnqueueTask(this-> opencl.command_queue, cam_kernel, 0, NULL, NULL);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to queue task, error code: %d\n", any_error);
			break;
		}

		any_error = clEnqueueReadBuffer(this-> opencl.command_queue, cam_buff, CL_TRUE, 0, cam_pixmap_size * sizeof(boost::uint8_t), camera_pixmap, 0, NULL, NULL);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to read 'cam_buff', error code: %d\n", any_error);
			break;
		}

		boost::int8_t sock_result;
		sock_result = this-> cl_tcp_stream.recv(__sock, serialize.get_serial(), ss);

		player_info.achieve(serialize);
		serialize.reset();

		if (sock_result == -1 || sock_result == 0) break;

		sock_result = this-> cl_udp_stream.send(camera_pixmap, cam_pixmap_size);
		if (sock_result == -1 || sock_result == 0) break;

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

		// dont remove or edit
		player_info.key_code = 0x0;

	} while(true);

	end:

	std::free(camera_pixmap);

	printf("player with the id of %d. thread ended.\n");

	close(__sock);
}
# include "types/colour_t.hpp"
boost::int8_t mdl::ffly_server::begin() {
	//boost::thread * dum = nullptr;
	//dum = new boost::thread(boost::bind(&ffly_server::uni_wmanager, this));

	if (this-> cl_tcp_stream.init(21299) == -1) return -1;
	if (this-> cl_udp_stream.init(10198) == -1) return -1;

	boost::thread * cl = nullptr;

	//firefly::graphics::draw_rect(this-> uni_par_colours, 0, 0, 50, 50, colour, this-> uni_xlen * FFLY_UNI_PAR_XLEN, this-> uni_ylen * FFLY_UNI_PAR_YLEN, &this-> opencl);

	// 200x200
	boost::uint8_t *temp = (boost::uint8_t *)malloc(160000);
	memset(temp, 244, 160000);

	firefly::graphics::draw_pixmap(0, 0, this-> uni_par_colours, this-> uni_xlen * FFLY_UNI_PAR_XLEN, this-> uni_ylen * FFLY_UNI_PAR_YLEN, temp, 200, 200, &this-> opencl);

	std::free(temp);

	this-> opencl.load_source("../src/render_camera.cl");
	this-> opencl.build_prog();

	do {
		int sock;
		printf("waiting for player/s to connect.\n");
		this-> cl_tcp_stream.accept(sock);

		uint_t player_id = this-> add_player();

		cl = new boost::thread(boost::bind(&ffly_server::player_handler, this, sock, player_id));

		printf("player has connected to server.\n\n");
	} while(true);
}
