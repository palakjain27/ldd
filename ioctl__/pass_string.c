
/************************************************************************************************
@ileName : ioctl_command.c
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
#include <linux/ioctl.h>	       //for ioctl command

/*******************************************************************************
			 LOCAL MACROS		
*******************************************************************************/

#define GPL_LICENSE				"GPL"
#define DRIVER_MODULE_AUTHOR			"SPANIDEA"
#define DRIVER_MODULE_DESC			"ioctl"
#define DRIVER_MODULE_VERSION			"V1.0"
#define WR_VALUE _IOW('4','b',char*)
#define RD_VALUE _IOR('4','c',char*)
#define WR_STRING _IOW('4','d',char*)
#define RD_STRING _IOR('4','e',char*)
#define OPERATIONS _IOWR('4','f',int*)

/*******************************************************************************
			 LOCAL TYPEDEFS		
*******************************************************************************/

/*******************************************************************************
			GLOBAL VARIABLES		
*******************************************************************************/
dev_t dev = 0;
static struct class *dev_class;
static struct cdev my_cdev;
/*******************************************************************************
			LOCAL VARIABLES		
*******************************************************************************/
char value ;
char data[20];
int a;
int count ;
/*******************************************************************************
			LOCAL FUNCTIONS		
*******************************************************************************/
static int my_driver_init(void);
static void my_driver_exit(void);
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
/*******************************************************************************
			STRUCTURES AND ENUMES		
*******************************************************************************/ 
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .open           = my_open,
        .unlocked_ioctl = my_ioctl,
        .release        = my_release,
};
/**********************************************************************************************
function	 : my_open
description	 : This function is used to open the device file.
input param      : NONE
output param     : NONE
**********************************************************************************************/ 
static int my_open(struct inode *inode, struct file *file)
{ 
        printk(KERN_INFO "Device File Opened !\n");
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
        printk(KERN_INFO "Device File Closed !\n");
        return 0;
}

/**********************************************************************************************
function	 : my_ioctl
description	 : Sends and receives data to the device.
input param      : NONE
output param     : NONE
**********************************************************************************************/
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
         switch(cmd) {
                case WR_VALUE:
                        copy_from_user(&value ,(char*) arg, sizeof(value));
                        break;
                case RD_VALUE:
                        copy_to_user((char*) arg, &value, sizeof(value));
                        break;
		case RD_STRING:
                        copy_to_user((char*) arg, &data, sizeof(data));
                        break;
		case WR_STRING:
			copy_from_user(&data ,(char*) arg, sizeof(data));
                        break;
		case OPERATIONS:
			copy_from_user(&a ,(int*) arg, sizeof(a));
			
			if (a%2==0)
			{
			printk("no is even\n");
			
			}	
			else{
			printk("no is odd\n");
			
			}
			copy_to_user((int*) arg, &a, sizeof(a));
			printk("%d",a);
			break;
			
        }
        return 0;
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
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "my_Dev")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

        /*Creating cdev structure*/
        cdev_init(&my_cdev,&fops);
        

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
        printk(KERN_INFO "Device makDriver Inserted!\n");
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
