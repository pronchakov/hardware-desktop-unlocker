#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>
#include <map>

int main(int argc, char* argv[])
{
    typedef std::map<int, int> RFID_numbers;
    struct input_event ev;

    RFID_numbers numbers;
    numbers[458782] = 1;
    numbers[458783] = 2;
    numbers[458784] = 3;
    numbers[458785] = 4;
    numbers[458786] = 5;
    numbers[458787] = 6;
    numbers[458788] = 7;
    numbers[458789] = 8;
    numbers[458790] = 9;
    numbers[458791] = 0;
    numbers[458792] = 13;

    int fevdev = -1;
    int result = 0;
    int rd;
    int value;
    char name[256] = "Unknown";
    char *device = "/dev/input/by-id/usb-13ba_Barcode_Reader-event-kbd";


    fevdev = open(device, O_RDONLY);
    if (fevdev == -1) {
        printf("Failed to open event device.\n");
        exit(1);
    }

    result = ioctl(fevdev, EVIOCGNAME(sizeof(name)), name);
    printf ("Reading From : %s (%s)\n", device, name);

    printf("Getting exclusive access: ");
    result = ioctl(fevdev, EVIOCGRAB, 1);
    printf("%s\n", (result == 0) ? "SUCCESS" : "FAILURE");

    int i = 1;
    while (1)
    {
        printf("\n");
	printf("Reading new code...\n");
        read(fevdev, &ev, sizeof(struct input_event));

		if (ev.type == 4) 
        	{
                	printf ("[%i]-Input: type[%i], code[%i], value[%i], number[%i]\n", i,  ev.type, ev.code, ev.value, numbers[ev.value]);
                	i++;
        	}
    }

    printf("Exiting.\n");
    result = ioctl(fevdev, EVIOCGRAB, 1);
    close(fevdev);
    return 0;
}
