                                                                             Block Driver
                                                                             **********

Working with block devices is therefore more complicated than working with character devices. Character devices have a single current position, while block devices must be able to move to any position in the device to provide random access to data. To simplify work with block devices, the Linux kernel provides an entire subsystem called the block I/O (or block layer) subsystem.
From the kernel perspective, the smallest logical unit of addressing is the block. Although the physical device can be addressed at sector level, the kernel performs all disk operations using blocks. Since the smallest unit of physical addressing is the sector, the size of the block must be a multiple of the size of the sector. Additionally, the block size must be a power of 2 and can not exceed the size of a page. The size of the block may vary depending on the file system used, the most common values being 512 bytes, 1 kilobytes and 4 kilobytes.



programming 

register a block i/o device 
****************************
To register a block I/O device, function register_blkdev() is used. To deregister a block I/O device, function unregister_blkdev() is used.
Block devices are characterized by random access to data organized in fixed-size blocks.
The only operations performed by this function are the dynamic allocation of a major (if the major argument is 0 when calling the function) and creating an entry in /proc/devices.


register a disk 
********************************

Although the register_blkdev() function obtains a major, it does not provide a device (disk) to the system. For creating and using block devices (disks), a specialized interface defined in linux/genhd.h is used.

The useful functions defined in linux/genhd.h are to register /allocate a disk, add it to the system, and de-register /unmount the disk.

The alloc_disk() function is used to allocate a disk, and the del_gendisk() function is used to deallocate it. Adding the disk to the system is done using the add_disk() function.

The alloc_disk() and add_disk() functions are typically used in the module initialization function, and the del_gendisk() function in the module exit function.

gendisk structure
******************************************************
The structure representing a single block device.
The struct gendisk structure stores information about a disk. As stated above, such a structure is obtained from the alloc_disk() call and its fields must be filled before it is sent to the add_disk() function.
major minor no 
disk_name
fops
queue
capacity
private_data

struct block_device_operations structure
***********************************************************************
file operation like open, release, ioctl,direct access, media changed,revalidate disk, getgeo
open() and release() operations are called directly from user space by utilities that may perform the following tasks: partitioning, file system creation, file system verification.
read write are used from request function.

request queues
******************************
Drivers for block devices use queues to store the block requests I/O that will be processed. A request queue is represented by the struct request_queue structure.
Request queues implement an interface that allows the use of multiple I/O schedulers. A scheduler must sort the requests and present them to the driver in order to maximize performance.

The blk_init_queue() function receives as first argument a pointer to the function which processes the requests for the device (of type request_fn_proc). In the example above, the function is my_block_request(). The lock parameter is a spinlock (initialized by the driver) that the kernel holds during the request() function call to ensure exclusive access to the queue. This spinlock can also be used in other driver functions to protect access to shared data with the request() function.

The function of type request_fn_proc is used to handle requests for working with the block device.The function receives the request queue associated with the device as an argument and can use various functions for processing the requests from the request queue.



 




