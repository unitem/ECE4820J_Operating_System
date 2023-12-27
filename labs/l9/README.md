# Non-programming Tasks

1. **What needs to be returned by read and write file operations for a character device?**

The number of bytes read/written. -1 if fails.

2. **How are exactly those major and minor numbers working?**

Major Number: This number identifies the driver associated with the device. Each driver in the system is assigned a unique major number. When a device file is accessed, the major number tells the system which device driver to use for that particular device. For example, all devices controlled by the same disk driver will have the same major number.

Minor Number: The minor number is used by the driver to differentiate between the various devices it controls. While the major number identifies the driver, the minor number allows the driver to distinguish between multiple devices of the same type. For instance, if you have several disk drives managed by the same driver, each disk drive would have a different minor number.

3. **Knowing the major number and minor numbers of a device, how to add a character device to /dev?**

```C
cdev_add(struct cdev *dev, dev_t num, unsigned int num);
```

4. **Where are the following terms located in linux source code?**

1. module_init	include/linux/module.h
2. module_exit    include/linux/module.h
3. printk  /include/linux/printk.h
4. container_of    /include/linux/kernel.h
5. dev_t  /include/linux/types.h
6. MAJOR  /include/linux/kdev_t.h
7. MINOR  /include/linux/kdev_t.h
8. MKDEV  /include/linux/kdev_t.h
9. alloc_chrdev_region  /include/linux/fs.h
10. module_param  /include/linux/moduleparam.h
11. cdev_init   /include/linux/cdev.h
12. cdev_add  /include/linux/cdev.h
13. cdev_del  /include/linux/cdev.h
14. THIS_MODULE  /include/linux/module.h

5. **How to generate random numbers when working inside the Linux kernel?**

`get_random_bytes(void *buf, int nbytes)`

6. **How to define and specify module options?**

Define: use the MODULE_PARM() macro.

```C
#include <linux/moduleparam.h>
module_param(name, type, perm);
MODULE_PARM_DESC(name, "Description of the parameter"); // for documentation
```

Specify: 
```bash
sudo insmod mymodule.ko myparam=value
```
