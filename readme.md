# Linux Kernel Development

## Refs

- [Linux Kernel Development Course - Daniel McCarthy](https://www.udemy.com/course/linux-kernel-development-course/?couponCode=KEEPLEARNING)

- [Bootlin Elixir Cross Referencer](https://elixir.bootlin.com/linux/latest/source)

## Building

- With CMake

```bash
ggm@gAN515-52:~/Documents/tsmotter/lkd (master)$ rm -rf build && cmake -S . -B build -D m_target=character-hello
...
ggm@gAN515-52:~/Documents/tsmotter/lkd (master)$ cmake --build build --parallel `nproc`
...
```

- Without CMake

```bash
ggm@gAN515-52:~/Documents/tsmotter/lkd (master)$ make
...
ggm@gAN515-52:~/Documents/tsmotter/lkd (master)$ make clean
```

## Study notes

### Lesson 1 - Hello World Device

- During building, I had the following problem:

```bash
ggm@gAN515-52:~/Documents/tsmotter/lkd $ make
make -C /lib/modules/6.5.0-28-generic/build M=/home/ggm/Documents/tsmotter/lkd modules
make[1]: Entering directory '/usr/src/linux-headers-6.5.0-28-generic'
warning: the compiler differs from the one used to build the kernel
  The kernel was built by: x86_64-linux-gnu-gcc-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
  You are using:
  CC [M]  /home/ggm/Documents/tsmotter/lkd/hello.o
/bin/sh: 1: gcc-12: not found
make[3]: *** [scripts/Makefile.build:251: /home/ggm/Documents/tsmotter/lkd/hello.o] Error 127
make[2]: *** [/usr/src/linux-headers-6.5.0-28-generic/Makefile:2039: /home/ggm/Documents/tsmotter/lkd] Error 2
make[1]: *** [Makefile:234: __sub-make] Error 2
make[1]: Leaving directory '/usr/src/linux-headers-6.5.0-28-generic'
make: *** [Makefile:4: all] Error 2
```

- [Here is the found solution:](https://askubuntu.com/a/1500018)

```bash
ggm@gAN515-52:~/Documents/tsmotter/lkd $ which gcc
/usr/bin/gcc
ggm@gAN515-52:~/Documents/tsmotter/lkd $ ls -al /usr/bin/gcc
lrwxrwxrwx 1 root root 6 ago  5  2021 /usr/bin/gcc -> gcc-11
ggm@gAN515-52:~/Documents/tsmotter/lkd $ sudo apt install --reinstall gcc-12
...
ggm@gAN515-52:~/Documents/tsmotter/lkd $ sudo ln -s -f /usr/bin/gcc-12 /usr/bin/gcc
ggm@gAN515-52:~/Documents/tsmotter/lkd $ ls -al /usr/bin/gcc
lrwxrwxrwx 1 root root 15 abr 28 10:55 /usr/bin/gcc -> /usr/bin/gcc-12
```

- Build works now...

```bash
ggm@gAN515-52:~/Documents/tsmotter/lkd $ make
CC is equal to: cc
make -C /lib/modules/6.5.0-28-generic/build M=/home/ggm/Documents/tsmotter/lkd modules
make[1]: Entering directory '/usr/src/linux-headers-6.5.0-28-generic'
warning: the compiler differs from the one used to build the kernel
  The kernel was built by: x86_64-linux-gnu-gcc-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
  You are using:           gcc-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
CC is equal to: gcc-12
  CC [M]  /home/ggm/Documents/tsmotter/lkd/hello.o
CC is equal to: gcc-12
  MODPOST /home/ggm/Documents/tsmotter/lkd/Module.symvers
  CC [M]  /home/ggm/Documents/tsmotter/lkd/hello.mod.o
  LD [M]  /home/ggm/Documents/tsmotter/lkd/hello.ko
  BTF [M] /home/ggm/Documents/tsmotter/lkd/hello.ko
Skipping BTF generation for /home/ggm/Documents/tsmotter/lkd/hello.ko due to unavailability of vmlinux
make[1]: Leaving directory '/usr/src/linux-headers-6.5.0-28-generic'
```

- The `.ko` file created is in fact the kernel object file that can be loaded into the kernel

- Load the module and see it's output in the kernel dmesg:

```bash
ggm@gAN515-52:~/Documents/tsmotter/lkd $ sudo insmod hello.ko
ggm@gAN515-52:~/Documents/tsmotter/lkd $ sudo dmesg | grep world
[52854.539596] Hello world!
```

- List the current modules and see the hello module:

```bash
ggm@gAN515-52:~/Documents/tsmotter/lkd $ lsmod | grep hello
Module                  Size  Used by
hello                  12288  0
```

- Remove the module and see it's output in the kernel dmesg:

```bash
ggm@gAN515-52:~/Documents/tsmotter/lkd $ sudo rmmod hello
ggm@gAN515-52:~/Documents/tsmotter/lkd $ sudo dmesg | grep world
[52854.539596] Hello world!
[53017.225366] Goodbye world!
```

- References to commands:

```bash
ggm@gAN515-52:~/Documents/tsmotter/lkd $ tldr insmod

  insmod

  Dynamically load modules into the Linux Kernel.
  More information: https://manned.org/insmod.

  - Insert a kernel module into the Linux kernel:
    insmod path/to/module.ko
ggm@gAN515-52:~/Documents/tsmotter/lkd $ tldr rmmod

  rmmod

  Remove modules from the Linux kernel.
  More information: https://manned.org/rmmod.

  - Remove a module from the kernel:
    sudo rmmod module_name

  - Remove a module from the kernel and display verbose information:
    sudo rmmod --verbose module_name

  - Remove a module from the kernel and send errors to syslog instead of `stderr`:
    sudo rmmod --syslog module_name
ggm@gAN515-52:~/Documents/tsmotter/lkd $ tldr lsmod

  lsmod

  Shows the status of Linux kernel modules.
  See also `modprobe`, which loads kernel modules.
  More information: https://manned.org/lsmod.

  - List all currently loaded kernel modules:
    lsmod
```

### Lesson 2 - Simple Character Device

- Created the file `character-hello.c`

- Edited the Makefile to `obj-m += character-hello.o`

```bash
ggm@gAN515-52:~/Documents/tsmotter/lkd $ make
...
ggm@gAN515-52:~/Documents/tsmotter/lkd $ sudo insmod character-hello.ko
ggm@gAN515-52:~/Documents/tsmotter/lkd $ lsmod | grep hello
character_hello        12288  0
ggm@gAN515-52:~/Documents/tsmotter/lkd $ sudo mknod /dev/mydev 98 0
ggm@gAN515-52:~/Documents/tsmotter/lkd $ ls -al /dev | grep 98
crw-r--r--   1 root root     98,     0 abr 28 11:56 mydev
ggm@gAN515-52:~/Documents/tsmotter/lkd $ sudo su
root@gAN515-52:/home/ggm/Documents/tsmotter/lkd# echo "foo" > /dev/mydev
root@gAN515-52:/home/ggm/Documents/tsmotter/lkd# cat /dev/mydev
foo
foo
foo
foo
^C
root@gAN515-52:/home/ggm/Documents/tsmotter/lkd#
exit
ggm@gAN515-52:~/Documents/tsmotter/lkd $ sudo rmmod character_hello
```

- In another terminal:

```bash
ggm@gAN515-52:~/Documents/tsmotter $ sudo dmesg -w
[56748.437235] [Hello.ko] init OK
[56788.334621] [Hello.ko] open OK
[56788.334704] [Hello.ko] write OK
[56788.334733] [Hello.ko] release OK
[56813.806724] [Hello.ko] open OK
[56813.806786] [Hello.ko] read OK
[56813.806811] [Hello.ko] read OK
[56813.806826] [Hello.ko] read OK
[56813.929183] [Hello.ko] release OK
[56890.458021] [Hello.ko] exit OK
```

### inode, files

- `struct file` represents an open file instance (a handle)

- `struct inode` are more related to paths and attributes

  - Short for "index node"

  - The exact structure and content of an inode can vary depending on the specific file system implementation, but it typically contains metadata about the file or directory, such as its size, ownership, permissions, timestamps, and pointers to the data blocks where the file's actual content is stored on disk.

- If more than one instance to `fopen` is called on the same file, it'll happen to exist multiple instances of `struct file` coexisting but only one instance of `struct inode`

- There are `struct inode_operations` like get_link, permission, create, link, unlink, symlink, mkdir, rmdir, mknod, rename, setattr, getattr, etc

- It is much more related to filesystems operations other than simple character devices