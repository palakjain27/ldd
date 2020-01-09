
/************************************************************************************************
@ileName : export.c
@author	 : Palak Jain
@teamLead: Rajesh Dommaraju 
@etails  : It is a real driver code, which communicates between user and kernel space we can do all open, read, write, close operations on device file.
@icense  : SpanIdea Systems Pvt. Ltd. All rights reserved.
************************************************************************************************/

/*******************************************************************************
			INCLUDES
*******************************************************************************/	
#include <linux/kernel.h>  // for kernel functions 
#include <linux/init.h>    //for int and exit functions 
#include <linux/module.h>  // for module info 
#include <linux/kdev_t.h>  //for device file creation 
#include <linux/fs.h>      //for file operations 
#include <linux/cdev.h>    //for major and minor no. registration
#include <linux/device.h>  //for device file creation
#include<linux/slab.h>     //kmalloc()
#include<linux/uaccess.h>  //copy_to/from_user()

/*******************************************************************************
			 LOCAL MACROS		
*******************************************************************************/

#define GPL_LICENSE				"GPL"
#define DRIVER_MODULE_AUTHOR			"Palak"
#define DRIVER_MODULE_DESC			"driver to communicate between user and kernel space"
#define DRIVER_MODULE_VERSION			"V1.3.0"
#define memory_size 				 1024

/*******************************************************************************
			 LOCAL TYPEDEFS		
*******************************************************************************/

/*******************************************************************************
			GLOBAL VARIABLES		
*******************************************************************************/
dev_t dev = 0;
static struct class *dev_class;
static struct cdev my_cdev;
uint8_t *kernel_buffer;
/*******************************************************************************
			LOCAL VARIABLES		
*******************************************************************************/

/*******************************************************************************
			LOCAL FUNCTIONS		
*******************************************************************************/
static int __init my_driver_init(void);
static void __exit my_driver_exit(void);
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t my_write(struct file *filp, const char *buf, size_t len, loff_t * off);
/*******************************************************************************
			STRUCTURES AND ENUMES		
*******************************************************************************/ 
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = my_read,
        .write          = my_write,
        .open           = my_open,
        .release        = my_release,
};
/*******************************************************************************
			EXPORT SNIPPET		
*******************************************************************************/
int count =0;
void my_shared_func(void)
{ 
	printk(KERN_INFO"shared function is been called!!!\n");
	count++;
}
//EXPORT_SYMBOL_GPL(my_shared_func);
EXPORT_SYMBOL(my_shared_func);
EXPORT_SYMBOL(count);
/**********************************************************************************************
function	 : my_open
description	 : This function is used to open the device file.
input param      : NONE
output param     : NONE
**********************************************************************************************/ 
static int my_open(struct inode *inode, struct file *file)
{
        /*Creating Physical memory*/
        if((kernel_buffer = kmalloc(memory_size , GFP_KERNEL)) == 0){
            printk(KERN_INFO "Cannot allocate memory in kernel\n");
            return -1;
        }
        printk(KERN_INFO "Device File Opened...!!!\n");
        return 0;
}
/**********************************************************************************************
function	 : my_release
description	 : This operation is invoked when the file structure is being released.
input param      : NONE
output param     : NONE
**********************************************************************************************/ 
static int my_release(struct inode *inode, struct file *file)
{
        kfree(kernel_buffer);
        printk(KERN_INFO "Device File Closed...!!!\n");
        return 0;
}

/**********************************************************************************************
function	 : my_read
description	 : This function is used while taking data from kernel to user space.
input param      : NONE
output param     : NONE
**********************************************************************************************/
static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        copy_to_user(buf, kernel_buffer, memory_size);
        printk(KERN_INFO "Data Write : Done!\n");
        return memory_size;
}
/**********************************************************************************************
function	 : my_write
description	 : Sends data from user space to the kernel space.
input param      : NONE
output param     : NONE
**********************************************************************************************/
static ssize_t my_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        copy_from_user(kernel_buffer, buf, len);
        printk(KERN_INFO "Data Read : Done!\n");
        return len;
}
/**********************************************************************************************
function	 : my_driver_init
description	 : This function is initialised when module gets inserted.
		   This function is passed as an argument to module_init.
input param      : NONE
output param     : NONE
**********************************************************************************************/
static int __init my_driver_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "my_Dev")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

        /*Creating cdev structure*/
        cdev_init(&my_cdev,&fops);
        my_cdev.owner = THIS_MODULE;
        my_cdev.ops = &fops;

        /*Adding character device to the system*/
        if((cdev_add(&my_cdev,dev,1)) < 0){
            printk(KERN_INFO "Cannot add the device to the system\n");
            unregister_chrdev_region(dev,1);
        }

        /*Creating struct class*/
        if((dev_class = class_create(THIS_MODULE,"my_class")) == NULL){
            printk(KERN_INFO "Cannot create the struct class\n");
            unregister_chrdev_region(dev,1);
        }

        /*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"my_device")) == NULL){
            printk(KERN_INFO "Cannot create the Device 1\n");
           class_destroy(dev_class);
        }
        printk(KERN_INFO "Device Driver Insert...Done!!!\n");
	return 0;


return -1;
}
/**********************************************************************************************
function	 : my_driver_exit
description	 : This function is initialised when module gets removed.
		   This function is passed as an argument to module_exit.
input param      : NONE
output param     : NONE
**********************************************************************************************/ 
void __exit my_driver_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}
 
module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE(GPL_LICENSE);
MODULE_AUTHOR(DRIVER_MODULE_AUTHOR);
MODULE_VERSION(DRIVER_MODULE_VERSION);
MODULE_DESCRIPTION(DRIVER_MODULE_DESC);
