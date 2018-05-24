# ifndef __ffly__drm__h
# define __ffly__drm__h
# include "ffint.h"
# include <asm/ioctl.h>
#define DRM_COMMAND_BASE 0x40
#define DRM_COMMAND_END 0xA0

#define DRM_IOCTL_BASE 'd'
#define DRM_IO(nr) _IO(DRM_IOCTL_BASE, nr)
#define DRM_IOR(nr, type) _IOR(DRM_IOCTL_BASE, nr, type)
#define DRM_IOW(nr, type) _IOW(DRM_IOCTL_BASE, nr, type)
#define DRM_IOWR(nr, type) _IOWR(DRM_IOCTL_BASE, nr, type)

#define DRM_IOCTL_GEM_CLOSE DRM_IOW (0x09, struct drm_gem_close)
#define DRM_IOCTL_ADD_CTX DRM_IOWR(0x20, struct drm_context)
#define DRM_IOCTL_RM_CTX DRM_IOWR(0x21, struct drm_context)

struct drm_gem_close {
	ff_u32_t handle;
	ff_u32_t __pad;
};

struct drm_pci_dev_info {
	ff_u16_t vendor_id;
	ff_u16_t device_id;
};

typedef int unsigned drm_context_t;
struct drm_context {
	drm_context_t handle;
	int unsigned flags;
};

struct drm_device {
	union {
		struct drm_pci_dev_info pci;
	} devinfo;
};

int drm_ioctl(int, unsigned long, void*);
int ffly_drm_open(char const*);
void ffly_drm_close();

int ffly_drm_context_add(int, drm_context_t*);
int ffly_drm_context_rm(int, drm_context_t);

void ffly_drm_pci_dev(struct drm_device*, int, int);

# endif /*__ffly__drm__h*/
