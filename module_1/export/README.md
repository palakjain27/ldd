File name: export.c

Description :
		This file is using a export symbol in simple hello world module. 

****************Why export symbol*************************
When multiple drivers (modules) are written in the same device, some of the functions may be used from one module to another module.

*********************Symbol*******************
A symbol is a name representing an space in the memory, which stores data (variable, for reading and writing) or instructions (function, for executing).It can be a function or variable.
Using EXPORT_SYMBOL macro (ex. EXPORT_SYMBOL(func_name)), those symbols are exposed to all the loadable kernel driver.The kbuild mechanism is told that the symbol referred to should be part of the global list of kernel symbols. That allows the kernel modules to access them.	

**************Method for using export command****************** 
*Declare and define the symbol (functions or variables) which you want to make it visible to other kernel modules. Then below the definition, use EXPORT_SYMBOL(symbol name). Now it is visible to 	      all loadable modules.
*Now take the kernel driver who is gonna use the above exported symbol. Declare the symbol using extern. Then use the symbol directly.
*Finally, load the module first, who has the definition of the export symbol. Then load the caller module using insmod.

******************Steps to compile*********************
1.Run						make

2.Insert module					sudo insmod export.ko  and export1.ko(inserting module in sys/module)

3.See the added module				sys/modules

4.Shared functions and variables 		“Module.symvers"
are available in 				

5.To check whether our shared function 		cat /proc/kallsyms | grep my_shared_func
and variable become the part of kernel’s 	cat /proc/kallsyms | grep count
symbol table or not.				  						

6.Shared function and variable are mentioned	cat Module.symvers

7.See the logs in dmesg 			dmesg | tail -10     (to see kernel prints)	

8.Remove module					sudo rmmod export and export1

9.Run						make clean



