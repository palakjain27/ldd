/********************************************************************************/
/*                        INCLUDES                                              */
/********************************************************************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

/********************************************************************************/
/*                        DEFINES                                               */
/********************************************************************************/
#define RW_VALUE 				_IOW('A', 'a', int32_t *)
#define RD_VALUE 				_IOR('A', 'b', int32_t *)
#define RD_MAJOR 				_IOR('A', 'c', int32_t *)
#define RD_MINOR 				_IOR('A', 'd', int32_t *)
#define RW_BUFFR 				_IOW('A', 'e', char *)
#define RD_BUFFR 				_IOW('A', 'f', char *)
#define RD_CHANGECASE 				_IOR('A', 'g', char *)
#define RD_REVERSE	 			_IOR('A', 'h', char *)
#define SIZE 	 				100
#define CHR_DEVICE_NAME 		"DJ-0002"
#define CHR_DEVICE_CLASS_NAME 	"cDClassName"
#define CHR_DEVICE_FILE_NAME 	"cDFileName"
/********************************************************************************/
/*                        GLOBALS                                               */
/********************************************************************************/
int32_t 		value = 0, major = 0, minor = 0;
char 			kernel_buff[SIZE];
dev_t 			dev = 0;
static struct 	class *dev_class;
static struct 	cdev c_dev;

/********************************************************************************/
/*                        DECLARATIONS                                          */
/********************************************************************************/
static int 		dev_open(struct inode *, struct file *);
static int 		dev_close(struct inode *, struct file *);
static ssize_t 	dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t 	dev_write(struct file *, const char *, size_t, loff_t *);
static long 	dev_ioctl(struct file *, unsigned int, unsigned long);
static void 	changeUpperToLowerToUpper(char *);

/********************************************************************************/
/*      Fun Name: dev_open()                                                    */
/*      Fun Desc: performs operation when device file is opened                 */
/*      Fun Param: inode structure, file structure                              */
/*      Return Type: int returns 0 on success                                   */
/********************************************************************************/
static int dev_open(struct inode *i, struct file *f)
{
	printk(KERN_INFO "Device file opened\n");
	return 0;
}

/********************************************************************************/
/*      Fun Name: dev_close()                                                   */
/*      Fun Desc: performs operation when the device file is closed             */
/*      Fun Param: inode structure, file structure                              */
/*      Return Type: int returns 0 on success                                   */
/********************************************************************************/
static int dev_close(struct inode *i, struct file *f)
{
	printk(KERN_INFO "Device file closed\n");
	return 0;
}

/********************************************************************************/
/*      Fun Name: dev_read()                                                    */
/*      Fun Desc: performs operation when the device file is read               */
/*      Fun Param: file structure, user buffer, size, offset                    */
/*      Return Type: int returns 0 on success                                   */
/********************************************************************************/
static ssize_t dev_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "Device file read\n");
 	return 0;	
}

/********************************************************************************/
/*      Fun Name: dev_write()                                                   */
/*      Fun Desc: performs operation when written in file                       */
/*      Fun Param: file structure, kernel buffer, size, offset                  */
/*      Return Type: int returns 0 on success                                   */
/********************************************************************************/
static ssize_t dev_write(struct file *f, const char *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "Device file written\n");
	return 0;
}

// void swap(char *a, char *b)
// {
// 	char temp = *a;
// 	*a = *b;
// 	*b = temp;
// }

/********************************************************************************/
/*      Fun Name: calLength()                                                   */
/*      Fun Desc: calculate the length of the string		                    */
/*      Fun Param: string 										                */
/*      Return Type: length of the string	                                    */
/********************************************************************************/
int calLength(char *s)
{
	int i = 0;
	while( s[i] != '\0' )
		i++;

	return i;
}

/********************************************************************************/
/*      Fun Name: reverseString()                                               */
/*      Fun Desc: reverse the passed string					                    */
/*      Fun Param: string 										                */
/*      Return Type: NULL 					                                    */
/********************************************************************************/
void reverseString(char s[])
{
	int len,i;
	len = calLength(s);
	for ( i = 0; i < len/2; ++i )
	{
		// swap(&s[i],&s[len-i-1]);
		char temp;
		temp = s[i];
		s[i] = s[len-i-1];
		s[len-i-1] = temp;
	}
}

/********************************************************************************/
/*      Fun Name: changeUpperToLowerToUpper()                                   */
/*      Fun Desc: changes string case 						                    */
/*      Fun Param: string 										                */
/*      Return Type: NULL 					                                    */
/********************************************************************************/
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

/********************************************************************************/
/*      Fun Name: dev_ioctl()                                                   */
/*      Fun Desc: performs operation when user calls ioctl()                    */
/*      Fun Param: file structure, user operation, user argument                */
/*      Return Type: int returns 0 on success                                   */
/********************************************************************************/
static long dev_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	major = (int32_t)MAJOR(dev);
	minor = (int32_t)MINOR(dev);

	switch(cmd)
	{
		case RW_VALUE:	copy_from_user(&value, (int32_t*)arg, sizeof(value));
						printk(KERN_INFO "value = %d\n", value);
						break;

		case RD_VALUE:	copy_to_user((int32_t*) arg, &value, sizeof(value));
						break;

		case RW_BUFFR:	copy_from_user(&kernel_buff, (char *)arg, sizeof(kernel_buff));
						printk(KERN_INFO "Message: %s\n", kernel_buff);
						calLength(kernel_buff);
						break;

		case RD_BUFFR: 	copy_to_user((char *)arg, &kernel_buff, sizeof(kernel_buff));
						break;

		case RD_MAJOR:	copy_to_user((int32_t*) arg, &major, sizeof(major));
						printk(KERN_INFO "Major Number: %d\n", major);
						break;

		case RD_MINOR: 	copy_to_user((int32_t *) arg, &minor, sizeof(minor));
						printk(KERN_INFO "Minor Number: %d\n", minor);
						break;

		case RD_CHANGECASE: changeUpperToLowerToUpper(kernel_buff);
							copy_to_user((char *)arg, &kernel_buff, sizeof(kernel_buff));
							printk(KERN_INFO "Changed String: %s\n", kernel_buff);
							break;

		case RD_REVERSE: reverseString(kernel_buff);
						 copy_to_user((char *)arg, &kernel_buff, sizeof(kernel_buff));
						 printk(KERN_INFO "Reversed String: %s\n", kernel_buff);
						 break;

		default: 		printk(KERN_INFO "NO COMMAND FOUND\n");
						break;
	}
	
	return 0;
}

static struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.read = dev_read,
	.write = dev_write,
	.open = dev_open,
	.unlocked_ioctl = dev_ioctl,
	.release = dev_close,
	
};

/********************************************************************************/
/*      Fun Name: dev_init()                                                    */
/*      Fun Desc: Constructor of device DJ-0002 		                        */
/*      Fun Param: NULL                                                         */
/*      Return Type: int returns 0 on success else -1 on failure                */
/********************************************************************************/
static int dev_init(void)
{
	//Allocating Major number
	if(alloc_chrdev_region(&dev,0,1,CHR_DEVICE_NAME))
	{	
		printk(KERN_INFO "Can't allocate memory\n");
		return -1;
	}

	printk(KERN_INFO "Major = %d, Minor = %d\n", MAJOR(dev), MINOR(dev));
	
	//Creating cdev structure
	cdev_init(&c_dev,&fops);
	
	//Adding charater device to the system
	if( (cdev_add(&c_dev,dev,1)) < 0 )
	{
		printk(KERN_INFO "Can't add device to the system\n");
		unregister_chrdev_region(dev,1);
		return -1;
	}
	
	//creating struct class
	if( (dev_class = class_create(THIS_MODULE, CHR_DEVICE_CLASS_NAME)) == NULL )
	{
		printk(KERN_INFO "Can't create the struct class\n");
		unregister_chrdev_region(dev,1);
		return -1;
	}

	//Creating device
	if( (device_create(dev_class, NULL, dev, NULL, CHR_DEVICE_FILE_NAME)) == NULL)
	{
		printk(KERN_INFO "Can't create Device\n");
		class_destroy(dev_class);
		unregister_chrdev_region(dev,1);
		return -1;
	}
	
	printk(KERN_INFO "Device Driver Inserted...!!\n");
	return 0;

}

/********************************************************************************/
/*      Fun Name: dev_exit()                                                    */
/*      Fun Desc: Destructor of device DJ-0002			                        */
/*      Fun Param: NULL                                                         */
/*      Return Type: NULL					                                    */
/********************************************************************************/
static void dev_exit(void)
{
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(&c_dev);
	unregister_chrdev_region(dev,1);
	printk(KERN_INFO "Device Driver Removed....!!\n");
}


module_init(dev_init);
module_exit(dev_exit);

/********************************************************************************/
/*                        MODULE INFORMATION                                    */
/********************************************************************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Omesh Sudhakar");
MODULE_DESCRIPTION("IOCTL Driver Program");
