/*
 * File: gpu_map.c
 *
 * Description:
 * 	This is an example program illustrating how to use the memory-mapped IO
 * 	feature of MemGuard which allows a user-space application to modify the
 * 	throtting budget ratio of any CPU core by writing to a variable in a
 * 	shared-memory area.
 *
 * What does this program do?
 * 	- Opens the device file created by MemGuard in the /dev/ directory
 * 	- Uses mmap on the device file to map the shared memory based buffer
 * 	  created by MemGuard into its own address space (see "device_mmap" in
 * 	  memguard.c)
 * 	- Manipulates the shared area by casting it into a pre-defined structure
 * 	  ("struct user_info" in gpu_map.h)
 * 	  - Enables the "budget_ratio" feature of MemGuard
 * 	  - Starting with a budget_ratio of "10", incrememt it by 10 every
 * 	    second until it reaches 100 then decrement it by 10 until it reaches
 * 	    10. Repeat for 60 seconds
 * 	  - Disable the "budget_ratio" feature
 * 	- Release shared memory
 * 	- Exit
 *
 * Author: Waqar Ali <wali@ku.edu>
 */
#include "gpu_map.h"
#include "stdbool.h"

int main(void)
{
	int dev_fd = 0;
	bool increment = true;
	void *shm_area = NULL;
	struct user_info *uinfo = NULL;
	int iterations = 0, iter_limit = 60;

	dev_fd = open(DEV_FILE, O_RDONLY);
	if (dev_fd <= 0) {
		printf("Failed to open device file\n");
		return -1;
	}

	shm_area = mmap(NULL, MMAP_SIZE, PROT_READ | PROT_WRITE,
			MAP_PRIVATE, dev_fd, 0);

	if ((long)shm_area <= 0) {
		printf("Failed to mmap device file\n");
		close(dev_fd);
		return -2;
	}

	uinfo = (struct user_info *)shm_area;
	uinfo->enable = 1;

	for (uinfo->budget_ratio = 10; iterations < iter_limit; iterations++) {
		if (increment) {
			uinfo->budget_ratio += 10;
			if (uinfo->budget_ratio == 100)
				increment = false;
		} else {
			uinfo->budget_ratio -= 10;
			if (uinfo->budget_ratio == 10)
				increment = true;
		}

		sleep(1);
	}

	uinfo->enable = 0;
	munmap(shm_area, MMAP_SIZE);
	close(dev_fd);

	return 0;
}
