/************************************************************************************************
@ileName : test_string.c
@author	 : Palak Jain
@teamLead: Rajesh Dommaraju 
@etails  : It is a test app, in the user sapce which communicates between user and kernel space we can do all open, read, write, close operations on device file.
@icense  : SpanIdea Systems Pvt. Ltd. All rights reserved.
************************************************************************************************/

/*******************************************************************************
			INCLUDES
*******************************************************************************/	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>

/*******************************************************************************
			IOCTL MACROS
*******************************************************************************/	
 

#define RD_VALUE 				_IOW('5', 'k', int *)
#define WR_VALUE 				_IOR('5', 'l', int *)
/**********************************************************************************************
function	 : main
description	 : User app to perform file operation between user and kernel  
input param      : NONE
output param     : NONE
**********************************************************************************************/ 
int main()
{
        int fd,v,n,no;
        	
        printf("\nOpening Driver\n");
        fd = open("/dev/sbull", O_RDWR);
        if(fd < 0) 
	{
                printf("Cannot open device file...\n");
                return 0;
        }

while(1)
{
	printf("\n enter any options between 1--3\n");
	scanf("%d",&n);
	switch(n) {
               case 1: 
			printf( "Enter the value to send\n" );
			scanf( "%d",&no );
			printf( "Sending....\n" );
			ioctl( fd, WR_VALUE, (int*)&no );
			printf( "Done..!\n" );
			break;
		case 2: 
			printf( "Reading value from the Driver\n" );
			ioctl( fd, RD_VALUE, (int*)&v );
			printf( "Done...!\n" );
			printf( "Value is %d\n", v );
			break;
               case 3:
			printf("Closing Driver\n");
        		close(fd);
			exit(1);
			break;
		
		
		default:
			printf ("invalid option");
			break;
        }
 }
    
return 0;    
        
}
