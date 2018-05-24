# ifndef __ffly__amdgpu__h
# define __ffly__amdgpu__h
# include "ffint.h"
# include "amdgpu_drm.h"
struct amdgpu_ctx {
	ff_u32_t ctx_id;
};

struct amdgpu_dev {
	int fd;
};

struct amdgpu_info {
	// mhz
	ff_u32_t gfx_sclk;
	ff_u32_t gfx_mclk;

	// temperature in millidegrees
	ff_u32_t temp;

	ff_u32_t load;

	// watts
	ff_i32_t avg_power;
};

struct amdgpu_bo {
	ff_u32_t handle;
};



int ffly_amdgpu_gem_mmap(int, struct amdgpu_bo*, void**);
int ffly_amdgpu_gem_object_create(int, ff_u64_t, ff_u64_t, ff_u64_t, ff_u64_t, struct amdgpu_bo*);
int ffly_amdgpu_gem_object_free(int, struct amdgpu_bo*);

int ffly_amdgpu_memory_info(int, struct drm_amdgpu_memory_info*);
int ffly_amdgpu_info(int, struct amdgpu_info*);
int ffly_amdgpu_ctx_alloc(int, struct amdgpu_ctx*);
int ffly_amdgpu_ctx_free(int, struct amdgpu_ctx*);
#endif /*__ffly__amdgpu__h*/
