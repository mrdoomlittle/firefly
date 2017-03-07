# include "draw_pixmap.hpp"

# ifdef USING_OPENCL
boost::int8_t mdl::firefly::graphics::draw_pixmap(uint_t __xoffset, uint_t __yoffset, boost::uint8_t *__pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, boost::uint8_t *__pixmap, uint_t __pm_xlen, uint_t __pm_ylen, opencl *__opencl) {
	if ((__xoffset + __pm_xlen) > __pb_xlen || (__yoffset + __pm_ylen) > __pb_ylen) {
		fprintf(stderr, "error: pixmap is out of bounds.\n");
		return FFLY_FAILURE;
	}

	static cl_program program;
	static cl_kernel kernel;
	static bool initialized = false;
	cl_int any_error = CL_SUCCESS;

	static cl_mem xoffset = nullptr, yoffset = nullptr, pixbuff = nullptr, pb_xlen = nullptr, pixmap = nullptr;
	static uint_t _xoffset = __xoffset, _yoffset = __yoffset, _pb_xlen = __pb_xlen;


	static struct _stuff {
        ~_stuff() {
			printf("cleaning up opencl.\n");
			if (xoffset != nullptr) clReleaseMemObject(xoffset);
			if (yoffset != nullptr) clReleaseMemObject(yoffset);
			if (pixbuff != nullptr) clReleaseMemObject(pixbuff);
			if (pb_xlen != nullptr) clReleaseMemObject(pb_xlen);
			if (pixmap != nullptr) clReleaseMemObject(pixmap);
			clReleaseProgram(program);
			clReleaseKernel(kernel);
        }
    } stuff;

	uint_t pixmap_size = (__pm_xlen * __pm_ylen) * 4;
	uint_t pixbuff_size = (__pb_xlen * __pb_ylen) * 4;

	if (!initialized) {
		if (__opencl-> load_source("../src/graphics/draw_pixmap.cl") == FFLY_FAILURE) return FFLY_FAILURE;
		if (__opencl-> build_prog() == FFLY_FAILURE) return FFLY_FAILURE;

		program = __opencl-> program;

		kernel = clCreateKernel(program, "draw_pixmap", &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create kernel for 'draw_pixmap', error code: %d\n", any_error);
			return FFLY_FAILURE;
		}

		xoffset = clCreateBuffer(__opencl-> context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint_t), &_xoffset, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create buffer for 'xoffset', error code: %d, from 'draw_pixmap'\n", any_error);
			return FFLY_FAILURE;
		}

		yoffset = clCreateBuffer(__opencl-> context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint_t), &_yoffset, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create buffer for 'yoffset', error code: %d, from 'draw_pixmap'\n", any_error);
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

		if ((any_error = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&xoffset)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to set kernel arg for '', error code: %d, from 'draw_pixmap'\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&yoffset)) != CL_SUCCESS) {
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

		initialized = true;
	}

	if (_xoffset != __xoffset) {

		_xoffset = __xoffset;
		if ((any_error = clEnqueueWriteBuffer(__opencl-> command_queue, xoffset, CL_TRUE, 0, sizeof(uint_t), &_xoffset, 0, NULL, NULL)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to write 'xoffset' to buffer, error code: %d, from 'draw_pixmap'\n", any_error);
			return FFLY_FAILURE;
		}
	}

	if (_yoffset != __yoffset) {

		_yoffset = __yoffset;
		if ((any_error = clEnqueueWriteBuffer(__opencl-> command_queue, yoffset, CL_TRUE, 0, sizeof(uint_t), &_yoffset, 0, NULL, NULL)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to write 'yoffset' to buffer, error code: %d, from 'draw_pixmap'\n", any_error);
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

	if ((any_error = clEnqueueWriteBuffer(__opencl-> command_queue, pixmap, CL_TRUE, 0, pixmap_size * sizeof(boost::uint8_t), __pixmap, 0, NULL, NULL)) != CL_SUCCESS) {
		fprintf(stderr, "opencl: failed to write 'pixmap' to buffer, error code: %d, from 'draw_pixmap'\n", any_error);
		return FFLY_FAILURE;
	}

	if ((any_error = clEnqueueWriteBuffer(__opencl-> command_queue, pixbuff, CL_TRUE, 0, pixbuff_size * sizeof(boost::uint8_t), __pixbuff, 0, NULL, NULL)) != CL_SUCCESS) {
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

	if ((any_error = clEnqueueReadBuffer(__opencl-> command_queue, pixbuff, CL_TRUE, 0, pixbuff_size * sizeof(boost::uint8_t), __pixbuff, 0, NULL, NULL)) != CL_SUCCESS) {
		fprintf(stderr, "opencl: failed to read buffer. error code: %d, from 'draw_rect'\n", any_error);
		return FFLY_FAILURE;
	}

	return FFLY_SUCCESS;
}
# endif
