# include "fill.h"
# include "../system/io.h"
# include "../types/bool_t.h"
# include "../ffly_def.h"
# include "colour_attr.hpp"
# include "../system/errno.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../firefly.hpp"
# include "../system/thread.h"
__global__ void static pixfill(mdl::firefly::types::byte_t *__dst, mdl::firefly::types::colour_t *__colour) {
	mdl::firefly::types::byte_t *p = __dst+((threadIdx.x+(blockIdx.x*blockDim.x))*4);
	*(p+R_OFF) = __colour->r, *(p+G_OFF) = __colour->g, *(p+B_OFF) = __colour->b, *(p+A_OFF) = __colour->a;
}

// mutithreading hasen't been tested on this and may not work.

struct context {
    mdl::firefly::types::byte_t *dev_dst;
    mdl::firefly::types::colour_t *dev_colour;
    mdl::firefly::types::colour_t colour;
    mdl_uint_t size;
    mdl::firefly::types::bool_t prepared;
};

/*
    how many threads can access at one time.
*/
# define THREADS 8
static struct context parent_ctx = {
    dev_dst:nullptr,
    dev_colour:nullptr,
    colour:{r:0, g:0, b:0, a:0},
    size:0,
    prepared:ffly_false
};

static struct context ctx_list[THREADS];
void static cleanup(void *__arg_p) {
	mdl::firefly::system::io::fprintf(ffly_log, "cleanup for fill.\n");
    mdl_uint_t i = 0;
    while(i != THREADS) {
        struct context *ctx = &ctx_list[i];
    	if (ctx->dev_dst != nullptr)
    		mdl::firefly::memory::gpu_mem_free(ctx->dev_dst);
    	if (ctx->dev_colour != nullptr)
    		mdl::firefly::memory::gpu_mem_free(ctx->dev_colour);
        i++;
    }
}
# include "../system/err.h"
# include "../system/mutex.h"
ffly_mutex_t static mutex = FFLY_MUTEX_INIT;
mdl::firefly::types::err_t mdl::firefly::graphics::gpu_pixfill(types::byte_t *__dst, mdl_uint_t __nopix, types::colour_t __colour) {
	types::cl_err_t err;
	mdl_uint_t size = __nopix*4;

    ffly_tid_t tid = ffly_gettid();
    // thread id should only be null if its the parent prossess calling
    struct context *ctx = tid == FFLY_TID_NULL?&parent_ctx:&ctx_list[tid];
    types::bool_t static inited = ffly_false;
    if (!inited) {
        if (_ok(ffly_mutex_trylock(&mutex))) {
            if (!inited) {
                mdl_uint_t i = 0;
                while(i != THREADS) {
                    ctx[i++] = (struct context) {
                        dev_dst:nullptr,
                        dev_colour:nullptr,
                        colour:{r:0, g:0, b:0, a:0},
                        size:0,
                        prepared:ffly_false
                    };
                }
                ffly_act_add_task(&__ffly_act__, act_gid_cleanup, &cleanup, nullptr);
                inited = ffly_true;
            }
            ffly_mutex_unlock(&mutex);
        }
    }
 
    while(!inited);
    if (!ctx->prepared) {
		if (memory::gpu_mem_alloc((void**)&ctx->dev_colour, sizeof(types::colour_t)) != FFLY_SUCCESS) {
			system::io::fprintf(ffly_err, "cuda, failed to allocate memory for colour.\n");
			return FFLY_FAILURE;
		}

		if (memory::gpu_mem_alloc((void**)&ctx->dev_dst, size) != FFLY_SUCCESS) {
			system::io::fprintf(ffly_err, "cuda, failed to allocate memory for dest.\n");
			return FFLY_FAILURE;
		}

		if ((err = cudaMemcpy(ctx->dev_colour, &__colour, sizeof(types::colour_t), cudaMemcpyHostToDevice)) != ffly_cl_success) {
			system::io::fprintf(ffly_err, "cuda, failed to copy colour to device, %s\n", cudaGetErrorString(err));
			return FFLY_FAILURE;
		}
		ctx->prepared = ffly_true;
	}

	if (ctx->size != size) {
		if (ctx->dev_dst != nullptr) memory::gpu_mem_free(ctx->dev_dst);
		if (memory::gpu_mem_alloc((void**)&ctx->dev_dst, size) != FFLY_SUCCESS) {
			system::io::fprintf(ffly_err, "cuda, failed to allocate memory for dest.\n");
			return FFLY_FAILURE;
		}
		ctx->size = size;
	}

	if ((err = cudaMemcpy(ctx->dev_dst, __dst, size, cudaMemcpyHostToDevice)) != ffly_cl_success) {
		system::io::fprintf(ffly_err, "cuda, failed to copy dest to device, %s\n", cudaGetErrorString(err));
		return FFLY_FAILURE;
	}

	if (ctx->colour.r != __colour.r || ctx->colour.g != __colour.g || ctx->colour.b != __colour.b || ctx->colour.a != __colour.a) {
		if ((err = cudaMemcpy(ctx->dev_colour, &__colour, sizeof(types::colour_t), cudaMemcpyHostToDevice)) != ffly_cl_success) {
			system::io::fprintf(ffly_err, "cuda, failed to copy colour to device.\n");
			return FFLY_FAILURE;
		}
		ctx->colour = __colour;
	}

	mdl_uint_t blk_size;
	mdl_uint_t no_blks, left, off;

	if (!(__nopix>>8)) {
		blk_size = __nopix;
		no_blks = 1;
	} else {
		blk_size = 1<<8;
		no_blks = __nopix>>8;
	}

	system::io::fprintf(ffly_log, "no_blks: %u, blk_size: %u\n", no_blks, blk_size);
	pixfill<<<no_blks, blk_size>>>(ctx->dev_dst, ctx->dev_colour);
	if ((left = (__nopix-(off = (no_blks*(1<<8)))))>0 && (__nopix>>8)>0)
		pixfill<<<1, left>>>(ctx->dev_dst+(off*4), ctx->dev_colour);
	if ((err = cudaMemcpy(__dst, ctx->dev_dst, size, cudaMemcpyDeviceToHost)) != ffly_cl_success) {
		system::io::fprintf(ffly_err, "cuda, failed to copy dest from device to host.\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}
