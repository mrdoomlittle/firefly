# include "../drm.h"
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include "xf86drm.h"
# include <sys/ioctl.h>
# include "../amdgpu.h"
int main() {
	int fd;
	fd = ffly_drm_open("/dev/dri/card0");
//	if (ioctl(fd, DRM_IOCTL_SET_MASTER, 0) == -1) {
//		printf("failed to set master.\n");
//	}
//	struct drm_device dev;
//	ffly_drm_pci_dev(&dev, 226, 0);
//	printf("vendor-id: %u\n", dev.devinfo.pci.vendor_id);
//	printf("device-id: %u\n", dev.devinfo.pci.device_id);
	/*
	drm_context_t context;
	if (!ffly_drm_create_context(fd, &context)) {
		if (ffly_drm_destroy_context(fd, context) == -1) {
			printf("failed to destory context.\n");
		}
	} else
		printf("failed to create context, %s\n", strerror(errno));
	*/
//	drm_context_t handle;
//	if (drmCreateContext(fd, &handle)) {
//		printf("failed.\n");
//	} else
//		drmDestroyContext(fd, handle);
//	ioctl(fd, DRM_IOCTL_DROP_MASTER, 0);
/*
	struct drm_amdgpu_info info;
	bzero(&info, sizeof(struct drm_amdgpu_info));
	ff_u64_t r = 0;
	info.ret_p = (ff_u64_t)&r; 
	info.ret_size = 8;
	info.query = AMDGPU_INFO_VRAM_USAGE;
	int ret;
	if ((ret = ffly_amdgpu_info(fd, &info)) == -1) {
		printf("failed to get info, %s\n", strerror(errno));
	}

	printf("usage: %lu\n", r);
*/

	struct amdgpu_info info;
	if (ffly_amdgpu_info(fd, &info) == -1) {
		printf("failed to get info.\n");
	}
	printf("gfx sclk: %u\n", info.gfx_sclk);
	printf("gfx mclk: %u\n", info.gfx_mclk);
	printf("temp: %f-degree\n", ((float)info.temp)*0.001);
	printf("load: %u%%\n", info.load);
	printf("power: %ld-watts\n", info.avg_power);
/*
	struct amdgpu_ctx ctx;
	if (ffly_amdgpu_ctx_alloc(fd, &ctx) == -1) {
		printf("failed to alloc context, %s\n", strerror(errno));
	} else {
		printf("context with id: %lu\n", ctx.ctx_id);
		if (ffly_amdgpu_ctx_free(fd, &ctx) == -1) {
			printf("failed to free context.\n");
		}
	}
*/
	ffly_drm_close(fd);
}
