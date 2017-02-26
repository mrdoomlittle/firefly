# ifndef __graphics__draw__rect__hpp
# define __graphics__draw__rect__hpp
# include <boost/cstdint.hpp>
# include <eint_t.hpp>
# include <emu2d.hpp>
# include "../types/colour_t.hpp"

# ifdef USING_OPENCL
	# include <CL/cl.hpp>
	# include "../opencl_helper.hpp"
# endif

# include "../types/status.hpp"
namespace mdl {
namespace firefly {
namespace graphics {
# ifdef USING_OPENCL
boost::int8_t draw_rect(boost::uint8_t *__pixbuff, uint_t __b_xcoord, uint_t __b_ycoord, uint_t __e_xcoord, uint_t __e_ycoord, colour_t __colour, uint_t __pb_xlen, uint_t __pb_ylen, opencl *__opencl) {
	if (__e_xcoord > __pb_xlen || __e_ycoord > __pb_ylen || __e_xcoord < __b_xcoord || __e_ycoord < __b_ycoord) {
		fprintf(stderr, "error: somthing is wong with the 'draw_rect' arguments.\n");
		return FFLY_FAILURE;
	}

	static bool initialized = false;
	static cl_program program;
	static cl_kernel kernel;

	static cl_mem pixbuff, b_xcoord, b_ycoord, e_xcoord, e_ycoord, colour, pb_xlen;
	static uint_t _b_xcoord = __b_xcoord, _b_ycoord = __b_ycoord, _e_xcoord = __e_xcoord, _e_ycoord = __e_ycoord, _pb_xlen = __pb_xlen;
	static colour_t _colour = __colour;

	uint_t pixbuff_size = (__pb_xlen * __pb_ylen) * 4;
	cl_int any_error = CL_SUCCESS;

	if (!initialized) {
		if (__opencl-> load_source("../src/graphics/draw_rect.cl") == FFLY_FAILURE) return FFLY_FAILURE;
		if (__opencl-> build_prog() == FFLY_FAILURE) return FFLY_FAILURE;

		program = __opencl-> program;

		kernel = clCreateKernel(program, "draw_rect", &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create kernel for 'draw_rect', error code: %d\n", any_error);
			return FFLY_FAILURE;
		}

		pixbuff = clCreateBuffer(__opencl-> context, CL_MEM_WRITE_ONLY,  pixbuff_size * sizeof(boost::uint8_t), NULL, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create 'pixbuff', error code: %d, from 'draw_rect'\n", any_error);
			return FFLY_FAILURE;
		}

		b_xcoord = clCreateBuffer(__opencl-> context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint_t), &_b_xcoord, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create buffer for 'b_xcoord', error code: %d, from 'draw_rect'\n", any_error);
			return FFLY_FAILURE;
		}

		b_ycoord = clCreateBuffer(__opencl-> context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint_t), &_b_ycoord, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create buffer for 'b_ycoord', error code: %d, from 'draw_rect'\n");
			return FFLY_FAILURE;
		}

		e_xcoord = clCreateBuffer(__opencl-> context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint_t), &_e_xcoord, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create buffer for 'e_xcoord', error code: %d, from 'draw_rect'\n");
			return FFLY_FAILURE;
		}

		e_ycoord = clCreateBuffer(__opencl-> context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint_t), &_e_ycoord, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create buffer for 'e_ycoord', error code: %d, from 'draw_rect'\n");
			return FFLY_FAILURE;
		}

		colour = clCreateBuffer(__opencl-> context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(colour_t), &_colour, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create buffer for 'colour', error code: %d, from 'draw_rect'\n");
			return FFLY_FAILURE;
		}

		pb_xlen = clCreateBuffer(__opencl-> context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint_t), &_pb_xlen, &any_error);
		if (any_error != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to create buffer for 'pb_xlen', error code: %d, from 'draw_rect'\n");
			return FFLY_FAILURE;
		}

		if ((any_error = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&pixbuff)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to set kernel arg for 'pixbuff', error code: %d, from 'draw_rect'\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_xcoord)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to set kernel arg for 'b_xcoord', error code: %d, from 'draw_rect'\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&b_ycoord)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to set kernel arg for 'b_ycoord', error code: %d, from 'draw_rect'\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&e_xcoord)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to set kernel arg for 'e_xcoord', error code: %d, from 'draw_rect'\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&e_ycoord)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to set kernel arg for 'e_ycoord', error code: %d, from 'draw_rect'\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = clSetKernelArg(kernel, 5, sizeof(cl_mem), (void *)&colour)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to set kernel arg for 'colour', error code: %d, from 'draw_rect'\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = clSetKernelArg(kernel, 6, sizeof(cl_mem), (void *)&pb_xlen)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to set kernel arg for 'pb_xlen', error code: %d, from 'draw_rect'\n", any_error);
			return FFLY_FAILURE;
		}

		initialized = true;
	}

	if (_b_xcoord != __b_xcoord) {
		if ((any_error = clEnqueueWriteBuffer(__opencl-> command_queue, b_xcoord, CL_TRUE, 0, sizeof(uint_t), &_b_xcoord, 0, NULL, NULL)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to write 'b_xcoord' to buffer, error code: %d, from 'draw_rect'\n", any_error);
			return FFLY_FAILURE;
		}

		_b_xcoord = __b_xcoord;
	}

	if (_b_ycoord != __b_ycoord) {
		if ((any_error = clEnqueueWriteBuffer(__opencl-> command_queue, b_ycoord, CL_TRUE, 0, sizeof(uint_t), &_b_ycoord, 0, NULL, NULL)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to write 'b_ycoord' to buffer, error code: %d, from 'draw_rect'\n", any_error);
			return FFLY_FAILURE;
		}

		_b_ycoord = __b_ycoord;
	}

	if (_e_xcoord != __e_xcoord) {
		if ((any_error = clEnqueueWriteBuffer(__opencl-> command_queue, e_xcoord, CL_TRUE, 0, sizeof(uint_t), &_e_xcoord, 0, NULL, NULL)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to write 'e_xcoord' to buffer, error code: %d, from 'draw_rect'\n", any_error);
			return FFLY_FAILURE;
		}

		_e_xcoord = __e_xcoord;
	}

	if (_e_ycoord != __e_ycoord) {
		if ((any_error = clEnqueueWriteBuffer(__opencl-> command_queue, e_ycoord, CL_TRUE, 0, sizeof(uint_t), &_e_ycoord, 0, NULL, NULL)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to write 'e_ycoord' to buffer, error code: %d, from 'draw_rect'\n", any_error);
			return FFLY_FAILURE;
		}

		_e_ycoord = __e_ycoord;
	}

	if (_pb_xlen != __pb_xlen) {
		if ((any_error = clEnqueueWriteBuffer(__opencl-> command_queue, pb_xlen, CL_TRUE, 0, sizeof(uint_t), &_pb_xlen, 0, NULL, NULL)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to write 'pb_xlen' to buffer, error code: %d, from 'draw_rect'\n", any_error);
			return FFLY_FAILURE;
		}

		_pb_xlen = __pb_xlen;
	}

	if (_colour.r != __colour.r || _colour.g != __colour.g || _colour.b != __colour.b || _colour.a != __colour.a) {
		if ((any_error = clEnqueueWriteBuffer(__opencl-> command_queue, colour, CL_TRUE, 0, sizeof(colour_t), &_colour, 0, NULL, NULL)) != CL_SUCCESS) {
			fprintf(stderr, "opencl: failed to write 'colour' to buffer, error code: %d, from 'draw_rect'\n", any_error);
			return FFLY_FAILURE;
		}

		_colour = __colour;
	}

	std::size_t gwork_size[2] = {__e_xcoord - __b_xcoord, __e_ycoord - __b_ycoord};
	std::size_t lwork_size[2] = {1, 1};

	if ((any_error = clEnqueueNDRangeKernel(__opencl-> command_queue, kernel, 2, NULL, gwork_size, lwork_size, 0, NULL, NULL)) != CL_SUCCESS) {
		fprintf(stderr, "opencl: failed to set nd range, error code: %d, from 'draw_rect'\n", any_error);
		return FFLY_FAILURE;
	}

	if ((any_error = clEnqueueTask(__opencl-> command_queue, kernel, 0, NULL, NULL)) != CL_SUCCESS) {
		fprintf(stderr, "opencl: failed to queue task, error code: %d, from 'draw_rect'\n", any_error);
		return FFLY_FAILURE;
	}

	if ((any_error = clEnqueueReadBuffer(__opencl-> command_queue, pixbuff, CL_TRUE, 0, pixbuff_size * sizeof(boost::uint8_t), __pixbuff, 0, NULL, NULL)) != CL_SUCCESS) {
		fprintf(stderr, "opencl: failed to read buffer. error code: %d, from 'draw_rect'\n", any_error);
		return FFLY_FAILURE;
	}

}
# elif defined(USING_CUDA)
boost::int8_t draw_rect(boost::uint8_t *__pixbuff, uint_t __b_xcoord, uint_t __b_ycoord, uint_t __e_xcoord, uint_t __e_ycoord, colour_t __colour, uint_t __pd_xlen, uint_t __pd_ylen);
# endif
}
}
}

# endif /*__graphics__draw__rect__hpp*/
