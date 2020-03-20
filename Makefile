obj-m = memguard.o

KVERSION = $(shell uname -r)
BLDDIR= /lib/modules/$(KVERSION)/build

all: memguard gpu_map

memguard:
	make -C $(BLDDIR) M=$(PWD) modules

gpu_map: gpu_map.c
	$(CC) $^ -o $@

clean:
	make -C $(BLDDIR) M=$(PWD) clean
	rm -f gpu_map
