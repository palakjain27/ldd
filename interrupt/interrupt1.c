/************************************************************************************************
@ileName : interrupt1.c
@author	 : Palak Jain
@teamLead: Rajesh Dommaraju 
@etails  : It is a keyboard interrupt driver
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
#include <linux/interrupt.h>
#include <asm/io.h>
/*******************************************************************************
			 LOCAL MACROS		
*******************************************************************************/
 
#define IRQ_NO 1
 
/**********************************************************************************************
function	 : irq_handler
description	 : This function is initialised when module gets inserted.
		   This function is passed as an argument to module_init.
input param      : irq ,*dev_id
output param     : NONE
**********************************************************************************************/ 
static irqreturn_t irq_handler(int irq,void *dev_id) {
 int p;
	p=inb(0x60); //input byte of data //low level i/o access //port no 
 switch (p)
{ 
   case 28:
	printk("\nenter is pressed\n");
	break;
   case 156: 
	printk("\nenter is released\n");
  	break;
    case 48:
	printk("\nb is pressed\n");
	break;
    case 29:
	printk("\nctrl is pressed\n");
	break;
     case 42:
	printk("\nshift is pressed\n");
	break;
     case 46:
	printk("\nc is pressed\n");
	 default:
	break;	
}
  printk(KERN_INFO "Shared IRQ: Interrupt Occurred");
  return IRQ_HANDLED;
}
/*******************************************************************************
			LOCAL FUNCTIONS		
*******************************************************************************/ 
  
static int my_driver_init(void);
static void my_driver_exit(void);
 
/**********************************************************************************************
function	 : my_driver_init
description	 : This function is initialised when module gets inserted.
		   This function is passed as an argument to module_init.
input param      : NONE
output param     : NONE
**********************************************************************************************/ 

static int my_driver_init(void)
{
        if (request_irq(IRQ_NO, irq_handler, IRQF_SHARED, "my_value", (void *)(irq_handler))) {
            printk(KERN_INFO "my_device: cannot register IRQ ");
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
void my_driver_exit(void)
{
        free_irq(IRQ_NO,(void *)(irq_handler));
        printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}
 
module_init(my_driver_init);
module_exit(my_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("palak");
MODULE_DESCRIPTION("A simple device driver - Interrupts");
MODULE_VERSION("1.9");
 
