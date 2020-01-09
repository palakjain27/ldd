File : sysfs.c 
*********************************************
Description :
Sysfs is a virtual filesystem exported by the kernel, similar to /proc. The files in Sysfs contain information about devices and drivers. Some files in Sysfs are even writable, for configuration and control of devices attached to the system. Sysfs is always mounted on /sys.

Sysfs is the commonly used method to export system information from the kernel space to the user space for specific devices. 
 The procfs is used to export the process specific information.
 The debugfs is used to used for exporting the debug information by the developer.

************Kernel Objects***********

Heart of the sysfs model is the Kobject. Kobject is the glue that binds the sysfs and the kernel, which is represented by struct kobject and defined in <linux/kobject.h>. A struct kobject represents a kernel object, maybe a device or so, such as the things that show up as directory in the sysfs filesystem.So Kobj is used to create kobject directory in /sys.

*********SysFS in Linux************

There are several steps in creating and using sysfs.
    Create directory in /sys
    Create Sysfs file

*************Create directory in /sys****************

We can use this function (kobject_create_and_add) to create directory.
	
struct kobject * kobject_create_and_add ( const char * name, struct kobject * parent);
	<name> – the name for the kobject
	<parent> – the parent kobject of this kobject, if any.

This function creates a kobject structure dynamically and registers it with sysfs. If the kobject was not able to be created, NULL will be returned.
When you are finished with this structure, call kobject_put and the structure will be dynamically freed when it is no longer being used.

**********Create Sysfs file*******
Attributes are represented as regular files in sysfs with one value per file. There are loads of helper function that can be used to create the kobject attributes.They can be found in header file sysfs.h

1.create attributes
	Kobj_attribute is defined as,
	struct kobj_attribute {
	    struct attribute attr;
	    ssize_t (*show)(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
	    ssize_t (*store)(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);
	};

	attr – the attribute representing the file to be created,
	show – the pointer to the function that will be called when the file is read in sysfs,
	store – the pointer to the function which will be called when the file is written in sysfs.

2.show and store function
Store function will be called whenever we are writing something to the sysfs attribute. 
	ssize_t (*show)(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
Show function will be called whenever we are reading sysfs attribute.
	ssize_t (*store)(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);
 

3.create sysfs file

int sysfs_create_file ( struct kobject *  kobj, const struct attribute * attr);
	kobj – object we’re creating for.
	attr – attribute descriptor.

One can use another function ‘ sysfs_create_group ‘ to create a group of attributes.Once you have done with sysfs file, you should delete this file using sysfs_remove_file

void sysfs_remove_file ( struct kobject *  kobj, const struct attribute * attr);
	kobj – object we’re creating for.
	attr – attribute descriptor.


*********compilation************* 
1.Kobj is used to create kobject directory in /sys
