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
#include<linux/ioctl.h> 
/*******************************************************************************
			 LOCAL MACROS		
*******************************************************************************/
#define MAX_PKT_SIZE     512
#define READ_STRING	_IOR('I','a',char*)
#define WRITE_STRING	_IOW('I','b',char*)
#define RD_CHANGECASE 	_IOR('I', 'c', char *)
#define RD_REVERSE	_IOR('I', 'd', char *)
//#define USB_INFO	_IOR('I','c',int32_t*)
//#define MIN(a,b)         (((a) <= (b)) ? (a) :(b))
//#define BULK_EP_OUT     0x01
#define BULK_EP_IN      0x82
/*******************************************************************************
			GLOBAL VARIABLES		
*******************************************************************************/

static struct usb_class_driver skel_class;
static struct usb_device *device;
static unsigned char bulk_buf[MAX_PKT_SIZE];
//int32_t a[5];
/********************************************************************************
        Fun Name: calLength()                                                   
        Fun Desc: calculate the length of the string		                  
        Fun Param: string 										                
        Return Type: length of the string	                                    
********************************************************************************/
int calLength(char *s)
{
	int i = 0;
	while( s[i] != '\0' )
		i++;

	return i;
}

/********************************************************************************
      Fun Name: reverseString()                                               
      Fun Desc: reverse the passed string					                    
      Fun Param: string 										                
      Return Type: NULL 					                                    
********************************************************************************/
void reverseString(char s[])
{
	int len,i;
	len = calLength(s);
	for ( i = 0; i < len/2; ++i )
	{
		
		char temp;
		temp = s[i];
		s[i] = s[len-i-1];
		s[len-i-1] = temp;
	}
}

/********************************************************************************
      Fun Name: changeUpperToLowerToUpper()                                   
      Fun Desc: changes string case 						                    
      Fun Param: string 										                
      Return Type: NULL 					                                    
********************************************************************************/
static void changeUpperToLowerToUpper(char s[])
{
	int i = 0;
	while( s[i] != '\0')
	{
		if( s[i] >= 'A' && s[i] <= 'Z' )
		{
			s[i] = s[i] + 32;
			i++;
		}
		else if ( s[i] <= 'z' && s[i] >= 'A' )
		{
			s[i] = s[i] - 32;
			i++;
		}
		else
			i++;	
	}
}

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

/*static ssize_t pen_read(struct file *f,char __user *buf , size_t cnt, loff_t *ppos)
{
int retval;
int read_cnt;
retval = usb_bulk_msg(device,usb_rcvbulkpipe(device,BULK_EP_IN),bulk_buf,MAX_PKT_SIZE,&read_cnt,5000);
if(retval)
{
	printk(KERN_ERR "Bulk message returned %d \n", retval);
	return retval;
}
if (copy_to_user(buf, bulk_buf,MIN(cnt, read_cnt)))
{
	return -EFAULT;
}
return MIN(cnt, read_cnt);
}
static ssize_t pen_write(struct file*f, const char __user *buf, size_t cnt,loff_t *ppos)
{
int retval;
int wrote_cnt =MIN(cnt,MAX_PKT_SIZE);
if(copy_from_user(bulk_buf, buf,MIN(cnt,MAX_PKT_SIZE)))
{
	return -EFAULT;
}
/*write data in to bulk endpoint*//*

retval =usb_bulk_msg(device,usb_sndbulkpipe(device,BULK_EP_OUT),bulk_buf,MIN(cnt,MAX_PKT_SIZE),&wrote_cnt ,5000);;
if(retval)
{
	printk(KERN_ERR "Bulk message returned %d\n", retval);
	return retval;
}
return wrote_cnt;
}*/
/**********************************************************************************************
function	 : pen_ioctl
description	 : Sends data from user space to the kernel space.
input param      : NONE
output param     : NONE
**********************************************************************************************/
static long pen_ioctl(struct file *file,unsigned int cmd,unsigned long arg)
{
	switch (cmd){

		case WRITE_STRING:
			printk(KERN_ALERT "Write function\n" );
			copy_from_user(&bulk_buf, (char*)arg, sizeof(bulk_buf));    
			printk(KERN_INFO "STRING :%s\n",bulk_buf);
			break;
		case READ_STRING:
			printk(KERN_ALERT "Read function\n" );
			copy_to_user((char*)arg, &bulk_buf, sizeof(bulk_buf));
			 break;
		case RD_CHANGECASE: 
			changeUpperToLowerToUpper(bulk_buf);
			copy_to_user((char *)arg, &bulk_buf, sizeof(bulk_buf));
			printk(KERN_INFO "Changed case String: %s\n", bulk_buf);
			break;

		case RD_REVERSE: 
			reverseString(bulk_buf);
			copy_to_user((char *)arg, &bulk_buf, sizeof(bulk_buf));
		        printk(KERN_INFO "Reversed String: %s\n", bulk_buf);
			 break;
		/*case USB_INFO:
			iface_desc = interface->cur_altsetting;
			a[0]=iface_desc->desc.bInterfaceNumber;
			a[1]=id->idVendor;
			a[2]=id->idProduct;
			a[3]=iface_desc->desc.bNumEndpoints;
			a[4]=iface_desc->desc.bInterfaceClass;
			printk (KERN_INFO "Pen i/f %d now probed:(%04X:%04X)\n",iface_desc->desc.bInterfaceNumber, id->idVendor, id->idProduct);
			printk(KERN_INFO "ID->bNumEndpoints:%02X\n",iface_desc->desc.bNumEndpoints);
			printk(KERN_INFO "ID->bInterfaceclass: %02X\n", iface_desc->desc.bInterfaceClass);
			copy_to_user((int32_t*)arg, &a, sizeof(a));
			for(int i=0; i < iface_desc->desc.bNumEndpoints; i++)
			{
				endpoint = &iface_desc->endpoint[i].desc;
				printk(KERN_INFO "ED[%d]->bEndpointAddress: 0x%02X\n",i, endpoint->bEndpointAddress);
				printk(KERN_INFO "ED[%d]->bmAttributes: 0x%02X\n", i, endpoint->bmAttributes);
				printk(KERN_INFO "ED[%d]->wMaxPacketSize: 0x%04X (%d)\n",i,endpoint->wMaxPacketSize, endpoint->wMaxPacketSize);
			}*/
			
		default: 		
			printk(KERN_INFO "NO COMMAND FOUND\n");
			break;
		  }
return 0;
}
/*******************************************************************************
			STRUCTURES AND ENUMES		
*******************************************************************************/ 
static struct file_operations skel_fops =
{
    .owner = THIS_MODULE,
    .open = pen_open,
    .release = pen_close,
    .unlocked_ioctl=pen_ioctl,
    //.read =pen_read,
    //.write =pen_write,
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
    device = interface_to_usbdev (interface);
    //initialising usb_class_driver struct
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
//to print the description of interface and endpoint 
	struct usb_host_interface *iface_desc;
	struct usb_endpoint_descriptor *endpoint;
	int i;
	iface_desc = interface->cur_altsetting;
	printk (KERN_INFO "Pen i/f %d now probed:(%04X:%04X)\n",iface_desc->desc.bInterfaceNumber, id->idVendor, id->idProduct);
	printk(KERN_INFO "ID->bNumEndpoints:%02X\n",iface_desc->desc.bNumEndpoints);
	printk(KERN_INFO "ID->bInterfaceclass: %02X\n", iface_desc->desc.bInterfaceClass);

		for( i=0; i < iface_desc->desc.bNumEndpoints; i++)
		{
			endpoint = &iface_desc->endpoint[i].desc;
			printk(KERN_INFO "ED[%d]->bEndpointAddress: 0x%02X\n",i, endpoint->bEndpointAddress);
			printk(KERN_INFO "ED[%d]->bmAttributes: 0x%02X\n", i, endpoint->bmAttributes);//type of endpoint
			printk(KERN_INFO "ED[%d]->wMaxPacketSize: 0x%04X (%d)\n",i,endpoint->wMaxPacketSize, endpoint->wMaxPacketSize);//USB address of specific endpoint that include dir of data, that data will go to system or to device.
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
    printk(KERN_INFO "PEN drive removed\n");
    usb_deregister_dev(interface, &skel_class);
    printk(KERN_INFO "Pen i/f %d now disconnected \n",interface->cur_altsetting->desc.bInterfaceNumber);
}


static struct usb_device_id pen_table[] =
{
    { USB_DEVICE(0x15d9, 0x0a4f) },
    {} 
};
MODULE_DEVICE_TABLE(usb, pen_table);//to find the device table 
//User mode code can read these tables to choose which modules to load.
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







