File : param_passing.c

**********Description************** 
We can pass the arguments to any other functions in same program. Similarly we can pass argument to device driver.

********Module Parameters Macros********
    1.module_param() ----macro creates the sub-directory under /sys/module.
	prototype: module_param(name, type, perm);
	This will create the sysfs entry. (/sys/module/param_passing/parameters/my_value)

    2.module_param_array() ----This macro is used to send the array as a argument
	prototype :module_param_array(name,type,num,perm);

    3.module_param_cb() ----This macro used to register the callback whenever the argument (parameter) got changed

--Permissions 
    S_IWUSR
    S_IRUSR
    S_IXUSR
    S_IRGRP
    S_IWGRP
    S_IXGRP 

In this:
	S_I is common header.
	R = read ,W =write ,X= Execute.
	USR =user, GRP =Group
	Using OR ‘|’ (or operation) we can set multiple permissions at a time.

**************Steps to compile********************
1.Run						make

2.Insert module and parameter passing		sudo insmod param_passing.ko my_value=14 my_name="palak" my_arr_value=100,102,104,106

3.Adding value 					echo 13 > /sys/module/param_passing/parameters/my_cb_value

4.Removing module 				sudo rmmod param_passing

