# Non-programming Exercises.

## Ex. 1 - Input/Output

1. Theoretically, it is possible, if the speed of scan is accelerated and matches the speed of the wireless network. However, in practice, it is impossible, because the speed of scan needs calculation, much slower than the speed of wireless network.

2. Use a loadable driver. It can load and unload modules dynamically.

3. layers:

a) Computing the track, sector, and head for a disk read - device independent OS software
b) Writing command to the device registers - device driver layer
c) Checking is the user is allowed to use the device - interrupt handler
d) Converting binary integers to ASCII for printing - user level IO software

4. (32bit * 2 / 4) / 100nsec = 16 * 10^7 bits/sec = 2 * 10^7 B/s

5. A thin client is a computer that runs depending on a remote server instead of a localized hard drive, where most applications, sensitive data, and memory, are stored. It is thin and easy to maintain, but working heavily depending on network connection. 

6. A hard disk is a data storage device that uses magnetic storage to store and retrieve digital information. It contains platters, R/W heads, actuator arm, spindle and motor. To read data, the HDD spins the platters at a high speed, and the read/write heads are positioned over the appropriate track. Writing data involves altering the magnetic orientation of tiny regions on the platter. The geometry of a hard disk refers to its physical characteristics, such as the number of platters, the arrangement of tracks and sectors, and the size of the storage capacity. 

7. RAID (Redundant Array of Independent Disks) is a technology that combines multiple physical drives into a single logical unit to provide improved performance, fault tolerance, or a combination of both. Here are the key differences between RAID 0, 1, 5, 6, and 10:

1. RAID 0:

- **Striping:**
  - Data is divided into blocks, and each block is written to a separate disk drive.
- **Performance:**
  - Improved read and write performance because multiple drives can be accessed simultaneously.
- **Fault Tolerance:**
  - No fault tolerance; if one drive fails, all data is lost.
- **Capacity Utilization:**
  - 100% capacity utilization, as there is no mirroring or parity.

2. RAID 1:

- **Mirroring:**
  - Data is duplicated on two or more drives (mirrored).
- **Performance:**
  - Read performance is improved because data can be read from multiple drives simultaneously. Write performance is typically the same as that of a single drive.
- **Fault Tolerance:**
  - Good fault tolerance; if one drive fails, data is still accessible from the mirrored drive(s).
- **Capacity Utilization:**
  - 50% capacity utilization, as data is duplicated on each drive.

3. RAID 5:

- **Striping with Parity:**
  - Data is striped across multiple drives, and parity information is distributed across all drives.
- **Performance:**
  - Good read performance; moderate write performance due to parity calculations.
- **Fault Tolerance:**
  - Good fault tolerance; if one drive fails, data can be reconstructed from parity information.
- **Capacity Utilization:**
  - (n-1) capacity utilization, where n is the number of drives.

4. RAID 6:

- **Double Parity:**
  - Similar to RAID 5, but with additional parity information. It can tolerate the failure of two drives.
- **Performance:**
  - Similar to RAID 5 but with a higher overhead for parity calculations.
- **Fault Tolerance:**
  - High fault tolerance; can withstand the failure of up to two drives simultaneously.
- **Capacity Utilization:**
  - (n-2) capacity utilization, where n is the number of drives.

5. RAID 10 (RAID 1+0):

- **Combination of RAID 1 and RAID 0:**
  - Data is mirrored first and then striped.
- **Performance:**
  - Excellent read and write performance due to both mirroring and striping.
- **Fault Tolerance:**
  - High fault tolerance; can withstand the failure of one drive in each mirrored pair.
- **Capacity Utilization:**
  - 50% capacity utilization, as data is mirrored and then striped.

6. Summary:

- RAID 0 provides performance improvement but no fault tolerance.
- RAID 1 provides fault tolerance through mirroring.
- RAID 5 and RAID 6 offer a balance of performance and fault tolerance through striping with parity.
- RAID 10 combines mirroring and striping for high performance and fault tolerance.

The choice of RAID level depends on the specific requirements for performance, fault tolerance, and capacity utilization in a given storage environment.

## Ex. 2 - Multiprocessors

1. It could be used by:
    1. preloading a 1 into the register to be used. 
    2. `SWP` this register and the memory word
    3. lock the memory word (if it was locked, the caller must loop)
    4. If it was previously unlocked, it is now locked.

2. virtualization

Virtualization means the act of creating a virtual version of something, usually hardware. It includs virtual computer hardware platforms, storage devices, and computer network resources. The main challenges are:
    1. Resource distribution
    2. VM Sprawl
    3. Backward compatibility
    4. Performance monitoring
    5. Backup
    6. Security

## Ex. 3 - File Systems

1. /usr/lib/mutt

2. One block can contain 1KB / 4B = 256 block addresses, pointing to 256 * 1KB = 256KB data. So the maximum file size is 13KB + 256KB + 256*256KB + 256*256*256KB = 16GB.

3. Average rotation latency: 1/2 * 1 / (15000 / 60) = 0.002s = 2ms

(i) 1KB: 8 + 2 + 1024 / 262144 * 1000 = 13.9 ms
(ii) 2KB: 8 + 2 + 2048 / 262144 * 1000 = 17.8 ms
(ii) 4KB: 8 + 2 + 4096 / 262144 * 1000 = 25.6 ms

## Ex. 4 - Security

1. 208,827,064,576; Because if the user must set password using lower and upper letters, numbers, and symbols, there are 26 + 26 + 10 + 32 = 94 characters to choose from. So the number of possible passwords would be approximatedly about 94^8, which is much larger.

2. The main reason is that the `malloc` will recycle memory that was previous allocated and freed from the same process and then request new page(s) from the operating system.
    1. In the first case, the memory will contain data leftover from previous allocations. So it won’t be zero. This is the usual case when performing small allocations.
    2. In the second case, the memory will be from the OS. This happens when the program runs out of memory, e.g. when we are requesting a very large allocation. When the OS gives us memory, it could have been freed from a different process. So that memory could contain sensitive information such as a password. So to prevent attackers reading such data, the OS will zero it before it gives it to us.

3. Not very secure, since it can be attacked at its weakest part.

4. No.  every system has flaws which could probably be exploited by attackers. We cannot put all the resources to maintain security and we can only improve the security level.


