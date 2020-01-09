/************************************************************************************************
@ileName : sysfs.c
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
#include<linux/sysfs.h> 
#include<linux/kobject.h> 
 
 
volatile int my_value = 0;
 
struct kobject *kobj_ref;
/*******************************************************************************
			LOCAL FUNCTIONS		
*******************************************************************************/
static int my_driver_init(void);
static void my_driver_exit(void);
  
/*************** Sysfs Fuctions **********************/
static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
 
struct kobj_attribute my_attr = __ATTR(my_value, 0660, sysfs_show, sysfs_store);
 
/**********************************************************************************************
function	 : sysfs_show
description	 : This function is used to read in proc device file.
input param      : NONE
output param     : NONE
**********************************************************************************************/ 
static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
        printk(KERN_INFO "Sysfs - Read!!!\n");
        return sprintf(buf, "\n%d\n", my_value);//The purpose is to copy the value read back from the device to the sysfs file created for this device attribute. 
}
/**********************************************************************************************
function	 : sysfs_store
description	 : This function is used to write in proc device file.
input param      : NONE
output param     : NONE
**********************************************************************************************/  
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
        printk(KERN_INFO "Sysfs - Write!!!\n");
        sscanf(buf,"%d",&my_value);
        return count;
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
        /*Creating a directory in /sys/kernel/ */
        kobj_ref = kobject_create_and_add("my_sysfs",kernel_kobj);
 
        /*Creating sysfs file for etx_value*/
        if(sysfs_create_file(kobj_ref,&my_attr.attr)){
                printk(KERN_INFO"Cannot create sysfs file......\n");
                kobject_put(kobj_ref);
		sysfs_remove_file(kernel_kobj,&my_attr.attr);
    }
        printk(KERN_INFO "Device Driver Insert...Done!!!\n");
    return 0;
}
 
/**********************************************************************************************
function	 : my_driver_exit
description	 : This function is initialised when module gets removed.
		   This function is passed as an argument to module_exit.
input param      : NONE
output param     : NONE
**********************************************************************************************/ 
static void my_driver_exit(void)
{
        kobject_put(kobj_ref); 
        sysfs_remove_file(kernel_kobj, &my_attr.attr);
        printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}
 
module_init(my_driver_init);
module_exit(my_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("palak");
MODULE_DESCRIPTION("A simple device driver - SysFs");
MODULE_VERSION("1.8");
