# The obj-m variable is used to specify kernel modules to build
#obj-m += hello.o
obj-m += character-hello.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean