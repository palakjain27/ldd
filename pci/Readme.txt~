                                                                             PCI Driver
                                                                             **********

PCI (Peripheral Component Interconnect) is an interconnection system between a microprocessor and attached devices in which expansion slots are spaced closely for high speed operation

 Peripheral Component Interconnect Express, commonly known as PCI Express and abbreviated as “PCIe”, is a high speed computer bus architecture which has become the industry standard for nearly all internal computer component connections.


 To find PCI hardware details use lspci command
  
  lspci -t -> list of bridges above a device
 

  tree /sys/bus/pci/devices/ --> lists of devices are sorted
 
          For example  0000:00:14.0 when split into domain (16 bits), bus (8 bits), device (5 bits) and  function (3 bits).

Registering a PCI Driver

The main structure that all PCI drivers must create in order to be registered with the kernel properly is the struct pci_driver structure.


static struct pci_driver pci_driver = {
.name = "pci_skel",
.id_table = ids,
.probe = probe,
.remove = remove,
}; 


For Registering a Driver 
	int _pci_driver_register(struct platform_driver * const *drivers);
	

Unregistering a Driver

void pci_unregister_driver(struct platform_driver * const *drivers);


                                                                                     DEVICE ENUMERATION
                                                                                     ******************
                
                           platform_device_register(&my_device);




static struct platform_device my_device = {
    .name                 = "my-platform-device",
    .id                   = PLATFORM_DEVID_NONE,
    .dev.platform_data    = &my_device_pdata
};


MODULE_DEVICE_TABLE(pci, my_device);

This statement creates a local variable called __mod_pci_device_table that points to the list of struct pci_device_id


If that symbol is found, it pulls the data out of the module and adds it to the file /lib/modules/KERNEL_VERSION/modules.pcimap.


                                                                       DEVICE PROBE 
                                                                       *************
Enabling the PCI Device

In the probe function for the PCI driver, before the driver can access any device resource (I/O region or interrupt) of the PCI device, the driver must call the pci_enable_device
function:

int pci_enable_device(struct pci_dev *dev)

This function actually enables the device. It wakes up the device and in some cases also assigns its interrupt line and I/O regions





Please note that we are using platform_driver_probe() instead of platform_driver_register()

   
The difference between these two functions is that with platform_driver_register(), we are asking the OS to put this driver in the list of drivers it maintains for doing device to driver matching when the devices come in/out of the system. Since platform devices are either always present always absent in system (and not-hot-pluggable), we don't need to put our platform driver in the OS driver list. With platform_driver_probe(), we are asking the OS to check if a platform device is present with the matching name. If the device is present in the system, the corresponding probe() function is called. If not present, the driver is simply ignored.


                                                             PLATFORM DEVICE AND DRIVER BINDING
                                                             **********************************
     
When the platform_driver_probe() function is called, the operating system scans through the list of available platform devices and checks if the driver.name matches with the device name. If yes, it calls the driver probe function with the platform data.   


1. In order to unbind a device from a driver, simply write the bus id of the device to the unbind file:

     echo -n "1-1:1.0" > /sys/bus/usb/drivers/ub/unbind    and the device will no longer be bound to the driver

2.  

To bind a device to a driver, the device must first not be controlled by any other driver. To ensure this, look for the "driver" symlink in the device directory: tree /sys/bus/pci/devices/000:00:1f.3



3. Then write the bus id of the device you wish to bind, into the bind file for that driver:

    echo -n "1-1:1.0" > /sys/bus/pci/drivers/sound_card_driver/bind





  
