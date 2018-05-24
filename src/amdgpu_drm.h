# ifndef __ffly__amdgpu__drm__h
# define __ffly__amdgpu__drm__h
# include "ffint.h"
# include "drm.h"
#define DRM_AMDGPU_GEM_CREATE 0x00
#define DRM_AMDGPU_GEM_MMAP     0x01
#define DRM_AMDGPU_CTX 0x02
#define DRM_AMDGPU_INFO 0x05


#define DRM_IOCTL_AMDGPU_GEM_CREATE DRM_IOWR(DRM_COMMAND_BASE+DRM_AMDGPU_GEM_CREATE, union drm_amdgpu_gem_create)
#define DRM_IOCTL_AMDGPU_GEM_MMAP DRM_IOWR(DRM_COMMAND_BASE+DRM_AMDGPU_GEM_MMAP, union drm_amdgpu_gem_mmap)
#define DRM_IOCTL_AMDGPU_CTX DRM_IOWR(DRM_COMMAND_BASE+DRM_AMDGPU_CTX, union drm_amdgpu_ctx)
#define DRM_IOCTL_AMDGPU_INFO DRM_IOW(DRM_COMMAND_BASE+DRM_AMDGPU_INFO, struct drm_amdgpu_info)

#define AMDGPU_GEM_DOMAIN_CPU 0x1
#define AMDGPU_GEM_DOMAIN_GTT 0x2
#define AMDGPU_GEM_DOMAIN_VRAM 0x4
#define AMDGPU_GEM_DOMAIN_GDS 0x8
#define AMDGPU_GEM_DOMAIN_GWS 0x10
#define AMDGPU_GEM_DOMAIN_OA 0x20

/* Flag that CPU access will be required for the case of VRAM domain */
#define AMDGPU_GEM_CREATE_CPU_ACCESS_REQUIRED (1<<0)
/* Flag that CPU access will not work, this VRAM domain is invisible */
#define AMDGPU_GEM_CREATE_NO_CPU_ACCESS (1<<1)
/* Flag that USWC attributes should be used for GTT */
#define AMDGPU_GEM_CREATE_CPU_GTT_USWC (1<<2)
/* Flag that the memory should be in VRAM and cleared */
#define AMDGPU_GEM_CREATE_VRAM_CLEARED (1<<3)
/* Flag that create shadow bo(GTT) while allocating vram bo */
#define AMDGPU_GEM_CREATE_SHADOW (1<<4)
/* Flag that allocating the BO should use linear VRAM */
#define AMDGPU_GEM_CREATE_VRAM_CONTIGUOUS (1<<5)
/* Flag that BO is always valid in this VM */
#define AMDGPU_GEM_CREATE_VM_ALWAYS_VALID (1<<6)
/* Flag that BO sharing will be explicitly synchronized */
#define AMDGPU_GEM_CREATE_EXPLICIT_SYNC (1<<7)

struct drm_amdgpu_ctx_in {
	ff_u32_t op;
	ff_u32_t flags;
	ff_u32_t ctx_id;
	ff_s32_t priority;
};

struct drm_amdgpu_sched_in {
	ff_u32_t op;
	ff_u32_t fd;
	ff_s32_t priority;
	ff_u32_t flags;
};

union drm_amdgpu_ctx_out {
	struct {
		ff_u32_t ctx_id;
		ff_u32_t __pad;
	} alloc;

	struct {
		ff_u64_t flags;
		ff_u32_t hangs;
		ff_u32_t reset_status;
	} state;
};

struct drm_amdgpu_query_fw {
	ff_u32_t fw_type;
	ff_u32_t ip_inst;
	ff_u32_t index;
	ff_u32_t __pad;
};

struct drm_amdgpu_heap_info {
	ff_u64_t total_heap_size;
	ff_u64_t usable_heap_size;
	ff_u64_t heap_usage;
	ff_u64_t max_allocation;
};

struct drm_amdgpu_memory_info {
	struct drm_amdgpu_heap_info vram;
	struct drm_amdgpu_heap_info cpu_accessible_vram;
	struct drm_amdgpu_heap_info gtt;
};

struct drm_amdgpu_info {
	ff_u64_t ret_p;
	ff_u32_t ret_size;
	ff_u32_t query;
	union {	
		struct {
			ff_u32_t id;
			ff_u32_t __pad;
		} mode_crtc;

		struct {
			ff_u32_t type;
			ff_u32_t ip_inst;
		} query_hw_ip;

		struct {
			ff_u32_t dword_offset;
			ff_u32_t count;
			ff_u32_t inst;
			ff_u32_t flags;
		} read_mmr_reg;

		struct drm_amdgpu_query_fw query_fw;

		struct {
			ff_u32_t type;
			ff_u32_t offset;
		} vbios_info;

		struct {
			ff_u32_t type;
		} sensor_info;
	};
};

union drm_amdgpu_ctx {
	struct drm_amdgpu_ctx_in in;
	union drm_amdgpu_ctx_out out;
};

union drm_amdgpu_sched {
	struct drm_amdgpu_sched_in in;
};

struct drm_amdgpu_gem_create_in  {
	ff_u64_t bo_size;
	ff_u64_t alignment;
	ff_u64_t domains;
	ff_u64_t domain_flags;
};

struct drm_amdgpu_gem_create_out  {
	ff_u32_t handle;
	ff_u32_t __pad;
};

union drm_amdgpu_gem_create {
	struct drm_amdgpu_gem_create_in in;
	struct drm_amdgpu_gem_create_out out;
};

struct drm_amdgpu_gem_mmap_in {
	ff_u32_t handle;
	ff_u32_t __pad;
};

struct drm_amdgpu_gem_mmap_out {
	ff_u64_t addr_ptr;
};

union drm_amdgpu_gem_mmap {
	struct drm_amdgpu_gem_mmap_in in;
	struct drm_amdgpu_gem_mmap_out out;
};

// context operations
#define AMDGPU_CTX_OP_ALLOC_CTX 1
#define AMDGPU_CTX_OP_FREE_CTX  2
#define AMDGPU_CTX_OP_QUERY_STATE   3
#define AMDGPU_CTX_OP_QUERY_STATE2  4

/**
 *  Query h/w info: Flag that this is integrated (a.h.a. fusion) GPU
 *
 */
#define AMDGPU_IDS_FLAGS_FUSION         0x1
#define AMDGPU_IDS_FLAGS_PREEMPTION     0x2

/* indicate if acceleration can be working */
#define AMDGPU_INFO_ACCEL_WORKING		0x00
/* get the crtc_id from the mode object id? */
#define AMDGPU_INFO_CRTC_FROM_ID		0x01
/* query hw IP info */
#define AMDGPU_INFO_HW_IP_INFO			0x02
/* query hw IP instance count for the specified type */
#define AMDGPU_INFO_HW_IP_COUNT			0x03
/* timestamp for GL_ARB_timer_query */
#define AMDGPU_INFO_TIMESTAMP			0x05
/* Query the firmware version */
#define AMDGPU_INFO_FW_VERSION			0x0e
	/* Subquery id: Query VCE firmware version */
	#define AMDGPU_INFO_FW_VCE		0x1
	/* Subquery id: Query UVD firmware version */
	#define AMDGPU_INFO_FW_UVD		0x2
	/* Subquery id: Query GMC firmware version */
	#define AMDGPU_INFO_FW_GMC		0x03
	/* Subquery id: Query GFX ME firmware version */
	#define AMDGPU_INFO_FW_GFX_ME		0x04
	/* Subquery id: Query GFX PFP firmware version */
	#define AMDGPU_INFO_FW_GFX_PFP		0x05
	/* Subquery id: Query GFX CE firmware version */
	#define AMDGPU_INFO_FW_GFX_CE		0x06
	/* Subquery id: Query GFX RLC firmware version */
	#define AMDGPU_INFO_FW_GFX_RLC		0x07
	/* Subquery id: Query GFX MEC firmware version */
	#define AMDGPU_INFO_FW_GFX_MEC		0x08
	/* Subquery id: Query SMC firmware version */
	#define AMDGPU_INFO_FW_SMC		0x0a
	/* Subquery id: Query SDMA firmware version */
	#define AMDGPU_INFO_FW_SDMA		0x0b
	/* Subquery id: Query PSP SOS firmware version */
	#define AMDGPU_INFO_FW_SOS		0x0c
	/* Subquery id: Query PSP ASD firmware version */
	#define AMDGPU_INFO_FW_ASD		0x0d
	/* Subquery id: Query VCN firmware version */
	#define AMDGPU_INFO_FW_VCN		0x0e
/* number of bytes moved for TTM migration */
#define AMDGPU_INFO_NUM_BYTES_MOVED		0x0f
/* the used VRAM size */
#define AMDGPU_INFO_VRAM_USAGE			0x10
/* the used GTT size */
#define AMDGPU_INFO_GTT_USAGE			0x11
/* Information about GDS, etc. resource configuration */
#define AMDGPU_INFO_GDS_CONFIG			0x13
/* Query information about VRAM and GTT domains */
#define AMDGPU_INFO_VRAM_GTT			0x14
/* Query information about register in MMR address space*/
#define AMDGPU_INFO_READ_MMR_REG		0x15
/* Query information about device: rev id, family, etc. */
#define AMDGPU_INFO_DEV_INFO			0x16
/* visible vram usage */
#define AMDGPU_INFO_VIS_VRAM_USAGE		0x17
/* number of TTM buffer evictions */
#define AMDGPU_INFO_NUM_EVICTIONS		0x18
/* Query memory about VRAM and GTT domains */
#define AMDGPU_INFO_MEMORY			0x19
/* Query vce clock table */
#define AMDGPU_INFO_VCE_CLOCK_TABLE		0x1A
/* Query vbios related information */
#define AMDGPU_INFO_VBIOS			0x1B
	/* Subquery id: Query vbios size */
	#define AMDGPU_INFO_VBIOS_SIZE		0x1
	/* Subquery id: Query vbios image */
	#define AMDGPU_INFO_VBIOS_IMAGE		0x2
/* Query UVD handles */
#define AMDGPU_INFO_NUM_HANDLES			0x1C
/* Query sensor related information */
#define AMDGPU_INFO_SENSOR			0x1D
	/* Subquery id: Query GPU shader clock */
	#define AMDGPU_INFO_SENSOR_GFX_SCLK		0x1
	/* Subquery id: Query GPU memory clock */
	#define AMDGPU_INFO_SENSOR_GFX_MCLK		0x2
	/* Subquery id: Query GPU temperature */
	#define AMDGPU_INFO_SENSOR_GPU_TEMP		0x3
	/* Subquery id: Query GPU load */
	#define AMDGPU_INFO_SENSOR_GPU_LOAD		0x4
	/* Subquery id: Query average GPU power	*/
	#define AMDGPU_INFO_SENSOR_GPU_AVG_POWER	0x5
	/* Subquery id: Query northbridge voltage */
	#define AMDGPU_INFO_SENSOR_VDDNB		0x6
	/* Subquery id: Query graphics voltage */
	#define AMDGPU_INFO_SENSOR_VDDGFX		0x7
	/* Subquery id: Query GPU stable pstate shader clock */
	#define AMDGPU_INFO_SENSOR_STABLE_PSTATE_GFX_SCLK		0x8
	/* Subquery id: Query GPU stable pstate memory clock */
	#define AMDGPU_INFO_SENSOR_STABLE_PSTATE_GFX_MCLK		0x9
/* Number of VRAM page faults on CPU access. */
#define AMDGPU_INFO_NUM_VRAM_CPU_PAGE_FAULTS	0x1E
#define AMDGPU_INFO_VRAM_LOST_COUNTER		0x1F

#define AMDGPU_INFO_MMR_SE_INDEX_SHIFT	0
#define AMDGPU_INFO_MMR_SE_INDEX_MASK	0xff
#define AMDGPU_INFO_MMR_SH_INDEX_SHIFT	8
#define AMDGPU_INFO_MMR_SH_INDEX_MASK	0xff

# endif /*__ffly__amdgpu__drm__h*/
