# include "../drm.h"
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include "xf86drm.h"
# include <sys/ioctl.h>
int main() {
	int fd;
	fd = ffly_drm_open("/dev/dri/card0");
	if (ioctl(fd, DRM_IOCTL_SET_MASTER, 0) == -1) {
		printf("failed to set master.\n");
	}
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
	drm_context_t handle;
	if (drmCreateContext(fd, &handle)) {
		printf("failed.\n");
	} else
		drmDestroyContext(fd, handle);
	ioctl(fd, DRM_IOCTL_DROP_MASTER, 0);
	ffly_drm_close(fd);
}
