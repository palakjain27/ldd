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
 
#define WR_VALUE _IOW('4','b',char)
#define RD_VALUE _IOR('4','c',char)
#define WR_STRING _IOW('4','d',char*)
#define RD_STRING _IOR('4','e',char*) 
#define OPERATIONS _IOWR('4','f',int*)

/**********************************************************************************************
function	 : main
description	 : User app to perform file operation between user and kernel  
input param      : NONE
output param     : NONE
**********************************************************************************************/ 
int main()
{
        int fd,n,a,count;
        char value, number;
	char data[20],data1[20];	
        printf("\nOpening Driver\n");
        fd = open("/dev/my_device", O_RDWR);
        if(fd < 0) 
	{
                printf("Cannot open device file...\n");
                return 0;
        }

while(1)
{
	printf("\n enter any options between 1--6\n");
	scanf("%d",&n);
	switch(n) {
                case 1:
			printf("Enter the Value to send\n");
        		scanf("%c ",&number);
                        ioctl(fd, WR_VALUE, (char*) &number); 
                        break;
                case 2:
                        printf("Reading Value from Driver\n");
        		ioctl(fd, RD_VALUE, (char*) &value);
			printf("Value is %c\n",value);
                        break;
		
		case 3:
			printf("Enter the Value to send\n");
			scanf("%s", data1);
                        ioctl(fd, WR_STRING, (char*) &data1);
                        break;
		case 4:
                        printf("Reading Value from Driver\n");
        		ioctl(fd, RD_STRING, (char*) &data1);
		         printf("Value is %s\n",data1);
                        break;
		case 5:
                        printf("enter the value \n");
			scanf("%d", &a);
        		ioctl(fd, OPERATIONS, (int*) &a);
			 printf("%d",a);
			if (a==1)
			printf("no is odd\n");
			else 
			printf("no is even\n");
                        break;
		case 6:
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
