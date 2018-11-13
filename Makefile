CC=gcc
BUILD_PATH=/lib/modules/$(shell uname -r)/build
obj-m=src/hooks.o

all:
	make -C $(BUILD_PATH) M=$(PWD) modules

clean:
	make -C $(BUILD_PATH) M=$(PWD) clean
