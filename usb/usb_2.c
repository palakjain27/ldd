/************************************************************************************************
@ileName : real_driver.c
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
static struct usb_device *device;
/**********************************************************************************************
function	 : pen_probe
description	 : This function is called when usb is inserted and give information about the device inserted
input param      : NONE
output param     : NONE
**********************************************************************************************/
static int pen_probe(struct usb_interface *interface, const struct usb_device_id*id)
{
struct usb_host_interface*iface_desc;
struct usb_endpoint_descriptor*endpoint;
int i;
	iface_desc = interface->cur_altsetting;
	printk (KERN_INFO "Pen i/f %d now probed:(%04X:%04X)\n",iface_desc->desc.bInterfaceNumber, id->idVendor, id->idProduct);
	printk(KERN_INFO "ID->bNumEndpoints:%02X\n",iface_desc->desc.bNumEndpoints);
	printk(KERN_INFO "ID->bInterfaceclass: %02X\n", iface_desc->desc.bInterfaceClass);
	for(i=0; i < iface_desc->desc.bNumEndpoints; i++)
	{
		endpoint = &iface_desc->endpoint[i].desc;
		printk(KERN_INFO "ED[%d]->bEndpointAddress: 0x%02X\n",i, endpoint->bEndpointAddress);
		printk(KERN_INFO "ED[%d]->bmAttributes: 0x%02X\n", i, endpoint->bmAttributes);
		printk(KERN_INFO "ED[%d]->wMaxPacketSize: 0x%04X (%d)\n",i,endpoint->wMaxPacketSize, endpoint->wMaxPacketSize);
	}
device =interface_to_usbdev(interface);
return 0;
}
/**********************************************************************************************
function	 : pen_disconect
description	 : This function is called when usb is removed
input param      : NONE
output param     : NONE
**********************************************************************************************/
static void pen_disconnect(struct usb_interface *interface)
{
	printk(KERN_INFO "Pen i/f %d now disconnected \n",interface->cur_altsetting->desc.bInterfaceNumber);
}
/*******************************************************************************
			STRUCTURES AND ENUMES		
*******************************************************************************/
static struct usb_device_id pen_table[]=
{
	{ USB_DEVICE(0x54c,0x5ba)},{}
};
MODULE_DEVICE_TABLE (usb, pen_table);
static struct usb_driver pen_driver =
{
	.name = "pen_driver",
	.id_table = pen_table,
	.probe = pen_probe,
	.disconnect = pen_disconnect,
};
/**********************************************************************************************
function	 : my_driver_init
description	 : This function is initialised when module gets inserted.
		   This function is passed as an argument to module_init.
input param      : NONE
output param     : NONE
**********************************************************************************************/
static int __init pen_init(void)
{
	return usb_register(&pen_driver);
}
/**********************************************************************************************
function	 : my_driver_exit
description	 : This function is initialised when module gets removed.
		   This function is passed as an argument to module_exit.
input param      : NONE
output param     : NONE
**********************************************************************************************/ 
static void __exit pen_exit(void)
{
	usb_deregister(&pen_driver);
}

module_init(pen_init);
module_exit(pen_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR ("palak");
MODULE_DESCRIPTION("USB PEN Info DRIVER");
