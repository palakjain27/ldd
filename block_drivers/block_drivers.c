#include <linux/module.h>  
#include <linux/fs.h>			//register_blkdev() and struct block_device_operations  
#include <linux/errno.h>  
#include <linux/blk_types.h>  
#include <linux/vmalloc.h>  
#include <linux/genhd.h>  		//gendisk structure
#include <linux/blkdev.h>    	        //blk_init_queue ,struct request_queue
#include <linux/hdreg.h>  
#include <linux/device.h>
#include <linux/uaccess.h>  
#include <linux/blkpg.h>  
#include <linux/version.h>
#include <linux/bio.h>			//bio structure
#include <linux/ioctl.h>
#include<linux/moduleparam.h>           //parameter passing 

/********************************************************************************/
/*                        DEFINES                                               */
/********************************************************************************/
#define	SBULL_MINOR			 0
#define	KERNEL_SECTOR_SIZE 	         512 //kernel sector size 
#define SBULL_NAME 		       "sbull"
//#define GET_DEVICE_ID 			0xAA00 		/* Ioctl command definition */

/********************************************************************************/
/*                        GLOBALS                                               */
/********************************************************************************/
static int sbull_major 		= 0;
//static int nsectors 		= 1024;		//size of the disk
static int hardsect_size 	= 512;  //device hard sector size  

int size = 2048;
int blksize = 1024;

static int v=0;//no of sectors //size of disk
static struct request_queue *Queue;

/********************************************************************************/
/*                        DEFINITION                                            */
/********************************************************************************/
static void sbull_request_fn(struct request_queue *q);

//creating the device structure
static struct sbull_dev 
{
    unsigned int size;             /* Device size in sectors */
    u8 *data;                       /* The data array */
    spinlock_t lock;                /* For mutual exclusion */
    struct gendisk *gd;             /* The gendisk structure */ //kernel understanding of block device 
}dev;

module_param(v, int, S_IRUSR|S_IWUSR);                      //integer value
/*----------------------Module_param_cb()--------------------------------
int notify_param(const char *val, const struct kernel_param *kp)
{
        int res = param_set_int(val, kp); // Use helper for write variable
        if(res==0) {
                printk(KERN_INFO "Call back function called...\n");
                printk(KERN_INFO "New value of my_cb_value = %d\n", my_cb_value);
                return 0;
        }
        return -1;
}
// to get notification whenever value got change. we need to register our handler function to its file operation structure.
const struct kernel_param_ops my_param_ops = 
{
        .set = &notify_param, // Use our setter ...
        .get = &param_get_int, // .. and standard getter
};

module_param_cb(my_cb_value, &my_param_ops, &my_cb_value, S_IRUGO|S_IWUSR );*/
/********************************************************************************/
/*      Fun Name: deviceOpen()                                                  */
/*      Fun Desc: performs operation when device file is opened                 */
/*      Fun Param: inode structure, file structure                              */
/*      Return Type: int returns 0 on success                                   */
/********************************************************************************/
static int sbull_open(struct block_device *b, fmode_t mode)
{
	printk(KERN_INFO "rb: Device is opened \n");
	return 0;
}

/********************************************************************************/
/*      Fun Name: deviceClose()                                                 */
/*      Fun Desc: performs operation when device file is closed                 */
/*      Fun Param: inode structure, file structure                              */
/*      Return Type: int returns 0 on success                                   */
/********************************************************************************/
static void sbull_release(struct gendisk *g, fmode_t mode)
{
	printk(KERN_INFO "sbull_release function \n");
}
/********************************************************************************/
/*      Fun Name: ioctl()                                                 */
/*      Fun Desc: performs operation when device file is closed                 */
/*      Fun Param: inode structure, file structure                              */
/*      Return Type: int returns 0 on success                                   */
/********************************************************************************/
/*static int sbull_ioctl (struct inode *inode, struct file *file,unsigned int cmd, unsigned long arg)
{
		 switch(cmd) {
		case WR_VALUE:
                        copy_from_user(&v ,(int*) arg, sizeof(v));
                        break;
                case RD_VALUE:
                        copy_to_user((int*) arg, &v, sizeof(v));
			break;*//*unsigned char status;
	switch (cmd) {
	case GET_DEVICE_ID:
		outb(GET_IDENTITY_CMD, COMMAND_REGISTER);
		// Wait as long as the controller is busy 
		while ((status = inb(STATUS_REGISTER)) & BUSY_STATUS);
		// Obtain ID and return it to user space 
		return put_user(inb(DATA_REGISTER), (long __user *)arg);
	default:
	   return -EINVAL;
	}
}*/
//Block device operation structure 
struct block_device_operations sbull_operations = 
{
	.owner 		= THIS_MODULE,
	.open 		= sbull_open,
	.release 	= sbull_release,
	//.ioctl   	= sbull_ioctl,
};

/********************************************************************************/
/*      Fun Name: sbull_transfer()                                              */
/*      Fun Desc: perform the data tranfer operation 		                */
/*      Fun Param: request structure                             		*/
/*      Return Type: int returns 0 on success                                   */
/********************************************************************************/
static int sbull_transfer(struct request *req)
{
	int dir, size;
	unsigned int max_sectors;
	struct req_iterator iter;//processing sectors
	struct bio_vec bv;
	char *buffer, *addr;
//	sector_t sector_offset;
	sector_t start_sector = blk_rq_pos(req);
	max_sectors = blk_rq_sectors(req);			//total number of sector to process

	printk(KERN_INFO "sbull_transfer initialized...\n");

	//request direction, write of read ...
	dir = rq_data_dir(req);
	//sector_offset = 0;

	//calculating the address where we need to read and write ...
	//addr = dev.data + start_sector + sector_offset;
	addr = dev.data + start_sector;

	//iterate over all segments of bio ...
	rq_for_each_segment(bv, req, iter)
	{
		size = bv.bv_len;		//size of current segment ...

	
		buffer = page_address(bv.bv_page) + bv.bv_offset;	
		
		if (dir == WRITE)
		{
			printk(KERN_INFO "Writing to disk device...\n");
			printk(KERN_INFO "Memory Address: %p\n", addr);
			memcpy(addr, buffer, size);
			printk(KERN_INFO "Data: %s", addr);
		}
		else
		{
			printk(KERN_INFO "Reading from disk device...\n");			
			memcpy(buffer, addr, size);
			printk(KERN_INFO "Memory Address: %p\n", addr);
			printk(KERN_INFO "%s", buffer);
		}
		addr += size;
	}

	printk(KERN_INFO "sbull_transfer completed\n");
	return 0;
}

/********************************************************************************/
/*      Fun Name: sbull_request_fn()                                            */
/*      Fun Desc: performs operation when the function is scheduled to 	        */
/*					the request queue                	*/
/*      Fun Param: request_queue pointer 		                        */
/*      Return Type: void 					                */
/********************************************************************************/
static void sbull_request_fn(struct request_queue *q)
{
	struct request *req;
	int ret_value;

	printk(KERN_INFO "sbull_request_fn funtion\n");

	//fetch the request from the request queue and process the request ...
	while((req = blk_fetch_request(q)) != NULL)
	{
		//check if it is a read/write request or not ... 
		if (!blk_account_rq(req))
		{
			//skip the non-fs operation ...
			printk(KERN_INFO "Skip non-fs request\n");
			__blk_end_request_all(req, 0);
			continue;
		}

		//process the request
		ret_value = sbull_transfer(req);
		__blk_end_request_all(req, ret_value);
	}
}

/********************************************************************************/
/*      Fun Name: blk_driver_init()                                             */
/*      Fun Desc: performs operation when module is inserted                    */
/*      Fun Param: void 						        */
/*      Return Type: void 					                */
/********************************************************************************/
static int blk_driver_init( void )
{
	printk(KERN_INFO "blk_driver_init function initialized ..\n");
	 printk(KERN_INFO "my_value = %d  \n", v);
        //printk(KERN_INFO "my_cb_value = %d  \n", my_cb_value);
	//setting up our device information...
	//setting the size of the device with total number of sectors multiplied by hard sector size of each sector
	dev.size = v * hardsect_size;//nsectors * hardsect_size;

	//creating a virtual memory of the total memory of the device ...
	dev.data = vmalloc(dev.size);
	if (dev.data == NULL)
	{
		printk(KERN_NOTICE "vmalloc failure. \n");
		return -ENOMEM;
	}
	memset(dev.data, 0, dev.size);

	//setting up spinlock in the device
	spin_lock_init(&dev.lock);//no two process can simultaneously access the queue

	//initializing the request queue..
	Queue = blk_init_queue(sbull_request_fn, &dev.lock);
	if (Queue == NULL)	
	{
		printk(KERN_INFO "Queue initialization failed ...\n");
		vfree(dev.data);
		return -ENOMEM;
	}

	//registering our block device driver...
	sbull_major = register_blkdev(sbull_major, SBULL_NAME);
	if (sbull_major <= 0)
	{
		printk(KERN_WARNING "sbull: unable to get major number\n");
		vfree(dev.data);
		return -EBUSY;
	}

	//allocating the memory for the disk ...
	dev.gd = alloc_disk(1);					//1:num of minor number device supports
	if (!dev.gd)
	{
		printk(KERN_NOTICE "alloc_disk failure\n");
		blk_cleanup_queue(Queue);
		unregister_blkdev(sbull_major, SBULL_NAME);
		vfree(dev.data);
		return -ENOMEM;
	}

	dev.gd->major = sbull_major;
	dev.gd->first_minor = SBULL_MINOR; 		 
	dev.gd->fops = &sbull_operations;
	dev.gd->queue = Queue;
	dev.gd->private_data = &dev;//can be used to store a pointer to some private information for the disk
	strcpy(dev.gd->disk_name, SBULL_NAME);
	//set_capacity(dev.gd, nsectors * (hardsect_size/KERNEL_SECTOR_SIZE));
	set_capacity(dev.gd, v * (hardsect_size/KERNEL_SECTOR_SIZE));
	//adding the disk ...
	add_disk(dev.gd);

	printk(KERN_INFO "blk_driver_init function completed with %d sectors and %d bytes\n", dev.size, dev.size * KERNEL_SECTOR_SIZE);

	return 0;
}

/********************************************************************************/
/*      Fun Name: blk_driver_exit()                                             */
/*      Fun Desc: performs operation when module is removed 	                */
/*      Fun Param: void 						        */
/*      Return Type: void 					                */
/********************************************************************************/

static void blk_driver_exit( void )
{
	printk(KERN_INFO "blk_driver_exit function initialized\n");
	del_gendisk(dev.gd);
	put_disk(dev.gd);
	unregister_blkdev(sbull_major, SBULL_NAME);
	blk_cleanup_queue(Queue);
	vfree(dev.data);
	printk(KERN_INFO "blk_driver_exit function completed\n");

}

module_init(blk_driver_init);
module_exit(blk_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OMESH SUDHAKAR");
