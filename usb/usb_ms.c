/************************************************************************************************
@ileName : usb_ms.c
@author	 : Palak Jain
@teamLead: Rajesh Dommaraju 
@etails  : It is a real driver code, which communicates between user and kernel space we can do all open, read, write, close operations on device file.
@icense  : SpanIdea Systems Pvt. Ltd. All rights reserved.
************************************************************************************************/

/*******************************************************************************
			INCLUDES
*******************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include<linux/uaccess.h>              //copy_to/from_user()
/*******************************************************************************
			 LOCAL MACROS		
*******************************************************************************/
#define mem_size       100
#define BULK_EP_OUT    0x02
#define BULK_EP_IN     0x81
#define MAX_PKT_SIZE   512
/*******************************************************************************
			GLOBAL VARIABLES		
*******************************************************************************/
static int len = 1;

static struct usb_device *device;
static struct usb_class_driver skel_class;
static unsigned char bulk_buf[MAX_PKT_SIZE];

/**********************************************************************************************
function	 : pen_open
description	 : This function is used to open the device file.
input param      : NONE
output param     : NONE
**********************************************************************************************/ 
static int pen_open(struct inode *i, struct file *f)
{
    	printk(KERN_ALERT "Open function\n" );
    	return 0;
}
/**********************************************************************************************
function	 : pen_close
description	 : This function is used to close the device file.
input param      : NONE
output param     : NONE
**********************************************************************************************/ 
static int pen_close(struct inode *i, struct file *f)
{
	printk(KERN_ALERT "Close function\n" );
        return 0;
}
/**********************************************************************************************
function	 : pen_read
description	 : This function is used while taking data from kernel to user space.
input param      : NONE
output param     : NONE
**********************************************************************************************/
static ssize_t pen_read(struct file *f, char __user *buf, size_t cnt, loff_t *off)
{
    printk(KERN_ALERT "Read function\n" );
    if(len)
        len=0;
    else{
        len=1;
        return 0;
    }

    if (copy_to_user(buf, bulk_buf, mem_size))
    {
        return -EFAULT;
    }

    return mem_size;
}
/**********************************************************************************************
function	 : pen_write
description	 : Sends data from user space to the kernel space.
input param      : NONE
output param     : NONE
**********************************************************************************************/
static ssize_t pen_write(struct file *f, const char __user *buf, size_t cnt, loff_t *off)
{
    printk(KERN_ALERT "Write function\n" );

    if (copy_from_user(bulk_buf, buf, cnt))
    {
        return -EFAULT;
    }

    return cnt;
}
/*******************************************************************************
			STRUCTURES AND ENUMES		
*******************************************************************************/ 
static struct file_operations skel_fops =
{
    .owner = THIS_MODULE,
    .open = pen_open,
    .release = pen_close,
    .read = pen_read,
    .write = pen_write,
};

/*************************************************************************
function	: pen_probe
description	:1. usb_register_dev has been called to allocate a minor number for this USB device during the probe function.
                 2. A USB device driver commonly has to convert data from a given struct usb_interface
		    structure into a struct usb_device structure that the USB core needs for a wide range of
		    function calls. To do this, the function interface_to_usbdev is provided.
		 3. This function is called by the USB core.
**************************************************************************/

static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    int retval;
    printk(KERN_ALERT "Probe function\n" );
    device = interface_to_usbdev(interface);
    skel_class.name = "usb/pen11";
    skel_class.fops = &skel_fops;


    //the USB driver must call the usb_register_dev function in the probe function when it wants to register a device with the USB core.
    if ((retval = usb_register_dev(interface, &skel_class)) < 0)
    {
        printk(KERN_ERR "Not able to get a minor for this device.");// Something prevented us from registering this driver 
    }
    else
    {
        printk(KERN_INFO "Minor obtained: %d\n", interface->minor);
    }
    return retval;
}

/*************************************************************************
function	: pen_disconnect
description	: 1. The function usb_deregister_dev must be called to give the minor number back to the USB core.
		  2. This function is called by the USB core, when the struct usb_interface has been removed from the system or 
		     when the driver is being unloaded from the USB core.
**************************************************************************/

static void pen_disconnect(struct usb_interface *interface)
{
    usb_deregister_dev(interface, &skel_class);
}


static struct usb_device_id pen_table[] =
{
    { USB_DEVICE(0x15d9, 0x0a4f) },
    {} 
};
MODULE_DEVICE_TABLE (usb, pen_table);

static struct usb_driver pen_driver =
{
    .name = "pen_driver",
    .probe = pen_probe,
    .disconnect = pen_disconnect,
    .id_table = pen_table,
};

/*************************************************************************
function	: pen_init
description	: this function is initialised when module gets inserted
		  this function is passed as an argument to module_init
input param	: NONE
output param	: NONE
**************************************************************************/
static int pen_init(void)
{
    int result;
    printk(KERN_ALERT "Init function\n" );	
    //To register the struct usb_driver with the USB core
    if ((result = usb_register(&pen_driver)))
    {
        printk(KERN_ERR "usb_register failed. Error number %d", result);
    }
    return result;
}

/*************************************************************************
function	: pen_exit
description	: this function is initialised when module gets removed
		  this function is passed as an argument to module_exit
input param	: NONE
output param	: NONE
**************************************************************************/
static void pen_exit(void)
{
    printk(KERN_ALERT "Exit function\n" );

    // struct usb_driver needs to be unregistered from the kernel
    usb_deregister(&pen_driver);
}

module_init(pen_init);
module_exit(pen_exit);

MODULE_LICENSE("GPL");







