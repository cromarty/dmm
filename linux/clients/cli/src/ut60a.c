#include "config.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <uni_t_dmm/fs9721lp3.h>
#include <uni_t_dmm/uni_t_dmm.h>


int ut60a_measurement_string(
    FS9721_LP3_FRAME_T frame,
    float measurement
    ) {

        switch (frame.mode & 0x0c) {
    case AC_FLAG:
        printf("AC\n");
        break;

    case DC_FLAG:
        printf("DC\n");
        break;

    case AUTO_FLAG:
        printf("Auto\n");
        break;

    case RS232_FLAG:
        // do nothing
        break;

    default:
        break;
    }

    switch (frame.flag[0] & 0xf) {
    case MICRO_FLAG:
        printf("Micro\n");
        break;

    case NANO_FLAG:
        printf("Nano\n");
        break;

   case KILO_FLAG:
        printf("Kilo\n");
        break;

    case DIODE_FLAG:
        printf("Diode\n");
        break;

    default:
        break;
    }

    switch (frame.flag[1] & 0xf) {
    case MILLI_FLAG:
        printf("Milli\n");
        break;

    case PERCENT_FLAG:
        printf("Percent\n");
        break;

    case MEGA_FLAG:
        printf("Mega\n");
        break;

    case BEEP_FLAG:
        printf("Continuity\n");
        break;
    default:
        break;
    }
    switch (frame.flag[2] & 0xf) {
    case FARADS_FLAG:
        printf("Farads\n");
        break;

    case OHMS_FLAG:
        printf("Ohms\n");
        break;

    case RELATIVE_FLAG:
        printf("Relative\n");
        break;

case HOLD_FLAG:
        printf("Hold\n");
        break;

    default:
        break;
    }
    switch (frame.flag[3] & 0xf) {
    case AMPS_FLAG:
        printf("Amps\n");
        break;

    case VOLTS_FLAG:
        printf("Volts\n");
        break;

    case HERTZ_FLAG:
        printf("Hertz\n");
        break;

    case LOW_BATTERY_FLAG:
        printf("Low battery\n");
        break;

    default:
        break;
    }

/*
    switch (frame.user & 0xf) {
    case USER_BIT3_FLAG:
        printf("User 1\n");
        break;

    case USER_BIT2_FLAG:
        printf("User 2\n");
        break;

    case USER_BIT1_FLAG:
        printf("User 3\n");
        break;

    case USER_BIT0_FLAG:
        printf("User 4\n");
        break;

    default:
        break;
    }
*/






    return 0;
}

int ut60a_callback(int n, FS9721_LP3_FRAME_T frame, int cbError)
{
	float meas;
	char func[32];

	dmm_decode_measurement(frame, &meas);
	ut60a_measurement_string(frame, meas);
	//dmm_dump_raw_frame(frame);
	//dmm_dump_frame_analysis(frame);

	return 0;
} // ut60a_callback



int main(int argc, char **argv)
{
	struct termios oldtio;
	struct termios newtio;
	int dev;
	int res;
	pthread_t t;

	dmm_set_callback(ut60a_callback);

	dev = dmm_open("/dev/ut60a", &oldtio);
	printf("Result of open: %d\n", dev);

	res = dmm_start_read_thread(&t, &dev);
	sleep(10);
	res = dmm_cancel_read_thread(t);


	dmm_close(dev, &oldtio);

	return 0;
}

