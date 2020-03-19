#ifndef __GPU_MAP_H__
#define __GPU_MAP_H__

#ifndef __KERNEL__

/*******************************************************************************
 * NOTE : Definitions and includes for user-space MemGuard-GPU library only
 ******************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

#define MMAP_SIZE			(sizeof(struct user_info))

#else

#define MMAP_BUF_SIZE			(8192)

#endif /* __KERNEL__ */

struct user_info {
	int enable;
	int budget;
};

#define DEV_NAME			"memgpu"
#define DEV_FILE			"/dev/memgpu"

#endif /* __GPU_MAP_H__ */
