# include "../drm.h"
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <sys/ioctl.h>
# include "../amdgpu.h"
# include "../amdgpu_drm.h"
# include <sys/mman.h>
void show_meminfo(int __fd) {
	struct drm_amdgpu_memory_info info;
	bzero(&info, sizeof(struct drm_amdgpu_memory_info));
	if (ffly_amdgpu_memory_info(__fd, &info) == -1) {
		printf("failed to get memory info.\n");
	}

	struct drm_amdgpu_heap_info *heap;

	heap = &info.vram;
	printf("vram:\n");
	printf("\ttotal heap size: %u\n", heap->heap_usage);
	printf("\tusable heap size: %u\n", heap->heap_usage);
	printf("\theap usage: %u\n", heap->heap_usage);
	printf("\tmax allocation: %u\n", heap->heap_usage);

	heap = &info.cpu_accessible_vram;
	printf("cpu accessible vram:\n");
	printf("\ttotal heap size: %u\n", heap->heap_usage);
	printf("\tusable heap size: %u\n", heap->heap_usage);
	printf("\theap usage: %u\n", heap->heap_usage);
	printf("\tmax allocation: %u\n", heap->heap_usage);

	heap = &info.gtt;
	printf("gtt:\n");
	printf("\ttotal heap size: %u\n", heap->heap_usage);
	printf("\tusable heap size: %u\n", heap->heap_usage);
	printf("\theap usage: %u\n", heap->heap_usage);
	printf("\tmax allocation: %u\n", heap->heap_usage);
}

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
	show_meminfo(fd);
	struct amdgpu_bo bo;
	if (!ffly_amdgpu_gem_object_create(fd, 1024, 0/*this will get auto set if below page size*/, AMDGPU_GEM_DOMAIN_CPU,
		AMDGPU_GEM_CREATE_CPU_ACCESS_REQUIRED|AMDGPU_GEM_CREATE_VRAM_CLEARED, &bo))
	{
		void *p;
		if (ffly_amdgpu_gem_mmap(fd, &bo, &p) == -1) {
			printf("failed to map, %s\n", strerror(errno));
		}
		
		void *ptr;
		ptr = mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, (ff_u64_t)p);

		show_meminfo(fd);

		*(ff_u64_t*)ptr = 21299;
		printf("%u\n", *(ff_u64_t*)ptr);

		munmap(ptr, 1024);
		if (ffly_amdgpu_gem_object_free(fd, &bo) == -1) {
			printf("failed to free gem object, %s\n", strerror(errno));
		}
	} else
		printf("failed to create gem object, %s\n", strerror(errno));
	ffly_drm_close(fd);
}
