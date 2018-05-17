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
