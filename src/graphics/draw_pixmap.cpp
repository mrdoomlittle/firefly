# include "draw_pixmap.hpp"
mdl::firefly::types::err_t mdl::firefly::graphics::cpu_draw_pixmap(uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, uint_t __pm_xfs, types::pixmap_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, uint_t __pm_rxlen) {
	uint_t xaxis_point = 0, yaxis_point = 0;
	while (yaxis_point != __pm_ylen) {
		xaxis_point = 0;
		while (xaxis_point != __pm_xlen)
		{
			mdl::uint_t pixbuff_point = ((xaxis_point + __xfs) + ((yaxis_point + __yfs) * __pb_xlen)) * 4;
			mdl::uint_t pixmap_point = ((xaxis_point + __pm_xfs) + (yaxis_point * __pm_rxlen)) * 4;

			types::byte_t alpha = __pixmap[pixmap_point + 3];
			types::byte_t inv_alpha = 255 - __pixmap[pixmap_point + 3];

			if (__pixmap[pixmap_point + 3] != 0x0) {
				types::byte_t new_r = (types::byte_t)((alpha * __pixmap[pixmap_point] + inv_alpha * __pixbuff[pixbuff_point]) >> 8);
				types::byte_t new_g = (types::byte_t)((alpha * __pixmap[pixmap_point + 1] + inv_alpha * __pixbuff[pixbuff_point + 1]) >> 8);
				types::byte_t new_b = (types::byte_t)((alpha * __pixmap[pixmap_point + 2] + inv_alpha * __pixbuff[pixbuff_point + 2]) >> 8);

				__pixbuff[pixbuff_point] = new_r;
				__pixbuff[pixbuff_point + 1] = new_g;
				__pixbuff[pixbuff_point + 2] = new_b;

				__pixbuff[pixbuff_point + 3] = __pixmap[pixmap_point + 3];
			}

			xaxis_point ++;
		}
		yaxis_point ++;
	}
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::graphics::draw_pixmap(types::gpu_dr_pm_mem_t& __gpu_dr_pm_mem, uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, types::pixmap_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, boost::uint16_t __angle) {
# if !defined(__GCOMPUTE_GPU) && !defined(__GCOMPUTE_CPU)
	fprintf(stderr, "draw_pixmap: warning, in order for this to work '__GCOMPUTE_GPU' or '__GCOMPUTE_CPU' needs to be defined at compile time.\n")
	return FFLY_NOP;
# endif
//	printf("%d\n", *((__pixbuff + ((__pb_xlen * __pb_ylen)*4)) - 1 ));
//	printf("%d, %d, %d, %d, %d, %d\n", __xfs, __yfs, __pb_xlen, __pb_ylen, __pm_xlen, __pm_ylen);
//return FFLY_SUCCESS;
//	__xfs = 0;
//	__yfs = 0;
//	__pm_xlen = 10;
//	__pm_ylen = 10;

//	printf("%d\n", *((__pixmap + (__pm_xlen * __pm_ylen) * 4) -1) );
//	if (__pm_xlen == 120) return FFLY_SUCCESS;
# if defined(__GCOMPUTE_GPU) && !defined(__GCOMPUTE_CPU)
	if (!ffly_graphics::inited) {
		fprintf(stderr, "draw_pixmap: failed to draw as 'gpu::inited' is false.\n");
		return FFLY_NOP;
	}
# endif

# if defined(__WITH_TASK_HANDLE) && defined(__GCOMPUTE_CPU)
	static std::atomic<uint_t> finished{0};
	static auto _cpu_draw_pixmap = [](void *__args) -> void * {
		void **args = (void **)__args;
		cpu_draw_pixmap(*(uint_t*)*args, *(uint_t*)*(args + 1), (types::pixmap_t)*(args + 2), *(uint_t*)*(args + 3), *(uint_t*)*(args + 4),
			*(uint_t*)*(args + 5), (types::pixmap_t)*(args + 6), *(uint_t*)*(args + 7), *(uint_t*)*(args + 8), *(uint_t*)*(args + 9));
		(*(std::atomic<uint_t> *)*(args + 10))++;
	};
# endif
	if (__pm_xlen + __xfs > __pb_xlen) {
		__pm_xlen -= (__pm_xlen + __xfs) - __pb_xlen;
	}
	if (__pm_ylen + __yfs > __pb_ylen)
		__pm_ylen -= (__pm_ylen + __yfs) - __pb_ylen;

# if defined(__GCOMPUTE_GPU) && defined(__GCOMPUTE_GPU)
	uint_t xblock_len = ffly_graphics::inited != true? _CPU_MX_X : gpu::mx_threads, yblock_len = ffly_graphics::inited != true? _CPU_MX_Y : gpu::mx_blocks;
# endif

# if defined(__GCOMPUTE_CPU) && !defined(__GCOMPUTE_GPU)
	uint_t xblock_len = _CPU_MX_X, yblock_len = _CPU_MX_Y;
# endif

# if !defined(__GCOMPUTE_CPU) && defined(__GCOMPUTE_GPU)
	uint_t xblock_len = gpu::mx_threads, yblock_len = gpu::mx_blocks;
# endif

	uint_t xblock_c = 1, yblock_c = 1;

	if (__pm_xlen > xblock_len)
		xblock_c = (uint_t)ceil((float)__pm_xlen / (float)xblock_len);
	if (__pm_ylen > yblock_len)
		yblock_c = (uint_t)ceil((float)__pm_ylen / (float)yblock_len);

# if defined(__WITH_TASK_HANDLE) && defined(__GCOMPUTE_CPU)
	uint_t wk_c = 0;
	typedef struct {
		void *args[11];
		uint_t xfs, yfs, pm_xlen, pm_ylen, pm_xfs;
	} arg_holder;
	arg_holder args[xblock_c*yblock_c];
# endif
	uint_t _pm_xlen = xblock_c != 1? xblock_len : __pm_xlen, _pm_ylen = yblock_c != 1? yblock_len : __pm_ylen;
	for (uint_t y = 0; y != yblock_c; y ++) {
		if ((y + 1) * yblock_len >= __pm_ylen && xblock_c != 1)
			_pm_ylen = __pm_ylen - (y * yblock_len);

		_pm_xlen = xblock_c != 1? xblock_len : __pm_xlen;
		for (uint_t x = 0; x != xblock_c; x ++) {
			if ((x + 1) * xblock_len >= __pm_xlen && yblock_c != 1)
				_pm_xlen = __pm_xlen - (x * xblock_len);
# ifdef __GCOMPUTE_GPU
//			if (gpu_draw_pixmap(__xfs + (x * xblock_len), __yfs + (y * yblock_len), __pixbuff, __pb_xlen, __pb_ylen, x * xblock_len, __pixmap, _pm_xlen, _pm_ylen, __pm_xlen, __angle) == FFLY_FAILURE)
//			printf("%d - %d\n", xblock_c, yblock_c);
			if (ffly_graphics::inited) {
				if (gpu_draw_pixmap(__gpu_dr_pm_mem, __xfs + (x * xblock_len), __yfs + (y * yblock_len), __pixbuff, __pb_xlen, __pb_ylen, x * xblock_len, __pixmap, _pm_xlen, _pm_ylen, __pm_xlen, __angle) == FFLY_FAILURE) {
# ifndef __GCOMPUTE_CPU
					return FFLY_FAILURE;
# endif
# if defined(__WITH_TASK_HANDLE) && defined(__GCOMPUTE_CPU)
					goto cpu_muli_compute;
# endif /*__GCOMPUTE_CPU*/

# if !defined(__WITH_TASK_HANDLE) && defined(__GCOMPUTE_CPU)
					if (cpu_draw_pixmap(__xfs + (x * xblock_len), __yfs + (y * yblock_len), __pixbuff, __pb_xlen, __pb_ylen, x * xblock_len, __pixmap, _pm_xlen, _pm_ylen, __pm_xlen) == FFLY_FAILURE)
						return FFLY_FAILURE;
# endif
				}
			}
# endif /*__GCOMPUTE_GPU*/
# if defined(__WITH_TASK_HANDLE) && defined(__GCOMPUTE_CPU)
# ifdef __GCOMPUTE_GPU
			else {
				cpu_muli_compute:
# endif /*__GCOMPUTE_GPU*/
			if (system::_task_handle.inited) {
/*
				args[wk_c] = {
					.args = {&args[wk_c].xfs, &args[wk_c].yfs, __pixbuff, &__pb_xlen, &__pb_ylen, &args[wk_c].pm_xfs, __pixmap, &args[wk_c].pm_xlen, &args[wk_c].pm_ylen, &__pm_xlen, &finished},
					.xfs = __xfs + (x * xblock_len),
					.yfs = __yfs + (y * yblock_len),
					.pm_xlen = _pm_xlen,
					.pm_ylen = _pm_ylen,
					.pm_xfs = x * xblock_len
				};
*/
					args[wk_c].xfs = __xfs + (x * xblock_len);
					args[wk_c].yfs = __yfs + (y * yblock_len);
					args[wk_c].pm_xlen = _pm_xlen;
					args[wk_c].pm_ylen = _pm_ylen;
					args[wk_c].pm_xfs = x * xblock_len;

					args[wk_c].args[0] = &args[wk_c].xfs;
					args[wk_c].args[1] = &args[wk_c].yfs;
					args[wk_c].args[2] = __pixbuff;
					args[wk_c].args[3] = &__pb_xlen;
					args[wk_c].args[4] = &__pb_ylen;
					args[wk_c].args[5] = &args[wk_c].pm_xfs;
					args[wk_c].args[6] = __pixmap;
					args[wk_c].args[7] = &args[wk_c].pm_xlen;
					args[wk_c].args[8] = &args[wk_c].pm_ylen;
					args[wk_c].args[9] = &__pm_xlen;
					args[wk_c].args[10] = &finished;

				if (system::_task_handle.handle(_cpu_draw_pixmap, args[wk_c].args, true, true) == FFLY_FAILURE) {
					finished = 0;
					return FFLY_FAILURE;
				}
				wk_c++;
			} else
				if (cpu_draw_pixmap(__xfs + (x * xblock_len), __yfs + (y * yblock_len), __pixbuff, __pb_xlen, __pb_ylen, x * xblock_len, __pixmap, _pm_xlen, _pm_ylen, __pm_xlen) == FFLY_FAILURE)
					return FFLY_FAILURE;
# ifdef __GCOMPUTE_GPU
			}
# endif /*__GCOMPUTE_GPU*/
# endif /*defined(__WITH_TASK_HANDLE) && defined(__GCOMPUTE_CPU)*/
		}
		__pixmap+=(_pm_ylen * __pm_xlen) * 4;
	}

# if defined(__WITH_TASK_HANDLE) && defined(__GCOMPUTE_CPU)
	if (system::_task_handle.inited) {
		while(finished != wk_c) {}
		finished = 0;
	}
# endif
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::graphics::draw_pixmap(uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, types::pixmap_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, boost::uint16_t __angle) {
	types::gpu_dr_pm_mem_t static gpu_dr_pm_mem;
	return draw_pixmap(gpu_dr_pm_mem, __xfs, __yfs, __pixbuff, __pb_xlen, __pb_ylen, __pixmap, __pm_xlen, __pm_ylen, __angle);
}

// code need fixing
# if defined(__USING_OPENCL) || defined(USING_OPENCL)
mdl::firefly::types::err_t static mdl::firefly::graphics::gpu_draw_pixmap(uint_t __xfs, uint_t __yfs, types::pixmap_t *__pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, types::pixmap_t *__pixmap, uint_t __pm_xlen, uint_t __pm_ylen) {
//	gpu_draw_pixmap(__xfs, __yfs, __pixbuff, __pm_xlen, __pb_ylen, __pixmap, __pm_xlen, __pm_ylen, __opencl_helper__);
}

mdl::firefly::types::err_t mdl::firefly::graphics::gpu_draw_pixmap(uint_t __xfs, uint_t __yfs, types::pixmap_t *__pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, types::pixmap_t *__pixmap, uint_t __pm_xlen, uint_t __pm_ylen, opencl *__opencl) {
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
