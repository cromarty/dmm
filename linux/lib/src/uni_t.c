#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include "fs9721lp3.h"
#include "unit-t.h"

DMM_CALLBACK_T *dmmcb = NULL;


int dmm_open(const char *device, struct termios *oldtio) {
	int fd;
	struct termios newtio;
	unsigned int mcr;

	// Open serial device for reading and writing and not as controlling tty because we don't want to get killed if linenoise sends CTRL-C.
	fd = open(device, O_RDONLY | O_NOCTTY ); 
	if (fd <0) {
		perror(device);
		return(-1);
	}

	tcgetattr(fd, oldtio);

	bzero(&newtio, sizeof(newtio));

	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = ICRNL;
	newtio.c_oflag = 0;
	newtio.c_lflag = ~ICANON;
	newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
	newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
	newtio.c_cc[VERASE]   = 0;     /* del */
	newtio.c_cc[VKILL]    = 0;     /* @ */
	newtio.c_cc[VEOF]     = 0;     /* Ctrl-d */
	newtio.c_cc[VTIME]    = 1;     /* inter-character timer unused */
	newtio.c_cc[VMIN]     = 14;     /* blocking read until 1 character arrives */
	newtio.c_cc[VSWTC]    = 0;     /* '\0' */
	newtio.c_cc[VSTART]   =17;     /* Ctrl-q */ 
	newtio.c_cc[VSTOP]    =19;     /* Ctrl-s */
	newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
	newtio.c_cc[VEOL]     = 0;     /* '\0' */
	newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
	newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
	newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
	newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
	newtio.c_cc[VEOL2]    = 0;     /* '\0' */

	tcsetattr(fd, TCSANOW, &newtio);
	mcr = 0;
	ioctl(fd,TIOCMGET,&mcr);
	mcr &= ~TIOCM_RTS;
	mcr |= TIOCM_DTR;
	ioctl(fd,TIOCMSET,&mcr);
	tcflush(fd, TCIFLUSH);
	return(fd);
}// end DMMOpen

void dmm_close(int fd, struct termios *oldtio) {
	tcsetattr(fd, TCSANOW, oldtio);
	close(fd);
} // end DMMClose

void *_dmm_read_thread(void *arg) {
	int *fd = (int*)arg;
	int i, j, ready;
	int bytesRead;
	int nextByte = 1;
	FS9721_LP3_FRAME_T frame;
	int timeouts = 0;
	char readBuffer[256];
//	struct timeval timeout;
	//timeout.tv_sec = TIMEOUT_SEC;
	//timeout.tv_usec = TIMEOUT_USEC;


	fd_set readFDS, exceptFDS;

	FD_ZERO(&readFDS);
	FD_SET(*fd, &readFDS);
	FD_ZERO(&exceptFDS);
	FD_SET(*fd, &exceptFDS);

	while(timeouts < MAX_TIMEOUTS )
	{
struct timeval timeout;
timeout.tv_sec = TIMEOUT_SEC;
timeout.tv_usec = TIMEOUT_USEC;
		/*
		*
		* select returns 0 if timeout, 1 if activity possible, -1 if error.
		* note that -1 means error in the select function call, not the same as checking the errorFDS for error conditions
		*
		*/
		ready = select(FD_SETSIZE, &readFDS, NULL, &exceptFDS, &timeout);
		if (ready < 0) {
			dmmcb(*fd, frame, CB_ERROR_SELECT);
pthread_exit((void*)CB_ERROR_SELECT);
		}


		if (ready == 0) {
			// time out, continue the loop
			timeouts++;
dmmcb(*fd, frame, CB_ERROR_TIMEOUT);
			if (timeouts >= MAX_TIMEOUTS) {
		dmmcb(*fd, frame, CB_ERROR_MAXTIMEOUTS);
				pthread_exit((void*)CB_ERROR_MAXTIMEOUTS);
}


			continue;
		}

		/* activity possible */
		for ( i = 0 ; i < FD_SETSIZE ; ++i ) {
			/* check the error set first */
			if (FD_ISSET(i, &exceptFDS)) {
				if (i == *fd) {
					/* some kind of error associated with the fd file descriptor */
dmmcb(*fd, frame, CB_ERROR_EXCEPT);
					pthread_exit((void*)CB_ERROR_EXCEPT);
				}
			}

			// check read set
			if (FD_ISSET(i, &readFDS)) {
				timeouts = 0;
				if ( i == *fd) {
					bzero(readBuffer, 256);
					bytesRead = read(*fd, readBuffer, 256);
					if (bytesRead < 0) {
		dmmcb(*fd, frame, CB_ERROR_READ);
						pthread_exit((void*)CB_ERROR_READ);
					}

// frame population
for ( j = 0 ; j < bytesRead ; j++ )
{

if (readBuffer[j] & (nextByte<<4))
{
frame.data[nextByte - 1] = readBuffer[j];
nextByte++;
}

if (nextByte == 15)
{
nextByte = 1;
// callback
if (dmmcb != NULL)
	dmmcb(*fd, frame, CB_ERROR_NONE);
}

}


				}
			}

		}

	}

	return (void*)CB_ERROR_NONE;

} // end dmm_read

int dmm_default_callback(int n, FS9721_LP3_FRAME_T frame, int cbError)
{
	printf("Inside default callback\n");
}

void dmm_set_callback(DMM_CALLBACK_T *cb) {
	dmmcb = cb;
}



int dmm_decode_measurement(FS9721_LP3_FRAME_T frame, float *measurement) {
    int i;
    unsigned int dot = 0;
    float fdot = 1.0;

    *measurement = 0.0;

    for (i = 0; i < 7; i += 2) {
        unsigned int nDigit = ((0x07 & frame.digit[i]) << 4) + (0x0F & frame.digit[1 + i]);

        if (i > 0)
            dot |= ((0x08 & frame.digit[i]) >> 3);

        if (dot)
            fdot = (float)fdot * 10.0;

        switch (nDigit) {
        case 0x7D:
            nDigit = 0;
            break;
        case 0x05:
            nDigit = 1;
            break;
        case 0x5B:
            nDigit = 2;
            break;
        case 0x1F:
            nDigit = 3;
            break;
        case 0x27:
            nDigit = 4;
            break;
        case 0x3E:
            nDigit = 5;
            break;
        case 0x7E:
            nDigit = 6;
            break;
        case 0x15:
            nDigit = 7;
            break;
        case 0x7F:
            nDigit = 8;
            break;
        case 0x3F:
            nDigit = 9;
            break;
        case 0x68:
            nDigit = 0;
            // this is the overflow setting

        }

        if (dot) {
            *measurement = (float)nDigit / (fdot) + (*measurement);
        } else {
            *measurement = (float)nDigit + (*measurement * 10.0);
        }

    }     // end of for loop

    if (frame.flag[1] & 0x08)
        *measurement /= 1000.0;            // milli

    if (frame.flag[0] & 0x08)
        *measurement /= 1000000.0;         // micro


    if (frame.digit[0] & 0x08)
        *measurement *= -1; // negate

    return 0;

    }


int dmm_measurement_string(
    FS9721_LP3_FRAME_T frame,
    float measurement,
    char *measString
    ) {
    char measTemp[16];

    bzero(measTemp, 16);
        switch (frame.mode & 0x0c) {
    case 0x08:
        strcat(measString, "AC ");
        break;

    case 0x04:
        strcat(measString, "DC ");
        break;
    }
    sprintf(measTemp, "%4.3f ", measurement);
    strcat(measString, measTemp);
    strcat(measString, "V");
    return 0;
}



