/************************************************************************************************
@ileName : proc.c
@author	 : Palak Jain
@teamLead: Rajesh Dommaraju 
@etails  : It is a real driver code, which communicates between user and kernel space we can do all open, read, write, close operations on device file.
@icense  : SpanIdea Systems Pvt. Ltd. All rights reserved.
************************************************************************************************/

/*******************************************************************************
			INCLUDES
*******************************************************************************/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/ioctl.h>
#include<linux/proc_fs.h>
/*******************************************************************************
			GLOBAL VARIABLES		
*******************************************************************************/
char my_array[20]="proc_array\n";
static int len = 1;

/*******************************************************************************
			LOCAL FUNCTIONS		
*******************************************************************************/
static int my_driver_init(void);
static void my_driver_exit(void);
 
/***************** Procfs Functions *******************/
static int open_proc(struct inode *inode, struct file *file);
static int release_proc(struct inode *inode, struct file *file);
static ssize_t read_proc(struct file *filp, char __user *buffer, size_t length,loff_t * offset);
static ssize_t write_proc(struct file *filp, const char *buff, size_t len, loff_t * off);
 
/*******************************************************************************
			STRUCTURES AND ENUMES		
*******************************************************************************/  
static struct file_operations proc_fops = {
        .open = open_proc,
        .read = read_proc,
        .write = write_proc,
        .release = release_proc
};
/**********************************************************************************************
function	 : open_proc
description	 : This function is used to open the device file.
input param      : NONE
output param     : NONE
**********************************************************************************************/  
static int open_proc(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "proc file opend.....\t");
    return 0;
}
/**********************************************************************************************
function	 : release_proc
description	 : This operation is invoked when the file structure is being released.
input param      : NONE
output param     : NONE
**********************************************************************************************/  
static int release_proc(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "proc file released.....\n");
    return 0;
}
/**********************************************************************************************
function	 : read_proc
description	 : This function is used while taking data from kernel to user space.
input param      : NONE
output param     : NONE
**********************************************************************************************/ 
static ssize_t read_proc(struct file *filp, char __user *buffer, size_t length,loff_t * offset)
{
    printk(KERN_INFO "proc file read.....\n");
 if(len)
        len=0;
    else{
        len=1;
        return 0;
    }
	
    copy_to_user(buffer,my_array,20);
    memset(my_array,0,20); 
    return length;;
}
/**********************************************************************************************
function	 : write_proc
description	 : Sends data from user space to the kernel space.
input param      : NONE
output param     : NONE
**********************************************************************************************/
static ssize_t write_proc(struct file *filp, const char *buff, size_t len, loff_t * off)
{
    printk(KERN_INFO "proc file wrote.....\n");
    copy_from_user(my_array,buff,len);
    return len;
}
/**********************************************************************************************
function	 : my_driver_init
description	 : This function is initialised when module gets inserted.
		   This function is passed as an argument to module_init.
input param      : NONE
output param     : NONE
**********************************************************************************************/      
static int my_driver_init(void)
{
 
        /*Creating Proc entry*/
        proc_create("my_proc",0666,NULL,&proc_fops);
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
void my_driver_exit(void)
{
    remove_proc_entry("my_proc",NULL);
    printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}
 
module_init(my_driver_init);
module_exit(my_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Palak jain");
MODULE_DESCRIPTION("A simple proc device driver");
MODULE_VERSION("1.6");
