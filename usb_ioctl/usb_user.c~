#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define READ_STRING	_IOR('I','a',char*)
#define WRITE_STRING	_IOW('I','b',char*)
#define RD_CHANGECASE 	_IOR('I', 'c', char *)
#define RD_REVERSE	_IOR('I', 'd', char *)
//#define USB_INFO	_IOR('I','c',int32_t*)

char write_buf[100], read_buf[100], changed_case_buff[100];
//int32_t a[5];

int main()
{
        int fd;
        char option;

        fd = open("/dev/pen11", O_RDWR);
        if(fd < 0) 
	{
                printf("Cannot open device file...\n");
                return 0;
        }

        while(1) 
	{
                printf("****Please Enter the Option****\n");
                printf("        1. Write               \n");
                printf("        2. Read                \n");
		printf("        3. changed case         \n");
		printf("        4. Reverse string        \n");
		//printf("      3. USB Info              \n");
                printf("        5. Exit                 \n");
                printf("*********************************\n");
                scanf(" %c", &option);
                printf("Your Option = %c\n", option);
                
                switch(option) 
		{
                        case '1':
				fd = open("/dev/pen11", O_RDWR);
                                if(fd < 0)
				{
					printf("device file is removed..\n");
					return 0;
				}
				else
				{
					printf("Enter the string to write into driver :");
		                        scanf(" %s", write_buf);
		                        printf("Data Writing ...");
		                        ioctl( fd, WRITE_STRING, (char*)write_buf);
		                        printf("Done!\n");
				}
                                break;

                        case '2':
				fd = open("/dev/pen11", O_RDWR);
				if(fd < 0) 
				{
					printf("device file is removed....\n");
					return 0;
				}	
                                else
				{
					printf("Data Reading ...");
		                     	ioctl( fd, READ_STRING, (char*)read_buf );
		                        printf("Done!\n\n");
		                        printf("Data = %s\n\n", read_buf);
				}
                                break;
			case '3': 
				fd = open("/dev/pen11", O_RDWR);
				if(fd < 0) 
				{
					printf("device file is removed....\n");
					return 0;
				}	
                                else
				{
				printf( "Reading Changed case of the message\n" );
				ioctl( fd, RD_CHANGECASE, (char *)&changed_case_buff );
				printf( "Done..!\n" );
				printf( "Changed case message is %s\n", changed_case_buff );}
				break;
			case '4': 
				fd = open("/dev/pen11", O_RDWR);
				if(fd < 0) 
				{
					printf("device file is removed....\n");
					return 0;
				}	
                                else
				{printf( "Reading Reverse of the message\n" );
				ioctl( fd, RD_REVERSE, (char *)&changed_case_buff );
				printf( "Done..!\n" );
				printf( "Changed case message is %s\n", changed_case_buff );}
				break;
			
			/*case '3':
				fd = open("/dev/pen11", O_RDWR);
				if(fd < 0) 
				{
					printf("device file is removed....\n");
					return 0;
				}	
                                else
				{
					printf("Data Reading ...");
		                     	ioctl( fd, USB_INFO, (int32_t*)&a );
					printf ( "Interface No=%d\n",a[0]);
					//printf( "VendorID:%04X\n",a[1]);
					//printf( "ProductID:%04X\n",a[2]);
					//printf( "ID->bNumEndpoints:%02X\n",a[3]);
					//printf( "ID->bInterfaceclass: %02X\n",a[4]);
						
				}
				 break;*/
                        case '5':
                                close(fd);
                                exit(1);
                                break;

                        default:
                                printf("Enter Valid option = %c\n",option);
                                break;

                }
        }
        close(fd);
}


