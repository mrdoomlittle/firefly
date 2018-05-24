# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <sys/ioctl.h>
# include "drm.h"
# include "system/string.h"
# include "linux/limits.h"
int ffly_drm_open(char const *__file) {
	int fd;
	if ((fd = open(__file, O_RDWR)) == -1) {
		printf("failed to open.\n");
	}
	return fd;
}

void ffly_drm_close(int __fd) {
	close(__fd);
}

int drm_ioctl(int __fd, unsigned long __req, void *__arg) {
	int ret;
	ret = ioctl(__fd, __req, __arg);
	return ret;
}

int ffly_drm_context_add(int __fd, drm_context_t *__handle) {
	struct drm_context ctx;
	bzero(&ctx, sizeof(struct drm_context));
	if (drm_ioctl(__fd, DRM_IOCTL_ADD_CTX, &ctx) == -1)
		return -1;
	*__handle = ctx.handle;
	return 0;
}

int ffly_drm_context_rm(int __fd, drm_context_t __handle) {
	struct drm_context ctx;
	ctx.handle = __handle;
	if (drm_ioctl(__fd, DRM_IOCTL_RM_CTX, &ctx) == -1)
		return -1;
	return 0;
}

void
ffly_drm_pci_dev(struct drm_device *__dev,
	int __major, int __minor)
{
	char conf[PATH_MAX+1];
	char *p = conf;
	strcpy(p, "/sys/dev/char");
	p+=13;
	*(p++) = '/';
	p+=ffly_nots(__major, p);
	*(p++) = ':';
	p+=ffly_nots(__minor, p);
	*(p++) = '/';
	strcpy(p, "device/config");
	p+=13;
	*p = '\0';

	printf("config: %s\n", conf);
	int fd;
	fd = open(conf, O_RDONLY);

	ff_u8_t config[64];
	read(fd, config, 64);
	close(fd);

	__dev->devinfo.pci.vendor_id = *(ff_u16_t*)config;
	__dev->devinfo.pci.device_id = *(ff_u16_t*)(config+2); 
}
