#include <linux/pci.h>
#include<linux/module.h>
//Functions required for us will be taken from PCI .h file.

#define MY_VENDOR_ID 		0x168C
#define MY_DEVICE_ID_NET 	0x0042

static int  network_driver_init(void);
static void network_driver_exit(void);
int pci_probe (struct pci_dev *pdev, const struct pci_device_id *network_driver_pci_table); 
void pci_remove (struct pci_dev *pdev);

//Record the supported device. //First we will create a device.
struct pci_device_id network_driver_pci_table[]
=
 {
    {MY_VENDOR_ID, MY_DEVICE_ID_NET}, {0}, /* Interface chip manufacturer ID *//* Device ID for the network */
 };
MODULE_DEVICE_TABLE(pci,network_driver_pci_table);
//In above code we are given PCI driver related data.
struct pci_driver network_pci_driver =
{
	.name = "ntwrk", /* Unique name */
	.probe = pci_probe,
	.remove = pci_remove,
	.id_table = network_driver_pci_table,
  /* suspend() and resume() optional methods*/
};
int pci_probe (struct pci_dev *pdev, const struct pci_device_id *network_driver_pci_table)
{
        int i;
        printk(KERN_INFO "probe invoked\n");
        if (( i = pci_enable_device(pdev)) < 0);
        	printk(KERN_INFO "pci driver(%04X:%04X) is plugged \n",network_driver_pci_table->vendor,network_driver_pci_table->device);
        return i;
}

void pci_remove (struct pci_dev *pdev)
{
        printk(KERN_INFO "PCI RELEASED\n");
}

// Here basic functions of our device driver gets declared. Which includes function related to connecting device driver name, removing device driver. Also it contains device ID table.
static int  network_driver_init(void)
{
	pci_register_driver(&network_pci_driver);
	return 0;
}

// Here device driver gets initialize.
static void network_driver_exit(void)
{
	pci_unregister_driver(&network_pci_driver);
}

// Here we define unregister function which will be get called after removing driver.
module_init(network_driver_init);
module_exit(network_driver_exit);
MODULE_DEVICE_TABLE(pci,network_driver_pci_table);
