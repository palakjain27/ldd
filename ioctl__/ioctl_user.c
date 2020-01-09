/********************************************************************************/
/*                        INCLUDES                                              */
/********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

/********************************************************************************/
/*                        DEFINES                                               */
/********************************************************************************/
#define RW_VALUE 				_IOW('A', 'a', int32_t *)
#define RD_VALUE 				_IOR('A', 'b', int32_t *)
#define RD_MAJOR 				_IOR('A', 'c', int32_t *)
#define RD_MINOR 				_IOR('A', 'd', int32_t *)
#define RW_BUFFR 				_IOW('A', 'e', char *)
#define RD_BUFFR 				_IOW('A', 'f', char *)
#define RD_CHANGECASE 			_IOR('A', 'g', char *)
#define RD_REVERSE	 			_IOR('A', 'h', char *)
#define SIZE 	 		100

/********************************************************************************/
/*                        GLOBALS                                               */
/********************************************************************************/

/********************************************************************************/
/*                        DECLARATIONS                                          */
/********************************************************************************/

/********************************************************************************/
/*      Fun Name: dev_open()                                                    */
/*      Fun Desc: performs operation when device file is opened                 */
/*      Fun Param: inode structure, file structure                              */
/*      Return Type: int returns 0 on success                                   */
/********************************************************************************/
int main()
{
	int 	fd;
	int32_t value, number, major, minor;
	char 	write_buff[SIZE], read_buff[SIZE], changed_case_buff[SIZE];

	printf( "\n Opening Device file\n" );
	fd = open( "/dev/cDFileName", O_RDWR );
	if( fd < 0 )
	{
		printf( "Oops! Can't open device file...\n" );
		return 0;
	}

	while( 1 )
	{
		int choice;
		printf("\n-----Choices-----\n");
		printf("1. Write Value\n");
		printf("2. Read Value\n");
		printf("3. Write Message\n");
		printf("4. Read Message\n");
		printf("5. Read Major and Minor\n");
		printf("6. Read Changed case of Message\n");
		printf("7. Read Reverse of Message\n");
		printf("8. Exit\n");
		printf("-----------------\n");
		printf("Enter your choice: ");
		scanf( "%d", &choice );

		switch( choice )
		{
			case 1: printf( "Enter the value to send\n" );
					scanf( "%d",&number );
					printf( "Sending....\n" );
					ioctl( fd, RW_VALUE, (int32_t*)&number );
					printf( "Done..!\n" );
					break;

			case 2: printf( "Reading value from the Driver\n" );
					ioctl( fd, RD_VALUE, (int32_t*)&value );
					printf( "Done...!\n" );
					printf( "Value is %d\n", value );
					break;

			case 3: printf( "Enter the message to send\n" );
					scanf( " %[^\t\n]s", write_buff );
					printf( "Sending....\n" );
					ioctl( fd, RW_BUFFR, (char *)&write_buff );
					printf( "Sent..!\n" );
					break;

			case 4: printf( "Receiving message ...\n" );
					ioctl( fd, RD_BUFFR, (char *)&read_buff );
					printf( "Done...!\n" );
					printf( "Message is %s\n", read_buff );
					break;

			case 5: ioctl( fd, RD_MAJOR, (int32_t*)&major );
					ioctl( fd, RD_MINOR, (int32_t*)&minor );
					printf( "Major no is %d and Minor no is %d\n", major, minor );
					break;

			case 6: printf( "Reading Changed case of the message\n" );
					ioctl( fd, RD_CHANGECASE, (char *)&changed_case_buff );
					printf( "Done..!\n" );
					printf( "Changed case message is %s\n", changed_case_buff );
					break;

			case 7: printf( "Reading Reverse of the message\n" );
					ioctl( fd, RD_REVERSE, (char *)&changed_case_buff );
					printf( "Done..!\n" );
					printf( "Changed case message is %s\n", changed_case_buff );
					break;

			case 8: exit( 1 );
					break;

			default: printf( "Please enter a valid choice\n" );
					 break;
		}
	}

	printf( "Closing Device file\n" );
	close( fd );

	return 0;
}
