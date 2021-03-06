/************************************************************************************************
@ileName : hello.c
@author	 : Palak Jain
@teamLead: Rajesh Dommaraju 
@etails  : It is a simple HelloWorld driver to load a module in the kernel  
@icense  : SpanIdea Systems Pvt. Ltd. All rights reserved.
************************************************************************************************/

/*******************************************************************************
			INCLUDES
*******************************************************************************/

#include <linux/init.h> //includes init and exit function 
#include <linux/module.h> //to include information like License, Author, Module Description, Module Version and basic functions of module 

/*******************************************************************************
			 LOCAL MACROS		
*******************************************************************************/

#define GPL_LICENSE				"GPL"
#define DRIVER_MODULE_AUTHOR			"PALAK"
#define DRIVER_MODULE_DESC			"Simple Hello World Module with export command"
#define DRIVER_MODULE_VERSION			"V1.0.0"

/*******************************************************************************
			 LOCAL TYPEDEFS		
*******************************************************************************/

/*******************************************************************************
			GLOBAL VARIABLES		
*******************************************************************************/

/*******************************************************************************
			LOCAL VARIABLES		
*******************************************************************************/

/*******************************************************************************
			LOCAL FUNCTIONS		
*******************************************************************************/
static int hello_init(void);
static void hello_exit(void);

/*******************************************************************************
			GLOBAL VARIABLES		
*******************************************************************************/
//export symbol declarations
extern int count;
void my_shared_func(void);//function declartion is by default extern
/**********************************************************************************************
function	 : hello_init
description	 : This function is initialised when module gets inserted.
		   This function is passed as an argument to module_init.
input param      : NONE
output param     : NONE
**********************************************************************************************/
static int hello_init(void) {
    printk(KERN_INFO "Hello World\n");
    return 0;
}

/**********************************************************************************************
function	 : hello_exit
description	 : This function is initialised when module gets unloaded from kernel.
		   This function is passed as an argument to module_exit.
input param      : NONE
output param     : NONE
**********************************************************************************************/
static void hello_exit(void) {
    printk("Exiting ...\n"); 
}
// sequence in which the module run 
module_init(hello_init);
module_exit(hello_exit);

//file information 
MODULE_LICENSE(GPL_LICENSE);
MODULE_AUTHOR(DRIVER_MODULE_AUTHOR);
MODULE_VERSION(DRIVER_MODULE_VERSION);
MODULE_DESCRIPTION(DRIVER_MODULE_DESC);

