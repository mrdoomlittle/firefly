# include "draw_pixmap.hpp"
mdl::firefly::types::err_t mdl::firefly::graphics::cpu_draw_pixmap(uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, types::pixmap_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen) {
	uint_t xaxis_point = 0, yaxis_point = 0;
	while (yaxis_point != __pm_ylen) {
		while (xaxis_point != __pm_xlen)
		{
			mdl::uint_t pixbuff_point = ((xaxis_point + __xfs) + (yaxis_point * __pb_xlen)) * 4;
			mdl::uint_t pixmap_point = (xaxis_point + (yaxis_point * __pm_xlen)) * 4;

			types::byte_t alpha = __pixmap[pixmap_point + 3];
			types::byte_t inv_alpha = 255 - __pixmap[pixmap_point + 3];

			if (__pixmap[pixmap_point + 3] != 0x0) {
				types::byte_t new_r = (types::byte_t)((alpha * __pixmap[pixmap_point] + inv_alpha * __pixbuff[pixbuff_point]) >> 8);
				types::byte_t new_g = (types::byte_t)((alpha * __pixmap[pixmap_point + 1] + inv_alpha * __pixbuff[pixbuff_point + 1]) >> 8);
				types::byte_t new_b = (types::byte_t)((alpha * __pixmap[pixmap_point + 2] + inv_alpha * __pixbuff[pixbuff_point + 2]) >> 8);

				__pixbuff[pixbuff_point] = new_r;
				__pixbuff[pixbuff_point + 1] = new_g;
				__pixbuff[pixbuff_point + 2] = new_b;

				//__pixbuff[pixbuff_pos + 3] = __pixmap[pixmap_pos + 3];
			}

			xaxis_point ++;
		}
		yaxis_point ++;
	}
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::graphics::draw_pixmap(uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, types::pixmap_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, boost::uint16_t __angle) {
	static bool inited = false;
	static uint_t mx_threads;
	if (!inited) {
		mx_threads = 200;
		inited = true;
	}

	if (__pm_xlen != 320) return FFLY_SUCCESS;

	uint_t pm_block_c = 1;
	if (__pm_ylen > CU_MAX_BLOCKS) {
		pm_block_c = (uint_t)ceil((float)__pm_ylen/(float)CU_MAX_BLOCKS);
	}

	uint_t pm_tblock_c = 1;
	if (__pm_xlen > mx_threads) {
		pm_tblock_c = (uint_t)ceil((float)__pm_xlen/(float)mx_threads);
	}

	uint_t _pm_ylen = pm_block_c != 1? CU_MAX_BLOCKS : __pm_ylen, _pm_xlen = pm_tblock_c != 1? mx_threads : __pm_xlen;

	for (uint_t y = 0; y != pm_block_c; y ++) {
		if ((y + 1) * CU_MAX_BLOCKS >= __pm_ylen)
			_pm_ylen = __pm_ylen - (y * CU_MAX_BLOCKS);

		_pm_xlen = pm_tblock_c != 1? mx_threads : __pm_xlen;
		for (uint_t x = 0; x != pm_tblock_c; x ++) {
			if ((x + 1) * mx_threads >= __pm_xlen)
				_pm_xlen = __pm_xlen - (x * mx_threads);

			if (gpu_draw_pixmap(__xfs + (x * mx_threads), __yfs + (y * CU_MAX_BLOCKS), __pixbuff, __pb_xlen, __pb_ylen, x * mx_threads, __pixmap, _pm_xlen, _pm_ylen, __pm_xlen, __angle) == FFLY_FAILURE) goto cpu_draw;
			//printf("%d\n", __yfs + (y * CU_MAX_BLOCKS));
		}
		//printf("%d\n", _pm_ylen);
		__pixmap+=(_pm_ylen * __pm_xlen) * 4;
	}

	return FFLY_SUCCESS;
	cpu_draw:

	if (cpu_draw_pixmap(__xfs, __yfs, __pixbuff, __pb_xlen, __pb_ylen, __pixmap, __pm_xlen, __pm_ylen) == FFLY_FAILURE)
		return FFLY_FAILURE;
}


// code need fixing
# if defined(__USING_OPENCL) || defined(USING_OPENCL)
mdl::firefly::types::err_t mdl::firefly::graphics::draw_pixmap(uint_t __xfs, uint_t __yfs, types::pixmap_t *__pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, types::pixmap_t *__pixmap, uint_t __pm_xlen, uint_t __pm_ylen, opencl *__opencl) {
	if ((__xfs + __pm_xlen) > __pb_xlen || (__yfs + __pm_ylen) > __pb_ylen) {
		fprintf(stderr, "error: pixmap is out of bounds.\n");
		return FFLY_FAILURE;
	}

	static cl_program program;
	static cl_kernel kernel;
	static bool inited = false;
	cl_int any_error = CL_SUCCESS;

	static cl_mem __xfs = nullptr, yfs = nullptr, pixbuff = nullptr, pb_xlen = nullptr, pixmap = nullptr;
	static uint_t ___xfs = __xfs, _yfs = __yfs, _pb_xlen = __pb_xlen;


	static struct _stuff {
        ~_stuff() {
			printf("cleaning up opencl.\n");
			if (__xfs != nullptr) clReleaseMemObject(__xfs);
			if (yfs != nullptr) clReleaseMemObject(yfs);
			if (pixbuff != nullptr) clReleaseMemObject(pixbuff);
			if (pb_xlen != nullptr) clReleaseMemObject(pb_xlen);
			if (pixmap != nullptr) clReleaseMemObject(pixmap);
			clReleaseProgram(program);
			clReleaseKernel(kernel);
        }
    } stuff;

	uint_t pixmap_size = (__pm_xlen * __pm_ylen) * 4;
	uint_t pixbuff_size = (__pb_xlen * __pb_ylen) * 4;

	if (!inited) {
		if (__opencl-> load_source("../src/graphics/draw_pixmap.cl") == FFLY_FAILURE) return FFLY_FAILURE;
		if (__opencl-> build_prog() == FFLY_FAILURE) return FFLY_FAILURE;

		program = __opencl-> program;

		kernel = clCreateKernel(program, "draw_pixmap", &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create kernel for 'draw_pixmap', error code: %d\n", any_error);
			return FFLY_FAILURE;
		}

		__xfs = clCreateBuffer(__opencl-> context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint_t), &___xfs, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create buffer for '__xfs', error code: %d, from 'draw_pixmap'\n", any_error);
			return FFLY_FAILURE;
		}

		yfs = clCreateBuffer(__opencl-> context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint_t), &_yfs, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create buffer for 'yfs', error code: %d, from 'draw_pixmap'\n", any_error);
			return FFLY_FAILURE;
		}

		pixbuff = clCreateBuffer(__opencl-> context, CL_MEM_WRITE_ONLY,  pixbuff_size * sizeof(boost::uint8_t), NULL, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create 'pixbuff', error code: %d, from 'draw_pixmap'\n", any_error);
			return FFLY_FAILURE;
		}

		pb_xlen = clCreateBuffer(__opencl-> context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint_t), &_pb_xlen, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create buffer for 'pb_xlen', error code: %d, from 'draw_pixmap'\n", any_error);
			return FFLY_FAILURE;
		}

		pixmap = clCreateBuffer(__opencl-> context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,  pixmap_size * sizeof(boost::uint8_t), __pixmap, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create buffer for 'pixmap', error code: %d, from 'draw_pixmap'\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&__xfs)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to set kernel arg for '', error code: %d, from 'draw_pixmap'\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&yfs)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to set kernel arg for '', error code: %d, from 'draw_pixmap'\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&pixbuff)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to set kernel arg for '', error code: %d, from 'draw_pixmap'\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&pb_xlen)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to set kernel arg for '', error code: %d, from 'draw_pixmap'\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&pixmap)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to set kernel arg for '', error code: %d, from 'draw_pixmap'\n", any_error);
			return FFLY_FAILURE;
		}

		inited = true;
	}

	if (___xfs != __xfs) {

		___xfs = __xfs;
		if ((any_error = clEnqueueWriteBuffer(__opencl-> command_queue, __xfs, CL_TRUE, 0, sizeof(uint_t), &___xfs, 0, NULL, NULL)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to write '__xfs' to buffer, error code: %d, from 'draw_pixmap'\n", any_error);
			return FFLY_FAILURE;
		}
	}

	if (_yfs != __yfs) {

		_yfs = __yfs;
		if ((any_error = clEnqueueWriteBuffer(__opencl-> command_queue, yfs, CL_TRUE, 0, sizeof(uint_t), &_yfs, 0, NULL, NULL)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to write 'yfs' to buffer, error code: %d, from 'draw_pixmap'\n", any_error);
			return FFLY_FAILURE;
		}
	}

	if (_pb_xlen != __pb_xlen) {

		_pb_xlen = __pb_xlen;
		if ((any_error = clEnqueueWriteBuffer(__opencl-> command_queue, pb_xlen, CL_TRUE, 0, sizeof(uint_t), &_pb_xlen, 0, NULL, NULL)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to write 'pb_xlen' to buffer, error code: %d, from 'draw_pixmap'\n", any_error);
			return FFLY_FAILURE;
		}
	}

	std::size_t gwork_size[2] = {__pm_xlen, __pm_ylen};
	std::size_t lwork_size[2] = {1, 1};

	if ((any_error = clEnqueueWriteBuffer(__opencl-> command_queue, pixmap, CL_TRUE, 0, pixmap_size * sizeof(types::__pixmap_t), __pixmap, 0, NULL, NULL)) != CL_SUCCESS) {
		fprintf(stderr, "opencl: failed to write 'pixmap' to buffer, error code: %d, from 'draw_pixmap'\n", any_error);
		return FFLY_FAILURE;
	}

	if ((any_error = clEnqueueWriteBuffer(__opencl-> command_queue, pixbuff, CL_TRUE, 0, pixbuff_size * sizeof(types::__pixmap_t), __pixbuff, 0, NULL, NULL)) != CL_SUCCESS) {
		fprintf(stderr, "opencl: failed to write 'pixbuff' to buffer, error code: %d, from 'draw_pixmap'\n", any_error);
		return FFLY_FAILURE;
	}

	if ((any_error = clEnqueueNDRangeKernel(__opencl-> command_queue, kernel, 2, NULL, gwork_size, lwork_size, 0, NULL, NULL)) != CL_SUCCESS) {
		fprintf(stderr, "opencl: failed to set nd range, error code: %d, from 'draw_pixmap'\n", any_error);
		return FFLY_FAILURE;

	}

	if ((any_error = clEnqueueTask(__opencl-> command_queue, kernel, 0, NULL, NULL)) != CL_SUCCESS) {
		fprintf(stderr, "opencl: failed to queue task, error code: %d, from 'draw_pixmap'\n", any_error);
		return FFLY_FAILURE;
	}

	if ((any_error = clEnqueueReadBuffer(__opencl-> command_queue, pixbuff, CL_TRUE, 0, pixbuff_size * sizeof(types::__pixmap_t), __pixbuff, 0, NULL, NULL)) != CL_SUCCESS) {
		fprintf(stderr, "opencl: failed to read buffer. error code: %d, from 'draw_rect'\n", any_error);
		return FFLY_FAILURE;
	}

	return FFLY_SUCCESS;
}
# endif
