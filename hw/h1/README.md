This file includes the non-programming exercises in hw1.

# Ex. 1

stack: 
1. store function call frames and local variables.
2. LIFO: last in first out.
3. size determined at compile-time.

heap:
1. store dynamic memory allocation.
2. no specific order of allocation and deallocation.
3. size determined by OS or the programming language runtime

# Ex. 2

### Operations performed when a computer is powered on

When a computer is powered on, it should initialize the hardware, load the operating system, and prepare the computer for user interaction.
1. The power supply unit (`PSU`) provides electrical power to the various components of the computer, including the motherboard, CPU, RAM, and other devices.
2. BIOS initialize. BIOS is a firmware code, stored in a non-volatile memory chip on the motherboard, containing instructions for initializing hardware components.
3. BIOS performs a POST (Power-On Self-Test). It performs a series of diagnostic tests on the computer's hardware to ensure that it is functioning correctly. These tests check the CPU, RAM, storage devices, and other critical components. If any issues are detected, error codes or messages may be displayed.
4. BIOS/UEFI identifies and detects the boot devices, such as hard drives, solid-state drives, optical drives, USB drives, or network boot options. It determines the boot order based on the BIOS settings.
5. BIOS/UEFI loads and executes the boot loader program stored in the Master Boot Record (MBR) or EFI System Partition (ESP) of the boot device. The boot loader's job is to load the operating system kernel into RAM (Random Access Memory).
6. The operating system kernel initializes hardware components, sets up device drivers, establishes memory management, and starts system processes. It also mounts the root file system, which contains the core of the operating system and user programs.
7. The init system (such as systemd or init) is the first user-space process, responsible for initializing user-space services, daemons, and processes.
8. Finally, the user is presented with a login prompt in text mode or a graphical login screen. After logging in, the user can start using the computer.

###  The role of the BIOS and how does it interact with the OS

The role of the BIOS (Basic Input/Output System) includes:

1. Hardware Initialization: see above 2&3.

2. Boot Device Detection: see above 4.

3. Boot Loader Execution: see above 5.

4. CMOS/UEFI Configuration: The BIOS/UEFI firmware also stores various system settings and configuration options in a small memory chip known as CMOS (Complementary Metal-Oxide-Semiconductor) or NVRAM (Non-Volatile Random-Access Memory). These settings include the date and time, hardware settings, and boot device order. Users can access and modify these settings through the BIOS setup utility during the boot process.

Its interaction with the OS:

The BIOS serves as the bridge between the hardware and the operating system during the boot process, and provides a way for users to configure system settings. Once the BIOS has successfully loaded the OS kernel into memory and handed control over to the kernel, the OS takes over. The OS then continues the device initialization and user space initialization process.

### Hybrid and exo kernels

Hybrid kernel combines elements of both monolithic kernels and microkernels. It seeks to strike a balance between the two design philosophies. Some kernel functions run in the privileged kernel mode like a monolithic kernel. Other functions run in user mode, as separate processes or user-level server modules, like a microkernel. It uses interprocess communication (IPC) mechanism to allow user-mode components to interact with the kernel safely.

Exo kernel is different from traditional kernel designs. It's minimalistic, offering low-level abstractions for managing hardware resources, exposing hardware resources directly to applications, including CPU scheduling, memory management, and I/O operations. A library operating system (libOS) is implemented as a set of user-level libraries. These libOSes provide abstractions and services, such as file systems, network stacks, and process management, that are traditionally part of the kernel in monolithic systems.

# Ex. 3

1. a), c), and d). 

For a), disabling interrupts prevents the CPU from responding to hardware and software interrupts. If user-level code could disable interrupts, it would have the potential to disrupt the entire system's operation, leading to instability and potential security vulnerabilities. For b), it is generally a non-privileged operation that user-level programs can perform to retrieve system time information. For c), modifying the system time is a sensitive operation that can affect various aspects of system functionality, including file timestamps, scheduling, and security protocols. For d), Modifying the memory map involves altering the memory allocation and access permissions, which has far-reaching consequences for system stability and security. 

2. It depends on how they scheduled. If one cpu gets the 5 and 10 ms, the other gets 20 ms, the total takes 20. If one cpu gets 5 and 20 ms, the other gets 10ms, the total takes 25 ms. If one cpu gets the 10 and 20 ms, the other gets 5, the total takes 30 ms. 

# Ex. 4


