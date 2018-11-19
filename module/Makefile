BUILD_PATH=/lib/modules/$(shell uname -r)/build
obj-m += ksafebox.o  
ksafebox-objs := safebox.o hooks.o global.o files.o
all:  
	make -C $(BUILD_PATH) M=$(PWD) modules
clean:  
	make -C $(BUILD_PATH) M=$(PWD) clean