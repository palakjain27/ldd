/*uthor   : Prithi P
@ Teamlead : Rajesh Dommaraju
@ details  : It is PCI Driveer
@ licence  : Spanidea systems Pvt. Ltd. All rights reserved.

**************************************************************************/

/*************************************************************************
                INCLUDES
**************************************************************************/

#include <linux/pci.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/device.h>

/*************************************************************************
               MACROS
*************************************************************************/

#define MY_VENDOR_ID                            0x168C
#define MY_DEVICE_ID_NET                        0x0042

/************************************************************************
             GLOBAL VARIABLES

************************************************************************/
static int pci_driver_init(void);
static void  pci_driver_exit(void);
int pci_probe (struct pci_dev *pdev, const struct pci_device_id *id);
void pci_remove (struct pci_dev *pdev);


/************************************************************************
           TYPE OF DEVICE A DRIVER SUPPORTS
************************************************************************/

struct pci_device_id pci_id_table[]  =
{
	{
         PCI_DEVICE(MY_VENDOR_ID,MY_DEVICE_ID_NET),                
  	},{}
};

MODULE_DEVICE_TABLE(pci,pci_id_table);

/*************************************************************************
function        : pci_driver my_pci_driver
description     : Initialisation of driver structure
**************************************************************************/

static struct pci_driver my_pci_driver =
	{
	 .name = "network_driver",
         .id_table = pci_id_table,
         .probe = pci_probe,
         .remove = pci_remove,
	};
/*************************************************************************
function        : pci_probe
description     : This function actually enables the device. It wakes up the device and in some
                  cases also assigns its interrupt line and I/O regions.
**************************************************************************/

int pci_probe (struct pci_dev *pdev, const struct pci_device_id *pci_id_table)
{
	

	int i;
	printk(KERN_INFO "probe invoked\n");
	if (( i = pci_enable_device(pdev)) < 0);
	
	printk(KERN_INFO "pci driver(%04X:%04X) is plugged \n",pci_id_table->vendor,pci_id_table->device);
	return i;


	
}

void pci_remove (struct pci_dev *pdev)
{
	printk(KERN_INFO "PCI RELEASED\n");
}	


/*************************************************************************
function        : pci_driver_init
description     : Driver gets initialized ang registered
**************************************************************************/


static int pci_driver_init(void)
		{
			pci_register_driver(&my_pci_driver);
                        printk(KERN_INFO "**********INITIALIZED PCI**********\n");
			return 0;
		}

/*************************************************************************
function        : pci_driver_exit
description     : it unregisters the function and remove the driver
**************************************************************************/
static void  pci_driver_exit(void)
		{
			pci_unregister_driver(&my_pci_driver);
                        printk(KERN_INFO "*********UNINITIALIZED PCI************\n");
		}

module_init(pci_driver_init);
module_exit(pci_driver_exit);
//MODULE_DEVICE_TABLE(pci,pci_id_table);


