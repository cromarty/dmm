#ifndef UNI_T_H
#define UNI_T_H

#include <termios.h>
#include <pthread.h>

#include "fs9721lp3.h"

#define MAX_TIMEOUTS 10
#define TIMEOUT_SEC 5
#define TIMEOUT_USEC 0

/**
*
* Numeric condition codes passed to the callback from the read thread.
*
*/
enum {
	CB_ERROR_NONE = 0,
	CB_ERROR_SELECT,
	CB_ERROR_EXCEPT,
CB_ERROR_READ,
	CB_ERROR_TIMEOUT,
	CB_ERROR_MAXTIMEOUTS
};

/* callback function prototype */
typedef int (DMM_CALLBACK_T)(int n, FS9721_LP3_FRAME_T frame, int cbErr);
/**
*
* Open the DMM port.  Save the oldtios structure in the calling program to pass to dmm_close (see below)
*
*/
int dmm_open(const char *device, struct termios *oldtio);

/**
*
* Close the DMM.  Pass the oldtios structure saved after the call to dmm_open above.
*
*/
void dmm_close(int fd, struct termios *oldtio);

/**
*
* The DMM read thread.  The arg only contains the file-descriptor returned by dmm_open.
*
*/
void *_dmm_read_thread(void *arg);

/**
*
*/
int dmm_default_callback(int n, FS9721_LP3_FRAME_T frame, int cbError);

/**
**
* Call to set callback function.  See prototype above.
*
*/
void dmm_set_callback(DMM_CALLBACK_T *cb);

/**
*
* Pass frame returned to callback to decode data to a floating point number which is the measurement taken by
* the meter.
*
*/
int dmm_decode_measurement(FS9721_LP3_FRAME_T frame, float *measurement);

/**
*
* Start the read thread
*
*/
int dmm_start_read_thread(pthread_t *t, int *fd);

/**
*
* Cancel the read thread
*
*/
int dmm_cancel_read_thread(pthread_t t);

#endif





