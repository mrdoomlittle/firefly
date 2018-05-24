# include "amdgpu.h"
int ffly_amdgpu_info(int __fd, struct amdgpu_info *__info) {
	struct drm_amdgpu_info info;
	ff_u64_t r = 0;
	info.ret_p = (ff_u64_t)&r;
	info.ret_size = 8;
	info.query = AMDGPU_INFO_SENSOR;
	
	int ret;
	ff_u32_t *sensor_type = &info.sensor_info.type;

	*sensor_type = AMDGPU_INFO_SENSOR_GFX_SCLK;
	if ((ret = drm_ioctl(__fd, DRM_IOCTL_AMDGPU_INFO, &info)) == -1)
		return -1;
	__info->gfx_sclk = r;

	*sensor_type = AMDGPU_INFO_SENSOR_GFX_MCLK;
	if ((ret = drm_ioctl(__fd, DRM_IOCTL_AMDGPU_INFO, &info)) == -1)
		return -1;
	__info->gfx_mclk = r;

	*sensor_type = AMDGPU_INFO_SENSOR_GPU_TEMP;
	if ((ret = drm_ioctl(__fd, DRM_IOCTL_AMDGPU_INFO, &info)) == -1)
		return -1;
	__info->temp = r;

	*sensor_type = AMDGPU_INFO_SENSOR_GPU_LOAD;
	if ((ret = drm_ioctl(__fd, DRM_IOCTL_AMDGPU_INFO, &info)) == -1)
		return -1;
	__info->load = r;

	*sensor_type = AMDGPU_INFO_SENSOR_GPU_AVG_POWER;
	if ((ret = drm_ioctl(__fd, DRM_IOCTL_AMDGPU_INFO, &info)) == -1)
		return -1;
	__info->avg_power = r;
	return ret;
}

int ffly_amdgpu_memory_info(int __fd, struct drm_amdgpu_memory_info *__info) {
	struct drm_amdgpu_info info;
	info.ret_p = (ff_u64_t)__info;
	info.ret_size = sizeof(struct drm_amdgpu_memory_info);
	info.query = AMDGPU_INFO_MEMORY;
	
	if (drm_ioctl(__fd, DRM_IOCTL_AMDGPU_INFO, &info) == -1) 
		return -1;
	return 0;
}

int ffly_amdgpu_gem_mmap(int __fd, struct amdgpu_bo *__bo, void **__p) {
	union drm_amdgpu_gem_mmap args;

	args.in.handle = __bo->handle;
	int ret;
	if ((ret = drm_ioctl(__fd, DRM_IOCTL_AMDGPU_GEM_MMAP, &args)) == -1)
		return -1;
	*__p = (void*)args.out.addr_ptr;
	return 0;
}

int
ffly_amdgpu_gem_object_create(int __fd, ff_u64_t __size, ff_u64_t __alignment,
	ff_u64_t __domains, ff_u64_t __flags, struct amdgpu_bo *__bo)
{
	union drm_amdgpu_gem_create args;

	args.in.bo_size = __size;
	args.in.domains = __domains;
	args.in.domain_flags = __flags;
	args.in.alignment = __alignment;

	int ret;
	if ((ret = drm_ioctl(__fd, DRM_IOCTL_AMDGPU_GEM_CREATE, &args)) == -1)
		return -1;
	__bo->handle = args.out.handle;
	return 0;
}

int ffly_amdgpu_gem_object_free(int __fd, struct amdgpu_bo *__bo) {
	struct drm_gem_close args;
	args.handle = __bo->handle;
	drm_ioctl(__fd, DRM_IOCTL_GEM_CLOSE, &args);
	return 0;
}

int ffly_amdgpu_ctx_alloc(int __fd, struct amdgpu_ctx *__ctx) {
	union drm_amdgpu_ctx ctx;
	ctx.in.op = AMDGPU_CTX_OP_ALLOC_CTX;
	int ret;
	if ((ret = drm_ioctl(__fd, DRM_IOCTL_AMDGPU_CTX, &ctx)) == -1)
		return -1;
	__ctx->ctx_id = ctx.out.alloc.ctx_id;
	return 0;
}

int ffly_amdgpu_ctx_free(int __fd, struct amdgpu_ctx *__ctx) {
	union drm_amdgpu_ctx ctx;
	ctx.in.op = AMDGPU_CTX_OP_FREE_CTX;
	ctx.in.ctx_id = __ctx->ctx_id;
	int ret;
	if ((ret = drm_ioctl(__fd, DRM_IOCTL_AMDGPU_CTX, &ctx)) == -1)
		return -1;
	return 0;
}
