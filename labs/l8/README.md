# Non-programming exercises

1. vm stands for virtual memory

    vm makes the programs think that they have more memory than they actually have. It is used when the programs use more memory than the physical memory. Pros: it can help the programs to run when they use more memory than the physical memory. Cons: it is slower than the physical memory.

2. In the kernel source code, the page table is defined in `src/servers/vm/pt.h` There are `1024` entries in a page dir (`src/include/arch/i386/include/vm.h`). An entry contains following fields:

    1. Frame number: physical frame (or page) number where the corresponding virtual page is stored in physical memory (RAM).
    2. Valid bit: whether the page is in RAM
    3. Read/Write Permission.
    4. User/Supervisor bit: whether the page can be accessed in user mode or only in supervisor mode.
    5. Dirty bit: whether the page has been written to.
    6. Accessed bit: whether the page has been read from or written to.
    7. Cache disable bit: whether caching is disabled for the page.

3. Basic functions used to handle vm: there are many functions used to handle vm in `src/servers/vm/`.

4. To find all the places where the vm used inside the kernel: use

```bash
find /usr/src -name "*.c" | xargs grep -l "vm"
```

It appears in so many different places all the processes need virtual memory and their corresponding virtual address to run.

5. `malloc` and  `calloc` are userspace functions. So minix uses `kmalloc`, `kfree`, `vmalloc` and  `vfree` for kernel security cause.

6.  When we follow the code procedure of `malloc`, we can find that when we allocate memories, the following functions are called one by one:
    
    1. `mmap`
    2. `do_mmap`
    3. `map_page_region`
    4. `map_ph_writept`
    5. `pt_writemap`

    These functions map a block memory assigned by the memory management and insert it to the page table and return the pointer of that block to the userspace. 
    
    `get_user_pages` are used to map userspace memory to the kernel.

    Good, since users can only access vm instead of changing memory directly. The security wall decreases the chances of memory changed accidentally. However, this will increase the time needed for memory management and does harm to performance.

7. Page fault handling:

OS first sent page fault message to `main.c`, then it checks if there exists available page frame in the physical memory. If so, it set up a mapping relationship between the page and the page frame. If not, it will first write back the lease frequently used page frame in the physical memory to the hard drive. After that, it reads the page to page frame.
