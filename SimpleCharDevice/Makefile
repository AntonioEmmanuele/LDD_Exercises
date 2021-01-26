obj-m := ex1.o
#the main should never have the same name of the object you want to build
ex1-objs := lib.o main.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
