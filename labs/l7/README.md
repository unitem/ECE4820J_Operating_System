# Non-programming exercises.

1. **What is a kernel module, and how does it differ from a regular library?**

A kernel module is a piece of code that can be loaded into the kernel of an operating system on demand, without needing to reboot the system. 
    1. Kernel modules can be seen as a plugin to the kernel program, and have high privilege and can perform tasks that are not accessible to user-level code. Regular libraries, on the other hand, operate in user space and have restricted access to system resources and hardware.

    2. Kernel modules run in the same memory space as the kernel, allowing them to directly access kernel functions and data structures. Regular libraries operate in the memory space of the user-level process that loads them, isolated from the kernel space for security and stability reasons.

    3. Kernel modules often provide low-level functions like interacting with hardware or managing system resources. Regular libraries typically offer higher-level functionality, such as data manipulation, network communication, or graphical user interface components.

    4. Kernel modules can be dynamically loaded and unloaded from the kernel as needed, which is useful for adding functionality to the kernel without rebooting. Regular libraries are loaded and unloaded by user-level applications.

2. **How to compile a kernel module?**

    1. Create a Makefile. It looks like:
        ```Makefile
        obj-m += mymodule.o

        all:
            make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

        clean:
            make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

        ```

    2. Write the code

    3. Compile the module
        Run `make` in the directory where the Makefile is located.

    4. Load the module
    ```bash
    sudo insmod mymodule.ko
    ```

    5. Check the module
    Use `lsmod` to check if the module is loaded.

    6. Remove the module
    ```bash
    sudo rmmod mymodule
    ```

3. **How are mutex defined and used? How good is this approach?**

    1. There are 3 mutexes in `base.c`:
    ```c
    static DEFINE_MUTEX(dadfs_sb_lock);
    static DEFINE_MUTEX(dadfs_inodes_mgmt_lock);
    static DEFINE_MUTEX(dadfs_directory_children_update_lock);
    ```
    2. Separating function to three parts is clear.

4. **Based on the source code, how is information shared between the kernel and user spaces?**

    1. Using `mmap`, processes in user space can access the memory allocated by kernel.
    2. `mmap` will return a pointer to the memory to the process in user space. 

