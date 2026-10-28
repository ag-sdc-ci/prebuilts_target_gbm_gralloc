#ifndef __ANDROID_GRALLOC_HANDLE_H__
#define __ANDROID_GRALLOC_HANDLE_H__

#include <cutils/native_handle.h>
#include <stdint.h>

/* support users of drm_gralloc/gbm_gralloc */
#define gralloc_gbm_handle_t gralloc_handle_t
#define gralloc_drm_handle_t gralloc_handle_t

struct gralloc_handle_t {
	native_handle_t base;

	/* dma-buf file descriptor */
	int prime_fd;

	/* api variables */
	uint32_t magic; /* differentiate between allocator impls */
	uint32_t version; /* api version */

	uint32_t width; /* width of buffer in pixels */
	uint32_t height; /* height of buffer in pixels */
	uint32_t format; /* pixel format (Android) */
	uint32_t usage; /* android libhardware usage flags */

	uint32_t stride; /* the stride in bytes */
	int data_owner; /* owner of data (for validation) */
	uint64_t modifier __attribute__((aligned(8))); /* buffer modifiers */

	union {
		void *data; /* pointer to struct gralloc_gbm_bo_t */
		uint64_t reserved;
	} __attribute__((aligned(8)));
};

#define GRALLOC_HANDLE_VERSION 4
#define GRALLOC_HANDLE_MAGIC 0x60585350
#define GRALLOC_HANDLE_NUM_FDS 1
#define GRALLOC_HANDLE_NUM_INTS (	\
	((sizeof(struct gralloc_handle_t) - sizeof(native_handle_t))/sizeof(int))	\
	 - GRALLOC_HANDLE_NUM_FDS)

static inline struct gralloc_handle_t *gralloc_handle(buffer_handle_t handle)
{
	return (struct gralloc_handle_t *)handle;
}

static inline native_handle_t *gralloc_handle_create(int32_t width,
                                                     int32_t height,
                                                     int32_t hal_format,
                                                     int32_t usage)
{
	struct gralloc_handle_t *handle;
	native_handle_t *nhandle = native_handle_create(GRALLOC_HANDLE_NUM_FDS,
							GRALLOC_HANDLE_NUM_INTS);

	if (!nhandle)
		return NULL;

	handle = gralloc_handle(nhandle);
	handle->magic = GRALLOC_HANDLE_MAGIC;
	handle->version = GRALLOC_HANDLE_VERSION;
	handle->width = width;
	handle->height = height;
	handle->format = hal_format;
	handle->usage = usage;
	handle->prime_fd = -1;

	return nhandle;
}

#endif
