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
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/usb.h>
/**********************************************************************************************
function	 : pen_probe
description	 : This function is called when usb is inserted
input param      : NONE
output param     : NONE
**********************************************************************************************/
static int pen_probe(struct usb_interface *interface,const struct usb_device_id *id)
{
	printk(KERN_INFO "Pen drive plugged(%04X:%04X)\n",id->idVendor,id->idProduct);
	return 0;
}
/**********************************************************************************************
function	 : pen_discoonect
description	 : This function is called when usb is removed
input param      : NONE
output param     : NONE
**********************************************************************************************/
static void pen_disconnect(struct usb_interface *interface)
{
	printk(KERN_INFO "pen drive removed\n");
}
/*******************************************************************************
			STRUCTURES AND ENUMES		
*******************************************************************************/
static struct usb_device_id pen_table[]=
{
	{USB_DEVICE(0x54c,0x5ba)},{}
};//registering the vendor and product id

MODULE_DEVICE_TABLE(usb, pen_table);

static struct usb_driver pen_driver=
{
	.name="pen_driver",
	.id_table=pen_table,
	.probe=pen_probe,
	.disconnect=pen_disconnect,
};
/**********************************************************************************************
function	 : my_driver_init
description	 : This function is initialised when module gets inserted.
		   This function is passed as an argument to module_init.
input param      : NONE
output param     : NONE
**********************************************************************************************/
static int my_pen_init(void)
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
static void my_pen_exit(void)
{
	usb_deregister(&pen_driver);
}
module_init(my_pen_init);
module_exit(my_pen_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PALAK");
MODULE_DESCRIPTION("USB PEN REGISTRATION DRIVER");
